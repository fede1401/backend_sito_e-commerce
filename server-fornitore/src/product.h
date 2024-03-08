// Product.h
#ifndef PRODUCT_H
#define PRODUCT_H

#include "main.h"
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


};

#endif // PRODUCT_H