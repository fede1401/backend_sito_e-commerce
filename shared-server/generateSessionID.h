
#ifndef GENERATESESSIONID_H
#define GENERATESESSIONID_H

#include "../server-customer/src/main.h"


std::string generateSessionID()
    {
        // Caratteri validi per il Session ID
        const std::string valid_characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

        // Inizializzazione del generatore di numeri casuali
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, valid_characters.size() - 1);

        // Generazione del Session ID casuale
        std::string session_id;
        for (int i = 0; i < 10; ++i)
        {
            session_id += valid_characters[dis(gen)];
        }

        return session_id;
    }


#endif // GENERATESESSIONID_H
