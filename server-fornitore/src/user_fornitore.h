#ifndef USER_FORNITORE_H
#define USER_FORNITORE_H

#include "../../shared-server/user.h"
#include "../../shared-server/isSpecialCharacter.h"

class UtenteFornitore : public Utente
{
public:
    // Attributi specifici per UtenteFornitore
    std::string azienda_produzione;

    // Costruttore di UtenteFornitore

    UtenteFornitore() : Utente("", "", "", "", "", "", "", "", -1),
                        azienda_produzione("")
    {
    }

    UtenteFornitore(
        std::string nome_utente, std::string session_id, std::string categoria, std::string nome, std::string cognome,
        std::string numero_telefono, std::string password, std::string email, int stato,
        std::string azienda_produzione)

        : Utente(nome_utente, session_id, categoria, nome, cognome, numero_telefono, password, email, stato), azienda_produzione(azienda_produzione)
    {
    }

    // Metodo specifico per UtenteFornitore
    void mostraInformazioniFornitore()
    {
        mostraInformazioni(); // Chiama il metodo della classe base
        std::cout << "Azienda di produzione: " << azienda_produzione << std::endl;
        std::cout << "Stato: " << stato << std::endl;
    }

    // Metodo per effettuare la registrazione di un utente fornitore dati tutti i campi di input.
    std::string effettuaRegistrazione(Con2DB db1,
                               std::string in_nome_utente,
                               std::string in_categoria,
                               std::string in_nome, std::string in_cognome,
                               std::string sessionID,
                               std::string in_numero_telefono,
                               std::string in_email,
                               std::string in_password, std::string in_conferma_password,
                               std::string in_aziendaProd)
    {

        int stato = 1;

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Registrazione utente fornitore.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        // Controllo che il nome utente sia univoco con gli altri utenti.
        bool resultUsername = check_nome_utente_univoco(db1, in_nome_utente, nomeRequisito, statoReq, sessionID);
        if (resultUsername == false)
        {
            result = "Il nome utente non è univoco, la registrazione non è andata a buon fine.";
            return result;
        }

        // Controllo che l'email sia univoca
        bool resultEmailUn = check_email_univoca(db1, in_email, nomeRequisito, statoReq, sessionID);
        if (resultEmailUn == false)
        {
            result = "L'email non è univoca, la registrazione non è andata a buon fine.";
            return result;
        }

        // Controllo se la mail contiene il carattere "@".
        bool resultEmail = check_email(db1, in_email, nomeRequisito, statoReq, sessionID);
        if (resultEmail == false)
        {
            result = "L'email non contiene il carattere -@-";
            return result;
        }

        // Controllo se la password rispetta i criteri: lunghezza di almeno 8, almeno una lettere maiuscola, un numero e un carattere speciale.
        bool resultPassw = check_password(db1, in_password, in_conferma_password, nomeRequisito, statoReq, sessionID);
        if (resultPassw == false)
        {
            result = "La password non rispetta i criteri definiti.";
            return result;
        }

        // Controllo se la password data in input è uguale alla conferma password.
        if (in_password != in_conferma_password)
        {
            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1, "ERROR", "Le password non corrispondono", session_id, nomeRequisito, statoReq);
            std::cout << "Errore: Le password non corrispondono." << std::endl;
            
            result = "I valori del campo -password- e -conferma password- non sono uguali";
            return result;
        }

        // Se tutti i check hanno dato buoni risultati, possiamo aggiungere l'utente al database:

        // Inserisco nel database una riga corrispettiva al nuovo utente con tutti i campi presi in input dal metodo:
        sprintf(sqlcmd, "INSERT INTO Utente (nome_utente, session_id, categoriaUtente, nome, cognome, indirizzo_mail, numero_di_telefono, password, stato ) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d)",
                in_nome_utente.c_str(), sessionID.c_str(), in_categoria.c_str(), in_nome.c_str(), in_cognome.c_str(), in_email.c_str(), in_numero_telefono.c_str(), in_password.c_str(), stato);

        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Inserisco nel database una riga corrispettiva al nuovo utente con tutti i campi presi in input dal metodo:
        sprintf(sqlcmd, "INSERT INTO UtenteFornitore (nome_utente_fornitore, nome_AziendaProduttrice ) VALUES ('%s', '%s')", in_nome_utente.c_str(), in_aziendaProd.c_str());

        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Conferma di inserimento nel db
        std::cout << "Utente inserito." << std::endl;

        // Log
        statoReq = statoRequisito::Success;
        messageLog = "Utente fornitore " + in_nome_utente + " inserito";
        InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

        // Riempio il costruttore dell'utente compratore con i campi dati in input al metodo effettua registrazione:
        *this = UtenteFornitore(in_nome_utente, session_id, in_categoria, in_nome, in_cognome, in_numero_telefono, in_password, in_email, stato, in_aziendaProd);

        printf("Registrazione avvenuta con successo.\n");

        result = "Registrazione utente fornitore : " + in_nome_utente + " avvenuta con successo.";
        return result;
    }

    

    // Metodo per verificare se un'email è valida per la registrazione di un utente.
    bool check_email(Con2DB db1, std::string in_email, std::string nomeRequisito, statoRequisito statoReq, std::string sessionID)
    {
        bool result = true;

        // Verifica se l'email contiene il carattere '@'
        if (in_email.find("@") == std::string::npos)
        {
            // Se l'email non contiene '@', impostiamo lo stato del requisito a NotSuccess
            statoReq = statoRequisito::NotSuccess;

            // Log dell'errore e restituzione di false per indicare che l'email non è valida
            InsertToLogDB(db1, "ERROR", "La mail deve contenere il carattere -@-.", sessionID, nomeRequisito, statoReq);
            std::cout << "Errore: La mail deve contenere il carattere '@'." << std::endl;
            return false;
        }

        // Restituisce true se la l'email soddisfa i criteri di validità
        return result;
    }

    // Metodo per verificare la validità di una password
    bool check_password(Con2DB db1, std::string in_password, std::string in_conferma_password, std::string nomeRequisito, statoRequisito statoReq, std::string sessionID)
    {

        bool result = true;

        // Verifica se la lunghezza della password è inferiore a 8 caratteri
        if (in_password.length() < 8)
        {

            // Se la password è troppo corta, impostiamo lo stato del requisito a NotSuccess
            statoReq = statoRequisito::NotSuccess;

            // Registra un errore nel log del database e stampa un messaggio di errore
            InsertToLogDB(db1, "ERROR", "La password deve contenere almeno 8 caratteri.", session_id, nomeRequisito, statoReq);
            std::cout << "Errore: La passowrd deve contenere almeno 8 caratteri." << std::endl;

            // Restituisce false per indicare che la password non è valida
            return false;
        }

        // Verifica se la lunghezza della conferma della password è inferiore a 8 caratteri
        if (in_conferma_password.length() < 8)
        {
            // Se la password è troppo corta, impostiamo lo stato del requisito a NotSuccess
            statoReq = statoRequisito::NotSuccess;

            // Registra un errore nel log del database e stampa un messaggio di errore
            InsertToLogDB(db1, "ERROR", "La password deve contenere almeno 8 caratteri.", session_id, nomeRequisito, statoReq);
            std::cout << "Errore: La passowrd deve contenere almeno 8 caratteri." << std::endl;

            // Restituisce false per indicare che la password non è valida
            return false;
        }

        // Verifica se la password contiene almeno un carattere maiuscolo, un numero e un carattere speciale
        bool hasUpperCase = false;
        bool hasDigit = false;
        bool hasSpecialChar = false;
        for (size_t i = 0; i < in_password.length(); i++)
        {
            if (std::isupper(in_password[i]))
            {
                hasUpperCase = true;
            }

            else if (std::isdigit(in_password[i]))
            {
                hasDigit = true;
            }

            else if (isSpecialCharacter(in_password[i]))
            {
                hasSpecialChar = true;
            }
        }

        // Verifica se la password contiene almeno un carattere maiuscolo
        if (!hasUpperCase)
        {
            // Stampa un messaggio di errore e registra l'errore nel log del database
            std::cout << "La nuova password deve contenere almeno un carattere maiuscolo." << std::endl;

            // Registra un errore nel log del database e stampa un messaggio di errore
            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "ERROR", "La nuova passowrd deve contenere almeno un carattere maiuscolo.", session_id, nomeRequisito, statoReq);

            // Restituisce false per indicare che la password non è valida
            return false;
        }

        if (!hasDigit)
        {
            // Stampa un messaggio di errore e registra l'errore nel log del database
            std::cout << "La nuova password deve contenere almeno un numero." << std::endl;

            // Registra un errore nel log del database e stampa un messaggio di errore
            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "ERROR", "La nuova passowrd deve contenere almeno un numero.", session_id, nomeRequisito, statoReq);

            // Restituisce false per indicare che la password non è valida
            return false;
        }

        if (!hasSpecialChar)
        {
            // Stampa un messaggio di errore e registra l'errore nel log del database
            std::cout << "La nuova password deve contenere almeno un carattere speciale." << std::endl;

            // Registra un errore nel log del database e stampa un messaggio di errore
            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "ERROR", "La nuova passowrd deve contenere almeno un carattere speciale.", session_id, nomeRequisito, statoReq);

            // Restituisce false per indicare che la password non è valida
            return false;
        }

        // Restituisce true se la password soddisfa tutti i criteri di validità
        return result;
    }

    // Metodo utilizzato per verificare che un nome utente è univoco tra gli utenti compratori, fornitori e trasportatori
    bool check_nome_utente_univoco(Con2DB db1, std::string in_nome_utente, std::string nomeRequisito, statoRequisito statoReq, std::string sessionID)
    {
        bool result = true;

        // Variabile per memorizzare i messaggi di log
        std::string messageLog = "";

        // Verifica se il nome utente è già presente nella tabella UtenteCompratore
        sprintf(sqlcmd, "SELECT * FROM Utente WHERE nome_utente = '%s'", in_nome_utente.c_str());
        PGresult *res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        PQclear(res);
        // Se il numero di righe dal risultato della query è maggiore di 0, l'utente già esiste nel database, ed è stato già registrato.
        if (rows > 0)
        {
            // Log
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Il nome utente " + in_nome_utente + " è già in uso. ";
            InsertToLogDB(db1, "ERROR", messageLog, session_id, nomeRequisito, statoReq);

            // Stampa messaggio di errore
            std::cout << "Errore: Il nome utente è già in uso." << std::endl;
            return false;
        }

        // Restituiamo true se il nome utente è univoco tra tutti gli utenti
        return result;
    }

    // Metodo utilizzato per verificare che un email è univoco tra gli utenti compratori.
    bool check_email_univoca(Con2DB db1, std::string in_email, std::string nomeRequisito, statoRequisito statoReq, std::string sessionID)
    {

        bool result = true;

        // Variabile per memorizzare i messaggi di log
        std::string messageLog = "";

        // Verifico se l'email è già presente nella tabella degli utenti compratori.
        sprintf(sqlcmd, "SELECT * FROM Utente WHERE indirizzo_mail = '%s'", in_email.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        PQclear(res);

        // Se il numero di righe dal risultato della query è maggiore di 0, l'email già esiste nel database.
        if (rows > 0)
        {

            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Indirizzo mail " + in_email + " è già in uso. ";
            InsertToLogDB(db1, "ERROR", messageLog, session_id, nomeRequisito, statoReq);

            std::cout << "Errore: L'indirizzo mail è già in uso." << std::endl;
            return false;
        }

        // Restituiamo true se l'email è univoco tra gli utenti compratori
        return result;
    }


    // Metodo utilizzato per aggiornare l'azienda produttirce di un utente fornitore nel database
    std::string aggiornaNomeAziendaProduttrice(Con2DB db1, std::string input_nome_utente, std::string nuovaAziendaProduttrice)
    {

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Aggiornamento azienda Produttrice.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        std::string sessionID = "";

        // Caricamento del sessionID.
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
            messageLog = "Non esiste " + input_nome_utente + " , poichè non è stato registrato, non può essere aggiornato il nome dell azienda produttrice .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + input_nome_utente + ", utente non loggato, non può essere aggiornato il nome dell azienda produttrice.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifichiamo che l'utente si tratti di un utente fornitore:
        std::string categoriaUtente = "";
        sprintf(sqlcmd, "SELECT categoriaUtente FROM Utente WHERE nome_utente = '%s'", input_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare la categoria dell'utente.
        if (rows == 1)
        {
            categoriaUtente = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));
        }
        PQclear(res);

        if (categoriaUtente != "UtenteFornitore")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "L utente " + input_nome_utente + " non è un utente fornitore, perciò non può essere aggiornato il nome dell azienda produttrice.";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Aggiornamento del nome dell'azienda produttrice dell'utente fornitore nel database
        sprintf(sqlcmd, "UPDATE UtenteFornitore set nome_AziendaProduttrice='%s' WHERE nome_utente_fornitore = '%s'",
                nuovaAziendaProduttrice.c_str(), input_nome_utente.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Log
        statoReq = statoRequisito::Success;
        messageLog = "Aggiornamento azienda produttrice per utente: " + input_nome_utente;
        InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

        result = messageLog;
        return result;
    }



    // Metodo utilizzato per effettuare il login di un utente dato il suo nome utente, la sua password e il sessionID che sarà creato dal server.
    std::string effettua_login(Con2DB db1, std::string input_nome_utente, std::string input_passw, std::string sessionID) override
    {
        try
        {
            // Definizione di alcune variabili per il logging
            std::string nomeRequisito = "Login utente fornitore.";
            statoRequisito statoReq = statoRequisito::Wait;
            std::string messageLog = "";

             // Dichiarazione variabile per il risultato dell'operazione.
            std::string result = "";

            // Controllo prima di eseguire il login della superclasse se l'utente è un utenteFornitore, altrimenti non può essere esguito il login.
            sprintf(sqlcmd, "SELECT categoriaUtente FROM Utente WHERE nome_utente = '%s'", input_nome_utente.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows == 1){
                std::string categoria = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));

                PQclear(res);
                if (categoria != "UtenteFornitore"){
                    // Log dell'errore e uscita dalla funzione
                    statoReq = statoRequisito::NotSuccess;
                    messageLog = "Utente " + input_nome_utente + " è di un altra categoria, non un utente fornitore, non può essere effettuato il login.";
                    InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
                    
                    result = messageLog;
                    return result;
                } 
            }

            // Chiamata al metodo della classe base
            Utente::effettua_login(db1, input_nome_utente, input_passw, sessionID);

            sprintf(sqlcmd, "SELECT * FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", input_nome_utente.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);

            if (rows == 1)
            {
                std::string azienda_produzione = PQgetvalue(res, 0, PQfnumber(res, "nome_AziendaProduttrice"));
                this->azienda_produzione = azienda_produzione;
            }
            
            // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
            if (rows == 0)
            {
                // Log dell'errore e uscita dalla funzione
                statoReq = statoRequisito::NotSuccess;
                messageLog = "Utente " + input_nome_utente + " è di un altra categoria, non un utente fornitore.";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
                
                result = messageLog;
                return result;
            }

            PQclear(res);
            return "Login utente trasportatore avvenuto con successo";
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return e.what();
        }
    }
};

#endif // USER_FORNITORE_H
