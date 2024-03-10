#ifndef ISSPECIALCHARACTER_H
#define ISSPECIALCHARACTER_H

#include "../server-customer/src/main.h"


bool isSpecialCharacter(char c)
{
    // Puoi aggiungere altri caratteri speciali se necessario
    return !std::isalnum(c);
}

#endif // ISSPECIALCHARACTER_H