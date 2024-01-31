// Carta.h
#ifndef CARTA_H
#define CARTA_H

#include <string>

class Carta {
public:
    std::string identificatore_utente;
    std::string numero_carta;
    std::string cvv;

    // Costruttore
    Carta(std::string id_utente, std::string numero, std::string codice)
        : identificatore_utente(id_utente), numero_carta(numero), cvv(codice) {}
};

#endif // CARTA_H
