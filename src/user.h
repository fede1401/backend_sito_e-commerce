#ifndef USER_H
#define USER_H

#include "main.h"

bool isSpecialCharacter(char c)
{
    // Puoi aggiungere altri caratteri speciali se necessario
    return !std::isalnum(c);
}

// PGresult *res;
// char sqlcmd[1000];

// int rows, k;

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


    void effettua_login(std::string input_nome_utente, std::string input_passw)
    {

        // Connession al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        std::string categoriaUtenteLogin = categoria;
        std::string sessionID = "";

        // Controlla se l'utente è già loggato:
        int stato_utente;

        // UTENTE COMPRATORE
        if (categoriaUtenteLogin == "UtenteCompratore"){
            sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
        }

        // UTENTE FORNITORE
        if (categoriaUtenteLogin == "UtenteFornitore"){
            sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
        }

        // UTENTE TRASPORTATORE
        if (categoriaUtenteLogin == "UtenteTrasportatore"){
            sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
        }


        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Riga contenente lo stato dell'utente con nome utente : std::string input_nome_utente
        if (rows == 1)
        {

            // Importante che come parametri PQgetvalues al secondo campo indica il numero di riga, dato che c'è solo uno stato di un'utente, si troverà all'indice 0 della riga
            stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

            if (stato_utente == 1)
            {
                std::cout << "L'utente è già connesso." << std::endl;
                
                // Carico il session ID:
                
                if (categoria == "UtenteCompratore"){ 
                    sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str());
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));} 
                }

                if (categoria == "UtenteFornitore"){ 
                    sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", input_nome_utente.c_str());
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));} 
                }

                
                if (categoria == "UtenteTrasportatore"){ if (rows==1){ 
                    sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", input_nome_utente.c_str());
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));}  
                    
                }
                
                InsertToLogDB("WARNING", "Utente già connesso.", sessionID );
                return;
            }
            else

            {
                // Aggiorniamo lo stato dell'utente: da disconnesso a connesso:

                // SESSION ID
                // Generiamo il session id:
                std::string sessionID = generateSessionID();

                // Controllo se il sessionID sia univoco con i session ID di tutte le tipologie d'utente:
                sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE session_id_c = '%s'", sessionID.c_str());
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);

                if (rows > 0){
                    InsertToLogDB("ERROR", "Il session ID è già in uso da utenti compratori.", sessionID);
                    std::cout << "Errore: Il session ID è già in uso da utenti compratori." << std::endl;
                    return;
                }

                sprintf(sqlcmd, "SELECT * FROM UtenteFornitore WHERE session_id_f = '%s'", sessionID.c_str());
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);

                if (rows > 0){
                    InsertToLogDB("ERROR", "Il session ID è già in uso da utenti fornitori.", sessionID);
                    std::cout << "Errore: Il session ID è già in uso da utenti fornitori." << std::endl;
                    return;
                }

                sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore WHERE session_id_t = '%s'", sessionID.c_str());
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);

                if (rows > 0){
                    InsertToLogDB("ERROR", "Il session ID è già in uso da utenti trasportatori.", sessionID);
                    std::cout << "Errore: Il session ID è già in uso da utenti trasportatori." << std::endl;
                    return;
                }

                // Se è univoco aggiorniamo il sessionID nella tabella corrette:
                if (categoriaUtenteLogin == "UtenteCompratore"){                    
                    sprintf(sqlcmd, "UPDATE %s set session_id_c='%s' WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), sessionID.c_str(), input_nome_utente.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    InsertToLogDB("INFO", "Aggiornamento sessionID", sessionID);
                }

                if (categoriaUtenteLogin == "UtenteFornitore"){
                    sprintf(sqlcmd, "UPDATE %s set session_id_f='%s' WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), sessionID.c_str(), input_nome_utente.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    InsertToLogDB("INFO", "Aggiornamento sessionID", sessionID);
                }

                if (categoriaUtenteLogin == "UtenteTrasportatore"){
                    sprintf(sqlcmd, "UPDATE %s set session_id_t='%s' WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), sessionID.c_str(), input_nome_utente.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    InsertToLogDB("INFO", "Aggiornamento sessionID", sessionID);
                }

                



                // Verifica della password:
                std::string password_utente;
                char *password_u;

                // UTENTE COMPRATORE
                if (categoriaUtenteLogin == "UtenteCompratore"){
                    // sprintf(sqlcmd, "SELECT password FROM UtenteCompratore WHERE nome_utente = '%s'", input_nome_utente.c_str());   // SELECT password FROM UtenteCompratore WHERE nome_utente = 'fede14';
                    sprintf(sqlcmd, "SELECT password FROM %s WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str()); 
                }

                if (categoriaUtenteLogin == "UtenteFornitore"){
                    // sprintf(sqlcmd, "SELECT password FROM UtenteCompratore WHERE nome_utente = '%s'", input_nome_utente.c_str());   // SELECT password FROM UtenteCompratore WHERE nome_utente = 'fede14';
                    sprintf(sqlcmd, "SELECT password FROM %s WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str()); 
                }

                if (categoriaUtenteLogin == "UtenteTrasportatore"){
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
                    InsertToLogDB("ERROR", "Utente non trovato.", sessionID);
                    std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                    return;
                }

                // Ricorda di liberare la memoria del risultato della query
                PQclear(res);

                // Controllo della password data in input e quella segnata nel database:
                if (input_passw != password_utente)
                {
                    std::cout << "Errore: La passowrd non è corretta, riprovare." << std::endl;
                    InsertToLogDB("ERROR", "Password non corretta", sessionID);
                    return;
                }
                else
                {
                    std::cout << "La passowrd inserita: " << password_utente << "è corretta." << std::endl;

                    // Aggiorno stato:

                    if (categoriaUtenteLogin == "UtenteCompratore"){
                        // sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 1 WHERE nome_utente = '%s'", input_nome_utente.c_str());
                        sprintf(sqlcmd, "UPDATE %s set stato = 1 WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                        res = db1.ExecSQLcmd(sqlcmd);

                        PQclear(res);

                        InsertToLogDB("INFO", "Aggiornamento dello stato utente", sessionID);
                    }

                    if (categoriaUtenteLogin == "UtenteFornitore"){
                        // sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 1 WHERE nome_utente = '%s'", input_nome_utente.c_str());
                        sprintf(sqlcmd, "UPDATE %s set stato = 1 WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                        res = db1.ExecSQLcmd(sqlcmd);

                        PQclear(res);

                        InsertToLogDB("INFO", "Aggiornamento dello stato utente", sessionID);
                    }

                    if (categoriaUtenteLogin == "UtenteTrasportatore"){
                        // sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 1 WHERE nome_utente = '%s'", input_nome_utente.c_str());
                        sprintf(sqlcmd, "UPDATE %s set stato = 1 WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                        res = db1.ExecSQLcmd(sqlcmd);

                        PQclear(res);

                        InsertToLogDB("INFO", "Aggiornamento dello stato utente", sessionID);
                    }
                    
                    /*
                    std::cout << "Lo stato dell'utente "  << input_nome_utente << " prima del login è: " << stato_utente << std::endl;

                    // Controlla se lo stto dell'utente è stato aggiornato:

                    // UTENTE COMPRATORE
                    if (categoriaUtenteLogin == "UtenteCompratore"){
                        sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                    }


                    // UTENTE FORNITORE
                    if (categoriaUtenteLogin == "UtenteFornitore"){
                        sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                    }


                    // UTENTE TRASPORTATORE
                    if (categoriaUtenteLogin == "UtenteTrasportatore"){
                        sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                    }


                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);

                    if (rows == 1)
                    {
                        stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));
                        std::cout << "Lo stato dell'utente " << input_nome_utente <<  " dopo il login è: " << stato_utente << std::endl;
                        std::cout << "\n\n" << std::endl;
                    }

                    else
                    {
                        std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                        return;
                    }
                    */
                    

                        /*
                        // Creo il costruttore della classe utente compratore dopo il login:
                        UtenteCompratore compratore;
                        sprintf(sqlcmd, "SELECT * FROM %s WHERE nome_utente_compratore = '%s'", categoria.c_str(), input_nome_utente.c_str());

                        res = db1.ExecSQLtuples(sqlcmd);
                        rows = PQntuples(res);

                        if (rows == 1){
                            compratore.nome_utente = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_compratore"));
                            compratore.categoria = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));
                            compratore.nome = PQgetvalue(res, 0, PQfnumber(res, "nome"));
                            compratore.cognome = PQgetvalue(res, 0, PQfnumber(res, "cognome"));
                            compratore.email = PQgetvalue(res, 0, PQfnumber(res, "indirizzo_mail"));
                            compratore.numero_telefono = PQgetvalue(res, 0, PQfnumber(res, "numero_di_telefono"));
                            compratore.password = PQgetvalue(res, 0, PQfnumber(res, "password"));
                            compratore.data_compleanno = PQgetvalue(res, 0, PQfnumber(res, "data_compleanno"));
                            compratore.via_residenza = PQgetvalue(res, 0, PQfnumber(res, "via_di_residenza"));
                            compratore.numero_civico = atoi(PQgetvalue(res, 0, PQfnumber(res, "numero_civico")));
                            compratore.CAP = PQgetvalue(res, 0, PQfnumber(res, "CAP"));
                            compratore.città_residenza = PQgetvalue(res, 0, PQfnumber(res, "citta_di_residenza"));
                            compratore.saldo = atof(PQgetvalue(res, 0, PQfnumber(res, "saldo")));
                            compratore.stato = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));
                        }
                        else{
                            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                            return;
                        }
                        */
                        
                    }
                }
            }
            else
            {
                InsertToLogDB("ERROR", "Utente non trovato", sessionID);
                std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                return;
            }

    return;
    }


    std::string generateSessionID() {
    // Caratteri validi per il Session ID
    const std::string valid_characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Inizializzazione del generatore di numeri casuali
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, valid_characters.size() - 1);

    // Generazione del Session ID casuale
    std::string session_id;
    for (int i = 0; i < 10; ++i) {
        session_id += valid_characters[dis(gen)];
    }

    return session_id;
    }











    void effettua_logout(std::string input_nome_utente)
    {

        // Connessione al database
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");


        // Controlla se l'utente è già loggato:
        int stato_utente;
        std::string categoriaUtenteLogin = categoria;
        std::string sessionID = "";


        if (categoriaUtenteLogin == "UtenteCompratore"){
            sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
        }

        if (categoriaUtenteLogin == "UtenteFornitore"){
            sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
        }

        if (categoriaUtenteLogin == "UtenteTrasportatore"){
            sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
        }


        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);


        if (rows == 1)
        {
                // std::cout << "Fino a riga 58 tutto ok." << std::endl;

            // Importante che come parametri PQgetvalues al secondo campo indica il numero di riga, dato che c'è solo uno stato di un'utente, si troverà all'indice 0 della riga
            stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

            if (stato_utente == 0){
                InsertToLogDB("ERROR", "Utente è già disconnesso", sessionID);
                std::cout << "Errore: L'utente è già disconnesso" << std::endl;
                return;
            }
                
            else{
                // Aggiorniamo lo stato dell'utente, da connesso a disconnesso:

                if (categoria == "UtenteCompratore"){ 
                    sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str());
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));} 
                }

                if (categoria == "UtenteFornitore"){ 
                    sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", input_nome_utente.c_str());
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));} 
                }

                
                if (categoria == "UtenteTrasportatore"){ if (rows==1){ 
                    sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", input_nome_utente.c_str());
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);
                    sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));}  
                    
                }


                if (categoriaUtenteLogin == "UtenteCompratore"){
                    // sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 0 WHERE nome_utente = '%s'", input_nome_utente.c_str());
                    sprintf(sqlcmd, "UPDATE %s set stato = 0 WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    InsertToLogDB("INFO", "Disconnessione utente", sessionID);
                }

                if (categoriaUtenteLogin == "UtenteFornitore"){
                    // sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 0 WHERE nome_utente = '%s'", input_nome_utente.c_str());
                    sprintf(sqlcmd, "UPDATE %s set stato = 0 WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    InsertToLogDB("INFO", "Disconnessione utente", sessionID);
                }

                if (categoriaUtenteLogin == "UtenteTrasportatore"){
                    // sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 0 WHERE nome_utente = '%s'", input_nome_utente.c_str());
                    sprintf(sqlcmd, "UPDATE %s set stato = 0 WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    InsertToLogDB("INFO", "Disconnessione utente", sessionID);
                }  

                


                // A questo punto possiamo resettare il session id associato all'utente:
                if (categoriaUtenteLogin == "UtenteCompratore"){
                    sprintf(sqlcmd, "UPDATE %s set session_id_c='' WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    InsertToLogDB("INFO", "Aggiornamento sessionID", "");
                }

                if (categoriaUtenteLogin == "UtenteFornitore"){
                    sprintf(sqlcmd, "UPDATE %s set session_id_f='' WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    InsertToLogDB("INFO", "Aggiornamento sessionID", "");
                }

                if (categoriaUtenteLogin == "UtenteTrasportatore"){
                    sprintf(sqlcmd, "UPDATE %s set session_id_t='' WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);

                    InsertToLogDB("INFO", "Aggiornamento sessionID", "");
                }

                


                // Controlla se lo stato dell'utente è stato aggiornato:

                if (categoriaUtenteLogin == "UtenteCompratore"){
                    sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                }

                if (categoriaUtenteLogin == "UtenteFornitore"){
                    sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_fornitore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                }

                if (categoriaUtenteLogin == "UtenteTrasportatore"){
                    sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_trasportatore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
                }


                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);

                if (rows == 1) {
                    stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));
                    std::cout << "Lo stato dell'utente " << input_nome_utente << " dopo il logout è: " << stato_utente << std::endl;
                    std::cout << "\n\n" << std::endl;
                }
                else {
                    std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                    return;
                }

            }

        }
        else
        {
            InsertToLogDB("ERROR", "Utente non trovato.", sessionID);
            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
            return;
        } 
    return;           
    }














    void elimina_profilo(){
        // Per seguire i paradigmi di OOP:

        // Utilizza i membri dell'istanza corrente per ottenere il nome utente e la categoria dell'utente
        std::string nomeUtenteDaEliminare = nome_utente;

        std::string categoriaUtenteDaEliminare = categoria;

        // Eliminiamo in cascata l'utente dal database:
        // Connession al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        std::string sessionID = "";
        if (categoria == "UtenteCompratore"){ 
            sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));} 
        }

        if (categoria == "UtenteFornitore"){ 
            sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));} 
        }

                
        if (categoria == "UtenteTrasportatore"){ if (rows==1){ 
            sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));}          
        }

        // In base alla categoria dell'utente eliminiamo l'utente
        if (categoria == "UtenteCompratore"){
            sprintf(sqlcmd, "DELETE FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            InsertToLogDB("INFO", "Eliminazione profilo.", sessionID);
        }
        if (categoria == "UtenteFornitore"){
            sprintf(sqlcmd, "DELETE FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            InsertToLogDB("INFO", "Eliminazione profilo.", sessionID);
        }
        if (categoria == "UtenteTrasportatore"){
            sprintf(sqlcmd, "DELETE FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", nomeUtenteDaEliminare.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            InsertToLogDB("INFO", "Eliminazione profilo.", sessionID);
        }

    return;
    }






    void aggiornaNumeroDiTelefono(std::string nuovoNumeroTelefono){
        
        // Utilizza i membri dell'istanza corrente per ottenere il nome utente e la categoria dell'utente
        std::string nomeUtente = nome_utente;

        //std::string categoriaUtente = categoria;

         // Connession al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        std::string sessionID = "";
        if (categoria == "UtenteCompratore"){ 
            sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtente.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));} 
        }

        if (categoria == "UtenteFornitore"){ 
            sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", nomeUtente.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));} 
        }

                
        if (categoria == "UtenteTrasportatore"){ if (rows==1){ 
            sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", nomeUtente.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));}          
        }



        // In base alla categoria dell'utente aggiorniamo il numero di telefono
        if (categoria == "UtenteCompratore"){
            sprintf(sqlcmd, "UPDATE UtenteCompratore set numero_di_telefono = '%s' WHERE nome_utente_compratore = '%s'", nuovoNumeroTelefono.c_str(), nomeUtente.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res); 

            InsertToLogDB("INFO", "Aggiornamento numero di telefono.", sessionID);

        }
        if (categoria == "UtenteFornitore"){
            sprintf(sqlcmd, "UPDATE UtenteFornitore set numero_di_telefono = '%s' WHERE nome_utente_fornitore = '%s'", nuovoNumeroTelefono.c_str(), nomeUtente.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res); 

            InsertToLogDB("INFO", "Aggiornamento numero di telefono.", sessionID);

        }
        if (categoria == "UtenteTrasportatore"){
            sprintf(sqlcmd, "UPDATE UtenteTrasportatore set numero_di_telefono = '%s' WHERE nome_utente_trasportatore = '%s'", nuovoNumeroTelefono.c_str(), nomeUtente.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res); 

            InsertToLogDB("INFO", "Aggiornamento numero di telefono.", sessionID);

        }
    return;
    }



    void aggiornaPassword(std::string vecchiaPassw, std::string nuovaPassw){
        // Utilizza i membri dell'istanza corrente per ottenere il nome utente e la categoria dell'utente
        std::string nomeUtente = nome_utente;
        std::string passwUtente = password;
        //std::string categoriaUtente = categoria;

        // Connession al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        std::string sessionID = "";
        if (categoria == "UtenteCompratore"){ 
            sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtente.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));} 
        }

        if (categoria == "UtenteFornitore"){ 
            sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", nomeUtente.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));} 
        }

                
        if (categoria == "UtenteTrasportatore"){ if (rows==1){ 
            sprintf(sqlcmd, "SELECT session_id_t FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", nomeUtente.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_t"));}          
        }


        // Controlliamo innanzitutto se la vecchiaPassword inserita dall'utente è uguale a quella nel database:
        // Verifica della password:
        if (passwUtente != vecchiaPassw){
            InsertToLogDB("ERROR", "La password attuale inserita non è corretta.", sessionID);
            std::cout << "Errore: La password attuale inserita non è corretta" << std::endl;
            return;
        }
        else{
            // La password inserita dall'utente è uguale a quella nel database:
            // Controllo se la nuova password rispetta i criteri: lunghezza di almeno 8, almeno una lettere maiuscola, un numero e un carattere speciale.
            if (nuovaPassw.length() < 8){
                InsertToLogDB("ERROR", "La nuova passowrd deve contenere almeno 8 caratteri.", sessionID);
                std::cout << "Errore: La nuova passowrd deve contenere almeno 8 caratteri." << std::endl;
                return;
            }
                                    
            bool hasUpperCase = false;
            bool hasDigit = false;
            bool hasSpecialChar = false;
            for (size_t i = 0; i < nuovaPassw.length(); i++)
            {
                if (std::isupper(nuovaPassw[i])) {  hasUpperCase = true; } 
                                        
                else if (std::isdigit(nuovaPassw[i])) { hasDigit = true;} 
                            
                else if (isSpecialCharacter(nuovaPassw[i])) {  hasSpecialChar = true; }
                
            }

            if (!hasUpperCase) { 
                std::cout << "La nuova password deve contenere almeno un carattere maiuscolo." << std::endl;  
                InsertToLogDB("ERROR", "La nuova passowrd deve contenere almeno un carattere maiuscolo.", sessionID);
            }

            if (!hasDigit) { 
                std::cout << "La nuova password deve contenere almeno un numero." << std::endl; 
                InsertToLogDB("ERROR", "La nuova passowrd deve contenere almeno un numero.", sessionID);
            }

            if (!hasSpecialChar) {  
                std::cout << "La nuova password deve contenere almeno un carattere speciale." << std::endl; 
                InsertToLogDB("ERROR", "La nuova passowrd deve contenere almeno un carattere speciale.", sessionID);
            }


            // In base alla categoria dell'utente aggiorniamo la password
            if (categoria == "UtenteCompratore"){
                sprintf(sqlcmd, "UPDATE UtenteCompratore set password = '%s' WHERE nome_utente_compratore = '%s'", nuovaPassw.c_str(), nomeUtente.c_str());
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res); 

                InsertToLogDB("INFO", "Aggiornamento password.", sessionID);

            }
            if (categoria == "UtenteFornitore"){
                sprintf(sqlcmd, "UPDATE UtenteFornitore set password = '%s' WHERE nome_utente_fornitore = '%s'", nuovaPassw.c_str(), nomeUtente.c_str());
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res); 

                InsertToLogDB("INFO", "Aggiornamento password.", sessionID);

            }
            if (categoria == "UtenteTrasportatore"){
                sprintf(sqlcmd, "UPDATE UtenteTrasportatore set password = '%s' WHERE nome_utente_trasportatore = '%s'", nuovaPassw.c_str(), nomeUtente.c_str());
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res); 

                InsertToLogDB("INFO", "Aggiornamento password.", sessionID);
            }
        }
        
    return;
    }
    
    
    
    

};

#endif // USER_H