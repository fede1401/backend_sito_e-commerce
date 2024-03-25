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
    std::string statoVotoStelleToString(votoStelle in_stato)
    {
        switch (in_stato)
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


    // Metodo che data una stringa rappresentante il voto delle stelle ritorni il tipo enumerativo votoStelle corrispondente.
    votoStelle stringToVotoStelle(std::string in_stato)
    {
        if (in_stato == "1"){
            return votoStelle::Uno;
        }

        if (in_stato == "2"){
            return votoStelle::Due;
        }

        if (in_stato == "3"){
            return votoStelle::Tre;
        }

        if (in_stato == "4"){
            return votoStelle::Quattro;
        }

        if (in_stato == "5"){
            return votoStelle::Cinque;
        }
    }




#endif // STATOVOTOSTELLETOSTRING_H
