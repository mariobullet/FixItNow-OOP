#ifndef RECEPTIONER_H
#define RECEPTIONER_H

#include "Angajat.h"
#include <vector>

class Receptioner : public Angajat
{
private:
    vector<int> reqIds;

public:
    Receptioner(int id, const string &nume, const string &prenume, const string &cnp, const string &dataAngajarii, const string &oras)
        : Angajat(id, nume, prenume, cnp, dataAngajarii, oras)
    {
    }

    void addHandledRequest(int reqId) { reqIds.push_back(reqId); }
    const vector<int> &getHandledRequests() const { return reqIds; }

    string getTip() const override { return "Receptioner"; }

    void afisare() const override;
};

#endif