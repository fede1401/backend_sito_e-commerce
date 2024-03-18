// ListaDesideri.h
#ifndef LISTA_DESIDERI_H
#define LISTA_DESIDERI_H

#include "main.h"


class ListaDesideri {
public:
    std::string nome_utente_compratore;
    int codice_prodotto;
    int quantitàProdotti;

    // Costruttore
    ListaDesideri(std::string nome_utente_compratore, int codice_prod, int quantitàProdotti) : 
         nome_utente_compratore(nome_utente_compratore), codice_prodotto(codice_prod), quantitàProdotti(quantitàProdotti) {}

    ListaDesideri() : nome_utente_compratore(""), codice_prodotto(-1), quantitàProdotti(-1){}


    // Metodo per aggiungere un prodotto alla lista desideri dato il nome dell'utente e il codice del prodotto.
    void add_prodotto(Con2DB db1, std::string in_nome_utente_compratore, int in_codProdotto){
                
        // Definizione di alcune variabili per il logging        
        std::string nomeRequisito = "Aggiunta prodotto alla lista desideri.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

    
        // Caricamento del sessionID.
        std::string sessionID = "";
        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare il sessionID
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id"));}  
        PQclear(res);

        if (rows != 1){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste " + in_nome_utente_compratore + " , poichè non è stato registrato, non può essere aggiunto il prodotto alla lista desideri";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            return;
        } 

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == ""){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + in_nome_utente_compratore + ", utente non loggato, non può essere aggiunto il prodotto alla lista desideri";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            return;
        }

        // Verifichiamo che l'utente si tratti di un utente compratore:
        std::string categoriaUtente = "";
        sprintf(sqlcmd, "SELECT categoriaUtente FROM Utente WHERE nome_utente = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare la categoria dell'utente.
        if (rows==1){ categoriaUtente = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));}  
        PQclear(res);

        if (categoriaUtente != "UtenteCompratore"){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "L utente " + in_nome_utente_compratore + " non è un utente compratore, perciò non può essere aggiunto il prodotto alla lita desideri";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            return;
        }

        // Controllo se il codice del prodotto da inserire nella lista desideri esiste nel database
        sprintf(sqlcmd, "SELECT * FROM Prodotto WHERE codProdotto = '%d'", in_codProdotto);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        // Se il numero di righe del risultato della query è < 1, allora non esiste il prodotto con quel codice.
        if (rows < 1){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Il prodotto con codice" + std::to_string(in_codProdotto) + " non esiste.";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);

            std::cout << "Il prodotto non esiste!" << std::endl;
            return;
        }

        // Controllo se il prodotto è stato già inserito dall'utente nella lista desideri:
        int codProdotto; 
        bool trovato=false;
        
        // Seleziono i codici dei prodotti nella lista desideri dell'utente:
        sprintf(sqlcmd, "SELECT codProdotto FROM ListaDesideri WHERE nome_utente_compratore = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        for (int i = 0; i < rows; i++)
        {
            codProdotto = atoi(PQgetvalue(res, i, PQfnumber(res, "codProdotto")));
            // Se il codice del prodotto i-esimo è uguale a quello del prodotto che l'utente vuole inserire, il prodotto già è presente nella lista desideri
            if (codProdotto == in_codProdotto){
                // Il prodotto è già stato inserito, perciò ne aumentiamo la quantità:
                // Prima carichiamo la quantità precedente:
                trovato = true;
                int quantitàPrecedente;
                sprintf(sqlcmd, "SELECT quantitàProd FROM ListaDesideri WHERE nome_utente_compratore = '%s' AND codProdotto = '%d'", in_nome_utente_compratore.c_str(),in_codProdotto);
                PGresult *res1 = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res1);
                quantitàPrecedente = atoi(PQgetvalue(res1, 0, PQfnumber(res1, "quantitàProd"))); 
                PQclear(res1);

                // Aumenta la quantità del prodotto da inserire
                quantitàPrecedente = quantitàPrecedente + 1;
                
                // Aggiorno la quantità del prodotto da inserire
                sprintf(sqlcmd, "UPDATE ListaDesideri set quantitàProd = '%d' WHERE nome_utente_compratore = '%s' AND codProdotto = '%d'", quantitàPrecedente, in_nome_utente_compratore.c_str(),in_codProdotto);
                PGresult *res2 = db1.ExecSQLcmd(sqlcmd);
                PQclear(res2);

                // Log 
                statoReq = statoRequisito::Success;
                messageLog = "Il prodotto con codice " +  std::to_string(in_codProdotto)  + " già esiste nella lista desideri, ne aggiungiamo la quantità per utente: " + in_nome_utente_compratore;
                InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

                // Anima l'oggetto:
                this->nome_utente_compratore = in_nome_utente_compratore;
                this->codice_prodotto = in_codProdotto;
                this->quantitàProdotti = quantitàPrecedente;

                // Interrompi il loop dopo aver aggiornato il prodotto
                break;
            }
            //break;

        }
         
        // Controlliamo se il prodotto non è nella lista desideri e dobbiamo inserirlo lo inseriamo per la prima volta 
        if (trovato == false){
             // Inseriamo il prodotto per la prima volta nella lista desideri:
            sprintf(sqlcmd, "INSERT INTO ListaDesideri (nome_utente_compratore, codProdotto, quantitàProd) VALUES ('%s', '%d', '%d')", in_nome_utente_compratore.c_str(), in_codProdotto, 1);
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);   

            // Anima l'oggetto:
            this->nome_utente_compratore = in_nome_utente_compratore;
            this->codice_prodotto = in_codProdotto;
            this->quantitàProdotti = 1;

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Inserimento del prodotto con codice " +  std::to_string(in_codProdotto)  + " nella lista desideri per utente: " + in_nome_utente_compratore;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }
           
    return;      
    }


    // Metodo per rimuovere un prodotto dalla lista desideri dato il nome dell'utente e il codice del prodotto.
    void remove_prodotto(Con2DB db1, std::string in_nome_utente_compratore,  int in_cod_prodotto){
        
        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Rimozione prodotto dalla lista dei desideri.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";


        // Caricamento del sessionID.
        std::string sessionID = "";
        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare il sessionID
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id"));}  
        PQclear(res);

        if (rows != 1){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste " + in_nome_utente_compratore + " , poichè non è stato registrato, non può essere rimosso il prodotto dalla lista desideri";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            return;
        } 

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == ""){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato, non può essere rimosso il prodotto dalla lista .", sessionID, nomeRequisito, statoReq);
            return;
        }

        // Verifichiamo che l'utente si tratti di un utente compratore:
        std::string categoriaUtente = "";
        sprintf(sqlcmd, "SELECT categoriaUtente FROM Utente WHERE nome_utente = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare la categoria dell'utente.
        if (rows==1){ categoriaUtente = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));}  
        PQclear(res);

        if (categoriaUtente != "UtenteCompratore"){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "L utente " + in_nome_utente_compratore + " non è un utente compratore, perciò non può essere rimosso il prodotto dalla lista desideri";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            return;
        }

        // Controlliamo se il codice del prodotto esiste all'interno della lista desideri dell'utente compratore
        sprintf(sqlcmd, "SELECT * FROM ListaDesideri WHERE nome_utente_compratore = '%s' AND codProdotto ='%d'", in_nome_utente_compratore.c_str(), in_cod_prodotto);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        // Se il numero di righe del risultato della query è < 1, allora non esiste il prodotto da eliminare.
        if (rows < 1){
            // Log dell'errore (il prodotto non esiste nella lista desideri dell'utente) e uscita dalla funzione
            std::cout << "La riga da eliminare non esiste!" << std::endl;

            // Log
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Il prodotto con codice " +  std::to_string(in_cod_prodotto)  + " non esiste";
            InsertToLogDB(db1, "ERROR", messageLog , sessionID, nomeRequisito, statoReq);
            return;
        }

        // Se il numero di righe del risultato della query è > 1, allora esiste il prodotto da eliminare e lo eliminiamo
        else{
            // Eliminazione del prodotto dalla lsta desideri dell'utente compratore.
            sprintf(sqlcmd, "DELETE FROM ListaDesideri WHERE nome_utente_compratore='%s' AND codProdotto='%d'", in_nome_utente_compratore.c_str(), in_cod_prodotto);
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Rimozione del prodotto con codice " +  std::to_string(in_cod_prodotto)  + " dalla lista desideri per utente: " + in_nome_utente_compratore;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }
    return;
    }
};

#endif // LISTA_DESIDERI_H
