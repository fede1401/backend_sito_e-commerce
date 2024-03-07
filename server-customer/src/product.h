// Product.h
#ifndef PRODUCT_H
#define PRODUCT_H

#include "main.h"
#include "ordine.h"
#include "../../shared-server/getCurrentDateAsString.h"          // Migliore separazione delle responsabilità
#include "../../shared-server/statoOrdineToString.h"


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

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non si può rimuovere un prodotto nel sito .", sessionID, nomeRequisito, statoReq);
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
            cod_product = codProdotto;
            nome = PQgetvalue(res, 0, PQfnumber(res, "nome"));
            categoria = PQgetvalue(res, 0, PQfnumber(res, "categoria"));
            descrizione = PQgetvalue(res, 0, PQfnumber(res, "descrizione"));
            prezzo_euro = atof(PQgetvalue(res, 0, PQfnumber(res, "prezzoEuro")));
            azienda_produzione = PQgetvalue(res, 0, PQfnumber(res, "nome_AziendaProduttrice"));
            numero_copie_disponibili = atoi(PQgetvalue(res, 0, PQfnumber(res, "num_copie_dispo")));

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
        if (rows == 1)
        {
            sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));
        }
        PQclear(res);

        // Verifica se l'utente è loggato e ha una sessionID valida
        if (sessionID == "")
        {
            // Log dell'errore e uscita dalla funzione
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non si può acquistare un prodotto.", sessionID, nomeRequisito, statoReq);
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