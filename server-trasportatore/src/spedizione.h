// Spedizione.h
#ifndef SPEDIZIONE_H
#define SPEDIZIONE_H

#include "main.h"
#include "../../shared-server/statoSpedizioneToString.h"




class Spedizione
{
public:
    int idSpedizione;
    int idOrdine;
    std::string nome_utente_trasportatore;
    StatoSpedizione stato_spedizione;
    std::string ditta_spedizione;

    // Definizione costruttori:
    Spedizione() : idSpedizione(-1),
                   idOrdine(-1),
                   nome_utente_trasportatore(""),
                   stato_spedizione(),
                   ditta_spedizione(""){}

    Spedizione(int idSpedizione, int idOrdine,
               std::string nome_utente_trasportatore,
               StatoSpedizione stato_spedizione,
               std::string ditta_spedizione) :

                                               idSpedizione(idSpedizione),
                                               idOrdine(idOrdine),
                                               nome_utente_trasportatore(nome_utente_trasportatore),
                                               stato_spedizione(stato_spedizione),
                                               ditta_spedizione(ditta_spedizione)
    {}

    void impostaStatoSpedizione(StatoSpedizione nuovstato)
    {
        stato_spedizione = nuovstato;
    }

    // Funzione che permette di assegnare un ordine che ha lo stato "in elaborazione" a un utente trasportatore che ha come attibuto "disponibilità"=0 .
    Spedizione assegnaOrdineTrasportatore(Con2DB db1)
    {
        Spedizione spedizione;

        StatoSpedizione stato_spedizione;
        int idOrdine;
        std::string sessionID = "";


        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Assegna ordine a Utente trasportatore.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";


        // Recupero di tutti gli utenti trasportatori con attributo "disponibilità" = 0, cioè che non sono impegnati in alcuna spedizione.
        sprintf(sqlcmd, "SELECT nome_utente_trasportatore FROM UtenteTrasportatore WHERE dispo='0'");
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        std::string nome_utente_trasportatore;
        std::string nome_ditta_spedizione;

        stato_spedizione = StatoSpedizione::InTransito;
        std::string statoSpedizioneStr = statoSpedizioneToString(stato_spedizione);


        // Se il numero di righe del risultato della query è maggiore o uguale a 1, cioè ci sono utenti trasportatori con attributo "disponibilità" = 0:
        if (rows >= 1)
        {
            nome_utente_trasportatore = PQgetvalue(res, 0, 0);
            PQclear(res);

            // Caricamento del sessionID.
            sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", nome_utente_trasportatore.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows == 1) { sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t")); }
            PQclear(res);

            // Verifica se l'utente è loggato e ha una sessionID valida
            if (sessionID == "")
            {
                // Log dell'errore e uscita dalla funzione
                InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non si può assegnare ordine al trasportatore .", sessionID, nomeRequisito, statoReq);
                return;
            }

            // Recupero del nome della ditta di spedizione dell'utente trasportatore:
            sprintf(sqlcmd, "SELECT nome_DittaSpedizione FROM UtenteTrasportatore WHERE nome_utente_trasportatore='%s'", nome_utente_trasportatore.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            
            // Se il numero di righe del risultato della query è uguale di 1, allora c'è una ditta di spdedizione associata all'utente
            if (rows == 1)
            {
                nome_ditta_spedizione = PQgetvalue(res, 0, 0);
                PQclear(res);

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

                // Recupero degli id degli ordini che hanno come attributo "statoOrdine" = elaborazione.
                sprintf(sqlcmd, "SELECT idOrdine FROM Ordine WHERE statoOrdine='in elaborazione'");
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);

                // Se il numero di righe del risultato della query è maggiore o uguale di 1, cioè ci sono ordini con lo stato "in elaborazione" allora possiamo creare delle spedizione
                if (rows >= 1)
                {
                    idOrdine = atoi(PQgetvalue(res, 0, 0));
                    // Inseriamo nel database la spedizione dell'utente trasportatore libero all'ordine associato
                    PQclear(res);

                    // Inserisco nel database una riga corrispondente alla spedizione effettuata dall'utente trasportatore libero (con disponibilità 0) e con un ordine "in elaborazione":
                    sprintf(sqlcmd, "INSERT INTO Spedizione (idSpedizione, idOrdine, nome_utente_trasportatore, statoSpedizione, nome_DittaSpedizione) VALUES (DEFAULT, '%d', '%s', '%s', '%s')",
                            idOrdine, nome_utente_trasportatore.c_str(), statoSpedizioneStr.c_str(), nome_ditta_spedizione.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Log
                    messageLog = "Assegnato ordine al trasportatore " + nome_utente_trasportatore;
                    InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

                    // Una volta assegnata la spedizione all'utente trasportatore dobbiamo modificare la disponibilità di quest'ultimo.
                    sprintf(sqlcmd, "UPDATE UtenteTrasportatore set dispo='1' WHERE nome_utente_trasportatore = '%s'", nome_utente_trasportatore.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Log
                    InsertToLogDB(db1, "INFO", "Modificata disponibilità utente trasportatore", sessionID, nomeRequisito, statoReq);

                    // Inoltre dobbiamo rendere lo stato dell'ordine "spedito", poichè è stato spedito.
                    sprintf(sqlcmd, "UPDATE Ordine set statoOrdine ='spedito' WHERE idOrdine = '%d'", idOrdine);
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Log
                    statoReq = statoRequisito::Success;
                    InsertToLogDB(db1, "INFO", "Avviata spedizione ordine", sessionID, nomeRequisito, statoReq);

                    // Animiamo l'oggetto Spedizione:
                    spedizione.idOrdine = idOrdine;
                    spedizione.nome_utente_trasportatore = nome_utente_trasportatore;
                    spedizione.stato_spedizione = stato_spedizione;
                    spedizione.ditta_spedizione = nome_ditta_spedizione;
                }

                // Se il numero di righe del risultato della query è < 1 allora non esistono ordini con stato "in elaborazione"
                else
                {
                    // Log dell'errore e uscita dalla funzione
                    std::cout << "Gli ordini sono tutti spediti." << std::endl;

                    statoReq = statoRequisito::NotSuccess;
                    InsertToLogDB(db1, "WARNING", "Ordini tutti spediti", sessionID, nomeRequisito, statoReq);

                    return spedizione;
                }
            }

            // Se il numero di righe del risultato della query è diverso da 1, allora NON c'è una ditta di spdedizione associata all'utente
            else
            {
                std::cout << "L'utente non è stato trovato." << std::endl;

                statoReq = statoRequisito::NotSuccess;

                InsertToLogDB(db1, "WARNING", "Utente non trovato", sessionID, nomeRequisito, statoReq);

                return spedizione;
            }
        }

        // Se il numero di righe del risultato della query è < 1, NON ci sono utenti trasportatori con attributo "disponibilità" = 0, perciò non possono essere create spedizioni.
        else
        {
            std::cout << "Nessun utente ha disponibilità per prendere in carico l'ordine." << std::endl;

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1, "WARNING", "Nessun utente ha la disponibilità per prendere in carico l ordine", sessionID, nomeRequisito, statoReq);
            return spedizione;
        }

        return spedizione;
    }



    // Nell'implementazione di questo metodo l'utente trasportatore associato alla spedizione avvisa il sistema che ha completato la spedizione e consegnato il prodotto dell'ordine.
    void spedizioneConsegnata(Con2DB db1, std::string in_nome_utente_trasportatore, int idSpedizione)
    {
        std::string sessionID = "";


        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Consegna Spedizione.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";


        // Verifico che l'id della spedizione esista effettuando una query.
        sprintf(sqlcmd, "SELECT * FROM Spedizione WHERE idSpedizione = '%d'", idSpedizione);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe del risultato della query è < di 1, allora l'id non esiste, errore
        if (rows < 1){
            
            // Log dell'errore e uscita dalla funzione
            messageLog = "Non esiste id " + std::to_string(idSpedizione) + " della spedizione";
            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
            return;
        }

        // Aggiorno lo stato della spedizione nella tabella Spedizione:
        sprintf(sqlcmd, "UPDATE Spedizione set statoSpedizione='consegnato' WHERE idSpedizione = '%d'", idSpedizione);
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Log
        statoReq = statoRequisito::Success;
        messageLog = "Ordine consegnato da " + in_nome_utente_trasportatore;
        InsertToLogDB(db1, "INFO", "Ordine consegnato", sessionID, nomeRequisito, statoReq);


        // Aggiorno la disponibilità dell'utente Trasportatore , il quale può effettuare una nuova consegna:
        
        // Recupero il nome dell'utente trasportatore associato all'id della spedizione
        std::string nome_utente_trasportatore;
        sprintf(sqlcmd, "SELECT nome_utente_trasportatore FROM Spedizione WHERE idSpedizione = '%d'", idSpedizione);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe del risultato della query è uguale a 1, allora esiste l'utente trasportatore associato alla spedizione
        if (rows == 1)
        {
            nome_utente_trasportatore = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_trasportatore"));
            PQclear(res);

            // Controlliamo se l'utente trasportatore corrisponde a quello che vuole avvisare che la spedizione è consegnata:
            if (in_nome_utente_trasportatore != nome_utente_trasportatore)
            {
                // Log dell'errore e uscita dalla funzione
                InsertToLogDB(db1, "ERROR", "Utente che sta cercando di eliminare la recensione non corrisponde a quello che vuole avvisare che la spedizione è consegnata", sessionID, nomeRequisito, statoReq);
                return;
            }

            // Caricamento del sessionID.
            sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", nome_utente_trasportatore.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows == 1) { sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t")); }
            PQclear(res);

            // Verifica se l'utente è loggato e ha una sessionID valida
            if (sessionID == "")
            {
                // Log dell'errore e uscita dalla funzione
                InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non si può avvisare che la spedizione è consegnata .", sessionID, nomeRequisito, statoReq);
                return;
            }

            // A questo punto l'utente trasportatore può prendere nuove consegne, perciò aggiorniamo la sua disponibilità.
            sprintf(sqlcmd, "UPDATE UtenteTrasportatore set dispo='0' WHERE nome_utente_trasportatore = '%s'", nome_utente_trasportatore.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            nomeRequisito = "Utente Trasportatore liberato.";
            statoReq = statoRequisito::Success;
            messageLog = "Disponibilità utente trasportatore " + in_nome_utente_trasportatore + "per prendere in consegna un nuovo pacco ";
            InsertToLogDB(db1, "INFO", "Disponibilità utente trasportatore per prendere in consegna un nuovo pacco", sessionID, nomeRequisito, statoReq);
        }

        // Se il numero di righe del risultato della query è diverso da 1, allora NON esiste l'utente trasportatore associato alla spedizione.
        else
        {
            std::cout << "Nessun utente trasportatore associato alla spedizione! " << std::endl;

            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "WARNING", "Nessun utente trasportatore associato alla spedizione.", sessionID, nomeRequisito, statoReq);
            return;
        }

        std::cout << "Spedizione " << idSpedizione << " consegnata! " << std::endl;
        return;
    }

};

#endif // SPEDIZIONE_H