#include "Supervizor.h"

void Supervizor::afisare() const
{
    cout << "Supervizor: " << getNume() << " " << getPrenume() << ", domiciliu" << getOras() << ", salariu: " << calculeazaSalariu() << "\n";
}
