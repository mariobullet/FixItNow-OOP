#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include "ServiceManager.h"
#include "Tehnician.h"
#include "Receptioner.h"
#include "Supervizor.h"
#include "Electrocasnic.h"

void proceseazaCerere(ServiceManager &sm, const CerereReparatie &req)
{
    bool acceptat = sm.primesteCerere(req);
    if (acceptat)
    {
        cout << " [OK] Cerere " << req.getId() << " receptionata: ";
        req.getAparat()->afisareDetalii();
        cout << "\n";
    }
}

void citesteCereriDinFisier(ServiceManager &sm, const string &numeFisier)
{
    ifstream fisier(numeFisier);
    if (!fisier.is_open())
    {
        cerr << "Nu s a putut deschide fisierul: " << numeFisier << "\n";
        return;
    }

    int id, an, complexitate;
    double pret;
    string tip, marca, model, data, ora;

    while (fisier >> id >> tip >> marca >> model >> an >> pret)
    {
        Electrocasnic *aparat = nullptr;

        if (tip == "Frigider")
        {
            bool areCongelator;
            fisier >> areCongelator;
            aparat = new Frigider(marca, model, an, pret, areCongelator);
        }
        else if (tip == "Televizor")
        {
            double diag;
            string um;
            fisier >> diag >> um;
            aparat = new Televizor(marca, model, an, pret, diag, um);
        }
        else if (tip == "MasinaSpalat")
        {
            double capacitate;
            fisier >> capacitate;
            aparat = new MasinaSpalat(marca, model, an, pret, capacitate);
        }

        fisier >> data >> ora >> complexitate;

        if (aparat)
        {
            string timestamp = data + " " + ora;
            CerereReparatie req(id, *aparat, timestamp, complexitate);

            proceseazaCerere(sm, req);

            delete aparat;
        }
    }

    fisier.close();
}

int main()
{
    ServiceManager &sm = ServiceManager::getInstance();

    sm.adaugaInCatalog("Samsung", "Frost200");
    sm.adaugaInCatalog("Samsung", "UltraHD");
    sm.adaugaInCatalog("LG", "OLED55");
    sm.adaugaInCatalog("LG", "DirectDrive");
    sm.adaugaInCatalog("Arctic", "M100");
    sm.adaugaInCatalog("Bosch", "SilentMax");

    try
    {
        Tehnician *t1 = new Tehnician(1, "Nelu", "Popa", "1900101123457", "2020-01-01", "Bucuresti");
        t1->addSkill("Frigider", "Samsung");
        t1->addSkill("Televizor", "Samsung");
        t1->addSkill("MasinaSpalat", "Arctic");
        sm.addEmployee(t1);

        Tehnician *t2 = new Tehnician(2, "Vladimir", "Danescu", "1850101123451", "2021-01-01", "Ploiesti");
        t2->addSkill("Televizor", "LG");
        t2->addSkill("MasinaSpalat", "LG");
        t2->addSkill("MasinaSpalat", "Bosch");
        sm.addEmployee(t2);

        Tehnician *t3 = new Tehnician(4, "Laurentiu", "Marinel", "1920601123451", "2022-06-01", "Bucuresti");
        t3->addSkill("Frigider", "Samsung");
        sm.addEmployee(t3);

        Supervizor *sup = new Supervizor(3, "Maria", "Ionescu", "2800101123452", "2019-01-01", "Bucuresti");
        sm.addEmployee(sup);

        Receptioner *rec = new Receptioner(5, "Elena", "Popa", "2950101123451", "2023-01-01", "Bucuresti");
        sm.addEmployee(rec);

        if (!sm.verificaFunctionalitateService())
            return 0;

        citesteCereriDinFisier(sm, "cereri.txt");

        cout << "\n Alocare cereri:\n";
        sm.alocaCereri(cout);

        sm.afiseazaDashboardStatus();

        cout << "\n SIMULARE EXECUTIE:\n";
        sm.simuleazaExecutieReparatii();

        cout << " STATUS FINAL \n";
        sm.afiseazaDashboardStatus();

        sm.genereazaRaportTopSalariiCSV();
        sm.genereazaRaportTopDurataCSV();
        sm.genereazaRaportAsteptareCSV();
        sm.afiseazaRaportRefuzuri();
    }
    catch (const exception &e)
    {
        cout << "EROARE: " << e.what() << "\n";
    }

    ServiceManager::destroyInstance();
    return 0;
}