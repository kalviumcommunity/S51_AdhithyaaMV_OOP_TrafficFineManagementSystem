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
        this->fines.push_back(fine);
    }

    double getTotalFines() const {
        double total = 0;
        for (const auto& fine : this->fines) {
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
        this->violations.push_back(violation);
        this->owner.addFine(Fine(fine_amount, violation));
    }
};

int main() {
    int numPeople, numVehicles, numViolations;
    string personName, licenseNumber, registrationNumber, violationDescription;
    double fineAmount;

    cout << "Enter number of people: ";
    cin >> numPeople;
    cin.ignore(); 

    vector<Person> people;

    for (int i = 0; i < numPeople; ++i) {
        cout << "Enter name of person " << (i + 1) << ": ";
        getline(cin, personName);

        cout << "Enter license number of person " << (i + 1) << ": ";
        getline(cin, licenseNumber);

        people.push_back(Person(personName, licenseNumber));
    }

    for (int i = 0; i < numPeople; ++i) {
        Person& person = people[i];

        cout << "Enter number of vehicles for " << person.name << ": ";
        cin >> numVehicles;
        cin.ignore(); 

        for (int j = 0; j < numVehicles; ++j) {
            cout << "Enter registration number of vehicle " << (j + 1) << ": ";
            getline(cin, registrationNumber);

            Vehicle vehicle(registrationNumber, person);

            cout << "Enter number of violations for vehicle " << (j + 1) << ": ";
            cin >> numViolations;
            cin.ignore(); 

            for (int k = 0; k < numViolations; ++k) {
                cout << "Enter description of violation " << (k + 1) << ": ";
                getline(cin, violationDescription);

                cout << "Enter fine amount for violation " << (k + 1) << ": ";
                cin >> fineAmount;
                cin.ignore(); 

                Violation violation(violationDescription);
                vehicle.addViolation(violation, fineAmount);
            }
        }
    }

    for (const auto& person : people) {
        cout << "Total fines for " << person.name << ": " << person.getTotalFines() << endl;
    }

    return 0;
}
