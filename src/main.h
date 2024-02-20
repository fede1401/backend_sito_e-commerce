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
#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db/pgsql.h"


//#include "carrello.h"
//#include "lista_desideri.h"
//#include "carta.h"
//#include "ordine.h"
//#include "reso.h"
//#include "recensione.h"
//#include "user.h"
//#include "user_compratore.h"
//#include "user_fornitore.h"
//#include "user_trasportatore.h"
//#include "product.h"
//#include "spedizione.h"
//#include "log2db.h"
//#include "test_generator.h"


PGresult *res;
char sqlcmd[1000];

int rows, k;

#define HORIZON 20  // TICKS

/*
typedef enum {COMPRAVENDITA, RICERCA, COMPRATORE_UPD_RESIDENZA, COMPR_UPD_NUMTEL, COMPR_UPD_PASSWORD, COMPR_LOGOUT,
                    COMPR_LOGIN, COMPR_ELIMINAPROFILO, COMPR_CONTROLLA_ORDINI_EFFETTUATI,
                    INSERT_CARRELLO, REMOVE_CARRELLO, INSERT_LISTA_DESIDERI, REMOVE_LISTA_DESIDERI, AGGIUNTA_CARTA_PAGAMENTO,
                    EFFETTUA_RECENSIONE, RIMUOVE_RECENSIONE, EFFETTUA_RESO, ANNULLA_ORDINE,
                    FORN_UPD_PASSWORD, FORN_LOGOUT, FORN_LOGIN, FORN_ELIMINAPROFILO, FORN_UPD_NUMTEL, FORN_INSERT_PRODOTTO, FORN_REMOVE_PRODOTTO,
                    TRASP_UPD_PASSWORD, TRASP_LOGOUT, TRASP_LOGIN, TRASP_ELIMINAPROFILO, TRASP_UPD_NUMTEL, TRASP_AVVIA_SPED, TRASP_AVVISA_SPEDITO } server_type;


 std::vector<std::string> server_types = {
        "COMPRAVENDITA", "RICERCA", "COMPRATORE_UPD_RESIDENZA", "COMPR_UPD_NUMTEL", "COMPR_UPD_PASSWORD", "COMPR_LOGOUT", "COMPR_LOGIN", "COMPR_ELIMINAPROFILO", 
        "COMPR_CONTROLLA_ORDINI_EFFETTUATI", "INSERT_CARRELLO", "REMOVE_CARRELLO", "INSERT_LISTA_DESIDERI", "REMOVE_LISTA_DESIDERI", "AGGIUNTA_CARTA_PAGAMENTO",
        "EFFETTUA_RECENSIONE", "RIMUOVE_RECENSIONE", "EFFETTUA_RESO", "ANNULLA_ORDINE",
        "FORN_UPD_PASSWORD", "FORN_LOGOUT", "FORN_LOGIN", "FORN_ELIMINAPROFILO", "FORN_UPD_NUMTEL", "FORN_INSERT_PRODOTTO", "FORN_REMOVE_PRODOTTO", "FORN_UPD_AZIENDA"
        "TRASP_UPD_PASSWORD", "TRASP_LOGOUT", "TRASP_LOGIN", "TRASP_ELIMINAPROFILO", "TRASP_UPD_NUMTEL", "TRASP_UPD_DITTA", "TRASP_AVVIA_SPED", "TRASP_AVVISA_SPEDITO"
    };
*/    

enum class statoRequisito {
    Success, 
    NotSuccess,
    Wait
  };


void InsertToLogDB(std::string statoLog, std::string message, std::string sessionID, std::string nomeRequisito, statoRequisito statoReq);
std::string statoRequisitoToString(statoRequisito statoReq);

//void svolgiAzione(UtenteCompratore compratore, UtenteTrasportatore trasportatore, UtenteFornitore fornitore, int index_azioneDaSvolgere);

int msleep(long msec);
int micro_sleep(long usec);
int long get_nanos(void);

#endif
