#ifndef ELECTROCASNIC_H
#define ELECTROCASNIC_H

#include <string>
#include <iostream>

using namespace std;

class Electrocasnic
{
protected:
    string tip;
    string marca;
    string model;
    int anFabricatie;
    double pretCatalog;

public:
    Electrocasnic(const string &tip, const string &marca, const string &model, int an, double pret)
        : tip(tip), marca(marca), model(model), anFabricatie(an), pretCatalog(pret) {}

    virtual ~Electrocasnic() {}
    virtual Electrocasnic *clone() const = 0;

    int getAn() const { return anFabricatie; }
    double getPret() const { return pretCatalog; }

    const string &getTip() const { return tip; }
    const string &getMarca() const { return marca; }
    const string &getModel() const { return model; }

    virtual void afisareDetalii() const
    {
        cout << tip << " " << marca << " " << model << " an: " << anFabricatie << ", pret: " << pretCatalog;
    }
};

class Frigider : public Electrocasnic
{
private:
    bool areCongelator;

public:
    Frigider(const string &marca, const string &model, int an, double pret, bool congelator)
        : Electrocasnic("Frigider", marca, model, an, pret), areCongelator(congelator) {}

    Electrocasnic *clone() const override { return new Frigider(*this); }

    void afisareDetalii() const override
    {
        Electrocasnic::afisareDetalii();
        cout << " [congelator: " << (areCongelator ? "DA" : "NU") << "]";
    }
};

class Televizor : public Electrocasnic
{
private:
    double diagonala;
    string unitateMasura;

public:
    Televizor(const string &marca, const string &model, int an, double pret, double diag, const string &um)
        : Electrocasnic("Televizor", marca, model, an, pret), diagonala(diag), unitateMasura(um) {}

    Electrocasnic *clone() const override { return new Televizor(*this); }

    void afisareDetalii() const override
    {
        Electrocasnic::afisareDetalii();
        cout << " [Diag: " << diagonala << " " << unitateMasura << "]";
    }
};

class MasinaSpalat : public Electrocasnic
{
private:
    double cap;

public:
    MasinaSpalat(const string &marca, const string &model, int an, double pret, double capacitate)
        : Electrocasnic("MasinaSpalat", marca, model, an, pret), cap(capacitate) {}

    Electrocasnic *clone() const override { return new MasinaSpalat(*this); }

    void afisareDetalii() const override
    {
        Electrocasnic::afisareDetalii();
        cout << " [Capacitate: " << cap << "kg]";
    }
};

#endif