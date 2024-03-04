// Carrello.h
#ifndef CARRELLO_H
#define CARRELLO_H

#include "main.h"


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
    void add_prodotto(Con2DB db1,std::string in_nome_utente_compratore, int in_cod_prodotto){
        
        Carrello carrello;

        std::string nomeRequisito = "Aggiunta prodotto al carrello.";
        statoRequisito statoReq = statoRequisito::Wait;

        std::string messageLog = "";

        // Caricamento del sessionID utile per il log.
        std::string sessionID = "";
        sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));}  
        PQclear(res);


        if (sessionID == ""){
            messageLog = "Non esiste una sessionID per " + in_nome_utente_compratore + ", utente non loggato o non registrato, non può essere aggiunto il prodotto al carrello";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            return;
        }

        ///////////////////////////////////// 

        // Selezioniamo il codice del prodotto tramite il suo nome:
        // sprintf(sqlcmd, "SELECT codProdotto FROM Prodotto WHERE nome = '%s'", nomeProdotto.c_str());
        // res = db1.ExecSQLtuples(sqlcmd);
        // rows = PQntuples(res);

        // int in_cod_prodotto=-1;

        // if (rows == 1){  in_cod_prodotto = atoi(PQgetvalue(res, 0, PQfnumber(res, "codProdotto"))); }
        // PQclear(res);
        /////////////////////////////////////


        // Controllo se esiste il codice del prodotto da inserire:
        // SELECT:
        sprintf(sqlcmd, "SELECT * FROM Prodotto WHERE codProdotto = '%d'", in_cod_prodotto);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        if (rows < 1){
            std::cout << "Il prodotto non esiste!" << std::endl;

            statoReq = statoRequisito::NotSuccess;

            messageLog = "Il prodotto con codice " + std::to_string(in_cod_prodotto) + " non esiste.";

            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
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
            PQclear(res);
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
                PQclear(res);

                //Update
                sprintf(sqlcmd, "UPDATE Carrello set quantitàProd = '%d' WHERE nome_utente_compratore = '%s' AND codProdotto = '%d'", quantitàPrecedente, in_nome_utente_compratore.c_str(),in_cod_prodotto);
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                statoReq = statoRequisito::Success;

                messageLog = "Il prodotto esiste già, aggiungiamo la quantità nel carrello di " + in_nome_utente_compratore;

                InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);


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

            statoReq = statoRequisito::Success;

            messageLog = "Inserimento del prodotto nel carrello di " + in_nome_utente_compratore;

            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }
           
    return;
    }


    void remove_prodotto(Con2DB db1,std::string in_nome_utente_compratore, int codProdotto){
        

        std::string nomeRequisito = "Rimozione prodotto dal carrello.";
        statoRequisito statoReq = statoRequisito::Wait;

        std::string messageLog = "";

        // Caricamento del sessionID utile per il log.
        std::string sessionID = "";
        sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));}  
        PQclear(res);


        if (sessionID == ""){
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non può essere rimosso il prodotto al carrello.", sessionID, nomeRequisito, statoReq);
            return;
        }


        // Selezioniamo il codice del prodotto tramite il suo nome:
        //sprintf(sqlcmd, "SELECT codProdotto FROM Prodotto WHERE nome = '%s'", nomeProdotto.c_str());
        //res = db1.ExecSQLtuples(sqlcmd);
        //rows = PQntuples(res);

        //  int in_cod_prodotto;

        //if (rows == 1){  in_cod_prodotto = atoi(PQgetvalue(res, 0, PQfnumber(res, "codProdotto"))); }
        //PQclear(res);
        /////////////////////////////////////


        sprintf(sqlcmd, "SELECT * FROM Carrello WHERE nome_utente_compratore = '%s' AND codProdotto ='%d'", in_nome_utente_compratore.c_str(), codProdotto);
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
            // Eliminazione del prodotto dal carrello dell'utente compratore.
            sprintf(sqlcmd, "DELETE FROM Carrello WHERE nome_utente_compratore='%s' AND codProdotto='%d'", in_nome_utente_compratore.c_str(), codProdotto);
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            statoReq = statoRequisito::Success;

            messageLog = "Rimozione del prodotto con il codice " + std::to_string(codProdotto) + " dal carrello di " + in_nome_utente_compratore;

            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }
    return;
    }


};

#endif // CARRELLO_H
