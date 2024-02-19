#ifndef LOG2DB_H
#define LOG2DB_H


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


void svolgiAzione(UtenteCompratore compratore, UtenteTrasportatore trasportatore, UtenteFornitore fornitore, int index_azioneDaSvolgere){

    std::string azione = server_types[index_azioneDaSvolgere];

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



    int i5 = rand()%5;
    int i100 = rand()%100;


    if (azione == "COMPRAVENDITA"){
         
        Product prodotto;
        Ordine ordine = prodotto.acquistaProdotto(compratore.nome_utente, arr_vie_spedizione[i5], arr_citta_spedizione[i5], numeri_civici[i100]);
    }


    if (azione == "RICERCA"){
        int i100 = rand()%100;
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
        compratore.effettua_login(compratore.nome_utente, compratore.password);
    }

    if (azione == "COMPR_ELIMINAPROFILO"){
        compratore.elimina_profilo();
    }

    if (azione == "COMPR_CONTROLLA_ORDINI_EFFETTUATI"){
        Ordine ordine;
        ordine.visione_ordini_effettuati(compratore.nome_utente);
    }

    if (azione == "INSERT_CARRELLO"){
        Carrello carrello;
        carrello.add_prodotto(compratore.nome_utente, 2); // da sistemare, inserire come secondo parametro il nome del prodotto.
    }









    



    return;
}






#endif