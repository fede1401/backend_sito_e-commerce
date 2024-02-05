// Carrello.h
#ifndef CARRELLO_H
#define CARRELLO_H

#include <string>

class Carrello {
public:
    std::string nome_utente_compratore;
    int codice_prodotto;

    // Costruttore
    Carrello(std::string nome_utente_compratore, int codice_prod) : nome_utente_compratore(nome_utente_compratore), codice_prodotto(codice_prod) {}

    Carrello(): nome_utente_compratore(""), codice_prodotto(-1){}


    void add_prodotto(std::string in_nome_utente_compratore, int in_cod_prodotto){
        
        ///////////////////////////////////// 
        // Controllo se esiste l'id dell'utente preso in input:
        
        // Connessione al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");      
        std::cout << "Connessione al database avvenuta con successo." << std::endl;

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


        ///////////////////////////////////// 
        // Inseriamo il prodotto nel carrello:
        sprintf(sqlcmd, "INSERT INTO Carrello (idUtComp, codProdotto) VALUES ('%d', '%d')", idUtenCompr, in_cod_prodotto);
        res = db1.ExecSQLcmd(sqlcmd);
        PQclear(res);   
        /////////////////////////////////////         
    }



    void remove_prodotto(std::string in_nome_utente_compratore, int in_cod_prodotto){
        
        ///////////////////////////////////// 
        // Controlliamo se esiste la riga del prodotto da eliminare:

        // Connessione al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");      
        std::cout << "Connessione al database avvenuta con successo." << std::endl;


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


        sprintf(sqlcmd, "SELECT * FROM Carrello WHERE idUtComp = '%d' AND codProdotto ='%d'", idUtenCompr, in_cod_prodotto);
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
