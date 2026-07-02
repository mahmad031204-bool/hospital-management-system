#include "HospitalManagementSystem.h"
#include "Utils.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <climits>
#include <sys/stat.h>

HospitalManagementSystem::HospitalManagementSystem(std::string dataDir)
    : dataDir_(std::move(dataDir)) {}

// ---------------------------------------------------------------------
// Persistence
// ---------------------------------------------------------------------

void HospitalManagementSystem::ensureDataDir() const {
#if defined(_WIN32)
    std::string cmd = "mkdir \"" + dataDir_ + "\" 2> nul";
    system(cmd.c_str());
#else
    mkdir(dataDir_.c_str(), 0755);
#endif
}

template <typename T>
std::vector<T> HospitalManagementSystem::loadEntities(const std::string& filename) const {
    std::vector<T> result;
    std::ifstream file(dataDir_ + "/" + filename);
    if (!file.is_open()) return result;

    std::string line;
    while (std::getline(file, line)) {
        if (Utils::trim(line).empty()) continue;
        result.push_back(T::deserialize(line));
    }
    return result;
}

template <typename T>
void HospitalManagementSystem::saveEntities(const std::string& filename, const std::vector<T>& entities) const {
    std::ofstream file(dataDir_ + "/" + filename, std::ios::trunc);
    if (!file.is_open()) {
        std::cout << "Warning: could not write to " << filename << ".\n";
        return;
    }
    for (const auto& entity : entities) {
        file << entity.serialize() << "\n";
    }
}

void HospitalManagementSystem::loadAll() {
    patients_ = loadEntities<Patient>("patients.dat");
    doctors_ = loadEntities<Doctor>("doctors.dat");
    appointments_ = loadEntities<Appointment>("appointments.dat");
    bills_ = loadEntities<Bill>("bills.dat");

    for (const auto& p : patients_) nextPatientId_ = std::max(nextPatientId_, p.getId() + 1);
    for (const auto& d : doctors_) nextDoctorId_ = std::max(nextDoctorId_, d.getId() + 1);
    for (const auto& a : appointments_) nextAppointmentId_ = std::max(nextAppointmentId_, a.getId() + 1);
    for (const auto& b : bills_) nextBillId_ = std::max(nextBillId_, b.getId() + 1);
}

void HospitalManagementSystem::saveAll() const {
    ensureDataDir();
    saveEntities("patients.dat", patients_);
    saveEntities("doctors.dat", doctors_);
    saveEntities("appointments.dat", appointments_);
    saveEntities("bills.dat", bills_);
}

// ---------------------------------------------------------------------
// Lookups
// ---------------------------------------------------------------------

Patient* HospitalManagementSystem::findPatientById(int id) {
    for (auto& p : patients_) if (p.getId() == id) return &p;
    return nullptr;
}

Patient* HospitalManagementSystem::findPatientByExactName(const std::string& name) {
    std::string target = Utils::toUpper(name);
    for (auto& p : patients_) if (Utils::toUpper(p.getName()) == target) return &p;
    return nullptr;
}

Doctor* HospitalManagementSystem::findDoctorById(int id) {
    for (auto& d : doctors_) if (d.getId() == id) return &d;
    return nullptr;
}

std::vector<Doctor*> HospitalManagementSystem::findDoctorsBySpecialization(const std::string& specialization) {
    std::vector<Doctor*> result;
    for (auto& d : doctors_) {
        if (Utils::toUpper(d.getSpecialization()) == Utils::toUpper(specialization)) {
            result.push_back(&d);
        }
    }
    return result;
}

// ---------------------------------------------------------------------
// Menu actions
// ---------------------------------------------------------------------

void HospitalManagementSystem::registerPatient() {
    std::cout << "\n--- Register Patient ---\n";
    std::string name = Utils::sanitize(Utils::readNonEmptyLine("Enter patient name: "));
    int age = Utils::readInt("Enter patient age: ", 0, 130);
    std::string gender = Utils::sanitize(Utils::readNonEmptyLine("Enter patient gender: "));
    std::string contact = Utils::sanitize(Utils::readLine("Enter contact number (optional): "));
    std::string diagnosis = Utils::sanitize(Utils::readNonEmptyLine("Enter diagnosis: "));

    Patient patient(nextPatientId_++, name, age, gender, contact, diagnosis, Utils::currentDate());
    patients_.push_back(patient);
    saveAll();

    std::cout << "\nPatient registered successfully! Assigned Patient ID: " << patient.getId() << "\n";
}

void HospitalManagementSystem::addDoctor() {
    std::cout << "\n--- Add Doctor ---\n";
    std::string name = Utils::sanitize(Utils::readNonEmptyLine("Enter doctor name: "));
    std::string specialization = Utils::sanitize(Utils::readNonEmptyLine("Enter doctor specialization: "));
    std::string contact = Utils::sanitize(Utils::readLine("Enter contact number (optional): "));

    Doctor doctor(nextDoctorId_++, name, specialization, contact);
    doctors_.push_back(doctor);
    saveAll();

    std::cout << "\nDoctor added successfully! Assigned Doctor ID: " << doctor.getId() << "\n";
}

void HospitalManagementSystem::scheduleAppointment() {
    std::cout << "\n--- Schedule Appointment ---\n";
    if (patients_.empty()) {
        std::cout << "No patients registered yet.\n";
        return;
    }
    int patientId = Utils::readInt("Enter patient ID: ", 1, INT_MAX);
    Patient* patient = findPatientById(patientId);
    if (!patient) {
        std::cout << "Patient not found.\n";
        return;
    }

    std::vector<Doctor*> matches = findDoctorsBySpecialization(patient->getDiagnosis());
    Doctor* chosenDoctor = nullptr;

    if (!matches.empty()) {
        std::cout << "Doctors matching diagnosis \"" << patient->getDiagnosis() << "\":\n";
        for (auto* d : matches) d->display();
        int doctorId = Utils::readInt("Enter the Doctor ID to book (0 to choose manually): ", 0, INT_MAX);
        if (doctorId != 0) chosenDoctor = findDoctorById(doctorId);
    } else {
        std::cout << "No doctor's specialization automatically matches \""
                   << patient->getDiagnosis() << "\". Please choose a doctor manually.\n";
    }

    if (!chosenDoctor) {
        if (doctors_.empty()) {
            std::cout << "No doctors available.\n";
            return;
        }
        std::cout << "Available doctors:\n";
        for (const auto& d : doctors_) d.display();
        int doctorId = Utils::readInt("Enter Doctor ID: ", 1, INT_MAX);
        chosenDoctor = findDoctorById(doctorId);
        if (!chosenDoctor) {
            std::cout << "Doctor not found.\n";
            return;
        }
    }

    std::string date = Utils::sanitize(Utils::readNonEmptyLine("Enter appointment date (YYYY-MM-DD): "));
    std::string reason = Utils::sanitize(Utils::readLine("Enter reason/notes (optional): "));

    Appointment appt(nextAppointmentId_++, patient->getId(), chosenDoctor->getId(), date, reason);
    appointments_.push_back(appt);
    saveAll();

    std::cout << "\nAppointment scheduled: " << patient->getName() << " with Dr. "
               << chosenDoctor->getName() << " on " << date
               << " (Appointment ID: " << appt.getId() << ")\n";
}

void HospitalManagementSystem::viewPatientInformation() const {
    std::cout << "\n--- View Patient Information ---\n";
    if (patients_.empty()) {
        std::cout << "No patients registered yet.\n";
        return;
    }
    int id = Utils::readInt("Enter patient ID: ", 1, INT_MAX);
    for (const auto& p : patients_) {
        if (p.getId() == id) {
            p.display();
            return;
        }
    }
    std::cout << "Patient not found.\n";
}

void HospitalManagementSystem::viewDoctorInformation() const {
    std::cout << "\n--- View Doctor Information ---\n";
    if (doctors_.empty()) {
        std::cout << "No doctors registered yet.\n";
        return;
    }
    int id = Utils::readInt("Enter doctor ID: ", 1, INT_MAX);
    for (const auto& d : doctors_) {
        if (d.getId() == id) {
            d.display();
            return;
        }
    }
    std::cout << "Doctor not found.\n";
}

void HospitalManagementSystem::searchPatients() const {
    std::cout << "\n--- Search Patients by Name ---\n";
    if (patients_.empty()) {
        std::cout << "No patients registered yet.\n";
        return;
    }
    std::string query = Utils::readNonEmptyLine("Enter name (or part of it) to search: ");
    bool found = false;
    for (const auto& p : patients_) {
        if (Utils::containsIgnoreCase(p.getName(), query)) {
            p.display();
            found = true;
        }
    }
    if (!found) std::cout << "No matching patients found.\n";
}

void HospitalManagementSystem::updatePatientDiagnosis() {
    std::cout << "\n--- Update Patient Diagnosis ---\n";
    if (patients_.empty()) {
        std::cout << "No patients registered yet.\n";
        return;
    }
    int id = Utils::readInt("Enter patient ID: ", 1, INT_MAX);
    Patient* p = findPatientById(id);
    if (!p) {
        std::cout << "Patient not found.\n";
        return;
    }
    std::cout << "Current diagnosis: " << p->getDiagnosis() << "\n";
    std::string newDiagnosis = Utils::sanitize(Utils::readNonEmptyLine("Enter new diagnosis: "));
    p->setDiagnosis(newDiagnosis);
    saveAll();
    std::cout << "Diagnosis updated.\n";
}

void HospitalManagementSystem::listAppointments() const {
    std::cout << "\n--- All Appointments ---\n";
    if (appointments_.empty()) {
        std::cout << "No appointments scheduled yet.\n";
        return;
    }
    for (const auto& a : appointments_) {
        a.display();
    }
}

void HospitalManagementSystem::generateBill() {
    std::cout << "\n--- Generate Bill ---\n";
    if (patients_.empty()) {
        std::cout << "No patients registered yet.\n";
        return;
    }
    int patientId = Utils::readInt("Enter patient ID: ", 1, INT_MAX);
    Patient* patient = findPatientById(patientId);
    if (!patient) {
        std::cout << "Patient not found.\n";
        return;
    }

    Bill bill(nextBillId_++, patient->getId(), Utils::currentDate());

    bool addingItems = true;
    while (addingItems) {
        std::string description = Utils::sanitize(Utils::readNonEmptyLine("Enter charge description (e.g. Consultation): "));
        double amount = Utils::readDouble("Enter amount: ", 0.0, 1e9);
        bill.addItem(description, amount);
        addingItems = Utils::readYesNo("Add another item? (y/n): ");
    }

    bills_.push_back(bill);
    saveAll();

    std::cout << "\nBill generated for " << patient->getName() << " (Bill ID: " << bill.getId() << ")\n";
    bill.display();
}

void HospitalManagementSystem::viewBill() const {
    std::cout << "\n--- View Bill ---\n";
    if (bills_.empty()) {
        std::cout << "No bills generated yet.\n";
        return;
    }
    int id = Utils::readInt("Enter bill ID: ", 1, INT_MAX);
    for (const auto& b : bills_) {
        if (b.getId() == id) {
            b.display();
            return;
        }
    }
    std::cout << "Bill not found.\n";
}

void HospitalManagementSystem::displayReport() const {
    std::cout << "\n================= Hospital Report =================\n";
    std::cout << "Total Patients      : " << patients_.size() << "\n";
    std::cout << "Total Doctors       : " << doctors_.size() << "\n";
    std::cout << "Total Appointments  : " << appointments_.size() << "\n";
    std::cout << "Total Bills         : " << bills_.size() << "\n";

    double revenue = 0.0;
    for (const auto& b : bills_) revenue += b.total();
    std::cout << "Total Billed Amount : " << revenue << "\n";
    std::cout << "-----------------------------------------------------\n";

    std::cout << "\nPatients on file:\n";
    for (const auto& p : patients_) {
        std::cout << "  #" << p.getId() << " " << p.getName()
                   << " (" << p.getAge() << ", " << p.getGender()
                   << ") - " << p.getDiagnosis() << "\n";
    }

    std::cout << "\nUpcoming/Recorded Appointments:\n";
    for (const auto& a : appointments_) {
        const Patient* p = nullptr;
        const Doctor* d = nullptr;
        for (const auto& pp : patients_) if (pp.getId() == a.getPatientId()) p = &pp;
        for (const auto& dd : doctors_) if (dd.getId() == a.getDoctorId()) d = &dd;

        std::cout << "  #" << a.getId() << " "
                   << (p ? p->getName() : "Unknown Patient") << " with Dr. "
                   << (d ? d->getName() : "Unknown") << " on " << a.getDate()
                   << " [" << toString(a.getStatus()) << "]\n";
    }
    std::cout << "=====================================================\n";
}

// ---------------------------------------------------------------------
// Menu loop
// ---------------------------------------------------------------------

void HospitalManagementSystem::showMenu() const {
    std::cout << "\n===================================================\n";
    std::cout << "          HOSPITAL MANAGEMENT SYSTEM\n";
    std::cout << "===================================================\n";
    std::cout << " 1. Register Patient\n";
    std::cout << " 2. Add Doctor\n";
    std::cout << " 3. Schedule Appointment\n";
    std::cout << " 4. View Patient Information\n";
    std::cout << " 5. View Doctor Information\n";
    std::cout << " 6. Search Patients by Name\n";
    std::cout << " 7. Update Patient Diagnosis\n";
    std::cout << " 8. List All Appointments\n";
    std::cout << " 9. Generate Bill\n";
    std::cout << "10. View Bill\n";
    std::cout << "11. Display Hospital Report\n";
    std::cout << " 0. Exit\n";
    std::cout << "===================================================\n";
}

void HospitalManagementSystem::run() {
    loadAll();

    int choice;
    do {
        showMenu();
        choice = Utils::readInt("Enter your choice: ", 0, 11);

        switch (choice) {
            case 1: registerPatient(); break;
            case 2: addDoctor(); break;
            case 3: scheduleAppointment(); break;
            case 4: viewPatientInformation(); break;
            case 5: viewDoctorInformation(); break;
            case 6: searchPatients(); break;
            case 7: updatePatientDiagnosis(); break;
            case 8: listAppointments(); break;
            case 9: generateBill(); break;
            case 10: viewBill(); break;
            case 11: displayReport(); break;
            case 0: std::cout << "\nAll data saved. Goodbye!\n"; break;
        }

        if (choice != 0) Utils::pause();
    } while (choice != 0);
}
