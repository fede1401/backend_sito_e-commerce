// INCLUDE_H
#ifndef INCLUDE_H
#define INCLUDE_H

#include "spedizione.h"
#include "utente_trasportatore.h"

#include "../../shared-server/log2db.h"
#include "../../shared-server/msleep.h"
#include "../../shared-server/isSpecialCharacter.h"
#include "../../shared-server/generateSessionID.h"      // Migliore separazione delle responsabilità
#include "../../shared-server/checkSessionID.h"


std::string registrazione(Con2DB db1,
                               std::string in_nome_utente, std::string in_categoria, std::string in_nome, std::string in_cognome,
                               std::string in_sessionID,
                               std::string in_numero_telefono, std::string in_email,
                               std::string in_password, std::string in_conferma_password,
                               std::string in_dittaSped);

std::string login(Con2DB db1, std::string in_nome_utente, std::string in_password, std::string in_sessionID);

std::string logout(Con2DB db1, std::string in_nome_utente);

 std::string elimina_profilo(Con2DB db1, std::string in_nome_utente);

std::string aggiorna_password(Con2DB db1, std::string in_nome_utente, std::string in_vecchia_password, std::string in_nuova_password);

std::string aggiorna_numero_telefono(Con2DB db1, std::string in_nome_utente, std::string in_nuovo_numero_telefono);

std::string aggiorna_nome_ditta_spedizione(Con2DB db1, std::string input_nome_utente, std::string nuovaDittaSpedizione);

bool check_email(Con2DB db1, std::string in_email, std::string in_nome_requisito, statoRequisito in_stato_requisito, std::string in_sessionID);
bool check_password(Con2DB db1, std::string in_password, std::string in_conferma_password, std::string in_nome_requisito, statoRequisito in_stato_requisito, std::string in_sessionID);
bool check_nome_utente_univoco(Con2DB db1, std::string in_nome_utente, std::string in_nome_requisito, statoRequisito in_stato_requisito, std::string in_sessionID);          
bool check_email_univoca(Con2DB db1, std::string in_email, std::string in_nome_requisito, statoRequisito in_stato_requisito, std::string in_sessionID);

std::string prendi_in_carico_spedizione(Con2DB db1, std::string in_nome_utente_trasportatore);
std::string avvisa_spedizione_consegnata(Con2DB db1, std::string in_nome_utente_trasportatore, int in_id_spedizione);


#endif // INCLUDE_H
