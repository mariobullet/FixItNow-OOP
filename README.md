# FixItNow-OOP
Online repair shop for electronic devices

Technical Documentation - Home Appliance Service Manager
Introduction
This project represents a C++ application for managing the activity of a home appliance service center. The application simulates the complete workflow: from hiring personnel and receiving repair requests, to the automatic allocation of technicians and the generation of final reports.

1. Application Architecture
The application is built modularly, separating entities (Employees, Appliances) from the business logic (the Manager). Below is the visual representation of the class relationships:

Code snippet

classDiagram
    class ServiceManager {
        - instance: ServiceManager*
        - employees: vector~Angajat*~
        - requests: vector~CerereReparatie*~
        - catalog: vector~string~
        - refuzuri: vector~AparatRefuzat~
        + getInstance()
        + alocaCereri()
        + genereazaRapoarteCSV()
    }

    class Angajat {
        <<Abstract>>
        # nume: string
        # cnp: string
        + calculeazaSalariu()
        + getTip()
    }

    class Tehnician {
        - skills: vector
        - maxDurata: int
        + poateRepara()
        + adaugaBonus()
    }

    class Receptioner {
    }

    class Supervizor {
    }

    class Electrocasnic {
        <<Abstract>>
        # marca: string
        # pretCatalog: double
        + clone()
    }

    class Frigider {
    }
    class Televizor {
    }
    class MasinaSpalat {
    }

    class CerereReparatie {
        - id: int
        - aparat: Electrocasnic*
        - durataEstimata: int
        - pretReparatie: double
        - esteFinalizata: bool
    }

    ServiceManager *-- Angajat
    ServiceManager *-- CerereReparatie
    Angajat <|-- Tehnician
    Angajat <|-- Receptioner
    Angajat <|-- Supervizor
    CerereReparatie *-- Electrocasnic
    Electrocasnic <|-- Frigider
    Electrocasnic <|-- Televizor
    Electrocasnic <|-- MasinaSpalat
Component Description
A. Application Core
ServiceManager (Singleton): This is the central class coordinating the entire system. I chose the Singleton pattern to guarantee that there is a single instance managing resources (employees, requests, catalog). Validations, technician allocation, and report generation take place here.

B. Employee Hierarchy
Angajat (Employee): Abstract base class. Here I implemented validations for the CNP (Personal Numeric Code), including the control digit check, and age validation.

Tehnician (Technician): The most complex derived class. Besides standard data, it holds skills (which brands they can repair), an availability counter (max 3 simultaneous requests), and statistics for reports (max duration of a repair).

Supervizor & Receptioner: Have specific roles in the business logic (e.g., different salary calculation for the supervisor, the necessity of a receptionist for service validation).

C. Appliance Hierarchy
I use polymorphism to manage different types of appliances (Frigider - Fridge, Televizor - TV, MasinaSpalat - Washing Machine) in a uniform way.

Each derived class has specific attributes (e.g., diagonal for TV, capacity for washing machines) which are displayed by overriding virtual methods.

D. Repair Request (CerereReparatie)
This class links an appliance to the service process.

Cost and duration calculations are done automatically in the constructor, based on the appliance's age and the complexity of the defect.

2. Workflow
The execution flow simulates a real day of activity:

Initialization:

The catalog is populated with supported models.

Personnel hiring is performed. The application automatically checks if the team is complete (min 3 technicians, 1 receptionist, 1 supervisor). If not, execution stops.

Request Processing (Input from File):

Data is read from cereri.txt. Here I implemented a Factory-like logic: depending on the keyword read ("Frigider", "Televizor"), the corresponding object is instantiated.

Business validations occur here: unique timestamp, existing model in the catalog.

Invalid requests are rejected and counted in the refusal report.

Automatic Allocation:

The system iterates through valid requests.

The algorithm searches for the first technician who is competent (knows the respective brand) and available (has < 3 active jobs).

If found, the request becomes "Assigned", and the technician receives a salary bonus. Otherwise, the request remains "Waiting".

Simulation and Reporting:

A dashboard is displayed in the console showing request status.

Time passage is simulated (status changes to "Finished").

Finally, the 3 required CSV files are exported (top_salarii.csv, top_durata.csv, cereri_asteptare.csv).

3. Technical Elements and Challenges
During development, I focused on several important technical aspects:

Memory Management (Rule of 5 & Prototype):

Since the CerereReparatie class contains a pointer to a polymorphic object (Electrocasnic*), I implemented the Rule of 5 (Copy Constructor, Assignment Operator, Destructor, etc.) to avoid shallow copy issues and memory leaks.

I used the Prototype pattern (the clone() method) to correctly copy appliances without knowing their exact type at compile time.

Avoiding dynamic_cast:

A challenge was identifying the employee type without abusing RTTI (dynamic_cast). I solved this elegantly via a pure virtual method getTip() in the base class. Thus, iteration through the employee vector is much faster and safer.

Polymorphic Reading:

Reading from the file was interesting because each line could represent a different object type with a different number of parameters. I solved this via a decision structure that reads common parameters first, then specific ones based on the type.

Data Validation:

I implemented the official CNP validation algorithm (verifying the control digit), ensuring data integrity rather than just a simple length check.

4. Testing Scenarios
To ensure the application is robust, I ran the following scenarios:

Happy Path: A normal run with correct data, where all requests are allocated and reports are generated.

Insufficient Resources Test: Tried starting the application with only 2 technicians. The application detected the issue and stopped in a controlled manner.

Stress Test (Overload):

Introduced more requests in the file than the total capacity of the technicians.

Result: Surplus requests received the [WAITING] status and were correctly written to cereri_asteptare.csv.

Business Validation Test:

Unsupported product ("Sony") -> Automatically rejected and added to the refusal report.

Duplicate timestamp -> Rejected.

Salary calculation -> Manually verified that the 2% bonuses are applied correctly.
