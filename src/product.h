// Product.h
#ifndef PRODUCT_H
#define PRODUCT_H


#include <iostream>
#include <string>
#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db/pgsql.h"
#include "ordine.h"
#include <ctime>

//PGresult *res;
//char sqlcmd[1000];

//int rows, k;

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


    void add_new_product(std::string in_nome, std::string in_categoria, float in_prezzo_euro, std::string in_descrizione, std::string in_azienda_produzione, int in_numero_copie_disponibili){

        // Connessione al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");
        std::cout << "Connessione al database avvenuta con successo." << std::endl;
            
            /*
            ///////////////////////////////////// 
            // Troviamo l'id dell'azienda di produzione che sarà inserita nel database
            // SELECT:
            int idAziendaProd;
            sprintf(sqlcmd, "SELECT idAziendaProd FROM AziendaProd WHERE nome = '%s'", in_azienda_produzione.c_str());
            res = db1.ExecSQLtuples(sqlcmd);
            rows = PQntuples(res);
            if (rows == 1) { 
                idAziendaProd = atoi(PQgetvalue(res, 0, PQfnumber(res, "idAziendaProd"))); 
            }
            else{
                std::cout << "L'azienda non esiste." << std::endl;
                return;
            }
            PQclear(res);  
            /////////////////////////////////////
            */
            

        // Assicuriamoci che l'utente che inserirà il prodotto nel sito è un Utente Fornitore
        sprintf(sqlcmd, "SELECT nome_utente_fornitore FROM UtenteFornitore WHERE nome_AziendaProduttrice = '%s'", in_azienda_produzione.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows == 1) { 
            std::cout << "L'utente che inserisce il prodotto nel sito è un utente fornitore" << std::endl;
        }
        else{
            std::cout << "L'utente che inserisce il prodotto nel sito NON è un utente fornitore" << std::endl;
            return;
        }
        PQclear(res);  

        *this = Product(in_nome, in_categoria, in_prezzo_euro, in_descrizione, in_azienda_produzione, in_numero_copie_disponibili);


        // Se il prodotto inserito è già presente nella tabella Prodotto, allora dobbiamo solamente incrementare la quantità di copie, altrimenti dovremo inserirlo:
        sprintf(sqlcmd, "SELECT codProdotto FROM Prodotto WHERE nome='%s' AND categoria='%s' AND descrizione='%s' AND prezzoEuro='%f' AND nome_AziendaProduttrice='%s'" , 
        in_nome.c_str(), in_categoria.c_str(), in_descrizione.c_str(), in_prezzo_euro, in_azienda_produzione.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

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

                    // 2.
                    numCopieDisponibili = numCopieDisponibili + 1;

                    // 3.
                    sprintf(sqlcmd, "UPDATE Prodotto set num_copie_dispo = '%d' WHERE nome='%s' AND categoria='%s' AND descrizione='%s' AND prezzoEuro='%f' AND nome_AziendaProduttrice='%s'" , 
                    numCopieDisponibili , in_nome.c_str(), in_categoria.c_str(), in_descrizione.c_str(), in_prezzo_euro, in_azienda_produzione.c_str());
                    res = db1.ExecSQLcmd(sqlcmd);
                    PQclear(res); 
                }
        }
        else{
            // Il prodotto non è all'interno del backend, inseriamolo:
            sprintf(sqlcmd, "INSERT INTO Prodotto(codProdotto, nome, categoria,descrizione, prezzoEuro, nome_AziendaProduttrice, num_copie_dispo) VALUES (DEFAULT, '%s', '%s', '%s', '%f', '%s', '%d')", 
                                                            in_nome.c_str(), in_categoria.c_str(), in_descrizione.c_str(), in_prezzo_euro, in_azienda_produzione.c_str(), in_numero_copie_disponibili);
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res); 
        }
        
            
    return;
    }


    void ricerca_mostra_Prodotto(std::string nomeProdotto){

        // Connession al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");


        // Cerchiamo il prodotto nel database:
        sprintf(sqlcmd, "SELECT * FROM Prodotto WHERE nome = '%s'", nomeProdotto.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Il prodotto non è stato trovato
        if (rows < 1){
            std::cout << "Errore: Non esiste il prodotto che si sta ricercando:" << std::endl;
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
        PQclear(res); 

    return;
    }



    Ordine acquistaProdotto(std::string nomeUtenteCompratore, std::string via_spedizione, std::string città_spedizione, std::string numero_civico_spedizione){

        Ordine ordine;
        std::string dataOrdineEffettuato;

        StatoOrdine stato_ordine;

        //std::string nomeDittaSpedizione;

        
        // Connession al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        // Selezioniamo il codice del prodotto
        sprintf(sqlcmd, "SELECT codProdotto FROM Prodotto WHERE nome='%s' AND categoria='%s' AND descrizione='%s' AND FLOAT8EQ(prezzoEuro, '%f') AND nome_AziendaProduttrice='%s'", 
                                                            nome.c_str(), categoria.c_str(), descrizione.c_str(), prezzo_euro, azienda_produzione.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows == 1){
            
            //Ottengo i vari dati per costruire l'ordine:

            cod_product = atoi(PQgetvalue(res, 0, PQfnumber(res, "codProdotto")));

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
            sprintf(sqlcmd, "INSERT INTO Ordine (idOrdine, codProdotto, nome_utente_compratore, dataOrdineEffettuato, statoOrdine, viaSpedizione, cittaSpedizione, numCivSpedizione) VALUES (DEFAULT, '%d', '%s', '%s', '%s','%s','%s','%s' )", 
            cod_product, nomeUtenteCompratore.c_str(), dataOrdineEffettuato.c_str(), statoOrdineStr.c_str(), via_spedizione.c_str(),città_spedizione.c_str(), numero_civico_spedizione.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);     


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
        }
        else{
            std::cout << "Errore: il prodotto non è stato trovato!" << std::endl;
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