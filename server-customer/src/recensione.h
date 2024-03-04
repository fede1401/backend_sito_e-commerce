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

    void effettuaRecensione(Con2DB db1, std::string in_nome_utente_compratore, int idOrdine, std::string descrizione, votoStelle voto_stella)
    {

        std::string sessionID = "";

        // Connession al database:
        // Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        std::string stato_spedizione;

        std::string nomeRequisito = "Effettuazione recensione.";
        statoRequisito statoReq = statoRequisito::Wait;

        // Innanzitutto controllo se l'ordine è stato spedito e arrivato correttamente
        sprintf(sqlcmd, "SELECT statoSpedizione FROM Spedizione WHERE idOrdine = '%d'", idOrdine);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows == 1)
        {
            stato_spedizione = PQgetvalue(res, 0, PQfnumber(res, "statoSpedizione"));
            PQclear(res);

            if (stato_spedizione == "consegnato")
            {

                // Selezioniamo il nome del'utente compratore:
                std::string nome_utente_compratore;
                sprintf(sqlcmd, "SELECT nome_utente_compratore FROM Ordine WHERE idOrdine = '%d'", idOrdine);
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);
                if (rows == 1)
                {
                    nome_utente_compratore = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_compratore"));
                    PQclear(res);

                    // Controlliamo se l'utente compratore dell'ordine corrisponde a quello che vuole effettuare l'azione:
                    if (in_nome_utente_compratore != nome_utente_compratore)
                    {
                        InsertToLogDB(db1, "ERROR", "Utente che sta cercando di effettuare la recensione non corrisponde a quello dell ordine della recensione", sessionID, nomeRequisito, statoReq);
                        return;
                    }

                    // Caricamento del sessionID utile per il log.
                    sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nome_utente_compratore.c_str());
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);

                    if (rows == 1)
                    {
                        sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));
                    }
                    PQclear(res);

                    if (sessionID == "")
                    {
                        InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non si può effettuare una recensione .", sessionID, nomeRequisito, statoReq);
                        return;
                    }

                    std::string votoStelleStr = statoVotoStelleToString(voto_stella);

                    sprintf(sqlcmd, "INSERT INTO Recensione (idRec, nome_utente_compratore, idOrdine, descrizione, votoStelle) VALUES (DEFAULT, '%s', '%d', '%s', '%s')",
                            nome_utente_compratore.c_str(), idOrdine, descrizione.c_str(), votoStelleStr.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    statoReq = statoRequisito::Success;

                    InsertToLogDB(db1, "INFO", "Effettuata recensione compratore", sessionID, nomeRequisito, statoReq);
                }
                else
                {
                    std::cout << "L'ordine non è stato trovato!" << std::endl;

                    statoReq = statoRequisito::NotSuccess;

                    InsertToLogDB(db1, "WARNING", "Ordine non trovato", sessionID, nomeRequisito, statoReq);
                    return;
                }
            }
            else
            {
                std::cout << "L'ordine è stato spedito, ma non è ancora arrivato, perciò non può essere effettuata la recensione!" << std::endl;

                statoReq = statoRequisito::NotSuccess;
                InsertToLogDB(db1, "WARNING", "Ordine spedito, ma non arrivato, perciò non può essere effettuata la recensione", sessionID, nomeRequisito, statoReq);
                return;
            }
        }
        else
        {
            std::cout << "L'ordine non è stato ancora spedito, perciò non può essere effettuata la recensione!" << std::endl;

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1, "WARNING", "Ordine non spedito, non può essere effettuata la recensione", sessionID, nomeRequisito, statoReq);
            return;
        }
        std::cout << "Recensione effettuata" << std::endl;
        return;
    }

    void remove_recensione(Con2DB db1, std::string in_nome_utente_compratore, int idRecensione)
    {

        // Connession al database:
        // Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        std::string nomeRequisito = "Rimozione recensione.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string sessionID = "";

        // Caricamento del nome dell'utente compratore che vuole rimuovere la recensione effettuata
        std::string nome_utente_compratore;
        sprintf(sqlcmd, "SELECT nome_utente_compratore FROM Recensione WHERE idRec = '%d'", idRecensione);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows == 1)
        {
            nome_utente_compratore = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_compratore"));
        }
        PQclear(res);

        // Controlliamo se l'utente compratore dell'ordine corrisponde a quello che vuole effettuare l'azione:
        if (in_nome_utente_compratore != nome_utente_compratore)
        {
            InsertToLogDB(db1, "ERROR", "Utente che sta cercando di eliminare la recensione non corrisponde a quello dell ordine della recensione", sessionID, nomeRequisito, statoReq);
            return;
        }

        // Caricamento del sessionID utile per il log.
        sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows == 1)
        {
            sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));
        }
        PQclear(res);

        if (sessionID == "")
        {
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non si può rimuovere una recensione .", sessionID, nomeRequisito, statoReq);
            return;
        }

        sprintf(sqlcmd, "SELECT * FROM Recensione WHERE idRec = '%d'", idRecensione);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);
        if (rows < 1)
        {
            std::cout << "La riga da eliminare non esiste!" << std::endl;

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1, "ERROR", "La recensione non esiste.", sessionID, nomeRequisito, statoReq);
            return;
        }
        else
        {
            // Eliminazione della recensione tramite l'id.
            sprintf(sqlcmd, "DELETE FROM Recensione WHERE idRec = '%d'", idRecensione);
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            statoReq = statoRequisito::Success;

            InsertToLogDB(db1, "INFO", "Recensione eliminata.", sessionID, nomeRequisito, statoReq);
        }
        return;
    }

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
