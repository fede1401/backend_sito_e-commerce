// Ordine.h
#ifndef ORDINE_H
#define ORDINE_H

#include <string>

enum class StatoOrdine {
    InElaborazione,
    Spedito
    };


class Ordine {
public:
    int identificatore_ordine;
    int codice_prodotto;
    std::string nome_uteCompratore;
    std::string data_ordine_effettuato;
    StatoOrdine stato_ordine;


    // Definizione costruttori:  
    Ordine():
      identificatore_ordine(-1),
      codice_prodotto(-1),
      nome_uteCompratore(""),
      data_ordine_effettuato(""),
      stato_ordine(){}
          
    Ordine( int identificatore_ordine,  int codice_prodotto, std::string nome_uteCompratore, std::string data_ordine, StatoOrdine stato_ordine ): 
          
          identificatore_ordine(identificatore_ordine),
          codice_prodotto(codice_prodotto), 
          nome_uteCompratore(nome_uteCompratore),           
          data_ordine_effettuato(data_ordine), 
          stato_ordine(stato_ordine) {}  


    void impostaStato(StatoOrdine nuovoStato) {   
      stato_ordine = nuovoStato;    
    }

};



#endif // ORDINE_H