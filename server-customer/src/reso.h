// Reso.h
#ifndef RESO_H
#define RESO_H

#include "main.h"


class Reso
{
public:
    // Attributi per la classe Reso.
    int m_dReso;
    std::string m_nomeUtenteCompratore;
    int m_idOrdine;
    motivazioneReso m_motivazioneReso;

    // Costruttori:
    Reso() : m_dReso(-1),
             m_nomeUtenteCompratore(""),
             m_idOrdine(-1),
             m_motivazioneReso() {}

    Reso(int id_reso, std::string nome_utente_compratore, int id_ordine, motivazioneReso motivazione_reso) : m_dReso(id_reso),
                                                                                                           m_nomeUtenteCompratore(nome_utente_compratore),
                                                                                                           m_idOrdine(id_ordine),
                                                                                                           m_motivazioneReso(motivazione_reso) {}

    void impostaStato(motivazioneReso in_nuovoStato)
    {
        m_motivazioneReso = in_nuovoStato;
    }


    // Metodo utilizzato per permettere ad un utente compratore di effettuate un reso di un ordine.
    std::string effettua_reso(Con2DB db1, std::string in_nome_utente_compratore, int in_id_ordine, motivazioneReso in_motivazione_reso)
    {

        std::string sessionID = "";
        std::string stato_spedizione;

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Effettuazione Reso.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        // Caricamento del sessionID.
        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente_compratore.c_str());
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
            messageLog = "Non esiste " + in_nome_utente_compratore + " , poichè non è stato registrato, non può essere effettuato il reso .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "L utente " + in_nome_utente_compratore + " non è un utente compratore, non può essere effettuato il reso .";
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
        if (rows == 1)
        {
            categoriaUtente = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));
        }
        PQclear(res);

        if (categoriaUtente != "UtenteCompratore")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "L utente " + in_nome_utente_compratore + " non è un utente compratore, perciò non può essere effettuato il reso.";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }


        // Verifico che l'id dell'ordine esista nel database:
        sprintf(sqlcmd, "SELECT * FROM Ordine WHERE idOrdine = '%d'",  in_id_ordine);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è < 1, allora non esiste l'ordine.
        if (rows < 1){
            PQclear(res);

            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste ordine con id : " + std::to_string(in_id_ordine);
            InsertToLogDB(db1, "ERROR", messageLog , sessionID, nomeRequisito, statoReq);
                    
            result = messageLog;
            return result;
        }
        PQclear(res);



        // Verifico che l'utente non abbia già effettuato un reso per lo stesso ordine.
        sprintf(sqlcmd, "SELECT * FROM Reso WHERE nome_utente_compratore = '%s' AND idOrdine = '%d'", in_nome_utente_compratore.c_str(), in_id_ordine);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora già esiste un reso da parte dell'utente dello stesso ordine acquistato.
        if (rows == 1){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Attualmente esiste già una reso da parte di " + in_nome_utente_compratore + " per ordine: " + std::to_string(in_id_ordine);
            InsertToLogDB(db1, "ERROR", messageLog , sessionID, nomeRequisito, statoReq);
                    
            result = messageLog;
            return result;
        }


        // Recupero dello stato della spedizione dell'ordine tramite il suo id per verificare se l'ordine è stato spedito e arrivato correttamente.
        sprintf(sqlcmd, "SELECT statoSpedizione FROM Spedizione WHERE idOrdine = '%d'", in_id_ordine);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare lo stato della spedizione.
        if (rows == 1)
        {
            stato_spedizione = PQgetvalue(res, 0, PQfnumber(res, "statoSpedizione"));
            PQclear(res);

            // Se lo stato della spedizione è "consegnato"
            if (stato_spedizione == "consegnato")
            {

                // Selezioniamo il nome del'utente compratore che ha effettuato l'ordine tramite il suo id.
                std::string nome_utente_compratore;
                sprintf(sqlcmd, "SELECT nome_utente_compratore FROM Ordine WHERE idOrdine = '%d'", in_id_ordine);
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);
                // Se il numero di righe del risultato della query è 1, allora possiamo recuperare il nome dell'utente compratore.
                if (rows == 1)
                {
                    nome_utente_compratore = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_compratore"));
                    PQclear(res);

                    // Se il nome dell'utente compratore che ha effettuato l'ordine è diverso dall'utente che vuole effettuare un reso, allora non è possibile effettuarlo.
                    if (in_nome_utente_compratore != nome_utente_compratore)
                    {
                        // Log dell'errore e uscita dalla funzione
                        messageLog = "Utente che sta cercando di effettuare il reso ( " + in_nome_utente_compratore + ") non corrisponde a quello dell ordine ( " + nome_utente_compratore + ").";
                        InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
                        
                        result = messageLog;
                        return result;
                    }

                    // Rendiamo la motivazione del reso in stringa così che posso aggiungerlo al database.
                    std::string motivazione_resoStr = statoMotivazioneResoToString(in_motivazione_reso);

                    // Inserisco nel database una riga corrispondente al reso.
                    sprintf(sqlcmd, "INSERT INTO Reso (idReso, nome_utente_compratore, idOrdine, motivazioneReso) VALUES (DEFAULT, '%s', '%d', '%s')",
                            nome_utente_compratore.c_str(), in_id_ordine, motivazione_resoStr.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Devo riempire l'oggetto Reso della classe:
                    // Esegui una query SELECT per ottenere l'ultimo ID inserito nella tabella Reso:
                    // 1. Selezioniamo tutti gli idReso dalla tabella Recensione:
                    sprintf(sqlcmd, "SELECT idReso FROM Reso");
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    // 2. Prendiamo l'ultimo id
                    this->m_dReso = atoi(PQgetvalue(res, rows - 1, 0));
                    PQclear(res);

                    this->m_nomeUtenteCompratore = in_nome_utente_compratore;
                    this->m_idOrdine = in_id_ordine;
                    this->m_motivazioneReso = in_motivazione_reso;

                    // Log
                    statoReq = statoRequisito::Success;
                    messageLog = "Effettuata reso del prodotto da " + in_nome_utente_compratore;
                    InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
                }

                // Se non esiste nessun utente associato all'id dell'ordine , allora l'ordine non esiste.
                if (rows == 0)
                {
                    std::cout << "L'utente non è stato trovato!" << std::endl;

                    // Log dell'errore e uscita dalla funzione
                    statoReq = statoRequisito::NotSuccess;
                    messageLog = "Utente " + in_nome_utente_compratore + " non trovato";
                    InsertToLogDB(db1, "WARNING", messageLog, sessionID, nomeRequisito, statoReq);
                    
                    result = messageLog;
                    return result;
                }
            }

            // Se lo stato della spedizione non è stato ancora "consegnato" allora il reso non può essere effettuato
            else
            {
                std::cout << "L'ordine è stato spedito, ma non è ancora arrivato, perciò non può essere effettuato il reso!" << std::endl;

                // Log dell'errore e uscita dalla funzione
                statoReq = statoRequisito::NotSuccess;
                messageLog = "Ordine con codice " + std::to_string(in_id_ordine) + " spedito, ma non arrivato, perciò non può essere effettuato il reso";
                InsertToLogDB(db1, "WARNING", messageLog, sessionID, nomeRequisito, statoReq);
                
                result = messageLog;
                return result;
            }
        }

        // Se il numero delle righe del risultato delle query è 0, allora l'ordine non è stato trovato.
        if (rows == 0)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Ordine con codice " + std::to_string(in_id_ordine) + " non è stato ancora spedito (si trova in fase di elaborazione).";
            InsertToLogDB(db1, "WARNING", messageLog, sessionID, nomeRequisito, statoReq);
           
            result = messageLog;
            return result;
        }
        
    std::cout << "Reso effettuato" << std::endl;
    
    result = messageLog;
    return result;
    }
};

#endif // RESO_H
