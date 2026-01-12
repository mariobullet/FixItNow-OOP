#include "CerereReparatie.h"

CerereReparatie::CerereReparatie(int id, const Electrocasnic &aparat, const string &timestamp, int complexitate)
    : id(id), timestamp(timestamp), complexitate(complexitate), esteAsignata(false), esteFinalizata(false)
{
    this->aparat = aparat.clone();
    if (complexitate == 0)
    {
        durataEstimata = 0;
        pretReparatie = 0.0;
    }
    else
    {
        int vechime = 2026 - this->aparat->getAn();
        if (vechime < 1)
            vechime = 1;
        durataEstimata = vechime * complexitate;
        pretReparatie = this->aparat->getPret() * durataEstimata;
    }
}

CerereReparatie::~CerereReparatie()
{
    if (aparat)
        delete aparat;
}

CerereReparatie::CerereReparatie(const CerereReparatie &other)
    : id(other.id), timestamp(other.timestamp), complexitate(other.complexitate), durataEstimata(other.durataEstimata),
      pretReparatie(other.pretReparatie), esteAsignata(other.esteAsignata), esteFinalizata(other.esteFinalizata)
{
    if (other.aparat)
        aparat = other.aparat->clone();
    else
        aparat = nullptr;
}

CerereReparatie &CerereReparatie::operator=(const CerereReparatie &other)
{
    if (this != &other)
    {
        if (aparat)
            delete aparat;
        id = other.id;
        timestamp = other.timestamp;
        complexitate = other.complexitate;
        durataEstimata = other.durataEstimata;
        pretReparatie = other.pretReparatie;
        esteAsignata = other.esteAsignata;
        esteFinalizata = other.esteFinalizata;

        if (other.aparat)
            aparat = other.aparat->clone();
        else
            aparat = nullptr;
    }
    return *this;
}