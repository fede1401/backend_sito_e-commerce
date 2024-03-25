#ifndef STATOMOTIVAZIONERESOTOSTRING_H
#define STATOMOTIVAZIONERESOTOSTRING_H

#include "../server-customer/src/main.h"


enum class motivazioneReso {
    Difettoso,
    MisuraErrata,
    NonConformeAlleAspettative,
    CambioOpinione
    };


// Metodo che prende un tipo enumerativo motivazioneReso come input e restituisce una stringa che rappresenta quel particolare stato.
    std::string statoMotivazioneResoToString(motivazioneReso in_stato) {
        switch (in_stato) {
            case motivazioneReso::Difettoso :
                return "difettoso";
            case motivazioneReso::MisuraErrata :
                return "misura errata";
            case motivazioneReso::NonConformeAlleAspettative :
                return "non conforme alle aspettative";
            case motivazioneReso::CambioOpinione :
                return "cambio opinione";
            default:
                return ""; // gestione degli errori o valori non validi
        }
    }


    // Metodo che prende una stringa che rappresenta quello stato e lo trasforma nel tipo enumerativo.
    motivazioneReso stringToStatoMotivazioneReso(std::string in_stato)
    {    
        if (in_stato == "difettoso"){
            return motivazioneReso::Difettoso;
        }

        if (in_stato == "misura errata"){
            return   motivazioneReso::MisuraErrata;
        }

        if (in_stato == "non conforme alle aspettative"){
            return motivazioneReso::NonConformeAlleAspettative;
        }

        if (in_stato == "cambio opinione"){
            return  motivazioneReso::CambioOpinione;
        }
    }



#endif // STATOMOTIVAZIONERESOTOSTRING_H
