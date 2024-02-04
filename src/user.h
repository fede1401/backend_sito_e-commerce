#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <cctype> // Per isupper() e isdigit()
#include <sstream>
#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db/pgsql.h"


PGresult *res;
char sqlcmd[1000];

int rows, k;

class Utente {
public:
    std::string id;
    std::string categoria;
    std::string nome_utente;
    std::string nome;
    std::string cognome;
    std::string numero_telefono;
    std::string password;
    std::string email;

    Utente(std::string nome_utente, std::string categoria, std::string nome, std::string cognome, std::string numero_telefono, std::string password, std::string email) 
            : categoria(categoria), nome_utente(nome_utente), nome(nome), cognome(cognome), numero_telefono(numero_telefono), password(password), email(email) {}


    void mostraInformazioni() {
        std::cout << "ID: " << id << std::endl;
        std::cout << "Nome utente: " << nome_utente << std::endl;
        std::cout << "Nome: " << nome << std::endl;
        std::cout << "Cognome: " << cognome << std::endl;
        std::cout << "Numero di telefono: " << numero_telefono << std::endl;
        std::cout << "Password: " << password << std::endl;
        std::cout << "Email: " << email << std::endl;
    }


    void effettua_login(std::string categoria, std::string input_nome_utente, std::string input_passw ){

        //nome_utente = utente.nome_utente;
        //password = utente.password;

        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce");

        /////////////////////////////////////////////
        // Controlla se l'utente è già loggato:
        int stato_utente;

        //sprintf(sqlcmd, "SELECT stato FROM UtenteCompratore WHERE nome_utente = '%s'", input_nome_utente.c_str());
        sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente = '%s'", categoria.c_str(), input_nome_utente.c_str());

        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        std::cout << "Rows: " << rows << std::endl;

        if (rows == 1) {
            //std::cout << "Fino a riga 58 tutto ok." << std::endl;

            // Importante che come parametri PQgetvalues al secondo campo indica il numero di riga, dato che c'è solo uno stato di un'utente, si troverà all'indice 0 della riga
            stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

        } else {
            std::cout << "Errore: Lo stato non è stato trovato." << std::endl;
            return;
        }

        std::cout << "Lo stato dell'utente è: " << stato_utente << std::endl;
        /////////////////////////////////////////////


        /////////////////////////////////////////////
        // Verifica della password:
        std::string password_utente;
        char *password_u;

        //sprintf(sqlcmd, "SELECT password FROM UtenteCompratore WHERE nome_utente = '%s'", input_nome_utente.c_str());   // SELECT password FROM UtenteCompratore WHERE nome_utente = 'fede14';
        sprintf(sqlcmd, "SELECT password FROM %s WHERE nome_utente = '%s'", categoria.c_str(), input_nome_utente.c_str());   // SELECT password FROM UtenteCompratore WHERE nome_utente = 'fede14';

        PGresult *res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        
        if (rows == 1) {

        password_u = PQgetvalue(res, 0, PQfnumber(res, "password"));
        password_utente.assign(password_u);

        std::cout << "Fino a riga 90 tutto ok." << std::endl;
        
        // Ora password_string contiene la password come stringa
        } 
        else {
            // Altrimenti, il nome utente non è stato trovato o ci sono più utenti con lo stesso nome utente
            // Gestisci questa situazione di conseguenza
            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
            return;
        }

        // Ricorda di liberare la memoria del risultato della query
        PQclear(res);


        if (input_passw != password_utente){
            std::cout << "Errore: La passowrd non è corretta, riprovare." << std::endl;
            return;

        }
        else{
            std::cout << "La passowrd " << password_utente << "è corretta." << std::endl;
        }

        std::cout << "Fino a riga 114 tutto ok." << std::endl;
        /////////////////////////////////////////////



        /////////////////////////////////////////////
        // Aggiorno stato:
    
        //sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 1 WHERE nome_utente = '%s'", input_nome_utente.c_str());
        sprintf(sqlcmd, "UPDATE %s set stato = 1 WHERE nome_utente = '%s'",categoria.c_str() ,input_nome_utente.c_str());

        res = db1.ExecSQLcmd(sqlcmd);
        
        PQclear(res); 
        /////////////////////////////////////////////


        /////////////////////////////////////////////
        // Controlla se lo stto dell'utente è stato aggiornato:

        //sprintf(sqlcmd, "SELECT stato FROM UtenteCompratore WHERE nome_utente = '%s'", input_nome_utente.c_str());
        sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente = '%s'", categoria.c_str(), input_nome_utente.c_str());

        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        std::cout << "Rows: " << rows << std::endl;

        if (rows == 1) {
            //std::cout << "Fino a riga 58 tutto ok." << std::endl;

            // Importante che come parametri PQgetvalues al secondo campo indica il numero di riga, dato che c'è solo uno stato di un'utente, si troverà all'indice 0 della riga
            stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

        } else {
            std::cout << "Errore: Lo stato non è stato trovato." << std::endl;
            return;
        }

        std::cout << "Lo stato dell'utente dopo l'update è: " << stato_utente << std::endl;
        /////////////////////////////////////////////

        

        
        return;
    }


    void effettua_logout(std::string categoria, std::string input_nome_utente){
        
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce");

        
        /////////////////////////////////////////////
        // Controlla se l'utente è già loggato:
        int stato_utente;

        //sprintf(sqlcmd, "SELECT stato FROM UtenteCompratore WHERE nome_utente = '%s'", input_nome_utente.c_str());
        sprintf(sqlcmd, "SELECT stato FROM %S WHERE nome_utente = '%s'", categoria.c_str(), input_nome_utente.c_str());

        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        std::cout << "Rows: " << rows << std::endl;

        if (rows == 1) {
            //std::cout << "Fino a riga 58 tutto ok." << std::endl;

            // Importante che come parametri PQgetvalues al secondo campo indica il numero di riga, dato che c'è solo uno stato di un'utente, si troverà all'indice 0 della riga
            stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

        } else {
            std::cout << "Errore: Lo stato non è stato trovato." << std::endl;
            return;
        }

        std::cout << "Lo stato dell'utente è: " << stato_utente << std::endl;
        /////////////////////////////////////////////

        if (stato_utente == 1){

            /////////////////////////////////////////////
            // Aggiorno stato:
    
            //sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 0 WHERE nome_utente = '%s'", input_nome_utente.c_str());
            sprintf(sqlcmd, "UPDATE %s set stato = 0 WHERE nome_utente = '%s'", categoria.c_str(), input_nome_utente.c_str());
        
            res = db1.ExecSQLcmd(sqlcmd);
        
            PQclear(res); 
            
            /////////////////////////////////////////////
        }
        else{
            std::cout << "Lo stato non è connesso perciò non può essere effettuata la disconnessione: " << stato_utente << std::endl;
        }

        /////////////////////////////////////////////
        // Controlla se lo stto dell'utente è stato aggiornato:

        // sprintf(sqlcmd, "SELECT stato FROM UtenteCompratore WHERE nome_utente = '%s'", input_nome_utente.c_str());
        sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente = '%s'", categoria.c_str(), input_nome_utente.c_str());

        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        std::cout << "Rows: " << rows << std::endl;

        if (rows == 1) {
            //std::cout << "Fino a riga 58 tutto ok." << std::endl;

            // Importante che come parametri PQgetvalues al secondo campo indica il numero di riga, dato che c'è solo uno stato di un'utente, si troverà all'indice 0 della riga
            stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

        } else {
            std::cout << "Errore: Lo stato non è stato trovato." << std::endl;
            return;
        }

        std::cout << "Lo stato dell'utente dopo l'update è: " << stato_utente << std::endl;
        /////////////////////////////////////////////

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

