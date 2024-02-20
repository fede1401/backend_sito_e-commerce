#include "svolgiAzione.h"


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


void initUSER(UtenteCompratore compratore, UtenteFornitore fornitore, UtenteTrasportatore trasportatore){

    // Array di nomi utente
    std::array<std::string, 100> nomi_utente;
    for (int i = 0; i < 100; ++i) {
        nomi_utente[i] = "Utente" + std::to_string(i + 1);
    }

    // Array di nomi
    std::array<std::string, 10> nomi = {"Mario", "Luigi", "Giovanni", "Alessia", "Chiara", "Francesco", "Elena", "Roberto", "Laura", "Paolo"};

    // Array di cognomi
    std::array<std::string, 10> cognomi = {"Rossi", "Bianchi", "Verdi", "Ferrari", "Russo", "Esposito", "Romano", "Gallo", "Conti", "De Luca"};

    // Array di email
    std::array<std::string, 100> email;
    for (int i = 0; i < 100; ++i) {
        email[i] = nomi_utente[i] + "@example.com";
    }

    // Array di numeri di telefono
    std::array<std::string, 100> numeri_telefono;
    for (int i = 0; i < 100; ++i) {
        numeri_telefono[i] = "123456789" + std::to_string(i);
    }

    // Array di password
    std::array<std::string, 100> password;
    for (int i = 0; i < 100; ++i) {
        password[i] = "P.assword" + std::to_string(i + 1);
    }

    // Array di conferma password
    std::array<std::string, 100> confermaPassword;
    for (int i = 0; i < 100; ++i) {
        confermaPassword[i] = "P.assword" + std::to_string(i + 1);
    }

    // Array di nomi aziende produttrici
    std::array<std::string, 5> nomi_aziende_produttrici = {"Azienda1", "Azienda2", "Azienda3", "Azienda4", "Azienda5"};

    // Array di nomi ditte di spedizioni
    std::array<std::string, 5> nomi_ditte_spedizioni = {"Ditta1", "Ditta2", "Ditta3", "Ditta4", "Ditta5"};

    // Array di vie di residenza
    std::array<std::string, 5> vie_di_residenza = {"Via Roma", "Via Milano", "Via Napoli", "Via Firenze", "Via Venezia"};

    // Array di numeri civici
    std::array<std::string, 100> numeri_civici;
    for (int i = 0; i < 100; ++i) {
        numeri_civici[i] = std::to_string(i + 1);
    }

    // Array di date di nascita
    std::array<std::string, 100> date_di_nascita;
    for (int i = 0; i < 100; ++i) {
        date_di_nascita[i] = "01/01/2000"; // Esempio di data di nascita fittizia
    }

    // Array di CAP
    std::array<std::string, 5> CAP = {"12345", "54321", "67890", "09876", "45678"};

    // Array di città di residenza
    std::array<std::string, 5> citta_di_residenza = {"Roma", "Milano", "Napoli", "Firenze", "Venezia"};


    int i_100C = rand()%100;
    int i_10C = rand()%10;
    int i_5C = rand()%5;

    compratore.effettuaRegistrazione(nomi_utente[i_100C], "UtenteCompratore", 
                                    nomi[i_10C], cognomi[i_10C],
                                    numeri_telefono[i_100C], email[i_100C], 
                                    vie_di_residenza[i_5C], numeri_civici[i_100C], CAP[i_5C], 
                                    citta_di_residenza[i_5C], 
                                    password[i_100C], confermaPassword[i_100C], 
                                    date_di_nascita[i_100C]);


    int i_100F = rand()%100;
    int i_10F = rand()%10;
    int i_5F = rand()%5;

    fornitore.effettuaRegistrazione(nomi_utente[i_100F], "UtenteFornitore", 
                                    nomi[i_10F],  cognomi[i_10F],
                                    numeri_telefono[i_100F], email[i_100F], 
                                    password[i_100F], confermaPassword[i_100F], 
                                    nomi_aziende_produttrici[i_5F]);

    int i_100T = rand()%100;
    int i_10T = rand()%10;
    int i_5T = rand()%5;

    trasportatore.effettuaRegistrazione(nomi_utente[i_100T], "UtenteTrasportatore", 
                                        nomi[i_10T], cognomi[i_10T],
                                        numeri_telefono[i_100T], email[i_100T], 
                                        password[i_100T], confermaPassword[i_100T], 
                                        nomi_ditte_spedizioni[i_5T]);

    return;
}


int main(){
    Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");
    std::cout << "Connessione al database avvenuta con successo." << std::endl;

    //PGresult *res;

    //char sqlcmd[1000]; 

    //test_generator testUtenti;

    //testUtenti.TestAzioniUtenti();
    //test_generator test2;
    //test2.TestCompletoNoUtenti();

    /* init random number generator  */
    srand((unsigned)time(NULL));

    //server_types x;

    // definito per il numero di iterazioni del while:
    int t = 0;           

    int i = 0;

    std::array<UtenteCompratore, 100> utentiCompratori;
    std::array<UtenteFornitore, 100> utentiFornitori;
    std::array<UtenteTrasportatore, 100> utentiTrasportatori;

    while (i<100){

        UtenteCompratore compratore;
        UtenteFornitore fornitore;
        UtenteTrasportatore trasportatore;

        initUSER(compratore, fornitore, trasportatore);

        utentiCompratori[i] = compratore;
        utentiFornitori[i] = fornitore;
        utentiTrasportatori[i] = trasportatore;

        i++;

        /* sleep   */
        micro_sleep(10000);
    }       

    print_select("UtenteCompratore");
    print_select("UtenteFornitore");
    print_select("UtenteTrasportatore");

    
    while (t <= HORIZON){

        

        int index_azioneDaSvolgere = rand()%34;

        //svolgiAzione(compratore, trasportatore, fornitore, index_azioneDaSvolgere);
        
        std::cout << "t: " << t << std::endl; 
        
        // update time 
        t++;

        /* sleep   */
        micro_sleep(500);
    }

    print_select("LogTable");



}