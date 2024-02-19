
#ifndef LOG2DB_H
#define LOG2DB_H

#include "main.h"

enum class statoRequisito {
    Success, 
    NotSuccess,
    Wait
  };



void InsertToLogDB(std::string statoLog, std::string message, std::string sessionID, std::string nomeRequisito, statoRequisito statoReq){

    Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    std::chrono::system_clock::time_point tp = std::chrono::system_clock::time_point(std::chrono::milliseconds(now_ms));

    std::time_t now_c = std::chrono::system_clock::to_time_t(tp);

    std::stringstream ss;
    ss << std::put_time(std::gmtime(&now_c), "%Y-%m-%d %H:%M:%S");
    std::string timevalue = ss.str();

    pid_t pid = getpid();

    std::string statoReqToString = statoRequisitoToString(statoReq);

    //std::string statoLog = "INFO";
    //std::string message = "Utente compratore è stato loggato!";

    sprintf(sqlcmd, "INSERT INTO LogTable (timevalue, pid, statoLog , messaggio, sessionId, nomeRequisiti, statoRequisito) VALUES ('%s', %ld, '%s', '%s', '%s', '%s', '%s')", 
    timevalue.c_str(), (long)pid, statoLog.c_str(), message.c_str(), sessionID.c_str(), nomeRequisito.c_str(), statoReqToString.c_str());
    
    res = db1.ExecSQLcmd(sqlcmd);
    PQclear(res);  
}


std::string statoRequisitoToString(statoRequisito statoReq) {
        switch (statoReq) {
            case statoRequisito::Success :
                return "SUCCESS";
            case statoRequisito::NotSuccess :
                return "NOT SUCCESS";
            case statoRequisito::Wait :
                return "WAIT";
            default:
                return ""; // gestione degli errori o valori non validi
        }
    }




#endif // LOG2DB_H
