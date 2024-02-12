// Carta.h
#ifndef CARTA_H
#define CARTA_H

#include <string>

class Carta {
public:
    std::string nome_utente;
    std::string numero_carta;
    std::string cvv;


    // Costruttore
    Carta():
        nome_utente(""),
        numero_carta(""),
        cvv("")
        {}

    Carta(std::string nome_utente, std::string numero, std::string codice): 
        nome_utente(nome_utente), 
        numero_carta(numero), 
        cvv(codice) {}


    void aggiungi_carta(std::string in_nome_utente, std::string in_numeroCarta, std::string in_cvv){

        // Connessione al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");
        std::cout << "Connessione al database avvenuta con successo." << std::endl;


        // Check se il nome utente è di un utente compratore:
        sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", in_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        
        if (rows == 1){
            // Il nome utente è di un utente compratore, possiamo aggiungere la carta al database:
            sprintf(sqlcmd, "INSERT INTO Carta (idCarta, nome_utente_compratore, numeroCarta, cvv) VALUES (DEFAULT, '%s', '%s', '%s')", 
                                                        in_nome_utente.c_str(), in_numeroCarta.c_str(), in_cvv.c_str());
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res); 

        } 
        else{
            std::cout << "Errore: L'utente non è stato trovato." << std::endl;
            return;
        }

    return;
    }


    void remove_carta(int idCarta){

        // Connession al database:
        Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

        sprintf(sqlcmd, "SELECT * FROM Carta WHERE idCarta = '%d'", idCarta);
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        if (rows < 1){
            std::cout << "La riga da eliminare non esiste!" << std::endl;
            return;
        }
        else{
            // Eliminazione del prodotto dal carrello dell'utente compratore.
            sprintf(sqlcmd, "DELETE FROM Carta WHERE idCarta = '%d'", idCarta);
            res = db1.ExecSQLcmd(sqlcmd);
            PQclear(res);
        }
    }



};

#endif // CARTA_H
