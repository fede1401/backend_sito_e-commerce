// Carrello.h
#ifndef CARRELLO_H
#define CARRELLO_H

#include <string>
#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db/pgsql.h"

PGresult *res;
char sqlcmd[1000];

int rows, k;


class Carrello {
public:
    std::string nome_utente_compratore;
    int codice_prodotto;
    int quantitàProdotti;

    // Costruttore
    Carrello(std::string nome_utente_compratore, int codice_prod, int quantitàProdotti) : 
            nome_utente_compratore(nome_utente_compratore), codice_prodotto(codice_prod), quantitàProdotti(quantitàProdotti) {}

    // Costruttore predefinito
    Carrello(): nome_utente_compratore(""), codice_prodotto(-1), quantitàProdotti(-1){}


    // Funzione per aggiungere un prodotto al carrello
    void add_prodotto(std::string in_nome_utente_compratore, int in_cod_prodotto){
        
        /*
        ///////////////////////////////////// 
        // Controllo se esiste l'id dell'utente preso in input:
        // SELECT:
        int idUtenCompr;
        sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE nome_utente = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows < 1){
            std::cout << "L'utente non esiste" << std::endl;
            return;
        }
        else{
            idUtenCompr = atoi(PQgetvalue(res, 0, PQfnumber(res, "idUtComp")));
        }
        /////////////////////////////////////
        */
        Carrello carrello;

        // Connessione al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");      
        std::cout << "Connessione al database avvenuta con successo." << std::endl;


        ///////////////////////////////////// 
        // Controllo se esiste il codice del prodotto da inserire:
        // SELECT:
        sprintf(sqlcmd, "SELECT * FROM Prodotto WHERE codProdotto = '%d'", in_cod_prodotto);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows < 1){
            std::cout << "Il prodotto non esiste!" << std::endl;
            return;
        }
        /////////////////////////////////////

        
        // Controllo se il prodotto è stato già inserito dall'utente nel carrello:
        int codProdotto; 
        bool trovato=false;
        
        sprintf(sqlcmd, "SELECT codProdotto FROM Carrello WHERE nome_utente_compratore = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        for (int i = 0; i < rows; i++)
        {
            codProdotto = atoi(PQgetvalue(res, i, PQfnumber(res, "codProdotto")));
            if (codProdotto == in_cod_prodotto){
                // Il prodotto è già stato inserito, perciò ne aumentiamo la quantità:
                // Prima carichiamo la quantità precedente:
                trovato = true;
                int quantitàPrecedente;
                sprintf(sqlcmd, "SELECT quantitàProd FROM Carrello WHERE nome_utente_compratore = '%s' AND codProdotto = '%d'", in_nome_utente_compratore.c_str(),in_cod_prodotto);
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);
                quantitàPrecedente = atoi(PQgetvalue(res, 0, PQfnumber(res, "quantitàProd"))); 

                quantitàPrecedente = quantitàPrecedente + 1;
                //Update
                sprintf(sqlcmd, "UPDATE Carrello set quantitàProd = '%d' WHERE nome_utente_compratore = '%s' AND codProdotto = '%d'", quantitàPrecedente, in_nome_utente_compratore.c_str(),in_cod_prodotto);
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                // Anima l'oggetto
                carrello.codice_prodotto = codProdotto;
                carrello.nome_utente_compratore = in_nome_utente_compratore;
                carrello.quantitàProdotti = quantitàPrecedente;
            }
        }
         
        if (trovato == false){
             // Inseriamo il prodotto per la prima volta nel carrello:
            sprintf(sqlcmd, "INSERT INTO Carrello (nome_utente_compratore, codProdotto, quantitàProd) VALUES ('%s', '%d', '%d')", in_nome_utente_compratore.c_str(), in_cod_prodotto, 1);
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);   
            carrello.codice_prodotto = in_cod_prodotto;
            carrello.nome_utente_compratore = in_nome_utente_compratore;
            carrello.quantitàProdotti = 1;

        }
           
    
    }


    void remove_prodotto(std::string in_nome_utente_compratore, int in_cod_prodotto){
        
        /*
        ///////////////////////////////////// 
        // Controlliamo se esiste la riga del prodotto da eliminare:
        // SELECT dell'id utente dato il suo nome utente:
        int idUtenCompr;
        sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE nome_utente = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows < 1){
            std::cout << "L'utente non esiste" << std::endl;
            return;
        }
        else{
            idUtenCompr = atoi(PQgetvalue(res, 0, PQfnumber(res, "idUtComp")));
        }
        */
        
        // Connessione al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");      
        std::cout << "Connessione al database avvenuta con successo." << std::endl;


        
        sprintf(sqlcmd, "SELECT * FROM Carrello WHERE nome_utente_compratore = '%d' AND codProdotto ='%d'", in_nome_utente_compratore.c_str(), in_cod_prodotto);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows < 1){
            std::cout << "La riga da eliminare non esiste!" << std::endl;
            return;
        }
        else{
            // eliminare
        }

    }


};

#endif // CARRELLO_H
