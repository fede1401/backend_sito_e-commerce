// Carta.h
#ifndef CARTA_H
#define CARTA_H

#include "main.h"


class Carta {
public:
    // Attributi per la classe Carta.
    std::string nome_utente;
    std::string numero_carta;
    std::string cvv;


    // Costruttore
    Carta():
        nome_utente(""),
        numero_carta(""),
        cvv("")
        {}

    Carta(std::string nome_utente, std::string numero, std::string codice): 
        nome_utente(nome_utente), 
        numero_carta(numero), 
        cvv(codice) {}


    // Funzione per aggiungere una carta di pagamento dato il nome dell'utente, il numero della carta e il cvv.
    std::string aggiungi_carta(Con2DB db1, std::string in_nome_utente, std::string in_numeroCarta, std::string in_cvv){

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Aggiunta carta di pagamento.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        // Caricamento del sessionID.
        std::string sessionID = "";
        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, si può recuperare il sessionID.
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id"));}  
        PQclear(res);   

        // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
        if (rows == 0){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste " + in_nome_utente + " , poichè non è stato registrato, non può essere aggiunta la carta di pagamento.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        } 

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == ""){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + in_nome_utente + ", utente non loggato, non può essere aggiunta la carta di pagamento.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }


        // Verifichiamo che l'utente si tratti di un utente compratore:
        std::string categoriaUtente = "";
        sprintf(sqlcmd, "SELECT categoriaUtente FROM Utente WHERE nome_utente = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, si può recuperare la categoria dell'utente.
        if (rows==1){ categoriaUtente = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));}  
        PQclear(res);

        if (categoriaUtente != "UtenteCompratore"){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "L utente " + in_nome_utente + " non è un utente compratore, perciò non può essere aggiunta la carta di pagamento";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // NON è necessario verificare che ci siano carte con numero di carta univoco, ci potrebbero essere utenti che inseriscono la stessa carta.

        // Il nome utente è di un utente compratore --> possiamo aggiungere la carta di pagamento al database:
        sprintf(sqlcmd, "INSERT INTO Carta (idCarta, nome_utente_compratore, numeroCarta, cvv) VALUES (DEFAULT, '%s', '%s', '%s')", in_nome_utente.c_str(), in_numeroCarta.c_str(), in_cvv.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res); 

        // Riempio i campi della classe
        this->nome_utente = in_nome_utente;
        this->numero_carta = in_numeroCarta;
        this-> cvv = in_cvv;

        // Log
        statoReq = statoRequisito::Success;
        messageLog = "Inserimento carta di pagamento con il numero " + in_numeroCarta + " per utente compratore " + in_nome_utente;
        InsertToLogDB(db1, "INFO", messageLog , sessionID, nomeRequisito, statoReq);
        
    result = messageLog;
    return result;
    }


    // Funzione per rimuovere la carta di pagamento dell'utente compratore con un determinato id di carta.
    std::string remove_carta(Con2DB db1, std::string in_nome_utente_compratore, int in_idCarta)
    {
        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Rimozione carta di pagamento.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        // Caricamento del sessionID.
        std::string sessionID = "";
        // Ottieni il sessionID dall'utente compratore nel database
        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, possiamo recuperare il sessionID.
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id"));} 
        PQclear(res);  
        
        // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
        if (rows == 0){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste " + in_nome_utente_compratore + " , poichè non è stato registrato, non può essere rimossa la carta di pagamento.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        } 

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == ""){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + in_nome_utente_compratore + ", utente non loggato, non può essere rimossa la carta di pagamento.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }


        // Verifichiamo che l'utente si tratti di un utente compratore:
        std::string categoriaUtente = "";
        sprintf(sqlcmd, "SELECT categoriaUtente FROM Utente WHERE nome_utente = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, possiamo recuperare la categoria dell'utente.
        if (rows==1){ categoriaUtente = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));}  
        PQclear(res);

        if (categoriaUtente != "UtenteCompratore"){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "L utente " + in_nome_utente_compratore + " non è un utente compratore, perciò non può essere rimossa la carta di pagamento";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }


        // Verifica se la carta esiste nel database
        sprintf(sqlcmd, "SELECT * FROM Carta WHERE idCarta = '%d'", in_idCarta);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        // Se il numero di righe del risultato della query è <1, non esiste la carta di pagamento con quell'id.
        if (rows < 1){
            // La carta non esiste nel database, log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Carta con id " + std::to_string (in_idCarta) + " non trovata";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            std::cout << "La riga da eliminare non esiste!" << std::endl;
            
            result = messageLog;
            return result;
        }
        else{
            // La carta esiste nel database, possiamo eliminarla
            sprintf(sqlcmd, "DELETE FROM Carta WHERE idCarta = '%d'", in_idCarta);
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log della rimozione della carta
            statoReq = statoRequisito::Success;
            messageLog = "Eliminazione carta di pagamento " + std::to_string (in_idCarta) + " dell utente compratore " + in_nome_utente_compratore;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }
    
    result = messageLog;
    return result;
    }



};

#endif // CARTA_H
