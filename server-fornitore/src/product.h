// Product.h
#ifndef PRODUCT_H
#define PRODUCT_H

#include "main.h"
#include "ordine.h"

class Product {
    public:
    int cod_product;
    std::string nome;
    std::string categoria;
    float prezzo_euro;
    std::string descrizione;
    std::string azienda_produzione;
    int numero_copie_disponibili;

    Product(): 
        nome(""),
        categoria(""),
        prezzo_euro(-1.0),
        descrizione(""),
        azienda_produzione(""),
        numero_copie_disponibili(-1){}


    Product(std::string nome, std::string categoria, 
           float prezzo_euro, std::string descrizione, std::string azienda_produzione, int numero_copie_disponibili)
        : nome(nome), categoria(categoria),
          prezzo_euro(prezzo_euro), descrizione(descrizione), azienda_produzione(azienda_produzione), numero_copie_disponibili(numero_copie_disponibili) {}

    void mostraInformazioni() {
        std::cout << "Codice prodotto: " << cod_product << std::endl;
        std::cout << "Nome: " << nome << std::endl;
        std::cout << "Categoria: " << categoria << std::endl;
        std::cout << "Prezzo in euro: " << prezzo_euro << std::endl;
        std::cout << "Descrizione: " << descrizione << std::endl;
        std::cout << "Azienda produzione: " << azienda_produzione << std::endl;
        std::cout << "Numero copie disponibili: " << numero_copie_disponibili << std::endl;
    }


    void add_new_product(Con2DB db1, std::string in_nome_utente_fornitore ,std::string in_nome, std::string in_categoria, float in_prezzo_euro, std::string in_descrizione, std::string in_azienda_produzione, int in_numero_copie_disponibili){

        std::string sessionID = "";
    
        std::string nomeRequisito = "Aggiunta prodotto al sito.";
        statoRequisito statoReq = statoRequisito::Wait;


        *this = Product(in_nome, in_categoria, in_prezzo_euro, in_descrizione, in_azienda_produzione, in_numero_copie_disponibili);

        // Caricamento del sessionID utile per il log.
        sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", in_nome_utente_fornitore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));}  
        PQclear(res);                        

        if (sessionID == ""){
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non si può aggiungere un prodotto nel sito .", sessionID, nomeRequisito, statoReq);
            return;
        }


        // Se il prodotto inserito è già presente nella tabella Prodotto, allora dobbiamo solamente incrementare la quantità di copie, altrimenti dovremo inserirlo:
        sprintf(sqlcmd, "SELECT codProdotto FROM Prodotto WHERE nome='%s' AND categoria='%s' AND descrizione='%s' AND prezzoEuro='%f' AND nome_AziendaProduttrice='%s'" , 
        in_nome.c_str(), in_categoria.c_str(), in_descrizione.c_str(), in_prezzo_euro, in_azienda_produzione.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);
        std::cout << "Le righe dopo la query di codProdotto è: " << rows << std::endl; 

        if (rows >= 1){
            // Il prodotto è già all'interno del backend, perciò dobbiamo solamente aumentarne la quantità di 1
                // 1. Caricihiamo il numero di copie disponibili;
                // 2. Le incrementiamo di 1;
                // 3. Eseguiamo un UPDATE per modificare il valore delle copie disponibili;

                int numCopieDisponibili;

                // 1.
                sprintf(sqlcmd, "SELECT num_copie_dispo FROM Prodotto WHERE nome='%s' AND categoria='%s' AND descrizione='%s' AND prezzoEuro='%f' AND nome_AziendaProduttrice='%s'" , 
                in_nome.c_str(), in_categoria.c_str(), in_descrizione.c_str(), in_prezzo_euro, in_azienda_produzione.c_str());
                res = db1.ExecSQLtuples(sqlcmd);
                rows = PQntuples(res);
                
                if (rows == 1){
                    numCopieDisponibili = atoi(PQgetvalue(res, 0, PQfnumber(res, "num_copie_dispo")));

                    PQclear(res);

                    // 2.
                    numCopieDisponibili = numCopieDisponibili + 1;

                    // 3.
                    sprintf(sqlcmd, "UPDATE Prodotto set num_copie_dispo = '%d' WHERE nome='%s' AND categoria='%s' AND descrizione='%s' AND prezzoEuro='%f' AND nome_AziendaProduttrice='%s'" , 
                    numCopieDisponibili , in_nome.c_str(), in_categoria.c_str(), in_descrizione.c_str(), in_prezzo_euro, in_azienda_produzione.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res); 

                    statoReq = statoRequisito::Success;

                    InsertToLogDB(db1, "INFO", "Aumentata quantità del prodotto inserito", sessionID, nomeRequisito, statoReq);
                }
        }
        else{
            // Il prodotto non è all'interno del backend, inseriamolo:
            sprintf(sqlcmd, "INSERT INTO Prodotto(codProdotto, nome, categoria,descrizione, prezzoEuro, nome_AziendaProduttrice, num_copie_dispo) VALUES (DEFAULT, '%s', '%s', '%s', '%f', '%s', '%d')", 
                                                            in_nome.c_str(), in_categoria.c_str(), in_descrizione.c_str(), in_prezzo_euro, in_azienda_produzione.c_str(), in_numero_copie_disponibili);
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res); 

            statoReq = statoRequisito::Success;

            InsertToLogDB(db1, "INFO", "Prodotto inserito nel sito", sessionID, nomeRequisito, statoReq);
        }
        
            
    return;
    }


    void remove_prodotto(Con2DB db1, std::string in_nome_utente_fornitore, std::string in_nome_prodotto){
        
        std::string nomeRequisito = "Rimozione prodotto dal sito.";
        statoRequisito statoReq = statoRequisito::Wait;
        std::string sessionID = "";


        // Verifica se l'utente fornitore che vuole rimuovere il prodotto dal sito è effettivamente un utente fornitore:
        sprintf(sqlcmd, "SELECT * FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", in_nome_utente_fornitore);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);
        if (rows != 1){
            InsertToLogDB(db1, "ERROR", "Utente non fornitore vuole rimuovere il prodotto", "", nomeRequisito, statoReq);

            return;
        }

        // Caricamento del sessionID utile per il log.
        sprintf(sqlcmd, "SELECT session_id_f FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", in_nome_utente_fornitore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_f"));}  
        PQclear(res);     


        if (sessionID == ""){
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non si può rimuovere un prodotto nel sito .", sessionID, nomeRequisito, statoReq);
            return;
        }


        // Selezione del codice del prodotto tramite il suo nome:
        sprintf(sqlcmd, "SELECT codProdotto FROM Prodotto WHERE nome = '%s'", in_nome_prodotto);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        int codProdotto;
        if (rows == 1){codProdotto = atoi(PQgetvalue(res, 0, PQfnumber(res, "codProdotto")));}
        PQclear(res);
        
        
        sprintf(sqlcmd, "SELECT * FROM Prodotto WHERE codProdotto = '%d'", codProdotto);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);
        if (rows < 1){
            std::cout << "La riga da eliminare non esiste!" << std::endl;

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1, "ERROR", "Il prodotto da eliminare non esiste", "", nomeRequisito, statoReq);

            return;
        }
        else{
            // Eliminazione del prodotto dal carrello dell'utente compratore.
            sprintf(sqlcmd, "DELETE FROM Prodotto WHERE codProdotto = '%d'", codProdotto);
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);

            statoReq = statoRequisito::Success;
            InsertToLogDB(db1, "INFO", "Eliminato prodotto", "", nomeRequisito, statoReq);

        }
    return;
    }


    // Sistemare per aggiugnere l'utente che ricerca il prodotto
    void ricerca_mostra_Prodotto(Con2DB db1, std::string nomeProdotto){


        std::string nomeRequisito = "Ricerca e mostra informazioni del prodotto.";
        statoRequisito statoReq = statoRequisito::Wait;

        // Cerchiamo il prodotto nel database:
        sprintf(sqlcmd, "SELECT * FROM Prodotto WHERE nome = '%s'", nomeProdotto.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        //PQclear(res);
        // Il prodotto non è stato trovato
        if (rows < 1){
            std::cout << "Errore: Non esiste il prodotto che si sta ricercando:" << std::endl;

            statoReq = statoRequisito::NotSuccess;

            PQclear(res); 
            InsertToLogDB(db1, "ERROR", "Non esiste il prodotto che si sta ricercando", "", nomeRequisito, statoReq);
            return;
        }
        else{
            // Animo l'oggetto Product:
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

            statoReq = statoRequisito::Success;

            PQclear(res); 

            InsertToLogDB(db1, "INFO", "Visione del prodotto ricercato", "", nomeRequisito, statoReq);

            
            /*int numCols = PQnfields(res);
            for (int i = 0; i < rows; ++i) {
                std::cout << "Row " << i << ": ";
                for (int j = 0; j < numCols; ++j) {
                    std::cout << PQgetvalue(res, i, j) << ",   ";
                }
                std::cout << std::endl;
            }
            */
            
        }
        //PQclear(res); 

    return;
    }



    Ordine acquistaProdotto(Con2DB db1, std::string nomeUtenteCompratore, std::string nomeProdotto, std::string via_spedizione, std::string città_spedizione, std::string numero_civico_spedizione, std::string CAP_spedizione){

        Ordine ordine;
        std::string dataOrdineEffettuato;

        StatoOrdine stato_ordine;

        std::string nomeRequisito = "Acquisto Prodotto.";
        statoRequisito statoReq = statoRequisito::Wait;

        //std::string nomeDittaSpedizione;

        //printf("Nome prodotto: %s \n", nome);
        

        // Caricamento del sessionID utile per il log.
        std::string sessionID = "";
        sprintf(sqlcmd, "SELECT session_id_c FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", nomeUtenteCompratore.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows==1){ sessionID = PQgetvalue(res, 0, PQfnumber(res, "session_id_c"));}  
        PQclear(res);     


        if (sessionID == ""){
            InsertToLogDB(db1, "ERROR", "Non esiste una sessionID, utente non loggato o non registrato, non si può acquistare un prodotto.", sessionID, nomeRequisito, statoReq);
            return;
        }                   


        // Selezioniamo il codice del prodotto
        //sprintf(sqlcmd, "SELECT codProdotto FROM Prodotto WHERE nome='%s' AND categoria='%s' AND descrizione='%s' AND FLOAT8EQ(prezzoEuro, '%f') AND nome_AziendaProduttrice='%s'", 
        //                                                    nome.c_str(), categoria.c_str(), descrizione.c_str(), prezzo_euro, azienda_produzione.c_str());
        sprintf(sqlcmd, "SELECT codProdotto FROM Prodotto WHERE nome='%s'", nomeProdotto.c_str());
        
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows == 1){
            
            //Ottengo i vari dati per costruire l'ordine:

            cod_product = atoi(PQgetvalue(res, 0, PQfnumber(res, "codProdotto")));
            PQclear(res);


            dataOrdineEffettuato = getCurrentDateAsString();


            // Inizialmente assegnamo lo stato della consegna a "InElaborazione":
            stato_ordine = StatoOrdine::InElaborazione;
            std::string statoOrdineStr = statoOrdineToString(stato_ordine);

            // // Otteniamo il nome della ditta di spedizione:
            // sprintf(sqlcmd, "SELECT nome_DittaSpedizione FROM UtenteTrasportatore WHERE nome_utente_trasportatore='%s'", nomeUtenteTrasportatore.c_str());
            // res = db1.ExecSQLtuples(sqlcmd);
            // rows = PQntuples(res);

            // if (rows==1)  {   nomeDittaSpedizione = PQgetvalue(res, 0, PQfnumber(res, "nome_DittaSpedizione"));     }
            // else{
            //     std::cout << "Errore: Non è stato trovato l'utente che trasporterà il prodotto." << std::endl;
            //     return ordine;
            // }

            // Inseriamo i valori nel database:
            sprintf(sqlcmd, "INSERT INTO Ordine (idOrdine, codProdotto, nome_utente_compratore, dataOrdineEffettuato, statoOrdine, viaSpedizione, cittaSpedizione, numCivSpedizione, CAPSpedizione) VALUES (DEFAULT, '%d', '%s', '%s', '%s','%s','%s','%s', '%s' )", 
            cod_product, nomeUtenteCompratore.c_str(), dataOrdineEffettuato.c_str(), statoOrdineStr.c_str(), via_spedizione.c_str(),città_spedizione.c_str(), numero_civico_spedizione.c_str(), CAP_spedizione.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);     

            statoReq = statoRequisito::Success;

            InsertToLogDB(db1, "INFO", "Utente ha acquistato il prodotto, ordine inserito nel db", sessionID, nomeRequisito, statoReq);

            // Esegui una query SELECT per ottenere l'ultimo ID inserito nella tabella Ordine:
            // 1. Selezioniamo tutti gli idOrdine dalla tabella Ordine:
            sprintf(sqlcmd, "SELECT idOrdine FROM Ordine");
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            // 2. Prendiamo l'ultimo id
            ordine.identificatore_ordine = atoi(PQgetvalue(res, rows-1, 0));
            std::cout << "Id ordine: " << ordine.identificatore_ordine << std::endl; 
            
            ordine.codice_prodotto = cod_product;
            ordine.nome_uteCompratore = nomeUtenteCompratore;
            ordine.data_ordine_effettuato = dataOrdineEffettuato;
            ordine.impostaStato(StatoOrdine::InElaborazione);
            ordine.via_spedizione = via_spedizione;
            ordine.città_spedizione = città_spedizione;
            ordine.numero_civico_spedizione = numero_civico_spedizione;
            ordine.CAP_spedizione = CAP_spedizione;
        }
        else{
            std::cout << "Errore: il prodotto non è stato trovato!" << std::endl;

            statoReq = statoRequisito::NotSuccess;

            InsertToLogDB(db1, "ERROR", "Il prodotto non è stato trovato!", sessionID, nomeRequisito, statoReq);
            return ordine;
        }
    return ordine;
    }


    std::string statoOrdineToString(StatoOrdine stato) {
        switch (stato) {
            case StatoOrdine::InElaborazione:
                return "in elaborazione";
            case StatoOrdine::Spedito:
                return "spedito";
            case StatoOrdine::Annullato:
                return "annullato";
            default:
                return ""; // gestione degli errori o valori non validi
        }
    }



    std::string getCurrentDateAsString() {
    // Ottieni il tempo corrente
    std::time_t now = std::time(nullptr);

    // Converti il tempo corrente in una struct tm
    std::tm* timeinfo = std::localtime(&now);

    // Costruisci una stringa dalla data nel formato GG-MM-AAAA
    std::stringstream ss;
    ss << timeinfo->tm_mday << '-'
       << (timeinfo->tm_mon + 1) << '-'
       << (timeinfo->tm_year + 1900);

    return ss.str();
    }


};





#endif // PRODUCT_H