#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Violation {
public:
    string description;
    static int totalViolations;

    Violation(const string& desc)
        : description(desc) {
        totalViolations++;
    }

    static int getTotalViolations() {
        return totalViolations;
    }
};

int Violation::totalViolations = 0;

class Fine {
public:
    double amount;
    Violation* violation;
    static double totalFinesAmount;

    Fine(double amt, Violation* viol)
        : amount(amt), violation(viol) {
        totalFinesAmount += amt;
    }

    static double getTotalFinesAmount() {
        return totalFinesAmount;
    }
};

double Fine::totalFinesAmount = 0;

class Person {
public:
    string name;
    string license_number;
    vector<Fine*> fines;

    Person(const string& n, const string& license)
        : name(n), license_number(license) {}

    ~Person() {
        for (Fine* fine : fines) {
            delete fine;
        }
    }

    void addFine(Fine* fine) {
        fines.push_back(fine);
    }

    double getTotalFines() const {
        double total = 0;
        for (const auto& fine : fines) {
            total += fine->amount;
        }
        return total;
    }
};

class Vehicle {
public:
    string registration_number;
    Person* owner;
    vector<Violation*> violations;

    Vehicle(const string& reg_num, Person* own)
        : registration_number(reg_num), owner(own) {}

    ~Vehicle() {
        for (Violation* violation : violations) {
            delete violation;
        }
    }

    void addViolation(Violation* violation, double fine_amount) {
        violations.push_back(violation);
        owner->addFine(new Fine(fine_amount, violation));
    }
};

int main() {
    int numPeople, numVehicles, numViolations;
    string personName, licenseNumber, registrationNumber, violationDescription;
    double fineAmount;

    cout << "Enter number of people: ";
    cin >> numPeople;
    cin.ignore();

    vector<Person*> people;

    for (int i = 0; i < numPeople; ++i) {
        cout << "Enter name of person " << (i + 1) << ": ";
        getline(cin, personName);

        cout << "Enter license number of person " << (i + 1) << ": ";
        getline(cin, licenseNumber);

        people.push_back(new Person(personName, licenseNumber));
    }

    for (int i = 0; i < numPeople; ++i) {
        Person* person = people[i];

        cout << "Enter number of vehicles for " << person->name << ": ";
        cin >> numVehicles;
        cin.ignore();

        for (int j = 0; j < numVehicles; ++j) {
            cout << "Enter registration number of vehicle " << (j + 1) << ": ";
            getline(cin, registrationNumber);

            Vehicle* vehicle = new Vehicle(registrationNumber, person);

            cout << "Enter number of violations for vehicle " << (j + 1) << ": ";
            cin >> numViolations;
            cin.ignore();

            for (int k = 0; k < numViolations; ++k) {
                cout << "Enter description of violation " << (k + 1) << ": ";
                getline(cin, violationDescription);

                cout << "Enter fine amount for violation " << (k + 1) << ": ";
                cin >> fineAmount;
                cin.ignore();

                Violation* violation = new Violation(violationDescription);
                vehicle->addViolation(violation, fineAmount);
            }

            delete vehicle;
        }
    }

    cout << "Total number of violations recorded: " << Violation::getTotalViolations() << endl;
    cout << "Total amount of fines issued: " << Fine::getTotalFinesAmount() << endl;

    for (Person* person : people) {
        cout << "Total fines for " << person->name << ": " << person->getTotalFines() << endl;
        delete person;
    }

    return 0;
}
