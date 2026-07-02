#pragma once

#include <string>

enum class AppointmentStatus { Scheduled, Completed, Cancelled };

std::string toString(AppointmentStatus status);
AppointmentStatus appointmentStatusFromString(const std::string& s);

// Represents an appointment linking a patient with a doctor.
class Appointment {
public:
    Appointment() = default;
    Appointment(int id, int patientId, int doctorId, std::string date,
                std::string reason, AppointmentStatus status = AppointmentStatus::Scheduled);

    int getId() const { return id_; }
    int getPatientId() const { return patientId_; }
    int getDoctorId() const { return doctorId_; }
    const std::string& getDate() const { return date_; }
    const std::string& getReason() const { return reason_; }
    AppointmentStatus getStatus() const { return status_; }

    void setStatus(AppointmentStatus status) { status_ = status; }

    std::string serialize() const;
    static Appointment deserialize(const std::string& line);

    void display() const;

private:
    int id_ = 0;
    int patientId_ = 0;
    int doctorId_ = 0;
    std::string date_;
    std::string reason_;
    AppointmentStatus status_ = AppointmentStatus::Scheduled;
};
