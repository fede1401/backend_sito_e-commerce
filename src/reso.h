// Reso.h
#ifndef RESO_H
#define RESO_H

#include <string>

class Reso {
public:
    std::string identificatore_utente;
    std::string codice_prodotto;
    std::string motivazione_reso;

    // Costruttore
    Reso(std::string id_utente, std::string codice_prod, std::string motivazione)
        : identificatore_utente(id_utente), codice_prodotto(codice_prod), motivazione_reso(motivazione) {}
};

#endif // RESO_H
