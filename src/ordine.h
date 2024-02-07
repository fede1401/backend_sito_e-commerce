// Ordine.h
#ifndef ORDINE_H
#define ORDINE_H

#include <string>

class Ordine {
public:
    std::string nome_uteCompratore;
    int identificatore_ordine;
    int codice_prodotto;
    std::string nome_uteTrasportatore;
    std::string data_ordine_effettuato;
    std::string stato_consegna;
    std::string ditta_spedizione;
    std::string via_spedizione;
    std::string città_spedizione;
    std::string numero_civico_spedizione;

    Ordine():
      nome_uteCompratore(""),
      identificatore_ordine(-1),
      codice_prodotto(-1),
      nome_uteTrasportatore(""),
      data_ordine_effettuato(""),
      stato_consegna(""),
      ditta_spedizione(""),
      via_spedizione(""),
      città_spedizione(""),
      numero_civico_spedizione(""){}


    // Costruttore
    Ordine(std::string nome_uteCompratore, int identificatore_ordine, int codice_prodotto, std::string id_trasportatore,
           std::string data_ordine, std::string stato, std::string ditta, std::string via, std::string città, int numero_civico)
        : nome_uteCompratore(nome_uteCompratore), identificatore_ordine(identificatore_ordine), codice_prodotto(codice_prodotto),
          nome_uteTrasportatore(nome_uteTrasportatore), data_ordine_effettuato(data_ordine_effettuato), stato_consegna(stato_consegna),
          ditta_spedizione(ditta_spedizione), via_spedizione(via_spedizione), città_spedizione(città_spedizione), numero_civico_spedizione(numero_civico_spedizione) {}  
};

#endif // ORDINE_H
