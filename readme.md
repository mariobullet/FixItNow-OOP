                                        Documentatie Tehnica - Manager Service Electrocasnice

Introducere:
Acest proiect reprezinta o aplicatie C++ pentru gestionarea activitatii unui service de electrocasnice. Aplicatia simuleaza fluxul complet de lucru: de la angajarea personalului si primirea cererilor de reparatie, pana la alocarea automata a tehnicienilor si generarea rapoartelor finale.

Arhitectura Aplicatiei:
Aplicatia este construita modular, separand entitatile (Angajati, Electrocasnice) de logica de business (Managerul). Mai jos este reprezentarea vizuala a relatiilor dintre clase:

classDiagram
class ServiceManager { - instance: ServiceManager* - employees: vector~Angajat*~ - requests: vector~CerereReparatie\*~ - catalog: vector~string~ - refuzuri: vector~AparatRefuzat~ + getInstance() + alocaCereri() + genereazaRapoarteCSV()
}

    class Angajat {
        <<Abstract>>
        # nume: string
        # cnp: string
        + calculeazaSalariu()
        + getTip()
    }

    class Tehnician {
        - skills: vector
        - maxDurata: int
        + poateRepara()
        + adaugaBonus()
    }

    class Receptioner {
    }

    class Supervizor {
    }

    class Electrocasnic {
        <<Abstract>>
        # marca: string
        # pretCatalog: double
        + clone()
    }

    class Frigider {
    }
    class Televizor {
    }
    class MasinaSpalat {
    }

    class CerereReparatie {
        - id: int
        - aparat: Electrocasnic*
        - durataEstimata: int
        - pretReparatie: double
        - esteFinalizata: bool
    }

    ServiceManager *-- Angajat
    ServiceManager *-- CerereReparatie
    Angajat <|-- Tehnician
    Angajat <|-- Receptioner
    Angajat <|-- Supervizor
    CerereReparatie *-- Electrocasnic
    Electrocasnic <|-- Frigider
    Electrocasnic <|-- Televizor
    Electrocasnic <|-- MasinaSpalat

Descrierea Componentelor:

A. Core-ul Aplicatiei ServiceManager (Singleton): Este clasa centrala care coordoneaza totul. Am ales pattern-ul Singleton pentru a garanta ca exista o singura instanta care gestioneaza resursele (angajati, cereri, catalog). Aici se intampla validarile, alocarea tehnicienilor si generarea rapoartelor.

B. Ierarhia de Angajati Angajat: Clasa abstracta de baza. Aici am implementat validarile pentru CNP (inclusiv cifra de control) si varsta.

Tehnician: Cea mai complexa clasa derivata. Pe langa datele standard, retine competentele (ce branduri stie sa repare), un contor pentru disponibilitate (maxim 3 cereri simultan) si statistici pentru rapoarte (durata maxima a unei reparatii).

Supervizor & Receptioner: Au roluri specifice in logica de business (ex: calcul salarial diferit pentru supervizor, necesitatea receptionerului pentru validarea service-ului).

C. Ierarhia de Electrocasnice Folosesc polimorfismul pentru a gestiona tipuri diferite de aparate (Frigider, Televizor, MasinaSpalat) intr-un mod uniform.

Fiecare clasa derivata are atribute specifice (ex: diagonala la TV, capacitate la masina de spalat) care sunt afisate prin suprascrierea metodelor virtuale.

D. Cerere Reparatie Aceasta clasa face legatura dintre un aparat si procesul de service.

Calculul costului si al duratei se face automat in constructor, pe baza vechimii aparatului si a complexitatii defectului.

Modul de Functionare Fluxul de executie simuleaza o zi reala de activitate:

Initializare:

Se populeaza catalogul cu modele suportate.

Se face angajarea personalului. Aplicatia verifica automat daca echipa este completa (minim 3 tehnicieni, 1 receptioner, 1 supervizor). Daca nu, executia se opreste.

Procesarea Cererilor (Input din Fisier):

Citesc datele din cereri.txt. Aici am implementat o logica de tip Factory: in functie de cuvantul cheie citit ("Frigider", "Televizor"), instantiez obiectul corespunzator.

Tot aici se fac validarile de business: timestamp unic, model existent in catalog.

Alocarea Automata:

Parcurg lista de cereri valide.

Algoritmul cauta primul tehnician care este competent (stie marca respectiva) si disponibil (are < 3 lucrari).

Daca se gaseste, cererea devine "Asignata", iar tehnicianul primeste un bonus la salariu.

Simulare si Raportare:

Se afiseaza un dashboard in consola cu statusul cererilor.

Se simuleaza trecerea timpului (schimbarea starii in "Finalizat").

La final, se exporta cele 3 fisiere CSV cerute (top_salarii.csv, top_durata.csv, cereri_asteptare.csv).

Elemente Tehnice si Provocari Intalnite Pe parcursul dezvoltarii m-am concentrat pe cateva aspecte tehnice importante:

Gestionarea Memoriei (Rule of 5 & Prototype):

Deoarece clasa CerereReparatie contine un pointer catre un obiect polimorfic (Electrocasnic\*), am implementat Rule of 5 (Copy Constructor, Assignment Operator, Destructor etc.) pentru a evita shallow copy si memory leaks.

Am folosit pattern-ul Prototype (metoda clone()) pentru a copia corect electrocasnicele fara a le cunoaste tipul exact la compilare.

Evitarea dynamic_cast:

O provocare a fost identificarea tipului de angajat fara a abuza de RTTI (dynamic_cast). Am rezolvat elegant prin metoda virtuala pura getTip() in clasa de baza. Astfel, iterarea prin vectorul de angajati este mult mai rapida si sigura.

Citirea Polimorfica:

Citirea din fisier a fost interesanta deoarece fiecare linie putea reprezenta un alt tip de obiect cu un numar diferit de parametri. Am rezolvat asta printr-o structura decizionala care citeste parametrii comuni, apoi pe cei specifici in functie de tip.

Validarea Datelor:

Am implementat algoritmul oficial de validare a CNP-ului (verificarea cifrei de control), nu doar o simpla verificare de lungime.

Scenarii de Testare Pentru a ma asigura ca aplicatia este robusta, am rulat urmatoarele scenarii:

O rulare normala, cu date corecte, unde toate cererile sunt alocate si rapoartele generate.

Test Resurse Insuficiente: Am incercat sa pornesc aplicatia cu doar 2 tehnicieni. Aplicatia a detectat problema si s-a oprit controlat.

Test Supraincarcare :

Am introdus in fisier mai multe cereri decat capacitatea totala a tehnicienilor.

Rezultat: Cererile surplus au primit statusul [ASTEPTARE] si au fost scrise corect in cereri_asteptare.csv.
