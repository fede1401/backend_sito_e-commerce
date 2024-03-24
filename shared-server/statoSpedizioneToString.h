#ifndef STATOSPEDIZIONETOSTRING_H
#define STATOSPEDIZIONETOSTRING_H

#include "../server-customer/src/main.h"
//#include "../server-trasportatore/src/spedizione.h"


enum class StatoSpedizione
{
    InTransito,
    Consegnato
};

    // Metodo che prende uno stato di spedizione come input e restituisce una stringa che rappresenta quel particolare stato.
    std::string statoSpedizioneToString(StatoSpedizione in_stato)
    {
        switch (in_stato)
        {
        case StatoSpedizione::InTransito:
            return "in transito";
        case StatoSpedizione::Consegnato:
            return "consegnato";
        default:
            return ""; // gestione degli errori o valori non validi
        }
    }


#endif // STATOSPEDIZIONETOSTRING_H
