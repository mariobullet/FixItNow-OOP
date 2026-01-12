#ifndef SUPERVIZOR_H
#define SUPERVIZOR_H

#include "Angajat.h"
#include <vector>
#include <string>

class Supervizor : public Angajat
{
public:
    Supervizor(int id, const string &nume, const string &prenume, const string &cnp, const string &dataAngajarii, const string &oras)
        : Angajat(id, nume, prenume, cnp, dataAngajarii, oras)
    {
    }

    double calculeazaSalariu() const override
    {
        double bazaCalculata = Angajat::calculeazaSalariu();
        double sporConducere = 4000.0 * 0.20;

        return bazaCalculata + sporConducere;
    }

    void afisare() const override;

    string getTip() const override { return "Supervizor"; }
};

#endif