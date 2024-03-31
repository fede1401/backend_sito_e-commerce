#ifndef main_h
#define main_h

#include <iostream>
#include <string>
#include <cctype> // Per isupper() e isdigit()
#include <sstream>
#include <random>
#include <unistd.h>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <array>
#include <unordered_map>
#include <string.h>
#include <fstream>
#include <filesystem>


#include "../../con2db/pgsql.h"
#include "../../con2redis/src/con2redis.h"


#include "../../shared-server/votoStelle.h"
#include "../../shared-server/motivazioneReso.h"
#include "../../shared-server/statoOrdine.h"


PGresult *res;
char sqlcmd[2048];

int rows, k;

#define HORIZON 20  // TICKS


enum class statoRequisito {
    Success, 
    NotSuccess,
    Wait
  };


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


void InsertToLogDB(Con2DB db1,std::string statoLog, std::string message, std::string sessionID, std::string nomeRequisito, statoRequisito statoReq);


std::string statoRequisitoToString(statoRequisito statoReq);


int msleep(long msec);
int micro_sleep(long usec);
int long get_nanos(void);

#endif
