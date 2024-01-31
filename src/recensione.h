// Recensione.h
#ifndef RECENSIONE_H
#define RECENSIONE_H

#include <string>

class Recensione {
public:
    std::string identificatore_recensione;
    std::string identificatore_utente;
    std::string descrizione;
    int voto_in_stelle;
    std::string codice_prodotto;

    // Costruttore
    Recensione(std::string id_recensione, std::string id_utente, std::string desc, int voto, std::string codice_prod)
        : identificatore_recensione(id_recensione), identificatore_utente(id_utente),
          descrizione(desc), voto_in_stelle(voto), codice_prodotto(codice_prod) {}
};

#endif // RECENSIONE_H
