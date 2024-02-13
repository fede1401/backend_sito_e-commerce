
#ifndef LOG2DB_H
#define LOG2DB_H


#include <iostream>
#include <string>
#include <cctype> // Per isupper() e isdigit()
#include <sstream>
#include <random>
#include <unistd.h>
#include <chrono>
#include <iomanip>
#include "carrello.h"

#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2db/pgsql.h"


void InsertToLogDB(std::string statoLog, std::string message, std::string sessionID){

    Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    std::chrono::system_clock::time_point tp = std::chrono::system_clock::time_point(std::chrono::milliseconds(now_ms));

    std::time_t now_c = std::chrono::system_clock::to_time_t(tp);

    std::stringstream ss;
    ss << std::put_time(std::gmtime(&now_c), "%Y-%m-%d %H:%M:%S");
    std::string timevalue = ss.str();

    pid_t pid = getpid();

    //std::string statoLog = "INFO";
    //std::string message = "Utente compratore è stato loggato!";

    sprintf(sqlcmd, "INSERT INTO LogTable (timevalue, pid, statoLog , messaggio, sessionId) VALUES ('%s', %ld, '%s', '%s', '%s')", timevalue.c_str(), (long)pid, statoLog.c_str(), message.c_str(), sessionID.c_str());
    res = db1.ExecSQLcmd(sqlcmd);
    PQclear(res);  
}



#endif // LOG2DB_H
