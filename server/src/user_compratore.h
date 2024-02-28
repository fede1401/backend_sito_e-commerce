#ifndef USER_COMPRATORE_H
#define USER_COMPRATORE_H

// #include "main.h"
#include "user.h"

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
    float saldo;
    int stato;

    // Costruttori di UtenteCompratore

    UtenteCompratore() : Utente("", "", "", "", "", "", ""),
                         session_id(""),
                         data_compleanno(""),
                         via_residenza(""),
                         numero_civico(""),
                         CAP(""),
                         città_residenza(""),
                         saldo(0.0),
                         stato(0) {}

    UtenteCompratore(
        std::string nome_utente,
        std::string categoria,
        std::string nome,
        std::string cognome,
        std::string numero_telefono,
        std::string password,
        std::string email,
        std::string session_id,
        std::string data_compleanno,
        std::string via_residenza,
        std::string numero_civico,
        std::string CAP,
        std::string città_residenza,
        float saldo,
        int stato) :

                     Utente(nome_utente, categoria, nome, cognome, numero_telefono, password, email),
                     session_id(session_id),
                     data_compleanno(data_compleanno),
                     via_residenza(via_residenza),
                     numero_civico(numero_civico),
                     CAP(CAP),
                     città_residenza(città_residenza),
                     saldo(saldo),
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
        std::cout << "Saldo: " << saldo << std::endl;
        std::cout << "Stato: " << stato << std::endl;
    }
    

    void effettuaRegistrazione( Con2DB db1,
                               std::string in_nome_utente,
                               std::string in_categoria,
                               std::string in_nome, std::string in_cognome,
                               std::string in_numero_telefono,
                               std::string in_email,
                               std::string in_via_residenza, std::string in_numero_civico, std::string in_CAP, std::string in_città_residenza,
                               std::string in_password, std::string in_conferma_password,
                               std::string in_data_compleanno)
    {

        int stato = 0;
        float saldo = 0.0;
        //std::string session_id = "";

        std::string nomeRequisito = "Registrazione utente compratore.";
        statoRequisito statoReq = statoRequisito::Wait;

        /////////////////////////////////////
        // Controllo se la mail contiene il carattere "@".
        if (in_email.find("@") == std::string::npos)
        {

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "La mail deve contenere il carattere -@-.", session_id, nomeRequisito, statoReq);
            std::cout << "Errore: La mail deve contenere il carattere '@'." << std::endl;
            return;
        }
        /////////////////////////////////////

        /////////////////////////////////////
        // Controllo se la password rispetta i criteri: lunghezza di almeno 8, almeno una lettere maiuscola, un numero e un carattere speciale.
        if (in_password.length() < 8)
        {

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "La password deve contenere almeno 8 caratteri.", session_id, nomeRequisito, statoReq);
            std::cout << "Errore: La passowrd deve contenere almeno 8 caratteri." << std::endl;
            return;
        }

        if (in_conferma_password.length() < 8)
        {

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "La password deve contenere almeno 8 caratteri.", session_id, nomeRequisito, statoReq);
            std::cout << "Errore: La passowrd deve contenere almeno 8 caratteri." << std::endl;
            return;
        }

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

        if (!hasUpperCase)
        {
            std::cout << "La nuova password deve contenere almeno un carattere maiuscolo." << std::endl;

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "La nuova passowrd deve contenere almeno un carattere maiuscolo.", session_id, nomeRequisito, statoReq);
            return;
        }

        if (!hasDigit)
        {
            std::cout << "La nuova password deve contenere almeno un numero." << std::endl;

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "La nuova passowrd deve contenere almeno un numero.", session_id, nomeRequisito, statoReq);
            return;
        }

        if (!hasSpecialChar)
        {
            std::cout << "La nuova password deve contenere almeno un carattere speciale." << std::endl;

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "La nuova passowrd deve contenere almeno un carattere speciale.", session_id, nomeRequisito, statoReq);
            return;
        }
        /////////////////////////////////////

        /////////////////////////////////////
        // Controllo se la password è uguale al campo conferma_password
        if (in_password != in_conferma_password)
        {

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "Le password non corrispondono", session_id, nomeRequisito, statoReq);
            std::cout << "Errore: Le password non corrispondono." << std::endl;
            return;
        }
        /////////////////////////////////////

        /////////////////////////////////////
        // Connessione al database:
        //Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");
        //std::cout << "Connessione al database avvenuta con successo." << std::endl;
        /////////////////////////////////////

        /////////////////////////////////////
        // Controllo se il nome utente è univoco
        sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", in_nome_utente.c_str());
        PGresult *res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        PQclear(res);
        if (rows > 0)
        {
            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "Il nome utente è già in uso.", session_id, nomeRequisito, statoReq);
            std::cout << "Errore: Il nome utente è già in uso." << std::endl;

            sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", in_nome_utente.c_str());
            
            res = db1.ExecSQLtuples(sqlcmd);

            std::cout << "RES: " << res << std::endl;

            rows = PQntuples(res);

            //std::cout << "RES: " << res << std::endl;
            //std::cout << "ROWS: " << rows << std::endl;

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
                float saldo = atof(PQgetvalue(res, 0, PQfnumber(res, "saldo")));
                int stato = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));


                *this = UtenteCompratore(nome_utente, "UtenteCompratore", nome, cognome, numero_telefono, password, email, session_id, data_compleanno, via_residenza, numero_civico, CAP, città_residenza, saldo, stato);

            }
            else
            {
                std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                
            }

            PQclear(res);

            return;
        }

        // Controlliamo anche se il nome sia univoco con le due altre tabelle degli utenti: UtenteFornitore e UtenteTrasportatore:
        sprintf(sqlcmd, "SELECT * FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        PQclear(res);
        if (rows >= 1)
        {

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "Il nome utente è già in uso da utenti fornitori.", session_id, nomeRequisito, statoReq);
            std::cout << "Errore: Il nome utente è già in uso da utenti fornitori." << std::endl;
            return;
        }

        sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);
        if (rows >= 1)
        {

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "Il nome utente è già in uso da utenti trasportatori.", session_id, nomeRequisito, statoReq);
            std::cout << "Errore: Il nome utente è già in uso da utenti trasportatori." << std::endl;
            return;
        }
        /////////////////////////////////////

        /////////////////////////////////////
        // Controllo se l'email è univoca
        sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE indirizzo_mail = '%s'", in_email.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        
        PQclear(res);
        if (rows > 0)
        {
            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "Indirizzo mail è già in uso.", session_id, nomeRequisito, statoReq);
            std::cout << "Errore: L'indirizzo mail è già in uso." << std::endl;
            return;
        }
        /////////////////////////////////////

        /////////////////////////////////////
        // Trasformo la data di compleanno nel formato da inserire nel db:
        // 02/03/2023 in 02-03-2023.
        std::string formatted_date;
        std::stringstream ss(in_data_compleanno); // Utilizza un stringstream per manipolare la stringa
        std::string token;
        while (std::getline(ss, token, '/'))
        { // Assumendo che la data sia nel formato "GG/MM/AAAA"
            formatted_date += token + "-";
        }
        formatted_date.pop_back(); // Rimuove il carattere '-' in eccesso alla fine
        /////////////////////////////////////

        

        std::cout << "Categoria utente:" << in_categoria << std::endl;

        // SESSION ID
        // Generiamo il session id:
        std::string sessionID = generateSessionID();

        // Controllo se il sessionID sia univoco con i session ID di tutte le tipologie d'utente:
        sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE session_id_c = '%s'", sessionID.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);
        if (rows > 0)
        {

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "Il session ID è già in uso da utenti compratori.", sessionID, nomeRequisito, statoReq);
            std::cout << "Errore: Il session ID è già in uso da utenti compratori." << std::endl;
            return;
        }

        sprintf(sqlcmd, "SELECT * FROM UtenteFornitore WHERE session_id_f = '%s'", sessionID.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);
        if (rows > 0)
        {

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "Il session ID è già in uso da utenti fornitori.", sessionID, nomeRequisito, statoReq);
            std::cout << "Errore: Il session ID è già in uso da utenti fornitori." << std::endl;
            return;
        }

        sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore WHERE session_id_t = '%s'", sessionID.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);
        if (rows > 0)
        {

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "Il session ID è già in uso da utenti trasportatori.", sessionID, nomeRequisito, statoReq);
            std::cout << "Errore: Il session ID è già in uso da utenti trasportatori." << std::endl;
            return;
        }

        /////////////////////////////////////
        // Inserisco nel database il nuovo utente:
        sprintf(sqlcmd, "INSERT INTO UtenteCompratore (nome_utente_compratore, session_id_c, categoriaUtente, nome, cognome, indirizzo_mail, numero_di_telefono, password, data_compleanno, via_di_residenza, numero_civico, CAP, citta_di_residenza, saldo, stato ) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %f, %d)",
                in_nome_utente.c_str(), sessionID.c_str(), in_categoria.c_str(), in_nome.c_str(), in_cognome.c_str(), in_email.c_str(), in_numero_telefono.c_str(), in_password.c_str(), formatted_date.c_str(), in_via_residenza.c_str(), in_numero_civico.c_str(), in_CAP.c_str(), in_città_residenza.c_str(), saldo, stato);

        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);
        /////////////////////////////////////

        // Conferma di inserimento nel db
        std::cout << "Utente inserito." << std::endl;

        statoReq = statoRequisito::Success;

        InsertToLogDB(db1,"INFO", "Utente compratore inserito.", sessionID, nomeRequisito, statoReq);

        /////////////////////////////////////
        // Riempio il costruttore dell'utente compratore con i campi dati in input al metodo effettua registrazione:
        *this = UtenteCompratore(in_nome_utente, in_categoria, in_nome, in_cognome, in_numero_telefono, in_password, in_email, session_id, formatted_date, in_via_residenza, in_numero_civico, in_CAP, in_città_residenza, saldo, stato);
        /////////////////////////////////////

        return;
    }

    UtenteCompratore anima_oggetto(Con2DB db1, std::string categoriaUtenteLogin, std::string input_nome_utente, std::string input_passw)
    {
        // Connession al database:
        // Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

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
            compratore.saldo = atof(PQgetvalue(res, 0, PQfnumber(res, "saldo")));
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

    void aggiornaResidenza(Con2DB db1, std::string nuovaViaResidenza, std::string nuovoNumCiv, std::string nuovoCAP, std::string nuovaCittaResidenza)
    {
        printf("ENTRATO NEL METODO\n");
        
        // Utilizza i membri dell'istanza corrente per ottenere il nome utente.
        std::string nomeUtente = nome_utente;

        std::string nomeRequisito = "Aggiornamento residenza.";
        statoRequisito statoReq = statoRequisito::Wait;

        // Connession al database:
        // Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        std::string sessionID = "";
        sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows == 1)
        {
            sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));
        }
        PQclear(res);

        sprintf(sqlcmd, "UPDATE UtenteCompratore set via_di_residenza='%s', numero_civico ='%s', CAP='%s', citta_di_residenza='%s' WHERE nome_utente_compratore = '%s'",
                nuovaViaResidenza.c_str(), nuovoNumCiv.c_str(), nuovoCAP.c_str(), nuovaCittaResidenza.c_str(), nomeUtente.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);

        statoReq = statoRequisito::Success;

        printf("FINO A INSERTTOLOGDB tutto ok\n");

        InsertToLogDB(db1,"INFO", "Aggiornata la residenza utente compratore.", sessionID, nomeRequisito, statoReq);

        return;
    }
};

#endif // USER_COMPRATORE_H
