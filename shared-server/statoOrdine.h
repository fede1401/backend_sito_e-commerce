#ifndef STATOORDINE_H
#define STATOORDINE_H

#include "../server-customer/src/main.h"


enum class StatoOrdine {
    NonOrdinato,
    InElaborazione,
    Spedito,
    Annullato
    };


    // Metodo che prende un tipo enumerativo StatoOrdine come input e restituisce una stringa che rappresenta quel particolare stato.
    std::string statoOrdineToString(StatoOrdine in_stato)
    {
        switch (in_stato)
        {
        case StatoOrdine::InElaborazione:
            return "in elaborazione";
        case StatoOrdine::Spedito:
            return "spedito";
        case StatoOrdine::Annullato:
            return "annullato";
        default:
            return ""; // gestione degli errori o valori non validi
        }
    }


    // Metodo che data una stringa rappresentante lo stato dell'ordine torna il tipo enumerativo corrispondente.
    StatoOrdine stringToStatoOrdine(std::string in_stato)
    {
        if (in_stato == "in elaborazione" ){
            return StatoOrdine::InElaborazione;
        }

        if (in_stato == "spedito" ){
            return StatoOrdine::Spedito;
        }

        if (in_stato == "annullato" ){
            return StatoOrdine::Annullato;
        }
        
        return StatoOrdine::NonOrdinato;
    }




#endif // STATOORDINE_H
