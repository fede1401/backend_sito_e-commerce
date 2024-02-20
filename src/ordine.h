// Ordine.h
#ifndef ORDINE_H
#define ORDINE_H

//#include "main.h"


enum class StatoOrdine {
    InElaborazione,
    Spedito,
    Annullato
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

      std::string nomeRequisito = "Visione ordini effettuati.";
      statoRequisito statoReq = statoRequisito::Wait;

      // Caricamento del sessionID utile per il log.
      std::string sessionID = "";
      sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nome_utente_compratore.c_str());
      res = db1.ExecSQLtuples(sqlcmd);
      rows = PQntuples(res);
                        
      if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));}  


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

      statoReq = statoRequisito::Success;

      InsertToLogDB("INFO", "Visione degli ordini da parte dell utente.", sessionID, nomeRequisito, statoReq);
      return;
    }



    void annulla_ordine(int idOrdine){
      // Connession al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        std::string nomeRequisito = "Annullamento ordine.";
        statoRequisito statoReq = statoRequisito::Wait;


        std::string stato_ordine;
        StatoOrdine stato_ordine_annullato;

        std::string nome_utente_compratore;

        std::string sessionID = "";

        // Seleziono il nome dell'utente compratore che ha effettuato l'ordine:
        sprintf(sqlcmd, "SELECT nome_utente_compratore FROM Ordine WHERE idOrdine = '%d'", idOrdine);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows == 1){
            nome_utente_compratore = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_compratore"));
        }
        else{
          statoReq = statoRequisito::NotSuccess;

          InsertToLogDB("ERROR", "Nessune utente compratore ha effettuato l ordine da annullare.", sessionID, nomeRequisito, statoReq);

          std::cout << "Nessun utente compratore ha effettuato l'ordine da annullare!" << std::endl;
          return;
        }

        // A questo punto possiamo selezionare il sessionId:
        // Caricamento del sessionID utile per il log.
        sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
                        
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));}  


        // Devo controllare se l'ordine è stato spedito o meno:
          // Se l'ordine non è stato spedito, allora può essere annullato;
          // Se, invece, l'ordine è stato spedito, allora NON può essere annullato;
          sprintf(sqlcmd, "SELECT statoOrdine FROM Ordine WHERE idOrdine = '%d'", idOrdine);
          res = db1.ExecSQLtuples(sqlcmd);
          rows = PQntuples(res);

          if (rows==1){
              stato_ordine = PQgetvalue(res, 0, PQfnumber(res, "statoOrdine"));


              if (stato_ordine == "in elaborazione"){
                  // Allora l'ordine può essere annullato.
                  
                  stato_ordine_annullato = StatoOrdine::Annullato;
                  std::string stato_ordine_annullato_str = statoOrdineToString(stato_ordine_annullato);

                  // Modifichiamo lo stato dell'ordine con id = idOrdine:
                  sprintf(sqlcmd, "UPDATE Ordine set statoOrdine='%s' WHERE idOrdine='%d'", stato_ordine_annullato_str.c_str(), idOrdine);
                  res = db1.ExecSQLcmd(sqlcmd);
                  PQclear(res); 

                  statoReq = statoRequisito::Success;

                  InsertToLogDB("INFO", "Ordine annullato da parte dell utente Compratore.", sessionID, nomeRequisito, statoReq);

              }
              else{
                  std::cout << "L'ordine è stato spedito, perciò l'ordine non può essere annullato! All'arrivo del pacco potrai effettuare il reso" << std::endl;

                  statoReq = statoRequisito::NotSuccess;
                  InsertToLogDB("WARNING", "Ordine non annullabile perchè già spedito.", sessionID, nomeRequisito, statoReq);
                  return;
              }
              
            }
            else{
                std::cout << "L'ordine non è stato trovato" << std::endl;

                statoReq = statoRequisito::NotSuccess;
                InsertToLogDB("ERROR", "Ordine trovato.", sessionID, nomeRequisito, statoReq);

                return;
            }  
    return;
    }
        
    


    std::string statoOrdineToString(StatoOrdine stato) {
        switch (stato) {
            case StatoOrdine::InElaborazione:
                return "in elaborazione";
            case StatoOrdine::Spedito:
                return "spedito";
            case StatoOrdine::Annullato:
                return "annullato";
            default:
                return ""; // gestione degli errori o valori non validi
        }
    }


};



#endif // ORDINE_H