#ifndef USER_COMPRATORE_H
#define USER_COMPRATORE_H

#include <iostream>
#include <string>
#include <cctype> // Per isupper() e isdigit()
#include <sstream>
#include "user.h"
#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db/pgsql.h"


//PGresult *res;
//char sqlcmd[1000];

//int rows, k;



class UtenteCompratore : public Utente {
    public:
    // Attributi specifici per UtenteCompratore
    std::string data_compleanno;
    std::string via_residenza;
    std::string numero_civico;
    std::string CAP;
    std::string città_residenza;
    float saldo;
    int stato;

    // Costruttori di UtenteCompratore

    UtenteCompratore():
        Utente("", "", "", "", "", "", ""),
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
                     std::string data_compleanno, 
                     std::string via_residenza, 
                     std::string numero_civico, 
                     std::string CAP, 
                     std::string città_residenza, 
                     float saldo, 
                     int stato): 
                    
                    Utente(nome_utente, categoria, nome, cognome, numero_telefono, password, email), 
                    data_compleanno(data_compleanno), 
                    via_residenza(via_residenza), 
                    numero_civico(numero_civico),
                    CAP(CAP), 
                    città_residenza(città_residenza), 
                    saldo(saldo), 
                    stato(stato) {}


    // Metodo specifico per UtenteCompratore
    void mostraInformazioniCompratore() {
        mostraInformazioni(); // Chiama il metodo della classe base
        std::cout << "Data di compleanno: " << data_compleanno << std::endl;
        std::cout << "Via di residenza: " << via_residenza << std::endl;
        std::cout << "Numero civico: " << numero_civico << std::endl;
        std::cout << "CAP: " << CAP << std::endl;
        std::cout << "Città di residenza: " << città_residenza << std::endl;
        std::cout << "Saldo: " << saldo << std::endl;
        std::cout << "Stato: " << stato << std::endl;
    }

    /*void effettuaRegistrazione(std::string nome_utente, std::string nome, 
                                std::string cognome, std::string numero_telefono, 
                                std::string email, std::string via_residenza, 
                                int numero_civico, std::string CAP, std::string città_residenza,
                                std:: string password, std:: string conferma_password, 
                                std::string data_compleanno  ){
                                    */
    /*                               
    void effettuaRegistrazione(UtenteCompratore utente, std::string conferma_password){

        nome_utente = utente.nome_utente;
        categoria = utente.categoria;
        nome = utente.nome;
        cognome = utente.cognome;
        numero_telefono = utente.numero_telefono;
        email = utente.email;
        data_compleanno = utente.data_compleanno;
                    password = utente.password;
                    via_residenza = utente.via_residenza;
                    numero_civico = utente.numero_civico;
                    CAP = utente.CAP;
                    città_residenza = utente.città_residenza;
                    stato = utente.stato;
                    saldo = utente.saldo;


                    // Controllo se la mail contiene il carattere "@".
                    if (email.find("@") == std::string::npos) {
                        std::cout << "Errore: La mail deve contenere il carattere '@'." << std::endl;
                        return;
                    }
                                    
                    // Controllo se la password rispetta i criteri: lunghezza di almeno 8, almeno una lettere maiuscola, un numero e un carattere speciale.
                    if (password.length() < 8){
                        std::cout << "Errore: La passowrd deve contenere almeno 8 caratteri." << std::endl;
                        return;
                    }

                    if (conferma_password.length() < 8){
                        std::cout << "Errore: La passowrd deve contenere almeno 8 caratteri." << std::endl;
                        return;
                    }
                                
                    bool hasUpperCase = false;
                    bool hasDigit = false;
                    bool hasSpecialChar = false;
                    for (size_t i = 0; i < password.length(); i++){
                        if (std::isupper(password[i])) {  hasUpperCase = true; } 
                                    
                        else if (std::isdigit(password[i])) { hasDigit = true;} 
                        
                        else if (isSpecialCharacter(password[i])) {  hasSpecialChar = true; }
                        }

                    if (!hasUpperCase) { std::cout << "La password deve contenere almeno un carattere maiuscolo." << std::endl;  }
                    if (!hasDigit) { std::cout << "La password deve contenere almeno un numero." << std::endl; }
                    if (!hasSpecialChar) {  std::cout << "La password deve contenere almeno un carattere speciale." << std::endl; }
                                

                    // Controllo se la password è uguale al campo conferma_password
                    if (password != conferma_password){
                        std::cout << "Errore: Le password non corrispondono." << std::endl;
                        return;
                        }

                    Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce");
                    std::cout << "Connessione al database avvenuta con successo." << std::endl;

                                
                    // Controllo se il nome utente è univoco
                    sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE nome_utente = '%s'", nome_utente.c_str());
                    PGresult *res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);

                    PQclear(res);
                    if (rows > 0) {
                        std::cout << "Errore: Il nome utente è già in uso." << std::endl;
                            return;
                    }

                    // Controllo se l'email è univoca
                    sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE indirizzo_mail = '%s'", email.c_str());
                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);

                    PQclear(res);
                    if (rows > 0) {
                        std::cout << "Errore: L'indirizzo mail è già in uso." << std::endl;
                            return;
                    }


                    std::string formatted_date;
                    std::stringstream ss(data_compleanno); // Utilizza un stringstream per manipolare la stringa
                    std::string token;
                    while (std::getline(ss, token, '/')) { // Assumendo che la data sia nel formato "GG/MM/AAAA"
                        formatted_date += token + "-";
                    }
                    formatted_date.pop_back(); // Rimuove il carattere '-' in eccesso alla fine

                    // Trasforma 02/03/2023 in 02-03-2023.

                    UtenteCompratore(categoria, nome_utente, nome, cognome, numero_telefono, password, email, formatted_date, via_residenza, numero_civico, CAP, città_residenza, saldo, stato);

                    std::cout << "Fino a riga 167 tutto ok." << std::endl;

                    std::cout << "Categoria utente:" << utente.categoria << std::endl;

                    sprintf(sqlcmd, "INSERT INTO UtenteCompratore (idUtComp, categoriaUtente, nome_utente, nome, cognome, indirizzo_mail, numero_di_telefono, password, data_compleanno, via_di_residenza, numero_civico, CAP, citta_di_residenza, saldo, stato ) VALUES (DEFAULT, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%d', '%s', '%s', %f, %d)",
           categoria.c_str(), nome_utente.c_str(), nome.c_str(), cognome.c_str(), email.c_str(), numero_telefono.c_str(), password.c_str(), formatted_date.c_str(), via_residenza.c_str(), numero_civico, CAP.c_str(), città_residenza.c_str(), saldo, stato);
                    
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);        

                    std::cout << "Utente inserito." << std::endl;
        }
    */

    void effettuaRegistrazione( std::string in_nome_utente, 
                                std::string in_categoria,
                                std::string in_nome, std::string in_cognome, 
                                std::string in_numero_telefono, 
                                std::string in_email, 
                                std::string in_via_residenza, std::string in_numero_civico, std::string in_CAP, std::string in_città_residenza,
                                std:: string in_password, std:: string in_conferma_password, 
                                std::string in_data_compleanno) {

        int stato = 0;
        float saldo = 0.0;


        ///////////////////////////////////// 
        // Controllo se la mail contiene il carattere "@".
        if (in_email.find("@") == std::string::npos) {
            std::cout << "Errore: La mail deve contenere il carattere '@'." << std::endl;
            return;
            }
        ///////////////////////////////////// 
             

        ///////////////////////////////////// 
        // Controllo se la password rispetta i criteri: lunghezza di almeno 8, almeno una lettere maiuscola, un numero e un carattere speciale.
        if (in_password.length() < 8){
            std::cout << "Errore: La passowrd deve contenere almeno 8 caratteri." << std::endl;
            return;
        }

        if (in_conferma_password.length() < 8){
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

        if (!hasUpperCase) { std::cout << "La password deve contenere almeno un carattere maiuscolo." << std::endl;  }
        if (!hasDigit) { std::cout << "La password deve contenere almeno un numero." << std::endl; }
        if (!hasSpecialChar) {  std::cout << "La password deve contenere almeno un carattere speciale." << std::endl; }
        ///////////////////////////////////// 
                   

        ///////////////////////////////////// 
        // Controllo se la password è uguale al campo conferma_password
        if (in_password != in_conferma_password){
            std::cout << "Errore: Le password non corrispondono." << std::endl;
            return;
        }
        ///////////////////////////////////// 


        ///////////////////////////////////// 
        // Connessione al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");
        std::cout << "Connessione al database avvenuta con successo." << std::endl;
        ///////////////////////////////////// 


        /////////////////////////////////////                         
        // Controllo se il nome utente è univoco
        sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", in_nome_utente.c_str());
        PGresult *res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        PQclear(res);
        if (rows > 0) {
                std::cout << "Errore: Il nome utente è già in uso." << std::endl;
                return;
        }
        

        // Controlliamo anche se il nome sia univoco con le due altre tabelle degli utenti: UtenteFornitore e UtenteTrasportatore:
        sprintf(sqlcmd, "SELECT * FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        if (rows >= 1){
            std::cout << "Errore: Il nome utente è già in uso da utenti fornitori." << std::endl;
            return;
        }


        sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        if (rows >= 1){
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
        if (rows > 0) {
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
        while (std::getline(ss, token, '/')) { // Assumendo che la data sia nel formato "GG/MM/AAAA"
            formatted_date += token + "-";
        }
        formatted_date.pop_back(); // Rimuove il carattere '-' in eccesso alla fine
        ///////////////////////////////////// 
        

        /////////////////////////////////////
        // Riempio il costruttore dell'utente compratore con i campi dati in input al metodo effettua registrazione:
        *this = UtenteCompratore(in_nome_utente, in_categoria, in_nome, in_cognome, in_numero_telefono, in_password, in_email, formatted_date, in_via_residenza, in_numero_civico, in_CAP, in_città_residenza, saldo, stato);
        /////////////////////////////////////

        std::cout << "Categoria utente:" << in_categoria << std::endl;

        /////////////////////////////////////
        // Inserisco nel database il nuovo utente:
        sprintf(sqlcmd, "INSERT INTO UtenteCompratore (nome_utente_compratore, categoriaUtente, nome, cognome, indirizzo_mail, numero_di_telefono, password, data_compleanno, via_di_residenza, numero_civico, CAP, citta_di_residenza, saldo, stato ) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %f, %d)",
        in_nome_utente.c_str(), in_categoria.c_str(), in_nome.c_str(), in_cognome.c_str(), in_email.c_str(), in_numero_telefono.c_str(), in_password.c_str(), formatted_date.c_str(), in_via_residenza.c_str(), in_numero_civico.c_str(), in_CAP.c_str(), in_città_residenza.c_str(), saldo, stato);
                    
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);  
        /////////////////////////////////////      

        // Conferma di inserimento nel db
        std::cout << "Utente inserito." << std::endl;
    }



    UtenteCompratore anima_oggetto(std::string categoriaUtenteLogin, std::string input_nome_utente, std::string input_passw) {
        // Connession al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        // Creo il costruttore della classe utente compratore dopo il login:
        UtenteCompratore compratore;
        sprintf(sqlcmd, "SELECT * FROM %s WHERE nome_utente_compratore = '%s'", categoriaUtenteLogin.c_str(), input_nome_utente.c_str());
        
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
                compratore.numero_civico = PQgetvalue(res, 0, PQfnumber(res, "numero_civico"));
                compratore.CAP = PQgetvalue(res, 0, PQfnumber(res, "CAP"));
                compratore.città_residenza = PQgetvalue(res, 0, PQfnumber(res, "citta_di_residenza"));
                compratore.saldo = atof(PQgetvalue(res, 0, PQfnumber(res, "saldo")));
                compratore.stato = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));
        }
        else{
            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
            return compratore;
        }
    std::cout << "Nessun errore in anima oggetto!" << std::endl;
    return compratore;
    }




    void aggiornaResidenza(std::string nuovaViaResidenza, std::string nuovoNumCiv, std::string nuovoCAP, std::string nuovaCittaResidenza){
        // Utilizza i membri dell'istanza corrente per ottenere il nome utente.
        std::string nomeUtente = nome_utente;

        // Connession al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        sprintf(sqlcmd, "UPDATE UtenteCompratore set via_di_residenza='%s', numero_civico ='%s', CAP='%s', citta_di_residenza='%s' WHERE nome_utente_compratore = '%s'",
         nuovaViaResidenza.c_str(), nuovoNumCiv.c_str(), nuovoCAP.c_str(), nuovaCittaResidenza.c_str(), nomeUtente.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res); 

    return;
    }



};



#endif // USER_COMPRATORE_H
