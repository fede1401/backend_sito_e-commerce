#ifndef UTENTE_COMPRATORE_H
#define UTENTE_COMPRATORE_H

#include "../../shared-server/utente.h"


class UtenteCompratore : public Utente
{
public:
    // Attributi specifici per UtenteCompratore
    std::string m_dataCompleanno;
    std::string m_viaResidenza;
    std::string m_numeroCivico;
    std::string m_CAP;
    std::string m_cittàResidenza;

    // Costruttori di UtenteCompratore
    UtenteCompratore() : Utente("", "", "", "", "", "", "","", -1), m_dataCompleanno(""), m_viaResidenza(""), m_numeroCivico(""), m_CAP(""), m_cittàResidenza("") {}

    UtenteCompratore(
        std::string nome_utente, std::string session_id, std::string categoria, std::string nome, std::string cognome,
        std::string numero_telefono, std::string password, std::string email, int stato,
        std::string data_compleanno,
        std::string via_residenza, std::string numero_civico, std::string CAP, std::string città_residenza
        ) :
            Utente(nome_utente, session_id, categoria, nome, cognome, numero_telefono, password, email, stato), m_dataCompleanno(data_compleanno),
            m_viaResidenza(via_residenza), m_numeroCivico(numero_civico), m_CAP(CAP), m_cittàResidenza(città_residenza)
    {}

    // Metodo specifico per UtenteCompratore
    void mostraInformazioniCompratore()
    {
        mostraInformazioni(); // Chiama il metodo della classe base
        std::cout << "Data di compleanno: " << m_dataCompleanno << std::endl;
        std::cout << "Via di residenza: " << m_viaResidenza << std::endl;
        std::cout << "Numero civico: " << m_numeroCivico << std::endl;
        std::cout << "CAP: " << m_CAP << std::endl;
        std::cout << "Città di residenza: " << m_cittàResidenza << std::endl;
        std::cout << "Stato: " << m_stato << std::endl;
    }

    // Metodo per effettuare la registrazione di un utente compratore dati tutti i campi di input.
    std::string registrazione(Con2DB db1,
                               std::string in_nome_utente, std::string in_categoria, std::string in_nome, std::string in_cognome,
                               std::string in_sessionID,
                               std::string in_numero_telefono, std::string in_email,
                               std::string in_via_residenza, std::string in_numero_civico, std::string in_CAP, std::string in_città_residenza,
                               std::string in_password, std::string in_conferma_password,
                               std::string in_data_compleanno)
    {

        int stato = 1;

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";


        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Registrazione utente compratore.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Controllo che il nome utente sia univoco con gli altri utenti.
        bool resultUsername = check_nome_utente_univoco(db1, in_nome_utente, nomeRequisito, statoReq, in_sessionID);
        if (resultUsername == false)
        {
            result = "Il nome utente non è univoco, la registrazione non è andata a buon fine.";
            return result;
        }

        // Controllo che l'email sia univoca
        bool resultEmailUn = check_email_univoca(db1, in_email, nomeRequisito, statoReq, in_sessionID);
        if (resultEmailUn == false)
        {
            result = "L'email non è univoca, la registrazione non è andata a buon fine.";
            return result;
        }

        // Controllo se la mail contiene il carattere "@".
        bool resultEmail = check_email(db1, in_email, nomeRequisito, statoReq, in_sessionID);
        if (resultEmail == false)
        {
            result = "L'email non contiene il carattere -@-";
            return result;
        }

        // Controllo se la password rispetta i criteri: lunghezza di almeno 8, almeno una lettere maiuscola, un numero e un carattere speciale.
        bool resultPassw = check_password(db1, in_password, in_conferma_password, nomeRequisito, statoReq, in_sessionID);
        if (resultPassw == false)
        {
            result = "La password non rispetta i criteri definiti.";
            return result;
        }

        // Controllo se la password data in input è uguale alla conferma password.
        if (in_password != in_conferma_password)
        {
            // Log dell'errore (password e conferma password diverse) e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "La password inserita " + in_password + " non corrisponde con la conferma della password " + in_conferma_password;
            InsertToLogDB(db1, "ERROR", messageLog, in_sessionID, nomeRequisito, statoReq);

            std::cout << "Errore: Le password non corrispondono." << std::endl;

            result = "I valori del campo -password- e -conferma password- non sono uguali";
            return result;
        }


        // Transformo la data di compleanno in una stringa che accetta il database
        std::string formatted_date = codifica_dataCompleanno(in_data_compleanno);

        // Se tutti i check hanno dato buoni risultati, possiamo aggiungere l'utente al database:

        // Inserisco nel database una riga corrispettiva al nuovo utente con tutti i campi presi in input dal metodo:
        sprintf(sqlcmd, "INSERT INTO Utente (nome_utente, session_id, categoriaUtente, nome, cognome, indirizzo_mail, numero_di_telefono, password, stato ) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d)",
                in_nome_utente.c_str(), in_sessionID.c_str(), in_categoria.c_str(), in_nome.c_str(), in_cognome.c_str(), in_email.c_str(), in_numero_telefono.c_str(), in_password.c_str(), stato);

        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);


        // Inserisco nel database una riga corrispettiva al nuovo utente con tutti i campi presi in input dal metodo:
        sprintf(sqlcmd, "INSERT INTO UtenteCompratore (nome_utente_compratore, data_compleanno, via_di_residenza, numero_civico, CAP, citta_di_residenza ) VALUES ('%s', '%s', '%s', '%s', '%s', '%s')",
                in_nome_utente.c_str(), formatted_date.c_str(), in_via_residenza.c_str(), in_numero_civico.c_str(), in_CAP.c_str(), in_città_residenza.c_str());

        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Conferma di inserimento nel db
        std::cout << "Utente compratore inserito." << std::endl;

        // Log
        statoReq = statoRequisito::Success;
        messageLog = "Utente compratore " + in_nome_utente + " inserito";
        InsertToLogDB(db1, "INFO", messageLog, in_sessionID, nomeRequisito, statoReq);

        // Riempio il costruttore dell'utente compratore con i campi dati in input al metodo effettua registrazione:
        *this = UtenteCompratore(in_nome_utente, in_sessionID, in_categoria, in_nome, in_cognome, in_numero_telefono, in_password, in_email, stato, formatted_date, in_via_residenza, in_numero_civico, in_CAP, in_città_residenza);

        result = "Registrazione utente compratore : " + in_nome_utente + " avvenuta con successo.";
        return result;
    }



    // Metodo per verificare se un'email è valida per la registrazione di un utente.
    bool check_email(Con2DB db1, std::string in_email, std::string in_nome_requisito, statoRequisito in_stato_requisito, std::string in_sessionID)
    {
        bool result = true;

        // Verifica se l'email contiene il carattere '@'
        if (in_email.find("@") == std::string::npos)
        {
            // Se l'email non contiene '@', impostiamo lo stato del requisito a NotSuccess
            in_stato_requisito = statoRequisito::NotSuccess;

            // Log dell'errore e restituzione di false per indicare che l'email non è valida
            InsertToLogDB(db1, "ERROR", "La mail deve contenere il carattere -@-.", in_sessionID, in_nome_requisito, in_stato_requisito);
            std::cout << "Errore: La mail deve contenere il carattere '@'." << std::endl;
            return false;
        }

        // Restituisce true se la l'email soddisfa i criteri di validità
        return result;
    }

    // Metodo per verificare la validità di una password
    bool check_password(Con2DB db1, std::string in_password, std::string in_conferma_password, std::string in_nome_requisito, statoRequisito in_stato_requisito, std::string in_sessionID)
    {

        bool result = true;

        // Verifica se la lunghezza della password è inferiore a 8 caratteri
        if (in_password.length() < 8)
        {
            // Se la password è troppo corta, impostiamo lo stato del requisito a NotSuccess
            in_stato_requisito = statoRequisito::NotSuccess;

            // Registra un errore nel log del database e stampa un messaggio di errore
            InsertToLogDB(db1, "ERROR", "La password deve contenere almeno 8 caratteri.", in_sessionID, in_nome_requisito, in_stato_requisito);
            std::cout << "Errore: La passowrd deve contenere almeno 8 caratteri." << std::endl;

            // Restituisce false per indicare che la password non è valida
            return false;
        }

        // Verifica se la lunghezza della conferma della password è inferiore a 8 caratteri
        if (in_conferma_password.length() < 8)
        {
            // Se la password è troppo corta, impostiamo lo stato del requisito a NotSuccess
            in_stato_requisito = statoRequisito::NotSuccess;

            // Registra un errore nel log del database e stampa un messaggio di errore
            InsertToLogDB(db1, "ERROR", "La password deve contenere almeno 8 caratteri.", in_sessionID, in_nome_requisito, in_stato_requisito);
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
            in_stato_requisito = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "ERROR", "La nuova passowrd deve contenere almeno un carattere maiuscolo.", in_sessionID, in_nome_requisito, in_stato_requisito);

            // Restituisce false per indicare che la password non è valida
            return false;
        }

        // Verifica se la password contiene almeno un numero
        if (!hasDigit)
        {
            // Stampa un messaggio di errore e registra l'errore nel log del database
            std::cout << "La nuova password deve contenere almeno un numero." << std::endl;

            // Registra un errore nel log del database e stampa un messaggio di errore
            in_stato_requisito = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "ERROR", "La nuova passowrd deve contenere almeno un numero.", in_sessionID, in_nome_requisito, in_stato_requisito);

            // Restituisce false per indicare che la password non è valida
            return false;
        }

        // Verifica se la password contiene almeno un carattere speciale
        if (!hasSpecialChar)
        {
            // Stampa un messaggio di errore e registra l'errore nel log del database
            std::cout << "La nuova password deve contenere almeno un carattere speciale." << std::endl;

            // Registra un errore nel log del database e stampa un messaggio di errore
            in_stato_requisito = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "ERROR", "La nuova passowrd deve contenere almeno un carattere speciale.", in_sessionID, in_nome_requisito, in_stato_requisito);

            // Restituisce false per indicare che la password non è valida
            return false;
        }

        // Restituisce true se la password soddisfa tutti i criteri di validità
        return result;
    }

    // Metodo utilizzato per verificare che un nome utente è univoco tra gli utenti compratori, fornitori e trasportatori
    bool check_nome_utente_univoco(Con2DB db1, std::string in_nome_utente, std::string in_nome_requisito, statoRequisito in_stato_requisito, std::string in_sessionID)
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
            in_stato_requisito = statoRequisito::NotSuccess;
            messageLog = "Il nome utente " + in_nome_utente + " è già in uso. ";
            InsertToLogDB(db1, "ERROR", messageLog, in_sessionID, in_nome_requisito, in_stato_requisito);

            // Stampa messaggio di errore
            std::cout << "Errore: Il nome utente è già in uso." << std::endl;
            return false;
        }

        // Restituiamo true se il nome utente è univoco tra tutti gli utenti
        return result;
    }

    // Metodo utilizzato per verificare che un email è univoco tra gli utenti compratori.
    bool check_email_univoca(Con2DB db1, std::string in_email, std::string in_nome_requisito, statoRequisito in_stato_requisito, std::string in_sessionID)
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
            in_stato_requisito = statoRequisito::NotSuccess;
            messageLog = "Indirizzo mail " + in_email + " è già in uso. ";
            InsertToLogDB(db1, "ERROR", messageLog, in_sessionID, in_nome_requisito, in_stato_requisito);

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

    // Metodo utilizzato per aggiornare l'indirizzo di residenza di un utente compratore nel database
    std::string aggiorna_residenza(Con2DB db1, std::string in_nome_utente, std::string in_nuova_via_residenza, std::string in_nuovo_numero_civico, std::string in_nuovo_CAP, std::string in_nuova_citta_residenza)
    {
        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Aggiornamento residenza.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";


        std::string sessionID = "";

        // Caricamento del sessionID.
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
            messageLog = "Non esiste " + in_nome_utente + " , poichè non è stato registrato, non può essere aggiornata la residenza .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

            result = messageLog;
            return result;
        }

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + in_nome_utente + ", utente non loggato, non può essere aggiornata la residenza.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

            result = messageLog;
            return result;
        }

        // Verifichiamo che l'utente si tratti di un utente compratore:
        std::string categoriaUtente = "";
        sprintf(sqlcmd, "SELECT categoriaUtente FROM Utente WHERE nome_utente = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare la categoria dell'utente.
        if (rows==1){ categoriaUtente = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));}  
        PQclear(res);

        if (categoriaUtente != "UtenteCompratore"){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "L utente " + in_nome_utente + " non è un utente compratore, perciò non può essere aggiornata la residenza.";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);

            result = messageLog;
            return result;
        }

        // Aggiornamento dell'indirizzo di residenza dell'utente compratore dato in input nel database
        sprintf(sqlcmd, "UPDATE UtenteCompratore set via_di_residenza='%s', numero_civico ='%s', CAP='%s', citta_di_residenza='%s' WHERE nome_utente_compratore = '%s'",
                in_nuova_via_residenza.c_str(), in_nuovo_numero_civico.c_str(), in_nuovo_CAP.c_str(), in_nuova_citta_residenza.c_str(), in_nome_utente.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        // Log
        statoReq = statoRequisito::Success;
        messageLog = "Aggiornata residenza utente compratore: " + in_nome_utente;
        InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

        result = messageLog;
        return result;
    }


    // Metodo utilizzato per effettuare il login di un utente dato il suo nome utente, la sua password e il sessionID che sarà creato dal server.
    std::string login(Con2DB db1, std::string in_nome_utente, std::string in_password, std::string in_sessionID) override
    {
        try
        {
            // Definizione di alcune variabili per il logging
            std::string nomeRequisito = "Login utente compratore.";
            statoRequisito statoReq = statoRequisito::Wait;
            std::string messageLog = "";

            // Dichiarazione variabile per il risultato dell'operazione.
            std::string result = "";

            // Controllo prima di eseguire il login della superclasse se l'utente è un utenteCompratore, altrimenti non può essere esguito il login.
            sprintf(sqlcmd, "SELECT categoriaUtente FROM Utente WHERE nome_utente = '%s'", in_nome_utente.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows == 1){
                std::string categoria = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));

                PQclear(res);
                if (categoria != "UtenteCompratore"){
                    // Log dell'errore e uscita dalla funzione
                    statoReq = statoRequisito::NotSuccess;
                    messageLog = "Utente " + in_nome_utente + " è di un altra categoria, non un utente compratore, non può essere effettuato il login.";
                    InsertToLogDB(db1, "ERROR", messageLog, in_sessionID, nomeRequisito, statoReq);
                    
                    result = messageLog;
                    return result;
                } 
            }
            

            // Chiamata al metodo della classe base
            Utente::login(db1, in_nome_utente, in_password, in_sessionID);


            sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", in_nome_utente.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);

            if (rows == 1)
            {
                std::string data_compleanno = PQgetvalue(res, 0, PQfnumber(res, "data_compleanno"));
                std::string via_residenza = PQgetvalue(res, 0, PQfnumber(res, "via_di_residenza"));
                std::string numero_civico = PQgetvalue(res, 0, PQfnumber(res, "numero_civico"));
                std::string CAP = PQgetvalue(res, 0, PQfnumber(res, "CAP"));
                std::string città_residenza = PQgetvalue(res, 0, PQfnumber(res, "citta_di_residenza"));

                this->m_dataCompleanno = data_compleanno;
                this->m_viaResidenza = via_residenza;
                this->m_numeroCivico = numero_civico;
                this->m_CAP = CAP;
                this->m_cittàResidenza = città_residenza;
            }
            
            // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
            if (rows == 0)
            {
                // Log dell'errore e uscita dalla funzione
                statoReq = statoRequisito::NotSuccess;
                messageLog = "Utente " + in_nome_utente + " è di un altra categoria, non un utente compratore.";
                InsertToLogDB(db1, "ERROR", messageLog, in_sessionID, nomeRequisito, statoReq);
                
                result = messageLog;
                return result;
            }

            PQclear(res);
            return "Login utente compratore avvenuto con successo";
        }

        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return e.what();
        }
    }


};

#endif // UTENTE_COMPRATORE_H
