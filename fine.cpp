#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

// Interface for creating fine policies
class FinePolicy {
public:
    virtual ~FinePolicy() = default;
    virtual void applyPolicy(double& amount) const = 0;
    virtual string getPolicyDescription() const = 0;
};

// Concrete policies for fine application
class StandardFinePolicy : public FinePolicy {
public:
    void applyPolicy(double& amount) const override {
        // No modification needed for standard fine
    }

    string getPolicyDescription() const override {
        return "Applying standard fine policy.";
    }
};

class HeavyFinePolicy : public FinePolicy {
public:
    void applyPolicy(double& amount) const override {
        amount *= 2; // Double the fine for heavy violations
    }

    string getPolicyDescription() const override {
        return "Applying heavy fine policy with double the amount.";
    }
};

// Violation class to represent an offense and keep track of the total
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

// Abstract Fine class, depending on FinePolicy abstraction
class Fine {
protected:
    double amount;
    shared_ptr<Violation> violation;
    shared_ptr<FinePolicy> finePolicy;
    static double totalFinesAmount;

public:
    Fine(double amt, shared_ptr<Violation> viol, shared_ptr<FinePolicy> policy)
        : amount(amt), violation(viol), finePolicy(policy) {
        finePolicy->applyPolicy(amount); // Apply the fine policy on amount
        totalFinesAmount += amount;
    }

    virtual ~Fine() = default;

    double getAmount() const {
        return amount;
    }

    void setAmount(double amt) {
        totalFinesAmount -= amount;
        amount = amt;
        finePolicy->applyPolicy(amount); // Reapply policy on updated amount
        totalFinesAmount += amount;
    }

    static double getTotalFinesAmount() {
        return totalFinesAmount;
    }

    void applyFinePolicy() const {
        cout << finePolicy->getPolicyDescription() << ": " << amount << endl;
    }
};

double Fine::totalFinesAmount = 0;

// Interface for FineFactory to allow dependency injection
class FineFactory {
public:
    virtual ~FineFactory() = default;
    virtual unique_ptr<Fine> createFine(double amount, shared_ptr<Violation> violation, bool heavy) = 0;
};

// Concrete implementation of FineFactory using the Standard and Heavy policies
class ConcreteFineFactory : public FineFactory {
public:
    unique_ptr<Fine> createFine(double amount, shared_ptr<Violation> violation, bool heavy) override {
        shared_ptr<FinePolicy> policy;
        if (heavy) {
            policy = make_shared<HeavyFinePolicy>();
        } else {
            policy = make_shared<StandardFinePolicy>();
        }
        return make_unique<Fine>(amount, violation, policy);
    }
};

// FineManager handles multiple fines and their aggregation
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

// Entity class for named entities
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

// Person class that has a FineManager to manage fines
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

// Manages violations and provides an interface to add new ones
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

// Vehicle class manages violations and assigns fines through an injected FineFactory
class Vehicle {
private:
    string registration_number;
    Person* owner;
    ViolationManager violationManager;
    FineFactory& fineFactory;

public:
    Vehicle(const string& reg_num, Person* own, FineFactory& factory)
        : registration_number(reg_num), owner(own), fineFactory(factory) {}

    void addViolationWithFine(const string& violationDescription, double fineAmount, bool heavy = false) {
        auto violation = violationManager.addViolation(violationDescription);
        auto fine = fineFactory.createFine(fineAmount, violation, heavy);
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
    ConcreteFineFactory fineFactory;

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

            Vehicle vehicle(registrationNumber, person.get(), fineFactory);

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
