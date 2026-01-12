#ifndef CERERE_REPARATIE_H
#define CERERE_REPARATIE_H

#include <string>
#include "Electrocasnic.h"

using namespace std;

class CerereReparatie
{
private:
    int id;
    Electrocasnic *aparat;
    string timestamp;
    int complexitate;
    int durataEstimata;
    double pretReparatie;

    bool esteAsignata;
    bool esteFinalizata;

public:
    CerereReparatie(int id, const Electrocasnic &aparat, const string &timestamp, int complexitate);

    ~CerereReparatie();
    CerereReparatie(const CerereReparatie &other);
    CerereReparatie &operator=(const CerereReparatie &other);

    int getId() const { return id; }
    const string &getTimestamp() const { return timestamp; }
    int getComplexitate() const { return complexitate; }
    int getDurata() const { return durataEstimata; }
    double getPret() const { return pretReparatie; }
    const Electrocasnic *getAparat() const { return aparat; }

    bool eAsignata() const { return esteAsignata; }
    void marcheazaAsignata() { esteAsignata = true; }

    bool eFinalizata() const { return esteFinalizata; }
    void finalizeaza() { esteFinalizata = true; }
};

#endif