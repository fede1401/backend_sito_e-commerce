// INCLUDE_H
#ifndef INCLUDE_H
#define INCLUDE_H


#include "carrello.h"
#include "carta.h"
#include "lista_desideri.h"
#include "ordine.h"
#include "recensione.h"
#include "reso.h"
#include "utente_compratore.h"

#include "../../shared-server/log2db.h"
#include "../../shared-server/msleep.h"
#include "../../shared-server/prodotto.h"
#include "../../shared-server/isSpecialCharacter.h"
#include "../../shared-server/generateSessionID.h"          // Migliore separazione delle responsabilità
#include "../../shared-server/checkSessionID.h"



std::string registrazione(Con2DB db1,
                               std::string in_nome_utente, std::string in_categoria, std::string in_nome, std::string in_cognome,
                               std::string in_sessionID,
                               std::string in_numero_telefono, std::string in_email,
                               std::string in_via_residenza, std::string in_numero_civico, std::string in_CAP, std::string in_città_residenza,
                               std::string in_password, std::string in_conferma_password,
                               std::string in_data_compleanno);

bool check_email(Con2DB db1, std::string in_email, std::string in_nome_requisito, statoRequisito in_stato_requisito, std::string in_sessionID);
bool check_password(Con2DB db1, std::string in_password, std::string in_conferma_password, std::string in_nome_requisito, statoRequisito in_stato_requisito, std::string in_sessionID);
bool check_nome_utente_univoco(Con2DB db1, std::string in_nome_utente, std::string in_nome_requisito, statoRequisito in_stato_requisito, std::string in_sessionID);          
bool check_email_univoca(Con2DB db1, std::string in_email, std::string in_nome_requisito, statoRequisito in_stato_requisito, std::string in_sessionID);
std::string codifica_dataCompleanno(std::string in_data_compleanno);
std::string aggiorna_residenza(Con2DB db1, std::string in_nome_utente, std::string in_nuova_via_residenza, std::string in_nuovo_numero_civico, std::string in_nuovo_CAP, std::string in_nuova_citta_residenza);

std::string login(Con2DB db1, std::string in_nome_utente, std::string in_password, std::string in_sessionID);

std::string logout(Con2DB db1, std::string in_nome_utente);

std::string elimina_profilo(Con2DB db1, std::string in_nome_utente);

std::string aggiorna_password(Con2DB db1, std::string in_nome_utente, std::string in_vecchia_password, std::string in_nuova_password);

std::string aggiorna_numero_telefono(Con2DB db1, std::string in_nome_utente, std::string in_nuovo_numero_telefono);


std::string aggiungi_prodotto_carrello(Con2DB db1,std::string in_nome_utente_compratore, int in_cod_prodotto);
std::string rimuovi_prodotto_carrello(Con2DB db1,std::string in_nome_utente_compratore, int in_cod_prodotto);

std::string aggiungi_prodotto_lista_desideri(Con2DB db1, std::string in_nome_utente_compratore, int in_cod_prodotto);
std::string rimuovi_prodotto_lista_desideri(Con2DB db1, std::string in_nome_utente_compratore,  int in_cod_prodotto);

std::string aggiungi_carta_pagamento(Con2DB db1, std::string in_nome_utente, std::string in_numero_carta, std::string in_cvv);
std::string rimuovi_carta_pagamento(Con2DB db1, std::string in_nome_utente_compratore, int in_id_carta);

std::string effettua_reso(Con2DB db1, std::string in_nome_utente_compratore, int in_id_ordine, motivazioneReso in_motivazione_reso);

std::string effettua_recensione(Con2DB db1, std::string in_nome_utente_compratore, int in_id_ordine, std::string in_descrizione, votoStelle in_voto_stella);
std::string rimuovi_recensione(Con2DB db1, std::string in_nome_utente_compratore, int in_id_recensione);

std::string visione_ordini_effettuati(Con2DB db1, std::string in_nome_utente_compratore);

std::string annulla_ordine(Con2DB db1, std::string in_nome_utente_compratore, int in_id_ordine);



#endif // INCLUDE_H
