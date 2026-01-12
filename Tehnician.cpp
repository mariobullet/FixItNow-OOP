#include "Tehnician.h"

void Tehnician::addSkill(const string &category, const string &brand)
{
    bool gasit = false;
    for (int i = 0; i < skills.size(); ++i)
    {
        if (skills[i].categorie == category)
        {
            skills[i].branduri.push_back(brand);
            gasit = true;
            break;
        }
    }
    if (!gasit)
    {
        Competenta c;
        c.categorie = category;
        c.branduri.push_back(brand);
        skills.push_back(c);
    }
}

bool Tehnician::poateRepara(const string &category, const string &brand) const
{
    for (size_t i = 0; i < skills.size(); ++i)
    {
        if (skills[i].categorie == category)
        {
            const vector<string> &lista = skills[i].branduri;
            for (size_t j = 0; j < lista.size(); ++j)
            {
                if (lista[j] == brand)
                    return true;
            }
        }
    }
    return false;
}

void Tehnician::afisare() const
{
    cout << "Tehnician: " << getNume() << " " << getPrenume() << " nr lucrari: " << cereriCurente << "/3" << ", Salariu: " << calculeazaSalariu() << "\n";

    if (!skills.empty())
    {
        cout << "  Competente: \n";
        for (const auto &sk : skills)
        {
            cout << "- " << sk.categorie << ": ";
            for (const auto &b : sk.branduri)
                cout << b << " ";
            cout << "\n";
        }
    }
}