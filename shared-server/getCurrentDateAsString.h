
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


    std::string getCurrentDateMinSetString(){
        // Ottiene l'orario attuale in formato timestamp:
        auto now = std::chrono::system_clock::now();        // cattura del momento attuale.
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();        // si ottiene il numero di millisecondi trascorsi dall'epoca (time_since_epoch()) e convertendoli in un valore a 64 bit (count()).
        std::chrono::system_clock::time_point tp = std::chrono::system_clock::time_point(std::chrono::milliseconds(now_ms));    // costruzione di un oggetto time_point utilizzando i millisecondi calcolati
        std::time_t now_c = std::chrono::system_clock::to_time_t(tp);           // converte l'orario attuale tp in un formato di tipo std::time_t.


        // Converte l'orario attuale in formato stringa
        std::stringstream ss;
        ss << std::put_time(std::gmtime(&now_c), "%Y-%m-%d %H:%M:%S");
        std::string timevalue = ss.str();

        return timevalue;
    }


#endif // GETCURRENTDATEASSTRING_H
