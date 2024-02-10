// Spedizione.h
#ifndef SPEDIZIONE_H
#define SPEDIZIONE_H


#include <string>
#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db/pgsql.h"
#include <iostream>



enum class StatoSpedizione {
    InTransito,
    Consegnato
    };


class Spedizione{
public:
    int idSpedizione;
    int idOrdine;
    std::string nome_utente_trasportatore;
    StatoSpedizione stato_spedizione;
    std::string ditta_spedizione;

    // Definizione costruttori:
    Spedizione():
        idSpedizione(-1),
        idOrdine(-1),
        nome_utente_trasportatore(""),
        stato_spedizione(),
        ditta_spedizione("")
        {}


    Spedizione(int idSpedizione, int idOrdine, 
            std::string nome_utente_trasportatore, 
            StatoSpedizione stato_spedizione, 
            std::string ditta_spedizione):

            idSpedizione(idSpedizione),
            idOrdine(idOrdine),
            nome_utente_trasportatore(nome_utente_trasportatore),
            stato_spedizione(stato_spedizione),
            ditta_spedizione(ditta_spedizione){}



    void impostaStatoSpedizione(StatoSpedizione nuovstato){
        stato_spedizione = nuovstato;
    }


};




#endif // SPEDIZIONE_H