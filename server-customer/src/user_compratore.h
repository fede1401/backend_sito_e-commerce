#ifndef USER_COMPRATORE_H
#define USER_COMPRATORE_H

#include "../../shared-server/user.h"
#include "../../shared-server/isSpecialCharacter.h"


class UtenteCompratore : public Utente
{
public:
    // Attributi specifici per UtenteCompratore
    std::string session_id;
    std::string data_compleanno;
    std::string via_residenza;
    std::string numero_civico;
    std::string CAP;
    std::string città_residenza;
    int stato;

    // Costruttori di UtenteCompratore

    UtenteCompratore() : Utente("", "", "", "", "", "", ""),
                         session_id(""),
                         data_compleanno(""),
                         via_residenza(""),
                         numero_civico(""),
                         CAP(""),
                         città_residenza(""),
                         stato(0) {}

    UtenteCompratore(
        std::string nome_utente, std::string categoria, std::string nome, std::string cognome,
        std::string numero_telefono, std::string password, std::string email, std::string session_id,
        std::string data_compleanno,
        std::string via_residenza, std::string numero_civico, std::string CAP, std::string città_residenza,
        int stato) :

                     Utente(nome_utente, categoria, nome, cognome, numero_telefono, password, email), session_id(session_id), data_compleanno(data_compleanno),
                     via_residenza(via_residenza), numero_civico(numero_civico), CAP(CAP), città_residenza(città_residenza),
                     stato(stato)
    {
    }

    // Metodo specifico per UtenteCompratore
    void mostraInformazioniCompratore()
    {
        mostraInformazioni(); // Chiama il metodo della classe base
        std::cout << "Data di compleanno: " << data_compleanno << std::endl;
        std::cout << "Via di residenza: " << via_residenza << std::endl;
        std::cout << "Numero civico: " << numero_civico << std::endl;
        std::cout << "CAP: " << CAP << std::endl;
        std::cout << "Città di residenza: " << città_residenza << std::endl;
        std::cout << "Stato: " << stato << std::endl;
    }

    // Metodo per effettuare la registrazione di un utente compratore dati tutti i campi di input.
    void effettuaRegistrazione(Con2DB db1,
                               std::string in_nome_utente, std::string in_categoria, std::string in_nome, std::string in_cognome,
                               std::string sessionID,
                               std::string in_numero_telefono, std::string in_email,
                               std::string in_via_residenza, std::string in_numero_civico, std::string in_CAP, std::string in_città_residenza,
                               std::string in_password, std::string in_conferma_password,
                               std::string in_data_compleanno)
    {

        int stato = 1;

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Registrazione utente compratore.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Controllo se il sessionID è univoco.
        bool resultSession = check_sessionID(db1, nomeRequisito, statoReq, sessionID);
        if (resultSession == false)
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
            // Log dell'errore (password e conferma password diverse) e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "La password inserita " + in_password + " non corrisponde con la conferma della password " + in_conferma_password;
            InsertToLogDB(db1, "ERROR", messageLog, session_id, nomeRequisito, statoReq);

            std::cout << "Errore: Le password non corrispondono." << std::endl;
            return;
        }

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

        // Transformo la data di compleanno in una stringa che accetta il database
        std::string formatted_date = codifica_dataCompleanno(in_data_compleanno);

        // Se tutti i check hanno dato buoni risultati, possiamo aggiungere l'utente al database:

        // Inserisco nel database una riga corrispettiva al nuovo utente con tutti i campi presi in input dal metodo:
        sprintf(sqlcmd, "INSERT INTO UtenteCompratore (nome_utente_compratore, session_id_c, categoriaUtente, nome, cognome, indirizzo_mail, numero_di_telefono, password, data_compleanno, via_di_residenza, numero_civico, CAP, citta_di_residenza, stato ) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d)",
                in_nome_utente.c_str(), sessionID.c_str(), in_categoria.c_str(), in_nome.c_str(), in_cognome.c_str(), in_email.c_str(), in_numero_telefono.c_str(), in_password.c_str(), formatted_date.c_str(), in_via_residenza.c_str(), in_numero_civico.c_str(), in_CAP.c_str(), in_città_residenza.c_str(), stato);

        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Conferma di inserimento nel db
        std::cout << "Utente compratore inserito." << std::endl;

        // Log
        statoReq = statoRequisito::Success;
        messageLog = "Utente compratore " + in_nome_utente + " inserito";
        InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

        // Riempio il costruttore dell'utente compratore con i campi dati in input al metodo effettua registrazione:
        *this = UtenteCompratore(in_nome_utente, in_categoria, in_nome, in_cognome, in_numero_telefono, in_password, in_email, session_id, formatted_date, in_via_residenza, in_numero_civico, in_CAP, in_città_residenza, stato);

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

            // Registra un errore nel log del database e stampa un messaggio di errore
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

            // Registra un errore nel log del database e stampa un messaggio di errore
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
        sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", in_nome_utente.c_str());
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

        // Verifica se il nome utente è già presente nella tabella UtenteFornitore
        sprintf(sqlcmd, "SELECT * FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        PQclear(res);

        // Se il numero di righe dal risultato della query è maggiore di 0, l'utente già esiste nel database nella tabella degli utenti fornitori, ed è stato già registrato.
        if (rows >= 1)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Il nome utente " + in_nome_utente + " è già in uso da utenti fornitori. ";
            InsertToLogDB(db1, "ERROR", messageLog, session_id, nomeRequisito, statoReq);

            std::cout << "Errore: Il nome utente è già in uso da utenti fornitori." << std::endl;
            return false;
        }

        // Verifica se il nome utente è già presente nella tabella UtenteTrasportatore
        sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        // Se il numero di righe dal risultato della query è maggiore di 0, l'utente già esiste nel database nella tabella degli utenti trasportatori, ed è stato già registrato.
        if (rows >= 1)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Il nome utente " + in_nome_utente + " è già in uso da utenti trasportatori. ";
            InsertToLogDB(db1, "ERROR", messageLog, session_id, nomeRequisito, statoReq);

            std::cout << "Errore: Il nome utente è già in uso da utenti trasportatori." << std::endl;
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
        sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE indirizzo_mail = '%s'", in_email.c_str());
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

    // Metodo per codificare una data di compleanno nel formato richiesto per il database.
    std::string codifica_dataCompleanno(std::string in_data_compleanno)
    {

        // Trasformo la data di compleanno nel formato da inserire nel db:
        // 02/03/2023 in 02-03-2023.
        std::string formatted_date;
        std::stringstream ss(in_data_compleanno); // Utilizza un stringstream per manipolare la stringa
        std::string token;

        // Legge i token separati dal carattere '/'
        while (std::getline(ss, token, '/'))
        { // Assumendo che la data sia nel formato "GG/MM/AAAA"

            // Aggiunge il token alla stringa formattata seguito da '-'
            formatted_date += token + "-";
        }

        // Rimuove il carattere '-' in eccesso alla fine
        formatted_date.pop_back(); // Rimuove il carattere '-' in eccesso alla fine

        // Restituisce la data di compleanno nel formato richiesto per il database
        return formatted_date;
    }

    // Metodo per verificare se un sessionID è univoco tra gli utenti compratori, fornitori e trasportatori
    bool check_sessionID(Con2DB db1, std::string nomeRequisito, statoRequisito statoReq, std::string sessionID)
    {

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
            InsertToLogDB(db1, "ERROR", "Il session ID è già in uso da utenti compratori.", sessionID, nomeRequisito, statoReq);

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
            InsertToLogDB(db1, "ERROR", "Il session ID è già in uso da utenti fornitori.", sessionID, nomeRequisito, statoReq);

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
            InsertToLogDB(db1, "ERROR", "Il session ID è già in uso da utenti trasportatori.", sessionID, nomeRequisito, statoReq);

            std::cout << "Errore: Il session ID è già in uso da utenti trasportatori." << std::endl;
            return false;
        }

        // Restituiamo true se il sessionID è univoco tra tutti gli utenti
        return result;
    }


    UtenteCompratore anima_oggetto(Con2DB db1, std::string categoriaUtenteLogin, std::string input_nome_utente, std::string input_passw)
    {

        // Creo il costruttore della classe utente compratore dopo il login:
        UtenteCompratore compratore;
        sprintf(sqlcmd, "SELECT * FROM %s WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());

        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);
        if (rows == 1)
        {
            compratore.nome_utente = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_compratore"));
            compratore.session_id = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));
            compratore.categoria = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));
            compratore.nome = PQgetvalue(res, 0, PQfnumber(res, "nome"));
            compratore.cognome = PQgetvalue(res, 0, PQfnumber(res, "cognome"));
            compratore.email = PQgetvalue(res, 0, PQfnumber(res, "indirizzo_mail"));
            compratore.numero_telefono = PQgetvalue(res, 0, PQfnumber(res, "numero_di_telefono"));
            compratore.password = PQgetvalue(res, 0, PQfnumber(res, "password"));
            compratore.data_compleanno = PQgetvalue(res, 0, PQfnumber(res, "data_compleanno"));
            compratore.via_residenza = PQgetvalue(res, 0, PQfnumber(res, "via_di_residenza"));
            compratore.numero_civico = PQgetvalue(res, 0, PQfnumber(res, "numero_civico"));
            compratore.CAP = PQgetvalue(res, 0, PQfnumber(res, "CAP"));
            compratore.città_residenza = PQgetvalue(res, 0, PQfnumber(res, "citta_di_residenza"));
            compratore.stato = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));
        }
        else
        {
            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
            return compratore;
        }
        std::cout << "Nessun errore in anima oggetto!" << std::endl;
        return compratore;
    }


    // Metodo utilizzato per effettuare il login di un utente dato il suo nome utente, la sua password e il sessionID che sarà creato dal server.
    void effettua_login(Con2DB db1, std::string input_nome_utente, std::string input_passw, std::string sessionID)
    {
        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Login utente.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Controlliamo se l'utente è già loggato, che varia a seconda del valore dell'attributo "stato" all'interno del db.
        int stato_utente;

        // Viene recuperato il valore dell'attributo "stato".
        sprintf(sqlcmd, "SELECT stato FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str());
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

                // Caricamento del sessionID per inserirlo nel log.
                sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str());
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);
                if (rows == 1)
                {
                    sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));
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

                // controllo se il sessionID generato dal server-customer è univoco.
                bool resultSession = check_sessionID(db1, nomeRequisito, statoReq, sessionID);
                if (resultSession == false)
                {
                    return;
                }

                // Viene effettuata la verifica delle password: confrontiamo la password data in input dall'utente che vuole effettuare il login e la password che si trova nel db dell'utente.
                std::string password_utente;
                char *password_u;

                // Viene recuperata la password nel database 
                sprintf(sqlcmd, "SELECT password FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str());
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
                    messageLog = "Utente " + input_nome_utente + " non trovato";
                    InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

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
                    messageLog = "Password " + input_passw + " non corretta.";
                    InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

                    std::cout << "Errore: La passowrd non è corretta, riprovare." << std::endl;
                    return;
                }

                // Se le password corrispondono, l'utente può collegarsi.
                else
                {
                    std::cout << "La passowrd inserita: " << password_utente << "è corretta." << std::endl;

                    // Aggiornamento del session id nella tabella dell'utente compratore
                    sprintf(sqlcmd, "UPDATE UtenteCompratore set session_id_c='%s' WHERE nome_utente_compratore = '%s'", sessionID.c_str(), input_nome_utente.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Log
                    statoReq = statoRequisito::Wait;
                    messageLog = "Aggiornamento sessionID per utente " + input_nome_utente;
                    InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);


                    // Viene effettuato l'aggiornamento dello stato.

                    // Aggiornamento "stato" UtenteCompratore
                    sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 1 WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    // Log
                    statoReq = statoRequisito::Success;
                    messageLog = "Aggiornamento stato per utente " + input_nome_utente;
                    InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);


                    // Animo l'oggetto UtenteCompratore
                    sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str());
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);

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

                        // Aggiorniamo l'oggetto chiamante con i dettagli dell'utente trovato
                        *this = UtenteCompratore(nome_utente, "UtenteCompratore", nome, cognome, numero_telefono, password, email, session_id, data_compleanno, via_residenza, numero_civico, CAP, città_residenza, stato);
                    }
                    else
                    {
                        std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                    }

                    PQclear(res);
                }
            }
        }

        // Se il numero di righe del risultato della query relativo allo "stato" dell'utente è diversa da 1, non si può recuperare il valore dell'attributo "stato".
        else
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Utente " + input_nome_utente + " non trovato";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);

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

        // Caricamento del sessionID a seconda della categoria dell'utente.
        std::string sessionID = "";

        sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtenteLogout.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows == 1) { sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c")); }
        PQclear(res);

        if (rows != 1)
        {
            // Log dell'errore e uscita dalla funzione
            messageLog = "Non esiste " + nomeUtenteLogout + " , poichè non è stato registrato, non si può effettuare il logout .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            return;
        }

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato, non può effettuare il logout .", sessionID, nomeRequisito, statoReq);
            return;
        }

        // Controlliamo se l'utente è già loggato, che varia a seconda del valore dell'attributo "stato" all'interno del db.
        int stato_utente;

        // Viene recuperato il valore dell'attributo "stato".
        sprintf(sqlcmd, "SELECT stato FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtenteLogout.c_str());
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
                sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 0 WHERE nome_utente_compratore = '%s'", nomeUtenteLogout.c_str());
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                // Log
                statoReq = statoRequisito::Success;
                messageLog = "Disconnessione per utente: " + nomeUtenteLogout;
                InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

                // Viene resettato il sessionID associato all'utente poichè la sessione è finita dato che l'utente si discollega
                nomeRequisito = "Aggiornamento sessionID.";

                // Aggiornamento del session id nella tabella dell'utente compratore
                sprintf(sqlcmd, "UPDATE UtenteCompratore set session_id_c='' WHERE nome_utente_compratore = '%s'", nomeUtenteLogout.c_str());
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                // Log
                statoReq = statoRequisito::Success;
                InsertToLogDB(db1, "INFO", "Aggiornamento sessionID", "", nomeRequisito, statoReq);
            }
        }

        // Se il numero di righe del risultato della query relativo allo "stato" dell'utente è diversa da 1, non si può recuperare il valore dell'attributo "stato".
        else
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Utente " + nomeUtenteLogout + " non trovato.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

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

        // Caricamento del sessionID. 
        std::string sessionID = "";

        sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtenteDaEliminare.c_str()); 
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));} 
        PQclear(res); 

        if (rows != 1){
            // Log dell'errore e uscita dalla funzione
            messageLog = "Non esiste " + nomeUtenteDaEliminare + " , poichè non è stato registrato, non si può eliminare il profilo .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            return;
        }                   

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == ""){
            // Log dell'errore e uscita dalla funzione
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato, non può essere eliminato il profilo .", sessionID, nomeRequisito, statoReq);
            return;
        }

        // Eliminazione utente compratore
        sprintf(sqlcmd, "DELETE FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtenteDaEliminare.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Log
        statoReq = statoRequisito::Success;
        messageLog = "Eliminazione profilo utente " + nomeUtenteDaEliminare; 
        InsertToLogDB(db1, "INFO", messageLog , sessionID, nomeRequisito, statoReq);
        
    return;
    }


    // Metodo utilizzato per effettuare l'aggiornamento del numero di telefono di un utente.
    void aggiornaNumeroDiTelefono(Con2DB db1, std::string input_nome_utente ,std::string nuovoNumeroTelefono)
    {
        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Aggiornamento numero di telefono.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Caricamento del sessionID a seconda della categoria dell'utente. 
        std::string sessionID = "";

        sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str()); 
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));} 
        PQclear(res);      

        if (rows != 1){
            // Log dell'errore e uscita dalla funzione
            messageLog = "Non esiste " + input_nome_utente + " , poichè non è stato registrato, non si può aggiornare il numero di telefono .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            return;
        }               

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == ""){
            // Log dell'errore e uscita dalla funzione
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato, non può essere aggiornato il numero di telefono .", sessionID, nomeRequisito, statoReq);
            return;
        }

        // Aggiornamento del numero di telefono dell'utente compratore.
        sprintf(sqlcmd, "UPDATE UtenteCompratore set numero_di_telefono = '%s' WHERE nome_utente_compratore = '%s'", nuovoNumeroTelefono.c_str(), input_nome_utente.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Log
        statoReq = statoRequisito::Success;
        messageLog = "Aggiornamento numero di telefono : " + nuovoNumeroTelefono +" per utente: " + input_nome_utente;
        InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
            
    return;
    }



    // Metodo utilizzato per effettuare l'aggiornamento della password di un utente.
    void aggiornaPassword(Con2DB db1, std::string input_nome_utente, std::string vecchiaPassw, std::string nuovaPassw)
    {
        std::string passwDB = "";

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Aggiornamento password.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Caricamento del sessionID a seconda della categoria dell'utente. 
        std::string sessionID = "";

        sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str()); 
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));} 
        PQclear(res);          

        if (rows != 1){
            // Log dell'errore e uscita dalla funzione
            messageLog = "Non esiste " + input_nome_utente + " , poichè non è stato registrato, non si può aggiornare la password .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            return;
        }              

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == ""){
            // Log dell'errore e uscita dalla funzione
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato, non può essere aggiornata la password .", sessionID, nomeRequisito, statoReq);
            return;
        }

        // Verifichiamo che la vecchiaPassword inserita dall'utente è uguale a quella nel database relativa all'utente che vuole aggiornarla:

        // Recuperiamo la password dell'utente all'interno del db:
        sprintf(sqlcmd, "SELECT password FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str()); 
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows==1){ passwDB = PQgetvalue(res, 0, PQfnumber(res, "password"));} 
        PQclear(res);  


        // Se la password del db è diversa dalla password inserita dall'utente allora non può essere effettuata l'azione
        if (passwDB != vecchiaPassw)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "La password inserita " + vecchiaPassw +"  non è corretta.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
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
                messageLog = "La nuova password inserita " + nuovaPassw + " deve contenere almeno 8 caratteri.";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
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
                messageLog = "La nuova password inserita " + nuovaPassw + " deve contenere almeno un carattere maiuscolo.";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

                return;
            }

            // Verifica se la password contiene almeno un numero
            if (!hasDigit)
            {
                // Stampa un messaggio di errore e registra l'errore nel log del database
                std::cout << "La nuova password deve contenere almeno un numero." << std::endl;

                statoReq = statoRequisito::NotSuccess;
                messageLog = "La nuova password inserita " + nuovaPassw + " deve contenere almeno un numero.";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

                return;
            }

            if (!hasSpecialChar)
            {
                // Stampa un messaggio di errore e registra l'errore nel log del database
                std::cout << "La nuova password deve contenere almeno un carattere speciale." << std::endl;

                statoReq = statoRequisito::NotSuccess;
                messageLog = "La nuova password inserita " + nuovaPassw + " deve contenere almeno un carattere speciale.";
                InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

                return;
            }


            // Aggiornamento "password" UtenteCompratore
            sprintf(sqlcmd, "UPDATE UtenteCompratore set password = '%s' WHERE nome_utente_compratore = '%s'", nuovaPassw.c_str(), input_nome_utente.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Aggiornamento password (nuova password inserita: " + nuovaPassw + " ) per utente: " + input_nome_utente;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }
    return;
    }



    // Metodo utilizzato per aggiornare l'indirizzo di residenza di un utente compratore nel database
    void aggiornaResidenza(Con2DB db1, std::string input_nome_utente, std::string nuovaViaResidenza, std::string nuovoNumCiv, std::string nuovoCAP, std::string nuovaCittaResidenza)
    {
        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Aggiornamento residenza.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        std::string sessionID = "";

        // Caricamento del sessionID.
        sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows == 1)
        {
            sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));
        }
        PQclear(res);

        if (rows != 1)
        {
            // Log dell'errore e uscita dalla funzione
            messageLog = "Non esiste " + input_nome_utente + " , poichè non è stato registrato, non può essere aggiornata la residenza .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            return;
        }

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato, non può essere aggiornata la residenza.", sessionID, nomeRequisito, statoReq);
            return;
        }

        // Aggiornamento dell'indirizzo di residenza dell'utente compratore dato in input nel database
        sprintf(sqlcmd, "UPDATE UtenteCompratore set via_di_residenza='%s', numero_civico ='%s', CAP='%s', citta_di_residenza='%s' WHERE nome_utente_compratore = '%s'",
                nuovaViaResidenza.c_str(), nuovoNumCiv.c_str(), nuovoCAP.c_str(), nuovaCittaResidenza.c_str(), input_nome_utente.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Log
        statoReq = statoRequisito::Success;
        messageLog = "Aggiornata residenza utente compratore: " + input_nome_utente;
        InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

        return;
    }
};

#endif // USER_COMPRATORE_H
