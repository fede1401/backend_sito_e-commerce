#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <cctype> // Per isupper() e isdigit()
#include <sstream>

#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db/pgsql.h"

bool isSpecialCharacter(char c)
{
    // Puoi aggiungere altri caratteri speciali se necessario
    return !std::isalnum(c);
}

// PGresult *res;
// char sqlcmd[1000];

// int rows, k;

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


    void effettua_login(std::string categoria, std::string input_nome_utente, std::string input_passw)
    {

        // nome_utente = utente.nome_utente;
        // password = utente.password;

        // Connession al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");


        // UTENTE COMPRATORE
        if (categoria == "UtenteCompratore")
        {

            // Controlla se l'utente è già loggato:
            int stato_utente;

            sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_compratore = '%s'", categoria.c_str(), input_nome_utente.c_str());

            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);

            // Riga contenente lo stato dell'utente con nome utente : std::string input_nome_utente
            if (rows == 1)
            {

                // Importante che come parametri PQgetvalues al secondo campo indica il numero di riga, dato che c'è solo uno stato di un'utente, si troverà all'indice 0 della riga
                stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

                if (stato_utente == 1)
                {
                    std::cout << "L'utente è già connesso." << std::endl;
                    return;
                }
                else
                {
                    // Aggiorniamo lo stato dell'utente: da disconnesso a connesso:

                    // Verifica della password:
                    std::string password_utente;
                    char *password_u;

                    // sprintf(sqlcmd, "SELECT password FROM UtenteCompratore WHERE nome_utente = '%s'", input_nome_utente.c_str());   // SELECT password FROM UtenteCompratore WHERE nome_utente = 'fede14';
                    sprintf(sqlcmd, "SELECT password FROM %s WHERE nome_utente_compratore = '%s'", categoria.c_str(), input_nome_utente.c_str()); // SELECT password FROM UtenteCompratore WHERE nome_utente = 'fede14';

                    PGresult *res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);

                    if (rows == 1)
                    {

                        password_u = PQgetvalue(res, 0, PQfnumber(res, "password"));
                        password_utente.assign(password_u);
                    }
                    else
                    {
                        // Altrimenti, il nome utente non è stato trovato o ci sono più utenti con lo stesso nome utente
                        // Gestisci questa situazione di conseguenza
                        std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                        return;
                    }

                    // Ricorda di liberare la memoria del risultato della query
                    PQclear(res);

                    // Controllo della password data in input e quella segnata nel database:
                    if (input_passw != password_utente)
                    {
                        std::cout << "Errore: La passowrd non è corretta, riprovare." << std::endl;
                        return;
                    }
                    else
                    {
                        std::cout << "La passowrd " << password_utente << "è corretta." << std::endl;

                        // Aggiorno stato:

                        // sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 1 WHERE nome_utente = '%s'", input_nome_utente.c_str());
                        sprintf(sqlcmd, "UPDATE %s set stato = 1 WHERE nome_utente_compratore = '%s'", categoria.c_str(), input_nome_utente.c_str());

                        res = db1.ExecSQLcmd(sqlcmd);

                        PQclear(res);

                        std::cout << "Lo stato dell'utente prima di averlo aggiornato è: " << stato_utente << std::endl;

                        // Controlla se lo stto dell'utente è stato aggiornato:

                        sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_compratore = '%s'", categoria.c_str(), input_nome_utente.c_str());

                        res = db1.ExecSQLtuples(sqlcmd);
                        rows = PQntuples(res);

                        if (rows == 1)
                        {
                            stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));
                            std::cout << "Lo stato dell'utente dopo averlo aggiornato con l'update è: " << stato_utente << std::endl;
                        }

                        else
                        {
                            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                            return;
                        }

                        /*
                        // Creo il costruttore della classe utente compratore dopo il login:
                        UtenteCompratore compratore;
                        sprintf(sqlcmd, "SELECT * FROM %s WHERE nome_utente_compratore = '%s'", categoria.c_str(), input_nome_utente.c_str());

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
                            compratore.numero_civico = atoi(PQgetvalue(res, 0, PQfnumber(res, "numero_civico")));
                            compratore.CAP = PQgetvalue(res, 0, PQfnumber(res, "CAP"));
                            compratore.città_residenza = PQgetvalue(res, 0, PQfnumber(res, "citta_di_residenza"));
                            compratore.saldo = atof(PQgetvalue(res, 0, PQfnumber(res, "saldo")));
                            compratore.stato = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));
                        }
                        else{
                            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                            return;
                        }
                        */
                        
                    }
                }
            }
            else
            {
                std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                return;
            }
        }


        // UTENTE FORNITORE
        if (categoria == "UtenteFornitore")
        {
            // Controlla se l'utente è già loggato:
            int stato_utente;

            sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_fornitore = '%s'", categoria.c_str(), input_nome_utente.c_str());

            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);

            if (rows == 1)
            {

                // Importante che come parametri PQgetvalues al secondo campo indica il numero di riga, dato che c'è solo uno stato di un'utente, si troverà all'indice 0 della riga
                stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

                if (stato_utente == 1)
                {
                    std::cout << "L'utente è già connesso." << std::endl;
                    return;
                }
                else
                {
                    // Aggiorniamo lo stato dell'utente: da disconnesso a connesso:

                    // Verifica della password:
                    std::string password_utente;
                    char *password_u;

                    // sprintf(sqlcmd, "SELECT password FROM UtenteCompratore WHERE nome_utente = '%s'", input_nome_utente.c_str());   // SELECT password FROM UtenteCompratore WHERE nome_utente = 'fede14';
                    sprintf(sqlcmd, "SELECT password FROM %s WHERE nome_utente_fornitore = '%s'", categoria.c_str(), input_nome_utente.c_str()); // SELECT password FROM UtenteCompratore WHERE nome_utente = 'fede14';

                    PGresult *res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);

                    if (rows == 1)
                    {

                        password_u = PQgetvalue(res, 0, PQfnumber(res, "password"));
                        password_utente.assign(password_u);
                    }
                    else
                    {
                        // Altrimenti, il nome utente non è stato trovato o ci sono più utenti con lo stesso nome utente
                        // Gestisci questa situazione di conseguenza
                        std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                        return;
                    }

                    // Ricorda di liberare la memoria del risultato della query
                    PQclear(res);

                    // Controllo della password data in input e quella segnata nel database:
                    if (input_passw != password_utente)
                    {
                        std::cout << "Errore: La passowrd non è corretta, riprovare." << std::endl;
                        return;
                    }
                    else
                    {
                        std::cout << "La passowrd " << password_utente << "è corretta." << std::endl;

                        // Aggiorno stato:

                        // sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 1 WHERE nome_utente = '%s'", input_nome_utente.c_str());
                        sprintf(sqlcmd, "UPDATE %s set stato = 1 WHERE nome_utente_fornitore = '%s'", categoria.c_str(), input_nome_utente.c_str());

                        res = db1.ExecSQLcmd(sqlcmd);

                        PQclear(res);

                        std::cout << "Lo stato dell'utente prima di averlo aggiornato è: " << stato_utente << std::endl;

                        // Controlla se lo stto dell'utente è stato aggiornato:

                        sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_fornitore = '%s'", categoria.c_str(), input_nome_utente.c_str());

                        res = db1.ExecSQLtuples(sqlcmd);
                        rows = PQntuples(res);

                        if (rows == 1)
                        {
                            stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));
                            std::cout << "Lo stato dell'utente dopo averlo aggiornato con l'update è: " << stato_utente << std::endl;
                        }

                        else
                        {
                            std::cout << "Errore: Lo stato non è stato trovato." << std::endl;
                            return;
                        }

                        /*
                        // Creo il costruttore della classe utente fornitore dopo il login:
                        UtenteFornitore fornitore;
                        sprintf(sqlcmd, "SELECT * FROM %s WHERE nome_utente_fornitore = '%s'", categoria.c_str(), input_nome_utente.c_str());

                        res = db1.ExecSQLtuples(sqlcmd);
                        rows = PQntuples(res);

                        if (rows == 1){
                            fornitore.nome_utente = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_fornitore"));
                            fornitore.categoria = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));
                            fornitore.nome = PQgetvalue(res, 0, PQfnumber(res, "nome"));
                            fornitore.cognome = PQgetvalue(res, 0, PQfnumber(res, "cognome"));
                            fornitore.email = PQgetvalue(res, 0, PQfnumber(res, "indirizzo_mail"));
                            fornitore.numero_telefono = PQgetvalue(res, 0, PQfnumber(res, "numero_di_telefono"));
                            fornitore.azienda_produzione = PQgetvalue(res, 0, PQfnumber(res, "nome_AziendaProduttrice"));
                            fornitore.password = PQgetvalue(res, 0, PQfnumber(res, "password"));
                            fornitore.stato = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));
                        }
                        else{
                            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                            return;
                        }
                        */
                        

                    }
                }
            }
            else
            {
                std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                return;
            }


        }


        // UTENTE TRASPORTATORE
        if (categoria == "UtenteTrasportatore")
        {
            // Controlla se l'utente è già loggato:
            int stato_utente;

            sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_trasportatore = '%s'", categoria.c_str(), input_nome_utente.c_str());

            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);

            if (rows == 1)
            {

                // Importante che come parametri PQgetvalues al secondo campo indica il numero di riga, dato che c'è solo uno stato di un'utente, si troverà all'indice 0 della riga
                stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

                if (stato_utente == 1)
                {
                    std::cout << "L'utente è già connesso." << std::endl;
                    return;
                }
                else
                {
                    // Aggiorniamo lo stato dell'utente: da disconnesso a connesso:

                    // Verifica della password:
                    std::string password_utente;
                    char *password_u;

                    // sprintf(sqlcmd, "SELECT password FROM UtenteCompratore WHERE nome_utente = '%s'", input_nome_utente.c_str());   // SELECT password FROM UtenteCompratore WHERE nome_utente = 'fede14';
                    sprintf(sqlcmd, "SELECT password FROM %s WHERE nome_utente_trasportatore = '%s'", categoria.c_str(), input_nome_utente.c_str()); // SELECT password FROM UtenteCompratore WHERE nome_utente = 'fede14';

                    PGresult *res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);

                    if (rows == 1)
                    {

                        password_u = PQgetvalue(res, 0, PQfnumber(res, "password"));
                        password_utente.assign(password_u);
                    }
                    else
                    {
                        // Altrimenti, il nome utente non è stato trovato o ci sono più utenti con lo stesso nome utente
                        // Gestisci questa situazione di conseguenza
                        std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                        return;
                    }

                    // Ricorda di liberare la memoria del risultato della query
                    PQclear(res);

                    // Controllo della password data in input e quella segnata nel database:
                    if (input_passw != password_utente)
                    {
                        std::cout << "Errore: La passowrd non è corretta, riprovare." << std::endl;
                        return;
                    }
                    else
                    {
                        std::cout << "La passowrd " << password_utente << "è corretta." << std::endl;

                        // Aggiorno stato:

                        // sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 1 WHERE nome_utente = '%s'", input_nome_utente.c_str());
                        sprintf(sqlcmd, "UPDATE %s set stato = 1 WHERE nome_utente_trasportatore = '%s'", categoria.c_str(), input_nome_utente.c_str());

                        res = db1.ExecSQLcmd(sqlcmd);

                        PQclear(res);

                        std::cout << "Lo stato dell'utente prima di averlo aggiornato è: " << stato_utente << std::endl;

                        // Controlla se lo stto dell'utente è stato aggiornato:

                        sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_trasportatore = '%s'", categoria.c_str(), input_nome_utente.c_str());

                        res = db1.ExecSQLtuples(sqlcmd);
                        rows = PQntuples(res);

                        if (rows == 1)
                        {
                            stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));
                            std::cout << "Lo stato dell'utente dopo averlo aggiornato con l'update è: " << stato_utente << std::endl;
                        }

                        else
                        {
                            std::cout << "Errore: Lo stato non è stato trovato." << std::endl;
                            return;
                        }

                        /*
                        // Creo il costruttore della classe utente trasportatore dopo il login:
                        UtenteTrasportatore trasportatore;
                        sprintf(sqlcmd, "SELECT * FROM %s WHERE nome_utente_trasportatore = '%s'", categoria.c_str(), input_nome_utente.c_str());

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
                        }
                        else{
                            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                            return;
                        }
                        */

                        
                    }
                }
            }
            else
            {
                std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                return;
            }
        }

        return;
    }












    void effettua_logout(std::string categoria, std::string input_nome_utente)
    {

        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        
        // UTENTE COMPRATORE    
        if (categoria == "UtenteCompratore")
        {
            // Controlla se l'utente è già loggato:
            int stato_utente;
                
            sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_compratore = '%s'", categoria.c_str(), input_nome_utente.c_str());

            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);


            if (rows == 1)
            {
                // std::cout << "Fino a riga 58 tutto ok." << std::endl;

                // Importante che come parametri PQgetvalues al secondo campo indica il numero di riga, dato che c'è solo uno stato di un'utente, si troverà all'indice 0 della riga
                stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

                if (stato_utente == 0){
                    
                    std::cout << "Errore: L'utente è già disconnesso" << std::endl;
                    return;
                }
                
                else{
                    // Aggiorniamo lo stato dell'utente, da connesso a disconnesso:
                    
                    // sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 0 WHERE nome_utente = '%s'", input_nome_utente.c_str());
                    sprintf(sqlcmd, "UPDATE %s set stato = 0 WHERE nome_utente_compratore = '%s'", categoria.c_str(), input_nome_utente.c_str());

                    res = db1.ExecSQLcmd(sqlcmd);

                    PQclear(res);


                    // Controlla se lo stato dell'utente è stato aggiornato:

                    // sprintf(sqlcmd, "SELECT stato FROM UtenteCompratore WHERE nome_utente = '%s'", input_nome_utente.c_str());
                    sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_compratore = '%s'", categoria.c_str(), input_nome_utente.c_str());

                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);

                    if (rows == 1) {
                        stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));
                        std::cout << "Lo stato dell'utente dopo l'update logout è: " << stato_utente << std::endl;
                    }
                    else {
                        std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                        return;
                    }

                }

            }
            else
            {
                std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                return;
            }            
        }




        // UTENTE FORNITORE    
        if (categoria == "UtenteFornitore")
        {
            // Controlla se l'utente è già loggato:
            int stato_utente;
                
            sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_fornitore = '%s'", categoria.c_str(), input_nome_utente.c_str());

            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);


            if (rows == 1)
            {
                // std::cout << "Fino a riga 58 tutto ok." << std::endl;

                // Importante che come parametri PQgetvalues al secondo campo indica il numero di riga, dato che c'è solo uno stato di un'utente, si troverà all'indice 0 della riga
                stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

                if (stato_utente == 0){
                    
                    std::cout << "Errore: L'utente è già disconnesso" << std::endl;
                    return;
                }
                
                else{
                    // Aggiorniamo lo stato dell'utente, da connesso a disconnesso:
                    
                    // sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 0 WHERE nome_utente = '%s'", input_nome_utente.c_str());
                    sprintf(sqlcmd, "UPDATE %s set stato = 0 WHERE nome_utente_fornitore = '%s'", categoria.c_str(), input_nome_utente.c_str());

                    res = db1.ExecSQLcmd(sqlcmd);

                    PQclear(res);


                    // Controlla se lo stato dell'utente è stato aggiornato:

                    // sprintf(sqlcmd, "SELECT stato FROM UtenteCompratore WHERE nome_utente = '%s'", input_nome_utente.c_str());
                    sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_fornitore = '%s'", categoria.c_str(), input_nome_utente.c_str());

                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);

                    if (rows == 1) {
                        stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));
                        std::cout << "Lo stato dell'utente dopo l'update logout è: " << stato_utente << std::endl;
                    }
                    else {
                        std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                        return;
                    }

                }

            }
            else
            {
                std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                return;
            }            
        }




        // UTENTE TRASPORTATORE    
        if (categoria == "UtenteTrasportatore")
        {
            // Controlla se l'utente è già loggato:
            int stato_utente;
                
            sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_trasportatore = '%s'", categoria.c_str(), input_nome_utente.c_str());

            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);


            if (rows == 1)
            {
                // std::cout << "Fino a riga 58 tutto ok." << std::endl;

                // Importante che come parametri PQgetvalues al secondo campo indica il numero di riga, dato che c'è solo uno stato di un'utente, si troverà all'indice 0 della riga
                stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));

                if (stato_utente == 0){
                    
                    std::cout << "Errore: L'utente è già disconnesso" << std::endl;
                    return;
                }
                
                else{
                    // Aggiorniamo lo stato dell'utente, da connesso a disconnesso:
                    
                    // sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 0 WHERE nome_utente = '%s'", input_nome_utente.c_str());
                    sprintf(sqlcmd, "UPDATE %s set stato = 0 WHERE nome_utente_trasportatore = '%s'", categoria.c_str(), input_nome_utente.c_str());

                    res = db1.ExecSQLcmd(sqlcmd);

                    PQclear(res);


                    // Controlla se lo stato dell'utente è stato aggiornato:

                    // sprintf(sqlcmd, "SELECT stato FROM UtenteCompratore WHERE nome_utente = '%s'", input_nome_utente.c_str());
                    sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente_trasportatore = '%s'", categoria.c_str(), input_nome_utente.c_str());

                    res = db1.ExecSQLtuples(sqlcmd);
                    rows = PQntuples(res);

                    if (rows == 1) {
                        stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato")));
                        std::cout << "Lo stato dell'utente dopo l'update logout è: " << stato_utente << std::endl;
                    }
                    else {
                        std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                        return;
                    }

                }

            }
            else
            {
                std::cout << "Errore: L'utente non è stato trovato." << std::endl;
                return;
            }            
        }

    
    
    
    
    return;
    
    }

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
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

};

#endif // USER_H