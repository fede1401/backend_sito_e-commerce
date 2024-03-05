#ifndef USER_TRASPORTATORE_H
#define USER_TRASPORTATORE_H

#include "../../shared-server/user.h"



class UtenteTrasportatore : public Utente {
public:
    // Attributi specifici per UtenteTrasportatore
    std::string session_id;
    std::string ditta_spedizione;
    int stato;
    int disponibilità;


    // Costruttore di UtenteTrasportatore

    UtenteTrasportatore():
        Utente("", "", "", "", "", "", ""),
        session_id(""),
        ditta_spedizione(""),
        stato(0),
        disponibilità(-1) {}

    UtenteTrasportatore(std::string nome_utente, std::string categoria, std::string nome, std::string cognome, 
                        std::string numero_telefono, std::string password, std::string email, std::string session_id,
                        std::string ditta_spedizione, int stato, int disponibilità)
        : Utente(nome_utente, categoria, nome, cognome, numero_telefono, password, email), session_id(session_id), ditta_spedizione(ditta_spedizione), stato(stato), disponibilità(disponibilità) {}

    
    // Metodo specifico per UtenteTrasportatore
    void mostraInformazioniTrasportatore() {
        mostraInformazioni(); // Chiama il metodo della classe base
        std::cout << "Ditta di spedizione: " << ditta_spedizione << std::endl;
        std::cout << "Stato: " << stato << std::endl;
    }


    void effettuaRegistrazione( Con2DB db1,
                                std::string in_nome_utente, std::string in_categoria, std::string in_nome, std::string in_cognome, 
                                std::string sessionID,
                                std::string in_numero_telefono, std::string in_email, 
                                std:: string in_password, std:: string in_conferma_password, 
                                std:: string in_dittaSped
                                ) {

        int stato = 0;
        int disponibilità = 0;
        //std::string session_id = "";

        std::string nomeRequisito = "Registrazione utente trasportatore.";
        statoRequisito statoReq = statoRequisito::Wait;

        std::string messageLog = "";


        // controllo se il sessionID è univoco.
        bool resultSession = check_sessionID(db1, nomeRequisito, statoReq, sessionID);
        if (resultSession==false){
            return;
        }


        // Controllo se la mail contiene il carattere "@".
        bool resultEmail = check_email(db1, in_email, nomeRequisito, statoReq, sessionID);
        if (resultEmail==false){
            return;
        }


       // Controllo se la password rispetta i criteri: lunghezza di almeno 8, almeno una lettere maiuscola, un numero e un carattere speciale.
        bool resultPassw = check_password(db1, in_password, in_conferma_password,  nomeRequisito, statoReq, sessionID);
        if (resultPassw==false){
            return;
        } 

        ///////////////////////////////////// 
        // Controllo se la password è uguale al campo conferma_password
        if (in_password != in_conferma_password){
            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "Le password non corrispondono", session_id, nomeRequisito, statoReq);
            std::cout << "Errore: Le password non corrispondono." << std::endl;
            return;
        }
        ///////////////////////////////////// 


        // Controllo che il nome utente sia univoco con gli altri utenti.
        bool resultUsername = check_nome_utente_univoco(db1, in_nome_utente, nomeRequisito, statoReq, sessionID);
        if (resultUsername==false){
            return;
        }


         // Controllo che l'email sia univoca
        bool resultEmailUn = check_email_univoca(db1, in_email, nomeRequisito, statoReq, sessionID);
        if (resultEmailUn==false){
            return;
        }


        // Inserisco nel database il nuovo utente:
        sprintf(sqlcmd, "INSERT INTO UtenteTrasportatore (nome_utente_trasportatore, session_id_t, categoriaUtente, nome, cognome, indirizzo_mail, numero_di_telefono, password, nome_DittaSpedizione, stato, dispo ) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%d', '%d')",
        in_nome_utente.c_str(), sessionID.c_str(), in_categoria.c_str(), in_nome.c_str(), in_cognome.c_str(), in_email.c_str(), in_numero_telefono.c_str(), in_password.c_str(), in_dittaSped.c_str(), stato, disponibilità);
                    
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);  

        /////////////////////////////////////
        // Riempio il costruttore dell'utente compratore con i campi dati in input al metodo effettua registrazione:
        *this = UtenteTrasportatore(in_nome_utente, in_categoria, in_nome, in_cognome, in_numero_telefono, in_password, in_email, session_id, in_dittaSped, stato, disponibilità);
        /////////////////////////////////////
  

        // Conferma di inserimento nel db
        std::cout << "Utente inserito." << std::endl;

        statoReq = statoRequisito::Success;

        messageLog = "Utente trasportatore " + in_nome_utente + " inserito";

        InsertToLogDB(db1,"INFO", messageLog, sessionID, nomeRequisito, statoReq);

        return;
    }



    bool check_email(Con2DB db1, std::string in_email, std::string nomeRequisito, statoRequisito statoReq, std::string sessionID){
        bool result = true;
        if (in_email.find("@") == std::string::npos)
        {
            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "La mail deve contenere il carattere -@-.", sessionID, nomeRequisito, statoReq);
            std::cout << "Errore: La mail deve contenere il carattere '@'." << std::endl;
            return false;
        }
        return result;
    }



    bool check_password(Con2DB db1, std::string in_password, std::string in_conferma_password, std::string nomeRequisito, statoRequisito statoReq, std::string sessionID){
        
        bool result = true;


        if (in_password.length() < 8)
        {
            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "La password deve contenere almeno 8 caratteri.", session_id, nomeRequisito, statoReq);
            std::cout << "Errore: La passowrd deve contenere almeno 8 caratteri." << std::endl;
            return false;
        }

        if (in_conferma_password.length() < 8)
        {

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "La password deve contenere almeno 8 caratteri.", session_id, nomeRequisito, statoReq);
            std::cout << "Errore: La passowrd deve contenere almeno 8 caratteri." << std::endl;
            return false;
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
            return false;
        }

        if (!hasDigit)
        {
            std::cout << "La nuova password deve contenere almeno un numero." << std::endl;

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "La nuova passowrd deve contenere almeno un numero.", session_id, nomeRequisito, statoReq);
            return false;
        }

        if (!hasSpecialChar)
        {
            std::cout << "La nuova password deve contenere almeno un carattere speciale." << std::endl;

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "La nuova passowrd deve contenere almeno un carattere speciale.", session_id, nomeRequisito, statoReq);
            return false;
        }
    return result;
    }



    bool check_nome_utente_univoco(Con2DB db1, std::string in_nome_utente, std::string nomeRequisito, statoRequisito statoReq, std::string sessionID){

        bool result = true;

        std::string messageLog = "";

        // Controllo se il nome utente è univoco
        sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", in_nome_utente.c_str());
        PGresult *res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        PQclear(res);
        if (rows > 0)
        {
            statoReq = statoRequisito::NotSuccess;

            messageLog = "Il nome utente " + in_nome_utente +  " è già in uso. ";

            InsertToLogDB(db1,"ERROR", messageLog, session_id, nomeRequisito, statoReq);
            std::cout << "Errore: Il nome utente è già in uso." << std::endl;


            // Da correggere secondo me
            sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", in_nome_utente.c_str());

                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);

                if (rows == 1){
                    std::string nome_utente = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_trasportatore"));
                    std::string session_id = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));
                    std::string categoria = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));
                    std::string nome = PQgetvalue(res, 0, PQfnumber(res, "nome"));
                    std::string cognome = PQgetvalue(res, 0, PQfnumber(res, "cognome"));
                    std::string email = PQgetvalue(res, 0, PQfnumber(res, "indirizzo_mail"));
                    std::string numero_telefono = PQgetvalue(res, 0, PQfnumber(res, "numero_di_telefono"));
                    std::string ditta_spedizione = PQgetvalue(res, 0, PQfnumber(res, "nome_DittaSpedizione"));
                    std::string password = PQgetvalue(res, 0, PQfnumber(res, "password"));
                    int stato = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));
                    int disponibilità = atoi(PQgetvalue(res, 0, PQfnumber(res, "dispo")));

                    *this = UtenteTrasportatore(nome_utente, categoria, nome, cognome, numero_telefono, password, email, session_id, ditta_spedizione, stato, disponibilità);
                }
                else{
                    std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                }
                        
                PQclear(res);     

            return false;
        }


        // Controlliamo anche se il nome sia univoco con le due altre tabelle degli utenti: UtenteCompratore e UtenteFornitore:
        sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        PQclear(res);
        if (rows >= 1)
        {

            statoReq = statoRequisito::NotSuccess;

            messageLog = "Il nome utente" + in_nome_utente + " è già in uso da utenti compratori.";

            InsertToLogDB(db1,"ERROR", messageLog, session_id, nomeRequisito, statoReq);
            std::cout << "Errore: Il nome utente è già in uso da utenti compratori." << std::endl;
            return false;
        }

        sprintf(sqlcmd, "SELECT * FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);
        if (rows >= 1)
        {

            statoReq = statoRequisito::NotSuccess;

            messageLog = "Il nome utente" + in_nome_utente + " è già in uso da utenti fornitori.";

            InsertToLogDB(db1,"ERROR", messageLog, session_id,  nomeRequisito, statoReq);
            std::cout << "Errore: Il nome utente è già in uso da utenti fornitori." << std::endl;
            return false;
        }
    return result;
    }



    bool check_email_univoca(Con2DB db1, std::string in_email, std::string nomeRequisito, statoRequisito statoReq, std::string sessionID){
        bool result = true;

        std::string messageLog = "";
        
        // Controllo se l'email è univoca
        sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore WHERE indirizzo_mail = '%s'", in_email.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        
        PQclear(res);
        if (rows > 0)
        {
            statoReq = statoRequisito::NotSuccess;

            messageLog = "Indirizzo mail " + in_email +  " è già in uso. ";


            InsertToLogDB(db1,"ERROR", messageLog, session_id, nomeRequisito, statoReq);
            std::cout << "Errore: L'indirizzo mail è già in uso." << std::endl;
            return false;
        }
        return result;
    }



    bool check_sessionID(Con2DB db1, std::string nomeRequisito, statoRequisito statoReq, std::string sessionID){
        bool result = true;
    
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
            return false;
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
            return false;
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
            return false;
        }
    return result;
    }




    UtenteTrasportatore anima_oggetto (Con2DB db1, std::string categoriaUtenteLogin, std::string input_nome_utente, std::string input_passw){
        // Connession al database:
        //Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        // Creo il costruttore della classe utente trasportatore dopo il login:
        UtenteTrasportatore trasportatore;
        sprintf(sqlcmd, "SELECT * FROM %s WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());

        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        if (rows == 1){
            trasportatore.nome_utente = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_trasportatore"));
            trasportatore.categoria = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));
            trasportatore.nome = PQgetvalue(res, 0, PQfnumber(res, "nome"));
            trasportatore.cognome = PQgetvalue(res, 0, PQfnumber(res, "cognome"));
            trasportatore.email = PQgetvalue(res, 0, PQfnumber(res, "indirizzo_mail"));
            trasportatore.numero_telefono = PQgetvalue(res, 0, PQfnumber(res, "numero_di_telefono"));
            trasportatore.ditta_spedizione = PQgetvalue(res, 0, PQfnumber(res, "nome_DittaSpedizione"));
            trasportatore.password = PQgetvalue(res, 0, PQfnumber(res, "password"));
            trasportatore.stato = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));
            trasportatore.disponibilità = atoi(PQgetvalue(res, 0, PQfnumber(res, "dispo")));
        }
        else{
            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
            return trasportatore;
        }
    return trasportatore;
    }



    void aggiornaNomeDittaSpedizione(Con2DB db1, std::string input_nome_utente, std::string nuovaDittaSpedizione){
        // Utilizza i membri dell'istanza corrente per ottenere il nome utente.
        //std::string nomeUtente = nome_utente;

        std::string nomeRequisito = "Aggiornamento ditta Spedizione.";
        statoRequisito statoReq = statoRequisito::Wait;

        std::string messageLog = "";

        std::string sessionID = "";
        sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", input_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));}

        PQclear(res);  

        if (sessionID == ""){
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non può essere aggiornata la ditta di spedizione.", sessionID, nomeRequisito, statoReq);
            return;
        }

        sprintf(sqlcmd, "UPDATE UtenteTrasportatore set nome_DittaSpedizione='%s' WHERE nome_utente_trasportatore = '%s'",
                                                                            nuovaDittaSpedizione.c_str(), input_nome_utente.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res); 

        statoReq = statoRequisito::Success;

        messageLog = "Aggiornamento ditta spedizione per utente: " + input_nome_utente;

        InsertToLogDB(db1,"INFO", messageLog , sessionID, nomeRequisito, statoReq);

    return;
    }


};


#endif // USER_TRASPORTATORE_H
