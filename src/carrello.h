// Carrello.h
#ifndef CARRELLO_H
#define CARRELLO_H

#include <string>

class Carrello {
public:
    std::string identificatore_utente;
    std::string codice_prodotto;

    // Costruttore
    Carrello(std::string id_utente, std::string codice_prod)
        : identificatore_utente(id_utente), codice_prodotto(codice_prod) {}
};

#endif // CARRELLO_H
