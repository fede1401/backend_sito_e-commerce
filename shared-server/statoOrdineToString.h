#ifndef STATOORDINETOSTRING_H
#define STATOORDINETOSTRING_H

#include "../server-customer/src/main.h"
//#include "../server-customer/src/ordine.h"


enum class StatoOrdine {
    InElaborazione,
    Spedito,
    Annullato
    };


// Metodo che prende un tipo enumerativo StatoOrdine come input e restituisce una stringa che rappresenta quel particolare stato.
    std::string statoOrdineToString(StatoOrdine stato)
    {
        switch (stato)
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



#endif // STATOORDINETOSTRING_H
