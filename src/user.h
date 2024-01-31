#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <cctype> // Per isupper() e isdigit()
#include <sstream>
#include "/home/federico/sito_ecommerce/con2db/pgsql.h"


//PGresult *res;
//char sqlcmd[1000];

//int rows, k;

class Utente {
public:
    std::string id;
    std::string nome_utente;
    std::string nome;
    std::string cognome;
    std::string numero_telefono;
    std::string password;
    std::string email;

    Utente(std::string nome_utente, std::string nome, std::string cognome, std::string numero_telefono, std::string password, std::string email) 
            : nome_utente(nome_utente), nome(nome), cognome(cognome), numero_telefono(numero_telefono), password(password), email(email) {}


    void mostraInformazioni() {
        std::cout << "ID: " << id << std::endl;
        std::cout << "Nome utente: " << nome_utente << std::endl;
        std::cout << "Nome: " << nome << std::endl;
        std::cout << "Cognome: " << cognome << std::endl;
        std::cout << "Numero di telefono: " << numero_telefono << std::endl;
        std::cout << "Password: " << password << std::endl;
        std::cout << "Email: " << email << std::endl;
    }


    void effettua_login(std::string input_nome_utente, std::string input_passw ){

        //nome_utente = utente.nome_utente;
        //password = utente.password;
        

        







        return;
    }


};









/*int main() {
    // Esempi di utilizzo delle classi derivate

    UtenteCompratore compratore1("001", "mio_utente", "Mario", "Rossi", 123456789, "password123", "mario@email.com", 30,
                                 "01/01/1990", "Via Roma", 42, "12345", "Città di Prova", 100.0, "Attivo");

    UtenteFornitore fornitore1("002", "azienda_produzione", "Luigi", "Verdi", 987654321, "password456", "luigi@email.com", 35,
                               "AziendaXYZ", "Inattivo");

    UtenteTrasportatore trasportatore1("003", "ditta_spedizione", "Giovanni", "Bianchi", 555555555, "password789", "giovanni@email.com", 25,
                                       "DittaShip", "Attivo");

    // Visualizzazione delle informazioni specifiche per ciascun tipo di utente
    compratore1.mostraInformazioniCompratore();
    std::cout << "\n----------------\n\n";
    fornitore1.mostraInformazioniFornitore();
    std::cout << "\n----------------\n\n";
    trasportatore1.mostraInformazioniTrasportatore();

    return 0;
}
*/

#endif // USER_H

