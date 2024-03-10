#ifndef FINDCATEGORIAUTENTE_H
#define FINDCATEGORIAUTENTE_H

#include "../server-customer/src/main.h"


// Metodo utilizzato per trovare la categoria di un utente dato il suo nome_utente.
    std::string find_categoria_utente(Con2DB db1, std::string input_nome_utente)
    {

        // Variabile per memorizzare la categoria dell'utente
        std::string categoriaUtente = "";

        // Ricerca dell'utente nella tabella UtenteCompratore
        sprintf(sqlcmd, "SELECT * FROM UtenteCompratore WHERE nome_utente_compratore = '%s'", input_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe del risultato della query è uguale a 1, l'utente è stato trovato e memorizziamo il risultato
        if (rows == 1)
        {
            categoriaUtente = "UtenteCompratore";
        }

        // Ricerca dell'utente nella tabella UtenteFornitore
        sprintf(sqlcmd, "SELECT * FROM UtenteFornitore WHERE nome_utente_fornitore = '%s'", input_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe del risultato della query è uguale a 1, l'utente è stato trovato e memorizziamo il risultato
        if (rows == 1)
        {
            categoriaUtente = "UtenteFornitore";
        }

        // Ricerca dell'utente nella tabella UtenteTrasportatore
        sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore WHERE nome_utente_trasportatore = '%s'", input_nome_utente.c_str());
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        // Se il numero di righe del risultato della query è uguale a 1, l'utente è stato trovato e memorizziamo il risultato
        if (rows == 1)
        {
            categoriaUtente = "UtenteTrasportatore";
        }

        // Viene restituita la categoria dell'utente trovata.
        return categoriaUtente;
    }


#endif // FINDCATEGORIAUTENTE_H