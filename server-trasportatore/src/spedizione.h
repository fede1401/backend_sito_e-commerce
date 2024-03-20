// Spedizione.h
#ifndef SPEDIZIONE_H
#define SPEDIZIONE_H

#include "main.h"
#include "../../shared-server/statoSpedizioneToString.h"
#include <random>

class Spedizione
{
public:
    // Attributi per la classe Spedizione.
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
                   ditta_spedizione("") {}

    Spedizione(int idSpedizione, int idOrdine,
               std::string nome_utente_trasportatore,
               StatoSpedizione stato_spedizione,
               std::string ditta_spedizione) :

                                               idSpedizione(idSpedizione),
                                               idOrdine(idOrdine),
                                               nome_utente_trasportatore(nome_utente_trasportatore),
                                               stato_spedizione(stato_spedizione),
                                               ditta_spedizione(ditta_spedizione)
    {
    }

    void impostaStatoSpedizione(StatoSpedizione nuovstato)
    {
        stato_spedizione = nuovstato;
    }

    // Metodo utilizzato per permettere a un utente trasportatore di prendere in carico una spedizione.
    std::string prendiInCaricoSpedizione(Con2DB db1, std::string in_nome_utente_trasportatore)
    {

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Prendi in carico spedizione.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        // Inizializzazione generatore di numeri casuali per la scelta da parte dell'utente trasportatore dell'ordine che vuole.
        srand((unsigned)time(NULL));

        int dispoTrasportatore = -1;
        int sceltaOrdineDaSpedire = -1;
        std::string nome_ditta_spedizione;
        stato_spedizione = StatoSpedizione::InTransito;
        std::string statoSpedizioneStr = statoSpedizioneToString(stato_spedizione);

        // Caricamento del sessionID.
        std::string sessionID = "";

        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente_trasportatore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare il sessionID
        if (rows == 1)
        {
            sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id"));
        }
        PQclear(res);

        // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
        if (rows == 0)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste " + in_nome_utente_trasportatore + " , poichè non è stato registrato, non si può effettuare il logout .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
           
            result = messageLog;
            return result;
        }

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + in_nome_utente_trasportatore + ", utente non loggato, non può essere presa in carico la spedizione.";
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato, non può effettuare il logout .", sessionID, nomeRequisito, statoReq);
           
            result = messageLog;
            return result;
        }

        // else:

        // Verifichiamo che l'utente si tratti di un utente trasportatore:
        std::string categoriaUtente = "";
        sprintf(sqlcmd, "SELECT categoriaUtente FROM Utente WHERE nome_utente = '%s'", in_nome_utente_trasportatore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare la categoria dell'utente.
        if (rows == 1)
        {
            categoriaUtente = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));
        }
        PQclear(res);

        printf("Categoria utente %s\n\n", categoriaUtente);

        if (categoriaUtente != "UtenteTrasportatore")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "L utente " + in_nome_utente_trasportatore + " non è un utente trasportatore, perciò non può essere presa in carico la spedizione.";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Controllo se l'utente trasportatore ha l'attributo "disponibilità" = 0, cioè non è impegnato in alcuna spedizione.
        sprintf(sqlcmd, "SELECT dispo FROM UtenteTrasportatore WHERE nome_utente_trasportatore='%s'", in_nome_utente_trasportatore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe ottenute dal risultato della query sono uguali a 1, l'utente è stato trovato e possiamo recuperare il valore dell'attributo "disponibilità".
        if (rows == 1)
        {
            dispoTrasportatore = atoi(PQgetvalue(res, 0, PQfnumber(res, "dispo")));
        }
        PQclear(res);

        // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
        if (rows == 0)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste " + in_nome_utente_trasportatore + " .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Se il valore dell'attributo "disponibilità" è 0, allora l'utente trasportatore può prendere in carico delle spedizioni.
        if (dispoTrasportatore == 0)
        {
            // Recuperiamo l'id degli ordini che sono nello stato: "in elaborazione" e non ancora spediti:
            sprintf(sqlcmd, "SELECT idOrdine FROM Ordine WHERE statoOrdine='in elaborazione'");
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);

            // Se il numero di righe del risultato della query è maggiore di 0, cioè ci sono ordini con lo stato "in elaborazione" allora possiamo assegnare all'utente trasportatore un ordine da spedire
            if (rows > 0)
            {
                // Definisco un indice casuale
                int i = rand() % rows;

                // Scelta dell'utente trasportatore di prendere un ordine da spedire con stato "in elaborazione":
                idOrdine = atoi(PQgetvalue(res, i, 0));

                PQclear(res);

                // Recupero del nome della ditta di spedizione dell'utente trasportatore:
                sprintf(sqlcmd, "SELECT nome_DittaSpedizione FROM UtenteTrasportatore WHERE nome_utente_trasportatore='%s'", nome_utente_trasportatore.c_str());
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);

                // Se il numero di righe del risultato della query è uguale di 1, allora c'è una ditta di spdedizione associata all'utente
                if (rows == 1)
                {
                    nome_ditta_spedizione = PQgetvalue(res, 0, 0);
                    PQclear(res);
                }

                // Inserisco nel database un record relativo alla spedizione effettuata dall'utente trasportatore libero (con disponibilità 0) e con un ordine "in elaborazione":
                sprintf(sqlcmd, "INSERT INTO Spedizione (idSpedizione, idOrdine, nome_utente_trasportatore, statoSpedizione, nome_DittaSpedizione) VALUES (DEFAULT, '%d', '%s', '%s', '%s')",
                        idOrdine, in_nome_utente_trasportatore.c_str(), statoSpedizioneStr.c_str(), nome_ditta_spedizione.c_str());
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                // Log
                messageLog = "Assegnato ordine " + std::to_string(idOrdine) + " al trasportatore " + nome_utente_trasportatore;
                InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

                // Una volta assegnata la spedizione all'utente trasportatore dobbiamo modificare la disponibilità di quest'ultimo.
                sprintf(sqlcmd, "UPDATE UtenteTrasportatore set dispo='1' WHERE nome_utente_trasportatore = '%s'", in_nome_utente_trasportatore.c_str());
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                // Log
                messageLog = "Modificata disponibilità al trasportatore " + in_nome_utente_trasportatore;
                InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

                // Inoltre dobbiamo rendere lo stato dell'ordine "spedito", poichè è stato spedito.
                sprintf(sqlcmd, "UPDATE Ordine set statoOrdine ='spedito' WHERE idOrdine = '%d'", idOrdine);
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                // Log
                statoReq = statoRequisito::Success;
                messageLog = "Avviata spedizione per ordine " + std::to_string(idOrdine);
                InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

                // Animiamo l'oggetto Spedizione:
                // Esegui una query SELECT per ottenere l'ultimo ID inserito nella tabella Spedizione:
                // 1. Selezioniamo tutti gli idSpedizione dalla tabella Spedizione:
                sprintf(sqlcmd, "SELECT idSpedizione FROM Spedizione");
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);
                // 2. Prendiamo l'ultimo id
                this->idSpedizione = atoi(PQgetvalue(res, rows - 1, 0));
                PQclear(res);

                this->idOrdine = idOrdine;
                this->nome_utente_trasportatore = in_nome_utente_trasportatore;
                this->stato_spedizione = stato_spedizione;
                this->ditta_spedizione = nome_ditta_spedizione;
            }

            // Se il numero di righe del risultato della query è NON è > di 0, NON ci sono ordini con lo stato "in elaborazione".
            else
            {
                // Log dell'errore e uscita dalla funzione
                statoReq = statoRequisito::NotSuccess;
                messageLog = "Non esistono ordini con stato in elaborazione! ";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
                
                result = messageLog;
            return result;
            }
        }

        // Se il valore dell'attributo "disponibilità" è diverso da 0, allora l'utente trasportatore NON può prendere in carico delle spedizioni perchè è già occupato nella spedizione.
        else
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "L utente " + in_nome_utente_trasportatore + " è occupato già nella spedizione di altri ordini, perciò non può prendere in carico l ordine " + std::to_string(idOrdine);
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        result = messageLog;
        return result;
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
            if (rows == 1)
            {
                sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));
            }
            PQclear(res);

            if (rows != 1)
            {
                // Log dell'errore e uscita dalla funzione
                messageLog = "Non esiste " + nome_utente_trasportatore + " , poichè non è stato registrato, non può essere assegnato a lui nessun ordine .";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
                return;
            }

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
                    messageLog = "Assegnato ordine " + std::to_string(idOrdine) + " al trasportatore " + nome_utente_trasportatore;
                    InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

                    // Una volta assegnata la spedizione all'utente trasportatore dobbiamo modificare la disponibilità di quest'ultimo.
                    sprintf(sqlcmd, "UPDATE UtenteTrasportatore set dispo='1' WHERE nome_utente_trasportatore = '%s'", nome_utente_trasportatore.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Log
                    messageLog = "Modificata disponibilità al trasportatore " + nome_utente_trasportatore;
                    InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

                    // Inoltre dobbiamo rendere lo stato dell'ordine "spedito", poichè è stato spedito.
                    sprintf(sqlcmd, "UPDATE Ordine set statoOrdine ='spedito' WHERE idOrdine = '%d'", idOrdine);
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Log
                    statoReq = statoRequisito::Success;
                    messageLog = "Avviata spedizione per ordine " + std::to_string(idOrdine);
                    InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

                    // Animiamo l'oggetto Spedizione:
                    // Esegui una query SELECT per ottenere l'ultimo ID inserito nella tabella Spedizione:
                    // 1. Selezioniamo tutti gli idSpedizione dalla tabella Spedizione:
                    sprintf(sqlcmd, "SELECT idSpedizione FROM Spedizione");
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    // 2. Prendiamo l'ultimo id
                    this->idSpedizione = atoi(PQgetvalue(res, rows - 1, 0));
                    PQclear(res);

                    this->idOrdine = idOrdine;
                    this->nome_utente_trasportatore = nome_utente_trasportatore;
                    this->stato_spedizione = stato_spedizione;
                    this->ditta_spedizione = nome_ditta_spedizione;
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

                // Log
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
    std::string spedizioneConsegnata(Con2DB db1, std::string in_nome_utente_trasportatore, int idSpedizione)
    {
        std::string sessionID = "";

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Consegna Spedizione.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        // Caricamento del sessionID.
        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente_trasportatore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare il sessionID.
        if (rows == 1)
        {
            sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id"));
        }
        PQclear(res);

        // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
        if (rows == 0)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste " + in_nome_utente_trasportatore + " , poichè non è stato registrato, non si può avvisare che la spedizione è consegnata .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + in_nome_utente_trasportatore + ", utente non loggato, non si può avvisare che la spedizione è consegnata .";

            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifichiamo che l'utente si tratti di un utente trasportatore:
        std::string categoriaUtente = "";
        sprintf(sqlcmd, "SELECT categoriaUtente FROM Utente WHERE nome_utente = '%s'", in_nome_utente_trasportatore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare la categoria dell'utente.
        if (rows == 1)
        {
            categoriaUtente = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));
        }
        PQclear(res);

        if (categoriaUtente != "UtenteTrasportatore")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "L utente " + in_nome_utente_trasportatore + " non è un utente trasportatore, perciò non può essere presa in carico la spedizione.";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifico che l'id della spedizione esista effettuando una query.
        sprintf(sqlcmd, "SELECT * FROM Spedizione WHERE idSpedizione = '%d'", idSpedizione);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe del risultato della query è < di 1, allora l'id non esiste, errore
        if (rows < 1)
        {
            // Log dell'errore e uscita dalla funzione
            messageLog = "Non esiste id " + std::to_string(idSpedizione) + " della spedizione";
            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

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
                messageLog = "Utente che sta cercando di avvisare che la spedizione è consegnata ( " + in_nome_utente_trasportatore + ") non corrisponde a quello della spedizione ( " + nome_utente_trasportatore + ").";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
                
                result = messageLog;
                return result;
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

            // A questo punto l'utente trasportatore può prendere nuove consegne, perciò aggiorniamo la sua disponibilità.
            sprintf(sqlcmd, "UPDATE UtenteTrasportatore set dispo='0' WHERE nome_utente_trasportatore = '%s'", nome_utente_trasportatore.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            nomeRequisito = "Utente Trasportatore liberato.";
            statoReq = statoRequisito::Success;
            messageLog = "Disponibilità utente trasportatore " + in_nome_utente_trasportatore + " aggiornata per prendere in consegna un nuovo pacco ";
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }

        // Se il numero di righe del risultato della query è 0, allora NON esiste l'utente trasportatore associato alla spedizione.
        if (rows == 0)
        {
            std::cout << "Nessun utente trasportatore associato alla spedizione! " << std::endl;

            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "WARNING", "Nessun utente trasportatore associato alla spedizione.", sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        std::cout << "Spedizione " << idSpedizione << " consegnata! " << std::endl;
        
    result = messageLog;
    return result;
    }
    
};

#endif // SPEDIZIONE_H