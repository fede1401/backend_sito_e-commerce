// Ordine.h
#ifndef ORDINE_H
#define ORDINE_H

#include <string>
#include "spedizione.h"
#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db/pgsql.h"


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
    std::string via_spedizione;
    std::string città_spedizione;
    std::string numero_civico_spedizione;


    // Definizione costruttori:  
    Ordine():
      identificatore_ordine(-1),
      codice_prodotto(-1),
      nome_uteCompratore(""),
      data_ordine_effettuato(""),
      stato_ordine(),
      via_spedizione(""),
      città_spedizione(""),
      numero_civico_spedizione(""){}
          
    Ordine( int identificatore_ordine,  int codice_prodotto, std::string nome_uteCompratore, std::string data_ordine, StatoOrdine stato_ordine,
    std::string via_spedizione, std::string città_spedizione, std::string numero_civico_spedizione ): 
          
          identificatore_ordine(identificatore_ordine),
          codice_prodotto(codice_prodotto), 
          nome_uteCompratore(nome_uteCompratore),           
          data_ordine_effettuato(data_ordine), 
          stato_ordine(stato_ordine),
          via_spedizione(via_spedizione),
          città_spedizione(città_spedizione),
          numero_civico_spedizione(numero_civico_spedizione) {}  


    void impostaStato(StatoOrdine nuovoStato) {   
      stato_ordine = nuovoStato;    
    }


    void visione_ordini_effettuati(std::string nome_utente_compratore){

      // Connession al database:
      Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

      // Query per caricare tutti gli ordini effettuati:
      sprintf(sqlcmd, "SELECT * FROM Ordine WHERE nome_utente_compratore='%s'", nome_utente_compratore.c_str());
      res = db1.ExecSQLtuples(sqlcmd);
      rows = PQntuples(res);
      int numCols = PQnfields(res);

      for (int i = 0; i < rows; ++i) {
        Ordine ordine;
        ordine.identificatore_ordine = atoi(PQgetvalue(res, i, PQfnumber(res, "idOrdine")));
        ordine.codice_prodotto = atoi(PQgetvalue(res, i, PQfnumber(res, "codProdotto")));
        ordine.nome_uteCompratore = PQgetvalue(res, i, PQfnumber(res, "nome_utente_compratore"));
        ordine.data_ordine_effettuato =  PQgetvalue(res, i, PQfnumber(res, "dataOrdineEffettuato"));
        //ordine.stato_ordine = PQgetvalue(res, i, PQfnumber(res, "statoOrdine"));
        ordine.via_spedizione = PQgetvalue(res, i, PQfnumber(res, "viaSpedizione"));

        std::cout << ordine.identificatore_ordine << std::endl;
      }
      return;
    }


};



#endif // ORDINE_H