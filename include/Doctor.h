#pragma once

#include <string>

// Represents a single doctor record.
class Doctor {
public:
    Doctor() = default;
    Doctor(int id, std::string name, std::string specialization, std::string contact);

    int getId() const { return id_; }
    const std::string& getName() const { return name_; }
    const std::string& getSpecialization() const { return specialization_; }
    const std::string& getContact() const { return contact_; }

    std::string serialize() const;
    static Doctor deserialize(const std::string& line);

    void display() const;

private:
    int id_ = 0;
    std::string name_;
    std::string specialization_;
    std::string contact_;
};
