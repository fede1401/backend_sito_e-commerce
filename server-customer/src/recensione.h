// Recensione.h
#ifndef RECENSIONE_H
#define RECENSIONE_H

#include "main.h"
#include "../../shared-server/statoVotoStelleToString.h"


class Recensione
{
public:
    // Attributi per la classe Recensione.
    int idRecensione;
    std::string nome_utente_compratore;
    int idOrdine;
    std::string descrizione;
    votoStelle voto_stella;

    // Costruttori:
    Recensione() : idRecensione(-1),
                   nome_utente_compratore(""),
                   idOrdine(-1),
                   descrizione(""),
                   voto_stella() {}

    Recensione(int idRecensione, std::string nome_utente_compratore, int idOrdine,
               std::string descrizione, votoStelle voto_stella) :

                                                                  idRecensione(idRecensione), nome_utente_compratore(nome_utente_compratore),
                                                                  idOrdine(idOrdine),
                                                                  descrizione(descrizione), voto_stella(voto_stella)
    {
    }


    // Metodo utilizzato per permettere ad un utente compratore di effettuare una recensione di un ordine. 
    std::string effettuaRecensione(Con2DB db1, std::string in_nome_utente_compratore, int idOrdine, std::string descrizione, votoStelle voto_stella)
    {
        std::string sessionID = "";
        std::string stato_spedizione;

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Effettuazione recensione.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        // Caricamento del sessionID.
        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare il sessionID
        if (rows == 1){  sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id")); }
        PQclear(res);

        // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
        if (rows == 0){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste " + in_nome_utente_compratore + " , poichè non è stato registrato, non può essere effettuata la recensione .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }   

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "") {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + in_nome_utente_compratore + ", utente non loggato, non può essere effettuata la recensione.";
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
            messageLog = "L utente " + in_nome_utente_compratore + " non è un utente compratore, perciò non può essere effettuata la recensione.";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }


        // Recupero dello stato della spedizione dell'ordine tramite il suo id per verificare se l'ordine è stato spedito e arrivato correttamente.
        sprintf(sqlcmd, "SELECT statoSpedizione FROM Spedizione WHERE idOrdine = '%d'", idOrdine);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare lo stato della spedizione.
        if (rows == 1)
        {
            stato_spedizione = PQgetvalue(res, 0, PQfnumber(res, "statoSpedizione"));
            PQclear(res);

            // Se lo stato della spedizione è "consegnato" possiamo procedere nel metodo effettuaRecensione.
            if (stato_spedizione == "consegnato")
            {
                // Selezioniamo il nome del'utente compratore che ha effettuato l'ordine tramite il suo id.
                std::string nome_utente_compratore;
                sprintf(sqlcmd, "SELECT nome_utente_compratore FROM Ordine WHERE idOrdine = '%d'", idOrdine);
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);
                // Se il numero di righe del risultato della query è 1, allora possiamo recuperare il nome dell'utente compratore.
                if (rows == 1)
                {
                    nome_utente_compratore = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_compratore"));
                    PQclear(res);

                    // Se il nome dell'utente compratore che ha effettuato l'ordine è diverso dall'utente che vuole effettuare una recensione, allora non è possibile effettuarla
                    if (in_nome_utente_compratore != nome_utente_compratore)
                    {
                        // Log dell'errore e uscita dalla funzione
                        statoReq = statoRequisito::NotSuccess;
                        messageLog = "Utente che sta cercando di effettuare la recensione ( " + in_nome_utente_compratore + ") non corrisponde a quello dell ordine della recensione ( " + nome_utente_compratore+ ").";
                        InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
                        
                        result = messageLog;
                        return result;
                    }

                    // Rendiamo il voto in stelle in stringa così che posso aggiungerlo al database.
                    std::string votoStelleStr = statoVotoStelleToString(voto_stella);

                    // Inserisco nel database una riga corrispondente alla recensione.
                    sprintf(sqlcmd, "INSERT INTO Recensione (idRec, nome_utente_compratore, idOrdine, descrizione, votoStelle) VALUES (DEFAULT, '%s', '%d', '%s', '%s')",
                            nome_utente_compratore.c_str(), idOrdine, descrizione.c_str(), votoStelleStr.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Devo riempire l'oggetto Recensione della classe:
                    // Esegui una query SELECT per ottenere l'ultimo ID inserito nella tabella Recensione:
                    // 1. Selezioniamo tutti gli idRec dalla tabella Recensione:
                    sprintf(sqlcmd, "SELECT idRec FROM Recensione");
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    // 2. Prendiamo l'ultimo id
                    this->idRecensione = atoi(PQgetvalue(res, rows - 1, 0));
                    PQclear(res);

                    this->nome_utente_compratore = in_nome_utente_compratore;
                    this->idOrdine = idOrdine;
                    this->descrizione = descrizione;
                    this->voto_stella = voto_stella;

                    // Log 
                    statoReq = statoRequisito::Success;
                    messageLog = "Effettuata recensione compratore " + in_nome_utente_compratore;
                    InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
                }

                // Se non esiste nessun utente associato all'id dell'ordine , allora l'ordine non esiste.
                if (rows == 0)
                {
                    std::cout << "L'utente non è stato trovato!" << std::endl;

                    // Log dell'errore e uscita dalla funzione
                    statoReq = statoRequisito::NotSuccess;
                    messageLog = "Utente " + in_nome_utente_compratore + " non trovato";
                    InsertToLogDB(db1, "WARNING", messageLog , sessionID, nomeRequisito, statoReq);
                    
                    result = messageLog;
                    return result;
                }
            }

            // Se lo stato della spedizione non è stato ancora "consegnato" allora la recensione non può essere effettuata
            else
            {
                std::cout << "L'ordine è stato spedito, ma non è ancora arrivato, perciò non può essere effettuata la recensione!" << std::endl;

                // Log dell'errore e uscita dalla funzione
                statoReq = statoRequisito::NotSuccess;
                messageLog = "Ordine con codice " + std::to_string(idOrdine) + " spedito, ma non arrivato, perciò non può essere effettuata la recensione";
                InsertToLogDB(db1, "WARNING", messageLog, sessionID, nomeRequisito, statoReq);
                
                result = messageLog;
                return result;
            }
        }

        // Se il numero delle righe del risultato delle query è diverso da 1, allora l'ordine non è stato trovato
        else
        {   
            // Log dell'errore e uscita dalla funzione
            std::cout << "L'ordine non è stato trovato!" << std::endl;

            statoReq = statoRequisito::NotSuccess;
            messageLog = "Ordine con codice " + std::to_string(idOrdine) + " non trovato";
            InsertToLogDB(db1, "WARNING", messageLog , sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }
    
    std::cout << "Recensione effettuata" << std::endl;
    result = messageLog;
    return result;
    }



    // Funzione utilizzata per permettere all'utente compratore di rimuovere una recensione dal database tramite il suo id
    std::string remove_recensione(Con2DB db1, std::string in_nome_utente_compratore, int idRecensione)
    {
        std::string sessionID = "";

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Rimozione recensione.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";


        // Caricamento del sessionID.
        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare il sessionID
        if (rows == 1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id")); }
        PQclear(res);

        // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
        if (rows == 0){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste " + in_nome_utente_compratore + " , poichè non è stato registrato, non può essere rimossa la recensione .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }   

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + in_nome_utente_compratore + ", utente non loggato, non può essere rimossa la recensione.";
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
            messageLog = "L utente " + in_nome_utente_compratore + " non è un utente compratore, perciò non può essere rimossa la recensione.";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Recupero del nome dell'utente compratore che desidera rimuovere la recensione effettuata
        std::string nome_utente_compratore;
        sprintf(sqlcmd, "SELECT nome_utente_compratore FROM Recensione WHERE idRec = '%d'", idRecensione);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare il nome dell'utente compratore che ha effettuato la recensione.
        if (rows == 1)
        {
            nome_utente_compratore = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_compratore"));
        }
        PQclear(res);


        // Se l'utente che vuole rimuovere la recensione è diverso dall'utente che ha effettuato la recensione allora c'è un errore
        if (in_nome_utente_compratore != nome_utente_compratore)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Utente che sta cercando di effettuare la recensione ( " + in_nome_utente_compratore + ") non corrisponde a quello dell ordine della recensione ( " + nome_utente_compratore+ ").";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        
        // Verifico che la recensione esista tramite il suo recupero.
        sprintf(sqlcmd, "SELECT * FROM Recensione WHERE idRec = '%d'", idRecensione);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        // Se il numero di righe del risultato della query è minore di 1, allora la recensione non esiste, perciò non può essere eliminata.
        if (rows < 1)
        {
            // Log dell'errore e uscita dalla funzione
            std::cout << "La riga da eliminare non esiste!" << std::endl;

            statoReq = statoRequisito::NotSuccess;
            messageLog = "La recensione con id " + std::to_string(idRecensione) + " non esiste";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Se invece il numero di righe del risultato della query è 1, allora la recensione può essere eliminata.
        if (rows == 1)
        {
            // Eliminazione della recensione tramite l'id.
            sprintf(sqlcmd, "DELETE FROM Recensione WHERE idRec = '%d'", idRecensione);
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Recensione con id " + std::to_string(idRecensione) + " eliminata da parte di " + in_nome_utente_compratore;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }
    
    result = messageLog;
    return result;
    }


    
};

#endif // RECENSIONE_H
