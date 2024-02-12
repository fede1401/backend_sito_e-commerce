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
#include "spedizione.h"
#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db/pgsql.h"


void print_select(std::string nome_tabella){
    Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");
    std::cout << "Tabella: " << nome_tabella << std::endl;
    sprintf(sqlcmd, "SELECT * FROM %s", nome_tabella.c_str());
    res = db1.ExecSQLtuples(sqlcmd);
    rows = PQntuples(res);
    int numCols = PQnfields(res);

    //std::cout << "Rows: " << rows << std::endl;
    for (int i = 0; i < rows; ++i) {
        std::cout << "Row " << i << ": ";
        for (int j = 0; j < numCols; ++j) {
            std::cout << PQgetvalue(res, i, j) << ",   ";
        }
        std::cout << std::endl;
    }
    PQclear(res); 
    std::cout << "\n" << std::endl;
    
}


void test_effettuaRegistrazioneCompratori(){
    UtenteCompratore compratore;
    compratore.effettuaRegistrazione("marco1", "UtenteCompratore", "Marco", "Giggio", "3339993339", "marco.giggio@gmail.com", "Via della Roma", "36", "01010", "Roma", "Compleanno1.2", "Compleanno1.2", "11/11/2001");
}

void test_effettuaRegistrazioneFornitore(){
    UtenteFornitore fornitore;
    fornitore.effettuaRegistrazione("luigi2", "UtenteFornitore", "Luigi", "Faffo", "3333333333", "luigi.faffo@gmail.com", "Merdaaaa.1", "Merdaaaa.1", "Nike");

    UtenteFornitore fornitore2;
    fornitore.effettuaRegistrazione("mario1", "UtenteFornitore", "Mario", "Cocco", "3333333333", "mario.cocco@gmail.com", "Merdaaaa.1", "Merdaaaa.1", "Apple");
}

void test_add_newProdotti(){
    Product prodotto1;
    prodotto1.add_new_product("Air Force 1", "Abbigliamento e scarpe", 100.00, "Air force 1, colore bianco, taglia 40", "Nike", 100);

    Product prodotto2;
    prodotto2.add_new_product("Iphone 15", "Telefonia", 990.99, "Iphone 15, 16GB RAM, 1 TB archiviazione", "Apple", 10);
}

void test_aggiornamento_numeroDiTelefono(){

    UtenteCompratore compratore;
    compratore.effettuaRegistrazione("test_user1", "UtenteCompratore", "Alice", "Rossi", "1234567890", "alice.rossi@example.com", "Via Roma", "123", "00100", "Roma", "P@ssw0rd!", "P@ssw0rd!", "1990/05/15");
    compratore = compratore.anima_oggetto("UtenteCompratore", "test_user1", "P@ssw0rd!");
    compratore.effettua_login("test_user1", "P@ssw0rd!");
    compratore.effettua_logout("test_user1");

    print_select("UtenteCompratore");

    compratore.aggiornaNumeroDiTelefono("555555555");

    print_select("UtenteCompratore");

    /*
    Risultati ottenuti;
    Tabella: UtenteCompratore
    Row 0: test_user1,   UtenteCompratore,   Alice,   Rossi,   alice.rossi@example.com,   1234567890,   P@ssw0rd!,   1990-05-15,   Via Roma,   123,   00100,   Roma,   0.00,   0,   


    Tabella: UtenteCompratore
    Row 0: test_user1,   UtenteCompratore,   Alice,   Rossi,   alice.rossi@example.com,   1111111111,   P@ssw0rd!,   1990-05-15,   Via Roma,   123,   00100,   Roma,   0.00,   0,  
    */
}


void test_aggiornamento_Password(){
    UtenteCompratore compratore;
    compratore.effettuaRegistrazione("test_user1", "UtenteCompratore", "Alice", "Rossi", "1234567890", "alice.rossi@example.com", "Via Roma", "123", "00100", "Roma", "P@ssw0rd!", "P@ssw0rd!", "1990/05/15");
    compratore = compratore.anima_oggetto("UtenteCompratore", "test_user1", "P@ssw0rd!");
    compratore.effettua_login("test_user1", "P@ssw0rd!");
    compratore.effettua_logout("test_user1");

    print_select("UtenteCompratore");

    compratore.aggiornaPassword("P@ssw0rd!", "Mortadella444.");

    print_select("UtenteCompratore");
    return;
    /*
    Risultati:
    Tabella: UtenteCompratore
    Row 0: test_user1,   UtenteCompratore,   Alice,   Rossi,   alice.rossi@example.com,   555555555,   P@ssw0rd!,   1990-05-15,   Via Roma,   123,   00100,   Roma,   0.00,   0,   


    Tabella: UtenteCompratore
    Row 0: test_user1,   UtenteCompratore,   Alice,   Rossi,   alice.rossi@example.com,   555555555,   Mortadella444.,   1990-05-15,   Via Roma,   123,   00100,   Roma,   0.00,   0,  
    */
}   


void test_aggiornaResidenza(){
    UtenteCompratore compratore;
    compratore.effettuaRegistrazione("test_user1", "UtenteCompratore", "Alice", "Rossi", "1234567890", "alice.rossi@example.com", "Via Roma", "123", "00100", "Roma", "P@ssw0rd!", "P@ssw0rd!", "1990/05/15");
    compratore = compratore.anima_oggetto("UtenteCompratore", "test_user1", "P@ssw0rd!");
    compratore.effettua_login("test_user1", "P@ssw0rd!");
    compratore.effettua_logout("test_user1");

    print_select("UtenteCompratore");

    compratore.aggiornaResidenza("Via Salerno", "000", "01010", "Salerno");

    print_select("UtenteCompratore");

    /*
    Risultati ottenuti:
    Tabella: UtenteCompratore
    Row 0: test_user1,   UtenteCompratore,   Alice,   Rossi,   alice.rossi@example.com,   555555555,   Mortadella444.,   
    1990-05-15,   Via Roma,   123,   00100,   Roma,   0.00,   0,   


    Tabella: UtenteCompratore
    Row 0: test_user1,   UtenteCompratore,   Alice,   Rossi,   alice.rossi@example.com,   555555555,   Mortadella444.,   
    1990-05-15,   Via Salerno,   000,   01010,   Salerno,   0.00,   0,
    */
}


void test_aggiornaAziendaProduttrice(){
    UtenteFornitore fornitore;
    fornitore.effettuaRegistrazione("test_user2", "UtenteFornitore", "Biagio", "Anocacci", "333445567", "biagio.anocacci@gmail.com", "Candela111?", "Candela111?", "Nike");
    fornitore = fornitore.anima_oggetto("UtenteFornitore", "test_user2", "Candela111?");
    fornitore.effettua_login("test_user2", "Candela111?");
    fornitore.effettua_logout("test_user2");

    print_select("utenteFornitore");

    fornitore.aggiornaNomeAziendaProduttrice("Adidas");

    print_select("utenteFornitore");

    /*
    Risultati ottenuti:
    Tabella: utenteFornitore
    Row 0: test_user2,   UtenteFornitore,   Biagio,   Anocacci,   biagio.anocacci@gmail.com,   333445567,   Candela111?,   Nike,   0,   


    Tabella: utenteFornitore
    Row 0: test_user2,   UtenteFornitore,   Biagio,   Anocacci,   biagio.anocacci@gmail.com,   333445567,   Candela111?,   Adidas,   0, 
    */

}


void test_aggiornaDittaSpedizione(){
    UtenteTrasportatore trasportatore;
    trasportatore.effettuaRegistrazione("test_user3", "UtenteTrasportatore", "Marco", "Verdi", "345678900", "marco.verdi@gmail.com", "Test1234.", "Test1234.", "FedEx");
    trasportatore = trasportatore.anima_oggetto("UtenteTrasportatore", "test_user3", "Test1234.");
    trasportatore.effettua_login("test_user3", "Test1234.");
    trasportatore.effettua_logout("test_user3");

    print_select("UtenteTrasportatore");

    trasportatore.aggiornaNomeDittaSpedizione("Bartolini");

    print_select("UtenteTrasportatore");

    /* Risultati ottenuti:
    Tabella: UtenteTrasportatore
    Row 0: test_user3,   UtenteTrasportatore,   Marco,   Verdi,   marco.verdi@gmail.com,   345678900,   Test1234.,   FedEx,   0,   


    Tabella: UtenteTrasportatore
    Row 0: test_user3,   UtenteTrasportatore,   Marco,   Verdi,   marco.verdi@gmail.com,   345678900,   Test1234.,   Bartolini,   0,   
    */

}


// Mi viene un dubbio: supponiamo un impiegato lavora nell'azienda Nike e inserisce nel backend un prodotto Nike. 
// Se cambia azienda e ad esempio comincia a lavorare per l'Adidas, non ci sarà nessun utente associato all'inserimento del prodotto Nike
// Testiamolo:
void test2_aggiornaAziendaProduttrice(){
    UtenteFornitore fornitore;
    fornitore.effettuaRegistrazione("test_user4", "UtenteFornitore", "Maria", "Leta", "333445567", "maria.leta@gmail.com", "Candela111?", "Candela111?", "Nike");
    fornitore = fornitore.anima_oggetto("UtenteFornitore", "test_user4", "Candela111?");
    fornitore.effettua_login("test_user4", "Candela111?");
    fornitore.effettua_logout("test_user4");

    print_select("utenteFornitore");

    Product prodottoNike;
    prodottoNike.add_new_product("Air Force 1", "Abbigliamento e scarpe", 100.00, "Air force 1, colore bianco, taglia 40", "Nike", 100);

    print_select("Prodotto");

    fornitore.aggiornaNomeAziendaProduttrice("Adidas");

    print_select("utenteFornitore");
    print_select("Prodotto");
}


void test_ricercaProdotto(){
    Product prodotto;
    prodotto.ricerca_mostra_Prodotto("Air Force 1");
    return;
}


void test_acquistoProdotto(){
    Product prodotto1;
    prodotto1.ricerca_mostra_Prodotto("Air Force 1");
    print_select("Prodotto");
    print_select("Ordine");
    
    Ordine ordine;
    ordine = prodotto1.acquistaProdotto("marco1", "Via della testa", "Roma", "45");

    print_select("Ordine");

    return;
}



int main(){
    Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");
    std::cout << "Connessione al database avvenuta con successo." << std::endl;

    PGresult *res;

    char sqlcmd[1000]; 
    
    test_effettuaRegistrazioneCompratori();

    UtenteCompratore compratore;
    compratore = compratore.anima_oggetto("UtenteCompratore", "marco1", "Compleanno1.2");
    compratore.effettua_login("marco1", "Compleanno1.2");

    UtenteTrasportatore trasportatore;
    trasportatore.effettuaRegistrazione("_marco_", "UtenteTrasportatore", "Marco", "Marco", "3333333333", "marco.marco@gmail.com", "Marcoooo1.", "Marcoooo1.", "FedEx");
    trasportatore = trasportatore.anima_oggetto("UtenteTrasportatore", "_marco_", "Marcoooo1.");
    trasportatore.effettua_login("_marco_", "Marcoooo1.");

    UtenteFornitore fornitore;
    fornitore.effettuaRegistrazione("luigi2", "UtenteFornitore", "Luigi", "Faffo", "3333333333", "luigi.faffo@gmail.com", "Merdaaaa.1", "Merdaaaa.1", "Nike");
    fornitore = fornitore.anima_oggetto("UtenteFornitore", "luigi2", "Merdaaaa.1");
    fornitore.effettua_login("luigi2", "Merdaaaa.1");

    print_select("UtenteCompratore");
    print_select("UtenteFornitore");
    print_select("UtenteTrasportatore");

    Product prodotto;
    //prodotto.ricerca_mostra_Prodotto("Iphone 15");
    prodotto.add_new_product("Maglietta Nike", "Abbigliamento", 29.99, "Maglietta Nike color nero per sport", "Nike", 5);

    Ordine ordine;
    ordine = prodotto.acquistaProdotto("marco1", "Via della testa", "Roma", "45");

    UtenteTrasportatore trasportatore2;
    trasportatore2.effettuaRegistrazione("_trasportatore2_", "UtenteTrasportatore", "Lollo", "Lollo", "3333333333", "lollo.lollo@gmail.com", "Loooll00o.", "Loooll00o.", "Bartolini");
    trasportatore2 = trasportatore2.anima_oggetto("UtenteTrasportatore", "_trasportatore2_", "Loooll00o.");
    trasportatore2.effettua_login("_trasportatore2_", "Loooll00o.");

    Spedizione spedizione;
    //spedizione = spedizione.assegnaOrdineTrasportatore();

    ordine.visione_ordini_effettuati("marco1");

    spedizione = spedizione.assegnaOrdineTrasportatore();

    //print_select("Reso");

    spedizione.spedizioneConsegnata(1);

    //ordine.annulla_ordine(2);


    Reso reso;
    motivazioneReso motivazione_reso;
    motivazione_reso = motivazioneReso::Difettoso;
    reso.effettuaReso(1, motivazione_reso);

    print_select("Ordine");
    print_select("Spedizione");
    print_select("Reso");
    print_select("Recensione");

    Recensione recensione;
    votoStelle voto_stelle;
    voto_stelle = votoStelle::Quattro;
    recensione.effettuaRecensione(1, "Prodotto ottimo!", voto_stelle);

    print_select("Recensione");

    print_select("UtenteCompratore");
    print_select("UtenteFornitore");
    print_select("UtenteTrasportatore");

    compratore.effettua_logout("marco1");
    fornitore.effettua_logout("luigi2");
    trasportatore.effettua_logout("_marco_");

    print_select("UtenteCompratore");
    print_select("UtenteFornitore");
    print_select("UtenteTrasportatore");



    /*
    Tabella: Spedizione
    Row 0: 2,   2,   _trasportatore2_,   in transito,   Bartolini,   
    Row 1: 1,   1,   _marco_,   consegnato,   FedEx

    Tabella: Ordine
    Row 0: 1,   1,   marco1,   2024-02-10,   spedito,   Via della testa,   Roma,   45,   
    Row 1: 4,   1,   marco1,   2024-02-10,   in elaborazione,   Via della testa,   Roma,   45,   
    Row 2: 2,   1,   marco1,   2024-02-10,   spedito,   Via della testa,   Roma,   45,   
    Row 3: 5,   1,   marco1,   2024-02-10,   in elaborazione,   Via della testa,   Roma,   45,   
    Row 4: 6,   1,   marco1,   2024-02-10,   in elaborazione,   Via della testa,   Roma,   45,   
    Row 5: 7,   1,   marco1,   2024-02-10,   in elaborazione,   Via della testa,   Roma,   45,   
    Row 6: 3,   1,   marco1,   2024-02-10,   spedito,   Via della testa,   Roma,   45,   


    Tabella: Spedizione
    Row 0: 2,   2,   _trasportatore2_,   in transito,   Bartolini,   
    Row 1: 1,   1,   _marco_,   consegnato,   FedEx,   
    Row 2: 3,   3,   _marco_,   in transito,   FedEx,
    */

    //test_acquistoProdotto();

    //print_select("UtenteTrasportatore");


    //spedizione.spedizioneConsegnata(1);

    //print_select("Spedizione");
    //print_select("UtenteTrasportatore");







    //test_aggiornamento_Password();


    //test_aggiornamento_numeroDiTelefono();

    //test_aggiornaResidenza();

    //test_aggiornaAziendaProduttrice();

    //test_aggiornaDittaSpedizione();

    //test2_aggiornaAziendaProduttrice();

    //test_ricercaProdotto();

    //test_acquistoProdotto();



    
    /*UtenteCompratore compratore;
    compratore.effettuaRegistrazione("test_user1", "UtenteCompratore", "Alice", "Rossi", "1234567890", "alice.rossi@example.com", "Via Roma", 123, "00100", "Roma", "P@ssw0rd!", "P@ssw0rd!", "1990/05/15");
    compratore = compratore.anima_oggetto("UtenteCompratore", "test_user1", "P@ssw0rd!");
    compratore.effettua_login("test_user1", "P@ssw0rd!");
    compratore.effettua_logout("test_user1");

    UtenteFornitore fornitore;
    fornitore.effettuaRegistrazione("test_user2", "UtenteFornitore", "Biagio", "Anocacci", "333445567", "biagio.anocacci@gmail.com", "Candela111?", "Candela111?", "Nike");
    fornitore = fornitore.anima_oggetto("UtenteFornitore", "test_user2", "Candela111?");
    fornitore.effettua_login("test_user2", "Candela111?");
    fornitore.effettua_logout("test_user2");

    UtenteTrasportatore trasportatore;
    trasportatore.effettuaRegistrazione("test_user3", "UtenteTrasportatore", "Marco", "Verdi", "345678900", "marco.verdi@gmail.com", "Test1234.", "Test1234.", "FedEx");
    trasportatore = trasportatore.anima_oggetto("UtenteTrasportatore", "test_user3", "Test1234.");
    trasportatore.effettua_login("test_user3", "Test1234.");
    trasportatore.effettua_logout("test_user3");

    UtenteFornitore fornitore2;
    fornitore.effettuaRegistrazione("test_user3", "UtenteFornitore", "Mirto", "Bacci", "333445567", "mirto.bacci@gmail.com", "Candela123?", "Candela123?", "Nike");
    fornitore = fornitore.anima_oggetto("UtenteFornitore", "test_user3", "Candela111?");
    fornitore.effettua_login("test_user3", "Candela111?");
    fornitore.effettua_logout("test_user3");

    print_select("UtenteCompratore");
    print_select("utenteTrasportatore");
    print_select("utenteFornitore");

    
    UtenteFornitore fornitore;
    std::cout << "Prima del login" << std::endl;
    fornitore.effettuaRegistrazione("test_user2", "UtenteFornitore", "Biagio", "Anocacci", "333445567", "biagio.anocacci@gmail.com", "Candela111?", "Candela111?", "Nike");
    
    fornitore = fornitore.anima_oggetto("UtenteFornitore", "test_user2", "Candela111?");
    fornitore.effettua_logout("test_user2");
    fornitore.effettua_login("test_user2", "Candela111?");

    std::cout << fornitore.nome << std::endl;
    std::cout << fornitore.cognome << std::endl;
    std::cout << fornitore.nome_utente << std::endl;
    std::cout << fornitore.email << std::endl;


    UtenteTrasportatore trasportatore;
    trasportatore.effettuaRegistrazione("test_user3", "UtenteTrasportatore", "Marco", "Verdi", "345678900", "marco.verdi@gmail.com", "Test1234.", "Test1234.", "FedEx");
    trasportatore = trasportatore.anima_oggetto("UtenteTrasportatore", "test_user3", "Test1234.");
    trasportatore.effettua_logout("test_user3");
    trasportatore.effettua_login("test_user3", "Test1234.");
    std::cout << trasportatore.nome << std::endl;
    std::cout << trasportatore.cognome << std::endl;
    std::cout << trasportatore.nome_utente << std::endl;
    std::cout << trasportatore.email << std::endl;
    
    UtenteCompratore compratore2;
    compratore2.effettuaRegistrazione("test_user3", "UtenteCompratore", "Marco", "Verdi", "345678900", "marco.verdi@gmail.com","Via torino", 1, "00100", "Torino", "Test1234.", "Test1234.", "1999/09/18");
    compratore2.effettua_login("test_user3", "Test1234.");
    std::cout << compratore2.nome << std::endl;
    std::cout << compratore2.cognome << std::endl;
    std::cout << compratore2.nome_utente << std::endl;
    std::cout << compratore2.email << std::endl;

    print_select("UtenteCompratore");
    print_select("utenteTrasportatore");
    print_select("utenteFornitore");

    //print_select("Prodotto");  
    //print_select("Carrello");  
    //print_select("Carta");  
    */

    
    /*
    

    Carrello carrello1;
    carrello1.add_prodotto("test_user1", 1);

    Carta carta;
    carta.aggiungi_carta("test_user1", "55598747283434", "333");
    
    print_select("UtenteCompratore");
    print_select("utenteTrasportatore");
    print_select("utenteFornitore");

    print_select("Prodotto");  
    print_select("Carrello");  
    print_select("Carta");  

    compratore.elimina_profilo();

    std::cout << "Post eliminazione." << std::endl;

    print_select("UtenteCompratore");
    print_select("utenteTrasportatore");
    print_select("utenteFornitore");

    print_select("Prodotto");  
    print_select("Carrello");  
    print_select("Carta");  
    */
    

}




/*
int main() {

    Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");
    std::cout << "Connessione al database avvenuta con successo." << std::endl;

    PGresult *res;

    char sqlcmd[1000];

    
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

    

    //UtenteCompratore utente1("mario_1", "UtenteCompratore", "mario", "rossi", "3333333333", "Vaffsdns12?", "mario.rossi1@gmail.com", "15/09/2000", "via delle manine", 34, "01035", "terni", 0.0, 0);

    //UtenteCompratore utente;
    //utente.effettuaRegistrazione("marco15", "UtenteCompratore", "Marco", "Verdi", "339995551", "marco.verdi@gmail.com", "Via delle castile", 59, "01044", "Roma", "Mouse1234", "Mouse1234", "25/01/1999");

    UtenteCompratore utente2;
    utente2.effettuaRegistrazione("Gabriele15", "UtenteCompratore", "Gabriele", "ROssi", "339995551", "gabriele.rossi@gmail.com", "Via delle tirali", 59, "01056", "Roma", "Computer1234", "Computer1234", "25/01/1999"); 
    utente2.effettua_login("UtenteCompratore", "Gabriele15", "Computer1234");
    utente2.effettua_logout("UtenteCompratore","Gabriele15");

    //utente1.effettuaRegistrazione(utente1, "Vaffsdns12?");
    
    //std::cout << "Campi dell'utente: " << utente2.nome << ",  " << utente2.cognome << ",  " << utente2.categoria << ",  " << utente2.nome_utente << ",  " << utente2.email <<  std::endl;
    
    //UtenteTrasportatore utentetrasport;
    //utentetrasport.effettuaRegistrazione("Mario24", "UtenteTrasportatore", "Mario", "Verdi", "3334445559", "mario.verdi@gmail.com", "TestieraLatt!23", "TestieraLatt!23", "FedEx");
    //utentetrasport.effettua_login("UtenteTrasportatore", "Mario24", "TestieraLatt!23");

    //fprintf(stdout, "maiqn(): inserted in UtenteCompratore %s\n", PQgetvalue(res, 0, PQfnumber(res, "numeroTelefono")));


    //sprintf(sqlcmd, "SELECT * FROM UtenteCompratore");

    UtenteFornitore utenteforn;
    utenteforn.effettuaRegistrazione("Laura99", "UtenteFornitore", "Laura", "Mangialucchi", "333445567", "laura.mangialucchi@gmail.com", "Candela111?", "Candela111?", "Apple");
    std::cout << "\n Fornitore categoria: " << utenteforn.categoria << "\n" << std::endl;
    utenteforn.effettua_login("UtenteFornitore", "Laura99", "Candela111?");

    UtenteFornitore utenteforn2;
    utenteforn.effettuaRegistrazione("Biagio10", "UtenteFornitore", "Biagio", "Anocacci", "333445567", "biagio.anocacci@gmail.com", "Candela111?", "Candela111?", "Nike");
    std::cout << "\n Fornitore categoria: " << utenteforn.categoria << "\n" << std::endl;
    utenteforn.effettua_login("UtenteFornitore", "Biagio10", "Candela111?");
    
    utenteforn.effettuaRegistrazione("Andrea12", "UtenteFornitore", "Andrea", "Sacchini", "333445567", "andrea.sacchini@gmail.com", "Candelabro111?", "Candelabro111?", "Xiaomi");
    utenteforn.effettua_login("UtenteFornitore", "Andrea12", "Candelabro111?");

    print_select("UtenteCompratore");
    print_select("utenteTrasportatore");
    print_select("utenteFornitore");

    print_select("DittaSp");  
    print_select("AziendaProd");  

    Product prodotto1;
    prodotto1.add_new_product("Xiaomi lite9", "Telefonia", 540.00, "Xiaomi lite9, 8GB RAM, 1 TB archiviazione", "Xiaomi", 10);
    Product prodotto2;
    prodotto2.add_new_product("Iphone 15", "Telefonia", 990.00, "Iphone 15 nuovo, 16GB RAM, 1TB archiviazione", "Apple", 15);
    //print_select("Prodotto");   

    Carrello carrello1;
    carrello1.add_prodotto("Gabriele15", 2);

    print_select("Prodotto");
    print_select("Carrello"); 

    if (rows >= 1) {

        nome_utente = PQgetvalue(res, 0, PQfnumber(res, "nome_utente"));
        stato = PQgetvalue(res, 0, PQfnumber(res, "stato"));

        std::cout << "Campi dell'utente: " << nome_utente << ",  " << stato <<  std::endl;

    } else {
        std::cout << "Errore: Lo stato non è stato trovato." << std::endl;
        return;
    }
    

    return 0;
}



    //sprintf(sqlcmd, "BEGIN");
    //res = db1.ExecSQLcmd(sqlcmd);
    //PQclear(res);

    //sprintf(sqlcmd, "INSERT INTO AssistenzaClienti VALUES ('%s\')", "33333333331");

    //res = db1.ExecSQLcmd(sqlcmd);
    //PQclear(res);

        
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
    
    // Utilizzato per stampare su standard output (terminale) i risultati delle select.
    //fprintf(stdout, "main(): inserted in AssistenzaClienti %s\n", PQgetvalue(res,0, PQfnumber(res, "numeroTelefono")));

    //fprintf(stdout, "main(): inserted in AssistenzaClienti %s\n", PQgetvalue(res,10, PQfnumber(res, "numeroTelefono")));

    //fprintf(stdout, "main(): inserted in AssistenzaClienti %s\n", PQgetvalue(res,11, PQfnumber(res, "numeroTelefono")));

*/
