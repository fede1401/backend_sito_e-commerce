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

        
