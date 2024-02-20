// Spedizione.h
#ifndef SPEDIZIONE_H
#define SPEDIZIONE_H


#include "main.h"


enum class StatoSpedizione {
    InTransito,
    Consegnato
    };


class Spedizione{
public:
    int idSpedizione;
    int idOrdine;
    std::string nome_utente_trasportatore;
    StatoSpedizione stato_spedizione;
    std::string ditta_spedizione;

    // Definizione costruttori:
    Spedizione():
        idSpedizione(-1),
        idOrdine(-1),
        nome_utente_trasportatore(""),
        stato_spedizione(),
        ditta_spedizione("")
        {}


    Spedizione(int idSpedizione, int idOrdine, 
            std::string nome_utente_trasportatore, 
            StatoSpedizione stato_spedizione, 
            std::string ditta_spedizione):

            idSpedizione(idSpedizione),
            idOrdine(idOrdine),
            nome_utente_trasportatore(nome_utente_trasportatore),
            stato_spedizione(stato_spedizione),
            ditta_spedizione(ditta_spedizione){}



    void impostaStatoSpedizione(StatoSpedizione nuovstato){
        stato_spedizione = nuovstato;
    }


    // Un utente trasportatore con attibuto disponibilità=0 accede al sistema e controlla gli ordini che non sono ancora stati spediti e li prende in carico per la spedizione.
    Spedizione assegnaOrdineTrasportatore(Con2DB db1){

        Spedizione spedizione;

        StatoSpedizione stato_spedizione;
        int idOrdine;
        std::string sessionID = "";

        std::string nomeRequisito = "Assegna ordine a Utente trasportatore.";
        statoRequisito statoReq = statoRequisito::Wait;

        // Connession al database:
        //Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        //int idOrdine = identificatore_ordine;

        // Selezione utenti trasportatori con attributo disponibilità = 0;
        sprintf(sqlcmd, "SELECT nome_utente_trasportatore FROM UtenteTrasportatore WHERE dispo='0'");
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);
        std::string nome_utente_trasportatore;
        std::string nome_ditta_spedizione;

        stato_spedizione = StatoSpedizione::InTransito;
        std::string statoSpedizioneStr = statoSpedizioneToString(stato_spedizione);

        if (rows >= 1){
            nome_utente_trasportatore = PQgetvalue(res, 0, 0);

            // Caricamento del sessionID utile per il log.
            sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", nome_utente_trasportatore.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            PQclear(res);                        
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));}  

            // Selezione il nome della ditta di spedizione dell'utente trasportatore:
            sprintf(sqlcmd, "SELECT nome_DittaSpedizione FROM UtenteTrasportatore WHERE nome_utente_trasportatore='%s'", nome_utente_trasportatore.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            PQclear(res);
            if (rows==1){
              nome_ditta_spedizione = PQgetvalue(res, 0, 0);

              // Se ci sono utenti trasportatori liberi andiamo a prendere l'id dell'ordine della spedizione più vecchia:
              // 1. Selezioniamo gli ordini che non sono ancora stati spediti e prendiamo il primo tra quelli e lo assegniamo all'utente trasportatore.
                    // Abbiamo fatto questo perchè, se abbiamo 2 ordini e 1 solo utente trasportatore, una volta che viene aggiunto un nuovo utente trasportatore, andrà a selezionare l'ordine più vecchio 
                    // da spedire:
                    // Possibile situazione:
                    /*
                    Row 0: 1,   1,   marco1,   2024-02-10,   spedito,   Via della testa,   Roma,   45,   
                    Row 1: 3,   1,   marco1,   2024-02-10,   in elaborazione,   Via della testa,   Roma,   45,   
                    Row 2: 4,   1,   marco1,   2024-02-10,   in elaborazione,   Via della testa,   Roma,   45,   
                    Row 3: 2,   1,   marco1,   2024-02-10,   spedito,   Via della testa,   Roma,   45,   
                    Row 4: 5,   1,   marco1,   2024-02-10,   in elaborazione,   Via della testa,   Roma,   45,   


                    Tabella: Spedizione
                    Row 0: 1,   1,   _marco_,   in transito,   FedEx,   
                    Row 1: 2,   2,   _trasportatore2_,   in transito,   Bartolini,
                    */

                sprintf(sqlcmd, "SELECT idOrdine FROM Ordine WHERE statoOrdine='in elaborazione'");
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);
                PQclear(res);
                if (rows >= 1){
                    idOrdine = atoi(PQgetvalue(res, 0, 0));
                    // Inseriamo nel database la spedizione dell'utente trasportatore libero all'ordine associato

                    sprintf(sqlcmd, "INSERT INTO Spedizione (idSpedizione, idOrdine, nome_utente_trasportatore, statoSpedizione, nome_DittaSpedizione) VALUES (DEFAULT, '%d', '%s', '%s', '%s')", 
                    idOrdine, nome_utente_trasportatore.c_str(), statoSpedizioneStr.c_str(), nome_ditta_spedizione.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res); 


                    InsertToLogDB(db1, "INFO", "Assegnato ordine al trasportatore", sessionID, nomeRequisito, statoReq);

                    // A questo punto dobbiamo modificare la disponibilità dell'utente trasportatore:
                    sprintf(sqlcmd, "UPDATE UtenteTrasportatore set dispo='1' WHERE nome_utente_trasportatore = '%s'", nome_utente_trasportatore.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res); 

                    InsertToLogDB(db1, "INFO", "Modificata disponibilità utente trasportatore", sessionID, nomeRequisito, statoReq);


                    sprintf(sqlcmd, "UPDATE Ordine set statoOrdine ='spedito' WHERE idOrdine = '%d'", idOrdine);
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res); 

                    statoReq = statoRequisito::Success;

                    InsertToLogDB(db1, "INFO", "Avviata spedizione ordine", sessionID, nomeRequisito, statoReq);


                    // Animiamo l'oggetto Spedizione:
                    spedizione.idOrdine = idOrdine;
                    spedizione.nome_utente_trasportatore = nome_utente_trasportatore;
                    spedizione.stato_spedizione = stato_spedizione;
                    spedizione.ditta_spedizione = nome_ditta_spedizione; 

                }
                else{
                    std::cout << "Gli ordini sono tutti spediti." << std::endl;

                    statoReq = statoRequisito::NotSuccess;

                    InsertToLogDB(db1, "WARNING", "Ordini tutti spediti", sessionID, nomeRequisito, statoReq);

                    return spedizione; 
                }
            }
            else{
               std::cout << "L'utente non è stato trovato." << std::endl;

               statoReq = statoRequisito::NotSuccess;

              InsertToLogDB(db1, "WARNING", "Utente non trovato", sessionID, nomeRequisito, statoReq);

            return spedizione; 
            }
        }
        else{
          std::cout << "Nessun utente ha disponibilità per prendere in carico l'ordine." << std::endl;

          statoReq = statoRequisito::NotSuccess;

          InsertToLogDB(db1, "WARNING", "Nessun utente ha la disponibilità per prendere in carico l ordine", sessionID, nomeRequisito, statoReq);
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


    // Nell'implementazione di questo metodo l'utente trasportatore associato alla spedizione avvisa il sistema che ha completato la spedizione e consegnato il prodotto dell'ordine 
    void spedizioneConsegnata(Con2DB db1, int idSpedizione){
        // Connession al database:
        //Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        std::string sessionID = "";

        std::string nomeRequisito = "Consegna Spedizione.";
        statoRequisito statoReq = statoRequisito::Wait;


        // Aggiorno lo stato della spedizione nella tabella Spedizione:
        sprintf(sqlcmd, "UPDATE Spedizione set statoSpedizione='consegnato' WHERE idSpedizione = '%d'", idSpedizione);
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res); 

        statoReq = statoRequisito::Success;

        InsertToLogDB(db1, "INFO", "Ordine consegnato", sessionID, nomeRequisito, statoReq);


        // Aggiorno la disponibilità dell'utente Trasportatore che può effettuare una nuova consegna:
        // Prima selezioniamo il nome dell'utente Trasportatore:
        std::string nome_utente_trasportatore;
        sprintf(sqlcmd, "SELECT nome_utente_trasportatore FROM Spedizione WHERE idSpedizione = '%d'", idSpedizione);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);
        if (rows==1){
            nome_utente_trasportatore = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_trasportatore"));

            // Caricamento del sessionID utile per il log.
            sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", nome_utente_trasportatore.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            PQclear(res);                        
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));}  

            // Ora aggiorniamo la disponibilità dell'utente Trasportatore:
            sprintf(sqlcmd, "UPDATE UtenteTrasportatore set dispo='0' WHERE nome_utente_trasportatore = '%s'", nome_utente_trasportatore.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res); 

            nomeRequisito = "Utente Trasportatore liberato.";
            statoReq = statoRequisito::Success;

            InsertToLogDB(db1, "INFO", "Disponibilità utente trasportatore per prendere in consegna un nuovo pacco", sessionID, nomeRequisito, statoReq);
        }
        else{
            std::cout << "Nessun utente trasportatore associato alla spedizione! " << std::endl;
            
            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "WARNING", "Nessun utente trasportatore associato alla spedizione.", sessionID, nomeRequisito, statoReq);
            return;
        }
    std::cout << "Spedizione " << idSpedizione <<  " consegnata! " << std::endl;
    return;
    }

};




#endif // SPEDIZIONE_H