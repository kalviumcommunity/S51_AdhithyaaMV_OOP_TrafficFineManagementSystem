
#include <iostream>
#include <vector>
#include <string>
#include <memory>

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
    shared_ptr<Violation> violation;
    static double totalFinesAmount;

public:
    Fine(double amt, shared_ptr<Violation> viol) : amount(amt), violation(viol) {
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

class StandardFine : public Fine {
public:
    StandardFine(double amt, shared_ptr<Violation> viol) : Fine(amt, viol) {}

    void applyFinePolicy() const override {
        cout << "Applying standard fine policy: " << amount << endl;
    }
};

class HeavyFine : public Fine {
public:
    HeavyFine(double amt, shared_ptr<Violation> viol) : Fine(amt * 2, viol) {}

    void applyFinePolicy() const override {
        cout << "Applying heavy fine policy with double the amount: " << amount << endl;
    }
};

class FineFactory {
public:
    static unique_ptr<Fine> createFine(double amount, shared_ptr<Violation> viol, bool heavy = false) {
        if (heavy) {
            return make_unique<HeavyFine>(amount, viol);
        } else {
            return make_unique<StandardFine>(amount, viol);
        }
    }
};

class FineManager {
private:
    vector<unique_ptr<Fine>> fines;

public:
    void addFine(unique_ptr<Fine> fine) {
        fines.push_back(move(fine));
    }

    double getTotalFinesAmount() const {
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
    FineManager fineManager;

public:
    Person(const string& name, const string& license)
        : Entity(name), license_number(license) {}

    string getLicenseNumber() const {
        return license_number;
    }

    FineManager& getFineManager() {
        return fineManager;
    }
};

class ViolationManager {
private:
    vector<shared_ptr<Violation>> violations;

public:
    shared_ptr<Violation> addViolation(const string& description) {
        auto violation = make_shared<Violation>(description);
        violations.push_back(violation);
        return violation;
    }
};

class Vehicle {
private:
    string registration_number;
    Person* owner;
    ViolationManager violationManager;

public:
    Vehicle(const string& reg_num, Person* own)
        : registration_number(reg_num), owner(own) {}

    void addViolationWithFine(const string& violationDescription, double fineAmount, bool heavy = false) {
        auto violation = violationManager.addViolation(violationDescription);
        auto fine = FineFactory::createFine(fineAmount, violation, heavy);
        owner->getFineManager().addFine(move(fine));
    }
};

int main() {
    int numPeople;
    string personName, licenseNumber;

    cout << "Enter number of people: ";
    cin >> numPeople;
    cin.ignore();

    vector<unique_ptr<Person>> people;

    for (int i = 0; i < numPeople; ++i) {
        cout << "Enter name of person " << (i + 1) << ": ";
        getline(cin, personName);

        cout << "Enter license number of person " << (i + 1) << ": ";
        getline(cin, licenseNumber);

        auto person = make_unique<Person>(personName, licenseNumber);
        people.push_back(move(person));
    }

    for (auto& person : people) {
        int numVehicles;
        cout << "Enter number of vehicles for " << person->getName() << ": ";
        cin >> numVehicles;
        cin.ignore();

        for (int j = 0; j < numVehicles; ++j) {
            string registrationNumber;
            cout << "Enter registration number of vehicle " << (j + 1) << ": ";
            getline(cin, registrationNumber);

            Vehicle vehicle(registrationNumber, person.get());

            int numViolations;
            cout << "Enter number of violations for vehicle " << (j + 1) << ": ";
            cin >> numViolations;
            cin.ignore();

            for (int k = 0; k < numViolations; ++k) {
                string violationDescription;
                double fineAmount;
                bool isHeavy;

                cout << "Enter description of violation " << (k + 1) << ": ";
                getline(cin, violationDescription);

                cout << "Enter fine amount for violation " << (k + 1) << ": ";
                cin >> fineAmount;
                cin.ignore();

                cout << "Is it a heavy violation (1 for yes, 0 for no)? ";
                cin >> isHeavy;
                cin.ignore();

                vehicle.addViolationWithFine(violationDescription, fineAmount, isHeavy);
            }
        }
    }

    cout << "Total number of violations recorded: " << Violation::getTotalViolations() << endl;
    cout << "Total amount of fines issued: " << Fine::getTotalFinesAmount() << endl;

    for (const auto& person : people) {
        cout << "Total fines for " << person->getName() << ": " << person->getFineManager().getTotalFinesAmount() << endl;
        person->getFineManager().applyFinePolicies();
    }

    return 0;
}