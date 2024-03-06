// Recensione.h
#ifndef RECENSIONE_H
#define RECENSIONE_H

#include "main.h"

enum class votoStelle
{
    Uno,
    Due,
    Tre,
    Quattro,
    Cinque
};

class Recensione
{
public:
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


    // Funzione utilizzata per permettere ad un utente compratore di effettuare una recensione di un ordine. 
    void effettuaRecensione(Con2DB db1, std::string in_nome_utente_compratore, int idOrdine, std::string descrizione, votoStelle voto_stella)
    {
        std::string sessionID = "";
        std::string stato_spedizione;


        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Effettuazione recensione.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";


        // Recupero dello stato della spedizione dell'ordine tramite il suo id per verificare se l'ordine è stato spedito e arrivato correttamente.
        sprintf(sqlcmd, "SELECT statoSpedizione FROM Spedizione WHERE idOrdine = '%d'", idOrdine);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows == 1)
        {
            stato_spedizione = PQgetvalue(res, 0, PQfnumber(res, "statoSpedizione"));
            PQclear(res);

            // Se lo stato della spedizione è "consegnato"
            if (stato_spedizione == "consegnato")
            {

                // Selezioniamo il nome del'utente compratore che ha effettuato l'ordine tramite il suo id.
                std::string nome_utente_compratore;
                sprintf(sqlcmd, "SELECT nome_utente_compratore FROM Ordine WHERE idOrdine = '%d'", idOrdine);
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);
                if (rows == 1)
                {
                    nome_utente_compratore = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_compratore"));
                    PQclear(res);

                    // Se il nome dell'utente compratore che ha effettuato l'ordine è diverso dall'utente che vuole effettuare una recensione, allora non è possibile effettuarla
                    if (in_nome_utente_compratore != nome_utente_compratore)
                    {
                        // Log dell'errore e uscita dalla funzione
                        InsertToLogDB(db1, "ERROR", "Utente che sta cercando di effettuare la recensione non corrisponde a quello dell ordine della recensione", sessionID, nomeRequisito, statoReq);
                        return;
                    }

                    // Caricamento del sessionID.
                    sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nome_utente_compratore.c_str());
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    if (rows == 1){  sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c")); }
                    PQclear(res);

                    // Verifica se l'utente è loggato e ha una sessionID valida
                    if (sessionID == "") {
                        // Log dell'errore e uscita dalla funzione
                        InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non si può effettuare una recensione .", sessionID, nomeRequisito, statoReq);
                        return;
                    }

                    // Rendiamo il voto in stelle in stringa così che posso aggiungerlo al database.
                    std::string votoStelleStr = statoVotoStelleToString(voto_stella);

                    // Inserisco nel database una riga corrispondente alla recensione.
                    sprintf(sqlcmd, "INSERT INTO Recensione (idRec, nome_utente_compratore, idOrdine, descrizione, votoStelle) VALUES (DEFAULT, '%s', '%d', '%s', '%s')",
                            nome_utente_compratore.c_str(), idOrdine, descrizione.c_str(), votoStelleStr.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Log 
                    statoReq = statoRequisito::Success;
                    messageLog = "Effettuata recensione compratore " + in_nome_utente_compratore;
                    InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
                }

                // Se non esiste nessun utente associato all'id dell'ordine , allora l'ordine non esiste.
                else
                {
                    std::cout << "L'ordine non è stato trovato!" << std::endl;

                    // Log dell'errore e uscita dalla funzione
                    statoReq = statoRequisito::NotSuccess;
                    InsertToLogDB(db1, "WARNING", "Ordine non trovato", sessionID, nomeRequisito, statoReq);
                    return;
                }
            }

            // Se lo stato della spedizione non è stato ancora "consegnato" allora la recensione non può essere effettuata
            else
            {
                std::cout << "L'ordine è stato spedito, ma non è ancora arrivato, perciò non può essere effettuata la recensione!" << std::endl;

                // Log dell'errore e uscita dalla funzione
                statoReq = statoRequisito::NotSuccess;
                InsertToLogDB(db1, "WARNING", "Ordine spedito, ma non arrivato, perciò non può essere effettuata la recensione", sessionID, nomeRequisito, statoReq);
                return;
            }
        }

        // Se il numero delle righe del risultato delle query è diverso da 1, allora l'ordine non è stato trovato
        else
        {   
            // Log dell'errore e uscita dalla funzione
            std::cout << "L'ordine non è stato ancora spedito, perciò non può essere effettuata la recensione!" << std::endl;

            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "WARNING", "Ordine non spedito, non può essere effettuata la recensione", sessionID, nomeRequisito, statoReq);
            return;
        }
    std::cout << "Recensione effettuata" << std::endl;
    return;
    }



    // Funzione utilizzata per permettere all'utente compratore di rimuovere una recensione dal database tramite il suo id
    void remove_recensione(Con2DB db1, std::string in_nome_utente_compratore, int idRecensione)
    {
        std::string sessionID = "";

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Rimozione recensione.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";


        // Recupero del nome dell'utente compratore che desidera rimuovere la recensione effettuata
        std::string nome_utente_compratore;
        sprintf(sqlcmd, "SELECT nome_utente_compratore FROM Recensione WHERE idRec = '%d'", idRecensione);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows == 1)
        {
            nome_utente_compratore = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_compratore"));
        }
        PQclear(res);


        // Se l'utente che vuole rimuovere la recensione è diverso dall'utente che ha effettuato la recensione allora c'è un errore
        if (in_nome_utente_compratore != nome_utente_compratore)
        {
            // Log dell'errore e uscita dalla funzione
            InsertToLogDB(db1, "ERROR", "Utente che sta cercando di eliminare la recensione non corrisponde a quello dell ordine della recensione", sessionID, nomeRequisito, statoReq);
            return;
        }

        // Caricamento del sessionID.
        sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows == 1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c")); }
        PQclear(res);

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non si può rimuovere una recensione .", sessionID, nomeRequisito, statoReq);
            return;
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
            InsertToLogDB(db1, "ERROR", "La recensione non esiste.", sessionID, nomeRequisito, statoReq);
            return;
        }

        // Se invece il numero di righe del risultato della query è > di 1, allora la recensione può essere eliminata.
        else
        {
            // Eliminazione della recensione tramite l'id.
            sprintf(sqlcmd, "DELETE FROM Recensione WHERE idRec = '%d'", idRecensione);
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Recensione eliminata da parte di " + in_nome_utente_compratore;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }
    
    return;
    }


    // Metodo che prende un tipo enumerativo votoStelle come input e restituisce una stringa che rappresenta quel particolare stato.
    std::string statoVotoStelleToString(votoStelle stato)
    {
        switch (stato)
        {
        case votoStelle::Uno:
            return "1";
        case votoStelle::Due:
            return "2";
        case votoStelle::Tre:
            return "3";
        case votoStelle::Quattro:
            return "4";
        case votoStelle::Cinque:
            return "5";
        default:
            return ""; // gestione degli errori o valori non validi
        }
    }
};

#endif // RECENSIONE_H
