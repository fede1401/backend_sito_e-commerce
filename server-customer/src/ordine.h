// Ordine.h
#ifndef ORDINE_H
#define ORDINE_H

//#include "main.h"
#include "../../shared-server/statoOrdineToString.h"



class Ordine {
public:
    // Attributi per la classe Ordine.
    int m_idOrdine;
    int m_codiceProdotto;
    std::string m_nomeUtenteCompratore;
    std::string m_DataOrdineEffettuato;
    StatoOrdine m_statoOrdine;
    std::string m_viaSpedizione;
    std::string m_cittàSpedizione;
    std::string m_numeroCivicoSpedizione;
    std::string m_CAPSpedizione;


    // Definizione costruttori:  
    Ordine():
      m_idOrdine(-1),
      m_codiceProdotto(-1),
      m_nomeUtenteCompratore(""),
      m_DataOrdineEffettuato(""),
      m_statoOrdine(),
      m_viaSpedizione(""), m_cittàSpedizione(""), m_numeroCivicoSpedizione(""), m_CAPSpedizione(""){}
          
    Ordine( int id_ordine,  int codice_prodotto, std::string nome_utente_compratore, std::string data_ordine, StatoOrdine stato_ordine,
    std::string via_spedizione, std::string città_spedizione, std::string numero_civico_spedizione, std::string CAP_spedizione ): 
          
          m_idOrdine(id_ordine),
          m_codiceProdotto(codice_prodotto), 
          m_nomeUtenteCompratore(nome_utente_compratore),           
          m_DataOrdineEffettuato(data_ordine), 
          m_statoOrdine(stato_ordine),
          m_viaSpedizione(via_spedizione), m_cittàSpedizione(città_spedizione), m_numeroCivicoSpedizione(numero_civico_spedizione), m_CAPSpedizione(CAP_spedizione) {}  


    void impostaStato(StatoOrdine nuovoStato) {   
      m_statoOrdine = nuovoStato;    
    }


    // Metodo utilizzato per visualizzare gli ordini effettuati dall'utente compratore preso in input.
    std::string visione_ordini_effettuati(Con2DB db1, std::string in_nome_utente_compratore)
    {

      // Definizione di alcune variabili per il logging
      std::string nomeRequisito = "Visione ordini effettuati.";
      statoRequisito statoReq = statoRequisito::Wait;
      std::string messageLog = "";

      // Dichiarazione variabile per il risultato dell'operazione.
      std::string result = "";


      // Caricamento del sessionID.
      std::string sessionID = "";
      sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente_compratore.c_str());
      res = db1.ExecSQLtuples(sqlcmd);
      rows = PQntuples(res);
      // Se il numero di righe del risultato della query è 1, allora possiamo recuperare il sessionID
      if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id"));}  
      PQclear(res);  
       
      // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
      if (rows == 0){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste " + in_nome_utente_compratore + " , poichè non è stato registrato, non può visionare gli ordini .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
      }  

      // Verifica se l'utente è loggato e ha una sessionID valida
      if (sessionID == ""){
          // Log dell'errore e uscita dalla funzione
          statoReq = statoRequisito::NotSuccess;
          messageLog = "Non esiste una sessionID per " + in_nome_utente_compratore + ", utente non loggato, non possono essere visionati gli ordini effettuati.";
          InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
          
          result = messageLog;
          return result;
      }


      // Verifichiamo che l'utente si tratti di un utente compratore:
        std::string categoriaUtente = "";
        sprintf(sqlcmd, "SELECT categoriaUtente FROM Utente WHERE nome_utente = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare la categoria dell'utente.
        if (rows==1){ categoriaUtente = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));}  
        PQclear(res);

        if (categoriaUtente != "UtenteCompratore"){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "L utente " + in_nome_utente_compratore + " non è un utente compratore, perciò non possono essere visionati gli ordini effettuati.";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }


      // Carichiamo tutti gli ordini effettuati dall'utente compratore preso in input dal metodo
      sprintf(sqlcmd, "SELECT * FROM Ordine WHERE nome_utente_compratore='%s'", in_nome_utente_compratore.c_str());
      res = db1.ExecSQLtuples(sqlcmd);
      rows = PQntuples(res); // Numero di righe restituite dalla query

      // Scorrimento dei risultati e visualizzazione degli ordini
      for (int i = 0; i < rows; ++i) {
        this->m_idOrdine = atoi(PQgetvalue(res, i, PQfnumber(res, "idOrdine")));
        this->m_codiceProdotto = atoi(PQgetvalue(res, i, PQfnumber(res, "codProdotto")));
        this->m_nomeUtenteCompratore = PQgetvalue(res, i, PQfnumber(res, "nome_utente_compratore"));
        this->m_DataOrdineEffettuato =  PQgetvalue(res, i, PQfnumber(res, "dataOrdineEffettuato"));
        std::string statoOrdine = PQgetvalue(res, i, PQfnumber(res, "statoOrdine"));
        
        if (strcmp(statoOrdine.c_str(), "in elaborazione") == 0){
            this->m_statoOrdine = StatoOrdine::InElaborazione;
        }

        if (strcmp(statoOrdine.c_str(), "spedito") == 0){
            this->m_statoOrdine = StatoOrdine::Spedito;
        }

        if (strcmp(statoOrdine.c_str(), "annullato") == 0) {
            this->m_statoOrdine = StatoOrdine::Annullato;
        }

        this->m_viaSpedizione = PQgetvalue(res, i, PQfnumber(res, "viaSpedizione"));
        this->m_cittàSpedizione = PQgetvalue(res, i, PQfnumber(res, "cittaSpedizione"));
        this->m_numeroCivicoSpedizione = PQgetvalue(res, i, PQfnumber(res, "numCivSpedizione"));
        this->m_CAPSpedizione = PQgetvalue(res, i, PQfnumber(res, "CAPSpedizione"));
      }
      PQclear(res);

      // Log
      statoReq = statoRequisito::Success;
      messageLog = "Visione degli ordini da parte dell utente " + in_nome_utente_compratore;
      InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

      result = messageLog;
      return result;
    }


    // Metodo utilizzato per annullare un ordine di un utente compratore tramite l'id dell'ordine.
    std::string annulla_ordine(Con2DB db1, std::string in_nome_utente_compratore, int in_id_ordine)
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

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        // Caricamento del sessionID.
        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare il sessionID
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id"));}  
        PQclear(res); 
      
        // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
        if (rows == 0){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste " + in_nome_utente_compratore + " , poichè non è stato registrato, non può essere annullato l ordine .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }    


        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == ""){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + in_nome_utente_compratore + ", utente non loggato, non può essere annullato l ordine.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifichiamo che l'utente si tratti di un utente compratore:
        std::string categoriaUtente = "";
        sprintf(sqlcmd, "SELECT categoriaUtente FROM Utente WHERE nome_utente = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare la categoria dell'utente.
        if (rows==1){ categoriaUtente = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));}  
        PQclear(res);

        if (categoriaUtente != "UtenteCompratore"){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "L utente " + in_nome_utente_compratore + " non è un utente compratore, perciò non può essere annullato l ordine";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }


        // Recupero del nome dell'utente compratore che ha effettuato l'ordine tramite l'id dell'ordine:
        sprintf(sqlcmd, "SELECT nome_utente_compratore FROM Ordine WHERE idOrdine = '%d'", in_id_ordine);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe ottenute dai risultati della query è uguale a 1, l'utente esiste
        if (rows == 1){
            nome_utente_compratore = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_compratore"));
            PQclear(res);
        }
        
        // Se il numero di righe è 0, l'utente non esiste.
        if (rows == 0){
          PQclear(res);

          // Log
          statoReq = statoRequisito::NotSuccess;
          messageLog = "Nessune utente compratore ha effettuato l ordine da annullare.";
          InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

          std::cout << "Nessun utente compratore ha effettuato l'ordine da annullare!" << std::endl;
          
          result = messageLog;
          return result;
        }

        // Confrontiamo il nome dell'utente compratore che vuole annullare l'ordine con l'utente associato all'id dell'ordine.
        if (in_nome_utente_compratore != nome_utente_compratore){

          // Log
          messageLog = "L utente compratore di chi ha effettuato l ordine " + nome_utente_compratore + " non corrisponde a chi vuole annullare ordine: " + in_nome_utente_compratore;
          InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

          std::cout << "L'utente compratore dell'ordine non corrisponde a chi vuole annullare l'ordine" << std::endl;
          
          result = messageLog;
          return result;
        }

    
        // Devo controllare se l'ordine è stato spedito o meno:
          // Se l'ordine non è stato spedito, allora può essere annullato;
          // Se, invece, l'ordine è stato spedito, allora NON può essere annullato;

          // Recupero dello stato dell'ordine:
          sprintf(sqlcmd, "SELECT statoOrdine FROM Ordine WHERE idOrdine = '%d'", in_id_ordine);
          res = db1.ExecSQLtuples(sqlcmd);
          rows = PQntuples(res);
          
          // Se il numero di righe del risultato della query è 1, possiamo recuperare lo stato dell'ordine con l'id specificato.
          if (rows==1)
          {
              stato_ordine = PQgetvalue(res, 0, PQfnumber(res, "statoOrdine"));
              PQclear(res);

              // Se lo stato dell'ordine è "in elaborazione" l'ordine può essere annullato.
              if (stato_ordine == "in elaborazione"){
                  
                  stato_ordine_annullato = StatoOrdine::Annullato;
                  std::string stato_ordine_annullato_str = statoOrdineToString(stato_ordine_annullato);

                  // Modifichiamo lo stato dell'ordine a "Annullato"
                  sprintf(sqlcmd, "UPDATE Ordine set statoOrdine='%s' WHERE idOrdine='%d'", stato_ordine_annullato_str.c_str(), in_id_ordine);
                  res = db1.ExecSQLcmd(sqlcmd);
                  PQclear(res); 

                  // Log
                  statoReq = statoRequisito::Success;
                  messageLog = "Ordine con codice "+ std::to_string(in_id_ordine) +" annullato da parte dell utente Compratore " + in_nome_utente_compratore;
                  InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

              }

              // Se invece lo stato dell'ordine è "spedito" oppure "annullato", allora non si può annullare
              else{
                  std::cout << "L'ordine è stato spedito o già annullato, perciò l'ordine non può essere annullato! All'arrivo del pacco potrai effettuare il reso" << std::endl;

                  // Log
                  statoReq = statoRequisito::NotSuccess;
                  messageLog = "Ordine con codice "+ std::to_string(in_id_ordine) +" non annullabile perchè già spedito o già annullato. ";
                  InsertToLogDB(db1, "WARNING", messageLog, sessionID, nomeRequisito, statoReq);
                  
                  result = messageLog;
                  return result;
              }
              
          }
          // Il numero di righe dello stato dell'ordine è 0, perciò l'ordine non è stato trovato.
          if (rows == 0){
            std::cout << "L'ordine non è stato trovato" << std::endl;

            // Log
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Ordine con codice "+ std::to_string(in_id_ordine) +" non trovato. ";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
           return result;
          }  
          
    result = messageLog;
    return result;
    }
    


};



#endif // ORDINE_H