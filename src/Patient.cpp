#include "Patient.h"
#include "Utils.h"

#include <iostream>
#include <iomanip>

Patient::Patient(int id, std::string name, int age, std::string gender,
                  std::string contact, std::string diagnosis, std::string admissionDate)
    : id_(id), name_(std::move(name)), age_(age), gender_(std::move(gender)),
      contact_(std::move(contact)), diagnosis_(std::move(diagnosis)),
      admissionDate_(std::move(admissionDate)) {}

std::string Patient::serialize() const {
    return Utils::join({
        std::to_string(id_), name_, std::to_string(age_), gender_,
        contact_, diagnosis_, admissionDate_
    }, '|');
}

Patient Patient::deserialize(const std::string& line) {
    std::vector<std::string> f = Utils::split(line, '|');
    f.resize(7); // guard against short/corrupted lines
    Patient p;
    p.id_ = f[0].empty() ? 0 : std::stoi(f[0]);
    p.name_ = f[1];
    p.age_ = f[2].empty() ? 0 : std::stoi(f[2]);
    p.gender_ = f[3];
    p.contact_ = f[4];
    p.diagnosis_ = f[5];
    p.admissionDate_ = f[6];
    return p;
}

void Patient::display() const {
    std::cout << "----------------------------------------\n";
    std::cout << "Patient ID   : " << id_ << "\n";
    std::cout << "Name         : " << name_ << "\n";
    std::cout << "Age          : " << age_ << "\n";
    std::cout << "Gender       : " << gender_ << "\n";
    std::cout << "Contact      : " << contact_ << "\n";
    std::cout << "Diagnosis    : " << diagnosis_ << "\n";
    std::cout << "Admitted On  : " << admissionDate_ << "\n";
    std::cout << "----------------------------------------\n";
}
