// test_generator.h
#ifndef TESTGENERATOR_H
#define TESTGENERATOR_H

#include "main.h"
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

        /* 
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
        */
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

        /*
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
        */

    }

};




#endif // TESTGENERATOR_H