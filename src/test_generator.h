// test_generator.h
#ifndef TESTGENERATOR_H
#define TESTGENERATOR_H

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


    



};



#endif // TESTGENERATOR_H