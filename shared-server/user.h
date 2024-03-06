#ifndef USER_H
#define USER_H

#include "../server-fornitore/src/main.h"

bool isSpecialCharacter(char c)
{
    // Puoi aggiungere altri caratteri speciali se necessario
    return !std::isalnum(c);
}

class Utente
{
public:
    std::string nome_utente;
    std::string categoria;
    std::string nome;
    std::string cognome;
    std::string numero_telefono;
    std::string password;
    std::string email;

    Utente(std::string nome_utente, std::string categoria, std::string nome, std::string cognome, std::string numero_telefono, std::string password, std::string email)
        : nome_utente(nome_utente), categoria(categoria), nome(nome), cognome(cognome), numero_telefono(numero_telefono), password(password), email(email) {}

    void mostraInformazioni()
    {
        std::cout << "Nome utente: " << nome_utente << std::endl;
        std::cout << "Nome: " << nome << std::endl;
        std::cout << "Cognome: " << cognome << std::endl;
        std::cout << "Numero di telefono: " << numero_telefono << std::endl;
        std::cout << "Password: " << password << std::endl;
        std::cout << "Email: " << email << std::endl;
    }


    // Metodo utilizzato per trovare la categoria di un utente dato il suo nome_utente.
    std::string find_categoria_utente(Con2DB db1, std::string input_nome_utente){

        // Variabile per memorizzare la categoria dell'utente
        std::string categoriaUtente = "";

        // Ricerca dell'utente nella tabella UtenteCompratore
        sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe del risultato della query è uguale a 1, l'utente è stato trovato e memorizziamo il risultato
        if (rows==1){
            categoriaUtente = "UtenteCompratore";
        }


        // Ricerca dell'utente nella tabella UtenteFornitore
        sprintf(sqlcmd, "SELECT * FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", input_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe del risultato della query è uguale a 1, l'utente è stato trovato e memorizziamo il risultato
        if (rows==1){
            categoriaUtente = "UtenteFornitore";
        }


        // Ricerca dell'utente nella tabella UtenteTrasportatore
        sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", input_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe del risultato della query è uguale a 1, l'utente è stato trovato e memorizziamo il risultato
        if (rows==1){
            categoriaUtente = "UtenteTrasportatore";
        }

    // Viene restituita la categoria dell'utente trovata.
    return categoriaUtente;
    }



    // Metodo utilizzato per effettuare il login di un utente dato il suo nome utente, la sua password e il sessionID che sarà creato dal server.
    void effettua_login(Con2DB db1, std::string input_nome_utente, std::string input_passw, std::string sessionID)
    {

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Login utente.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";


        // Viene trovata la categoria dell'utente.
        std::string categoriaUtenteLogin = find_categoria_utente(db1, input_nome_utente);

        
        // Controlliamo se l'utente è già loggato, che varia a seconda del valore dell'attributo "stato" all'interno del db.
        int stato_utente;

        // Viene recuperato il valore dell'attributo "stato" a seconda della categoria dell'utente.
        if (categoriaUtenteLogin == "UtenteCompratore") { sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str()); }

        if (categoriaUtenteLogin == "UtenteFornitore") { sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str()); }

        if (categoriaUtenteLogin == "UtenteTrasportatore") { sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str()); }
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe del risultato della query è uguale a 1, si può recuperare il valore dell'attributo "stato".
        if (rows == 1)
        {
            stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

            // Se il valore dell'attributo "stato" è uguale a 1, allora l'utente è già connesso.
            if (stato_utente == 1)
            {
                std::cout << "L'utente è già connesso." << std::endl;

                // Caricamento del sessionID (a seconda della categoria dell'utente) per inserirlo nel log.

                if (categoria == "UtenteCompratore")
                {
                    sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str());
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    if (rows == 1) { sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c")); }
                }


                if (categoria == "UtenteFornitore")
                {
                    sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", input_nome_utente.c_str());
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    if (rows == 1) { sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f")); }
                }

                if (categoria == "UtenteTrasportatore")
                {
                    sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", input_nome_utente.c_str());
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    
                    if (rows == 1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t")); }
                }

                // Log del warning e uscita dalla funzione
                statoReq = statoRequisito::NotSuccess;
                messageLog = "Utente " + input_nome_utente + " già connesso";
                InsertToLogDB(db1, "WARNING", messageLog, sessionID, nomeRequisito, statoReq);
                
                return;
            }

            // Se il valore dell'attributo "stato" è uguale a 0, allora l'utente NON è connesso e dobbiamo effettuare il login
            else
            {
                // Aggiorniamo lo stato dell'utente: da disconnesso a connesso:

                // controllo se il sessionID è univoco.
                bool resultSession = check_sessionID(db1, nomeRequisito, statoReq, sessionID);
                if (resultSession==false){
                    return;
                }

                // Se è il sessionID è univoco lo aggiorniamo nella tabella corrette:
                if (categoriaUtenteLogin == "UtenteCompratore")
                {
                    // Aggiornamento del session id nella tabella dell'utente compratore
                    sprintf(sqlcmd, "UPDATE %s set session_id_c='%s' WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), sessionID.c_str(), input_nome_utente.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Log
                    statoReq = statoRequisito::Wait;
                    messageLog = "Aggiornamento sessionID per utente " + input_nome_utente;
                    InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
                }

                if (categoriaUtenteLogin == "UtenteFornitore")
                {
                    // Aggiornamento del session id nella tabella dell'utente fornitore
                    sprintf(sqlcmd, "UPDATE %s set session_id_f='%s' WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), sessionID.c_str(), input_nome_utente.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Log
                    statoReq = statoRequisito::Wait;
                    messageLog = "Aggiornamento sessionID per utente " + input_nome_utente;
                    InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
                }

                if (categoriaUtenteLogin == "UtenteTrasportatore")
                {
                    // Aggiornamento del session id nella tabella dell'utente trasportatore
                    sprintf(sqlcmd, "UPDATE %s set session_id_t='%s' WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), sessionID.c_str(), input_nome_utente.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Log
                    statoReq = statoRequisito::Wait;
                    messageLog = "Aggiornamento sessionID per utente " + input_nome_utente;
                    InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
                }


                // Viene effettuata la verifica delle password: confrontiamo la password data in input dall'utente che vuole effettuare il login e la password che si trova nel db dell'utente.
                std::string password_utente;
                char *password_u;

                // Viene recuperata la password nel database a seconda della categoria di utente:
                
                if (categoriaUtenteLogin == "UtenteCompratore")
                {
                    sprintf(sqlcmd, "SELECT password FROM %s WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                }

                if (categoriaUtenteLogin == "UtenteFornitore")
                {
                    sprintf(sqlcmd, "SELECT password FROM %s WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                }

                if (categoriaUtenteLogin == "UtenteTrasportatore")
                {
                    sprintf(sqlcmd, "SELECT password FROM %s WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                }

                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);

                // Se il numero di righe del risultato delle query è uguale a 1, la password può essere assegnata a una variabile.
                if (rows == 1)
                {
                    password_u = PQgetvalue(res, 0, PQfnumber(res, "password"));
                    password_utente.assign(password_u);
                }

                // Se il numero di righe del risultato delle query è diverso da 1 l'utente non è stato trovato
                else
                {
                    // Log dell'errore e uscita dalla funzione
                    statoReq = statoRequisito::NotSuccess;
                    InsertToLogDB(db1, "ERROR", "Utente non trovato.", sessionID, nomeRequisito, statoReq);
                    
                    std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                    return;
                }

                PQclear(res);


                // Viene effettuato il confronto della password data in input dall'utente e quella segnata nel database:
                // Se le passowrd non corrispondono allora l'utente non può collegarsi.
                if (input_passw != password_utente)
                {
                    // Log dell'errore e uscita dalla funzione
                    statoReq = statoRequisito::NotSuccess;
                    InsertToLogDB(db1, "ERROR", "Password non corretta", sessionID, nomeRequisito, statoReq);

                    std::cout << "Errore: La passowrd non è corretta, riprovare." << std::endl;
                    return;
                }

                // Se le password corrispondono, l'utente può collegarsi.
                else
                {
                    std::cout << "La passowrd inserita: " << password_utente << "è corretta." << std::endl;

                    // Viene effettuato l'aggiornamento dello stato a seconda della categoria dell'utente:

                    if (categoriaUtenteLogin == "UtenteCompratore")
                    {
                        // Aggiornamento "stato" UtenteCompratore
                        sprintf(sqlcmd, "UPDATE %s set stato = 1 WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                        res = db1.ExecSQLcmd(sqlcmd);
                        PQclear(res);

                        // Log
                        statoReq = statoRequisito::Success;
                        messageLog = "Aggiornamento stato per utente " + input_nome_utente;
                        InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
                    }

                    if (categoriaUtenteLogin == "UtenteFornitore")
                    {
                        // Aggiornamento "stato" UtenteFornitore
                        sprintf(sqlcmd, "UPDATE %s set stato = 1 WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                        res = db1.ExecSQLcmd(sqlcmd);
                        PQclear(res);

                        // Log
                        statoReq = statoRequisito::Success;
                        messageLog = "Aggiornamento stato per utente " + input_nome_utente;
                        InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
                    }

                    if (categoriaUtenteLogin == "UtenteTrasportatore")
                    {
                        // Aggiornamento "stato" UtenteTrasportatore
                        sprintf(sqlcmd, "UPDATE %s set stato = 1 WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                        res = db1.ExecSQLcmd(sqlcmd);
                        PQclear(res);

                        // Log
                        statoReq = statoRequisito::Success;
                        messageLog = "Aggiornamento stato per utente " + input_nome_utente;
                        InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
                    }
                }
            }
        }

        // Se il numero di righe del risultato della query relativo allo "stato" dell'utente è diversa da 1, non si può recuperare il valore dell'attributo "stato".
        else
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "ERROR", "Utente non trovato", "", nomeRequisito, statoReq);
            
            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
            return;
        }

        return;
    }


    // Metodo utilizzato per effettuare il logout di un utente dato il suo nome utente.
    void effettua_logout(Con2DB db1, std::string nomeUtenteLogout)
    {
        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Logout utente.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Viene trovata la categoria dell'utente.
        std::string categoriaUtenteLogin = find_categoria_utente(db1, nomeUtenteLogout);


        // Caricamento del sessionID a seconda della categoria dell'utente. 
        std::string sessionID = "";

        if (categoriaUtenteLogin == "UtenteCompratore") { sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtenteLogout.c_str()); }

        if (categoriaUtenteLogin == "UtenteFornitore") { sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", nomeUtenteLogout.c_str()); }

        if (categoriaUtenteLogin == "UtenteTrasportatore") { sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", nomeUtenteLogout.c_str()); }

        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        if (categoriaUtenteLogin == "UtenteCompratore")
        {
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));} 
        }

        if (categoriaUtenteLogin == "UtenteFornitore")
        {
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));} 
        }

        if (categoriaUtenteLogin == "UtenteTrasportatore")
        {
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));} 
        }

        PQclear(res);                 

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == ""){
            // Log dell'errore e uscita dalla funzione
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non può effettuare il logout .", sessionID, nomeRequisito, statoReq);
            return;
        }


        // Controlliamo se l'utente è già loggato, che varia a seconda del valore dell'attributo "stato" all'interno del db.
        int stato_utente;

        // Viene recuperato il valore dell'attributo "stato" a seconda della categoria dell'utente.
        if (categoriaUtenteLogin == "UtenteCompratore") { sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str()); }

        if (categoriaUtenteLogin == "UtenteFornitore") { sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str()); }

        if (categoriaUtenteLogin == "UtenteTrasportatore") { sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str()); }

        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe del risultato della query è uguale a 1, si può recuperare il valore dell'attributo "stato".
        if (rows == 1)
        {
            stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

            // Se il valore dell'attributo "stato" è uguale a 0, allora l'utente è già disconnesso.
            if (stato_utente == 0)
            {
                // Log dell'errore e uscita dalla funzione
                statoReq = statoRequisito::NotSuccess;
                messageLog = "Utente " + nomeUtenteLogout + " già disconnesso";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
                
                std::cout << "Errore: L'utente è già disconnesso" << std::endl;
                return;
            }

            // Se il valore dell'attributo "stato" è uguale a 1, allora l'utente è connesso e dobbiamo effettuare il logout
            else
            {
                // Aggiorniamo lo stato dell'utente, da connesso a disconnesso:

                if (categoriaUtenteLogin == "UtenteCompratore")
                {
                    // Aggiornamento "stato" UtenteCompratore
                    sprintf(sqlcmd, "UPDATE %s set stato = 0 WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Log
                    statoReq = statoRequisito::Success;
                    messageLog = "Disconnessione per utente: " + nomeUtenteLogout;
                    InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
                }

                if (categoriaUtenteLogin == "UtenteFornitore")
                {
                    // Aggiornamento "stato" UtenteFornitore
                    sprintf(sqlcmd, "UPDATE %s set stato = 0 WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Log
                    statoReq = statoRequisito::Success;
                    messageLog = "Disconnessione per utente: " + nomeUtenteLogout;
                    InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
                }

                if (categoriaUtenteLogin == "UtenteTrasportatore")
                {
                    // Aggiornamento "stato" UtenteTrasportatore
                    sprintf(sqlcmd, "UPDATE %s set stato = 0 WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Log
                    statoReq = statoRequisito::Success;
                    messageLog = "Disconnessione per utente: " + nomeUtenteLogout;
                    InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
                }


                // Viene resettato il sessionID associato all'utente poichè la sessione è finita dato che l'utente si discollega
                nomeRequisito = "Aggiornamento sessionID.";
                
                if (categoriaUtenteLogin == "UtenteCompratore")
                {
                    // Aggiornamento del session id nella tabella dell'utente compratore
                    sprintf(sqlcmd, "UPDATE %s set session_id_c='' WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Log
                    statoReq = statoRequisito::Success;
                    InsertToLogDB(db1, "INFO", "Aggiornamento sessionID", "", nomeRequisito, statoReq);
                }

                if (categoriaUtenteLogin == "UtenteFornitore")
                {
                    // Aggiornamento del session id nella tabella dell'utente fornitore
                    sprintf(sqlcmd, "UPDATE %s set session_id_f='' WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Log
                    statoReq = statoRequisito::Success;
                    InsertToLogDB(db1, "INFO", "Aggiornamento sessionID", "", nomeRequisito, statoReq);
                }

                if (categoriaUtenteLogin == "UtenteTrasportatore")
                {
                    // Aggiornamento del session id nella tabella dell'utente trasportatore
                    sprintf(sqlcmd, "UPDATE %s set session_id_t='' WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Log
                    statoReq = statoRequisito::Success;
                    InsertToLogDB(db1, "INFO", "Aggiornamento sessionID", "", nomeRequisito, statoReq);
                }

            }
        }

        // Se il numero di righe del risultato della query relativo allo "stato" dell'utente è diversa da 1, non si può recuperare il valore dell'attributo "stato".
        else
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "ERROR", "Utente non trovato.", sessionID, nomeRequisito, statoReq);
            
            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
            return;
        }
        return;
    }





    // Metodo utilizzato per effettuare l'eliminazione di un utente dato il suo nome utente.
    void elimina_profilo(Con2DB db1, std::string nomeUtenteDaEliminare)
    {

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Eliminazione profilo.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";


        // Viene trovata la categoria dell'utente.
        std::string categoriaUtenteLogin = find_categoria_utente(db1, nomeUtenteDaEliminare);


        // Caricamento del sessionID a seconda della categoria dell'utente. 
        std::string sessionID = "";

        if (categoriaUtenteLogin == "UtenteCompratore") { sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtenteDaEliminare.c_str()); }

        if (categoriaUtenteLogin == "UtenteFornitore"){ sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", nomeUtenteDaEliminare.c_str()); }

        if (categoriaUtenteLogin == "UtenteTrasportatore") { sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", nomeUtenteDaEliminare.c_str()); }

        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        if (categoriaUtenteLogin == "UtenteCompratore")
        {
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));} 
        }

        if (categoriaUtenteLogin == "UtenteFornitore")
        {
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));} 
        }

        if (categoriaUtenteLogin == "UtenteTrasportatore")
        {
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));} 
        }

        PQclear(res);                 

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == ""){
            // Log dell'errore e uscita dalla funzione
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non può essere eliminato il profilo .", sessionID, nomeRequisito, statoReq);
            return;
        }


        // Viene resettato il sessionID associato all'utente poichè la sessione è finita dato che l'utente si discollega
        nomeRequisito = "Aggiornamento sessionID.";

        if (categoria == "UtenteCompratore")
        {
            // Aggiornamento del session id nella tabella dell'utente compratore
            sprintf(sqlcmd, "UPDATE %s set session_id_c='' WHERE nome_utente_compratore = '%s'", categoria.c_str(), nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            InsertToLogDB(db1, "INFO", "Aggiornamento sessionID", "", nomeRequisito, statoReq);
        }

        if (categoria == "UtenteFornitore")
        {
            // Aggiornamento del session id nella tabella dell'utente fornitore
            sprintf(sqlcmd, "UPDATE %s set session_id_f='' WHERE nome_utente_fornitore = '%s'", categoria.c_str(), nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            InsertToLogDB(db1, "INFO", "Aggiornamento sessionID", "", nomeRequisito, statoReq);
        }

        if (categoria == "UtenteTrasportatore")
        {
            // Aggiornamento del session id nella tabella dell'utente trasportatore
            sprintf(sqlcmd, "UPDATE %s set session_id_t='' WHERE nome_utente_trasportatore = '%s'", categoria.c_str(), nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            InsertToLogDB(db1, "INFO", "Aggiornamento sessionID", "", nomeRequisito, statoReq);
        }


        // Viene effettuata l'eliminazione dell'utente a seconda della categoria
        if (categoria == "UtenteCompratore")
        {
            // Eliminazione utente compratore
            sprintf(sqlcmd, "DELETE FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Eliminazione profilo utente " + nomeUtenteDaEliminare; 
            InsertToLogDB(db1, "INFO", messageLog , sessionID, nomeRequisito, statoReq);
        }

        if (categoria == "UtenteFornitore")
        {
            // Eliminazione utente fornitore
            sprintf(sqlcmd, "DELETE FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Eliminazione profilo utente " + nomeUtenteDaEliminare; 
            InsertToLogDB(db1, "INFO", messageLog , sessionID, nomeRequisito, statoReq);
        }

        if (categoria == "UtenteTrasportatore")
        {
            // Eliminazione utente trasportatore
            sprintf(sqlcmd, "DELETE FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Eliminazione profilo utente " + nomeUtenteDaEliminare; 
            InsertToLogDB(db1, "INFO", messageLog , sessionID, nomeRequisito, statoReq);
        }

        return;
    }



    // Metodo utilizzato per effettuare l'aggiornamento del numero di telefono di un utente.
    void aggiornaNumeroDiTelefono(Con2DB db1, std::string input_nome_utente ,std::string nuovoNumeroTelefono)
    {
        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Aggiornamento numero di telefono.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Viene trovata la categoria dell'utente.
        std::string categoriaUtenteLogin = find_categoria_utente(db1, input_nome_utente);


        // Caricamento del sessionID a seconda della categoria dell'utente. 
        std::string sessionID = "";

        if (categoriaUtenteLogin == "UtenteCompratore") { sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str()); }

        if (categoriaUtenteLogin == "UtenteFornitore") { sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", input_nome_utente.c_str()); }

        if (categoriaUtenteLogin == "UtenteTrasportatore") { sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", input_nome_utente.c_str()); }

        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        if (categoriaUtenteLogin == "UtenteCompratore")
        {
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));} 
        }

        if (categoriaUtenteLogin == "UtenteFornitore")
        {
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));} 
        }

        if (categoriaUtenteLogin == "UtenteTrasportatore")
        {
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));} 
        }

        PQclear(res);                 

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == ""){
            // Log dell'errore e uscita dalla funzione
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non può essere aggiornato il numero di telefono .", sessionID, nomeRequisito, statoReq);
            return;
        }

        // Viene aggiornato il numero di telefono a seconda della categoria dell'utente
        if (categoria == "UtenteCompratore")
        {
            // Aggiornamento del numero di telefono dell'utente compratore.
            sprintf(sqlcmd, "UPDATE UtenteCompratore set numero_di_telefono = '%s' WHERE nome_utente_compratore = '%s'", nuovoNumeroTelefono.c_str(), input_nome_utente.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Aggiornamento numero di telefono utente: " + input_nome_utente;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }
        if (categoria == "UtenteFornitore")
        {
            // Aggiornamento del numero di telefono dell'utente fornitore.
            sprintf(sqlcmd, "UPDATE UtenteFornitore set numero_di_telefono = '%s' WHERE nome_utente_fornitore = '%s'", nuovoNumeroTelefono.c_str(), input_nome_utente.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Aggiornamento numero di telefono utente: " + input_nome_utente;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }
        if (categoria == "UtenteTrasportatore")
        {
            // Aggiornamento del numero di telefono dell'utente trasportatore.
            sprintf(sqlcmd, "UPDATE UtenteTrasportatore set numero_di_telefono = '%s' WHERE nome_utente_trasportatore = '%s'", nuovoNumeroTelefono.c_str(), input_nome_utente.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Aggiornamento numero di telefono utente: " + input_nome_utente;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }
        return;
    }



    // Metodo utilizzato per effettuare l'aggiornamento della password di un utente.
    void aggiornaPassword(Con2DB db1, std::string input_nome_utente, std::string vecchiaPassw, std::string nuovaPassw)
    {
        std::string passwDB = "";

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Aggiornamento password.";
        statoRequisito statoReq = statoRequisito::Wait;


        // Viene trovata la categoria dell'utente.
        std::string categoriaUtenteLogin = find_categoria_utente(db1, input_nome_utente);


        // Caricamento del sessionID a seconda della categoria dell'utente. 
        std::string sessionID = "";


        if (categoriaUtenteLogin == "UtenteCompratore") { sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str()); }

        if (categoriaUtenteLogin == "UtenteFornitore") { sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", input_nome_utente.c_str()); }

        if (categoriaUtenteLogin == "UtenteTrasportatore") { sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", input_nome_utente.c_str()); }

        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        if (categoriaUtenteLogin == "UtenteCompratore")
        {
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));} 
        }

        if (categoriaUtenteLogin == "UtenteFornitore")
        {
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));} 
        }

        if (categoriaUtenteLogin == "UtenteTrasportatore")
        {
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));} 
        }

        PQclear(res);                 

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == ""){
            // Log dell'errore e uscita dalla funzione
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non può essere aggiornata la password .", sessionID, nomeRequisito, statoReq);
            return;
        }


        // Verifichiamo che la vecchiaPassword inserita dall'utente è uguale a quella nel database relativa all'utente che vuole aggiornarla:

        // Recuperiamo la password dell'utente all'interno del db:
        if (categoriaUtenteLogin == "UtenteCompratore") { sprintf(sqlcmd, "SELECT password FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str()); }

        if (categoriaUtenteLogin == "UtenteFornitore") { sprintf(sqlcmd, "SELECT password FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", input_nome_utente.c_str()); }

        if (categoriaUtenteLogin == "UtenteTrasportatore") { sprintf(sqlcmd, "SELECT password FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", input_nome_utente.c_str()); }

        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        if (categoriaUtenteLogin == "UtenteCompratore")
        {
            if (rows==1){ passwDB = PQgetvalue(res, 0, PQfnumber(res, "password"));} 
        }

        if (categoriaUtenteLogin == "UtenteFornitore")
        {
            if (rows==1){ passwDB = PQgetvalue(res, 0, PQfnumber(res, "password"));} 
        }

        if (categoriaUtenteLogin == "UtenteTrasportatore")
        {
            if (rows==1){ passwDB = PQgetvalue(res, 0, PQfnumber(res, "password"));} 
        }

        PQclear(res);  


        // Se la password del db è diversa dalla password inserita dall'utente allora non può essere effettuata l'azione
        if (passwDB != vecchiaPassw)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "ERROR", "La password attuale inserita non è corretta.", sessionID, nomeRequisito, statoReq);
            
            std::cout << "Errore: La password attuale inserita non è corretta" << std::endl;
            return;
        }

        // Se la password del db è uguale alla password inserita dall'utente allora può essere aggiornata quest'ultima
        else
        {
            // Controllo se la nuova password rispetta i criteri: lunghezza di almeno 8, almeno una lettere maiuscola, un numero e un carattere speciale.
            

            // Verifica se la lunghezza della nuova password è inferiore a 8 caratteri
            if (nuovaPassw.length() < 8)
            {
                // Se la password è troppo corta, impostiamo lo stato del requisito a NotSuccess
                statoReq = statoRequisito::NotSuccess;

                // Registra un errore nel log del database e stampa un messaggio di errore
                InsertToLogDB(db1, "ERROR", "La nuova passowrd deve contenere almeno 8 caratteri.", sessionID, nomeRequisito, statoReq);
                std::cout << "Errore: La nuova passowrd deve contenere almeno 8 caratteri." << std::endl;

                return;
            }

            // Verifica se la password contiene almeno un carattere maiuscolo, un numero e un carattere speciale
            bool hasUpperCase = false;
            bool hasDigit = false;
            bool hasSpecialChar = false;
            for (size_t i = 0; i < nuovaPassw.length(); i++)
            {
                if (std::isupper(nuovaPassw[i]))
                {
                    hasUpperCase = true;
                }

                else if (std::isdigit(nuovaPassw[i]))
                {
                    hasDigit = true;
                }

                else if (isSpecialCharacter(nuovaPassw[i]))
                {
                    hasSpecialChar = true;
                }
            }

            // Verifica se la password contiene almeno un carattere maiuscolo
            if (!hasUpperCase)
            {
                // Stampa un messaggio di errore e registra l'errore nel log del database
                std::cout << "La nuova password deve contenere almeno un carattere maiuscolo." << std::endl;

                statoReq = statoRequisito::NotSuccess;
                InsertToLogDB(db1, "ERROR", "La nuova passowrd deve contenere almeno un carattere maiuscolo.", sessionID, nomeRequisito, statoReq);
            }

            // Verifica se la password contiene almeno un numero
            if (!hasDigit)
            {
                // Stampa un messaggio di errore e registra l'errore nel log del database
                std::cout << "La nuova password deve contenere almeno un numero." << std::endl;

                statoReq = statoRequisito::NotSuccess;
                InsertToLogDB(db1, "ERROR", "La nuova passowrd deve contenere almeno un numero.", sessionID, nomeRequisito, statoReq);
            }

            if (!hasSpecialChar)
            {
                // Stampa un messaggio di errore e registra l'errore nel log del database
                std::cout << "La nuova password deve contenere almeno un carattere speciale." << std::endl;

                statoReq = statoRequisito::NotSuccess;
                InsertToLogDB(db1, "ERROR", "La nuova passowrd deve contenere almeno un carattere speciale.", sessionID, nomeRequisito, statoReq);
            }


            // Aggiorniamo la password dell'utente a seconda della categoria di quest'ultimo.
            if (categoria == "UtenteCompratore")
            {
                // Aggiornamento "password" UtenteCompratore
                sprintf(sqlcmd, "UPDATE UtenteCompratore set password = '%s' WHERE nome_utente_compratore = '%s'", nuovaPassw.c_str(), input_nome_utente.c_str());
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                // Log
                statoReq = statoRequisito::Success;
                InsertToLogDB(db1, "INFO", "Aggiornamento password.", sessionID, nomeRequisito, statoReq);
            }
            if (categoria == "UtenteFornitore")
            {
                // Aggiornamento "password" UtenteFornitore
                sprintf(sqlcmd, "UPDATE UtenteFornitore set password = '%s' WHERE nome_utente_fornitore = '%s'", nuovaPassw.c_str(), input_nome_utente.c_str());
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                // Log
                statoReq = statoRequisito::Success;
                InsertToLogDB(db1, "INFO", "Aggiornamento password.", sessionID, nomeRequisito, statoReq);
            }
            if (categoria == "UtenteTrasportatore")
            {
                // Aggiornamento "password" UtenteTrasportatore
                sprintf(sqlcmd, "UPDATE UtenteTrasportatore set password = '%s' WHERE nome_utente_trasportatore = '%s'", nuovaPassw.c_str(), input_nome_utente.c_str());
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                // Log
                statoReq = statoRequisito::Success;
                InsertToLogDB(db1, "INFO", "Aggiornamento password.", sessionID, nomeRequisito, statoReq);
            }
        }

        return;
    }




    // Metodo per verificare se un sessionID è univoco tra gli utenti compratori, fornitori e trasportatori
    bool check_sessionID(Con2DB db1, std::string nomeRequisito, statoRequisito statoReq, std::string sessionID){
        bool result = true;
    
        // Verifico se il sessionID è già presente nella tabella UtenteCompratore
        sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE session_id_c = '%s'", sessionID.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        // Se il numero di righe dal risultato della query è maggiore di 0, il sessionID già esiste nel database, ed è stato assegnato ad un altro utente compratore.
        if (rows > 0)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB(db1,"ERROR", "Il session ID è già in uso da utenti compratori.", sessionID, nomeRequisito, statoReq);
            
            std::cout << "Errore: Il session ID è già in uso da utenti compratori." << std::endl;
            return false;
        }


        // Verifico se il sessionID è già presente nella tabella UtenteFornitore
        sprintf(sqlcmd, "SELECT * FROM UtenteFornitore WHERE session_id_f = '%s'", sessionID.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        // Se il numero di righe dal risultato della query è maggiore di 0, il sessionID già esiste nel database, ed è stato assegnato ad un altro utente fornitore.
        if (rows > 0)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB(db1,"ERROR", "Il session ID è già in uso da utenti fornitori.", sessionID, nomeRequisito, statoReq);
            
            std::cout << "Errore: Il session ID è già in uso da utenti fornitori." << std::endl;
            return false;
        }

        // Verifico se il sessionID è già presente nella tabella UtenteTrasportatore
        sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore WHERE session_id_t = '%s'", sessionID.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        // Se il numero di righe dal risultato della query è maggiore di 0, il sessionID già esiste nel database, ed è stato assegnato ad un altro utente trasportatore.
        if (rows > 0)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB(db1,"ERROR", "Il session ID è già in uso da utenti trasportatori.", sessionID, nomeRequisito, statoReq);
            
            std::cout << "Errore: Il session ID è già in uso da utenti trasportatori." << std::endl;
            return false;
        }

    // Restituiamo true se il sessionID è univoco tra tutti gli utenti
    return result;
    }




  /*
    UtenteCompratore animaUtenteCompratore(Con2DB db1, std::string input_nome_utente)
    {
        sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        UtenteCompratore compratore;

        if (rows == 1)
        {
            std::string nome_utente;
            nome_utente = (PQgetvalue(res, 0, PQfnumber(res, "nome_utente_compratore")));
            std::string session_id = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));
            std::string categoria = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));
            std::string nome = PQgetvalue(res, 0, PQfnumber(res, "nome"));
            std::string cognome = PQgetvalue(res, 0, PQfnumber(res, "cognome"));
            std::string email = PQgetvalue(res, 0, PQfnumber(res, "indirizzo_mail"));
            std::string numero_telefono = PQgetvalue(res, 0, PQfnumber(res, "numero_di_telefono"));
            std::string password = PQgetvalue(res, 0, PQfnumber(res, "password"));
            std::string data_compleanno = PQgetvalue(res, 0, PQfnumber(res, "data_compleanno"));
            std::string via_residenza = PQgetvalue(res, 0, PQfnumber(res, "via_di_residenza"));
            std::string numero_civico = PQgetvalue(res, 0, PQfnumber(res, "numero_civico"));
            std::string CAP = PQgetvalue(res, 0, PQfnumber(res, "CAP"));
            std::string città_residenza = PQgetvalue(res, 0, PQfnumber(res, "citta_di_residenza"));
            int stato = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

            compratore.nome_utente = nome_utente;
            compratore.session_id = session_id;
            compratore.categoria = categoria;
            compratore.nome = nome;
            compratore.cognome = cognome;
            compratore.email = email;
            compratore.numero_telefono = numero_telefono;
            compratore.password = password;
            compratore.data_compleanno = data_compleanno;
            compratore.via_residenza = via_residenza;
            compratore.numero_civico = numero_civico;
            compratore.CAP = CAP;
            compratore.città_residenza = città_residenza;
            compratore.stato = stato;
        }
        else
        {
            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
        }

        PQclear(res);
        return compratore;
    }

    void animaUtenteFornitore(Con2DB db1, std::string input_nome_utente)
    {
        sprintf(sqlcmd, "SELECT * FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", input_nome_utente.c_str());

        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        if (rows == 1)
        {
            std::string nome_utente = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_fornitore"));
            std::string session_id = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));
            std::string categoria = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));
            std::string nome = PQgetvalue(res, 0, PQfnumber(res, "nome"));
            std::string cognome = PQgetvalue(res, 0, PQfnumber(res, "cognome"));
            std::string email = PQgetvalue(res, 0, PQfnumber(res, "indirizzo_mail"));
            std::string numero_telefono = PQgetvalue(res, 0, PQfnumber(res, "numero_di_telefono"));
            std::string azienda_produzione = PQgetvalue(res, 0, PQfnumber(res, "nome_AziendaProduttrice"));
            std::string password = PQgetvalue(res, 0, PQfnumber(res, "password"));
            int stato = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

            //*this = UtenteFornitore(nome_utente, categoria, nome, cognome, numero_telefono, password, email, session_id, azienda_produzione, stato);
        }
        else
        {
            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
        }

        PQclear(res);
    }

    void animaUtenteTrasportatore(Con2DB db1, std::string input_nome_utente)
    {
    }
    */










};

#endif // USER_H