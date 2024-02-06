// Product.h
#ifndef PRODUCT_H
#define PRODUCT_H


#include <iostream>
#include <string>
#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db/pgsql.h"

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


};



#endif // PRODUCT_H
