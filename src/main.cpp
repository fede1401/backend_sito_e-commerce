#include <iostream>
#include <string>

#include "carrello.h"
#include "lista_desideri.h"
#include "carta.h"
#include "ordine.h"
#include "reso.h"
#include "recensione.h"
#include "user.h"
#include "user_compratore.h"
#include "user_fornitore.h"
#include "user_trasportatore.h"
#include "product.h"
#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db/pgsql.h"


void print_select(std::string nome_tabella){
    Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");
    sprintf(sqlcmd, "SELECT * FROM %s", nome_tabella.c_str());
    res = db1.ExecSQLtuples(sqlcmd);
    rows = PQntuples(res);
    int numCols = PQnfields(res);

    std::cout << "Rows: " << rows << std::endl;
    for (int i = 0; i < rows; ++i) {
        std::cout << "Row " << i << ": ";
        for (int j = 0; j < numCols; ++j) {
            std::cout << PQgetvalue(res, i, j) << ",   ";
        }
        std::cout << std::endl;
    }
    PQclear(res); 
    std::cout << "\n" << rows << std::endl;
    
}


int main() {

    Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");
    std::cout << "Connessione al database avvenuta con successo." << std::endl;

    PGresult *res;

    char sqlcmd[1000];

    /*
    sprintf(sqlcmd, "INSERT INTO AziendaProd (idAziendaProd, nome) VALUES (DEFAULT, 'Apple')");
    res = db1.ExecSQLcmd(sqlcmd);
    PQclear(res); 

    sprintf(sqlcmd, "INSERT INTO AziendaProd (idAziendaProd, nome) VALUES (DEFAULT, 'Nike')");
    res = db1.ExecSQLcmd(sqlcmd);
    PQclear(res); 

    sprintf(sqlcmd, "INSERT INTO AziendaProd(idAziendaProd, nome) VALUES (DEFAULT, 'Samsung Eletronics')");
    res = db1.ExecSQLcmd(sqlcmd);
    PQclear(res); 

    sprintf(sqlcmd, "INSERT INTO AziendaProd(idAziendaProd, nome) VALUES (DEFAULT, 'Xiaomi')");
    res = db1.ExecSQLcmd(sqlcmd);
    PQclear(res); 
    


    sprintf(sqlcmd, "INSERT INTO DittaSp(idDittaSp, nome) VALUES (DEFAULT, 'FedEx')");
    res = db1.ExecSQLcmd(sqlcmd);
    PQclear(res); 

    sprintf(sqlcmd, "INSERT INTO DittaSp(idDittaSp, nome) VALUES (DEFAULT, 'UPS')");
    res = db1.ExecSQLcmd(sqlcmd);
    PQclear(res); 

    sprintf(sqlcmd, "INSERT INTO DittaSp(idDittaSp, nome) VALUES (DEFAULT, 'DHL')");
    res = db1.ExecSQLcmd(sqlcmd);
    PQclear(res); 


    sprintf(sqlcmd, "SELECT * FROM AziendaProd");
    res = db1.ExecSQLtuples(sqlcmd);
    rows = PQntuples(res);
    int numCols = PQnfields(res);

    std::cout << "Rows: " << rows << std::endl;
    for (int i = 0; i < rows; ++i) {
        std::cout << "Row " << i << ": ";
        for (int j = 0; j < numCols; ++j) {
            std::cout << PQgetvalue(res, i, j) << "\t";
        }
        std::cout << std::endl;
    }
    PQclear(res); 


    sprintf(sqlcmd, "SELECT * FROM DittaSp");
    res = db1.ExecSQLtuples(sqlcmd);
    rows = PQntuples(res);
    numCols = PQnfields(res);

    std::cout << "Rows: " << rows << std::endl;
    for (int i = 0; i < rows; ++i) {
        std::cout << "Row " << i << ": ";
        for (int j = 0; j < numCols; ++j) {
            std::cout << PQgetvalue(res, i, j) << "\t";
        }
        std::cout << std::endl;
    }
    PQclear(res); 
    */

    //UtenteCompratore utente1("mario_1", "UtenteCompratore", "mario", "rossi", "3333333333", "Vaffsdns12?", "mario.rossi1@gmail.com", "15/09/2000", "via delle manine", 34, "01035", "terni", 0.0, 0);

    UtenteCompratore utente;
    //utente.effettuaRegistrazione("marco15", "UtenteCompratore", "Marco", "Verdi", "339995551", "marco.verdi@gmail.com", "Via delle castile", 59, "01044", "Roma", "Mouse1234", "Mouse1234", "25/01/1999");


    UtenteCompratore utente2;
    utente2.effettuaRegistrazione("Gabriele15", "UtenteCompratore", "Gabriele", "ROssi", "339995551", "gabriele.rossi@gmail.com", "Via delle tirali", 59, "01056", "Roma", "Computer1234", "Computer1234", "25/01/1999"); 

    //utente1.effettuaRegistrazione(utente1, "Vaffsdns12?");
    
    //std::cout << "Campi dell'utente: " << utente2.nome << ",  " << utente2.cognome << ",  " << utente2.categoria << ",  " << utente2.nome_utente << ",  " << utente2.email <<  std::endl;
    
    UtenteTrasportatore utentetrasport;
    utentetrasport.effettuaRegistrazione("Mario24", "UtenteTrasportatore", "Mario", "Verdi", "3334445559", "mario.verdi@gmail.com", "TestieraLatt!23", "TestieraLatt!23", "FedEx");
    utentetrasport.effettua_login("UtenteTrasportatore", "Mario24", "TestieraLatt!23");

    //fprintf(stdout, "maiqn(): inserted in UtenteCompratore %s\n", PQgetvalue(res, 0, PQfnumber(res, "numeroTelefono")));

    utente2.effettua_login("UtenteCompratore", "Gabriele15", "Computer1234");
    utente2.effettua_logout("UtenteCompratore","Gabriele15");


    //sprintf(sqlcmd, "SELECT * FROM UtenteCompratore");
    print_select("UtenteCompratore");
    print_select("utenteTrasportatore");
    print_select("DittaSp");   


    Carrello carrello1;
    //carrello1.add_prodotto("Gabriele15", ) 

    /*if (rows >= 1) {

        nome_utente = PQgetvalue(res, 0, PQfnumber(res, "nome_utente"));
        stato = PQgetvalue(res, 0, PQfnumber(res, "stato"));

        std::cout << "Campi dell'utente: " << nome_utente << ",  " << stato <<  std::endl;

    } else {
        std::cout << "Errore: Lo stato non è stato trovato." << std::endl;
        return;
    }
    */

    return 0;
}




    //sprintf(sqlcmd, "BEGIN");
    //res = db1.ExecSQLcmd(sqlcmd);
    //PQclear(res);

    //sprintf(sqlcmd, "INSERT INTO AssistenzaClienti VALUES ('%s\')", "33333333331");

    //res = db1.ExecSQLcmd(sqlcmd);
    //PQclear(res);

        /*
    sprintf(sqlcmd, "COMMIT"); 
    res = db1.ExecSQLcmd(sqlcmd);
    PQclear(res);

    //std::cout << "Operazioni avvenute." << std::endl;

    int rows;

    sprintf(sqlcmd, "SELECT * FROM AssistenzaClienti");
    res = db1.ExecSQLtuples(sqlcmd);
    rows = PQntuples(res);
    int numCols = PQnfields(res);
    
    // Vedere tutti i valori di una tabella 
    for (int i = 0; i < rows; ++i) {
        std::cout << "Row " << i << ": ";
        for (int j = 0; j < numCols; ++j) {
            std::cout << PQgetvalue(res, i, j) << "\t";
        }
        std::cout << std::endl;
    }

    std::cout << res << std::endl;
    fprintf(stdout, "\n");
    */
    // Utilizzato per stampare su standard output (terminale) i risultati delle select.
    //fprintf(stdout, "main(): inserted in AssistenzaClienti %s\n", PQgetvalue(res,0, PQfnumber(res, "numeroTelefono")));

    //fprintf(stdout, "main(): inserted in AssistenzaClienti %s\n", PQgetvalue(res,10, PQfnumber(res, "numeroTelefono")));

    //fprintf(stdout, "main(): inserted in AssistenzaClienti %s\n", PQgetvalue(res,11, PQfnumber(res, "numeroTelefono")));
