#ifndef USER_FORNITORE_H
#define USER_FORNITORE_H

#include "../../shared-server/user.h"


class UtenteFornitore : public Utente {
public:
    // Attributi specifici per UtenteFornitore
    std::string session_id;
    std::string azienda_produzione;
    int stato;

    // Costruttore di UtenteFornitore

    UtenteFornitore():
        Utente("", "", "", "", "", "", ""),
        session_id(""),
        azienda_produzione(""),
        stato(0) {}

    UtenteFornitore( 
                    std::string nome_utente, 
                    std::string categoria,
                    std::string nome, 
                    std::string cognome,
                    std::string numero_telefono, 
                    std::string password, 
                    std::string email,
                    std::string session_id,
                    std::string azienda_produzione, 
                    int stato)

        : Utente(nome_utente, categoria, nome, cognome, numero_telefono, password, email), 
        
        session_id(session_id), azienda_produzione(azienda_produzione), stato(stato) {}


    // Metodo specifico per UtenteFornitore
    void mostraInformazioniFornitore() {
        mostraInformazioni(); // Chiama il metodo della classe base
        std::cout << "Azienda di produzione: " << azienda_produzione << std::endl;
        std::cout << "Stato: " << stato << std::endl;
    }


    void effettuaRegistrazione( Con2DB db1,
                                std::string in_nome_utente, 
                                std::string in_categoria,
                                std::string in_nome, std::string in_cognome, 
                                std::string sessionID,
                                std::string in_numero_telefono, 
                                std::string in_email, 
                                std:: string in_password, std:: string in_conferma_password, 
                                std:: string in_aziendaProd
                                ) {

        int stato = 0;
        //std::string session_id = "";

        std::string nomeRequisito = "Registrazione utente fornitore.";
        statoRequisito statoReq = statoRequisito::Wait;

        std::string messageLog = "";


        ///////////////////////////////////// 
        // Controllo se la mail contiene il carattere "@".
        if (in_email.find("@") == std::string::npos) {
            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "La mail deve contenere il carattere -@-.", session_id, nomeRequisito, statoReq);
            std::cout << "Errore: La mail deve contenere il carattere '@'." << std::endl;
            return;
            }
        ///////////////////////////////////// 
             

        ///////////////////////////////////// 
        // Controllo se la password rispetta i criteri: lunghezza di almeno 8, almeno una lettere maiuscola, un numero e un carattere speciale.
        if (in_password.length() < 8){
            
            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "La password deve contenere almeno 8 caratteri.", session_id, nomeRequisito, statoReq);

            std::cout << "Errore: La passowrd deve contenere almeno 8 caratteri." << std::endl;
            return;
        }

        if (in_conferma_password.length() < 8){
            
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
            if (std::isupper(in_password[i])) {  hasUpperCase = true; } 
                                    
            else if (std::isdigit(in_password[i])) { hasDigit = true;} 
                        
            else if (isSpecialCharacter(in_password[i])) {  hasSpecialChar = true; }
            
        }

        if (!hasUpperCase) { 
            std::cout << "La nuova password deve contenere almeno un carattere maiuscolo." << std::endl;  
            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "La nuova passowrd deve contenere almeno un carattere maiuscolo.", session_id, nomeRequisito, statoReq);
            return;
        }

        if (!hasDigit) { 
            std::cout << "La nuova password deve contenere almeno un numero." << std::endl; 
            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "La nuova passowrd deve contenere almeno un numero.", session_id, nomeRequisito, statoReq);
            return;
        }

        if (!hasSpecialChar) {  
            std::cout << "La nuova password deve contenere almeno un carattere speciale." << std::endl; 
            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "La nuova passowrd deve contenere almeno un carattere speciale.", session_id, nomeRequisito, statoReq);
            return;
        }
        ///////////////////////////////////// 
                   

        ///////////////////////////////////// 
        // Controllo se la password è uguale al campo conferma_password
        if (in_password != in_conferma_password){
            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1,"ERROR", "Le password non corrispondono", session_id, nomeRequisito, statoReq);
            std::cout << "Errore: Le password non corrispondono." << std::endl;
            return;
        }
        ///////////////////////////////////// 



        /////////////////////////////////////                         
        // Controllo se il nome utente è univoco
        sprintf(sqlcmd, "SELECT * FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", in_nome_utente.c_str());
        PGresult *res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        PQclear(res);
        if (rows > 0) {
                statoReq = statoRequisito::NotSuccess;

                messageLog = "Il nome utente" + in_nome_utente  + "è già in uso.";

                InsertToLogDB(db1,"ERROR", messageLog , session_id, nomeRequisito, statoReq);
                std::cout << "Errore: Il nome utente è già in uso." << std::endl;
                
                sprintf(sqlcmd, "SELECT * FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", in_nome_utente.c_str());

                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);

                if (rows == 1){
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

                    *this = UtenteFornitore(nome_utente, categoria, nome, cognome, numero_telefono, password, email, session_id, azienda_produzione, stato);
                }
                else{
                    std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                }
                        
                PQclear(res);        
                
                
                return;
        }

         // Controlliamo anche se il nome sia univoco con le due altre tabelle degli utenti: UtenteCompratore e UtenteTrasportatore:
        sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        if (rows >= 1){

            statoReq = statoRequisito::NotSuccess;

            messageLog = "Il nome utente" + in_nome_utente  + " è già in uso da utenti compratori.";


            InsertToLogDB(db1,"ERROR", messageLog, session_id, nomeRequisito, statoReq);
            std::cout << "Errore: Il nome utente è già in uso da utenti compratori." << std::endl;
            return;
        }


        sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        if (rows >= 1){

            statoReq = statoRequisito::NotSuccess;

            messageLog = "Il nome utente" + in_nome_utente  + " è già in uso da utenti trasportatori.";


            InsertToLogDB(db1,"ERROR", messageLog, session_id, nomeRequisito, statoReq);
            std::cout << "Errore: Il nome utente è già in uso da utenti trasportatori." << std::endl;
            return;
        }
        /////////////////////////////////////  


        ///////////////////////////////////// 
        // Controllo se l'email è univoca
        sprintf(sqlcmd, "SELECT * FROM UtenteFornitore WHERE indirizzo_mail = '%s'", in_email.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        PQclear(res);
        if (rows > 0) {
            statoReq = statoRequisito::NotSuccess;

            messageLog = "Indirizzo mail " + in_email  + "è già in uso.";


            InsertToLogDB(db1,"ERROR", messageLog, session_id, nomeRequisito, statoReq);
            std::cout << "Errore: L'indirizzo mail è già in uso." << std::endl;
            return;
        }
        /////////////////////////////////////


        /////////////////////////////////////
        // Seleziono l'id della stringa AziendaProduzione
        /*int idAziendaProd;
        sprintf(sqlcmd, "SELECT idAziendaProd FROM AziendaProd WHERE nome = '%s'", in_aziendaProd.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows > 0){
            idAziendaProd = atoi(PQgetvalue(res, 0, PQfnumber(res, "idAziendaProd")));
        }
        PQclear(res);
        */
        /////////////////////////////////////
        


        std::cout << "Categoria utente:" << this->categoria << std::endl;


        // SESSION ID
        // Generiamo il session id:
        //std::string sessionID = generateSessionID();

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
        sprintf(sqlcmd, "INSERT INTO UtenteFornitore (nome_utente_fornitore, session_id_f, categoriaUtente, nome, cognome, indirizzo_mail, numero_di_telefono, password, nome_AziendaProduttrice, stato ) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%d')",
        in_nome_utente.c_str(),sessionID.c_str(), in_categoria.c_str(), in_nome.c_str(), in_cognome.c_str(), in_email.c_str(), in_numero_telefono.c_str(), in_password.c_str(), in_aziendaProd.c_str(), stato);
                    
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);  
        /////////////////////////////////////     

        /////////////////////////////////////
        // Riempio il costruttore dell'utente compratore con i campi dati in input al metodo effettua registrazione:
        *this = UtenteFornitore(in_nome_utente, in_categoria, in_nome, in_cognome, in_numero_telefono, in_password, in_email, session_id, in_aziendaProd, stato);
        ///////////////////////////////////// 

        // Conferma di inserimento nel db
        std::cout << "Utente inserito." << std::endl;

        statoReq = statoRequisito::Success;

        messageLog = "Utente fornitore " + in_nome_utente + " inserito";

        InsertToLogDB(db1,"INFO", messageLog, sessionID,  nomeRequisito, statoReq);

        printf("Registrazione avvenuta con successo.\n");

        return;
    }


    UtenteFornitore anima_oggetto(Con2DB db1, std::string categoriaUtenteLogin,std::string input_nome_utente, std::string input_passw){
        // Connession al database:
        //Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        UtenteFornitore fornitore;
        sprintf(sqlcmd, "SELECT * FROM %s WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());

        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        if (rows == 1){
            std::cout << "Utente trovato!!!!" << std::endl;
            fornitore.nome_utente = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_fornitore"));
            fornitore.categoria = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));
            fornitore.nome = PQgetvalue(res, 0, PQfnumber(res, "nome"));
            fornitore.cognome = PQgetvalue(res, 0, PQfnumber(res, "cognome"));
            fornitore.email = PQgetvalue(res, 0, PQfnumber(res, "indirizzo_mail"));
            fornitore.numero_telefono = PQgetvalue(res, 0, PQfnumber(res, "numero_di_telefono"));
            fornitore.azienda_produzione = PQgetvalue(res, 0, PQfnumber(res, "nome_AziendaProduttrice"));
            fornitore.password = PQgetvalue(res, 0, PQfnumber(res, "password"));
            fornitore.stato = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));
            std::cout << "Fornitore riempito!" << std::endl;
        }
        else{
            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
            return fornitore;
        }
    return fornitore;
    }



    void aggiornaNomeAziendaProduttrice(Con2DB db1, std::string input_nome_utente, std::string nuovaAziendaProduttrice){
        // Utilizza i membri dell'istanza corrente per ottenere il nome utente.
        //std::string nomeUtente = nome_utente;

        std::string nomeRequisito = "Aggiornamento azienda Produttrice.";
        statoRequisito statoReq = statoRequisito::Wait;

        std::string messageLog= "";

        std::string sessionID = "";
        sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", input_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));}  
        PQclear(res);

        if (sessionID == ""){
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non può essere aggiornato il nome dell azienda produttrice.", sessionID, nomeRequisito, statoReq);
            return;
        }

        sprintf(sqlcmd, "UPDATE UtenteFornitore set nome_AziendaProduttrice='%s' WHERE nome_utente_fornitore = '%s'",
                                                                            nuovaAziendaProduttrice.c_str(), input_nome_utente.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res); 

        statoReq = statoRequisito::Success;

        messageLog = "Aggiornamento azienda produttrice per utente: " + input_nome_utente;

        InsertToLogDB(db1,"INFO", messageLog, sessionID, nomeRequisito, statoReq);

    return;
    }
};




#endif // USER_FORNITORE_H
