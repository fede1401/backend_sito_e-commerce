
#ifndef GETCURRENTDATEASSTRING_H
#define GETCURRENTDATEASSTRING_H

#include "../server-customer/src/main.h"



// Funzione per ottenere la data corrente come stringa nel formato "GG-MM-AAAA"
    std::string getCurrentDateAsString()
    {
        // Ottieni il tempo corrente in secondi dal 1 gennaio 1970
        std::time_t now = std::time(nullptr);

        // Converti il tempo corrente in una struct tm
        std::tm *timeinfo = std::localtime(&now);

        // Costruisci una stringa dalla data nel formato GG-MM-AAAA
        std::stringstream ss;
        ss << timeinfo->tm_mday << '-'          // Aggiungi il giorno (GG) con un trattino come separatore
           << (timeinfo->tm_mon + 1) << '-'     // Aggiungi il mese (MM) con un trattino come separatore
           << (timeinfo->tm_year + 1900);        // Aggiungi l'anno (AAAA) con un trattino come separatore

        // timeinfo->tm_year restituisce l'anno - 1900, quindi aggiungiamo 1900 per ottenere l'anno corretto

    // Restituisci la stringa ottenuta dalla stringstream
    return ss.str();
    }


#endif // GETCURRENTDATEASSTRING_H
