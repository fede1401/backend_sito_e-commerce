// Spedizione.h
#ifndef SPEDIZIONE_H
#define SPEDIZIONE_H


#include <string>


enum class StatoSpedizione {
    InElaborazione,
    Spedito
    };


class Spedizione{
public:
    int idSpedizione;
    int idOrdine;
    std::string nome_utente_trasportatore;
    StatoSpedizione stato_spedizione;
    std::string ditta_spedizione;
    std::string via_spedizione;
    std::string città_spedizione;
    std::string numero_civico_spedizione;

    // Definizione costruttori:
    Spedizione():
        idSpedizione(-1),
        idOrdine(-1),
        nome_utente_trasportatore(""),
        stato_spedizione(),
        ditta_spedizione(""),
        via_spedizione(""),
        città_spedizione(""),
        numero_civico_spedizione("")
        {}


    Spedizione(int idSpedizione, int idOrdine, 
            std::string nome_utente_trasportatore, 
            StatoSpedizione stato_spedizione, 
            std::string ditta_spedizione,
            std::string via_spedizione, std::string città_spedizione, std::string numero_civico_spedizione):

            idSpedizione(idSpedizione),
            idOrdine(idOrdine),
            nome_utente_trasportatore(nome_utente_trasportatore),
            stato_spedizione(stato_spedizione),
            ditta_spedizione(ditta_spedizione),
            via_spedizione(via_spedizione),
            città_spedizione(città_spedizione),
            numero_civico_spedizione(numero_civico_spedizione){}



    void impostaStatoSpedizione(StatoSpedizione nuovstato){
        stato_spedizione = nuovstato;
    }

};




#endif // SPEDIZIONE_H