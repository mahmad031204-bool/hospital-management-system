# Hospital Management System (C++)

A console-based Hospital Management System written in modern C++17. Originally built as a first-semester CP (Computer Programming) project, this version has been refactored into a proper object-oriented, multi-file application with persistent storage, input validation, and a cleaner menu-driven workflow.

## Features

- **Patient management** — register patients, view details by ID, search by (partial) name, and update diagnoses.
- **Doctor management** — add doctors with a specialization, view details by ID.
- **Smart appointment scheduling** — automatically suggests doctors whose specialization matches the patient's diagnosis, with a manual fallback if no match exists.
- **Itemized billing** — generate multi-line bills per patient and compute totals.
- **Hospital-wide reporting** — a summary dashboard of patients, doctors, appointments, and revenue.
- **Persistent storage** — all data is saved to plain-text files under `data/` and reloaded automatically the next time the program starts.
- **Input validation** — numeric prompts reject non-numeric or out-of-range input instead of crashing or looping forever.

## What changed from the original version

The original single-file project used parallel arrays (`patientNames[]`, `patientAges[]`, ...), had no unique record IDs, could not update existing records, and had no way to reload data that was previously saved. This version:

- Introduces real classes (`Patient`, `Doctor`, `Appointment`, `Bill`) instead of parallel arrays.
- Uses `std::vector` instead of fixed-size arrays, so there's no artificial 100-record cap.
- Assigns a unique, auto-incrementing ID to every patient, doctor, appointment, and bill.
- Splits the code into logical headers/sources (`include/`, `src/`) instead of one flat file.
- Loads previously saved data back into memory on startup, instead of only appending to a text file.
- Adds appointment status tracking, itemized bills with running totals, name search, and a full reporting screen.
- Replaces unchecked `cin >>` calls with validated input helpers that handle bad input gracefully.

## Project structure

```
hospital-management-system/
├── include/                       # Header files (class declarations)
│   ├── Appointment.h
│   ├── Bill.h
│   ├── Doctor.h
│   ├── HospitalManagementSystem.h
│   ├── Patient.h
│   └── Utils.h
├── src/                            # Implementation files
│   ├── Appointment.cpp
│   ├── Bill.cpp
│   ├── Doctor.cpp
│   ├── HospitalManagementSystem.cpp
│   ├── Patient.cpp
│   ├── Utils.cpp
│   └── main.cpp
├── data/                           # Generated at runtime; holds .dat files
├── Makefile
├── LICENSE
└── README.md
```

## Building

You need a C++17 compiler (g++ 9+ / clang++ 10+ or later).

### Using the Makefile (Linux/macOS, or WSL/MinGW on Windows)

```bash
make          # builds the ./hms executable
make run      # builds and runs it
make clean    # removes build artifacts
```

### Manual compilation

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o hms
./hms
```

## Usage

Run the executable and follow the on-screen menu:

```
===================================================
          HOSPITAL MANAGEMENT SYSTEM
===================================================
 1. Register Patient
 2. Add Doctor
 3. Schedule Appointment
 4. View Patient Information
 5. View Doctor Information
 6. Search Patients by Name
 7. Update Patient Diagnosis
 8. List All Appointments
 9. Generate Bill
10. View Bill
11. Display Hospital Report
 0. Exit
===================================================
```

Data is written to `data/patients.dat`, `data/doctors.dat`, `data/appointments.dat`, and `data/bills.dat` after every change, so you can close the program and resume later with all records intact.

## Possible future improvements

- Add unit tests (e.g. with Catch2 or GoogleTest) for the model classes and lookup logic.
- Replace the pipe-delimited flat files with SQLite for more robust storage.
- Add authentication/roles (admin, receptionist, doctor).
- Build a simple GUI (Qt) or web front end on top of the same core classes.

## License

This project is licensed under the MIT License — see [LICENSE](LICENSE) for details.
