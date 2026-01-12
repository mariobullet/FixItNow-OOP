#include "Receptioner.h"

void Receptioner::afisare() const
{
    cout << "Receptioner: " << getNume() << " " << getPrenume() << ", domiciliu=" << getOras() << ", salariu: " << calculeazaSalariu() << "/n";

    if (!reqIds.empty())
    {
        cout << "  cereri receptionate: ";
        for (int id : reqIds)
            cout << id << ' ';
        cout << "\n";
    }
}
