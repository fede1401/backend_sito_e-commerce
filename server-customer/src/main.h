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
#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db/pgsql.h"
#include <string.h>

PGresult *res;
char sqlcmd[2048];

int rows, k;

#define HORIZON 20  // TICKS


enum class statoRequisito {
    Success, 
    NotSuccess,
    Wait
  };


void InsertToLogDB(Con2DB db1,std::string statoLog, std::string message, std::string sessionID, std::string nomeRequisito, statoRequisito statoReq);
std::string statoRequisitoToString(statoRequisito statoReq);


int msleep(long msec);
int micro_sleep(long usec);
int long get_nanos(void);

#endif
