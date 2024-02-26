// ListaDesideri.h
#ifndef LISTA_DESIDERI_H
#define LISTA_DESIDERI_H

#include "main.h"


class ListaDesideri {
public:
    std::string nome_utente_compratore;
    int codice_prodotto;

    // Costruttore
    ListaDesideri(std::string nome_utente_compratore, int codice_prod)
        : nome_utente_compratore(nome_utente_compratore), codice_prodotto(codice_prod) {}

    ListaDesideri() : nome_utente_compratore(""), codice_prodotto(-1){}


    void add_prodotto(Con2DB db1, std::string in_nome_utente_compratore, int in_cod_prodotto){
        
        // Connessione al database:
        //Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");      
        //std::cout << "Connessione al database avvenuta con successo." << std::endl;

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
        
        std::string nomeRequisito = "Aggiunta prodotto alla lista desideri.";
        statoRequisito statoReq = statoRequisito::Wait;


        // Caricamento del sessionID utile per il log.
        std::string sessionID = "";
        sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));}  


        ///////////////////////////////////// 
        // Controllo se esiste il codice del prodotto da inserire:
        // SELECT:
        sprintf(sqlcmd, "SELECT * FROM Prodotto WHERE codProdotto = '%d'", in_cod_prodotto);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);
        if (rows < 1){

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1, "ERROR", "Il prodotto non esiste.", sessionID, nomeRequisito, statoReq);
            std::cout << "Il prodotto non esiste!" << std::endl;
            return;
        }


        

        // Controllo se il prodotto è stato già inserito dall'utente nel carrello:
        int codProdotto; 
        bool trovato=false;
        
        sprintf(sqlcmd, "SELECT codProdotto FROM ListaDesideri WHERE nome_utente_compratore = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);
        for (int i = 0; i < rows; i++)
        {
            codProdotto = atoi(PQgetvalue(res, i, PQfnumber(res, "codProdotto")));
            if (codProdotto == in_cod_prodotto){
                // Il prodotto è già stato inserito, perciò ne aumentiamo la quantità:
                // Prima carichiamo la quantità precedente:
                trovato = true;
                int quantitàPrecedente;
                sprintf(sqlcmd, "SELECT quantitàProd FROM ListaDesideri WHERE nome_utente_compratore = '%s' AND codProdotto = '%d'", in_nome_utente_compratore.c_str(),in_cod_prodotto);
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);
                PQclear(res);
                quantitàPrecedente = atoi(PQgetvalue(res, 0, PQfnumber(res, "quantitàProd"))); 

                quantitàPrecedente = quantitàPrecedente + 1;
                //Update
                sprintf(sqlcmd, "UPDATE ListaDesideri set quantitàProd = '%d' WHERE nome_utente_compratore = '%s' AND codProdotto = '%d'", quantitàPrecedente, in_nome_utente_compratore.c_str(),in_cod_prodotto);
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                statoReq = statoRequisito::Success;

                InsertToLogDB(db1, "INFO", "Il prodotto già esiste, ne aggiungiamo la quantità.", sessionID, nomeRequisito, statoReq);
            }
        }
         
        if (trovato == false){
             // Inseriamo il prodotto per la prima volta nel carrello:
            sprintf(sqlcmd, "INSERT INTO ListaDesideri (nome_utente_compratore, codProdotto, quantitàProd) VALUES ('%s', '%d', '%d')", in_nome_utente_compratore.c_str(), in_cod_prodotto, 1);
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);   

            statoReq = statoRequisito::Success;
            InsertToLogDB(db1, "INFO", "Inserimento del prodotto nel db.", sessionID, nomeRequisito, statoReq);
        }
           
    return;      
    }


    void remove_prodotto(Con2DB db1, std::string in_nome_utente_compratore, int in_cod_prodotto){
        
        ///////////////////////////////////// 
        // Controlliamo se esiste la riga del prodotto da eliminare:

        // Connessione al database:
        //Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");      
        //std::cout << "Connessione al database avvenuta con successo." << std::endl;


        std::string nomeRequisito = "Rimozione prodotto dalla lista dei desideri.";
        statoRequisito statoReq = statoRequisito::Wait;
                

        // Caricamento del sessionID utile per il log.
        std::string sessionID = "";
        sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));}  


        /*
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
       

        sprintf(sqlcmd, "SELECT * FROM ListaDesideri WHERE nome_utente_compratore = '%s' AND codProdotto ='%d'", in_nome_utente_compratore.c_str(), in_cod_prodotto);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);
        if (rows < 1){
            std::cout << "La riga da eliminare non esiste!" << std::endl;

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1, "ERROR", "Il prodotto da eliminare non esiste", sessionID, nomeRequisito, statoReq);
            return;
        }
        else{
            // Eliminazione del prodotto dalla lsta desideri dell'utente compratore.
            sprintf(sqlcmd, "DELETE FROM ListaDesideri WHERE nome_utente_compratore='%s' AND codProdotto='%d", in_nome_utente_compratore.c_str(), in_cod_prodotto);
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            statoReq = statoRequisito::Success;

            InsertToLogDB(db1, "INFO", "Rimozione del prodotto nel db.", sessionID, nomeRequisito, statoReq);
        }
    return;
    }
};

#endif // LISTA_DESIDERI_H
