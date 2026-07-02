#pragma once

#include <string>
#include <vector>

struct BillItem {
    std::string description;
    double amount = 0.0;
};

// Represents an itemized bill for a patient.
class Bill {
public:
    Bill() = default;
    Bill(int id, int patientId, std::string date);

    int getId() const { return id_; }
    int getPatientId() const { return patientId_; }
    const std::string& getDate() const { return date_; }
    const std::vector<BillItem>& getItems() const { return items_; }
    bool isPaid() const { return paid_; }

    void addItem(const std::string& description, double amount) { items_.push_back({description, amount}); }
    void markPaid() { paid_ = true; }
    double total() const;

    std::string serialize() const;
    static Bill deserialize(const std::string& line);

    void display() const;

private:
    int id_ = 0;
    int patientId_ = 0;
    std::string date_;
    std::vector<BillItem> items_;
    bool paid_ = false;
};
