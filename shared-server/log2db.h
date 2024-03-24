
#ifndef LOG2DB_H
#define LOG2DB_H

#include "../server-customer/src/main.h"


    // Metodo per inserire un record di log nel database
    void InsertToLogDB(Con2DB db1, std::string in_statoLog, std::string in_message, std::string in_sessionID, std::string in_nomeRequisito, statoRequisito in_statoReq){

        // Ottiene l'orario attuale in formato timestamp:
        auto now = std::chrono::system_clock::now();        // cattura del momento attuale.
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();        // si ottiene il numero di millisecondi trascorsi dall'epoca (time_since_epoch()) e convertendoli in un valore a 64 bit (count()).
        std::chrono::system_clock::time_point tp = std::chrono::system_clock::time_point(std::chrono::milliseconds(now_ms));    // costruzione di un oggetto time_point utilizzando i millisecondi calcolati
        std::time_t now_c = std::chrono::system_clock::to_time_t(tp);           // converte l'orario attuale tp in un formato di tipo std::time_t.


        // Converte l'orario attuale in formato stringa
        std::stringstream ss;
        ss << std::put_time(std::gmtime(&now_c), "%Y-%m-%d %H:%M:%S");
        std::string timevalue = ss.str();


        // Ottiene l'ID del processo corrente
        pid_t pid = getpid();


        // Converte lo stato del requisito in formato stringa
        std::string statoReqToString;
        statoReqToString = statoRequisitoToString(in_statoReq);


        // Inserimento di un record log nel database nella tabella LogTable
        sprintf(sqlcmd, "INSERT INTO LogTable (timevalue, pid, statoLog , messaggio, sessionId, nomeRequisiti, statoRequisito) VALUES ('%s', %ld, '%s', '%s', '%s', '%s', '%s')", 
        timevalue.c_str(), (long)pid, in_statoLog.c_str(), in_message.c_str(), in_sessionID.c_str(), in_nomeRequisito.c_str(), statoReqToString.c_str());
        
        // Esegue il comando SQL per l'inserimento del record di log nel database
        res = db1.ExecSQLcmd(sqlcmd);

        // Viene liberata la memoria utilizzata per memorizzare il risultato della query SQL
        PQclear(res);  

    return;
    }


    // Metodo che prende uno stato di un requisito come input restituisce una stringa che rappresenta quel particolare stato.
    std::string statoRequisitoToString(statoRequisito in_statoReq) {
        switch (in_statoReq) {
            case statoRequisito::Success:
                return "SUCCESS";
            case statoRequisito::NotSuccess:
                return "NOT SUCCESS";
            case statoRequisito::Wait:
                return "WAIT";
            default:
                return ""; // gestione degli errori o valori non validi
        }
    }




#endif // LOG2DB_H
