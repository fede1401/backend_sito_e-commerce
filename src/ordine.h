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


    // Un utente trasportatore con attibuto disponibilità=0 accede al sistema e controlla gli ordini che non sono ancora stati spediti e li prende in carico per la spedizione.
    Spedizione presaInCaricoTrasportatore(int idOrdine){

        Spedizione spedizione;

        StatoSpedizione stato_spedizione;

        // Connession al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        //int idOrdine = identificatore_ordine;

        // Selezione utenti trasportatori con attributo disponibilità = 0;
        sprintf(sqlcmd, "SELECT nome_utente_trasportatore FROM UtenteTrasportatore WHERE dispo='0'");
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        std::string nome_utente_trasportatore;
        std::string nome_ditta_spedizione;

        stato_spedizione = StatoSpedizione::InTransito;
        std::string statoSpedizioneStr = statoSpedizioneToString(stato_spedizione);

        if (rows >= 1){
            nome_utente_trasportatore = PQgetvalue(res, 0, 0);

            // Selezione il nome della ditta di spedizione dell'utente trasportatore:
            sprintf(sqlcmd, "SELECT nome_DittaSpedizione FROM UtenteTrasportatore WHERE nome_utente_trasportatore='%s'", nome_utente_trasportatore.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows==1){
              nome_ditta_spedizione = PQgetvalue(res, 0, 0);

              // Inseriamo nel database la spedizione dell'utente trasportatore libero all'ordine associato
              sprintf(sqlcmd, "INSERT INTO Spedizione (idSpedizione, idOrdine, nome_utente_trasportatore, statoSpedizione, nome_DittaSpedizione) VALUES (DEFAULT, '%d', '%s', '%s', '%s')", 
              idOrdine, nome_utente_trasportatore.c_str(), statoSpedizioneStr.c_str(), nome_ditta_spedizione.c_str());
              res = db1.ExecSQLcmd(sqlcmd);
              PQclear(res); 

              // Ottieni l'ID della spedizione appena inserito
              if (PQntuples(res) == 1) {
                spedizione.idSpedizione = atoi(PQgetvalue(res, 0, PQfnumber(res, "idSpedizione")));
              }


              // A questo punto dobbiamo modificare la disponibilità dell'utente trasportatore:
              sprintf(sqlcmd, "UPDATE UtenteTrasportatore set dispo = 1 WHERE nome_utente_trasportatore = '%s'", nome_utente_trasportatore.c_str());
              res = db1.ExecSQLcmd(sqlcmd);
              PQclear(res); 


              
              sprintf(sqlcmd, "UPDATE Ordine set statoOrdine ='spedito' WHERE idOrdine = '%d'", idOrdine);
              res = db1.ExecSQLcmd(sqlcmd);
              PQclear(res); 


              // Animiamo l'oggetto Spedizione:
              spedizione.idOrdine = idOrdine;
              spedizione.nome_utente_trasportatore = nome_utente_trasportatore;
              spedizione.stato_spedizione = stato_spedizione;
              spedizione.ditta_spedizione = nome_ditta_spedizione; 

            }
            else{
               std::cout << "L'utente non è stato trovato." << std::endl;
            return spedizione; 
            }
        }
        else{
          std::cout << "Nessun utente ha disponibilità per prendere in carico l'ordine." << std::endl;
          return spedizione; 
        }

        return spedizione;
    }



    std::string statoSpedizioneToString(StatoSpedizione stato) {
        switch (stato) {
            case StatoSpedizione::InTransito:
                return "in transito";
            case StatoSpedizione::Consegnato:
                return "consegnato";
            default:
                return ""; // gestione degli errori o valori non validi
        }
    }


};



#endif // ORDINE_H