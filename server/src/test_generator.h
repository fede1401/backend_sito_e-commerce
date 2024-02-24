// test_generator.h
#ifndef TESTGENERATOR_H
#define TESTGENERATOR_H

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
#include "log2db.h"
#include "msleep.h"


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


void initUSER(Con2DB db1, UtenteCompratore compratore, UtenteFornitore fornitore, UtenteTrasportatore trasportatore)
{

    // Array di nomi utente
    std::array<std::string, 100> nomi_utente;
    for (int i = 0; i < 100; ++i)
    {
        nomi_utente[i] = "Utente" + std::to_string(i + 1);
    }

    // Array di nomi
    std::array<std::string, 10> nomi = {"Mario", "Luigi", "Giovanni", "Alessia", "Chiara", "Francesco", "Elena", "Roberto", "Laura", "Paolo"};

    // Array di cognomi
    std::array<std::string, 10> cognomi = {"Rossi", "Bianchi", "Verdi", "Ferrari", "Russo", "Esposito", "Romano", "Gallo", "Conti", "De Luca"};

    // Array di email
    std::array<std::string, 100> email;
    for (int i = 0; i < 100; ++i)
    {
        email[i] = nomi_utente[i] + "@example.com";
    }

    // Array di numeri di telefono
    std::array<std::string, 100> numeri_telefono;
    for (int i = 0; i < 100; ++i)
    {
        numeri_telefono[i] = "123456789" + std::to_string(i);
    }

    // Array di password
    std::array<std::string, 100> password;
    for (int i = 0; i < 100; ++i)
    {
        password[i] = "P.assword" + std::to_string(i + 1);
    }

    // Array di conferma password
    std::array<std::string, 100> confermaPassword;
    for (int i = 0; i < 100; ++i)
    {
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
    for (int i = 0; i < 100; ++i)
    {
        numeri_civici[i] = std::to_string(i + 1);
    }

    // Array di date di nascita
    std::array<std::string, 100> date_di_nascita;
    for (int i = 0; i < 100; ++i)
    {
        date_di_nascita[i] = "01/01/2000"; // Esempio di data di nascita fittizia
    }

    // Array di CAP
    std::array<std::string, 5> CAP = {"12345", "54321", "67890", "09876", "45678"};

    // Array di città di residenza
    std::array<std::string, 5> citta_di_residenza = {"Roma", "Milano", "Napoli", "Firenze", "Venezia"};

    int i_100C = rand() % 100;
    int i_10C = rand() % 10;
    int i_5C = rand() % 5;

    compratore.effettuaRegistrazione(db1, nomi_utente[i_100C], "UtenteCompratore",
                                     nomi[i_10C], cognomi[i_10C],
                                     numeri_telefono[i_100C], email[i_100C],
                                     vie_di_residenza[i_5C], numeri_civici[i_100C], CAP[i_5C],
                                     citta_di_residenza[i_5C],
                                     password[i_100C], confermaPassword[i_100C],
                                     date_di_nascita[i_100C]);

    int i_100F = rand() % 100;
    int i_10F = rand() % 10;
    int i_5F = rand() % 5;

    fornitore.effettuaRegistrazione(db1, nomi_utente[i_100F], "UtenteFornitore",
                                    nomi[i_10F], cognomi[i_10F],
                                    numeri_telefono[i_100F], email[i_100F],
                                    password[i_100F], confermaPassword[i_100F],
                                    nomi_aziende_produttrici[i_5F]);

    int i_100T = rand() % 100;
    int i_10T = rand() % 10;
    int i_5T = rand() % 5;

    trasportatore.effettuaRegistrazione(db1, nomi_utente[i_100T], "UtenteTrasportatore",
                                        nomi[i_10T], cognomi[i_10T],
                                        numeri_telefono[i_100T], email[i_100T],
                                        password[i_100T], confermaPassword[i_100T],
                                        nomi_ditte_spedizioni[i_5T]);

    
    std::cout << "Registrazione effettuata" << std::endl;

    return;
}


void effettua_test(){
    Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");
    std::cout << "Connessione al database avvenuta con successo." << std::endl;

    // PGresult *res;

    // char sqlcmd[1000];

    // test_generator testUtenti;

    // testUtenti.TestAzioniUtenti();
    // test_generator test2;
    // test2.TestCompletoNoUtenti();

    /* init random number generator  */
    srand((unsigned)time(NULL));

    // server_types x;

    // definito per il numero di iterazioni del while:
    int t = 0;

    int i = 0;

    // std::array<UtenteCompratore, 100> utentiCompratori;
    //  std::array<UtenteFornitore, 100> utentiFornitori;
    // std::array<UtenteTrasportatore, 100> utentiTrasportatori;

    UtenteCompratore compratore;
    UtenteFornitore fornitore;
    UtenteTrasportatore trasportatore;

    while (i < 20)
    {

         initUSER(db1, compratore, fornitore, trasportatore);

        // utentiCompratori[i] = compratore;
        // utentiFornitori[i] = fornitore;
        // utentiTrasportatori[i] = trasportatore;

        i++;

        /* sleep   */
        micro_sleep(1000);
    }

    print_select("UtenteCompratore");
    print_select("UtenteFornitore");
    print_select("UtenteTrasportatore");

    UtenteCompratore compratore1;
    UtenteFornitore fornitore1;
    UtenteTrasportatore trasportatore1;

    int n;
    while (t <= HORIZON)
    {

        n = rand() % 5;

        std::cout << "RAND" << n << std::endl;

        sprintf(sqlcmd, "SELECT * FROM UtenteCompratore");
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        compratore1.nome_utente = PQgetvalue(res, n, 0);
        compratore1.session_id = PQgetvalue(res, n, 1);
        compratore1.categoria = PQgetvalue(res, n, 2);
        compratore1.nome = PQgetvalue(res, n, 3);
        compratore1.cognome = PQgetvalue(res, n, 4);
        compratore1.email = PQgetvalue(res, n, 5);
        compratore1.numero_telefono = PQgetvalue(res, n, 6);
        compratore1.password = PQgetvalue(res, n, 7);
        compratore1.data_compleanno = PQgetvalue(res, n, 8);
        compratore1.via_residenza = PQgetvalue(res, n, 9);
        compratore1.numero_civico = PQgetvalue(res, n, 10);
        compratore1.CAP = PQgetvalue(res, n, 11);
        compratore1.città_residenza = PQgetvalue(res, n, 12);
        compratore1.saldo = atof(PQgetvalue(res, n, 13));
        compratore1.stato = atoi(PQgetvalue(res, n, 14));

        std::cout << compratore1.nome_utente << " " << compratore1.session_id << " " << compratore1.categoria << " " << compratore1.nome << " " << compratore1.cognome << " " << compratore1.email << " " << compratore1.numero_telefono << " " << compratore1.password << " " << compratore1.data_compleanno << " " << compratore1.via_residenza << " " << compratore1.numero_civico << " " << compratore1.CAP << " " << compratore1.città_residenza << " " << compratore1.saldo << " " << compratore1.stato << std::endl;


        sprintf(sqlcmd, "SELECT * FROM UtenteFornitore");
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        fornitore1.nome_utente = PQgetvalue(res, n, 0);
        fornitore1.session_id = PQgetvalue(res, n, 1);
        fornitore1.categoria = PQgetvalue(res, n, 2);
        fornitore1.nome = PQgetvalue(res, n, 3);
        fornitore1.cognome = PQgetvalue(res, n, 4);
        fornitore1.email = PQgetvalue(res, n, 5);
        fornitore1.numero_telefono = PQgetvalue(res, n, 6);
        fornitore1.password = PQgetvalue(res, n, 7);
        fornitore1.azienda_produzione = PQgetvalue(res, n, 8);
        fornitore1.stato = atoi(PQgetvalue(res, n, 9));

        sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore");
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        trasportatore1.nome_utente = PQgetvalue(res, n, 0);
        trasportatore1.session_id = PQgetvalue(res, n, 1);
        trasportatore1.categoria = PQgetvalue(res, n, 2);
        trasportatore1.nome = PQgetvalue(res, n, 3);
        trasportatore1.cognome = PQgetvalue(res, n, 4);
        trasportatore1.email = PQgetvalue(res, n, 5);
        trasportatore1.numero_telefono = PQgetvalue(res, n,6);
        trasportatore1.password = PQgetvalue(res, n, 7);
        trasportatore1.ditta_spedizione = PQgetvalue(res, n, 8);
        trasportatore1.stato = atoi(PQgetvalue(res, n, 9));
        trasportatore1.disponibilità = atoi(PQgetvalue(res, n, 10));

        int index_azioneDaSvolgere = rand() % 34;

        //svolgiAzione(db1, compratore1, trasportatore1, fornitore1, index_azioneDaSvolgere);

        std::cout << "t: " << t << std::endl;

        // update time
        t++;

        /* sleep   */
        micro_sleep(500);
    }
}



/*
class test_generator {
public:
    
    void TestRegistrazioneUtenteCompratore(){
        
        // Genera dati di input:
        std::string nome_utente = "test_user1";
        std::string categoria = "UtenteCompratore";
        std::string nome = "NomeTester1";
        std::string cognome = "CognomeTester1";
        std::string numeroTelefono = "1111111111";
        std::string email = "tester1.@gmail.com";
        std::string viaResidenza = "Via della Roma";
        std::string numeroCivico = "22";
        std::string CAP = "00000";
        std::string cittaResidenza = "Roma";
        std::string password = "Aaaaaaa1.";
        std::string confermaPassword = "Aaaaaaa1.";
        std::string dataCompleanno = "01/01/0001";

        // Registrazione utente Compratore:
        UtenteCompratore compratore;
        compratore.effettuaRegistrazione(nome_utente, 
                                        categoria, 
                                        nome, 
                                        cognome, 
                                        numeroTelefono, 
                                        email, 
                                        viaResidenza, 
                                        numeroCivico, 
                                        CAP, 
                                        cittaResidenza, 
                                        password, 
                                        confermaPassword, 
                                        dataCompleanno);

    return;
    }


    void TestRegistrazioneUtenteFornitore(){

        // Genera dati di input:
        std::string nome_utente = "test_user2";
        std::string categoria = "UtenteFornitore";
        std::string nome = "NomeTester2";
        std::string cognome = "CognomeTester2";
        std::string numeroTelefono = "1111111111";
        std::string email = "tester2.@gmail.com";
        std::string password = "Bbbbbbb.2";
        std::string confermaPassword = "Bbbbbbb.2";
        std::string aziendaProduzione = "Nike";

        UtenteFornitore fornitore;
        fornitore.effettuaRegistrazione(nome_utente,
                                        categoria,
                                        nome,
                                        cognome,
                                        numeroTelefono,
                                        email,
                                        password,
                                        confermaPassword,
                                        aziendaProduzione);

    return;
    }


    void TestRegistrazioneUtenteTrasportatore(){
        
        std::string nome_utente = "test_user3";
        std::string categoria = "UtenteTrasportatore";
        std::string nome = "NomeTester3";
        std::string cognome = "CognomeTester3";
        std::string numeroTelefono = "1111111111";
        std::string email = "tester3.@gmail.com";
        std::string password = "Ccccccc.3";
        std::string confermaPassword = "Ccccccc.3";
        std::string dittaSpedizione = "FedEx";

        UtenteTrasportatore trasportatore;
        trasportatore.effettuaRegistrazione(nome_utente,
                                        categoria,
                                        nome,
                                        cognome,
                                        numeroTelefono,
                                        email,
                                        password,
                                        confermaPassword,
                                        dittaSpedizione);

    return;
    }

    
    void TestEffettuaLoginUtenteCompratore(){

        std::string nome_utente = "test_user1";
        std::string password = "Aaaaaaa1.";

        UtenteCompratore compratore;
        compratore.effettua_login(nome_utente, password);
    return;
    }


    void TestEffettuaLoginUtenteFornitore(){

        std::string nome_utente = "test_user2";
        std::string password = "Bbbbbbb.2";

        UtenteFornitore fornitore;
        fornitore.effettua_login(nome_utente, password);
    return;
    }


    void TestEffettuaLoginUtenteTrasportatore(){

        std::string nome_utente = "test_user3";
        std::string password = "Ccccccc.3";

        UtenteTrasportatore trasportatore;
        trasportatore.effettua_login(nome_utente, password);
    return;
    } 


    void TestAggiornaNumeroTelefonoCompratore(){

        UtenteCompratore compratore;
        compratore.nome_utente = "test_user1";

        std::string nuovoNumeroTelefono = "222222222";

        compratore.aggiornaNumeroDiTelefono(nuovoNumeroTelefono);

    return;
    }


    void TestAggiornaNumeroTelefonoFornitore(){

        UtenteFornitore fornitore;
        fornitore.nome_utente = "test_user2";

        std::string nuovoNumeroTelefono = "33333333";

        fornitore.aggiornaNumeroDiTelefono(nuovoNumeroTelefono);
    return;
    }


    void TestAggiornaNumeroTelefonoTrasportatore(){
        UtenteTrasportatore trasportatore;
        trasportatore.nome_utente = "test_user3";

        std::string nuovoNumeroTelefono = "444444444";

        trasportatore.aggiornaNumeroDiTelefono(nuovoNumeroTelefono);
    }


    void TestAggiornaPasswordUtenteCompratore(){
        UtenteCompratore compratore;
        compratore.nome_utente = "test_user1";

        std::string vecchiaPassw = "Aaaaaaa1.";
        std::string nuovaPassw = "AoAoAoAo1.";
        compratore.aggiornaPassword(vecchiaPassw, nuovaPassw);

    return;
    }


    void TestAggiornaPasswordUtenteFornitore(){
        UtenteFornitore fornitore;
        fornitore.nome_utente = "test_user2";

        std::string vecchiaPassw = "Bbbbbbb.2";
        std::string nuovaPassw = "BoBoBoBo2.";
        fornitore.aggiornaPassword(vecchiaPassw, nuovaPassw);

    return;
    }

    void TestAggiornaPasswordUtenteTrasportatore(){
        UtenteTrasportatore trasportatore;
        trasportatore.nome_utente = "test_user3";

        std::string vecchiaPassw = "Ccccccc.3";
        std::string nuovaPassw = "CoCoCoCo.3";
        trasportatore.aggiornaPassword(vecchiaPassw, nuovaPassw);
    return;
    }



    void TestAggiornaResidenzaCompratore(){
        UtenteCompratore compratore;
        compratore.nome_utente = "test_user1";

        std::string viaResidenza = "Via delle Cicale";
        std::string numeroCivico = "00";
        std::string CAP = "909090";
        std::string CittaResidenza = "Milano";

        compratore.aggiornaResidenza(viaResidenza,numeroCivico,CAP,CittaResidenza);
    }


    void TestAggiornaNomeDittaSpedizioneTrasportatore(){
        UtenteTrasportatore trasportatore;
        trasportatore.nome_utente = "test_user3";

        std::string dittaSpedizione = "Bartolini";
        trasportatore.aggiornaNomeDittaSpedizione(dittaSpedizione);

        return;
    }


    void TestAggiornaNomeAziendaProduttriceFornitore(){
        UtenteFornitore fornitore;
        fornitore.nome_utente = "test_user2";

        std::string aziendaProduttrice = "Apple";
        fornitore.aggiornaNomeAziendaProduttrice(aziendaProduttrice);

        return;
    }




    void TestEffettuaLogoutCompratore(){
        UtenteCompratore compratore;
        compratore.nome_utente = "test_user1";

        compratore.effettua_logout(compratore.nome_utente);
        return;
    }

    void TestEffettuaLogoutFornitore(){
        UtenteFornitore fornitore;
        fornitore.nome_utente = "test_user2";

        fornitore.effettua_logout(fornitore.nome_utente);
        return;        
    }

    void TestEffettuaLogoutTrasportatore(){
        UtenteTrasportatore trasportatore;
        trasportatore.nome_utente = "test_user3";

        trasportatore.effettua_logout(trasportatore.nome_utente);
        return;   
    }


    void TestEliminaProfiloCompratore(){
        UtenteCompratore compratore;
        compratore.nome_utente = "test_user1";
        compratore.categoria = "UtenteCompratore";

        compratore.elimina_profilo();
        return;
    }

    void TestEliminaProfiloFornitore(){
        UtenteFornitore fornitore;
        fornitore.nome_utente = "test_user2";
        fornitore.categoria = "UtenteFornitore";

        fornitore.elimina_profilo();
        return;
    }


    void TestEliminaProfiloTrasportatore(){
        UtenteTrasportatore trasportatore;
        trasportatore.nome_utente = "test_user3";
        trasportatore.categoria = "UtenteTrasportatore";

        trasportatore.elimina_profilo();
        return;
    }


    void TestAggiungiProdottoSito(){
        Product prodotto;

        std::string nome = "Iphone 15";
        std::string categoria = "Telefonia";
        float prezzoEuro = 990.00;
        std::string descrizione = "Iphone 15, colore bianco, 16GB RAM e 1TB archiviazione";
        std::string aziendaProduttrice = "Apple";
        int copieDisponibili = 10;

        prodotto.add_new_product(nome, categoria, prezzoEuro, descrizione, aziendaProduttrice, copieDisponibili);
        return;
    }

    void TestRimuoviProdottoSito(){
        Product prodotto;
        int codProdotto = 1;

        prodotto.remove_prodotto(codProdotto);
        return;
    }


    void TestAggiungiProdottoCarrello(){
        Carrello carrello;

        std::string nome_utente_compratore = "test_user1";
        int codProdotto = 1;

        carrello.add_prodotto(nome_utente_compratore, codProdotto);

        return; 
    }

    void TestRimuoviProdottoCarrello(){
        Carrello carrello;

        std::string nome_utente_compratore = "test_user1";
        int codProdotto = 1;

        carrello.remove_prodotto(nome_utente_compratore, codProdotto);

        return; 
    }

    void TestAggiungiProdottoListaDesideri(){
        ListaDesideri listaDesideri;

        std::string nome_utente_compratore = "test_user1";
        int codProdotto = 1;

        listaDesideri.add_prodotto(nome_utente_compratore, codProdotto);

        return; 
    }

    void TestRimuoviProdottoListaDesideri(){
        ListaDesideri listaDesideri;

        std::string nome_utente_compratore = "test_user1";
        int codProdotto = 1;

        listaDesideri.remove_prodotto(nome_utente_compratore, codProdotto);

        return; 
    }


    void TestAggiungiCartaDiPagamento(){
        Carta carta;
        carta.nome_utente = "test_user1";
        carta.numero_carta = "23839274927492";
        carta.cvv = "111";

        carta.aggiungi_carta(carta.nome_utente, carta.numero_carta, carta.cvv);
        return;
    }

    void TestRimuoviCartaDiPagamento(){
        Carta carta;

        int idCarta = 1;

        carta.remove_carta(1);
    }


    void TestRicercaProdotto(){
        
        Product prodotto;
        std::string nomeProdotto = "Iphone 15";

        prodotto.ricerca_mostra_Prodotto(nomeProdotto);

        return;
    }


    void TestAcquistaProdotto(){
        Product prodotto;

        std::string nome_utente_compratore = "test_user1";
        std::string viaSpedizione = "Via delle Cicale";
        std::string numeroCivico = "00";
        std::string cittaSpedizione = "Milano";

        prodotto.acquistaProdotto(nome_utente_compratore, viaSpedizione, cittaSpedizione, numeroCivico);

        return;
    }


    void TestVisionaOrdiniEffettuati(){
        Ordine ordine;
        std::string nome_utente_compratore = "test_user1";

        ordine.visione_ordini_effettuati(nome_utente_compratore);

        return;
    }

    void TestAnnullaOrdine(){
        Ordine ordine;
        int idOrdine= 1;

        ordine.annulla_ordine(idOrdine);
    return;
    }

    void TestAssegnaTrasportatoreAOrdine(){
        Spedizione spedizione;
        spedizione.assegnaOrdineTrasportatore();
        return;
    }


    void TestAvvisaSpedizioneConsegnata(){
        Spedizione spedizione;

        int idSpedizione = 1;
        spedizione.spedizioneConsegnata(idSpedizione);

    return;
    }


    void TestEffettuaRecensione(){
        Recensione recensione;
        int idOrdine = 1;
        std::string descrizione = "Ottimo Prodotto";
        votoStelle voto_stelle;
        voto_stelle = votoStelle::Quattro;

        recensione.effettuaRecensione(idOrdine, descrizione, voto_stelle);

    return;
    }


    void TestRimuoviRecensione(){
        Recensione recensione;
        int idRecensione = 1;

        recensione.remove_recensione(idRecensione);

        return;
    }


    void TestEffettuaReso(){
        Reso reso;
        int idOrdine = 1;
        motivazioneReso motivazione_reso;
        motivazione_reso = motivazioneReso::Difettoso;

        reso.effettuaReso(idOrdine, motivazione_reso);

        return;
    }


    
    void TestAzioniUtenti(){

        std::string nome_utente_c = "test_user1";
        std::string categoria_c = "UtenteCompratore";
        std::string nome_c = "NomeTester1";
        std::string cognome_c = "CognomeTester1";
        std::string numeroTelefono_c = "1111111111";
        std::string email_c = "tester1.@gmail.com";
        std::string viaResidenza_c = "Via della Roma";
        std::string numeroCivico_c = "22";
        std::string CAP_c = "00000";
        std::string cittaResidenza_c = "Roma";
        std::string password_c = "Aaaaaaa1.";
        std::string confermaPassword_c = "Aaaaaaa1.";
        std::string dataCompleanno_c = "01/01/0001";

        // Registrazione utente Compratore:
        UtenteCompratore compratore;
        compratore.effettuaRegistrazione(nome_utente_c, 
                                        categoria_c, 
                                        nome_c, 
                                        cognome_c, 
                                        numeroTelefono_c, 
                                        email_c, 
                                        viaResidenza_c, 
                                        numeroCivico_c, 
                                        CAP_c, 
                                        cittaResidenza_c, 
                                        password_c, 
                                        confermaPassword_c, 
                                        dataCompleanno_c);


         // Genera dati di input:
        std::string nome_utente_f = "test_user2";
        std::string categoria_f = "UtenteFornitore";
        std::string nome_f = "NomeTester2";
        std::string cognome_f = "CognomeTester2";
        std::string numeroTelefono_f = "1111111111";
        std::string email_f = "tester2.@gmail.com";
        std::string password_f = "Bbbbbbb.2";
        std::string confermaPassword_f = "Bbbbbbb.2";
        std::string aziendaProduzione_f = "Nike";

        UtenteFornitore fornitore;
        fornitore.effettuaRegistrazione(nome_utente_f,
                                        categoria_f,
                                        nome_f,
                                        cognome_f,
                                        numeroTelefono_f,
                                        email_f,
                                        password_f,
                                        confermaPassword_f,
                                        aziendaProduzione_f);


        std::string nome_utente_t = "test_user3";
        std::string categoria_t = "UtenteTrasportatore";
        std::string nome_t = "NomeTester3";
        std::string cognome_t = "CognomeTester3";
        std::string numeroTelefono_t = "1111111111";
        std::string email_t = "tester3.@gmail.com";
        std::string password_t = "Ccccccc.3";
        std::string confermaPassword_t = "Ccccccc.3";
        std::string dittaSpedizione_t = "FedEx";

        UtenteTrasportatore trasportatore;
        trasportatore.effettuaRegistrazione(nome_utente_t,
                                        categoria_t,
                                        nome_t,
                                        cognome_t,
                                        numeroTelefono_t,
                                        email_t,
                                        password_t,
                                        confermaPassword_t,
                                        dittaSpedizione_t);


        compratore.effettua_login("test_user1", "Aaaaaaa1.");

        fornitore.effettua_login("test_user2", "Bbbbbbb.2");

        trasportatore.effettua_login("test_user3", "Ccccccc.3");

        print_select("UtenteCompratore");
        print_select("UtenteFornitore");
        print_select("UtenteTrasportatore");

        compratore.aggiornaNumeroDiTelefono("222222222");

        fornitore.aggiornaNumeroDiTelefono("33333333");


        std::string vecchiaPassw_c = "Aaaaaaa1.";
        std::string nuovaPassw_c = "AoAoAoAo1.";
        compratore.aggiornaPassword(vecchiaPassw_c, nuovaPassw_c);


        // Passsword errata
        vecchiaPassw_c = "Aaaaaaa1.sddsd";
        nuovaPassw_c = "AoAoAoAo1.";
        compratore.aggiornaPassword(vecchiaPassw_c, nuovaPassw_c);


        std::string vecchiaPassw_f = "Bbbbbbb.2";
        std::string nuovaPassw_f = "BoBoBoBo2.";
        fornitore.aggiornaPassword(vecchiaPassw_f, nuovaPassw_f);


        viaResidenza_c = "Via delle Cicale";
        numeroCivico_c = "00";
        CAP_c = "909090";
        std::string CittaResidenza_c = "Milano";

        compratore.aggiornaResidenza(viaResidenza_c,numeroCivico_c,CAP_c,CittaResidenza_c);


        dittaSpedizione_t = "Bartolini";
        trasportatore.aggiornaNomeDittaSpedizione(dittaSpedizione_t);


        std::string aziendaProduttrice_f = "Apple";
        fornitore.aggiornaNomeAziendaProduttrice(aziendaProduttrice_f);

        std::cout << "Aggiornamenti! " << std::endl; 

        print_select("UtenteCompratore");
        print_select("UtenteFornitore");
        print_select("UtenteTrasportatore");

        compratore.effettua_logout(compratore.nome_utente);

        fornitore.effettua_logout(fornitore.nome_utente);

        trasportatore.effettua_logout(trasportatore.nome_utente);


        std::cout << "Log out! " << std::endl; 

        print_select("UtenteCompratore");
        print_select("UtenteFornitore");
        print_select("UtenteTrasportatore");

        compratore.elimina_profilo();

        fornitore.elimina_profilo();

        trasportatore.elimina_profilo();


        std::cout << "Eliminazione profilo! " << std::endl; 

        print_select("UtenteCompratore");
        print_select("UtenteFornitore");
        print_select("UtenteTrasportatore");


        print_select("LogTable");

        return;

        
        Risultati ottenuti:
        Tabella: LogTable
        Row 0: 2024-02-15 17:36:38,   8075,   INFO,   Utente compratore inserito.,   ,   
        Row 1: 2024-02-15 17:36:38,   8075,   INFO,   Utente fornitore inserito.,   ,   
        Row 2: 2024-02-15 17:36:38,   8075,   INFO,   Utente trasportatore inserito.,   ,   
        Row 3: 2024-02-15 17:36:38,   8075,   INFO,   Aggiornamento sessionID,   ThpjltNlpt,   
        Row 4: 2024-02-15 17:36:38,   8075,   INFO,   Aggiornamento dello stato utente,   ThpjltNlpt,   
        Row 5: 2024-02-15 17:36:38,   8075,   INFO,   Aggiornamento sessionID,   RboJeMgvPP,   
        Row 6: 2024-02-15 17:36:38,   8075,   INFO,   Aggiornamento dello stato utente,   RboJeMgvPP,   
        Row 7: 2024-02-15 17:36:38,   8075,   INFO,   Aggiornamento sessionID,   JukdRMYZRC,   
        Row 8: 2024-02-15 17:36:38,   8075,   INFO,   Aggiornamento dello stato utente,   JukdRMYZRC,   
        Row 9: 2024-02-15 17:36:38,   8075,   INFO,   Aggiornamento numero di telefono.,   ThpjltNlpt,   
        Row 10: 2024-02-15 17:36:38,   8075,   INFO,   Aggiornamento numero di telefono.,   RboJeMgvPP,   
        Row 11: 2024-02-15 17:36:38,   8075,   INFO,   Aggiornamento password.,   ThpjltNlpt,   
        Row 12: 2024-02-15 17:36:38,   8075,   ERROR,   La password attuale inserita non è corretta.,   ThpjltNlpt,   
        Row 13: 2024-02-15 17:36:38,   8075,   INFO,   Aggiornamento password.,   RboJeMgvPP,   
        Row 14: 2024-02-15 17:36:38,   8075,   INFO,   Aggiornata la residenza utente compratore.,   ThpjltNlpt,   
        Row 15: 2024-02-15 17:36:38,   8075,   INFO,   Aggiornamento ditta spedizione,   JukdRMYZRC,   
        Row 16: 2024-02-15 17:36:38,   8075,   INFO,   Aggiornamento azienda produttrice,   RboJeMgvPP,   
        Row 17: 2024-02-15 17:36:38,   8075,   INFO,   Disconnessione utente,   ThpjltNlpt,   
        Row 18: 2024-02-15 17:36:38,   8075,   INFO,   Aggiornamento sessionID,   ,   
        Row 19: 2024-02-15 17:36:38,   8075,   INFO,   Disconnessione utente,   RboJeMgvPP,   
        Row 20: 2024-02-15 17:36:38,   8075,   INFO,   Aggiornamento sessionID,   ,   
        Row 21: 2024-02-15 17:36:38,   8075,   INFO,   Disconnessione utente,   JukdRMYZRC,   
        Row 22: 2024-02-15 17:36:38,   8075,   INFO,   Aggiornamento sessionID,   ,   
        Row 23: 2024-02-15 17:36:38,   8075,   INFO,   Eliminazione profilo.,   ,   
        Row 24: 2024-02-15 17:36:38,   8075,   INFO,   Eliminazione profilo.,   ,   
        Row 25: 2024-02-15 17:36:38,   8075,   INFO,   Eliminazione profilo.,   ,
        
    }


    void TestCompletoNoUtenti(){
        std::string nome_utente_c = "test_user1";
        std::string categoria_c = "UtenteCompratore";
        std::string nome_c = "NomeTester1";
        std::string cognome_c = "CognomeTester1";
        std::string numeroTelefono_c = "1111111111";
        std::string email_c = "tester1.@gmail.com";
        std::string viaResidenza_c = "Via della Roma";
        std::string numeroCivico_c = "22";
        std::string CAP_c = "00000";
        std::string cittaResidenza_c = "Roma";
        std::string password_c = "Aaaaaaa1.";
        std::string confermaPassword_c = "Aaaaaaa1.";
        std::string dataCompleanno_c = "01/01/0001";

        // Registrazione utente Compratore:
        UtenteCompratore compratore;
        compratore.effettuaRegistrazione(nome_utente_c, 
                                        categoria_c, 
                                        nome_c, 
                                        cognome_c, 
                                        numeroTelefono_c, 
                                        email_c, 
                                        viaResidenza_c, 
                                        numeroCivico_c, 
                                        CAP_c, 
                                        cittaResidenza_c, 
                                        password_c, 
                                        confermaPassword_c, 
                                        dataCompleanno_c);


         // Genera dati di input:
        std::string nome_utente_f = "test_user2";
        std::string categoria_f = "UtenteFornitore";
        std::string nome_f = "NomeTester2";
        std::string cognome_f = "CognomeTester2";
        std::string numeroTelefono_f = "1111111111";
        std::string email_f = "tester2.@gmail.com";
        std::string password_f = "Bbbbbbb.2";
        std::string confermaPassword_f = "Bbbbbbb.2";
        std::string aziendaProduzione_f = "Nike";

        UtenteFornitore fornitore;
        fornitore.effettuaRegistrazione(nome_utente_f,
                                        categoria_f,
                                        nome_f,
                                        cognome_f,
                                        numeroTelefono_f,
                                        email_f,
                                        password_f,
                                        confermaPassword_f,
                                        aziendaProduzione_f);


        std::string nome_utente_t = "test_user3";
        std::string categoria_t = "UtenteTrasportatore";
        std::string nome_t = "NomeTester3";
        std::string cognome_t = "CognomeTester3";
        std::string numeroTelefono_t = "1111111111";
        std::string email_t = "tester3.@gmail.com";
        std::string password_t = "Ccccccc.3";
        std::string confermaPassword_t = "Ccccccc.3";
        std::string dittaSpedizione_t = "FedEx";

        UtenteTrasportatore trasportatore;
        trasportatore.effettuaRegistrazione(nome_utente_t,
                                        categoria_t,
                                        nome_t,
                                        cognome_t,
                                        numeroTelefono_t,
                                        email_t,
                                        password_t,
                                        confermaPassword_t,
                                        dittaSpedizione_t);


        compratore.effettua_login("test_user1", "Aaaaaaa1.");

        fornitore.effettua_login("test_user2", "Bbbbbbb.2");

        trasportatore.effettua_login("test_user3", "Ccccccc.3");

        print_select("UtenteCompratore");
        print_select("UtenteFornitore");
        print_select("UtenteTrasportatore");

        Product prodotto;
        Ordine ordine;
        prodotto.add_new_product("Dunk Panda", "Abbigliamento, scarpe", 99.00, "Nike Panda, taglia 40", "Nike", 10);

        Carrello carrello;
        ListaDesideri listadesideri;

        carrello.add_prodotto("test_user1", 1);
        listadesideri.add_prodotto("test_user1", 1);

        ordine = prodotto.acquistaProdotto("test_user1", "Via delle Mani", "Roma", "44");

        ordine.visione_ordini_effettuati("test_user1");

        Spedizione spedizione;
        spedizione.assegnaOrdineTrasportatore();

        print_select("Spedizione");

        spedizione.spedizioneConsegnata(1);

        print_select("Spedizione");

        Recensione recensione;
        Reso reso;

        votoStelle votoStelle;
        votoStelle = votoStelle::Quattro;
        recensione.effettuaRecensione(1, "Buonissimo prodotto", votoStelle);

        motivazioneReso motivazioneReso;
        motivazioneReso = motivazioneReso::Difettoso;
        reso.effettuaReso(1, motivazioneReso);

        print_select("Ordine");
        print_select("Prodotto");
        print_select("Spedizione");
        print_select("Reso");
        print_select("Recensione");
        print_select("LogTable");

        return;

        
        Risultati ottenuti:
        Row 0: 2024-02-16 22:03:44,   7751,   INFO,   Utente compratore inserito.,   ,   
        Row 1: 2024-02-16 22:03:44,   7751,   INFO,   Utente fornitore inserito.,   ,   
        Row 2: 2024-02-16 22:03:44,   7751,   INFO,   Utente trasportatore inserito.,   ,   
        Row 3: 2024-02-16 22:03:44,   7751,   INFO,   Aggiornamento sessionID,   bkZKmrLJsy,   
        Row 4: 2024-02-16 22:03:44,   7751,   INFO,   Aggiornamento dello stato utente,   bkZKmrLJsy,   
        Row 5: 2024-02-16 22:03:44,   7751,   INFO,   Aggiornamento sessionID,   HkZxJbPmXi,   
        Row 6: 2024-02-16 22:03:44,   7751,   INFO,   Aggiornamento dello stato utente,   HkZxJbPmXi,   
        Row 7: 2024-02-16 22:03:44,   7751,   INFO,   Aggiornamento sessionID,   OtOVxPzjuv,   
        Row 8: 2024-02-16 22:03:44,   7751,   INFO,   Aggiornamento dello stato utente,   OtOVxPzjuv,   
        Row 9: 2024-02-16 22:03:45,   7751,   INFO,   Inserito prodotto,   HkZxJbPmXi,   
        Row 10: 2024-02-16 22:03:45,   7751,   INFO,   Inserimento del prodotto nel db.,   bkZKmrLJsy,   
        Row 11: 2024-02-16 22:03:45,   7751,   INFO,   Inserimento del prodotto nel db.,   bkZKmrLJsy,   
        Row 12: 2024-02-16 22:03:45,   7751,   INFO,   Utente ha acquistato il prodotto, ordine inserito nel db,   bkZKmrLJsy,   
        Row 13: 2024-02-16 22:03:45,   7751,   INFO,   Visione degli ordini da parte dell utente.,   bkZKmrLJsy,   
        Row 14: 2024-02-16 22:03:45,   7751,   INFO,   Assegnato ordine al trasportatore,   OtOVxPzjuv,   
        Row 15: 2024-02-16 22:03:45,   7751,   INFO,   Modificata disponibilità utente trasportatore,   OtOVxPzjuv,   
        Row 16: 2024-02-16 22:03:45,   7751,   INFO,   Avviata spedizione ordine,   OtOVxPzjuv,   
        Row 17: 2024-02-16 22:03:45,   7751,   INFO,   Disponibilità utente trasportatore per prendere in consegna un nuovo pacco,   OtOVxPzjuv,   
        Row 18: 2024-02-16 22:03:45,   7751,   INFO,   Effettuata recensione compratore,   bkZKmrLJsy,   
        Row 19: 2024-02-16 22:03:45,   7751,   INFO,   Effettuata reso del prodotto,   bkZKmrLJsy,
        

    }

};

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

    
    Risultati ottenuti;
    Tabella: UtenteCompratore
    Row 0: test_user1,   UtenteCompratore,   Alice,   Rossi,   alice.rossi@example.com,   1234567890,   P@ssw0rd!,   1990-05-15,   Via Roma,   123,   00100,   Roma,   0.00,   0,   


    Tabella: UtenteCompratore
    Row 0: test_user1,   UtenteCompratore,   Alice,   Rossi,   alice.rossi@example.com,   1111111111,   P@ssw0rd!,   1990-05-15,   Via Roma,   123,   00100,   Roma,   0.00,   0,  
    
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
    
    Risultati:
    Tabella: UtenteCompratore
    Row 0: test_user1,   UtenteCompratore,   Alice,   Rossi,   alice.rossi@example.com,   555555555,   P@ssw0rd!,   1990-05-15,   Via Roma,   123,   00100,   Roma,   0.00,   0,   


    Tabella: UtenteCompratore
    Row 0: test_user1,   UtenteCompratore,   Alice,   Rossi,   alice.rossi@example.com,   555555555,   Mortadella444.,   1990-05-15,   Via Roma,   123,   00100,   Roma,   0.00,   0,  
    
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

    
    Risultati ottenuti:
    Tabella: UtenteCompratore
    Row 0: test_user1,   UtenteCompratore,   Alice,   Rossi,   alice.rossi@example.com,   555555555,   Mortadella444.,   
    1990-05-15,   Via Roma,   123,   00100,   Roma,   0.00,   0,   


    Tabella: UtenteCompratore
    Row 0: test_user1,   UtenteCompratore,   Alice,   Rossi,   alice.rossi@example.com,   555555555,   Mortadella444.,   
    1990-05-15,   Via Salerno,   000,   01010,   Salerno,   0.00,   0,
    
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

    
    Risultati ottenuti:
    Tabella: utenteFornitore
    Row 0: test_user2,   UtenteFornitore,   Biagio,   Anocacci,   biagio.anocacci@gmail.com,   333445567,   Candela111?,   Nike,   0,   


    Tabella: utenteFornitore
    Row 0: test_user2,   UtenteFornitore,   Biagio,   Anocacci,   biagio.anocacci@gmail.com,   333445567,   Candela111?,   Adidas,   0, 
    

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

    Risultati ottenuti:
    Tabella: UtenteTrasportatore
    Row 0: test_user3,   UtenteTrasportatore,   Marco,   Verdi,   marco.verdi@gmail.com,   345678900,   Test1234.,   FedEx,   0,   


    Tabella: UtenteTrasportatore
    Row 0: test_user3,   UtenteTrasportatore,   Marco,   Verdi,   marco.verdi@gmail.com,   345678900,   Test1234.,   Bartolini,   0,   
    

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

*/


#endif // TESTGENERATOR_H