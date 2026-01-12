#ifndef ANGAJAT_H
#define ANGAJAT_H

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

using namespace std;

class Angajat
{
protected:
    const int id;
    string nume;
    string prenume;
    string cnp;
    string dataAngajarii;
    string orasDomiciliu;

    const double SALARIU_BAZA = 4000.0;

private:
    bool valideazaCNP(const string &cnp);
    bool valideazaVarsta(const string &cnp, const string &dataAngajarii);
    int getAn(const string &data) const;

public:
    Angajat(int id, const string &nume, const string &prenume, const string &cnp, const string &dataAngajarii, const string &orasDomiciliu);
    virtual ~Angajat() {}

    virtual string getTip() const = 0;

    int getId() const { return id; }
    const string &getNume() const { return nume; }
    const string &getPrenume() const { return prenume; }
    const string &getCnp() const { return cnp; }
    const string &getDataAngajarii() const { return dataAngajarii; }
    const string &getOras() const { return orasDomiciliu; }

    void setNume(const string &numeNou);

    virtual double calculeazaSalariu() const;

    virtual void afisare() const;
};

#endif