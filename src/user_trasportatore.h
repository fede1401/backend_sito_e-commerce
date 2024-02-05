#ifndef USER_TRASPORTATORE_H
#define USER_TRASPORTATORE_H

#include <iostream>
#include <string>
#include <cctype> // Per isupper() e isdigit()
#include <sstream>
#include "user.h"
#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db/pgsql.h"


class UtenteTrasportatore : public Utente {
public:
    // Attributi specifici per UtenteTrasportatore
    int ditta_spedizione;
    int stato;


    // Costruttore di UtenteTrasportatore

    UtenteTrasportatore():
        Utente("", "", "", "", "", "", ""),
        ditta_spedizione(-1),
        stato(0) {}

    UtenteTrasportatore(std::string categoria, std::string nome_utente, std::string nome, std::string cognome, 
                        std::string numero_telefono, std::string password, std::string email,
                        int ditta_spedizione, int stato)
        : Utente(categoria, nome_utente, nome, cognome, numero_telefono, password, email), ditta_spedizione(ditta_spedizione), stato(stato) {}

    
    // Metodo specifico per UtenteTrasportatore
    void mostraInformazioniTrasportatore() {
        mostraInformazioni(); // Chiama il metodo della classe base
        std::cout << "Ditta di spedizione: " << ditta_spedizione << std::endl;
        std::cout << "Stato: " << stato << std::endl;
    }


    void effettuaRegistrazione( std::string in_nome_utente, 
                                std::string in_categoria,
                                std::string in_nome, std::string in_cognome, 
                                std::string in_numero_telefono, 
                                std::string in_email, 
                                std:: string in_password, std:: string in_conferma_password, 
                                std:: string in_dittaSped
                                ) {

        int stato = 0;


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
        sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore WHERE nome_utente = '%s'", in_nome_utente.c_str());
        PGresult *res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        PQclear(res);
        if (rows > 0) {
                std::cout << "Errore: Il nome utente è già in uso." << std::endl;
                return;
        }
        ///////////////////////////////////// 


        ///////////////////////////////////// 
        // Controllo se l'email è univoca
        sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore WHERE indirizzo_mail = '%s'", in_email.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        PQclear(res);
        if (rows > 0) {
            std::cout << "Errore: L'indirizzo mail è già in uso." << std::endl;
            return;
        }
        /////////////////////////////////////


        /////////////////////////////////////
        // Seleziono l'id della stringa AziendaProduzione
        int idDittaSpe;
        sprintf(sqlcmd, "SELECT idDittaSp FROM DittaSp WHERE nome = '%s'", in_dittaSped.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows > 0){
            idDittaSpe = atoi(PQgetvalue(res, 0, PQfnumber(res, "idDittaSp")));
        }
        PQclear(res);
        /////////////////////////////////////
        

        /////////////////////////////////////
        // Riempio il costruttore dell'utente compratore con i campi dati in input al metodo effettua registrazione:
        *this = UtenteTrasportatore(in_categoria, in_nome_utente, in_nome, in_cognome, in_numero_telefono, in_password, in_email, idDittaSpe, stato);
        /////////////////////////////////////


        std::cout << "Categoria utente:" << this->categoria << std::endl;

        /////////////////////////////////////
        // Inserisco nel database il nuovo utente:
        sprintf(sqlcmd, "INSERT INTO UtenteTrasportatore (idUtTrasp, categoriaUtente, nome_utente, nome, cognome, indirizzo_mail, numero_di_telefono, password, idDittaSp, stato ) VALUES (DEFAULT, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%d', '%d')",
        in_categoria.c_str(), in_nome_utente.c_str(), in_nome.c_str(), in_cognome.c_str(), in_email.c_str(), in_numero_telefono.c_str(), in_password.c_str(), idDittaSpe, stato);
                    
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);  
        /////////////////////////////////////      

        // Conferma di inserimento nel db
        std::cout << "Utente inserito." << std::endl;
    }

};


#endif // USER_TRASPORTATORE_H
