#ifndef STATOVOTOSTELLETOSTRING_H
#define STATOVOTOSTELLETOSTRING_H

#include "../server-customer/src/main.h"
//#include "../server-customer/src/recensione.h"


enum class votoStelle
{
    Uno,
    Due,
    Tre,
    Quattro,
    Cinque
};


// Metodo che prende un tipo enumerativo votoStelle come input e restituisce una stringa che rappresenta quel particolare stato.
    std::string statoVotoStelleToString(votoStelle stato)
    {
        switch (stato)
        {
        case votoStelle::Uno:
            return "1";
        case votoStelle::Due:
            return "2";
        case votoStelle::Tre:
            return "3";
        case votoStelle::Quattro:
            return "4";
        case votoStelle::Cinque:
            return "5";
        default:
            return ""; // gestione degli errori o valori non validi
        }
    }


#endif // STATOVOTOSTELLETOSTRING_H
