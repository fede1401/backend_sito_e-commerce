// ListaDesideri.h
#ifndef LISTA_DESIDERI_H
#define LISTA_DESIDERI_H

#include <string>

class ListaDesideri {
public:
    std::string identificatore_utente;
    std::string codice_prodotto;

    // Costruttore
    ListaDesideri(std::string id_utente, std::string codice_prod)
        : identificatore_utente(id_utente), codice_prodotto(codice_prod) {}
};

#endif // LISTA_DESIDERI_H
