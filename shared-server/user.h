#ifndef USER_H
#define USER_H

#include "../server-fornitore/src/main.h"

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

    
};

#endif // USER_H