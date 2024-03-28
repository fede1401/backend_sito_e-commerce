
#include "include.h"

#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2redis/src/con2redis.h"
#include <string.h>

#include "../../shared-server/generateSessionID.h"      // Migliore separazione delle responsabilità
#include "../../shared-server/checkSessionID.h"
#include <fstream>
#include <filesystem>



// cc -Wall -g -ggdb -o streams streams.c -lhiredis
// Usage: ./streams <add count> <read count> [block time, default: 1]

#define DEBUG 1000                                  // Definizione di una costante per il debugging

#define READ_STREAM_TRASPORTATORE "stream5"         // Nome dello stream da cui leggere.
#define WRITE_STREAM_TRASPORTATORE "stream6"        // Nome dello stream su cui scrivere.

using namespace std;        // Consente di utilizzare le funzioni e le classi standard del C++ senza doverle qualificare con std::.
namespace fs = std::filesystem;


int main()
{

    redisContext *c2r;
    redisReply *reply;              // Inizializzazione risposta Redis
    redisReply *reply2;             // Inizializzazione risposta2 Redis
    int read_counter = 0;           // Contatore delle letture effettuate
    int send_counter = 0;           // Contatore degli invii effettuati
    int block = 1000000000;         // Tempo di blocco per la lettura da stream in nanosecondi
    int pid;                        // ID del processo
    
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

    int num_richieste_trasportatore = 0;      // Variabile utilizzata per enumerare le richieste del trasportatore nel file corrispondente ai risultati del test.
    int num_risposte_server = 0;       // Variabile utilizzata per enumerare le risposte nel file corrispondente ai risultati del test.

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

    printf("\nStreams create!\n");

    // Connessione al database.
    Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");
    // Stampa un messaggio di conferma della connessione al database
    printf("Connessione al database avvenuta con successo");


    // Creo una cartella che conterrà i risultati dei test effettuati per gli utenti trasportatori.
    std::string folder_path = "../result";

    // Verifica se la cartella esiste già
    if (!fs::exists(folder_path)) {
        // Se non esiste, crea la cartella
        if (fs::create_directory(folder_path)) {
            std::cout << "Cartella creata con successo.\n";
        } else {
            std::cerr << "Errore durante la creazione della cartella.\n";
        }
    } else {
        std::cout << "La cartella esiste già.\n";
    }


    // Apre il file corrispondente ai risultati dei test in modalità scrittura (se il file non esiste, lo crea; altrimenti sovrascrive il contenuto)
    std::ofstream outputFile("../result/test-result-trasportatore.txt", std::ios::app);
    // Verifica se il file è stato aperto correttamente
    if (!outputFile.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1; // Termina il programma con un codice di errore
    }


    while (1)
    {
        //  Lettura
        read_counter++;

        //micro_sleep(7000000); // 7 secondi di attesa necessari per far sì che il client mandi tutte le richieste

        printf("\n----------------------------------------------------------------------------------\n");
        printf("Lettura dei messaggi sulla Streams corrispondente alle richieste del trasportatore. \n ");

        // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_TRASPORTATORE corrispondenti alle richieste del client trasportatore.
        // Imposto COUNT a -1 per leggere tutti i messaggi disponibili nello stream
        reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_TRASPORTATORE);

        printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_TRASPORTATORE);

        // Verifica la risposta del comando e termina il programma in caso di errore
        assertReply(c2r, reply);

        // Stampa la risposta del comando
        dumpReply(reply, 0);

        // Elaborazione dei messaggi letti
        
        // Scorro il numero di Streams nella connessione Redis
        for (k = 0; k < ReadNumStreams(reply); k++)
        {
            ReadStreamName(reply, streamname, k);

            int numberMessageStream = ReadStreamNumMsg(reply, k);
            
            // Scorro il numero di messaggi della Stream Redis
            for (i = 0; i < ReadStreamNumMsg(reply, k); i++)  
            {

                ReadStreamNumMsgID(reply, k, i, msgid); 

                printf("Numero messaggio: %d \n", i);
                printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                printf("\tElenco dei valori del messaggio numero: %d\n", i);
                
                // Scorro il numero di valori del messaggio della Stream Redis
                // h deve partire da 0, altrimenti non troverà mai fval == "Action"
                for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h = h + 1)
                {
                    ReadStreamMsgVal(reply, k, i, h, fval);

                    printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                    // Qui bisogna estrapolare l'azione da effettuare e la copiamo nella variabile action.
                    if (strcmp(fval, "Action") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);

                        strcpy(action, fval);                        
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


                // Effettuiamo l'azione all'i-esima iterazione del messaggio
                if (std::string(action) == "EFFETTUA REGISTRAZIONE TRASPORTATORE")
                {
                    // Incrementiamo il valore della richiesta del trasportatore che verrà scritta nel file di risultato dei test.
                    num_richieste_trasportatore++;
            
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_trasportatore << "\n" << "\nRichiesta client numero: " << num_richieste_trasportatore << "\n" <<  action << " (" << nome_utente_trasportatore << ", " << categoriaUtente << ", " << nome << ", " << cognome << ", " << numeroTelefono << ", " << email << ", " << 
                            password  << ", " << confermaPassword << ", " << dittaSpedizione << " )\n" << std::endl;

                    // Genero il sessionID
                    std::string sessionID = generateSessionID();

                    // Controllo se il sessionID generato dal server-customer sia univcoco nella tabella dell'Utente
                    bool resultSession = check_sessionID(db1, "creazione sessionID per utente trasportatore", statoRequisito::Wait, sessionID);
                    
                    // Se il risultato è false il sessionID già esiste nel database, ed è stato assegnato ad un altro utente, dobbiamo generarlo un'altro finchè non ne abbiamo uno univoco
                    if (resultSession == false)
                    {
                        // Genero il sessionID finchè non sia univoco il suo valore.
                        while (resultSession == false){
                            printf("Nuova generazione sessionID perchè già esistente\n\n\n\n");
                            sessionID = generateSessionID();
                            resultSession = check_sessionID(db1, "creazione sessionID per utente trasportatore", statoRequisito::Wait, sessionID);
                        }
                    }

                    // Effettuo la registrazione dell'utente fornitore e ottengo la risposta.
                    std::string outputRegistrazione = trasportatore.registrazione(db1, nome_utente_trasportatore, categoriaUtente, nome, cognome, sessionID, numeroTelefono, email, password, confermaPassword, dittaSpedizione);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, outputRegistrazione.c_str());

                    // Invio la risposta al trasportatore.
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    
                    printf("\nElaborazione della richiesta del trasportatore dal server con risultato: %s\n", outputs);                   // Scrive nel file per le risposte del server compratore.

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta alla registrazione dell'utente trasportatore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);

                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);

                }


                if (std::string(action) == "AGGIORNA NOME DITTASPEDIZIONE")
                {
                    // Incrementiamo il valore della richiesta del trasportatore che verrà scritta nel file di risultato dei test.
                    num_richieste_trasportatore++;
            
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_trasportatore << "\n" <<  action << " (" << nome_utente_trasportatore << ", " << nuovaDittaSpedizione << " )\n" << std::endl;

                    std::string aggiornaDittaSpedizione = trasportatore.aggiorna_nome_ditta_spedizione(db1, nome_utente_trasportatore, nuovaDittaSpedizione);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, aggiornaDittaSpedizione.c_str());

                    // Invio la risposta al trasportatore.
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    
                    printf("\nElaborazione della richiesta del trasportatore dal server con risultato: %s\n", outputs);                   

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta all'aggiornamento della ditta di spedizione dell'utente trasportatore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);

                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);


                }


                if (std::string(action) == "AVVISA SPEDIZIONE EFFETTUATA")
                {
                    // Incrementiamo il valore della richiesta del trasportatore che verrà scritta nel file di risultato dei test.
                    num_richieste_trasportatore++;
            
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_trasportatore << "\n" <<  action << " (" << nome_utente_trasportatore << ", " << idSpedizione << " )\n" << std::endl;

                    std::string avvisaSpedizioneConsegnata = spedizione.avvisa_spedizione_consegnata(db1, nome_utente_trasportatore, idSpedizione);
                    
                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, avvisaSpedizioneConsegnata.c_str());

                    // Invio la risposta al trasportatore.
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("\nElaborazione della richiesta del trasportatore dal server con risultato: %s\n", outputs);                   // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta all'avviso della spedizione effettuata dell'utente trasportatore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);

                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);

                }


                if (std::string(action) == "PRENDI IN CARICO SPEDIZIONE")
                {
                    // Incrementiamo il valore della richiesta del trasportatore che verrà scritta nel file di risultato dei test.
                    num_richieste_trasportatore++;
            
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_trasportatore << "\n" <<  action << " (" << nome_utente_trasportatore << " )\n" << std::endl;

                    std::string prendiInCaricoSpedizione = spedizione.prendi_in_carico_spedizione(db1, nome_utente_trasportatore);
                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, prendiInCaricoSpedizione.c_str());

                    // Invio la risposta al trasportatore.
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    
                    printf("\nElaborazione della richiesta del trasportatore dal server con risultato: %s\n", outputs);                   

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta alla presa in carico della spedizione dell'utente trasportatore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);

                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);

                }


                if (std::string(action) == "EFFETTUA LOGIN TRASPORTATORE")
                {
                    // Genero il sessionID
                    std::string sessionID = generateSessionID();

                    // Controllo se il sessionID generato dal server-customer sia univcoco nella tabella dell'Utente
                    bool resultSession = check_sessionID(db1, "creazione sessionID per utente trasportatore", statoRequisito::Wait, sessionID);
                    
                    // Se il risultato è false il sessionID già esiste nel database, ed è stato assegnato ad un altro utente, dobbiamo generarlo un'altro finchè non ne abbiamo uno univoco
                    if (resultSession == false)
                    {
                        while (resultSession == false){
                            sessionID = generateSessionID();
                            resultSession = check_sessionID(db1, "creazione sessionID per utente trasportatore", statoRequisito::Wait, sessionID);
                        }
                    }

                    // Incrementiamo il valore della richiesta del trasportatore che verrà scritta nel file di risultato dei test.
                    num_richieste_trasportatore++;
            
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_trasportatore << "\n" <<  action << " (" << nome_utente_trasportatore << ", " << password << " )\n" << std::endl;

                    std::string effettuaLogin = trasportatore.login(db1, nome_utente_trasportatore, password, sessionID);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, effettuaLogin.c_str());

                    // Invio la risposta al trasportatore.
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);


                    printf("\nElaborazione della richiesta del trasportatore dal server con risultato: %s\n", outputs);                   

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta al login dell'utente trasportatore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);

                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);

                }


                if (std::string(action) == "EFFETTUA LOGOUT TRASPORTATORE")
                {
                    // Incrementiamo il valore della richiesta del trasportatore che verrà scritta nel file di risultato dei test.
                    num_richieste_trasportatore++;
            
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_trasportatore << "\n" <<  action << " (" << nome_utente_trasportatore << " )\n" << std::endl;

                    std::string effettuaLogout = trasportatore.logout(db1, nome_utente_trasportatore);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, effettuaLogout.c_str());

                    // Invio la risposta al trasportatore.
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    
                    printf("\nElaborazione della richiesta del trasportatore dal server con risultato: %s\n", outputs);                   

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta al logout dell'utente trasportatore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);

                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);

                }


                if (std::string(action) == "ELIMINA PROFILO TRASPORTATORE")
                {
                    // Incrementiamo il valore della richiesta del trasportatore che verrà scritta nel file di risultato dei test.
                    num_richieste_trasportatore++;
            
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_trasportatore << "\n" <<  action << " (" << nome_utente_trasportatore << " )\n" << std::endl;

                    std::string eliminaProfilo =  trasportatore.elimina_profilo(db1, nome_utente_trasportatore);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, eliminaProfilo.c_str());

                    // Invio la risposta al trasportatore.
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    
                    printf("\nElaborazione della richiesta del trasportatore dal server con risultato: %s\n", outputs);                   

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta all'eliminazione dell'utente trasportatore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);

                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);

                }

                if (std::string(action) == "AGGIORNA NUMERO TELEFONO TRASPORTATORE")
                {
                    // Incrementiamo il valore della richiesta del trasportatore che verrà scritta nel file di risultato dei test.
                    num_richieste_trasportatore++;
            
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_trasportatore << "\n" <<  action << " (" << nome_utente_trasportatore << ", " << nuovoNumeroTelefono << " )\n" << std::endl;

                    std::string aggiornaNumeroTelefono = trasportatore.aggiorna_numero_telefono(db1,nome_utente_trasportatore,  nuovoNumeroTelefono);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, aggiornaNumeroTelefono.c_str());

                    // Invio la risposta al trasportatore.
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    
                    printf("\nElaborazione della richiesta del trasportatore dal server con risultato: %s\n", outputs);                   

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta all'aggiornamento del numero di telefono dell'utente trasportatore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);

                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);

                }

                if (std::string(action) == "AGGIORNA PASSWORD TRASPORTATORE")
                {
                    // Incrementiamo il valore della richiesta del trasportatore che verrà scritta nel file di risultato dei test.
                    num_richieste_trasportatore++;
            
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_trasportatore << "\n" <<  action << " (" << nome_utente_trasportatore << ", " << vecchiaPassw << " ," << nuovaPassw << " )\n" << std::endl;

                    std::string aggiornaPassword = trasportatore.aggiorna_password(db1, nome_utente_trasportatore,vecchiaPassw, nuovaPassw);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, aggiornaPassword.c_str());

                    // Invio la risposta al trasportatore.
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    
                    printf("\nElaborazione della richiesta del trasportatore dal server con risultato: %s\n", outputs);                   

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta all'aggiornamento della password dell'utente trasportatore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);

                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);

                }

            } // for del numero dei messaggi dell'i-esima Stream

        } // for dell'iterazione delle varie Stream

        freeReplyObject(reply);

    } // while ()


    outputFile.close(); // Chiudi il file

    redisFree(c2r);
}