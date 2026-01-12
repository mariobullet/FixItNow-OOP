#include "Angajat.h"

Angajat::Angajat(int id, const string &nume, const string &prenume, const string &cnp, const string &dataAngajarii, const string &orasDomiciliu)
    : id(id), nume(nume), prenume(prenume), cnp(cnp), dataAngajarii(dataAngajarii), orasDomiciliu(orasDomiciliu)
{
    if (!valideazaCNP(this->cnp))
    {
        throw invalid_argument("CNP invalid");
    }

    if (!valideazaVarsta(this->cnp, this->dataAngajarii))
    {
        throw invalid_argument("Persoana nu are 16 ani");
    }
}

void Angajat::setNume(const string &numeNou)
{
    nume = numeNou;
}

double Angajat::calculeazaSalariu() const
{
    double salariu = SALARIU_BAZA;
    int vechimeAni = 2026 - getAn(dataAngajarii);

    if (vechimeAni > 0)
    {
        salariu += (vechimeAni * 100);
    }

    if (orasDomiciliu != "Bucuresti")
    {
        salariu += 200.0;
    }

    return salariu;
}

void Angajat::afisare() const
{
    cout << "ID: " << id << nume << " " << prenume << " (CNP: " << cnp << ") - angajat din: " << dataAngajarii << ", oras: " << orasDomiciliu << "\n";
}

int Angajat::getAn(const string &data) const
{
    if (data.size() < 4)
        return 0;
    return stoi(data.substr(0, 4));
}

bool Angajat::valideazaCNP(const string &cnp)
{
    if (cnp.size() != 13)
        return false;
    for (char c : cnp)
    {
        if (!isdigit(c))
            return false;
    }

    const int cheie[] = {2, 7, 9, 1, 4, 6, 3, 5, 8, 2, 7, 9};
    int suma = 0;
    for (int i = 0; i < 12; ++i)
    {
        suma += (cnp[i] - '0') * cheie[i];
    }

    int rest = suma % 11;
    int cifraControl = (rest == 10) ? 1 : rest;

    return (cifraControl == (cnp[12] - '0'));
}

bool Angajat::valideazaVarsta(const string &cnp, const string &dataAngajarii)
{
    int anNastere = getAn(dataAngajarii);
    char sex = cnp[0];
    int anCNP = 0;

    if (sex == '1' || sex == '2')
        anCNP = 1900 + stoi(cnp.substr(1, 2));
    else if (sex == '5' || sex == '6')
        anCNP = 2000 + stoi(cnp.substr(1, 2));

    int anAngajare = getAn(dataAngajarii);
    return (anAngajare - anCNP >= 16);
}