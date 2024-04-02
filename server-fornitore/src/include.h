// INCLUDE_H
#ifndef INCLUDE_H
#define INCLUDE_H


#include "utente_fornitore.h"

#include "../../shared-server/log2db.h"
#include "../../shared-server/msleep.h"
#include "../../shared-server/prodotto.h"
#include "../../shared-server/isSpecialCharacter.h"
#include "../../shared-server/generateSessionID.h"          // Migliore separazione delle responsabilità
#include "../../shared-server/checkSessionID.h"



std::string registrazione(Con2DB db1,
                               std::string in_nome_utente,
                               std::string in_categoria,
                               std::string in_nome, std::string in_cognome,
                               std::string in_sessionID,
                               std::string in_numero_telefono,
                               std::string in_email,
                               std::string in_password, std::string in_conferma_password,
                               std::string in_aziendaProd);
bool check_email(Con2DB db1, std::string in_email, std::string in_nome_requisito, statoRequisito in_stato_requisito, std::string in_sessionID);
bool check_password(Con2DB db1, std::string in_password, std::string in_conferma_password, std::string in_nome_requisito, statoRequisito in_stato_requisito, std::string in_sessionID);
bool check_nome_utente_univoco(Con2DB db1, std::string in_nome_utente, std::string in_nome_requisito, statoRequisito in_stato_requisito, std::string in_sessionID);          
bool check_email_univoca(Con2DB db1, std::string in_email, std::string in_nome_requisito, statoRequisito in_stato_requisito, std::string in_sessionID);

std::string aggiorna_nome_azienda_produttrice(Con2DB db1, std::string in_nome_utente, std::string in_nuova_azienda_produttrice);

std::string login(Con2DB db1, std::string in_nome_utente, std::string in_password, std::string in_sessionID);

std::string logout(Con2DB db1, std::string in_nome_utente);

std::string elimina_profilo(Con2DB db1, std::string in_nome_utente);

std::string aggiorna_numero_telefono(Con2DB db1, std::string in_nome_utente, std::string in_nuovo_numero_telefono);

std::string aggiorna_password(Con2DB db1, std::string in_nome_utente, std::string in_vecchia_password, std::string in_nuova_password);




#endif // INCLUDE_H
