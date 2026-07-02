#pragma once

#include <string>

// Represents a single patient record.
class Patient {
public:
    Patient() = default;
    Patient(int id, std::string name, int age, std::string gender,
            std::string contact, std::string diagnosis, std::string admissionDate);

    int getId() const { return id_; }
    const std::string& getName() const { return name_; }
    int getAge() const { return age_; }
    const std::string& getGender() const { return gender_; }
    const std::string& getContact() const { return contact_; }
    const std::string& getDiagnosis() const { return diagnosis_; }
    const std::string& getAdmissionDate() const { return admissionDate_; }

    void setDiagnosis(const std::string& diagnosis) { diagnosis_ = diagnosis; }

    // Serializes the record to a single '|'-delimited line for file storage.
    std::string serialize() const;

    // Rebuilds a Patient from a line produced by serialize().
    static Patient deserialize(const std::string& line);

    // Prints a formatted summary to std::cout.
    void display() const;

private:
    int id_ = 0;
    std::string name_;
    int age_ = 0;
    std::string gender_;
    std::string contact_;
    std::string diagnosis_;
    std::string admissionDate_;
};
