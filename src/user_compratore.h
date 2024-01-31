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


bool isSpecialCharacter(char c) {
    // Puoi aggiungere altri caratteri speciali se necessario
    return !std::isalnum(c);
}


class UtenteCompratore : public Utente {
    public:
    // Attributi specifici per UtenteCompratore
    std::string data_compleanno;
    std::string via_residenza;
    int numero_civico;
    std::string CAP;
    std::string città_residenza;
    float saldo;
    int stato;

    // Costruttore di UtenteCompratore

    UtenteCompratore(std::string nome_utente, 
                     std::string nome, 
                     std::string cognome, 
                     std::string numero_telefono, 
                     std::string password, 
                     std::string email,
                     std::string data_compleanno, 
                     std::string via_residenza, 
                     int numero_civico, std::string CAP, 
                     std::string città_residenza, 
                     float saldo, 
                     int stato): 
                    
                    Utente(nome_utente, nome, cognome, numero_telefono, password, email), 
                    data_compleanno(data_compleanno), via_residenza(via_residenza), numero_civico(numero_civico),
                    CAP(CAP), città_residenza(città_residenza), saldo(saldo), stato(stato) {}

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
        void effettuaRegistrazione(UtenteCompratore utente, std::string conferma_password){

                    nome_utente = utente.nome_utente;
                    nome = utente.nome;
                    cognome = utente.cognome;
                    numero_telefono = utente.numero_telefono;
                    email = utente.email;
                    data_compleanno = utente.data_compleanno;
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

                    UtenteCompratore(nome_utente, nome, cognome, numero_telefono, password, email, formatted_date, via_residenza, numero_civico, CAP, città_residenza, saldo, stato);

                    std::cout << "Fino a riga 167 tutto ok." << std::endl;

                    sprintf(sqlcmd, "INSERT INTO UtenteCompratore (idUtComp, nome_utente, nome, cognome, indirizzo_mail, numero_di_telefono, password, data_compleanno, via_di_residenza, numero_civico, CAP, citta_di_residenza, saldo, stato ) VALUES (DEFAULT, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%d', '%s', '%s', %f, %d)",
           nome_utente.c_str(), nome.c_str(), cognome.c_str(), email.c_str(), numero_telefono.c_str(), password.c_str(), formatted_date.c_str(), via_residenza.c_str(), numero_civico, CAP.c_str(), città_residenza.c_str(), saldo, stato);
                    
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res);        

                    std::cout << "Utente inserito." << std::endl;
        }




};



#endif // USER_COMPRATORE_H
