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
#include "product.h"
#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db/pgsql.h"


int main() {
    /*UtenteCompratore compratore1("001", "mio_utente", "", "", 123456789, "password123", "mario@email.com", 30,
                                 "01/01/1990", "Via Roma", 42, "12345", "Città di Prova", 100.0, "Attivo");

    Carrello carrelloUtente("001", "P001");
    ListaDesideri listaDesideriUtente("001", "P002");

    Carta cartaUtente("001", "1234567890123456", "123");

    Ordine ordineUtente("001", "ORD001", "P003", "T001", "2024-01-24", "In elaborazione", "DittaShip", "Via Roma", "Città di Prova", 42);

    Recensione recensioneUtente("R001", "001", "Prodotto fantastico!", 5, "P003");
    */

    Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "b_sito_ecommerce");
    std::cout << "Connessione al database avvenuta con successo." << std::endl;

    PGresult *res;

    char sqlcmd[1000];


    //sprintf(sqlcmd, "BEGIN");
    //res = db1.ExecSQLcmd(sqlcmd);
    //PQclear(res);

    //sprintf(sqlcmd, "INSERT INTO AssistenzaClienti VALUES ('%s\')", "33333333331");

    //res = db1.ExecSQLcmd(sqlcmd);
    //PQclear(res);

    UtenteCompratore utente1("mario_1", "mario", "rossi", "3333333333", "Vaffsdns12?", 
                            "mario.rossi1@gmail.com", "15/09/2000", "via delle mani", 
                            34, "01035", "terni", 0.0, 0);
    utente1.effettuaRegistrazione(utente1, "Vaffsdns12?");
    

    sprintf(sqlcmd, "SELECT * FROM UtenteCompratore");
    res = db1.ExecSQLtuples(sqlcmd);
    rows = PQntuples(res);

    fprintf(stdout, "main(): inserted in UtenteCompratore %s\n", PQgetvalue(res, 0, PQfnumber(res, "numeroTelefono")));


    utente1.effettua_login("mario_1", "Vaffsdns12");
    
    
    
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


    return 0;
}
