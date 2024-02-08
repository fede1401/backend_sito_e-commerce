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
    std::string ditta_spedizione;
    int stato;
    int disponibilità;


    // Costruttore di UtenteTrasportatore

    UtenteTrasportatore():
        Utente("", "", "", "", "", "", ""),
        ditta_spedizione(""),
        stato(0),
        disponibilità(-1) {}

    UtenteTrasportatore(std::string nome_utente, std::string categoria, std::string nome, std::string cognome, 
                        std::string numero_telefono, std::string password, std::string email,
                        std::string ditta_spedizione, int stato, int disponibilità)
        : Utente(nome_utente, categoria, nome, cognome, numero_telefono, password, email), ditta_spedizione(ditta_spedizione), stato(stato), disponibilità(disponibilità) {}

    
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
        int disponibilità = 0;


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
        sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", in_nome_utente.c_str());
        PGresult *res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        PQclear(res);
        if (rows > 0) {
                std::cout << "Errore: Il nome utente è già in uso." << std::endl;
                return;
        }

        // Controlliamo anche se il nome sia univoco con le due altre tabelle degli utenti: UtenteCompratore e UtenteFornitore:
        sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        if (rows >= 1){
            std::cout << "Errore: Il nome utente è già in uso da utenti compratori." << std::endl;
            return;
        }


        sprintf(sqlcmd, "SELECT * FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        if (rows >= 1){
            std::cout << "Errore: Il nome utente è già in uso da utenti fornitori." << std::endl;
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


        /*
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
        */
        
        

        /////////////////////////////////////
        // Riempio il costruttore dell'utente compratore con i campi dati in input al metodo effettua registrazione:
        *this = UtenteTrasportatore(in_nome_utente, in_categoria, in_nome, in_cognome, in_numero_telefono, in_password, in_email, in_dittaSped, stato, disponibilità);
        /////////////////////////////////////


        std::cout << "Categoria utente:" << this->categoria << std::endl;

        /////////////////////////////////////
        // Inserisco nel database il nuovo utente:
        sprintf(sqlcmd, "INSERT INTO UtenteTrasportatore (nome_utente_trasportatore, categoriaUtente, nome, cognome, indirizzo_mail, numero_di_telefono, password, nome_DittaSpedizione, stato, dispo ) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%d', '%d')",
        in_nome_utente.c_str(), in_categoria.c_str(), in_nome.c_str(), in_cognome.c_str(), in_email.c_str(), in_numero_telefono.c_str(), in_password.c_str(), in_dittaSped.c_str(), stato, disponibilità);
                    
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);  
        /////////////////////////////////////      

        // Conferma di inserimento nel db
        std::cout << "Utente inserito." << std::endl;
    }



    UtenteTrasportatore anima_oggetto (std::string categoriaUtenteLogin, std::string input_nome_utente, std::string input_passw){
        // Connession al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

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



    void aggiornaNomeDittaSpedizione(std::string nuovaDittaSpedizione){
        // Utilizza i membri dell'istanza corrente per ottenere il nome utente.
        std::string nomeUtente = nome_utente;

        // Connession al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        sprintf(sqlcmd, "UPDATE UtenteTrasportatore set nome_DittaSpedizione='%s' WHERE nome_utente_trasportatore = '%s'",
                                                                            nuovaDittaSpedizione.c_str(), nomeUtente.c_str());
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res); 

    return;
    }

};


#endif // USER_TRASPORTATORE_H
