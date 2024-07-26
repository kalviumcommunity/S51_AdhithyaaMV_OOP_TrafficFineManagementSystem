#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Violation {
public:
    string description;

    Violation(const string& desc)
        : description(desc) {}
};

class Fine {
public:
    double amount;
    Violation violation;

    Fine(double amt, const Violation& viol)
        : amount(amt), violation(viol) {}
};

class Person {
public:
    string name;
    string license_number;
    vector<Fine> fines;

    Person(const string& n, const string& license)
        : name(n), license_number(license) {}

    void addFine(const Fine& fine) {
        fines.push_back(fine);
    }

    double getTotalFines() const {
        double total = 0;
        for (const auto& fine : fines) {
            total += fine.amount;
        }
        return total;
    }
};

class Vehicle {
public:
    string registration_number;
    Person& owner;
    vector<Violation> violations;

    Vehicle(const string& reg_num, Person& own)
        : registration_number(reg_num), owner(own) {}

    void addViolation(const Violation& violation, double fine_amount) {
        violations.push_back(violation);
        owner.addFine(Fine(fine_amount, violation));
    }
};

int main() {
    string personName, licenseNumber, registrationNumber, violationDescription;
    double fineAmount;

    
    getline(cin, personName);
    
    getline(cin, licenseNumber);
    Person person(personName, licenseNumber);

    
    getline(cin, registrationNumber);
    Vehicle vehicle(registrationNumber, person);

    
    getline(cin, violationDescription);
    
    cin >> fineAmount;
    cin.ignore();

    Violation violation(violationDescription);
    vehicle.addViolation(violation, fineAmount);

    cout << "Total fines for " << person.name << ": " << person.getTotalFines() << endl;

    return 0;
}
