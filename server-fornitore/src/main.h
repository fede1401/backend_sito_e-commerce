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
#include <fstream>
#include <filesystem>
#include <string.h>


#include "../../con2db/pgsql.h"
#include "../../con2redis/src/con2redis.h"

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


void InsertToLogDB(Con2DB db1,std::string statoLog, std::string message, std::string sessionID, std::string nomeRequisito, statoRequisito statoReq);
std::string statoRequisitoToString(statoRequisito statoReq);

//void svolgiAzione(UtenteCompratore compratore, UtenteTrasportatore trasportatore, UtenteFornitore fornitore, int index_azioneDaSvolgere);

int msleep(long msec);
int micro_sleep(long usec);
int long get_nanos(void);

#endif
