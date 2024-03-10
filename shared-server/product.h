// Product.h
#ifndef PRODUCT_H
#define PRODUCT_H

#include "../server-customer/src/main.h"
#include "../server-customer/src/ordine.h"
#include "getCurrentDateAsString.h"          // Migliore separazione delle responsabilità
#include "statoOrdineToString.h"


class Product
{
public:
    int cod_product;
    std::string nome;
    std::string categoria;
    float prezzo_euro;
    std::string descrizione;
    std::string azienda_produzione;
    int numero_copie_disponibili;

    Product() : nome(""),
                categoria(""),
                prezzo_euro(-1.0),
                descrizione(""),
                azienda_produzione(""),
                numero_copie_disponibili(-1) {}

    Product(std::string nome, std::string categoria,
            float prezzo_euro, std::string descrizione, std::string azienda_produzione, int numero_copie_disponibili)
        : nome(nome), categoria(categoria),
          prezzo_euro(prezzo_euro), descrizione(descrizione), azienda_produzione(azienda_produzione), numero_copie_disponibili(numero_copie_disponibili) {}

    void mostraInformazioni()
    {
        std::cout << "Codice prodotto: " << cod_product << std::endl;
        std::cout << "Nome: " << nome << std::endl;
        std::cout << "Categoria: " << categoria << std::endl;
        std::cout << "Prezzo in euro: " << prezzo_euro << std::endl;
        std::cout << "Descrizione: " << descrizione << std::endl;
        std::cout << "Azienda produzione: " << azienda_produzione << std::endl;
        std::cout << "Numero copie disponibili: " << numero_copie_disponibili << std::endl;
    }

    // Funzione utilizzata per permettere ad un utente fornitore di aggiungere un prodotto al backend.
    void add_new_product(Con2DB db1, std::string in_nome_utente_fornitore, std::string in_nome, std::string in_categoria, float in_prezzo_euro, std::string in_descrizione, std::string in_azienda_produzione, int in_numero_copie_disponibili)
    {

        std::string sessionID = "";

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Aggiunta prodotto al sito.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Caricamento del sessionID.
        sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", in_nome_utente_fornitore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows == 1)
        {
            sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));
        }
        PQclear(res);

        if (rows != 1){
            // Log dell'errore e uscita dalla funzione
            messageLog = "Non esiste " + in_nome_utente_fornitore + " , poichè non è stato registrato, non può essere aggiunto il prodotto nel sito .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            return;
        }   

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato, non si può aggiungere un prodotto nel sito .", sessionID, nomeRequisito, statoReq);
            return;
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

        // Se il numero di righe del risultato della query è maggiore o uguale a 1, allora il prodotto è già all'interno del database
        if (rows >= 1)
        {
            // Il prodotto è già all'interno del backend, perciò dobbiamo solamente aumentarne la quantità di 1

            int numCopieDisponibili;

            // 1. Recuperiamo il numero di copie disponibili del prodotto
            sprintf(sqlcmd, "SELECT num_copie_dispo FROM Prodotto WHERE nome='%s' AND categoria='%s' AND descrizione='%s' AND prezzoEuro='%f' AND nome_AziendaProduttrice='%s'",
                    in_nome.c_str(), in_categoria.c_str(), in_descrizione.c_str(), in_prezzo_euro, in_azienda_produzione.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);

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

                // Animo l'oggetto
                *this = Product(in_nome, in_categoria, in_prezzo_euro, in_descrizione, in_azienda_produzione, in_numero_copie_disponibili);
            }
        }

        // Se il numero di righe del risultato della query non è maggiore o uguale a 1, cioè il prodotto non è all'interno del database dobbiamo aggiungerlo per la prima volta:
        else
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

            // Animo l'oggetto
            *this = Product(in_nome, in_categoria, in_prezzo_euro, in_descrizione, in_azienda_produzione, in_numero_copie_disponibili);
        }

        return;
    }

    // Funzione utilizzata per permettere ad un utente fornitore di rimuovere un prodotto
    void remove_prodotto(Con2DB db1, std::string in_nome_utente_fornitore, int codProdotto)
    {

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Rimozione prodotto dal sito.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        std::string sessionID = "";

        // Verifica se l'utente che vuole rimuovere il prodotto dal sito è effettivamente un utente fornitore:
        sprintf(sqlcmd, "SELECT * FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", in_nome_utente_fornitore);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        // Se il numero di righe del risultato della query è diverso da 1 non c'è nessun utente fornitore con quel nome utente.
        if (rows != 1)
        {
            // Log dell'errore e uscita dalla funzione.
            messageLog = "Utente fornitore " + in_nome_utente_fornitore + " non trovato";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);

            return;
        }

        // Caricamento del sessionID.
        sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", in_nome_utente_fornitore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows == 1)
        {
            sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));
        }
        PQclear(res);

        if (rows != 1){
            // Log dell'errore e uscita dalla funzione
            messageLog = "Non esiste " + in_nome_utente_fornitore + " , poichè non è stato registrato, non può essere rimosso il prodotto nel sito .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            return;
        }   

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato, non si può rimuovere un prodotto nel sito .", sessionID, nomeRequisito, statoReq);
            return;
        }

        // Verifica che il prodotto da rimuovere esista
        sprintf(sqlcmd, "SELECT * FROM Prodotto WHERE codProdotto = '%d'", codProdotto);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        // Se il numero di righe del risultato della query è minore di 1 non c'è nessun prodotto con quel codice, perciò non può essere rimosso.
        if (rows < 1)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Il prodotto con codice " + std::to_string(codProdotto) + " non esiste";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);

            std::cout << "La riga da eliminare non esiste!" << std::endl;

            return;
        }

        // Se il numero di righe del risultato della query è maggiore o uguale a 1 il prodotto può essere rimosso
        else
        {
            // Eliminazione del prodotto dal carrello dell'utente compratore.
            sprintf(sqlcmd, "DELETE FROM Prodotto WHERE codProdotto = '%d'", codProdotto);
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Eliminato prodotto con codice " + std::to_string(codProdotto) + " da " + in_nome_utente_fornitore;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }
        return;
    }

    
    
    // Metodo utilizzato per permettere ad un utente compratore di ricercare un prodotto
    void ricerca_mostra_Prodotto(Con2DB db1, std::string in_nome_utente_compratore, int codProdotto)
    {

        std::string sessionID = "";

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Ricerca e mostra informazioni del prodotto.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Caricamento del sessionID.
        sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", in_nome_utente_compratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows == 1)
        {
            sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));
        }
        PQclear(res);

        if (rows != 1){
            // Log dell'errore e uscita dalla funzione
            messageLog = "Non esiste " + in_nome_utente_compratore + " , poichè non è stato registrato, non può essere ricercato il prodotto .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            return;
        }   

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato, non si può rimuovere un prodotto nel sito .", sessionID, nomeRequisito, statoReq);
            return;
        }

        // Effettuiamo la ricerca del prodotto nel database nella tabella Prodotto
        sprintf(sqlcmd, "SELECT * FROM Prodotto WHERE codProdotto = '%d'", codProdotto);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe del risultato della query è minore di 1 non c'è nessun prodotto con quel codice, perciò non può essere effettuata la ricerca.
        if (rows < 1)
        {

            PQclear(res);

            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Non esiste il prodotto che si sta ricercando con codice" + std::to_string(codProdotto);
            InsertToLogDB(db1, "ERROR", messageLog , sessionID, nomeRequisito, statoReq);

            std::cout << "Errore: Non esiste il prodotto che si sta ricercando:" << std::endl;

            return;
        }

        // Se il numero di righe del risultato della query è minore di 1 non c'è nessun prodotto con quel codice, perciò non può essere effettuata la ricerca.
        else
        {
            // Animo l'oggetto prodotto:
            this->cod_product = codProdotto;
            this->nome = PQgetvalue(res, 0, PQfnumber(res, "nome"));
            this->categoria = PQgetvalue(res, 0, PQfnumber(res, "categoria"));
            this->descrizione = PQgetvalue(res, 0, PQfnumber(res, "descrizione"));
            this->prezzo_euro = atof(PQgetvalue(res, 0, PQfnumber(res, "prezzoEuro")));
            this->azienda_produzione = PQgetvalue(res, 0, PQfnumber(res, "nome_AziendaProduttrice"));
            this->numero_copie_disponibili = atoi(PQgetvalue(res, 0, PQfnumber(res, "num_copie_dispo")));

            // Mostro le informazioni del Prodotto;
            std::cout << "Nome prodotto:" << nome << std::endl;
            std::cout << "Categoria prodotto: " << categoria << std::endl;
            std::cout << "Descrizione: " << descrizione << std::endl;
            std::cout << "Prezzo in euro: " << prezzo_euro << std::endl;
            std::cout << "Azienda di produzione: " << azienda_produzione << std::endl;
            std::cout << "Numero delle copie disponibili: " << numero_copie_disponibili << std::endl;

            PQclear(res);

            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Visione del prodotto ricercato con codice" + std::to_string(codProdotto) + " da parte dell utente compratore " + in_nome_utente_compratore;
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);
        }

        return;
    }

    // Metodo utilizzato per permettere a un utente compratore di acquistare un prodotto specificando il codice del prodotto e le informazioni per la spedizione.
    Ordine acquistaProdotto(Con2DB db1, std::string nomeUtenteCompratore, int codProdotto, std::string via_spedizione, std::string città_spedizione, std::string numero_civico_spedizione, std::string CAP_spedizione)
    {

        Ordine ordine;
        std::string dataOrdineEffettuato;
        StatoOrdine stato_ordine;
        int numeroCopieDisponibili;

        // Definizione di alcune variabili per il logging
        std::string nomeRequisito = "Acquisto Prodotto.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string messageLog = "";

        // Caricamento del sessionID.
        std::string sessionID = "";
        sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtenteCompratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        printf("Rows prima: %d", rows);
        if (rows == 1)
        {
            sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));
        }
        PQclear(res);
        printf("Rows dopo PQclear: %d", rows);

        if (rows != 1){
            // Log dell'errore e uscita dalla funzione
            messageLog = "Non esiste " + nomeUtenteCompratore + " , poichè non è stato registrato, non può essere acquistato il prodotto .";
            InsertToLogDB(db1, "ERROR", messageLog, sessionID, nomeRequisito, statoReq);
            return ordine;
        }   
        



        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato, non si può acquistare un prodotto.", sessionID, nomeRequisito, statoReq);
            return ordine;
        }

        // Verifica che il prodotto esista
        sprintf(sqlcmd, "SELECT * FROM Prodotto WHERE codProdotto = '%d'", codProdotto);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        // Se il numero di righe del risultato della query è minore di 1 non c'è nessun prodotto con quel codice, perciò non può essere acquistato.
        if (rows < 1)
        {
            // Log dell'errore e uscita dalla funzione
            statoReq = statoRequisito::NotSuccess;
            messageLog = "Il prodotto con codice " + std::to_string(codProdotto) + " da acquistare non esiste";
            InsertToLogDB(db1, "ERROR", messageLog, "", nomeRequisito, statoReq);

            std::cout << "La riga da eliminare non esiste!" << std::endl;

            return ordine;
        }
        
        // Se il numero di righe del risultato della query è uguale o maggiore di 1 c'è un prodotto con quel codice, perciò può essere acquistato.
        else{
            // Assegniamo alla variabile dataOrdineEffettuato la data in cui l'ordine è stato effettuato
            dataOrdineEffettuato = getCurrentDateAsString();

            // Inizialmente assegnamo lo stato della consegna a "InElaborazione":
            stato_ordine = StatoOrdine::InElaborazione;
            std::string statoOrdineStr = statoOrdineToString(stato_ordine);

            // Inseriamo un record relativo all'ordine effettuato dall'utente compratore
            sprintf(sqlcmd, "INSERT INTO Ordine (idOrdine, codProdotto, nome_utente_compratore, dataOrdineEffettuato, statoOrdine, viaSpedizione, cittaSpedizione, numCivSpedizione, CAPSpedizione) VALUES (DEFAULT, '%d', '%s', '%s', '%s','%s','%s','%s', '%s' )",
                    codProdotto, nomeUtenteCompratore.c_str(), dataOrdineEffettuato.c_str(), statoOrdineStr.c_str(), via_spedizione.c_str(), città_spedizione.c_str(), numero_civico_spedizione.c_str(), CAP_spedizione.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            // Esegui una query SELECT per ottenere l'ultimo ID inserito nella tabella Ordine:
            // 1. Selezioniamo tutti gli idOrdine dalla tabella Ordine:
            sprintf(sqlcmd, "SELECT idOrdine FROM Ordine");
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            // 2. Prendiamo l'ultimo id
            ordine.identificatore_ordine = atoi(PQgetvalue(res, rows - 1, 0));
            PQclear(res);
            std::cout << "Id ordine: " << ordine.identificatore_ordine << std::endl;

            // Animo l'oggetto Ordine
            ordine.codice_prodotto = codProdotto;
            ordine.nome_uteCompratore = nomeUtenteCompratore;
            ordine.data_ordine_effettuato = dataOrdineEffettuato;
            ordine.impostaStato(StatoOrdine::InElaborazione);
            ordine.via_spedizione = via_spedizione;
            ordine.città_spedizione = città_spedizione;
            ordine.numero_civico_spedizione = numero_civico_spedizione;
            ordine.CAP_spedizione = CAP_spedizione;


            // Log
            statoReq = statoRequisito::Success;
            messageLog = "Utente " + nomeUtenteCompratore + " ha acquistato il prodotto con codice " + std::to_string(codProdotto) + ", ordine inserito nel db ";
            InsertToLogDB(db1, "INFO", messageLog, sessionID, nomeRequisito, statoReq);



            // Dobbiamo sottrarre di 1 il numero di copie disponibili del prodotto.
            sprintf(sqlcmd, "SELECT num_copie_dispo FROM Prodotto WHERE codProdotto = '%d'", codProdotto);
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);

            // Se il numero di righe del risultato della query è diverso di 1 non c'è nessun prodotto con quel codice, perciò non può essere acquistato.
            if (rows != 1) {
                
                PQclear(res);

                // Log dell'errore e uscita dalla funzione
                statoReq = statoRequisito::NotSuccess;
                messageLog = "Il prodotto com codice: " + std::to_string(codProdotto)  + " da acquistare non esiste";
                InsertToLogDB(db1, "ERROR", messageLog , "", nomeRequisito, statoReq);

                std::cout << "La riga da eliminare non esiste!" << std::endl;

                return ordine;
            }
            
            // Se il numero di righe del risultato della query è uguale ad 1 posso prendere la quantita di copie.
            else{
                numeroCopieDisponibili = atoi(PQgetvalue(res, 0, PQfnumber(res, "num_copie_dispo")));

                numeroCopieDisponibili = numeroCopieDisponibili - 1;

                // Aggiorniamo il record relativo al codice del prodotto con il nuovo numero di copie disponibili:
                sprintf(sqlcmd, "UPDATE Prodotto set num_copie_dispo='%d' WHERE codProdotto = '%d'", numeroCopieDisponibili, codProdotto);
                res = db1.ExecSQLcmd(sqlcmd);
                PQclear(res);

                // Log
                std::string nomeRequisito = "Aggiornamento numero copie disponibili prodotto.";
                statoReq = statoRequisito::Success;
                messageLog = "Aggiornata numero copie disponibili prodotto con codice " + codProdotto;
                InsertToLogDB(db1,"INFO", messageLog, sessionID, nomeRequisito, statoReq);                
            }

        }

    return ordine;
    }




};

#endif // PRODUCT_H