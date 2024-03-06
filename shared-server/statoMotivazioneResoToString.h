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
    std::string statoMotivazioneResoToString(motivazioneReso stato) {
        switch (stato) {
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



#endif // STATOMOTIVAZIONERESOTOSTRING_H
