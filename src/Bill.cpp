#include "Bill.h"
#include "Utils.h"

#include <iostream>
#include <iomanip>

Bill::Bill(int id, int patientId, std::string date)
    : id_(id), patientId_(patientId), date_(std::move(date)) {}

double Bill::total() const {
    double sum = 0.0;
    for (const auto& item : items_) sum += item.amount;
    return sum;
}

std::string Bill::serialize() const {
    std::vector<std::string> itemStrings;
    for (const auto& item : items_) {
        itemStrings.push_back(item.description + ":" + std::to_string(item.amount));
    }
    std::string itemsField = Utils::join(itemStrings, ';');

    return Utils::join({
        std::to_string(id_), std::to_string(patientId_), date_,
        paid_ ? "1" : "0", itemsField
    }, '|');
}

Bill Bill::deserialize(const std::string& line) {
    std::vector<std::string> f = Utils::split(line, '|');
    f.resize(5);
    Bill b;
    b.id_ = f[0].empty() ? 0 : std::stoi(f[0]);
    b.patientId_ = f[1].empty() ? 0 : std::stoi(f[1]);
    b.date_ = f[2];
    b.paid_ = (f[3] == "1");

    if (!f[4].empty()) {
        for (const auto& itemStr : Utils::split(f[4], ';')) {
            auto pos = itemStr.rfind(':');
            if (pos == std::string::npos) continue;
            BillItem item;
            item.description = itemStr.substr(0, pos);
            try {
                item.amount = std::stod(itemStr.substr(pos + 1));
            } catch (...) {
                item.amount = 0.0;
            }
            b.items_.push_back(item);
        }
    }
    return b;
}

void Bill::display() const {
    std::cout << "----------------------------------------\n";
    std::cout << "Bill ID     : " << id_ << "\n";
    std::cout << "Patient ID  : " << patientId_ << "\n";
    std::cout << "Date        : " << date_ << "\n";
    std::cout << "Status      : " << (paid_ ? "Paid" : "Unpaid") << "\n";
    std::cout << "Items:\n";
    std::cout << std::fixed << std::setprecision(2);
    for (const auto& item : items_) {
        std::cout << "  - " << std::left << std::setw(30) << item.description
                   << std::right << std::setw(10) << item.amount << "\n";
    }
    std::cout << "----------------------------------------\n";
    std::cout << "Total       : " << total() << "\n";
    std::cout << "----------------------------------------\n";
}
