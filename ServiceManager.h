#ifndef SERVICE_MANAGER_H
#define SERVICE_MANAGER_H

#include "Angajat.h"
#include "CerereReparatie.h"

struct AparatRefuzat
{
    string numeComplet;
    int contor;
};

class ServiceManager
{
private:
    static ServiceManager *instance;
    vector<Angajat *> employees;
    vector<CerereReparatie *> requests;
    vector<string> catalog;
    vector<AparatRefuzat> refuzuri;

    ServiceManager() = default;
    void inregistreazaRefuz(const string &marca, const string &model);

public:
    ServiceManager(const ServiceManager &) = delete;
    ServiceManager &operator=(const ServiceManager &) = delete;

    ~ServiceManager();
    static ServiceManager &getInstance();
    static void destroyInstance();

    Angajat *cautaAngajatDupaCNP(const string &cnp) const;
    void addEmployee(Angajat *emp) { employees.push_back(emp); }
    const vector<Angajat *> &getEmployees() const { return employees; }
    bool concediazaAngajat(int id);

    void adaugaInCatalog(const string &marca, const string &model)
    {
        catalog.push_back(marca + model);
    }
    bool verificaCatalog(const string &marca, const string &model) const
    {
        string cheie = marca + model;
        for (const auto &s : catalog)
            if (s == cheie)
                return true;
        return false;
    }
    bool verificaFunctionalitateService() const;

    bool primesteCerere(const CerereReparatie &req);
    void alocaCereri(ostream &out);

    void afiseazaRaportRefuzuri();

    void afiseazaDashboardStatus();
    void simuleazaExecutieReparatii();

    void genereazaRaportTopSalariiCSV();
    void genereazaRaportTopDurataCSV();
    void genereazaRaportAsteptareCSV();
};

#endif