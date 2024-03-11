
#include "include.h"


#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2redis/src/con2redis.h"
#include <string.h>

#include "../../shared-server/generateSessionID.h"      // Migliore separazione delle responsabilità


// cc -Wall -g -ggdb -o streams streams.c -lhiredis
// Usage: ./streams <add count> <read count> [block time, default: 1]

#define DEBUG 1000                                  // Definizione di una costante per il debugging

#define READ_STREAM_TRASPORTATORE "stream5"         // Nome dello stream da cui leggere.
#define WRITE_STREAM_TRASPORTATORE "stream6"        // Nome dello stream su cui scrivere.

using namespace std;        // Consente di utilizzare le funzioni e le classi standard del C++ senza doverle qualificare con std::.


int main()
{

    redisContext *c2r;
    redisReply *reply;              // Inizializzazione risposta Redis
    redisReply *reply2;             // Inizializzazione risposta2 Redis
    int read_counter = 0;           // Contatore delle letture effettuate
    int send_counter = 0;           // Contatore degli invii effettuati
    int block = 1000000000;         // Tempo di blocco per la lettura da stream in nanosecondi
    int pid;                        // ID del processo
    //unsigned seed;
    char username[100];             
    char key[100];                  // Buffer per la chiave da utilizzare in Redis
    char value[100];                // Buffer per il valore da utilizzare in Redis
    char streamname[100];           // Buffer per il nome dello stream Redis
    char msgid[100];                // Buffer per l'ID del messaggio Redis
    char fval[100];                 // Buffer per il valore del campo del messaggio Redis
    int i, k, h;                    // Variabili di iterazione
    char action[100];               // Buffer per l'azione da eseguire

    // Inizializzazioni variabili per compiere le vari azioni dell'utente.
    char nome_utente_trasportatore[100];
    char nome[100];
    char cognome[100];
    char email[100];
    char categoriaUtente[100];

    char password[100];
    char confermaPassword[100];

    char vecchiaPassw[100];
    char nuovaPassw[100];

    char numeroTelefono[100];
    char nuovoNumeroTelefono[100];
    
    char dittaSpedizione[100];
    char nuovaDittaSpedizione[100];
    int idSpedizione;


    char outputs[100];

    UtenteTrasportatore trasportatore;
    Spedizione spedizione;
    /*  prg  */

#if (DEBUG > 0)
    setvbuf(stdout, (char *)NULL, _IONBF, 0);
    setvbuf(stderr, (char *)NULL, _IONBF, 0);
#endif

    // Imposta il nome utente.
    strcpy(username, "federico");

    // Ottenimento dell'ID del processo corrente.
    pid = getpid();

    // Connessione a Redis utilizzando l'indirizzo "localhost" e la porta di default 6379
    printf("main(): pid %d: user %s: connecting to redis ...\n", pid, username);
    c2r = redisConnect("localhost", 6379);
    // Controlla se la connessione è stata stabilita correttamente o se si è verificato un errore
    if (c2r == NULL || c2r->err) {
        if (c2r) {
            // Stampa l'errore specifico
            printf("Errore di connessione: %s\n", c2r->errstr);
            // Libera la risorsa redis
            redisFree(c2r);
        } else {
            // Stampa un messaggio di errore generico
            printf("Errore di allocazione del contesto Redis\n");
        }
    // Gestisci l'errore e termina il programma o riprova la connessione
    }
    // Stampa un messaggio di connessione riuscita.
    printf("main(): pid %d: user %s: connected to redis, %d\n", pid, username, c2r->err);


    // Eliminazione stream di lettura se esiste.
    reply = RedisCommand(c2r, "DEL %s", READ_STREAM_TRASPORTATORE);
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReply(c2r, reply);
    // Stampa la risposta del comando
    dumpReply(reply, 0);

    // Eliminazione stream di scrittura se esiste.
    reply = RedisCommand(c2r, "DEL %s", WRITE_STREAM_TRASPORTATORE);
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReply(c2r, reply);
    // Stampa la risposta del comando
    dumpReply(reply, 0);
    

    // Inizializza gli stream/gruppi
    initStreams(c2r, READ_STREAM_TRASPORTATORE);
    initStreams(c2r, WRITE_STREAM_TRASPORTATORE);

    printf("Streams create!\n");

    // Connessione al database.
    Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");
    // Stampa un messaggio di conferma della connessione al database
    printf("Connessione al database avvenuta con successo");


    while (1)
    {
        //  Lettura
        read_counter++;

        micro_sleep(7000000); // 7 secondi di attesa necessari per far sì che il client mandi tutte le richieste

        // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER.
        // Imposto COUNT a -1 per leggere tutti i messaggi disponibili nello stream
        reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_TRASPORTATORE);

        printf("\n\nmain(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_TRASPORTATORE);

        // Verifica la risposta del comando e termina il programma in caso di errore
        assertReply(c2r, reply);

        // Stampa la risposta del comando
        dumpReply(reply, 0);

        printf("Effettuato il dump! \n");

        // Elaborazione dei messaggi letti
        // Scorro il numero di Streams nella connessione Redis
        for (k = 0; k < ReadNumStreams(reply); k++)
        {
            ReadStreamName(reply, streamname, k);

            int numberMessageStream = ReadStreamNumMsg(reply, k);
            printf("Number of message about Stream = %d \n", numberMessageStream);
            
            // Scorro il numero di messaggi della Streams Redis
            for (i = 0; i < ReadStreamNumMsg(reply, k); i++)  
            {
                printf("\n\n\n\nPROSSIMO MESSAGGIO NELLA STREAM.");

                ReadStreamNumMsgID(reply, k, i, msgid); 

                printf("Message number %d from Stream: %d\n", i, k );
                printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                
                // Scorro il numero di valori del messaggio della Streams Redis
                // h deve partire da 0, altrimenti non troverà mai fval == "Action"
                for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h = h + 1)
                {
                    ReadStreamMsgVal(reply, k, i, h, fval);

                    printf("\nValue %d from message number %d from Stream: %d\n", h, i, k );
                    printf("main(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);


                    // Qui bisogna estrapolare l'azione da effettuare:
                    if (strcmp(fval, "Action") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        printf("\nFval: %s\n", fval);

                        strcpy(action, fval);
                        
                        printf("Action: %s\n\n", action);
                    }


                    // Una volta estrapolata selezionare tutti i parametri necessari per effettuarla.
                    if (strcmp(fval, "nome_utente_trasportatore") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(nome_utente_trasportatore, fval);
                    }
                    if (strcmp(fval, "nome") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(nome, fval);
                    }
                    if (strcmp(fval, "cognome") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(cognome, fval);
                    }
                    if (strcmp(fval, "categoriaUtente") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(categoriaUtente, fval);
                    }
                    if (strcmp(fval, "password") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(password, fval);
                    }
                    if (strcmp(fval, "nuovoNumeroTelefono") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(nuovoNumeroTelefono, fval);
                    }
                    if (strcmp(fval, "vecchiaPassw") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(vecchiaPassw, fval);
                    }
                    if (strcmp(fval, "nuovaPassw") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(nuovaPassw, fval);
                    }
                    if (strcmp(fval, "numeroTelefono") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(numeroTelefono, fval);
                    }
                    if (strcmp(fval, "email") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(email, fval);
                    }
                    
                    if (strcmp(fval, "confermaPassword") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(confermaPassword, fval);
                    }
                    
                    
                    
                    if (strcmp(fval, "dittaSpedizione") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(dittaSpedizione, fval);
                    }
                    if (strcmp(fval, "nuovaDittaSpedizione") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(nuovaDittaSpedizione, fval);
                    }
                    if (strcmp(fval, "idSpedizione") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        idSpedizione = atoi(fval);
                    }

                }  // for dei valori dell'i-esimo messaggio dell'i-esima Stream.

                printf("Azione: %s\n", action);


                if (std::string(action) == "EFFETTUA REGISTRAZIONE TRASPORTATORE")
                {
                    std::string sessionID = generateSessionID();
                    trasportatore.effettuaRegistrazione(db1, nome_utente_trasportatore, categoriaUtente, nome, cognome, sessionID, numeroTelefono, email, password, confermaPassword, dittaSpedizione);

                    strcpy(outputs, "Effettuata registrazione trasportatore");

                    // send result to client
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("Effettuata azione: %s\n", action);

                    printf("Result: %s \n", outputs);

                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply2->str);
                    freeReplyObject(reply2);
                }


                if (std::string(action) == "AGGIORNA NOME DITTASPEDIZIONE")
                {
                    trasportatore.aggiornaNomeDittaSpedizione(db1, nome_utente_trasportatore, nuovaDittaSpedizione);

                    strcpy(outputs, "Aggiornamento ditta spedizione");

                    // send result to client
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("Effettuata azione: %s\n", action);

                    printf("Result: %s \n", outputs);

                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply2->str);
                    freeReplyObject(reply2);

                }


                if (std::string(action) == "AVVISA SPEDIZIONE EFFETTUATA")
                {
                    spedizione.spedizioneConsegnata(db1, nome_utente_trasportatore, idSpedizione);
                    
                    strcpy(outputs, "Spedizione effettuata");

                    // send result to client
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("Effettuata azione: %s\n", action);

                    printf("Result: %s \n", outputs);

                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply2->str);
                    freeReplyObject(reply2);
                }


                if (std::string(action) == "ASSEGNA ORDINE TRASPORTATORE")
                {
                    spedizione.assegnaOrdineTrasportatore(db1);

                    strcpy(outputs, "Spedizione assegnata");

                    // send result to client
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("Effettuata azione: %s\n", action);

                    printf("Result: %s \n", outputs);

                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply->str);
                    freeReplyObject(reply2);
                }


                if (std::string(action) == "EFFETTUA LOGIN TRASPORTATORE")
                {
                    std::string sessionID = generateSessionID();

                    trasportatore.effettua_login(db1, nome_utente_trasportatore, password, sessionID);

                    strcpy(outputs, "Login utente trasportatore avvenuta");

                    // send result to client
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("Effettuata azione: %s\n", action);

                    printf("Result: %s \n", outputs);

                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply2->str);
                    freeReplyObject(reply2);
                }


                if (std::string(action) == "EFFETTUA LOGOUT TRASPORTATORE")
                {
                    trasportatore.effettua_logout(db1, nome_utente_trasportatore);

                    strcpy(outputs, "Logout utente trasportatore avvenuta");

                    // send result to client
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("Effettuata azione: %s\n", action);

                    printf("Result: %s \n", outputs);

                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply2->str);
                    freeReplyObject(reply2);
                }


                if (std::string(action) == "ELIMINA PROFILO TRASPORTATORE")
                {
                    trasportatore.elimina_profilo(db1, nome_utente_trasportatore);

                    strcpy(outputs, "Eliminazione utente trasportatore avvenuta");

                    // send result to client
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("Effettuata azione: %s\n", action);

                    printf("Result: %s \n", outputs);

                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply2->str);
                    freeReplyObject(reply2);
                }

                if (std::string(action) == "AGGIORNA NUMERO TELEFONO TRASPORTATORE")
                {
                    trasportatore.aggiornaNumeroDiTelefono(db1,nome_utente_trasportatore,  nuovoNumeroTelefono);

                    strcpy(outputs, "Aggiornamento numero di telefono trasportatore");

                    // send result to client
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("Effettuata azione: %s\n", action);

                    printf("Result: %s \n", outputs);

                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply2->str);
                    freeReplyObject(reply2);
                }

                if (std::string(action) == "AGGIORNA PASSWORD TRASPORTATORE")
                {
                    trasportatore.aggiornaPassword(db1, nome_utente_trasportatore,vecchiaPassw, nuovaPassw);

                    strcpy(outputs, "Aggiornamento password utente trasportatore");

                    // send result to client
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("Effettuata azione: %s\n", action);

                    printf("Result: %s \n", outputs);

                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply2->str);
                    freeReplyObject(reply2);
                }

            } // for del numero dei messaggi dell'i-esima Stream

        } // for dell'iterazione delle varie Stream

        freeReplyObject(reply);

    } // while ()


    redisFree(c2r);
}