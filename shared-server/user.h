#ifndef USER_H
#define USER_H

#include "../server-fornitore/src/main.h"
#include "isSpecialCharacter.h"

class Utente
{


public:
    std::string nome_utente;
    std::string session_id;
    std::string categoria;
    std::string nome;
    std::string cognome;
    std::string numero_telefono;
    std::string password;
    std::string email;
    int stato;

    Utente() : nome_utente(""),
               session_id(""),
               categoria(""),
               nome(""),
               cognome(""),
               numero_telefono(""),
               password(""),
               email(""),
               stato(-1) {}

    Utente(std::string nome_utente, std::string session_id, std::string categoria, std::string nome, std::string cognome, std::string numero_telefono, std::string password, std::string email, int stato)
        : nome_utente(nome_utente), session_id(session_id), categoria(categoria), nome(nome), cognome(cognome), numero_telefono(numero_telefono), password(password), email(email), stato(stato) {}

    void mostraInformazioni()
    {
        std::cout << "Nome utente: " << nome_utente << std::endl;
        std::cout << "Nome: " << nome << std::endl;
        std::cout << "Cognome: " << cognome << std::endl;
        std::cout << "Numero di telefono: " << numero_telefono << std::endl;
        std::cout << "Password: " << password << std::endl;
        std::cout << "Email: " << email << std::endl;
    }

    // Metodo utilizzato per effettuare il login di un utente dato il suo nome utente, la sua password e il sessionID che sarà creato dal server.
    virtual std::string effettua_login(Con2DB db1, std::string input_nome_utente, std::string input_passw, std::string sessionID)
    {
        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Login utente.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Controlliamo se l'utente è già loggato, che varia a seconda del valore dell'attributo "stato" all'interno del db.
        int stato_utente;

        // Viene recuperato il valore dell'attributo "stato".
        sprintf(sqlcmd, "SELECT stato FROM Utente WHERE nome_utente = '%s'", input_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe del risultato della query relativo allo "stato" dell'utente è 0, non si può recuperare il valore dell'attributo "stato".
        if (rows == 0)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Utente " + input_nome_utente + " non trovato";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);

            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
            throw std::runtime_error("Errore: L'utente non è stato trovato.");
            //return;
        }

        // else: Se il numero di righe del risultato della query è uguale a 1, si può recuperare il valore dell'attributo "stato".
        if (rows == 1){
            stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));
        }
        PQclear(res);
        

        // Se il valore dell'attributo "stato" è uguale a 1, allora l'utente è già connesso.
        if (stato_utente == 1)
        {
            std::cout << "L'utente è già connesso." << std::endl;

            // Caricamento del sessionID per inserirlo nel log.
            sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", input_nome_utente.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows == 1)
            {
                sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id"));
            }

            // Log del warning e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Utente " + input_nome_utente + " già connesso";
            InsertToLogDB(db1, "WARNING", messageLog, sessionID, nomeRequisito, statoReq);

            // Animo l'oggetto Utente
            sprintf(sqlcmd, "SELECT * FROM Utente WHERE nome_utente = '%s'", input_nome_utente.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);

            // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
            if (rows == 0){
                // Log dell'errore e uscita dalla funzione
                statoReq = statoRequisito::NotSuccess;
                messageLog = "Utente " + input_nome_utente + " non trovato.";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
                throw std::runtime_error("Errore: Utente non trovato.");
                //return;
            }

            if (rows == 1)
            {
                std::string nome_utente;
                nome_utente = (PQgetvalue(res, 0, PQfnumber(res, "nome_utente")));
                std::string session_id = PQgetvalue(res, 0, PQfnumber(res, "session_id"));
                std::string categoria = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));
                std::string nome = PQgetvalue(res, 0, PQfnumber(res, "nome"));
                std::string cognome = PQgetvalue(res, 0, PQfnumber(res, "cognome"));
                std::string email = PQgetvalue(res, 0, PQfnumber(res, "indirizzo_mail"));
                std::string numero_telefono = PQgetvalue(res, 0, PQfnumber(res, "numero_di_telefono"));
                std::string password = PQgetvalue(res, 0, PQfnumber(res, "password"));
                int stato = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

                // Aggiorniamo l'oggetto chiamante con i dettagli dell'utente trovato
                this->nome_utente = nome_utente;
                this->session_id = session_id;
                this->nome = nome;
                this->cognome = cognome;
                this->email = email;
                this->numero_telefono = numero_telefono;
                this->password = password;
                this->stato = stato;
            }

            PQclear(res);

            return "Utente già connesso";
        }

        // else :Se il valore dell'attributo "stato" è uguale a 0, allora l'utente NON è connesso e dobbiamo effettuare il login

        // Aggiorniamo lo stato dell'utente: da disconnesso a connesso:

        // Viene effettuata la verifica delle password: confrontiamo la password data in input dall'utente che vuole effettuare il login e la password che si trova nel db dell'utente.
        std::string password_utente;
        char *password_u;

        // Viene recuperata la password nel database
        sprintf(sqlcmd, "SELECT password FROM Utente WHERE nome_utente = '%s'", input_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe del risultato delle query è uguale a 1, la password può essere assegnata a una variabile.
        if (rows == 1)
        {
            password_u = PQgetvalue(res, 0, PQfnumber(res, "password"));
            password_utente.assign(password_u);
        }

        // Se il numero di righe del risultato delle query è 0 l'utente non è stato trovato
        if (rows == 0)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Utente " + input_nome_utente + " non trovato";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
            throw std::runtime_error("Errore: L'utente non è stato trovato.");
            //return;
        }

        PQclear(res);

        // Viene effettuato il confronto della password data in input dall'utente e quella segnata nel database:
        // Se le passowrd non corrispondono allora l'utente non può collegarsi.
        if (input_passw != password_utente)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Password " + input_passw + " non corretta.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

            std::cout << "Errore: La passowrd non è corretta, riprovare." << std::endl;
            throw std::runtime_error("Errore: La passowrd non è corretta, riprovare.");
            //return;
        }

        // else :Se le password corrispondono, l'utente può collegarsi.

        std::cout << "La passowrd inserita: " << password_utente << "è corretta." << std::endl;

        // Aggiornamento del session id nella tabella dell'utente.
        sprintf(sqlcmd, "UPDATE Utente set session_id='%s' WHERE nome_utente = '%s'", sessionID.c_str(), input_nome_utente.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Log
        statoReq = statoRequisito::Wait;
        messageLog = "Aggiornamento sessionID per utente " + input_nome_utente;
        InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

        // Viene effettuato l'aggiornamento dello stato.

        // Aggiornamento "stato" Utente
        sprintf(sqlcmd, "UPDATE Utente set stato = 1 WHERE nome_utente = '%s'", input_nome_utente.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Log
        statoReq = statoRequisito::Success;
        messageLog = "Aggiornamento stato per utente " + input_nome_utente;
        InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

        // Animo l'oggetto Utente
        sprintf(sqlcmd, "SELECT * FROM Utente WHERE nome_utente = '%s'", input_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
        if (rows == 0){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Utente " + input_nome_utente + " non trovato.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            throw std::runtime_error("Errore: Utente non trovato.");
            //return;
        }

        if (rows == 1)
        {
            std::string nome_utente;
            nome_utente = (PQgetvalue(res, 0, PQfnumber(res, "nome_utente")));
            std::string session_id = PQgetvalue(res, 0, PQfnumber(res, "session_id"));
            std::string categoria = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));
            std::string nome = PQgetvalue(res, 0, PQfnumber(res, "nome"));
            std::string cognome = PQgetvalue(res, 0, PQfnumber(res, "cognome"));
            std::string email = PQgetvalue(res, 0, PQfnumber(res, "indirizzo_mail"));
            std::string numero_telefono = PQgetvalue(res, 0, PQfnumber(res, "numero_di_telefono"));
            std::string password = PQgetvalue(res, 0, PQfnumber(res, "password"));
            int stato = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

            // Aggiorniamo l'oggetto chiamante con i dettagli dell'utente trovato
            this->nome_utente = nome_utente;
            this->session_id = session_id;
            this->nome = nome;
            this->cognome = cognome;
            this->email = email;
            this->numero_telefono = numero_telefono;
            this->password = password;
            this->stato = stato;
        }

        PQclear(res);

        
        printf(this->nome_utente.c_str());
        printf("\n");


    return "Login avvenuto con successo";
    }


    // Metodo utilizzato per effettuare il logout di un utente dato il suo nome utente.
    std::string effettua_logout(Con2DB db1, std::string nomeUtenteLogout)
    {
        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Logout utente.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        // Caricamento del sessionID a seconda della categoria dell'utente.
        std::string sessionID = "";

        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", nomeUtenteLogout.c_str());
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
            messageLog = "Non esiste " + nomeUtenteLogout + " , poichè non è stato registrato, non si può effettuare il logout .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + nomeUtenteLogout + ", utente non loggato, non può essere effettuato il logout.";
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato, non può effettuare il logout .", sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Controlliamo se l'utente è già loggato, che varia a seconda del valore dell'attributo "stato" all'interno del db.
        int stato_utente;

        // Viene recuperato il valore dell'attributo "stato".
        sprintf(sqlcmd, "SELECT stato FROM Utente WHERE nome_utente = '%s'", nomeUtenteLogout.c_str());
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
                
                result = messageLog;
                return result;
            }

            // Se il valore dell'attributo "stato" è uguale a 1, allora l'utente è connesso e dobbiamo effettuare il logout.
            else
            {
                // Aggiorniamo lo stato dell'utente, da connesso a disconnesso:
                sprintf(sqlcmd, "UPDATE Utente set stato = 0 WHERE nome_utente = '%s'", nomeUtenteLogout.c_str());
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                // Log
                statoReq = statoRequisito::Success;
                messageLog = "Disconnessione per utente: " + nomeUtenteLogout;
                InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

                // Viene resettato il sessionID associato all'utente poichè la sessione è finita dato che l'utente si discollega
                nomeRequisito = "Aggiornamento sessionID.";

                // Aggiornamento del session id nella tabella dell'utente compratore
                sprintf(sqlcmd, "UPDATE Utente set session_id='' WHERE nome_utente = '%s'", nomeUtenteLogout.c_str());
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                // Log
                statoReq = statoRequisito::Success;
                messageLog = "Aggiornamento sessionID causato dalla disconnessione di Utente " + nomeUtenteLogout;
                InsertToLogDB(db1, "INFO", messageLog, "", nomeRequisito, statoReq);
            }
        }

        // Se il numero di righe del risultato della query relativo allo "stato" dell'utente è 0, non si può recuperare il valore dell'attributo "stato".
        if (rows == 0)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Utente " + nomeUtenteLogout + " non trovato.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
            
            result = messageLog;
            return result;
        }
        
        result = messageLog;
        return result;
    }


    // Metodo utilizzato per effettuare l'eliminazione di un utente dato il suo nome utente.
    std::string elimina_profilo(Con2DB db1, std::string nomeUtenteDaEliminare)
    {
        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Eliminazione profilo.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        // Caricamento del sessionID.
        std::string sessionID = "";

        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", nomeUtenteDaEliminare.c_str());
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
            messageLog = "Non esiste " + nomeUtenteDaEliminare + " , poichè non è stato registrato, non si può eliminare il profilo .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + nomeUtenteDaEliminare + ", utente non loggato, non si può eliminare il profilo .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Eliminazione utente compratore
        sprintf(sqlcmd, "DELETE FROM Utente WHERE nome_utente = '%s'", nomeUtenteDaEliminare.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Log
        statoReq = statoRequisito::Success;
        messageLog = "Eliminazione profilo utente " + nomeUtenteDaEliminare;
        InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

        result = messageLog;
        return result;
    }

    // Metodo utilizzato per effettuare l'aggiornamento del numero di telefono di un utente.
    std::string aggiornaNumeroDiTelefono(Con2DB db1, std::string input_nome_utente, std::string nuovoNumeroTelefono)
    {
        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Aggiornamento numero di telefono.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        // Caricamento del sessionID a seconda della categoria dell'utente.
        std::string sessionID = "";

        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", input_nome_utente.c_str());
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
            messageLog = "Non esiste " + input_nome_utente + " , poichè non è stato registrato, non si può aggiornare il numero di telefono .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + input_nome_utente + ", utente non loggato, non può essere aggiornato il numero di telefono";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Aggiornamento del numero di telefono dell'utente compratore.
        sprintf(sqlcmd, "UPDATE Utente set numero_di_telefono = '%s' WHERE nome_utente = '%s'", nuovoNumeroTelefono.c_str(), input_nome_utente.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Log
        statoReq = statoRequisito::Success;
        messageLog = "Aggiornamento numero di telefono : " + nuovoNumeroTelefono + " per utente: " + input_nome_utente;
        InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

        result = messageLog;
        return result;
    }


    // Metodo utilizzato per effettuare l'aggiornamento della password di un utente.
    std::string aggiornaPassword(Con2DB db1, std::string input_nome_utente, std::string vecchiaPassw, std::string nuovaPassw)
    {
        std::string passwDB = "";

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Aggiornamento password.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        // Caricamento del sessionID a seconda della categoria dell'utente.
        std::string sessionID = "";

        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", input_nome_utente.c_str());
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
            messageLog = "Non esiste " + input_nome_utente + " , poichè non è stato registrato, non si può aggiornare la password .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + input_nome_utente + ", utente non loggato, non può essere aggiornata la password.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifichiamo che la vecchiaPassword inserita dall'utente è uguale a quella nel database relativa all'utente che vuole aggiornarla:

        // Recuperiamo la password dell'utente all'interno del db:
        sprintf(sqlcmd, "SELECT password FROM Utente WHERE nome_utente = '%s'", input_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare la password.
        if (rows == 1)
        {
            passwDB = PQgetvalue(res, 0, PQfnumber(res, "password"));
        }
        PQclear(res);

        // Se la password del db è diversa dalla password inserita dall'utente allora non può essere effettuata l'azione
        if (passwDB != vecchiaPassw)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "La password inserita " + vecchiaPassw + "  non è corretta.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

            std::cout << "Errore: La password attuale inserita non è corretta" << std::endl;
            
            result = messageLog;
            return result;
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
                messageLog = "La nuova password inserita " + nuovaPassw + " deve contenere almeno 8 caratteri.";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
                std::cout << "Errore: La nuova passowrd deve contenere almeno 8 caratteri." << std::endl;

                result = messageLog;
                return result;
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
                messageLog = "La nuova password inserita " + nuovaPassw + " deve contenere almeno un carattere maiuscolo.";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

                result = messageLog;
                return result;
            }

            // Verifica se la password contiene almeno un numero
            if (!hasDigit)
            {
                // Stampa un messaggio di errore e registra l'errore nel log del database
                std::cout << "La nuova password deve contenere almeno un numero." << std::endl;

                statoReq = statoRequisito::NotSuccess;
                messageLog = "La nuova password inserita " + nuovaPassw + " deve contenere almeno un numero.";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

                result = messageLog;
                return result;
            }

            if (!hasSpecialChar)
            {
                // Stampa un messaggio di errore e registra l'errore nel log del database
                std::cout << "La nuova password deve contenere almeno un carattere speciale." << std::endl;

                statoReq = statoRequisito::NotSuccess;
                messageLog = "La nuova password inserita " + nuovaPassw + " deve contenere almeno un carattere speciale.";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

                result = messageLog;
                return result;
            }

            // Aggiornamento "password" UtenteCompratore
            sprintf(sqlcmd, "UPDATE Utente set password = '%s' WHERE nome_utente = '%s'", nuovaPassw.c_str(), input_nome_utente.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Aggiornamento password (nuova password inserita: " + nuovaPassw + " ) per utente: " + input_nome_utente;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }
        result = messageLog;
        return result;
    }


}
;

#endif // USER_H