# Traffic Violation and Fine Management System

## Overview

This C++ program models a system for managing traffic violations and fines. It allows you to record a person's details, a vehicle's details, and any violations and associated fines for that vehicle. The program then calculates and displays the total fines for a person.

## Features

- Input and store a person's name and license number.
- Input and store a vehicle's registration number and associate it with a person.
- Record a traffic violation and the associated fine for the vehicle.
- Calculate and display the total fines for the person.

## Classes

### `Violation`

- **Description**: Represents a traffic violation.
- **Members**:
  - `string description`: The description of the violation.
- **Constructor**:
  - `Violation(const string& desc)`: Initializes the description of the violation.

### `Fine`

- **Description**: Represents a fine associated with a violation.
- **Members**:
  - `double amount`: The amount of the fine.
  - `Violation violation`: The violation that the fine is associated with.
- **Constructor**:
  - `Fine(double amt, const Violation& viol)`: Initializes the amount and the associated violation.

### `Person`

- **Description**: Represents a person with a name and license number, and stores their fines.
- **Members**:
  - `string name`: The person's name.
  - `string license_number`: The person's license number.
  - `vector<Fine> fines`: A list of fines associated with the person.
- **Constructor**:
  - `Person(const string& n, const string& license)`: Initializes the name and license number.
- **Methods**:
  - `void addFine(const Fine& fine)`: Adds a fine to the person's list of fines.
  - `double getTotalFines() const`: Calculates and returns the total amount of fines.

### `Vehicle`

- **Description**: Represents a vehicle with a registration number and its associated violations.
- **Members**:
  - `string registration_number`: The vehicle's registration number.
  - `Person& owner`: A reference to the person who owns the vehicle.
  - `vector<Violation> violations`: A list of violations associated with the vehicle.
- **Constructor**:
  - `Vehicle(const string& reg_num, Person& own)`: Initializes the registration number and owner.
- **Methods**:
  - `void addViolation(const Violation& violation, double fine_amount)`: Adds a violati
