// Ordine.h
#ifndef ORDINE_H
#define ORDINE_H

//#include "main.h"
#include "../../shared-server/statoOrdineToString.h"



class Ordine {
public:
    // Attributi per la classe Ordine.
    int identificatore_ordine;
    int codice_prodotto;
    std::string nome_uteCompratore;
    std::string data_ordine_effettuato;
    StatoOrdine stato_ordine;
    std::string via_spedizione;
    std::string città_spedizione;
    std::string numero_civico_spedizione;
    std::string CAP_spedizione;


    // Definizione costruttori:  
    Ordine():
      identificatore_ordine(-1),
      codice_prodotto(-1),
      nome_uteCompratore(""),
      data_ordine_effettuato(""),
      stato_ordine(),
      via_spedizione(""),
      città_spedizione(""),
      numero_civico_spedizione(""),
      CAP_spedizione(""){}
          
    Ordine( int identificatore_ordine,  int codice_prodotto, std::string nome_uteCompratore, std::string data_ordine, StatoOrdine stato_ordine,
    std::string via_spedizione, std::string città_spedizione, std::string numero_civico_spedizione, std::string CAP_spedizione ): 
          
          identificatore_ordine(identificatore_ordine),
          codice_prodotto(codice_prodotto), 
          nome_uteCompratore(nome_uteCompratore),           
          data_ordine_effettuato(data_ordine), 
          stato_ordine(stato_ordine),
          via_spedizione(via_spedizione),
          città_spedizione(città_spedizione),
          numero_civico_spedizione(numero_civico_spedizione),
          CAP_spedizione(CAP_spedizione) {}  


    void impostaStato(StatoOrdine nuovoStato) {   
      stato_ordine = nuovoStato;    
    }


    // Metodo utilizzato per visualizzare gli ordini effettuati dall'utente compratore preso in input.
    void visione_ordini_effettuati(Con2DB db1, std::string nome_utente_compratore)
    {

      // Definizione di alcune variabili per il logging
      std::string nomeRequisito = "Visione ordini effettuati.";
      statoRequisito statoReq = statoRequisito::Wait;
      std::string messageLog = "";


      // Caricamento del sessionID.
      std::string sessionID = "";
      sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", nome_utente_compratore.c_str());
      res = db1.ExecSQLtuples(sqlcmd);
      rows = PQntuples(res);
      // Se il numero di righe del risultato della query è 1, allora possiamo recuperare il sessionID
      if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id"));}  
      PQclear(res);   

      if (rows != 1){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste " + nome_utente_compratore + " , poichè non è stato registrato, non può visionare gli ordini .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            return;
        }               

      // Verifica se l'utente è loggato e ha una sessionID valida
      if (sessionID == ""){
          // Log dell'errore e uscita dalla funzione
          statoReq = statoRequisito::NotSuccess;
          messageLog = "Non esiste una sessionID per " + nome_utente_compratore + ", utente non loggato, non possono essere visionati gli ordini effettuati.";
          InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
          return;
      }


      // Verifichiamo che l'utente si tratti di un utente compratore:
        std::string categoriaUtente = "";
        sprintf(sqlcmd, "SELECT categoriaUtente FROM Utente WHERE nome_utente = '%s'", nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare la categoria dell'utente.
        if (rows==1){ categoriaUtente = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));}  
        PQclear(res);

        if (categoriaUtente != "UtenteCompratore"){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "L utente " + nome_utente_compratore + " non è un utente compratore, perciò non possono essere visionati gli ordini effettuati.";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            return;
        }


      // Carichiamo tutti gli ordini effettuati dall'utente compratore preso in input dal metodo
      sprintf(sqlcmd, "SELECT * FROM Ordine WHERE nome_utente_compratore='%s'", nome_utente_compratore.c_str());
      res = db1.ExecSQLtuples(sqlcmd);
      rows = PQntuples(res); // Numero di righe restituite dalla query

      // Scorrimento dei risultati e visualizzazione degli ordini
      for (int i = 0; i < rows; ++i) {
        this->identificatore_ordine = atoi(PQgetvalue(res, i, PQfnumber(res, "idOrdine")));
        this->codice_prodotto = atoi(PQgetvalue(res, i, PQfnumber(res, "codProdotto")));
        this->nome_uteCompratore = PQgetvalue(res, i, PQfnumber(res, "nome_utente_compratore"));
        this->data_ordine_effettuato =  PQgetvalue(res, i, PQfnumber(res, "dataOrdineEffettuato"));
        std::string statoOrdine = PQgetvalue(res, i, PQfnumber(res, "statoOrdine"));
        
        if (statoOrdine.c_str() == "in elaborazione"){
            this->stato_ordine = StatoOrdine::InElaborazione;
        }

        if (statoOrdine.c_str() == "spedito"){
            this->stato_ordine = StatoOrdine::Spedito;
        }

        if (statoOrdine.c_str() == "annullato"){
            this->stato_ordine = StatoOrdine::Annullato;
        }

        this->via_spedizione = PQgetvalue(res, i, PQfnumber(res, "viaSpedizione"));
        this->città_spedizione = PQgetvalue(res, i, PQfnumber(res, "cittaSpedizione"));
        this->numero_civico_spedizione = PQgetvalue(res, i, PQfnumber(res, "numCivSpedizione"));
        this->CAP_spedizione = PQgetvalue(res, i, PQfnumber(res, "CAPSpedizione"));
      }
      PQclear(res);

      // Log
      statoReq = statoRequisito::Success;
      messageLog = "Visione degli ordini da parte dell utente " + nome_utente_compratore;
      InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

      return;
    }


    // Metodo utilizzato per annullare un ordine di un utente compratore tramite l'id dell'ordine.
    void annulla_ordine(Con2DB db1, std::string in_nome_utenteCompratore, int idOrdine)
    {
        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Annullamento ordine.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Variabili per memorizzare informazioni sull'ordine e sull'utente
        std::string stato_ordine;
        StatoOrdine stato_ordine_annullato;
        std::string nome_utente_compratore;
        std::string sessionID = "";


        // Caricamento del sessionID.
        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utenteCompratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare il sessionID
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id"));}  
        PQclear(res);   

        if (rows != 1){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste " + in_nome_utenteCompratore + " , poichè non è stato registrato, non può essere annullato l ordine .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            return;
        }               


        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == ""){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + in_nome_utenteCompratore + ", utente non loggato, non può essere annullato l ordine.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            return;
        }

        // Verifichiamo che l'utente si tratti di un utente compratore:
        std::string categoriaUtente = "";
        sprintf(sqlcmd, "SELECT categoriaUtente FROM Utente WHERE nome_utente = '%s'", in_nome_utenteCompratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare la categoria dell'utente.
        if (rows==1){ categoriaUtente = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));}  
        PQclear(res);

        if (categoriaUtente != "UtenteCompratore"){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "L utente " + in_nome_utenteCompratore + " non è un utente compratore, perciò non può essere annullato l ordine";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            return;
        }


        // Recupero del nome dell'utente compratore che ha effettuato l'ordine tramite l'id dell'ordine:
        sprintf(sqlcmd, "SELECT nome_utente_compratore FROM Ordine WHERE idOrdine = '%d'", idOrdine);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe ottenute dai risultati della query è uguale a 1, l'utente esiste
        if (rows == 1){
            nome_utente_compratore = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_compratore"));
            PQclear(res);
        }
        
        // Se il numero di righe è diverso da 1, l'utente non esiste.
        else{
          PQclear(res);

          // Log
          statoReq = statoRequisito::NotSuccess;
          InsertToLogDB(db1, "ERROR", "Nessune utente compratore ha effettuato l ordine da annullare.", sessionID, nomeRequisito, statoReq);

          std::cout << "Nessun utente compratore ha effettuato l'ordine da annullare!" << std::endl;
          return;
        }

        // Confrontiamo il nome dell'utente compratore che vuole annullare l'ordine con l'utente associato all'id dell'ordine.
        if (in_nome_utenteCompratore != nome_utente_compratore){

          // Log
          messageLog = "L utente compratore di chi ha effettuato l ordine " + nome_utente_compratore + " non corrisponde a chi vuole annullare ordine: " + in_nome_utenteCompratore;
          InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

          std::cout << "L'utente compratore dell'ordine non corrisponde a chi vuole annullare l'ordine" << std::endl;
          return;
        }

    
        // Devo controllare se l'ordine è stato spedito o meno:
          // Se l'ordine non è stato spedito, allora può essere annullato;
          // Se, invece, l'ordine è stato spedito, allora NON può essere annullato;

          // Recupero dello stato dell'ordine:
          sprintf(sqlcmd, "SELECT statoOrdine FROM Ordine WHERE idOrdine = '%d'", idOrdine);
          res = db1.ExecSQLtuples(sqlcmd);
          rows = PQntuples(res);
          
          if (rows==1){
              stato_ordine = PQgetvalue(res, 0, PQfnumber(res, "statoOrdine"));
              PQclear(res);

              // Se lo stato dell'ordine è "in elaborazione" l'ordine può essere annullato.
              if (stato_ordine == "in elaborazione"){
                  
                  stato_ordine_annullato = StatoOrdine::Annullato;
                  std::string stato_ordine_annullato_str = statoOrdineToString(stato_ordine_annullato);

                  // Modifichiamo lo stato dell'ordine a "Annullato"
                  sprintf(sqlcmd, "UPDATE Ordine set statoOrdine='%s' WHERE idOrdine='%d'", stato_ordine_annullato_str.c_str(), idOrdine);
                  res = db1.ExecSQLcmd(sqlcmd);
                  PQclear(res); 

                  // Log
                  statoReq = statoRequisito::Success;
                  messageLog = "Ordine con codice "+ std::to_string(idOrdine) +" annullato da parte dell utente Compratore " + in_nome_utenteCompratore;
                  InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

              }

              // Se invece lo stato dell'ordine è "spedito" oppure "annullato", allora non si può annullare
              else{
                  std::cout << "L'ordine è stato spedito o già annullato, perciò l'ordine non può essere annullato! All'arrivo del pacco potrai effettuare il reso" << std::endl;

                  // Log
                  statoReq = statoRequisito::NotSuccess;
                  messageLog = "Ordine con codice "+ std::to_string(idOrdine) +" non annullabile perchè già spedito o già annullato. ";
                  InsertToLogDB(db1, "WARNING", messageLog, sessionID, nomeRequisito, statoReq);
                  return;
              }
              
            }
            // Il numero di righe dello stato dell'ordine non è uguale a 1, perciò l'ordine non è stato trovato
            else{
                std::cout << "L'ordine non è stato trovato" << std::endl;

                // Log
                statoReq = statoRequisito::NotSuccess;
                messageLog = "Ordine con codice "+ std::to_string(idOrdine) +" non trovato. ";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
                return;
            }  
    return;
    }
    


};



#endif // ORDINE_H