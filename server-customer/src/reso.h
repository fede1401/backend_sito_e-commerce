// Reso.h
#ifndef RESO_H
#define RESO_H

#include "main.h"
#include "../../shared-server/statoMotivazioneResoToString.h"



class Reso {
public:
    int idReso;
    std::string nome_utente_compratore;
    int idOrdine;
    motivazioneReso motivazione_reso;

    // Costruttori:
    Reso():
        idReso(-1),
        nome_utente_compratore(""),
        idOrdine(-1), 
        motivazione_reso() {}


    Reso(int idReso, std::string nome_utente_compratore, int idOrdine, motivazioneReso motivazione_reso): 
        idReso(idReso), 
        nome_utente_compratore(nome_utente_compratore),
        idOrdine(idOrdine), 
        motivazione_reso(motivazione_reso) {}


    void impostaStato(motivazioneReso nuovoStato) {   
      motivazione_reso = nuovoStato;    
    }


    // Funzione utilizzata per permettere ad un utente compratore di effettuate un reso di un ordine.
    void effettuaReso(Con2DB db1, std::string in_nome_utente_compratore, int idOrdine, motivazioneReso motivazione_reso){

        std::string sessionID = "";
        std::string stato_spedizione;


        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Effettuazione Reso.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";


        // Recupero dello stato della spedizione dell'ordine tramite il suo id per verificare se l'ordine è stato spedito e arrivato correttamente.
        sprintf(sqlcmd, "SELECT statoSpedizione FROM Spedizione WHERE idOrdine = '%d'", idOrdine);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows==1){
            stato_spedizione = PQgetvalue(res, 0, PQfnumber(res, "statoSpedizione"));
            PQclear(res);

            // Se lo stato della spedizione è "consegnato"
            if (stato_spedizione == "consegnato"){

              // Selezioniamo il nome del'utente compratore che ha effettuato l'ordine tramite il suo id.
              std::string nome_utente_compratore;
              sprintf(sqlcmd, "SELECT nome_utente_compratore FROM Ordine WHERE idOrdine = '%d'", idOrdine);
              res = db1.ExecSQLtuples(sqlcmd);
              rows = PQntuples(res);
              if (rows == 1)
                {
                    nome_utente_compratore = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_compratore"));
                    PQclear(res);

                    // Se il nome dell'utente compratore che ha effettuato l'ordine è diverso dall'utente che vuole effettuare un reso, allora non è possibile effettuarlo.
                    if (in_nome_utente_compratore != nome_utente_compratore)
                    {
                        // Log dell'errore e uscita dalla funzione
                        messageLog = "Utente che sta cercando di effettuare il reso ( " + in_nome_utente_compratore + ") non corrisponde a quello dell ordine ( " + nome_utente_compratore+ ").";
                        InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
                        return;
                    }

                    // Caricamento del sessionID.
                    sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nome_utente_compratore.c_str());
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));}  
                    PQclear(res);     

                    if (rows != 1){
                        // Log dell'errore e uscita dalla funzione
                        messageLog = "Non esiste " + in_nome_utente_compratore + " , poichè non è stato registrato, non può essere effettuato il reso .";
                        InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
                        return;
                        }   

                    // Verifica se l'utente è loggato e ha una sessionID valida
                    if (sessionID == ""){
                        // Log dell'errore e uscita dalla funzione
                        InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato, non si può effettuare un reso .", sessionID, nomeRequisito, statoReq);
                        return;
                    }

                    // Rendiamo la motivazione del reso in stringa così che posso aggiungerlo al database.
                    std::string motivazione_resoStr = statoMotivazioneResoToString(motivazione_reso);


                    // Inserisco nel database una riga corrispondente al reso.  
                    sprintf(sqlcmd, "INSERT INTO Reso (idReso, nome_utente_compratore, idOrdine, motivazioneReso) VALUES (DEFAULT, '%s', '%d', '%s')", 
                    nome_utente_compratore.c_str(), idOrdine, motivazione_resoStr.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res); 

                    // Devo riempire l'oggetto Reso della classe:
                    // Esegui una query SELECT per ottenere l'ultimo ID inserito nella tabella Reso:
                    // 1. Selezioniamo tutti gli idReso dalla tabella Recensione:
                    sprintf(sqlcmd, "SELECT idReso FROM Reso");
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    // 2. Prendiamo l'ultimo id
                    this->idReso = atoi(PQgetvalue(res, rows - 1, 0));
                    PQclear(res);

                    this->nome_utente_compratore = in_nome_utente_compratore;
                    this->idOrdine = idOrdine;
                    this->motivazione_reso = motivazione_reso;

                    // Log 
                    statoReq = statoRequisito::Success;
                    messageLog = "Effettuata reso del prodotto da " + in_nome_utente_compratore;
                    InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

                }

                // Se non esiste nessun utente associato all'id dell'ordine , allora l'ordine non esiste.
                else{
                    std::cout << "L'ordine non è stato trovato!" << std::endl;

                    // Log dell'errore e uscita dalla funzione
                    statoReq = statoRequisito::NotSuccess;
                    messageLog = "Ordine con codice " + std::to_string(idOrdine) + " non trovato";
                    InsertToLogDB(db1, "WARNING", messageLog, sessionID, nomeRequisito, statoReq);
                    return;
                }
            }

            // Se lo stato della spedizione non è stato ancora "consegnato" allora il reso non può essere effettuato
            else{
                std::cout << "L'ordine è stato spedito, ma non è ancora arrivato, perciò non può essere effettuato il reso!" << std::endl;

                // Log dell'errore e uscita dalla funzione
                statoReq = statoRequisito::NotSuccess;
                messageLog = "Ordine con codice " + std::to_string(idOrdine) + " spedito, ma non arrivato, perciò non può essere effettuato il reso";
                InsertToLogDB(db1, "WARNING", messageLog, sessionID, nomeRequisito, statoReq);
                return;
            }
        }

        // Se il numero delle righe del risultato delle query è diverso da 1, allora l'ordine non è stato trovato
        else{

            std::cout << "L'ordine non è stato trovato!" << std::endl;

            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Ordine con codice " + std::to_string(idOrdine) + " non trovato";
            InsertToLogDB(db1, "WARNING", messageLog, sessionID, nomeRequisito, statoReq);
            return;

        }
    std::cout << "Reso effettuato" << std::endl;
    return;
    }



};

#endif // RESO_H
