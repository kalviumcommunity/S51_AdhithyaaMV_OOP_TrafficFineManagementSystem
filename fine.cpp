#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Violation {
private:
    string description;
    static int totalViolations;

public:
    Violation(const string& desc) : description(desc) {
        totalViolations++;
    }

    string getDescription() const {
        return description;
    }

    static int getTotalViolations() {
        return totalViolations;
    }
};

int Violation::totalViolations = 0;

class Fine {
protected:
    double amount;
    Violation* violation;
    static double totalFinesAmount;

public:
    Fine(double amt, Violation* viol) : amount(amt), violation(viol) {
        totalFinesAmount += amt;
    }

    Fine(double amt) : amount(amt), violation(nullptr) {
        totalFinesAmount += amt;
    }

    virtual ~Fine() = default;

    double getAmount() const {
        return amount;
    }

    void setAmount(double amt) {
        totalFinesAmount -= amount;
        amount = amt;
        totalFinesAmount += amt;
    }

    static double getTotalFinesAmount() {
        return totalFinesAmount;
    }

    virtual void applyFinePolicy() const = 0; 
};

double Fine::totalFinesAmount = 0;

class HeavyFine : public Fine {
public:
    HeavyFine(double amt, Violation* viol) : Fine(amt * 2, viol) {}

    void applyFinePolicy() const override {
        cout << "Applying heavy fine policy with double the amount: " << amount << endl;
    }
};

class Entity {
protected:
    string name;

public:
    Entity(const string& name) : name(name) {}
    virtual ~Entity() = default;

    string getName() const {
        return name;
    }
};

class Person : public Entity {
private:
    string license_number;
    vector<Fine*> fines;

public:
    Person(const string& name, const string& license)
        : Entity(name), license_number(license) {}

    ~Person() {
        for (Fine* fine : fines) {
            delete fine;
        }
    }

    string getLicenseNumber() const {
        return license_number;
    }

    void addFine(Fine* fine) {
        fines.push_back(fine);
    }

    double getTotalFines() const {
        double total = 0;
        for (const auto& fine : fines) {
            total += fine->getAmount();
        }
        return total;
    }

    void applyFinePolicies() const {
        for (const auto& fine : fines) {
            fine->applyFinePolicy();
        }
    }
};

class Vehicle {
private:
    string registration_number;
    Person* owner;
    vector<Violation*> violations;

public:
    Vehicle(const string& reg_num, Person* own)
        : registration_number(reg_num), owner(own) {}

    ~Vehicle() {
        for (Violation* violation : violations) {
            delete violation;
        }
    }

    void addViolation(Violation* violation, double fine_amount, bool heavy = false) {
        violations.push_back(violation);
        if (heavy) {
            owner->addFine(new HeavyFine(fine_amount, violation));
        } else {
            owner->addFine(new Fine(fine_amount, violation));
        }
    }
};

int main() {
    int numPeople, numVehicles, numViolations;
    string personName, licenseNumber, registrationNumber, violationDescription;
    double fineAmount;
    bool isHeavy;

    cout << "Enter number of people: ";
    cin >> numPeople;
    cin.ignore();

    vector<Person*> people;

    for (int i = 0; i < numPeople; ++i) {
        cout << "Enter name of person " << (i + 1) << ": ";
        getline(cin, personName);

        cout << "Enter license number of person " << (i + 1) << ": ";
        getline(cin, licenseNumber);

        Person* person = new Person(personName, licenseNumber);
        people.push_back(person);
    }

    for (int i = 0; i < numPeople; ++i) {
        Person* person = people[i];

        cout << "Enter number of vehicles for " << person->getName() << ": ";
        cin >> numVehicles;
        cin.ignore();

        vector<Vehicle*> vehicles;

        for (int j = 0; j < numVehicles; ++j) {
            cout << "Enter registration number of vehicle " << (j + 1) << ": ";
            getline(cin, registrationNumber);

            Vehicle* vehicle = new Vehicle(registrationNumber, person);
            vehicles.push_back(vehicle);

            cout << "Enter number of violations for vehicle " << (j + 1) << ": ";
            cin >> numViolations;
            cin.ignore();

            for (int k = 0; k < numViolations; ++k) {
                cout << "Enter description of violation " << (k + 1) << ": ";
                getline(cin, violationDescription);

                cout << "Enter fine amount for violation " << (k + 1) << ": ";
                cin >> fineAmount;
                cin.ignore();

                cout << "Is it a heavy violation (1 for yes, 0 for no)? ";
                cin >> isHeavy;
                cin.ignore();

                Violation* violation = new Violation(violationDescription);
                vehicle->addViolation(violation, fineAmount, isHeavy);
            }
        }

        for (Vehicle* vehicle : vehicles) {
            delete vehicle;
        }
    }

    cout << "Total number of violations recorded: " << Violation::getTotalViolations() << endl;
    cout << "Total amount of fines issued: " << Fine::getTotalFinesAmount() << endl;

    for (Person* person : people) {
        cout << "Total fines for " << person->getName() << ": " << person->getTotalFines() << endl;
        person->applyFinePolicies();
        delete person;
    }

    return 0;
}
