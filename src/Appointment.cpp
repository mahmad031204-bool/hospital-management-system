#include "Appointment.h"
#include "Utils.h"

#include <iostream>

std::string toString(AppointmentStatus status) {
    switch (status) {
        case AppointmentStatus::Scheduled: return "Scheduled";
        case AppointmentStatus::Completed: return "Completed";
        case AppointmentStatus::Cancelled: return "Cancelled";
    }
    return "Unknown";
}

AppointmentStatus appointmentStatusFromString(const std::string& s) {
    if (s == "Completed") return AppointmentStatus::Completed;
    if (s == "Cancelled") return AppointmentStatus::Cancelled;
    return AppointmentStatus::Scheduled;
}

Appointment::Appointment(int id, int patientId, int doctorId, std::string date,
                          std::string reason, AppointmentStatus status)
    : id_(id), patientId_(patientId), doctorId_(doctorId), date_(std::move(date)),
      reason_(std::move(reason)), status_(status) {}

std::string Appointment::serialize() const {
    return Utils::join({
        std::to_string(id_), std::to_string(patientId_), std::to_string(doctorId_),
        date_, reason_, toString(status_)
    }, '|');
}

Appointment Appointment::deserialize(const std::string& line) {
    std::vector<std::string> f = Utils::split(line, '|');
    f.resize(6);
    Appointment a;
    a.id_ = f[0].empty() ? 0 : std::stoi(f[0]);
    a.patientId_ = f[1].empty() ? 0 : std::stoi(f[1]);
    a.doctorId_ = f[2].empty() ? 0 : std::stoi(f[2]);
    a.date_ = f[3];
    a.reason_ = f[4];
    a.status_ = appointmentStatusFromString(f[5]);
    return a;
}

void Appointment::display() const {
    std::cout << "----------------------------------------\n";
    std::cout << "Appointment ID : " << id_ << "\n";
    std::cout << "Patient ID     : " << patientId_ << "\n";
    std::cout << "Doctor ID      : " << doctorId_ << "\n";
    std::cout << "Date           : " << date_ << "\n";
    std::cout << "Reason         : " << reason_ << "\n";
    std::cout << "Status         : " << toString(status_) << "\n";
    std::cout << "----------------------------------------\n";
}
