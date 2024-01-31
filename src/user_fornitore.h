#ifndef USER_FORNITORE_H
#define USER_FORNITORE_H

#include <iostream>
#include <string>
#include <cctype> // Per isupper() e isdigit()
#include <sstream>
#include "user.h"
#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db/pgsql.h"



class UtenteFornitore : public Utente {
public:
    // Attributi specifici per UtenteFornitore
    std::string azienda_produzione;
    bool stato;

    // Costruttore di UtenteFornitore
    UtenteFornitore(std::string nome_utente, 
                    std::string nome, 
                    std::string cognome,
                    std::string numero_telefono, 
                    std::string password, 
                    std::string email,
                    std::string azienda_produzione, 
                    bool stato)

        : Utente(nome_utente, nome, cognome, numero_telefono, password, email), azienda_produzione(azienda_produzione), stato(stato) {}


    // Metodo specifico per UtenteFornitore
    void mostraInformazioniFornitore() {
        mostraInformazioni(); // Chiama il metodo della classe base
        std::cout << "Azienda di produzione: " << azienda_produzione << std::endl;
        std::cout << "Stato: " << stato << std::endl;
    }
};




#endif // USER_FORNITORE_H
