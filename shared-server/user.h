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

    void effettua_login(Con2DB db1, std::string input_nome_utente, std::string input_passw, std::string sessionID)
    {

        std::string nomeRequisito = "Login utente.";
        statoRequisito statoReq = statoRequisito::Wait;

        std::string categoriaUtenteLogin = categoria;
        // std::string sessionID = "";

        // Controlla se l'utente è già loggato:
        int stato_utente;

        // Prendo lo stato dell'utente a seconda della categoria.
        // UTENTE COMPRATORE
        if (categoriaUtenteLogin == "UtenteCompratore")
        {
            sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
        }

        // UTENTE FORNITORE
        if (categoriaUtenteLogin == "UtenteFornitore")
        {
            sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
        }

        // UTENTE TRASPORTATORE
        if (categoriaUtenteLogin == "UtenteTrasportatore")
        {
            sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
        }
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Riga contenente lo stato dell'utente con nome utente : std::string input_nome_utente
        if (rows == 1)
        {
            stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

            if (stato_utente == 1)
            {
                std::cout << "L'utente è già connesso." << std::endl;

                // Carico il session ID:

                if (categoria == "UtenteCompratore")
                {
                    sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str());
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    if (rows == 1)
                    {
                        sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));
                    }
                }

                // animaUtenteCompratore(db1, input_nome_utente);

                if (categoria == "UtenteFornitore")
                {
                    sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", input_nome_utente.c_str());
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    if (rows == 1)
                    {
                        sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));
                    }
                }

                if (categoria == "UtenteTrasportatore")
                {
                    if (rows == 1)
                    {
                        sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", input_nome_utente.c_str());
                        res = db1.ExecSQLtuples(sqlcmd);
                        rows = PQntuples(res);
                        sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));
                    }
                }

                statoReq = statoRequisito::NotSuccess;

                InsertToLogDB(db1, "WARNING", "Utente già connesso.", sessionID, nomeRequisito, statoReq);
                return;
            }

            else

            {
                // Aggiorniamo lo stato dell'utente: da disconnesso a connesso:

                // SESSION ID
                // Generiamo il session id:
                // std::string sessionID = generateSessionID();

                // Controllo se il sessionID sia univoco con i session ID di tutte le tipologie d'utente:
                sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE session_id_c = '%s'", sessionID.c_str());
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);

                if (rows > 0)
                {

                    statoReq = statoRequisito::NotSuccess;

                    InsertToLogDB(db1, "ERROR", "Il session ID è già in uso da utenti compratori.", sessionID, nomeRequisito, statoReq);
                    std::cout << "Errore: Il session ID è già in uso da utenti compratori." << std::endl;
                    return;
                }

                sprintf(sqlcmd, "SELECT * FROM UtenteFornitore WHERE session_id_f = '%s'", sessionID.c_str());
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);

                if (rows > 0)
                {

                    statoReq = statoRequisito::NotSuccess;

                    InsertToLogDB(db1, "ERROR", "Il session ID è già in uso da utenti fornitori.", sessionID, nomeRequisito, statoReq);
                    std::cout << "Errore: Il session ID è già in uso da utenti fornitori." << std::endl;
                    return;
                }

                sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore WHERE session_id_t = '%s'", sessionID.c_str());
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);

                if (rows > 0)
                {

                    statoReq = statoRequisito::NotSuccess;

                    InsertToLogDB(db1, "ERROR", "Il session ID è già in uso da utenti trasportatori.", sessionID, nomeRequisito, statoReq);
                    std::cout << "Errore: Il session ID è già in uso da utenti trasportatori." << std::endl;
                    return;
                }

                // Se è univoco aggiorniamo il sessionID nella tabella corrette:
                if (categoriaUtenteLogin == "UtenteCompratore")
                {
                    sprintf(sqlcmd, "UPDATE %s set session_id_c='%s' WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), sessionID.c_str(), input_nome_utente.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    statoReq = statoRequisito::Wait;

                    InsertToLogDB(db1, "INFO", "Aggiornamento sessionID", sessionID, nomeRequisito, statoReq);
                }

                if (categoriaUtenteLogin == "UtenteFornitore")
                {
                    sprintf(sqlcmd, "UPDATE %s set session_id_f='%s' WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), sessionID.c_str(), input_nome_utente.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    statoReq = statoRequisito::Wait;

                    InsertToLogDB(db1, "INFO", "Aggiornamento sessionID", sessionID, nomeRequisito, statoReq);
                }

                if (categoriaUtenteLogin == "UtenteTrasportatore")
                {
                    sprintf(sqlcmd, "UPDATE %s set session_id_t='%s' WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), sessionID.c_str(), input_nome_utente.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    statoReq = statoRequisito::Wait;

                    InsertToLogDB(db1, "INFO", "Aggiornamento sessionID", sessionID, nomeRequisito, statoReq);
                }

                // Verifica della password:
                std::string password_utente;
                char *password_u;

                // UTENTE COMPRATORE
                if (categoriaUtenteLogin == "UtenteCompratore")
                {
                    // sprintf(sqlcmd, "SELECT password FROM UtenteCompratore WHERE nome_utente = '%s'", input_nome_utente.c_str());   // SELECT password FROM UtenteCompratore WHERE nome_utente = 'fede14';
                    sprintf(sqlcmd, "SELECT password FROM %s WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                }

                if (categoriaUtenteLogin == "UtenteFornitore")
                {
                    // sprintf(sqlcmd, "SELECT password FROM UtenteCompratore WHERE nome_utente = '%s'", input_nome_utente.c_str());   // SELECT password FROM UtenteCompratore WHERE nome_utente = 'fede14';
                    sprintf(sqlcmd, "SELECT password FROM %s WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                }

                if (categoriaUtenteLogin == "UtenteTrasportatore")
                {
                    // sprintf(sqlcmd, "SELECT password FROM UtenteCompratore WHERE nome_utente = '%s'", input_nome_utente.c_str());   // SELECT password FROM UtenteCompratore WHERE nome_utente = 'fede14';
                    sprintf(sqlcmd, "SELECT password FROM %s WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                }

                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);

                if (rows == 1)
                {
                    password_u = PQgetvalue(res, 0, PQfnumber(res, "password"));
                    password_utente.assign(password_u);
                }
                else
                {
                    // Altrimenti, il nome utente non è stato trovato o ci sono più utenti con lo stesso nome utente
                    // Gestisci questa situazione di conseguenza

                    statoReq = statoRequisito::NotSuccess;

                    InsertToLogDB(db1, "ERROR", "Utente non trovato.", sessionID, nomeRequisito, statoReq);
                    std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                    return;
                }

                // Ricorda di liberare la memoria del risultato della query
                PQclear(res);

                // Controllo della password data in input e quella segnata nel database:
                if (input_passw != password_utente)
                {
                    std::cout << "Errore: La passowrd non è corretta, riprovare." << std::endl;

                    statoReq = statoRequisito::NotSuccess;

                    InsertToLogDB(db1, "ERROR", "Password non corretta", sessionID, nomeRequisito, statoReq);
                    return;
                }
                else
                {
                    std::cout << "La passowrd inserita: " << password_utente << "è corretta." << std::endl;

                    // Aggiorno stato:

                    if (categoriaUtenteLogin == "UtenteCompratore")
                    {
                        // sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 1 WHERE nome_utente = '%s'", input_nome_utente.c_str());
                        sprintf(sqlcmd, "UPDATE %s set stato = 1 WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                        res = db1.ExecSQLcmd(sqlcmd);

                        PQclear(res);

                        statoReq = statoRequisito::Success;

                        InsertToLogDB(db1, "INFO", "Aggiornamento dello stato utente", sessionID, nomeRequisito, statoReq);
                    }

                    if (categoriaUtenteLogin == "UtenteFornitore")
                    {
                        // sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 1 WHERE nome_utente = '%s'", input_nome_utente.c_str());
                        sprintf(sqlcmd, "UPDATE %s set stato = 1 WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                        res = db1.ExecSQLcmd(sqlcmd);

                        PQclear(res);

                        statoReq = statoRequisito::Success;

                        InsertToLogDB(db1, "INFO", "Aggiornamento dello stato utente", sessionID, nomeRequisito, statoReq);
                    }

                    if (categoriaUtenteLogin == "UtenteTrasportatore")
                    {
                        // sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 1 WHERE nome_utente = '%s'", input_nome_utente.c_str());
                        sprintf(sqlcmd, "UPDATE %s set stato = 1 WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                        res = db1.ExecSQLcmd(sqlcmd);

                        PQclear(res);

                        statoReq = statoRequisito::Success;

                        InsertToLogDB(db1, "INFO", "Aggiornamento dello stato utente", sessionID, nomeRequisito, statoReq);
                    }
                }
            }
        }
        else
        {
            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1, "ERROR", "Utente non trovato", sessionID, nomeRequisito, statoReq);
            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
            return;
        }

        return;
    }

    std::string generateSessionID()
    {
        // Caratteri validi per il Session ID
        const std::string valid_characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

        // Inizializzazione del generatore di numeri casuali
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, valid_characters.size() - 1);

        // Generazione del Session ID casuale
        std::string session_id;
        for (int i = 0; i < 10; ++i)
        {
            session_id += valid_characters[dis(gen)];
        }

        return session_id;
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

    void effettua_logout(Con2DB db1)
    {

        // Utilizza i membri dell'istanza corrente per ottenere il nome utente
        std::string nomeUtenteLogout = nome_utente;

        std::string nomeRequisito = "Logout utente.";
        statoRequisito statoReq = statoRequisito::Wait;

        // Controlla se l'utente è già loggato:
        int stato_utente;
        std::string categoriaUtenteLogin = categoria;
        std::string sessionID = "";

        if (categoriaUtenteLogin == "UtenteCompratore")
        {
            sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str());
        }

        if (categoriaUtenteLogin == "UtenteFornitore")
        {
            sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str());
        }

        if (categoriaUtenteLogin == "UtenteTrasportatore")
        {
            sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str());
        }

        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        if (rows == 1)
        {

            // Importante che come parametri PQgetvalues al secondo campo indica il numero di riga, dato che c'è solo uno stato di un'utente, si troverà all'indice 0 della riga
            stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

            if (stato_utente == 0)
            {

                statoReq = statoRequisito::NotSuccess;

                InsertToLogDB(db1, "ERROR", "Utente è già disconnesso", sessionID, nomeRequisito, statoReq);
                std::cout << "Errore: L'utente è già disconnesso" << std::endl;
                return;
            }

            else
            {
                // Aggiorniamo lo stato dell'utente, da connesso a disconnesso:

                if (categoria == "UtenteCompratore")
                {
                    sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtenteLogout.c_str());
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    if (rows == 1)
                    {
                        sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));
                    }
                }

                if (categoria == "UtenteFornitore")
                {
                    sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", nomeUtenteLogout.c_str());
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    if (rows == 1)
                    {
                        sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));
                    }
                }

                if (categoria == "UtenteTrasportatore")
                {
                    if (rows == 1)
                    {
                        sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", nomeUtenteLogout.c_str());
                        res = db1.ExecSQLtuples(sqlcmd);
                        rows = PQntuples(res);
                        sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));
                    }
                }

                if (categoriaUtenteLogin == "UtenteCompratore")
                {
                    // sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 0 WHERE nome_utente = '%s'", input_nome_utente.c_str());
                    sprintf(sqlcmd, "UPDATE %s set stato = 0 WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    statoReq = statoRequisito::Success;

                    InsertToLogDB(db1, "INFO", "Disconnessione utente", sessionID, nomeRequisito, statoReq);
                }

                if (categoriaUtenteLogin == "UtenteFornitore")
                {
                    // sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 0 WHERE nome_utente = '%s'", input_nome_utente.c_str());
                    sprintf(sqlcmd, "UPDATE %s set stato = 0 WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    statoReq = statoRequisito::Success;

                    InsertToLogDB(db1, "INFO", "Disconnessione utente", sessionID, nomeRequisito, statoReq);
                }

                if (categoriaUtenteLogin == "UtenteTrasportatore")
                {
                    // sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 0 WHERE nome_utente = '%s'", input_nome_utente.c_str());
                    sprintf(sqlcmd, "UPDATE %s set stato = 0 WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    statoReq = statoRequisito::Success;

                    InsertToLogDB(db1, "INFO", "Disconnessione utente", sessionID, nomeRequisito, statoReq);
                }

                nomeRequisito = "Aggiornamento sessionID.";
                // A questo punto possiamo resettare il session id associato all'utente:
                if (categoriaUtenteLogin == "UtenteCompratore")
                {
                    sprintf(sqlcmd, "UPDATE %s set session_id_c='' WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    statoReq = statoRequisito::Success;

                    InsertToLogDB(db1, "INFO", "Aggiornamento sessionID", "", nomeRequisito, statoReq);
                }

                if (categoriaUtenteLogin == "UtenteFornitore")
                {
                    sprintf(sqlcmd, "UPDATE %s set session_id_f='' WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    statoReq = statoRequisito::Success;

                    InsertToLogDB(db1, "INFO", "Aggiornamento sessionID", "", nomeRequisito, statoReq);
                }

                if (categoriaUtenteLogin == "UtenteTrasportatore")
                {
                    sprintf(sqlcmd, "UPDATE %s set session_id_t='' WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    statoReq = statoRequisito::Success;

                    InsertToLogDB(db1, "INFO", "Aggiornamento sessionID", "", nomeRequisito, statoReq);
                }

                // Controlla se lo stato dell'utente è stato aggiornato:

                if (categoriaUtenteLogin == "UtenteCompratore")
                {
                    sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str());
                }

                if (categoriaUtenteLogin == "UtenteFornitore")
                {
                    sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str());
                }

                if (categoriaUtenteLogin == "UtenteTrasportatore")
                {
                    sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), nomeUtenteLogout.c_str());
                }

                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);

                if (rows == 1)
                {
                    stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));
                    std::cout << "Lo stato dell'utente " << nomeUtenteLogout << " dopo il logout è: " << stato_utente << std::endl;
                    std::cout << "\n\n"
                              << std::endl;
                }
                else
                {
                    std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                    return;
                }
            }
        }
        else
        {
            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1, "ERROR", "Utente non trovato.", sessionID, nomeRequisito, statoReq);
            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
            return;
        }
        return;
    }

    void elimina_profilo(Con2DB db1)
    {
        // Per seguire i paradigmi di OOP:

        // Utilizza i membri dell'istanza corrente per ottenere il nome utente e la categoria dell'utente
        std::string nomeUtenteDaEliminare = nome_utente;

        std::string categoriaUtenteDaEliminare = categoria;

        std::string nomeRequisito = "Eliminazione profilo.";
        statoRequisito statoReq = statoRequisito::Wait;

        std::string sessionID = "";
        if (categoria == "UtenteCompratore")
        {
            sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows == 1)
            {
                sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));
            }
        }

        if (categoria == "UtenteFornitore")
        {
            sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows == 1)
            {
                sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));
            }
        }

        if (categoria == "UtenteTrasportatore")
        {
            if (rows == 1)
            {
                sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", nomeUtenteDaEliminare.c_str());
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);
                sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));
            }
        }

        nomeRequisito = "Aggiornamento sessionID.";
        // A questo punto possiamo resettare il session id associato all'utente:
        if (categoria == "UtenteCompratore")
        {
            sprintf(sqlcmd, "UPDATE %s set session_id_c='' WHERE nome_utente_compratore = '%s'", categoria.c_str(), nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            statoReq = statoRequisito::Success;

            InsertToLogDB(db1, "INFO", "Aggiornamento sessionID", "", nomeRequisito, statoReq);
        }

        if (categoria == "UtenteFornitore")
        {
            sprintf(sqlcmd, "UPDATE %s set session_id_f='' WHERE nome_utente_fornitore = '%s'", categoria.c_str(), nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            statoReq = statoRequisito::Success;

            InsertToLogDB(db1, "INFO", "Aggiornamento sessionID", "", nomeRequisito, statoReq);
        }

        if (categoria == "UtenteTrasportatore")
        {
            sprintf(sqlcmd, "UPDATE %s set session_id_t='' WHERE nome_utente_trasportatore = '%s'", categoria.c_str(), nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            statoReq = statoRequisito::Success;

            InsertToLogDB(db1, "INFO", "Aggiornamento sessionID", "", nomeRequisito, statoReq);
        }


        // In base alla categoria dell'utente eliminiamo l'utente
        if (categoria == "UtenteCompratore")
        {
            sprintf(sqlcmd, "DELETE FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            statoReq = statoRequisito::Success;

            InsertToLogDB(db1, "INFO", "Eliminazione profilo.", sessionID, nomeRequisito, statoReq);
        }

        if (categoria == "UtenteFornitore")
        {
            sprintf(sqlcmd, "DELETE FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            statoReq = statoRequisito::Success;

            InsertToLogDB(db1, "INFO", "Eliminazione profilo.", sessionID, nomeRequisito, statoReq);
        }

        if (categoria == "UtenteTrasportatore")
        {
            sprintf(sqlcmd, "DELETE FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            statoReq = statoRequisito::Success;

            InsertToLogDB(db1, "INFO", "Eliminazione profilo.", sessionID, nomeRequisito, statoReq);
        }

        return;
    }



    void aggiornaNumeroDiTelefono(Con2DB db1, std::string nuovoNumeroTelefono)
    {

        // Utilizza i membri dell'istanza corrente per ottenere il nome utente e la categoria dell'utente
        std::string nomeUtente = nome_utente;

        std::string nomeRequisito = "Aggiornamento numero di telefono.";
        statoRequisito statoReq = statoRequisito::Wait;

        // std::string categoriaUtente = categoria;

        std::string sessionID = "";
        if (categoria == "UtenteCompratore")
        {
            sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtente.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows == 1)
            {
                sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));
            }
        }

        if (categoria == "UtenteFornitore")
        {
            sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", nomeUtente.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows == 1)
            {
                sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));
            }
        }

        if (categoria == "UtenteTrasportatore")
        {
            if (rows == 1)
            {
                sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", nomeUtente.c_str());
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);
                sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));
            }
        }

        if (sessionID == ""){
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non può essere aggiornato il numero di telefono.", sessionID, nomeRequisito, statoReq);
            return;
        }

        // In base alla categoria dell'utente aggiorniamo il numero di telefono
        if (categoria == "UtenteCompratore")
        {
            sprintf(sqlcmd, "UPDATE UtenteCompratore set numero_di_telefono = '%s' WHERE nome_utente_compratore = '%s'", nuovoNumeroTelefono.c_str(), nomeUtente.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            statoReq = statoRequisito::Success;

            InsertToLogDB(db1, "INFO", "Aggiornamento numero di telefono.", sessionID, nomeRequisito, statoReq);
        }
        if (categoria == "UtenteFornitore")
        {
            sprintf(sqlcmd, "UPDATE UtenteFornitore set numero_di_telefono = '%s' WHERE nome_utente_fornitore = '%s'", nuovoNumeroTelefono.c_str(), nomeUtente.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            statoReq = statoRequisito::Success;

            InsertToLogDB(db1, "INFO", "Aggiornamento numero di telefono.", sessionID, nomeRequisito, statoReq);
        }
        if (categoria == "UtenteTrasportatore")
        {
            sprintf(sqlcmd, "UPDATE UtenteTrasportatore set numero_di_telefono = '%s' WHERE nome_utente_trasportatore = '%s'", nuovoNumeroTelefono.c_str(), nomeUtente.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            statoReq = statoRequisito::Success;

            InsertToLogDB(db1, "INFO", "Aggiornamento numero di telefono.", sessionID, nomeRequisito, statoReq);
        }
        return;
    }




    void aggiornaPassword(Con2DB db1, std::string vecchiaPassw, std::string nuovaPassw)
    {
        // Utilizza i membri dell'istanza corrente per ottenere il nome utente e la categoria dell'utente
        std::string nomeUtente = nome_utente;
        std::string passwUtente = password;
        // std::string categoriaUtente = categoria;

        std::string nomeRequisito = "Aggiornamento password.";
        statoRequisito statoReq = statoRequisito::Wait;

        std::string sessionID = "";
        if (categoria == "UtenteCompratore")
        {
            sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtente.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows == 1)
            {
                sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));
            }
        }

        if (categoria == "UtenteFornitore")
        {
            sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", nomeUtente.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows == 1)
            {
                sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));
            }
        }

        if (categoria == "UtenteTrasportatore")
        {
            if (rows == 1)
            {
                sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", nomeUtente.c_str());
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);
                sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));
            }
        }


        if (sessionID == ""){
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non può essere aggiornata la password.", sessionID, nomeRequisito, statoReq);
            return;
        }


        // Controlliamo innanzitutto se la vecchiaPassword inserita dall'utente è uguale a quella nel database:
        // Verifica della password:
        if (passwUtente != vecchiaPassw)
        {

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1, "ERROR", "La password attuale inserita non è corretta.", sessionID, nomeRequisito, statoReq);
            std::cout << "Errore: La password attuale inserita non è corretta" << std::endl;
            return;
        }
        else
        {
            // La password inserita dall'utente è uguale a quella nel database:
            // Controllo se la nuova password rispetta i criteri: lunghezza di almeno 8, almeno una lettere maiuscola, un numero e un carattere speciale.
            if (nuovaPassw.length() < 8)
            {

                statoReq = statoRequisito::NotSuccess;

                InsertToLogDB(db1, "ERROR", "La nuova passowrd deve contenere almeno 8 caratteri.", sessionID, nomeRequisito, statoReq);
                std::cout << "Errore: La nuova passowrd deve contenere almeno 8 caratteri." << std::endl;
                return;
            }

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

            if (!hasUpperCase)
            {
                std::cout << "La nuova password deve contenere almeno un carattere maiuscolo." << std::endl;

                statoReq = statoRequisito::NotSuccess;
                InsertToLogDB(db1, "ERROR", "La nuova passowrd deve contenere almeno un carattere maiuscolo.", sessionID, nomeRequisito, statoReq);
            }

            if (!hasDigit)
            {
                std::cout << "La nuova password deve contenere almeno un numero." << std::endl;

                statoReq = statoRequisito::NotSuccess;
                InsertToLogDB(db1, "ERROR", "La nuova passowrd deve contenere almeno un numero.", sessionID, nomeRequisito, statoReq);
            }

            if (!hasSpecialChar)
            {
                std::cout << "La nuova password deve contenere almeno un carattere speciale." << std::endl;

                statoReq = statoRequisito::NotSuccess;
                InsertToLogDB(db1, "ERROR", "La nuova passowrd deve contenere almeno un carattere speciale.", sessionID, nomeRequisito, statoReq);
            }

            // In base alla categoria dell'utente aggiorniamo la password
            if (categoria == "UtenteCompratore")
            {
                sprintf(sqlcmd, "UPDATE UtenteCompratore set password = '%s' WHERE nome_utente_compratore = '%s'", nuovaPassw.c_str(), nomeUtente.c_str());
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                statoReq = statoRequisito::Success;

                InsertToLogDB(db1, "INFO", "Aggiornamento password.", sessionID, nomeRequisito, statoReq);
            }
            if (categoria == "UtenteFornitore")
            {
                sprintf(sqlcmd, "UPDATE UtenteFornitore set password = '%s' WHERE nome_utente_fornitore = '%s'", nuovaPassw.c_str(), nomeUtente.c_str());
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                statoReq = statoRequisito::Success;

                InsertToLogDB(db1, "INFO", "Aggiornamento password.", sessionID, nomeRequisito, statoReq);
            }
            if (categoria == "UtenteTrasportatore")
            {
                sprintf(sqlcmd, "UPDATE UtenteTrasportatore set password = '%s' WHERE nome_utente_trasportatore = '%s'", nuovaPassw.c_str(), nomeUtente.c_str());
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                statoReq = statoRequisito::Success;

                InsertToLogDB(db1, "INFO", "Aggiornamento password.", sessionID, nomeRequisito, statoReq);
            }
        }

        return;
    }
};

#endif // USER_H