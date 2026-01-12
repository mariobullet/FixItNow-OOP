#include "ServiceManager.h"
#include "Tehnician.h"
#include "Receptioner.h"
#include "Supervizor.h"
#include <algorithm>
#include <fstream>
#include <iomanip>

ServiceManager *ServiceManager::instance = nullptr;

ServiceManager::~ServiceManager()
{
    for (auto *e : employees)
        delete e;
    for (auto *r : requests)
        delete r;
    employees.clear();
    requests.clear();
}

ServiceManager &ServiceManager::getInstance()
{
    if (instance == nullptr)
        instance = new ServiceManager();
    return *instance;
}

void ServiceManager::destroyInstance()
{
    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}

bool ServiceManager::concediazaAngajat(int id)
{
    for (int i = 0; i < employees.size(); ++i)
    {
        if (employees[i]->getId() == id)
        {
            delete employees[i];
            employees.erase(employees.begin() + i);
            return true;
        }
    }
    return false;
}

Angajat *ServiceManager::cautaAngajatDupaCNP(const string &cnp) const
{
    for (auto *ang : employees)
        if (ang->getCnp() == cnp)
            return ang;
    return nullptr;
}

bool ServiceManager::verificaFunctionalitateService() const
{
    int countTech = 0, countRec = 0, countSup = 0;
    for (auto *ang : employees)
    {
        string tip = ang->getTip();
        if (tip == "Tehnician")
            countTech++;
        else if (tip == "Receptioner")
            countRec++;
        else if (tip == "Supervizor")
            countSup++;
    }
    if (countTech < 3 || countRec < 1 || countSup < 1)
    {
        cout << " Personal insuficient!\n";
        return false;
    }
    return true;
}

void ServiceManager::inregistreazaRefuz(const string &marca, const string &model)
{
    string nume = marca + "-" + model;
    for (auto &item : refuzuri)
    {
        if (item.numeComplet == nume)
        {
            item.contor++;
            return;
        }
    }
    refuzuri.push_back({nume, 1});
}

void ServiceManager::afiseazaRaportRefuzuri()
{
    cout << "RAPORT REFUZURI\n";
    if (refuzuri.empty())
    {
        cout << "Niciun refuz.\n";
        return;
    }
    sort(refuzuri.begin(), refuzuri.end(), [](const AparatRefuzat &a, const AparatRefuzat &b)
         { return a.contor > b.contor; });
    for (const auto &item : refuzuri)
    {
        cout << item.numeComplet << ": " << item.contor << "\n";
    }
}

bool ServiceManager::primesteCerere(const CerereReparatie &req)
{
    const Electrocasnic *ap = req.getAparat();
    if (!verificaCatalog(ap->getMarca(), ap->getModel()))
    {
        cout << "[RESPINS] Model nesuportat: " << ap->getMarca() << "\n";
        inregistreazaRefuz(ap->getMarca(), ap->getModel());
        return false;
    }
    for (auto *r : requests)
    {
        if (r->getTimestamp() == req.getTimestamp())
        {
            cout << "[RESPINS] Duplicate Timestamp\n";
            return false;
        }
    }
    requests.push_back(new CerereReparatie(req));
    return true;
}

void ServiceManager::alocaCereri(ostream &out)
{
    out << "PROCES de alocare\n";
    for (auto *req : requests)
    {
        if (req->eAsignata())
            continue;
        bool asignat = false;
        const Electrocasnic *ap = req->getAparat();

        for (auto *emp : employees)
        {
            if (emp->getTip() == "Tehnician")
            {
                Tehnician *tech = static_cast<Tehnician *>(emp);
                if (tech->poateRepara(ap->getTip(), ap->getMarca()) && tech->eDisponibil())
                {
                    tech->asigneazaCerere();
                    tech->adaugaBonus(req->getPret());

                    tech->actualizeazaMaxDurata(req->getDurata());

                    req->marcheazaAsignata();
                    out << "[ALOCAT] ID " << req->getId() << " " << tech->getNume() << "\n";
                    asignat = true;
                    break;
                }
            }
        }
        if (!asignat)
            out << "[ASTEPTARE] ID " << req->getId() << "\n";
    }
}
// facut cu ai (incercare de tip meniu-cereri reparatii)
void ServiceManager::afiseazaDashboardStatus()
{
    cout << "\n+-----+----------------------+---------------------+--------------------+\n";
    cout << "| ID  | Produs               | Status              | Detalii            |\n";
    cout << "+-----+----------------------+---------------------+--------------------+\n";

    for (auto *req : requests)
    {
        cout << "| " << setw(3) << req->getId() << " | "
             << setw(20) << req->getAparat()->getMarca().substr(0, 19) << " | ";

        if (req->eFinalizata())
        {
            cout << "FINALIZAT           | ";
        }
        else if (req->eAsignata())
        {
            cout << "IN CURS (Reparatie) | ";
        }
        else
        {
            cout << "IN ASTEPTARE        | ";
        }

        cout << "Durata: " << setw(3) << req->getDurata() << "h       |\n";
    }
    cout << "+-----+----------------------+---------------------+--------------------+\n";
}

void ServiceManager::simuleazaExecutieReparatii()
{
    cout << "\n[SIMULARE] Tehnicienii incep lucrul...\n";
    for (auto *req : requests)
    {
        if (req->eAsignata() && !req->eFinalizata())
        {
            cout << "   >> Se lucreaza la cererea " << req->getId()
                 << " (" << req->getAparat()->getMarca() << ")... ";
            req->finalizeaza();
            cout << "GATA!\n";
        }
    }
    cout << "[SIMULARE] Toate reparatiile asignate au fost finalizate.\n";
}

void ServiceManager::genereazaRaportTopSalariiCSV()
{
    ofstream csv("top_salarii.csv");
    if (!csv.is_open())
        return;
    csv << "Nume,Prenume,Functie,Salariu\n";

    vector<Angajat *> temp = employees;
    sort(temp.begin(), temp.end(), [](Angajat *a, Angajat *b)
         { return a->calculeazaSalariu() > b->calculeazaSalariu(); });
    if (temp.size() > 3)
        temp.resize(3);
    sort(temp.begin(), temp.end(), [](Angajat *a, Angajat *b)
         {
        if (a->getNume() != b->getNume()) return a->getNume() < b->getNume();
        return a->getPrenume() < b->getPrenume(); });

    for (auto *ang : temp)
    {
        csv << ang->getNume() << "," << ang->getPrenume() << "," << ang->getTip() << "," << ang->calculeazaSalariu() << "\n";
    }
    csv.close();
}

void ServiceManager::genereazaRaportTopDurataCSV()
{
    ofstream csv("top_durata_tehnician.csv");
    if (!csv.is_open())
        return;
    csv << "Nume,Prenume,CNP,MaxDurataReparatie\n";

    Tehnician *bestTech = nullptr;
    int maxDurata = -1;
    for (auto *emp : employees)
    {
        if (emp->getTip() == "Tehnician")
        {
            Tehnician *t = static_cast<Tehnician *>(emp);
            if (t->getMaxDurata() > maxDurata)
            {
                maxDurata = t->getMaxDurata();
                bestTech = t;
            }
        }
    }
    if (bestTech)
    {
        csv << bestTech->getNume() << "," << bestTech->getPrenume() << "," << bestTech->getCnp() << "," << bestTech->getMaxDurata() << "\n";
    }
    csv.close();
}

void ServiceManager::genereazaRaportAsteptareCSV()
{
    ofstream csv("cereri_asteptare.csv");
    if (!csv.is_open())
        return;
    csv << "ID,Tip,Marca,Model,Complexitate,Data\n";

    vector<CerereReparatie *> pending;
    for (auto *r : requests)
    {
        if (!r->eAsignata())
            pending.push_back(r);
    }
    sort(pending.begin(), pending.end(), [](CerereReparatie *a, CerereReparatie *b)
         {
        const auto* apA = a->getAparat();
        const auto* apB = b->getAparat();
        if (apA->getTip() != apB->getTip()) return apA->getTip() < apB->getTip();
        if (apA->getMarca() != apB->getMarca()) return apA->getMarca() < apB->getMarca();
        return apA->getModel() < apB->getModel(); });

    for (auto *r : pending)
    {
        const auto *ap = r->getAparat();
        csv << r->getId() << "," << ap->getTip() << "," << ap->getMarca() << "," << ap->getModel() << "," << r->getComplexitate() << "," << r->getTimestamp() << "\n";
    }
    csv.close();
}