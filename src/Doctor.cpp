#include "Doctor.h"
#include "Utils.h"

#include <iostream>

Doctor::Doctor(int id, std::string name, std::string specialization, std::string contact)
    : id_(id), name_(std::move(name)), specialization_(std::move(specialization)),
      contact_(std::move(contact)) {}

std::string Doctor::serialize() const {
    return Utils::join({std::to_string(id_), name_, specialization_, contact_}, '|');
}

Doctor Doctor::deserialize(const std::string& line) {
    std::vector<std::string> f = Utils::split(line, '|');
    f.resize(4);
    Doctor d;
    d.id_ = f[0].empty() ? 0 : std::stoi(f[0]);
    d.name_ = f[1];
    d.specialization_ = f[2];
    d.contact_ = f[3];
    return d;
}

void Doctor::display() const {
    std::cout << "----------------------------------------\n";
    std::cout << "Doctor ID       : " << id_ << "\n";
    std::cout << "Name            : Dr. " << name_ << "\n";
    std::cout << "Specialization  : " << specialization_ << "\n";
    std::cout << "Contact         : " << contact_ << "\n";
    std::cout << "----------------------------------------\n";
}
