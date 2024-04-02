#ifndef UTENTE_H
#define UTENTE_H

#include "../server-fornitore/src/main.h"
#include "isSpecialCharacter.h"

class Utente
{


public:
    std::string m_nomeUtente;
    std::string m_sessionID;
    std::string m_categoria;
    std::string m_nome;
    std::string m_cognome;
    std::string m_numeroTelefono;
    std::string m_password;
    std::string m_email;
    int m_stato;

    Utente() : m_nomeUtente(""),
               m_sessionID(""),
               m_categoria(""),
               m_nome(""), m_cognome(""),
               m_numeroTelefono(""),
               m_password(""),
               m_email(""),
               m_stato(-1) {}

    Utente(std::string nome_utente, std::string session_id, std::string categoria, std::string nome, std::string cognome, std::string numero_telefono, std::string password, std::string email, int stato)
        : m_nomeUtente(nome_utente), m_sessionID(session_id), m_categoria(categoria), m_nome(nome), m_cognome(cognome), m_numeroTelefono(numero_telefono), m_password(password), m_email(email), m_stato(stato) {}

    void mostraInformazioni()
    {
        std::cout << "Nome utente: " << m_nomeUtente << std::endl;
        std::cout << "Nome: " << m_nome << std::endl;
        std::cout << "Cognome: " << m_cognome << std::endl;
        std::cout << "Numero di telefono: " << m_numeroTelefono << std::endl;
        std::cout << "Password: " << m_password << std::endl;
        std::cout << "Email: " << m_email << std::endl;
    }

    // Metodo utilizzato per effettuare il login di un utente dato il suo nome utente, la sua password e il sessionID che sarà creato dal server.
    virtual std::string login(Con2DB db1, std::string in_nome_utente, std::string in_password, std::string in_sessionID)
    {
        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Login utente.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Controlliamo se l'utente è già loggato, che varia a seconda del valore dell'attributo "stato" all'interno del db.
        int stato_utente;

        // Viene recuperato il valore dell'attributo "stato".
        sprintf(sqlcmd, "SELECT stato FROM Utente WHERE nome_utente = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe del risultato della query relativo allo "stato" dell'utente è 0, non si può recuperare il valore dell'attributo "stato".
        if (rows == 0)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Utente " + in_nome_utente + " non esistente, non è registrato perciò non può essere effettuato il login.";
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
            std::string sessionID = "";
            sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows == 1) { sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id")); }

            // Log del warning e uscita dalla funzione
            statoReq = statoRequisito::Wait;
            messageLog = "Utente " + in_nome_utente + " già connesso e con la sessionID : " + sessionID + " , stai cercando di effettuare un nuovo login.";
            InsertToLogDB(db1, "WARNING", messageLog, sessionID, nomeRequisito, statoReq);

            // Viene effettuata la verifica delle password: confrontiamo la password data in input dall'utente che vuole effettuare il login e la password che si trova nel db dell'utente.
            std::string password_utente;
            char *password_u;

            // Viene recuperata la password nel database
            sprintf(sqlcmd, "SELECT password FROM Utente WHERE nome_utente = '%s'", in_nome_utente.c_str());
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
                messageLog = "Utente " + in_nome_utente + " non esistente, non è registrato perciò non può essere effettuato il login.";
                InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);

                std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                throw std::runtime_error("Errore: L'utente non è stato trovato.");
                //return;
            }

            PQclear(res);

            // Viene effettuato il confronto della password data in input dall'utente e quella segnata nel database:
            // Se le passowrd non corrispondono allora l'utente non può collegarsi.
            if (in_password != password_utente)
            {
                // Log dell'errore e uscita dalla funzione
                statoReq = statoRequisito::NotSuccess;
                messageLog = "Password " + in_password + " non corretta, non può essere effettuato il nuovo login per l utente: " + in_nome_utente;
                InsertToLogDB(db1, "ERROR", messageLog, in_sessionID, nomeRequisito, statoReq);

                std::cout << "Errore: La passowrd non è corretta, riprovare." << std::endl;
                throw std::runtime_error("Errore: La passowrd non è corretta, riprovare.");
                //return;
            }


            // Aggiorniamo il sessionID dell'utente con quello per la nuova sessione:
            sprintf(sqlcmd, "UPDATE Utente set session_id='%s' WHERE nome_utente = '%s'", in_sessionID.c_str(), in_nome_utente.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Aggiornamento sessionID per nuovo login dell utente " + in_nome_utente;
            InsertToLogDB(db1, "INFO", messageLog, in_sessionID, nomeRequisito, statoReq);

            // Animo l'oggetto Utente
            sprintf(sqlcmd, "SELECT * FROM Utente WHERE nome_utente = '%s'", in_nome_utente.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);

            // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
            if (rows == 0){
                // Log dell'errore e uscita dalla funzione
                statoReq = statoRequisito::NotSuccess;
                messageLog = "Utente " + in_nome_utente + " non esistente, non è registrato perciò non può essere effettuato il login.";
                InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
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
                this->m_nomeUtente = nome_utente;
                this->m_sessionID = session_id;
                this->m_nome = nome;
                this->m_cognome = cognome;
                this->m_email = email;
                this->m_numeroTelefono = numero_telefono;
                this->m_password = password;
                this->m_stato = stato;
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
        sprintf(sqlcmd, "SELECT password FROM Utente WHERE nome_utente = '%s'", in_nome_utente.c_str());
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
            messageLog = "Utente " + in_nome_utente + " non esistente, non è registrato perciò non può essere effettuato il login.";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);

            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
            throw std::runtime_error("Errore: L'utente non è stato trovato.");
            //return;
        }

        PQclear(res);

        // Viene effettuato il confronto della password data in input dall'utente e quella segnata nel database:
        // Se le passowrd non corrispondono allora l'utente non può collegarsi.
        if (in_password != password_utente)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Password " + in_password + " non corretta, non può essere effettuato il login per l utente " + in_nome_utente;
            InsertToLogDB(db1, "ERROR", messageLog, in_sessionID, nomeRequisito, statoReq);

            std::cout << "Errore: La passowrd non è corretta, riprovare." << std::endl;
            throw std::runtime_error("Errore: La passowrd non è corretta, riprovare.");
            //return;
        }

        // else :Se le password corrispondono, l'utente può collegarsi.

        std::cout << "La passowrd inserita: " << password_utente << "è corretta." << std::endl;

        // Aggiornamento del session id nella tabella dell'utente.
        sprintf(sqlcmd, "UPDATE Utente set session_id='%s' WHERE nome_utente = '%s'", in_sessionID.c_str(), in_nome_utente.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Log
        statoReq = statoRequisito::Wait;
        messageLog = "Aggiornamento sessionID per utente " + in_nome_utente;
        InsertToLogDB(db1, "INFO", messageLog, in_sessionID, nomeRequisito, statoReq);

        // Viene effettuato l'aggiornamento dello stato.

        // Aggiornamento "stato" Utente
        sprintf(sqlcmd, "UPDATE Utente set stato = 1 WHERE nome_utente = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Log
        statoReq = statoRequisito::Success;
        messageLog = "Utente " + in_nome_utente + " loggato, aggiornamento dello stato.";
        InsertToLogDB(db1, "INFO", messageLog, in_sessionID, nomeRequisito, statoReq);

        // Animo l'oggetto Utente
        sprintf(sqlcmd, "SELECT * FROM Utente WHERE nome_utente = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
        if (rows == 0){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Utente " + in_nome_utente + " non trovato.";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
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
            this->m_nomeUtente = nome_utente;
            this->m_sessionID = session_id;
            this->m_nome = nome;
            this->m_cognome = cognome;
            this->m_email = email;
            this->m_numeroTelefono = numero_telefono;
            this->m_password = password;
            this->m_stato = stato;
        }

        PQclear(res);

        
        printf(this->m_nomeUtente.c_str());
        printf("\n");


    return "Login avvenuto con successo";
    }


    // Metodo utilizzato per effettuare il logout di un utente dato il suo nome utente.
    std::string logout(Con2DB db1, std::string in_nome_utente)
    {
        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Logout utente.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        // Caricamento del sessionID a seconda della categoria dell'utente.
        std::string sessionID = "";

        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente.c_str());
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
            messageLog = "Non esiste " + in_nome_utente + " , poichè non è stato registrato, non si può effettuare il logout .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + in_nome_utente + ", utente non loggato, non può essere effettuato il logout.";
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato, non può effettuare il logout .", sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Controlliamo se l'utente è già loggato, che varia a seconda del valore dell'attributo "stato" all'interno del db.
        int stato_utente;

        // Viene recuperato il valore dell'attributo "stato".
        sprintf(sqlcmd, "SELECT stato FROM Utente WHERE nome_utente = '%s'", in_nome_utente.c_str());
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
                messageLog = "Utente " + in_nome_utente + " già disconnesso";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

                std::cout << "Errore: L'utente è già disconnesso" << std::endl;
                
                result = messageLog;
                return result;
            }

            // Se il valore dell'attributo "stato" è uguale a 1, allora l'utente è connesso e dobbiamo effettuare il logout.
            else
            {
                // Aggiorniamo lo stato dell'utente, da connesso a disconnesso:
                sprintf(sqlcmd, "UPDATE Utente set stato = 0 WHERE nome_utente = '%s'", in_nome_utente.c_str());
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                // Log
                statoReq = statoRequisito::Success;
                messageLog = "Disconnessione per utente: " + in_nome_utente;
                InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

                // Viene resettato il sessionID associato all'utente poichè la sessione è finita dato che l'utente si discollega
                nomeRequisito = "Aggiornamento sessionID.";

                // Aggiornamento del session id nella tabella dell'utente compratore
                sprintf(sqlcmd, "UPDATE Utente set session_id='' WHERE nome_utente = '%s'", in_nome_utente.c_str());
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                // Log
                statoReq = statoRequisito::Success;
                messageLog = "Aggiornamento sessionID causato dalla disconnessione di Utente " + in_nome_utente;
                InsertToLogDB(db1, "INFO", messageLog, "", nomeRequisito, statoReq);
            }
        }

        // Se il numero di righe del risultato della query relativo allo "stato" dell'utente è 0, non si può recuperare il valore dell'attributo "stato".
        if (rows == 0)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Utente " + in_nome_utente + " non trovato.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
            
            result = messageLog;
            return result;
        }
        
        result = messageLog;
        return result;
    }


    // Metodo utilizzato per effettuare l'eliminazione di un utente dato il suo nome utente.
    std::string elimina_profilo(Con2DB db1, std::string in_nome_utente)
    {
        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Eliminazione profilo.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        // Caricamento del sessionID.
        std::string sessionID = "";

        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente.c_str());
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
            messageLog = "Non esiste " + in_nome_utente + " , poichè non è stato registrato, non si può eliminare il profilo .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + in_nome_utente + ", utente non loggato, non si può eliminare il profilo .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Eliminazione utente compratore
        sprintf(sqlcmd, "DELETE FROM Utente WHERE nome_utente = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Log
        statoReq = statoRequisito::Success;
        messageLog = "Eliminazione profilo utente " + in_nome_utente;
        InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

        result = messageLog;
        return result;
    }

    // Metodo utilizzato per effettuare l'aggiornamento del numero di telefono di un utente.
    std::string aggiorna_numero_telefono(Con2DB db1, std::string in_nome_utente, std::string in_nuovo_numero_telefono)
    {
        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Aggiornamento numero di telefono.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        // Caricamento del sessionID a seconda della categoria dell'utente.
        std::string sessionID = "";

        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente.c_str());
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
            messageLog = "Non esiste " + in_nome_utente + " , poichè non è stato registrato, non si può aggiornare il numero di telefono .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + in_nome_utente + ", utente non loggato, non può essere aggiornato il numero di telefono";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Aggiornamento del numero di telefono dell'utente compratore.
        sprintf(sqlcmd, "UPDATE Utente set numero_di_telefono = '%s' WHERE nome_utente = '%s'", in_nuovo_numero_telefono.c_str(), in_nome_utente.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Log
        statoReq = statoRequisito::Success;
        messageLog = "Aggiornamento numero di telefono : " + in_nuovo_numero_telefono + " per utente: " + in_nome_utente;
        InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

        result = messageLog;
        return result;
    }


    // Metodo utilizzato per effettuare l'aggiornamento della password di un utente.
    std::string aggiorna_password(Con2DB db1, std::string in_nome_utente, std::string in_vecchia_password, std::string in_nuova_password)
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

        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente.c_str());
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
            messageLog = "Non esiste " + in_nome_utente + " , poichè non è stato registrato, non si può aggiornare la password .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + in_nome_utente + ", utente non loggato, non può essere aggiornata la password.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifichiamo che la vecchiaPassword inserita dall'utente è uguale a quella nel database relativa all'utente che vuole aggiornarla:

        // Recuperiamo la password dell'utente all'interno del db:
        sprintf(sqlcmd, "SELECT password FROM Utente WHERE nome_utente = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare la password.
        if (rows == 1)
        {
            passwDB = PQgetvalue(res, 0, PQfnumber(res, "password"));
        }
        PQclear(res);

        // Se la password del db è diversa dalla password inserita dall'utente allora non può essere effettuata l'azione
        if (passwDB != in_vecchia_password)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "La password inserita " + in_vecchia_password + "  non è corretta.";
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
            if (in_nuova_password.length() < 8)
            {
                // Se la password è troppo corta, impostiamo lo stato del requisito a NotSuccess
                statoReq = statoRequisito::NotSuccess;

                // Registra un errore nel log del database e stampa un messaggio di errore
                messageLog = "La nuova password inserita " + in_nuova_password + " deve contenere almeno 8 caratteri.";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
                std::cout << "Errore: La nuova passowrd deve contenere almeno 8 caratteri." << std::endl;

                result = messageLog;
                return result;
            }

            // Verifica se la password contiene almeno un carattere maiuscolo, un numero e un carattere speciale
            bool hasUpperCase = false;
            bool hasDigit = false;
            bool hasSpecialChar = false;
            for (size_t i = 0; i < in_nuova_password.length(); i++)
            {
                if (std::isupper(in_nuova_password[i]))
                {
                    hasUpperCase = true;
                }

                else if (std::isdigit(in_nuova_password[i]))
                {
                    hasDigit = true;
                }

                else if (isSpecialCharacter(in_nuova_password[i]))
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
                messageLog = "La nuova password inserita " + in_nuova_password + " deve contenere almeno un carattere maiuscolo.";
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
                messageLog = "La nuova password inserita " + in_nuova_password + " deve contenere almeno un numero.";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

                result = messageLog;
                return result;
            }

            if (!hasSpecialChar)
            {
                // Stampa un messaggio di errore e registra l'errore nel log del database
                std::cout << "La nuova password deve contenere almeno un carattere speciale." << std::endl;

                statoReq = statoRequisito::NotSuccess;
                messageLog = "La nuova password inserita " + in_nuova_password + " deve contenere almeno un carattere speciale.";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

                result = messageLog;
                return result;
            }

            // Aggiornamento "password" UtenteCompratore
            sprintf(sqlcmd, "UPDATE Utente set password = '%s' WHERE nome_utente = '%s'", in_nuova_password.c_str(), in_nome_utente.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Aggiornamento password (nuova password inserita: " + in_nuova_password + " ) per utente: " + in_nome_utente;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }
        result = messageLog;
        return result;
    }


}
;

#endif // UTENTE_H