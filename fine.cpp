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

    void setDescription(const string& desc) {  
        description = desc;
    }

    static int getTotalViolations() { 
        return totalViolations;
    }
};

int Violation::totalViolations = 0;

class Fine {
private:
    double amount; 
    Violation* violation;
    static double totalFinesAmount; 

public:
    Fine(double amt, Violation* viol) : amount(amt), violation(viol) {
        totalFinesAmount += amt;
    }

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
};

double Fine::totalFinesAmount = 0;

class Person {
private:
    string name; 
    string license_number; 
    vector<Fine*> fines; 

public:
    Person(const string& n, const string& license)
        : name(n), license_number(license) {}

    ~Person() {
        for (Fine* fine : fines) {
            delete fine;
        }
    }

    string getName() const {  
        return name;
    }

    void setName(const string& n) {  
        name = n;
    }

    string getLicenseNumber() const { 
        return license_number;
    }

    void setLicenseNumber(const string& license) {  
        license_number = license;
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

    string getRegistrationNumber() const {  
        return registration_number;
    }

    void setRegistrationNumber(const string& reg_num) { 
        registration_number = reg_num;
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

        Person* person = new Person(personName, licenseNumber);
        person->setName(personName);  
        person->setLicenseNumber(licenseNumber);

        people.push_back(person); 
    }

    for (int i = 0; i < numPeople; ++i) {
        Person* person = people[i];

        cout << "Enter number of vehicles for " << person->getName() << ": ";
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
                violation->setDescription(violationDescription);  
                vehicle->addViolation(violation, fineAmount);
            }

            delete vehicle; 
        }
    }

    cout << "Total number of violations recorded: " << Violation::getTotalViolations() << endl;
    cout << "Total amount of fines issued: " << Fine::getTotalFinesAmount() << endl;

    for (Person* person : people) {
        cout << "Total fines for " << person->getName() << ": " << person->getTotalFines() << endl;
        delete person; 
    }

    return 0;
}
