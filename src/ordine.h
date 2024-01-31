// Ordine.h
#ifndef ORDINE_H
#define ORDINE_H

#include <string>

class Ordine {
public:
    std::string identificatore_utente;
    std::string identificatore_ordine;
    std::string codice_prodotto;
    std::string identificatore_trasportatore;
    std::string data_ordine_effettuato;
    std::string stato_consegna;
    std::string ditta_spedizione;
    std::string via_spedizione;
    std::string città_spedizione;
    int numero_civico_spedizione;

    // Costruttore
    Ordine(std::string id_utente, std::string id_ordine, std::string codice_prod, std::string id_trasportatore,
           std::string data_ordine, std::string stato, std::string ditta, std::string via, std::string città, int numero_civico)
        : identificatore_utente(id_utente), identificatore_ordine(id_ordine), codice_prodotto(codice_prod),
          identificatore_trasportatore(id_trasportatore), data_ordine_effettuato(data_ordine), stato_consegna(stato),
          ditta_spedizione(ditta), via_spedizione(via), città_spedizione(città), numero_civico_spedizione(numero_civico) {}
};

#endif // ORDINE_H
