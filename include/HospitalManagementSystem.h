#pragma once

#include <string>
#include <vector>

#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"

// Owns all in-memory data, handles persistence to disk, and drives the
// console menu loop for the Hospital Management System.
class HospitalManagementSystem {
public:
    explicit HospitalManagementSystem(std::string dataDir = "data");

    // Loads existing data (if any) and runs the interactive menu until the
    // user chooses to exit.
    void run();

private:
    std::string dataDir_;

    std::vector<Patient> patients_;
    std::vector<Doctor> doctors_;
    std::vector<Appointment> appointments_;
    std::vector<Bill> bills_;

    int nextPatientId_ = 1;
    int nextDoctorId_ = 1;
    int nextAppointmentId_ = 1;
    int nextBillId_ = 1;

    // --- Persistence -------------------------------------------------
    void loadAll();
    void saveAll() const;
    void ensureDataDir() const;

    template <typename T>
    std::vector<T> loadEntities(const std::string& filename) const;

    template <typename T>
    void saveEntities(const std::string& filename, const std::vector<T>& entities) const;

    // --- Menu ----------------------------------------------------------
    void showMenu() const;

    void registerPatient();
    void addDoctor();
    void scheduleAppointment();
    void viewPatientInformation() const;
    void viewDoctorInformation() const;
    void searchPatients() const;
    void updatePatientDiagnosis();
    void listAppointments() const;
    void generateBill();
    void viewBill() const;
    void displayReport() const;

    // --- Lookups ---------------------------------------------------------
    Patient* findPatientById(int id);
    Patient* findPatientByExactName(const std::string& name);
    Doctor* findDoctorById(int id);
    std::vector<Doctor*> findDoctorsBySpecialization(const std::string& specialization);
};
