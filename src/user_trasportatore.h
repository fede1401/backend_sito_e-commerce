#ifndef USER_TRASPORTATORE_H
#define USER_TRASPORTATORE_H

#include <iostream>
#include <string>
#include <cctype> // Per isupper() e isdigit()
#include <sstream>
#include "user.h"
#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db/pgsql.h"


bool isSpecialCharacter(char c) {
    // Puoi aggiungere altri caratteri speciali se necessario
    return !std::isalnum(c);
}

class UtenteTrasportatore : public Utente {
public:
    // Attributi specifici per UtenteTrasportatore
    std::string ditta_spedizione;
    bool stato;

    // Costruttore di UtenteTrasportatore
    UtenteTrasportatore(std::string nome_utente,std::string categoria, std::string nome, std::string cognome, 
                        std::string numero_telefono, std::string password, std::string email,
                        std::string ditta_spedizione, bool stato)
        : Utente( nome_utente, categoria, nome, cognome, numero_telefono, password, email), ditta_spedizione(ditta_spedizione), stato(stato) {}

    // Metodo specifico per UtenteTrasportatore
    void mostraInformazioniTrasportatore() {
        mostraInformazioni(); // Chiama il metodo della classe base
        std::cout << "Ditta di spedizione: " << ditta_spedizione << std::endl;
        std::cout << "Stato: " << stato << std::endl;
    }


    void effettuaRegistrazione(UtenteTrasportatore utente, std::string conferma_password){
        nome_utente = utente.nome_utente;
        categoria = utente.categoria;
        nome = utente.nome;
        cognome = utente.cognome;
        numero_telefono = utente.numero_telefono;
        email = utente.email;
        password = utente.password;
        ditta_spedizione = utente.ditta_spedizione;
        stato = utente.stato;

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
    }

};


#endif // USER_TRASPORTATORE_H
