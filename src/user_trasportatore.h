#ifndef USER_TRASPORTATORE_H
#define USER_TRASPORTATORE_H

#include <iostream>
#include <string>
#include <cctype> // Per isupper() e isdigit()
#include <sstream>
#include "user.h"
#include "/home/federico/sito_ecommerce/con2db/pgsql.h"



class UtenteTrasportatore : public Utente {
public:
    // Attributi specifici per UtenteTrasportatore
    std::string ditta_spedizione;
    bool stato;

    // Costruttore di UtenteTrasportatore
    UtenteTrasportatore(std::string nome_utente, std::string nome, std::string cognome, 
                        std::string numero_telefono, std::string password, std::string email,
                        std::string ditta_spedizione, bool stato)
        : Utente( nome_utente, nome, cognome, numero_telefono, password, email), ditta_spedizione(ditta_spedizione), stato(stato) {}

    // Metodo specifico per UtenteTrasportatore
    void mostraInformazioniTrasportatore() {
        mostraInformazioni(); // Chiama il metodo della classe base
        std::cout << "Ditta di spedizione: " << ditta_spedizione << std::endl;
        std::cout << "Stato: " << stato << std::endl;
    }
};


#endif // USER_TRASPORTATORE_H
