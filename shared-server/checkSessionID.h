#ifndef CHECKSESSIONID_H
#define CHECKSESSIONID_H

#include "../server-customer/src/main.h"

    
    // Metodo per verificare se un sessionID è univoco tra gli utenti compratori, fornitori e trasportatori
    bool check_sessionID(Con2DB db1, std::string in_nome_requisito, statoRequisito in_stato_requisito, std::string in_sessionID)
    {
        bool result = true;

        // Verifico se il sessionID è già presente nella tabella UtenteCompratore
        sprintf(sqlcmd, "SELECT * FROM Utente WHERE session_id = '%s'", in_sessionID.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        // Se il numero di righe dal risultato della query è maggiore di 0, il sessionID già esiste nel database, ed è stato assegnato ad un altro utente compratore.
        if (rows > 0)
        {
            // Log dell'errore e uscita dalla funzione
            in_stato_requisito = statoRequisito::NotSuccess;
            InsertToLogDB(db1, "ERROR", "Il session ID è già in uso da utenti.", in_sessionID, in_nome_requisito, in_stato_requisito);

            std::cout << "Errore: Il session ID è già in uso da utenti." << std::endl;
            return false;
        }

        // Restituiamo true se il sessionID è univoco tra tutti gli utenti
        return result;
    }


#endif // CHECKSESSIONID_H