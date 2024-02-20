#ifndef SVOLGIAZIONE_H
#define SVOLGIAZIONE_H


#include "test_generator.h"


std::vector<std::string> server_types = {
        "COMPRAVENDITA", "RICERCA", "COMPRATORE_UPD_RESIDENZA", "COMPR_UPD_NUMTEL", "COMPR_UPD_PASSWORD", "COMPR_LOGOUT", "COMPR_LOGIN", "COMPR_ELIMINAPROFILO", 
        "COMPR_CONTROLLA_ORDINI_EFFETTUATI", "INSERT_CARRELLO", "REMOVE_CARRELLO", "INSERT_LISTA_DESIDERI", "REMOVE_LISTA_DESIDERI", "AGGIUNTA_CARTA_PAGAMENTO",
        "EFFETTUA_RECENSIONE", "RIMUOVE_RECENSIONE", "EFFETTUA_RESO", "ANNULLA_ORDINE",
        "FORN_UPD_PASSWORD", "FORN_LOGOUT", "FORN_LOGIN", "FORN_ELIMINAPROFILO", "FORN_UPD_NUMTEL", "FORN_INSERT_PRODOTTO", "FORN_REMOVE_PRODOTTO", "FORN_UPD_AZIENDA"
        "TRASP_UPD_PASSWORD", "TRASP_LOGOUT", "TRASP_LOGIN", "TRASP_ELIMINAPROFILO", "TRASP_UPD_NUMTEL", "TRASP_UPD_DITTA", "TRASP_AVVIA_SPED", "TRASP_AVVISA_SPEDITO"
    };


void svolgiAzione(UtenteCompratore compratore, UtenteTrasportatore trasportatore, UtenteFornitore fornitore, int index_azioneDaSvolgere){

    std::string azione = server_types[index_azioneDaSvolgere];

    // Array di nomi utente
    std::array<std::string, 100> nomi_utente;
    for (int i = 0; i < 100; ++i) {
        nomi_utente[i] = "Utente" + std::to_string(i + 1);
    }

     // Array di vie di residenza
    std::array<std::string, 5> vie_di_residenza = {"Via Roma", "Via Milano", "Via Napoli", "Via Firenze", "Via Venezia"};

    // Array di numeri civici
    std::array<std::string, 100> numeri_civici;
        for (int i = 0; i < 100; ++i) {
            numeri_civici[i] = std::to_string(i + 1);
        }

    // Array di CAP
    std::array<std::string, 5> CAP = {"12345", "54321", "67890", "09876", "45678"};

    // Array di città di residenza
    std::array<std::string, 5> citta_di_residenza = {"Roma", "Milano", "Napoli", "Firenze", "Venezia"};

    // Array di vie spedizione
    std::array<std::string, 5> arr_vie_spedizione = {"Via Roma", "Via Milano", "Via Napoli", "Via Firenze", "Via Venezia"};

    // Array cittaspedizione
    std::array<std::string, 5> arr_citta_spedizione = {"Roma", "Milano", "Napoli", "Firenze", "Venezia"};

    // Array Prodotti:
    std::array<std::string, 100> arr_prodotti;
    for (int i = 0; i < 100; ++i) {
        arr_prodotti[i] = "Prodotto" + std::to_string(i + 1);
    }

    // Array di password
    std::array<std::string, 100> password;
    for (int i = 0; i < 100; ++i) {
        password[i] = "password" + std::to_string(i + 1);
    }


    // Array di numeri di telefono casuali
    std::array<std::string, 100> numeri_di_telefono;

    // Genera i cento numeri di telefono casuali
    for (int i = 0; i < 100; ++i) {
        std::string numero = "39"; // Inizia con il prefisso italiano +39
        
        // Genera le restanti 10 cifre casuali
        for (int j = 0; j < 10; ++j) {
            numero += std::to_string(rand() % 10); // Aggiunge una cifra casuale compresa tra 0 e 9
        }
        
        numeri_di_telefono[i] = numero;
    }


    // Array di numeri di carte di pagamento:
    std::array<std::string, 100> carte_pagamento;
    for (int i = 0; i < 100; ++i) {
        std::string carta = ""; 
        
        // Genera le restanti 10 cifre casuali
        for (int j = 0; j < 10; ++j) {
            carta += std::to_string(rand() % 10); // Aggiunge una cifra casuale compresa tra 0 e 9
        }
        
        carte_pagamento[i] = carta;
    }

    // Array di numeri di carte di pagamento:
    std::array<std::string, 100> cvv_cartePagamento;
    for (int i = 0; i < 100; ++i) {
        std::string cvv = ""; 
        
        // Genera le restanti 10 cifre casuali
        for (int j = 0; j < 3; ++j) {
            cvv += std::to_string(rand() % 3); // Aggiunge una cifra casuale compresa tra 0 e 9
        }
        
        cvv_cartePagamento[i] = cvv;
    }


    // Descrizioni recensione;
    std::array<std::string, 5> recensioni = {"Buon prodotto", "Ottimo prodotto", "Prodotto mediocre", "Prodotto non buono", "Prodotto di scarsa qualità"};

    // Voto stelle
    std::array<votoStelle, 5> votoStelle = {votoStelle::Uno, votoStelle::Due, votoStelle::Tre, votoStelle::Quattro, votoStelle::Cinque};


    // Motivazioni reso
    std::array<motivazioneReso, 4> motivazioniReso = {motivazioneReso::CambioOpinione, motivazioneReso::Difettoso, motivazioneReso::MisuraErrata, motivazioneReso::NonConformeAlleAspettative};


    std::array<std::string, 5> categoriaProdotti = {"Telefonia", "Abbigliamento e scarpe", "Informatica", "Oggetti Automobili", "Sportivo" };

    // Array di nomi aziende produttrici
    std::array<std::string, 5> nomi_aziende_produttrici = {"Azienda1", "Azienda2", "Azienda3", "Azienda4", "Azienda5"};

    // Array di nomi ditte di spedizioni
    std::array<std::string, 5> nomi_ditte_spedizioni = {"Ditta1", "Ditta2", "Ditta3", "Ditta4", "Ditta5"};


    // Definisco 3 indici casuali
    int i4 = rand()%4;
    int i5 = rand()%5;
    int i100 = rand()%100;


    // In base all'azione da fare faccio qualcosa
    if (azione == "COMPRAVENDITA"){
        Product prodotto;
        Ordine ordine = prodotto.acquistaProdotto(compratore.nome_utente, arr_vie_spedizione[i5], arr_citta_spedizione[i5], numeri_civici[i100]);
    }


    if (azione == "RICERCA"){
        Product prodotto;
        prodotto.ricerca_mostra_Prodotto(arr_prodotti[i100]);
    }


    if (azione == "COMPRATORE_UPD_RESIDENZA"){
        compratore.aggiornaResidenza(vie_di_residenza[i5], numeri_civici[i100], CAP[i5], citta_di_residenza[i5]);
    }

    
    if (azione == "COMPR_UPD_NUMTEL"){
        compratore.aggiornaNumeroDiTelefono(numeri_di_telefono[i100]);
    }


    if (azione == "COMPR_UPD_PASSWORD"){
        compratore.aggiornaPassword(compratore.password, password[i100]);
    }

    if (azione == "COMPR_LOGOUT"){
        compratore.effettua_logout(compratore.nome_utente);
    }

    if (azione == "COMPR_LOGIN"){
        compratore.effettua_login(nomi_utente[i100], password[i100]);
    }

    if (azione == "COMPR_ELIMINAPROFILO"){
        compratore.elimina_profilo();
    }

    if (azione == "COMPR_CONTROLLA_ORDINI_EFFETTUATI"){
        Ordine ordine;
        ordine.visione_ordini_effettuati(compratore.nome_utente);
    }

    // Sistema parametro
    if (azione == "INSERT_CARRELLO"){
        Carrello carrello;
        carrello.add_prodotto(compratore.nome_utente, rand()%100); // da sistemare, inserire come secondo parametro il nome del prodotto.
    }

    // Sistema parametro
    if (azione == "REMOVE_CARRELLO"){
        Carrello carrello;
        carrello.remove_prodotto(compratore.nome_utente, rand()%100);
    }

    // Sistema parametro
    if (azione == "INSERT_LISTA_DESIDERI"){
        ListaDesideri listaDesideri;
        listaDesideri.add_prodotto(compratore.nome_utente, rand()%100); // da sistemare, inserire come secondo parametro il nome del prodotto.
    }

    // Sistema parametro

    if (azione == "REMOVE_LISTA_DESIDERI"){
        ListaDesideri listaDesideri;
        listaDesideri.remove_prodotto(compratore.nome_utente, rand()%100);
    }


    if (azione == "AGGIUNTA_CARTA_PAGAMENTO"){
        Carta carta;
        carta.aggiungi_carta(compratore.nome_utente, carte_pagamento[i100], cvv_cartePagamento[i100]);
    }

    // Sistema parametro
    if (azione == "EFFETTUA_RECENSIONE"){
        Recensione recensione;
        recensione.effettuaRecensione(rand()%100, recensioni[i5], votoStelle[i5]);
    }

    // Sistema parametro
    if (azione == "RIMUOVE_RECENSIONE"){
        Recensione recensione;
        recensione.remove_recensione(rand()%100);
    }

    // Sistema parametro
    if (azione == "EFFETTUA_RESO"){
        Reso reso;
        reso.effettuaReso(rand()%100, motivazioniReso[i4]);
    }


    // Sistema parametro
    if (azione == "ANNULLA_ORDINE"){
        Ordine ordine;
        ordine.annulla_ordine(rand()%100);
    }


    if (azione == "FORN_UPD_PASSWORD"){
        fornitore.aggiornaPassword(fornitore.password, password[i100]);
    }

    if (azione == "FORN_LOGOUT"){
        fornitore.effettua_logout(fornitore.nome_utente);
    }

    if (azione == "FORN_LOGIN"){
        fornitore.effettua_login(nomi_utente[i100], password[i100]);
    }

    if (azione == "FORN_ELIMINAPROFILO"){
        fornitore.elimina_profilo();
    }

    if (azione == "FORN_UPD_NUMTEL"){
        fornitore.aggiornaNumeroDiTelefono(numeri_di_telefono[i100]);
    }

    if (azione == "FORN_INSERT_PRODOTTO"){
        Product prodotto;
        prodotto.add_new_product(arr_prodotti[i100], categoriaProdotti[i5], rand()%1000, "Descrizione randoma", nomi_aziende_produttrici[i5], rand()%100);
    }

    if (azione == "FORN_REMOVE_PRODOTTO"){
        Product prodotto;
        prodotto.remove_prodotto(rand()%100);
    }

    if (azione == "FORN_UPD_AZIENDA"){
        fornitore.aggiornaNomeAziendaProduttrice(nomi_aziende_produttrici[i5]);
    }



    if (azione == "TRASP_UPD_PASSWORD"){
        trasportatore.aggiornaPassword(trasportatore.password, password[i100]);
    }

    if (azione == "TRASP_LOGOUT"){
        trasportatore.effettua_logout(trasportatore.nome_utente);
    }

    if (azione == "TRASP_LOGIN"){
        trasportatore.effettua_login(nomi_utente[i100], password[i100]);
    }

    if (azione == "TRASP_ELIMINAPROFILO"){
        trasportatore.elimina_profilo();
    }

    if (azione == "TRASP_UPD_NUMTEL"){
        trasportatore.aggiornaNumeroDiTelefono(numeri_di_telefono[i100]);
    }

    if (azione == "TRASP_UPD_DITTA"){
        trasportatore.aggiornaNomeDittaSpedizione(nomi_ditte_spedizioni[i5]);
    }

    if (azione == "TRASP_AVVIA_SPED"){
        Spedizione spedizione;
        spedizione.assegnaOrdineTrasportatore();
    }

    if (azione == "TRASP_AVVISA_SPEDITO"){
        Spedizione spedizione;
        spedizione.spedizioneConsegnata(rand()%100);
    }


    return;
}





#endif