// Product.h
#ifndef PRODOTTO_H
#define PRODOTTO_H

#include "../server-customer/src/ordine.h"
#include "getCurrentDateAsString.h"          // Migliore separazione delle responsabilità


class Prodotto
{
public:
    // Attributi per la classe Prodotto.
    int m_codProdotto;
    std::string m_nome;
    std::string m_categoria;
    float m_prezzoEuro;
    std::string m_descrizione;
    std::string m_aziendaProduzione;
    int m_numeroCopieDisponibili;

    Prodotto() : 
                m_codProdotto(-1),
                m_nome(""),
                m_categoria(""),
                m_prezzoEuro(-1.0),
                m_descrizione(""),
                m_aziendaProduzione(""),
                m_numeroCopieDisponibili(-1) {}

    Prodotto(int cod_prodotto, std::string nome, std::string categoria,
            float prezzo_euro, std::string descrizione, std::string azienda_produzione, int numero_copie_disponibili)

        : m_codProdotto(cod_prodotto), m_nome(nome), m_categoria(categoria),
          m_prezzoEuro(prezzo_euro), m_descrizione(descrizione), m_aziendaProduzione(azienda_produzione), m_numeroCopieDisponibili(numero_copie_disponibili) {}

    void mostraInformazioni()
    {
        std::cout << "Codice prodotto: " << m_codProdotto << std::endl;
        std::cout << "Nome: " << m_nome << std::endl;
        std::cout << "Categoria: " << m_categoria << std::endl;
        std::cout << "Prezzo in euro: " << m_prezzoEuro << std::endl;
        std::cout << "Descrizione: " << m_descrizione << std::endl;
        std::cout << "Azienda produzione: " << m_aziendaProduzione << std::endl;
        std::cout << "Numero copie disponibili: " << m_numeroCopieDisponibili << std::endl;
    }

    // Metodo utilizzato per permettere ad un utente fornitore di aggiungere un prodotto al backend.
    std::string aggiungi_prodotto_sito(Con2DB db1, std::string in_nome_utente_fornitore, std::string in_nome, std::string in_categoria, float in_prezzo_euro, std::string in_descrizione, std::string in_azienda_produzione, int in_numero_copie_disponibili)
    {
        std::string sessionID = "";

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Aggiunta prodotto al sito.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        // Caricamento del sessionID.
        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente_fornitore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare il sessionID
        if (rows == 1)
        {
            sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id"));
        }
        PQclear(res);

        // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
        if (rows == 0){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste " + in_nome_utente_fornitore + " , poichè non è stato registrato, non può essere aggiunto il prodotto nel sito .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }   

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + in_nome_utente_fornitore + ", utente non loggato, non può essere aggiunto il prodotto al sito";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }


        // Verifichiamo che l'utente si tratti di un utente fornitore:
        std::string categoriaUtente = "";
        sprintf(sqlcmd, "SELECT categoriaUtente FROM Utente WHERE nome_utente = '%s'", in_nome_utente_fornitore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare la categoria dell'utente.
        if (rows==1){ categoriaUtente = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));}  
        PQclear(res);

        if (categoriaUtente != "UtenteFornitore"){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "L utente " + in_nome_utente_fornitore + " non è un utente fornitore, perciò non può essere aggiunto il prodotto nel sito.";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }


        // Controlliamo se il prodotto che l'utente fornitore vuole inserire nel database è già presente o meno.
        // Se il prodotto è già presente dobbiamo solamente incrementare la quantità di copie, altrimenti dovremo inserirlo per la prima volta

        // Recupero del codice del prodotto tramite tutti i campi di input per controllare se è già present nel database
        sprintf(sqlcmd, "SELECT codProdotto FROM Prodotto WHERE nome='%s' AND categoria='%s' AND descrizione='%s' AND prezzoEuro='%f' AND nome_AziendaProduttrice='%s'",
                in_nome.c_str(), in_categoria.c_str(), in_descrizione.c_str(), in_prezzo_euro, in_azienda_produzione.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);
        std::cout << "Le righe dopo la query di codProdotto è: " << rows << std::endl;

        // Se il numero di righe del risultato della query è uguale a 1, allora il prodotto è già all'interno del database
        if (rows == 1)
        {
            // Il prodotto è già all'interno del backend, perciò dobbiamo solamente aumentarne la quantità di 1

            int numCopieDisponibili;

            // 1. Recuperiamo il numero di copie disponibili del prodotto
            sprintf(sqlcmd, "SELECT num_copie_dispo FROM Prodotto WHERE nome='%s' AND categoria='%s' AND descrizione='%s' AND prezzoEuro='%f' AND nome_AziendaProduttrice='%s'",
                    in_nome.c_str(), in_categoria.c_str(), in_descrizione.c_str(), in_prezzo_euro, in_azienda_produzione.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
        
            // Se il numero di righe del risultato della query è 1, allora può essere recuperato il numero di copie disponibili.
            if (rows == 1)
            {
                numCopieDisponibili = atoi(PQgetvalue(res, 0, PQfnumber(res, "num_copie_dispo")));

                PQclear(res);

                // 2. Incrementiamo la variabile di 1, perchè dovrà essere aggiornata nel db
                numCopieDisponibili = numCopieDisponibili + 1;

                // 3. Eseguiamo un UPDATE per modificare il valore delle copie disponibili, appena aggiornato.
                sprintf(sqlcmd, "UPDATE Prodotto set num_copie_dispo = '%d' WHERE nome='%s' AND categoria='%s' AND descrizione='%s' AND prezzoEuro='%f' AND nome_AziendaProduttrice='%s'",
                        numCopieDisponibili, in_nome.c_str(), in_categoria.c_str(), in_descrizione.c_str(), in_prezzo_euro, in_azienda_produzione.c_str());
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                // Log
                statoReq = statoRequisito::Success;
                messageLog = "Aumentata quantità del prodotto con nome " + in_nome  + " da parte di " + in_nome_utente_fornitore;
                InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

                // Animiamo l'oggetto Prodotto:
                // Esegui una query SELECT per ottenere l'ultimo codProdotto inserito nella tabella Prodotto:
                // 1. Selezioniamo tutti i codProdotto dalla tabella Prodotto:
                sprintf(sqlcmd, "SELECT codProdotto FROM Prodotto");
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);
                // 2. Prendiamo l'ultimo id
                this->m_codProdotto = atoi(PQgetvalue(res, rows - 1, 0));
                PQclear(res);

                // Animo l'oggetto
                *this = Prodotto(m_codProdotto ,in_nome, in_categoria, in_prezzo_euro, in_descrizione, in_azienda_produzione, in_numero_copie_disponibili);
            }
        }

        // Se il numero di righe del risultato della query è 0, cioè il prodotto non è all'interno del database dobbiamo aggiungerlo per la prima volta:
        if (rows == 0)
        {
            // Il prodotto non è all'interno del database, inseriamolo per la prima volta
            sprintf(sqlcmd, "INSERT INTO Prodotto(codProdotto, nome, categoria,descrizione, prezzoEuro, nome_AziendaProduttrice, num_copie_dispo) VALUES (DEFAULT, '%s', '%s', '%s', '%f', '%s', '%d')",
                    in_nome.c_str(), in_categoria.c_str(), in_descrizione.c_str(), in_prezzo_euro, in_azienda_produzione.c_str(), in_numero_copie_disponibili);
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Prodotto con nome " + in_nome +  " inserito nel sito da parte di " + in_nome_utente_fornitore;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);

            // Animiamo l'oggetto Prodotto:
            // Esegui una query SELECT per ottenere l'ultimo codProdotto inserito nella tabella Prodotto:
            // 1. Selezioniamo tutti i codProdotto dalla tabella Prodotto:
            sprintf(sqlcmd, "SELECT codProdotto FROM Prodotto");
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            // 2. Prendiamo l'ultimo id
            this->m_codProdotto = atoi(PQgetvalue(res, rows - 1, 0));
            PQclear(res);

            // Animo l'oggetto
            *this = Prodotto(this->m_codProdotto, in_nome, in_categoria, in_prezzo_euro, in_descrizione, in_azienda_produzione, in_numero_copie_disponibili);
        }

        result = messageLog;
        return result;
    }



    // Metodo utilizzato per permettere ad un utente fornitore di rimuovere un prodotto dal backend.
    std::string rimuovi_prodotto_sito(Con2DB db1, std::string in_nome_utente_fornitore, int in_cod_prodotto)
    {
        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Rimozione prodotto dal sito.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        std::string sessionID = "";

        // Caricamento del sessionID.
        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente_fornitore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare il sessionID
        if (rows == 1)
        {
            //sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));
            sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id"));
        }
        PQclear(res);

        // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
        if (rows == 0){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste " + in_nome_utente_fornitore + " , poichè non è stato registrato, non può essere rimosso il prodotto nel sito .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }   

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + in_nome_utente_fornitore + ", utente non loggato, non può essere rimosso il prodotto dal sito";
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato, non si può rimuovere un prodotto nel sito .", sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifichiamo che l'utente si tratti di un utente fornitore:
        std::string categoriaUtente = "";
        sprintf(sqlcmd, "SELECT categoriaUtente FROM Utente WHERE nome_utente = '%s'", in_nome_utente_fornitore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare la categoria dell'utente.
        if (rows==1){ categoriaUtente = PQgetvalue(res, 0, PQfnumber(res, "categoriaUtente"));}  
        PQclear(res);

        if (categoriaUtente != "UtenteFornitore"){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "L utente " + in_nome_utente_fornitore + " non è un utente fornitore, perciò non può essere rimosso il prodotto nel sito.";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }


        // Verifica che il prodotto da rimuovere esista
        sprintf(sqlcmd, "SELECT * FROM Prodotto WHERE codProdotto = '%d'", in_cod_prodotto);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        // Se il numero di righe del risultato della query è minore di 1 non c'è nessun prodotto con quel codice, perciò non può essere rimosso.
        if (rows < 1)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Il prodotto con codice " + std::to_string(in_cod_prodotto) + " non esiste";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);

            std::cout << "La riga da eliminare non esiste!" << std::endl;
            
            result = messageLog;
            return result;
        }

        // Se il numero di righe del risultato della query è 1 il prodotto può essere rimosso
        if (rows == 1)
        {
            // Eliminazione del prodotto dal carrello dell'utente compratore.
            sprintf(sqlcmd, "DELETE FROM Prodotto WHERE codProdotto = '%d'", in_cod_prodotto);
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Eliminato prodotto con codice " + std::to_string(in_cod_prodotto) + " da " + in_nome_utente_fornitore;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }

    result = messageLog;
    return result;
    }

    
    
    // Metodo utilizzato per permettere ad un utente compratore di ricercare un prodotto
    std::string ricerca_mostra_Prodotto(Con2DB db1, std::string in_nome_utente_compratore, std::string in_nome_prodotto)
    {

        std::string sessionID = "";

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Ricerca e mostra informazioni del prodotto.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        // Caricamento del sessionID.
        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare il sessionID
        if (rows == 1)
        {
            sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id"));
        }
        PQclear(res);

        // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
        if (rows == 0){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste " + in_nome_utente_compratore + " , poichè non è stato registrato, non può essere ricercato il prodotto .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }   

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + in_nome_utente_compratore + ", utente non loggato, non può essere ricercato il prodotto";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
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
            messageLog = "L utente " + in_nome_utente_compratore + " non è un utente compratore, perciò non può essere ricercato il prodotto.";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }



        // Effettuiamo la ricerca del prodotto nel database nella tabella Prodotto
        sprintf(sqlcmd, "SELECT * FROM Prodotto WHERE nome = '%s'", in_nome_prodotto.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        printf("Rows: %d", rows);

        // Se il numero di righe del risultato della query è minore di 1 non c'è nessun prodotto con quel codice, perciò non può essere effettuata la ricerca.
        if (rows < 1)
        {

            PQclear(res);

            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste nessun prodotto che si sta ricercando con nome" + in_nome_prodotto;
            InsertToLogDB(db1, "ERROR", messageLog , sessionID, nomeRequisito, statoReq);

            std::cout << "Errore: Non esiste il prodotto che si sta ricercando:" << std::endl;

            result = messageLog;
            return result;
        }

        // Se il numero di righe del risultato della query è 1 effettuiamo la ricerca.
        if (rows == 1)
        {
            // Animo l'oggetto prodotto:
            this->m_codProdotto = atoi(PQgetvalue(res, 0, PQfnumber(res, "codProdotto")));
            this->m_nome = PQgetvalue(res, 0, PQfnumber(res, "nome"));
            this->m_categoria = PQgetvalue(res, 0, PQfnumber(res, "categoria"));
            this->m_descrizione = PQgetvalue(res, 0, PQfnumber(res, "descrizione"));
            this->m_prezzoEuro = atof(PQgetvalue(res, 0, PQfnumber(res, "prezzoEuro")));
            this->m_aziendaProduzione = PQgetvalue(res, 0, PQfnumber(res, "nome_AziendaProduttrice"));
            this->m_numeroCopieDisponibili = atoi(PQgetvalue(res, 0, PQfnumber(res, "num_copie_dispo")));

            // Mostro le informazioni del Prodotto;
            std::cout << "Codice prodotto:" << m_codProdotto << std::endl;
            printf("\n");
            std::cout << "Nome prodotto:" << m_nome << std::endl;
            printf("\n");
            std::cout << "Categoria prodotto: " << m_categoria << std::endl;
            printf("\n");
            std::cout << "Descrizione: " << m_descrizione << std::endl;
            printf("\n");
            std::cout << "Prezzo in euro: " << m_prezzoEuro << std::endl;
            printf("\n");
            std::cout << "Azienda di produzione: " << m_aziendaProduzione << std::endl;
            printf("\n");
            std::cout << "Numero delle copie disponibili: " << m_numeroCopieDisponibili << std::endl;
            printf("\n");

            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Visione del prodotto ricercato con nome" + in_nome_prodotto + " da parte dell utente compratore " + in_nome_utente_compratore;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }

        if (rows > 1){
            for (int i = 0; i < rows; i++)
            {
                // Animo l'oggetto prodotto:
                this->m_codProdotto = atoi(PQgetvalue(res, i, PQfnumber(res, "codProdotto")));
                this->m_nome = PQgetvalue(res, i, PQfnumber(res, "nome"));
                this->m_categoria = PQgetvalue(res, i, PQfnumber(res, "categoria"));
                this->m_descrizione = PQgetvalue(res, i, PQfnumber(res, "descrizione"));
                this->m_prezzoEuro = atof(PQgetvalue(res, i, PQfnumber(res, "prezzoEuro")));
                this->m_aziendaProduzione = PQgetvalue(res, i, PQfnumber(res, "nome_AziendaProduttrice"));
                this->m_numeroCopieDisponibili = atoi(PQgetvalue(res, i, PQfnumber(res, "num_copie_dispo")));

                // Mostro le informazioni del Prodotto;
                std::cout << "Codice prodotto:" << m_codProdotto << std::endl;
                printf("\n");
                std::cout << "Nome prodotto:" << m_nome << std::endl;
                printf("\n");
                std::cout << "Categoria prodotto: " << m_categoria << std::endl;
                printf("\n");
                std::cout << "Descrizione: " << m_descrizione << std::endl;
                printf("\n");
                std::cout << "Prezzo in euro: " << m_prezzoEuro << std::endl;
                printf("\n");
                std::cout << "Azienda di produzione: " << m_aziendaProduzione << std::endl;
                printf("\n");
                std::cout << "Numero delle copie disponibili: " << m_numeroCopieDisponibili << std::endl;
                printf("\n");

                
            }
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Visione dei prodotti ricercato con nome" + in_nome_prodotto + " da parte dell utente compratore " + in_nome_utente_compratore;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }

        result = messageLog;
        return result;
    }





    // Metodo utilizzato per permettere a un utente compratore di acquistare un prodotto specificando il codice del prodotto e le informazioni per la spedizione.
    std::string acquistaProdotto(Con2DB db1, std::string in_nome_utente_compratore, int in_cod_prodotto, std::string in_via_spedizione, std::string in_città_spedizione, std::string in_numero_civico_spedizione, std::string in_CAP_spedizione)
    {

        Ordine ordine;
        std::string dataOrdineEffettuato;
        StatoOrdine stato_ordine;
        int numeroCopieDisponibili;

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Acquisto Prodotto.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Dichiarazione variabile per il risultato dell'operazione.
        std::string result = "";

        // Caricamento del sessionID.
        std::string sessionID = "";
        sprintf(sqlcmd, "SELECT session_id FROM Utente WHERE nome_utente = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        // Se il numero di righe del risultato della query è 1, allora possiamo recuperare il sessionID
        if (rows == 1)
        {
            sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id"));
        }
        PQclear(res);

        // Se il numero di righe del risultato della query è 0, allora non esiste nessun utente con quel nome_utente.
        if (rows == 0){
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste " + in_nome_utente_compratore + " , poichè non è stato registrato, non può essere acquistato il prodotto .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }   
        

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste una sessionID per " + in_nome_utente_compratore + ", utente non loggato, non può essere acquistato il prodotto .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
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
            messageLog = "L utente " + in_nome_utente_compratore + " non è un utente compratore, perciò non può essere acquistato il prodotto.";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);
            
            result = messageLog;
            return result;
        }

        // Verifica che il prodotto esista
        sprintf(sqlcmd, "SELECT * FROM Prodotto WHERE codProdotto = '%d'", in_cod_prodotto);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        // Se il numero di righe del risultato della query è minore di 1 non c'è nessun prodotto con quel codice, perciò non può essere acquistato.
        if (rows < 1)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Il prodotto con codice " + std::to_string(in_cod_prodotto) + " da acquistare non esiste";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);

            std::cout << "La riga da eliminare non esiste!" << std::endl;

            result = messageLog;
            return result;
        }
        
        // Se il numero di righe del risultato della query è 1 c'è un prodotto con quel codice, perciò può essere acquistato.
        if (rows == 1){
            // Assegniamo alla variabile dataOrdineEffettuato la data in cui l'ordine è stato effettuato
            dataOrdineEffettuato = getCurrentDateAsString();

            // Inizialmente assegnamo lo stato della consegna a "InElaborazione":
            stato_ordine = StatoOrdine::InElaborazione;
            std::string statoOrdineStr = statoOrdineToString(stato_ordine);

            // Inseriamo un record relativo all'ordine effettuato dall'utente compratore
            sprintf(sqlcmd, "INSERT INTO Ordine (idOrdine, codProdotto, nome_utente_compratore, dataOrdineEffettuato, statoOrdine, viaSpedizione, cittaSpedizione, numCivSpedizione, CAPSpedizione) VALUES (DEFAULT, '%d', '%s', '%s', '%s','%s','%s','%s', '%s' )",
                    in_cod_prodotto, in_nome_utente_compratore.c_str(), dataOrdineEffettuato.c_str(), statoOrdineStr.c_str(), in_via_spedizione.c_str(), in_città_spedizione.c_str(), in_numero_civico_spedizione.c_str(), in_CAP_spedizione.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Esegui una query SELECT per ottenere l'ultimo ID inserito nella tabella Ordine:
            // 1. Selezioniamo tutti gli idOrdine dalla tabella Ordine:
            sprintf(sqlcmd, "SELECT idOrdine FROM Ordine");
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            // 2. Prendiamo l'ultimo id
            ordine.m_idOrdine = atoi(PQgetvalue(res, rows - 1, 0));
            PQclear(res);
            std::cout << "Id ordine: " << ordine.m_idOrdine << std::endl;

            // Animo l'oggetto Ordine
            ordine.m_codiceProdotto = in_cod_prodotto;
            ordine.m_nomeUtenteCompratore = in_nome_utente_compratore;
            ordine.m_DataOrdineEffettuato = dataOrdineEffettuato;
            ordine.impostaStato(StatoOrdine::InElaborazione);
            ordine.m_viaSpedizione = in_via_spedizione;
            ordine.m_cittàSpedizione = in_città_spedizione;
            ordine.m_numeroCivicoSpedizione = in_numero_civico_spedizione;
            ordine.m_CAPSpedizione = in_CAP_spedizione;


            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Utente " + in_nome_utente_compratore + " ha acquistato il prodotto con codice " + std::to_string(in_cod_prodotto) + ", ordine inserito nel db ";
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);



            // Dobbiamo sottrarre di 1 il numero di copie disponibili del prodotto.
            sprintf(sqlcmd, "SELECT num_copie_dispo FROM Prodotto WHERE codProdotto = '%d'", in_cod_prodotto);
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);

            // Se il numero di righe del risultato della query è 0 non c'è nessun prodotto con quel codice, perciò non può essere acquistato.
            if (rows == 0) {
                
                PQclear(res);

                // Log dell'errore e uscita dalla funzione
                statoReq = statoRequisito::NotSuccess;
                messageLog = "Il prodotto com codice: " + std::to_string(in_cod_prodotto)  + " da acquistare non esiste";
                InsertToLogDB(db1, "ERROR", messageLog , "", nomeRequisito, statoReq);

                std::cout << "La riga da eliminare non esiste!" << std::endl;

                result = messageLog;
                return result;
            }
            
            // Se il numero di righe del risultato della query è uguale ad 1 posso prendere la quantita di copie.
            if (rows == 1){
                numeroCopieDisponibili = atoi(PQgetvalue(res, 0, PQfnumber(res, "num_copie_dispo")));

                numeroCopieDisponibili = numeroCopieDisponibili - 1;

                // Aggiorniamo il record relativo al codice del prodotto con il nuovo numero di copie disponibili:
                sprintf(sqlcmd, "UPDATE Prodotto set num_copie_dispo='%d' WHERE codProdotto = '%d'", numeroCopieDisponibili, in_cod_prodotto);
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                // Log
                std::string nomeRequisito = "Aggiornamento numero copie disponibili prodotto.";
                statoReq = statoRequisito::Success;
                messageLog = "Aggiornata numero copie disponibili prodotto con codice " + std::to_string(in_cod_prodotto) + " dopo l acquisto dell utente " + in_nome_utente_compratore;
                InsertToLogDB(db1,"INFO", messageLog, "", nomeRequisito, statoReq);                
            }

        }

    result = "Utente " + in_nome_utente_compratore + " ha acquistato il prodotto con codice " + std::to_string(in_cod_prodotto) + ", ordine inserito nel db ";
    return result;
    }




};

#endif // PRODOTTO_H