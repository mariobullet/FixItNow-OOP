#ifndef TEHNICIAN_H
#define TEHNICIAN_H

#include "Angajat.h"
#include <vector>
#include <string>
#include <algorithm>

struct Competenta
{
    string categorie;
    vector<string> branduri;
};

class Tehnician : public Angajat
{
private:
    vector<Competenta> skills;
    double valoareReparatii;
    int cereriCurente;
    int maxDurata;

public:
    Tehnician(int id, const string &nume, const string &prenume, const string &cnp, const string &dataAngajarii, const string &oras)
        : Angajat(id, nume, prenume, cnp, dataAngajarii, oras), valoareReparatii(0.0), cereriCurente(0), maxDurata(0)
    {
    }

    void addSkill(const string &category, const string &brand);
    void adaugaBonus(double cost) { valoareReparatii += cost; }

    void actualizeazaMaxDurata(int durata)
    {
        if (durata > maxDurata)
            maxDurata = durata;
    }
    int getMaxDurata() const { return maxDurata; }

    double calculeazaSalariu() const override
    {
        return Angajat::calculeazaSalariu() + (valoareReparatii * 0.02);
    }

    const vector<Competenta> &getSkills() const { return skills; }

    bool poateRepara(const string &category, const string &brand) const;
    bool eDisponibil() const { return cereriCurente < 3; }
    void asigneazaCerere() { cereriCurente++; }

    string getTip() const override { return "Tehnician"; }
    void afisare() const override;
};

#endif