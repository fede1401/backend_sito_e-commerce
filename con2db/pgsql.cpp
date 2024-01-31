
#include "pgsql.h"

#define DEBUG 1000  // Definisci la costante DEBUG per abilitare o disabilitare la stampa di debug


Con2DB::Con2DB(const char *hostname, const char *port, const char *username, const char *password, const char *dbname) {

  // Buffer per la stringa di connessione
  char buf[1000]; 

  // Formatta la stringa di connessione con i parametri forniti
  sprintf(buf, "host=%s port=%s user=%s password=%s dbname=%s", hostname, port, username, password, dbname );
  
  // connect linux user to database (must have privileges)
  conn = PQconnectdb(buf);

  //  PGconn *conn = PQconnectdb("user=enrico dbname=sdfs");
  //  PGconn *conn = PQconnectdb("user=sdfs dbname=sdfs");    // on MPM server

  // Verifica se la connessione è avvenuta correttamente
  if (PQstatus(conn) != CONNECTION_OK)  {    
        fprintf(stderr, "Con2DB(%s): Connection to database failed: %s\n", dbname, PQerrorMessage(conn));
        finish();
  }

#if 0
    fprintf(stderr, "Con2DB(%s): PQconnectPoll: %d\n", dbname, PQconnectPoll(conn));        
    fprintf(stderr, "Con2DB(%s): PQstatus: %d\n", dbname, PQstatus(conn));       
    fprintf(stderr, "Con2DB(%s): Connection to DB successfully established \n", dbname);        
#endif
}  /* Con2DB() */


// Distruttore della classe Con2DB
void Con2DB::finish() {
    
    PQfinish(conn);
    conn = NULL;
    exit(1);
}



/* use this for commands returning no data, e.g. INSERT */
PGresult* Con2DB::ExecSQLcmd(char *sqlcmd) {

  // Verifica se è presente una connessione al database
  if (conn == NULL) {
    fprintf(stderr, "ExecSQLcmd(): no connection to DB: PQexec on: %s\n", sqlcmd);
    exit (1);
  }
  
#if (DEBUG > 1000000)
  fprintf(stderr, "ExecSQLcmd(): PQexec on: %s\n", sqlcmd);        
  fprintf(stderr, "ExecSQLcmd(): PQconnectPoll: %d\n", PQconnectPoll(conn));        
  fprintf(stderr, "ExecSQLcmd(): PQstatus: %d\n", PQstatus(conn));        
#endif

  // Esegui il comando SQL
  PGresult *res = PQexec(conn, sqlcmd);    

#if (DEBUG > 1000000)
    fprintf(stderr, "ExecSQLcmd(): PQexec done\n");        
#endif
    
    // Verifica se il comando SQL è stato eseguito correttamente
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
      // Stampa un messaggio di errore e termina il programma
      fprintf(stderr, "ExecSQLcmd(): SQL command failed: %s\n", sqlcmd);
      fprintf(stderr, "ExecSQLcmd(): %s\n",
	    PQresStatus(PQresultStatus(res)));

      fprintf(stderr, "ExecSQLcmd(): PQresultErrorMessage: %s\n",
	    PQresultErrorMessage(res) ) ;
      
      // Libera la memoria occupata dal risultato
      PQclear(res);

      // Chiudi la connessione e termina il programma
      finish();
    }    

#if 0
    fprintf(stderr, "ExecSQLcmd() SQL command OK: %s\n", sqlcmd);        
#endif

    return (res);
}


/* use this for commands returning data, e.g. SELECT */
PGresult* Con2DB::ExecSQLtuples(char *sqlcmd)
{

  if (conn == NULL)
    // error
    {
     fprintf(stderr, "ExecSQLtuples(): no connection to DB: sqlcmd = %s\n", sqlcmd);
     exit (1);
    }

  // conn != NULL
  
  // Esegui il comando SQL
  PGresult *res = PQexec(conn, sqlcmd);    
	
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {

      fprintf(stderr, "ExecSQLtuples(): SQL command failed: %s\n", sqlcmd);
      fprintf(stderr, "ExecSQLtuples(): %s\n",
	    PQresStatus(PQresultStatus(res)));

      PQclear(res);
      finish();
    }    

#if 0
    fprintf(stderr, "ExecSQLtuples() OK: %s\n", sqlcmd);        
#endif

    return (res);
}
