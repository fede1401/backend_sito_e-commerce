# backend_sito_e-commerce
Backend sito e-commerce in c++ with postgreSQL and Redis

Connessione al database tramite terminale con il comando:
```psql -U sito_ecommerce -h localhost -d backend_sito_ecommerce```

Per controllare le righe delle tabelle dopo aver effettuato la connessione , nel terminale utilizzare il comando:
```SELECT * FROM nometabella```

Per eseguire il programma, dal terminale posizionarsi nella cartella src, ed eseguire il comando : ```make```.
Una volta eseguito, se non ci sono errori, spostiamoci nella cartella bin ed eseguiamo il comando: ```./main```.


## Informazioni su c++:
Per eseguire la stampa con variabili :
``` 
int stato_utente = 1;
std::cout << "Lo stato dell'utente è: " << stato_utente << std::endl;   
```



## Interazione con database:
Innanzitutto connettersi al database:
```Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce");```

Per effettuare una query di campo intero della tabella:
```
int stato_utente; 
sprintf(sqlcmd, "SELECT stato FROM UtenteCompratore WHERE nome_utente = '%s'", input_nome_utente.c_str());
res = db1.ExecSQLtuples(sqlcmd);
rows = PQntuples(res);
if (rows == 1) { stato_utente = atoi(PQgetvalue(res, 0, PQfnumber(res, "stato"))); }
PQclear(res); 
```

Per effettuare una query di campo stringa della tabella:
`
std::string password_utente;
```
char *password_u;
sprintf(sqlcmd, "SELECT password FROM UtenteCompratore WHERE nome_utente = '%s'", input_nome_utente.c_str());   // SELECT password FROM UtenteCompratore WHERE nome_utente = 'fede14';`
PGresult *res = db1.ExecSQLtuples(sqlcmd);
rows = PQntuples(res);
if (rows == 1) {
    `password_u = PQgetvalue(res, 0, PQfnumber(res, "password"));
    `password_utente.assign(password_u);
    }
PQclear(res); 
```

PQgetvalues prende in input il risultato della query, l'indice di riga della query e il campo ricercato.



Per effettuare un UPDATE:
```
sprintf(sqlcmd, "UPDATE UtenteCompratore set stato = 0 WHERE nome_utente = '%s'", input_nome_utente.c_str());
res = db1.ExecSQLcmd(sqlcmd);
PQclear(res); 
```

Dopo di che controllare che il tutto sia avvenuto correttamente con una SELECT.


Per effettuare una INSERT:
```
sprintf(sqlcmd, "INSERT INTO UtenteCompratore (idUtComp, nome_utente, nome, cognome, indirizzo_mail, numero_di_telefono, password, data_compleanno, via_di_residenza, numero_civico, CAP, citta_di_residenza, saldo, stato ) VALUES (DEFAULT, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%d', '%s', '%s', %f, %d)", nome_utente.c_str(), nome.c_str(), cognome.c_str(), email.c_str(), numero_telefono.c_str(), password.c_str(), formatted_date.c_str(), via_residenza.c_str(), numero_civico, CAP.c_str(), città_residenza.c_str(), saldo, stato);
res = db1.ExecSQLcmd(sqlcmd);
PQclear(res); 
```

        
Per effettuare una qualsiasi operazione postgreSQL con l'utilizzo di variabili all'interno utilizzare il campo %s, ad esempio:
``` sprintf(sqlcmd, "SELECT stato FROM %s WHERE nome_utente = '%s'", categoria.c_str(), input_nome_utente.c_str()); ```


Per stampare su terminale i risultati delle query che prendono tutte le colonne utilizzare questo codice di esempio:
```
sprintf(sqlcmd, "SELECT * FROM UtenteCompratore");
    res = db1.ExecSQLtuples(sqlcmd);
    rows = PQntuples(res);
    int numCols = PQnfields(res);

    std::cout << "Rows: " << rows << std::endl;
    for (int i = 0; i < rows; ++i) {
        std::cout << "Row " << i << ": ";
        for (int j = 0; j < numCols; ++j) {
            std::cout << PQgetvalue(res, i, j) << "\t";
        }
        std::cout << std::endl;
    }
    PQclear(res); 
```



Per eliminare in PostgreSQL :
1. Eliminare una riga in base alla chiave primaria o in base a una condizione:
```
DELETE FROM table_name WHERE id = '1';

DELETE FROM table_name WHERE nome_colonna = 'valore';
```

2. Eliminare tutte le righe di una tabella:
``` TRUNCATE table_name CASCADE;```

3. Elimnare una tabella:
``` DROP TABLE table_name CASCADE; ```

4. Eliminare un tipo enumerativo:
``` DROP TYPE nome_del_tipo_enum; ```




Per selezionare i tipi enumerativi in postgreSQL:
``` SELECT enumlabel FROM pg_enum WHERE enumtypid = 'nome_del_tipo_enum'::regtype;  ```


Per assegnare i privilegi per un tipo o tabella nel database postgreSQL:
```  
ALTER TABLE Spedizione OWNER TO :username ;
ALTER TYPE statoOrdine OWNER TO :username;
```


Per eseguire il main dobbiamo prima avviare il server redis in un terminale con il comando:
``` redis-server ```  