#ifndef USER_TRASPORTATORE_H
#define USER_TRASPORTATORE_H

#include "../../shared-server/user.h"
#include "../../shared-server/isSpecialCharacter.h"

class UtenteTrasportatore : public Utente
{
public:
    // Attributi specifici per UtenteTrasportatore
    std::string ditta_spedizione;
    int disponibilità;

    // Costruttore di UtenteTrasportatore

    UtenteTrasportatore() : Utente("", "", "", "", "", "", "", "", -1),
                            ditta_spedizione(""),
                            disponibilità(-1) {}

    UtenteTrasportatore(std::string nome_utente, std::string session_id, std::string categoria, std::string nome, std::string cognome,
                        std::string numero_telefono, std::string password, std::string email, int stato,
                        std::string ditta_spedizione, int disponibilità)
        : Utente(nome_utente, session_id, categoria, nome, cognome, numero_telefono, password, email, stato), ditta_spedizione(ditta_spedizione), disponibilità(disponibilità) {}

    // Metodo specifico per UtenteTrasportatore
    void mostraInformazioniTrasportatore()
    {
        mostraInformazioni(); // Chiama il metodo della classe base
        std::cout << "Ditta di spedizione: " << ditta_spedizione << std::endl;
        std::cout << "Stato: " << stato << std::endl;
    }

    // Metodo per effettuare la registrazione di un utente trasportatore dati tutti i campi di input.
    void effettuaRegistrazione(Con2DB db1,
                               std::string in_nome_utente, std::string in_categoria, std::string in_nome, std::string in_cognome,
                               std::string sessionID,
                               std::string in_numero_telefono, std::string in_email,
                               std::string in_password, std::string in_conferma_password,
                               std::string in_dittaSped)
    {

        int stato = 1;
        int disponibilità = 0;

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Registrazione utente trasportatore.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Controllo se il sessionID è univoco.
        // bool resultSession = check_sessionID(db1, nomeRequisito, statoReq, sessionID);
        // if (resultSession == false)
        // {
        //     return;
        // }

        // Controllo che il nome utente sia univoco con gli altri utenti.
        bool resultUsername = check_nome_utente_univoco(db1, in_nome_utente, nomeRequisito, statoReq, sessionID);
        if (resultUsername == false)
        {
            return;
        }

        // Controllo che l'email sia univoca
        bool resultEmailUn = check_email_univoca(db1, in_email, nomeRequisito, statoReq, sessionID);
        if (resultEmailUn == false)
        {
            return;
        }

        // Controllo se la mail contiene il carattere "@".
        bool resultEmail = check_email(db1, in_email, nomeRequisito, statoReq, sessionID);
        if (resultEmail == false)
        {
            return;
        }

        // Controllo se la password rispetta i criteri: lunghezza di almeno 8, almeno una lettere maiuscola, un numero e un carattere speciale.
        bool resultPassw = check_password(db1, in_password, in_conferma_password, nomeRequisito, statoReq, sessionID);
        if (resultPassw == false)
        {
            return;
        }

        // Controllo se la password data in input è uguale alla conferma password.
        if (in_password != in_conferma_password)
        {
            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1, "ERROR", "Le password non corrispondono", session_id, nomeRequisito, statoReq);
            std::cout << "Errore: Le password non corrispondono." << std::endl;
            return;
        }

        // Se tutti i check hanno dato buoni risultati, possiamo aggiungere l'utente al database:

        // Inserisco nel database una riga corrispettiva al nuovo utente con tutti i campi presi in input dal metodo:
        sprintf(sqlcmd, "INSERT INTO Utente (nome_utente, session_id, categoriaUtente, nome, cognome, indirizzo_mail, numero_di_telefono, password, stato ) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d)",
                in_nome_utente.c_str(), sessionID.c_str(), in_categoria.c_str(), in_nome.c_str(), in_cognome.c_str(), in_email.c_str(), in_numero_telefono.c_str(), in_password.c_str(), stato);
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Inserisco nel database una riga corrispettiva al nuovo utente con tutti i campi presi in input dal metodo:
        sprintf(sqlcmd, "INSERT INTO UtenteTrasportatore (nome_utente_trasportatore, nome_DittaSpedizione ) VALUES ('%s', '%s')", in_nome_utente.c_str(), in_dittaSped.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Conferma di inserimento nel db
        std::cout << "Utente inserito." << std::endl;

        // Log
        statoReq = statoRequisito::Success;
        messageLog = "Utente trasportatore " + in_nome_utente + " inserito";
        InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

        // Riempio il costruttore dell'utente compratore con i campi dati in input al metodo effettua registrazione:
        *this = UtenteTrasportatore(in_nome_utente, session_id, in_categoria, in_nome, in_cognome, in_numero_telefono, in_password, in_email, stato, in_dittaSped, disponibilità);

        return;
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

            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "ERROR", "La nuova passowrd deve contenere almeno un carattere maiuscolo.", session_id, nomeRequisito, statoReq);

            // Restituisce false per indicare che la password non è valida
            return false;
        }

        // Verifica se la password contiene almeno un numero
        if (!hasDigit)
        {

            // Stampa un messaggio di errore e registra l'errore nel log del database
            std::cout << "La nuova password deve contenere almeno un numero." << std::endl;

            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "ERROR", "La nuova passowrd deve contenere almeno un numero.", session_id, nomeRequisito, statoReq);

            // Restituisce false per indicare che la password non è valida
            return false;
        }

        // Verifica se la password contiene almeno un carattere speciale
        if (!hasSpecialChar)
        {

            // Stampa un messaggio di errore e registra l'errore nel log del database
            std::cout << "La nuova password deve contenere almeno un carattere speciale." << std::endl;

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



    // Metodo utilizzato per effettuare il login di un utente dato il suo nome utente, la sua password e il sessionID che sarà creato dal server.
    void effettua_login(Con2DB db1, std::string input_nome_utente, std::string input_passw, std::string sessionID) override
    {
        try
        {
            // Chiamata al metodo della classe base
            Utente::effettua_login(db1, input_nome_utente, input_passw, sessionID);

            // Definizione di alcune variabili per il logging
            std::string nomeRequisito = "Login utente trasportatore.";
            statoRequisito statoReq = statoRequisito::Wait;
            std::string messageLog = "";

            sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", input_nome_utente.c_str());

            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);

            if (rows == 1)
            {
                std::string ditta_spedizione = PQgetvalue(res, 0, PQfnumber(res, "nome_DittaSpedizione"));
                int disponibilità = atoi(PQgetvalue(res, 0, PQfnumber(res, "dispo")));

                this->disponibilità = disponibilità;
                this->ditta_spedizione = ditta_spedizione;

                printf(this->ditta_spedizione.c_str());
                printf("\n");
                //*this = UtenteTrasportatore(nome_utente, categoria, nome, cognome, numero_telefono, password, email, session_id, ditta_spedizione, stato, disponibilità);
            }
        
            // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
            if (rows == 0)
            {
                // Log dell'errore e uscita dalla funzione
                statoReq = statoRequisito::NotSuccess;
                messageLog = "Utente " + input_nome_utente + " non trovato.";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
                return;
            }

            PQclear(res);

            return;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return;
        }
    }

    // Metodo utilizzato per aggiornare la ditta di spedizione di un utente trasportatore nel database
    void aggiornaNomeDittaSpedizione(Con2DB db1, std::string input_nome_utente, std::string nuovaDittaSpedizione)
    {

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Aggiornamento ditta Spedizione.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Caricamento del sessionID.
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
            messageLog = "Non esiste " + input_nome_utente + " , poichè non è stato registrato, non può essere aggiornata la ditta di spedizione .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            return;
        }

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + input_nome_utente + ", utente non loggato, non può essere aggiornata la ditta di spedizione.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            return;
        }

        // Verifichiamo che l'utente si tratti di un utente trasportatore:
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

        if (categoriaUtente != "UtenteTrasportatore")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "L utente " + input_nome_utente + " non è un utente trasportatore, perciò non può essere aggiornato il nome della ditta di spedizione.";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            return;
        }

        // Aggiornamento del nome della ditta di spdeizione dell'utente trasportatore nel database
        sprintf(sqlcmd, "UPDATE UtenteTrasportatore set nome_DittaSpedizione='%s' WHERE nome_utente_trasportatore = '%s'",
                nuovaDittaSpedizione.c_str(), input_nome_utente.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Log
        statoReq = statoRequisito::Success;
        messageLog = "Aggiornamento ditta spedizione per utente: " + input_nome_utente;
        InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

        return;
    }
};

#endif // USER_TRASPORTATORE_H
