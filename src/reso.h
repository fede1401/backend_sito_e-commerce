// Reso.h
#ifndef RESO_H
#define RESO_H

#include "main.h"


enum class motivazioneReso {
    Difettoso,
    MisuraErrata,
    NonConformeAlleAspettative,
    CambioOpinione
    };


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


    void effettuaReso(int idOrdine, motivazioneReso motivazione_reso){

        std::string sessionID = "";

        std::string nomeRequisito = "Effettuazione Reso.";
        statoRequisito statoReq = statoRequisito::Wait;

        // Connession al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        std::string stato_spedizione;

        // Innanzitutto controllo se l'ordine è stato spedito e arrivato correttamente
        sprintf(sqlcmd, "SELECT statoSpedizione FROM Spedizione WHERE idOrdine = '%d'", idOrdine);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        if (rows==1){
            stato_spedizione = PQgetvalue(res, 0, PQfnumber(res, "statoSpedizione"));

            if (stato_spedizione == "consegnato"){

                // Selezioniamo il nome del'utente compratore:
              std::string nome_utente_compratore;
              sprintf(sqlcmd, "SELECT nome_utente_compratore FROM Ordine WHERE idOrdine = '%d'", idOrdine);
              res = db1.ExecSQLtuples(sqlcmd);
              rows = PQntuples(res);
              if (rows == 1){
                  nome_utente_compratore = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_compratore"));

                  // Caricamento del sessionID utile per il log.
                  sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nome_utente_compratore.c_str());
                  res = db1.ExecSQLtuples(sqlcmd);
                  rows = PQntuples(res);
                                        
                  if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));}  
                    
                  std::string motivazione_resoStr = statoMotivazioneResoToString(motivazione_reso);
                    
                  sprintf(sqlcmd, "INSERT INTO Reso (idReso, nome_utente_compratore, idOrdine, motivazioneReso) VALUES (DEFAULT, '%s', '%d', '%s')", 
                  nome_utente_compratore.c_str(), idOrdine, motivazione_resoStr.c_str());
                  res = db1.ExecSQLcmd(sqlcmd);
                  PQclear(res); 

                  statoReq = statoRequisito::Success;

                  InsertToLogDB("INFO", "Effettuata reso del prodotto", sessionID, nomeRequisito, statoReq);

                }
                else{
                    std::cout << "L'ordine non è stato trovato!" << std::endl;

                    statoReq = statoRequisito::NotSuccess;

                    InsertToLogDB("WARNING", "Ordine non trovato", sessionID, nomeRequisito, statoReq);
                    return;
                }
            }
            else{
                std::cout << "L'ordine è stato spedito, ma non è ancora arrivato, perciò non può essere effettuato il reso!" << std::endl;

                statoReq = statoRequisito::NotSuccess;

                InsertToLogDB("WARNING", "Ordine spedito, ma non arrivato, perciò non può essere effettuato il reso", sessionID, nomeRequisito, statoReq);
                return;
            }
        }
        else{
            std::cout << "L'ordine non è stato ancora spedito, perciò non può essere effettuato il reso!" << std::endl;

            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB("WARNING", "Ordine non spedito, non può essere effettuato il reso", sessionID, nomeRequisito, statoReq);
            return;
        }
    
    std::cout << "Reso effettuato" << std::endl;
    return;
    }



    std::string statoMotivazioneResoToString(motivazioneReso stato) {
        switch (stato) {
            case motivazioneReso::Difettoso :
                return "difettoso";
            case motivazioneReso::MisuraErrata :
                return "misura errata";
            case motivazioneReso::NonConformeAlleAspettative :
                return "non conforme alle aspettative";
            case motivazioneReso::CambioOpinione :
                return "cambio opinione";
            default:
                return ""; // gestione degli errori o valori non validi
        }
    }



};

#endif // RESO_H
