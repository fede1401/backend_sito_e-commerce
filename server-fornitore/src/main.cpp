
#include "include.h"
#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2redis/src/con2redis.h"
#include <string.h>
#include "../../shared-server/generateSessionID.h"          // Migliore separazione delle responsabilità
#include "../../shared-server/checkSessionID.h"
#include <fstream>
#include <filesystem>


// cc -Wall -g -ggdb -o streams streams.c -lhiredis
// Usage: ./streams <add count> <read count> [block time, default: 1]

#define DEBUG 1000                              // Definizione di una costante per il debugging

#define READ_STREAM_FORNITORE "stream3"         // Nome dello stream da cui leggere.
#define WRITE_STREAM_FORNITORE "stream4"        // Nome dello stream su cui scrivere.

using namespace std;        // Consente di utilizzare le funzioni e le classi standard del C++ senza doverle qualificare con std::.
namespace fs = std::filesystem;

int main()
{

    redisContext *c2r;
    redisReply *reply;          // Inizializzazione risposta Redis
    redisReply *reply2;         // Inizializzazione risposta2 Redis
    int read_counter = 0;       // Contatore delle letture effettuate
    int send_counter = 0;       // Contatore degli invii effettuati
    int block = 1000000000;     // Tempo di blocco per la lettura da stream in nanosecondi
    int pid;                    // ID del processo
    //unsigned seed;
    char username[100];
    char key[100];              // Buffer per la chiave da utilizzare in Redis
    char value[100];            // Buffer per il valore da utilizzare in Redis
    char streamname[100];       // Buffer per il nome dello stream Redis
    char msgid[100];            // Buffer per l'ID del messaggio Redis
    char fval[100];             // Buffer per il valore del campo del messaggio Redis
    int i, k, h;                // Variabili di iterazione
    char action[100];           // Buffer per l'azione da eseguire

    // Inizializzazioni variabili per compiere le vari azioni dell'utente.
    char nome_utente_fornitore[100];
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
    
    char nomeProdotto[100];
    char categoriaProdotto[100];
    char descrizioneProdotto[100];
    int numeroCopieDisponibili;
    float prezzoProdotto;
    int codiceProdotto;
    
    char aziendaProduzione[100];
    char nuovaAziendaProduzione[100];
    
    char outputs[100];

    int num_richieste_fornitore = -1;      // Variabile utilizzata per enumerare le richieste del fornitore nel file corrispondente ai risultati del test.
    int num_risposte_server = -1;       // Variabile utilizzata per enumerare le risposte nel file corrispondente ai risultati del test.

    UtenteFornitore fornitore;
    Prodotto prodotto;
    /*  prg  */

#if (DEBUG > 0)
    setvbuf(stdout, (char *)NULL, _IONBF, 0);
    setvbuf(stderr, (char *)NULL, _IONBF, 0);
#endif

    // Imposta il nome utente.
    strcpy(username, "federico");

    // Ottenimento dell'identificatore del processo
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
    reply = RedisCommand(c2r, "DEL %s", READ_STREAM_FORNITORE);
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReply(c2r, reply);
    // Stampa la risposta del comando
    dumpReply(reply, 0);

    // Eliminazione stream di scrittura se esiste.
    reply = RedisCommand(c2r, "DEL %s", WRITE_STREAM_FORNITORE);
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReply(c2r, reply);
    // Stampa la risposta del comando
    dumpReply(reply, 0);
    

    // Inizializza gli stream/gruppi
    initStreams(c2r, READ_STREAM_FORNITORE);
    initStreams(c2r, WRITE_STREAM_FORNITORE);

    printf("\nStreams create!\n");

    // Connessione al database.
    Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");
    // Stampa un messaggio di conferma della connessione al database
    printf("Connessione al database avvenuta con successo");


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

    
    // Apre il file in modalità scrittura (se il file non esiste, lo crea; altrimenti sovrascrive il contenuto)
    std::ofstream outputFile("../result/test-result-fornitore.txt", std::ios::app);
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
        printf("Lettura dei messaggi sulla Streams corrispondente alle richieste del fornitore. \n ");

        // Effettuo un comando di lettura dei messaggi delle richieste del client fornitore sulla Stream di lettura READ_STREAM_CUSTOMER.
        //Imposto COUNT a -1 per leggere tutti i messaggi disponibili nello stream
        reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_FORNITORE);

        printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_FORNITORE);

        // Verifica la risposta del comando e termina il programma in caso di errore
        assertReply(c2r, reply);

        // Stampa la risposta del comando
        dumpReply(reply, 0);

        //printf("Effettuato il dump! \n");

        // Elaborazione dei messaggi letti
        // Scorro il numero di Streams nella connessione Redis
        for (k = 0; k < ReadNumStreams(reply); k++)
        {
            ReadStreamName(reply, streamname, k);

            int numberMessageStream = ReadStreamNumMsg(reply, k);
            
            // Scorro il numero di messaggi della Streams Redis
            for (i = 0; i < ReadStreamNumMsg(reply, k); i++)  
            {

                ReadStreamNumMsgID(reply, k, i, msgid); 

                printf("Numero messaggio: %d \n", i);
                printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                printf("\tElenco dei valori del messaggio numero: %d\n", i);
                
                // Scorro il numero di valori del messaggio della Streams Redis
                // h deve partire da 0, altrimenti non troverà mai fval == "Action"
                for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h = h + 1)
                {
                    ReadStreamMsgVal(reply, k, i, h, fval);

                    printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                    // Scrive nel file
                    // outputFile <<  fval << "\n" << std::endl;

                    // Estrapoliamo l'azione da effettuare e la salvo nella variabile action:
                    if (strcmp(fval, "Action") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        //printf("\nFval: %s\n", fval);

                        strcpy(action, fval);
                        
                        //printf("Action: %s\n\n", action);
                    }


                    // Una volta estrapolata l'azione selezioniamo tutti i parametri necessari per effettuarla
                    if (strcmp(fval, "nome_utente_fornitore") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(nome_utente_fornitore, fval);
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
                    
                    if (strcmp(fval, "nomeProdotto") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(nomeProdotto, fval);
                    }
                    
                    if (strcmp(fval, "codiceProdotto") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        codiceProdotto = atoi(fval);
                    }
                    
                    if (strcmp(fval, "categoriaProdotto") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(categoriaProdotto, fval);
                    }
                    if (strcmp(fval, "aziendaProduzione") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(aziendaProduzione, fval);
                    }
                    if (strcmp(fval, "nuovaAziendaProduzione") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(nuovaAziendaProduzione, fval);
                    }
                    if (strcmp(fval, "descrizioneProdotto") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(descrizioneProdotto, fval);
                    }
                    if (strcmp(fval, "prezzoProdotto") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        prezzoProdotto = atof(fval);
                    }
                    if (strcmp(fval, "numeroCopieDisponibili") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        numeroCopieDisponibili = atoi(fval);
                    }
                    

                }  // for dei valori dell'i-esimo messaggio dell'i-esima Stream.
                
                //printf("Azione: %s\n", action);

                if (std::string(action) == "EFFETTUA REGISTRAZIONE FORNITORE")
                {
                    // Incrementiamo il valore della richiesta del fornitore che verrà scritta nel file di risultato dei test.
                    num_richieste_fornitore++;
            
                    // Scrive nel file
                    outputFile << "\nRichiesta fornitore numero: " << num_richieste_fornitore << "\n" <<  action << " (" << nome_utente_fornitore << ", " << categoriaUtente << ", " << nome << ", " << cognome << ", " << numeroTelefono << ", " << email << ", " << 
                            password  << ", " << confermaPassword << ", " << aziendaProduzione << " )\n" << std::endl;
                    
                    // Genero il sessionID
                    std::string sessionID = generateSessionID();

                    // Controllo se il sessionID generato dal server-customer sia univcoco nella tabella dell'Utente
                    bool resultSession = check_sessionID(db1, "creazione sessionID per utente fornitore", statoRequisito::Wait, sessionID);
                    
                    // Se il risultato è false il sessionID già esiste nel database, ed è stato assegnato ad un altro utente, dobbiamo generarlo un'altro finchè non ne abbiamo uno univoco
                    if (resultSession == false)
                    {
                        while (resultSession == false){
                            sessionID = generateSessionID();
                            resultSession = check_sessionID(db1, "creazione sessionID per utente fornitore", statoRequisito::Wait, sessionID);
                        }
                    }
                    
                    std::string outputRegistrazione = fornitore.registrazione(db1, nome_utente_fornitore, categoriaUtente, nome, cognome, sessionID, numeroTelefono, email, password, confermaPassword, aziendaProduzione);

                    strcpy(outputs, outputRegistrazione.c_str());

                    // send result to client
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    ////printf("Effettuata azione: %s\n", action);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    printf("\nElaborazione della richiesta del fornitore dal server con risultato: %s\n", outputs);

                    //printf("Result: %s \n", outputs);

                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_FORNITORE, key, value);
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE, key, value, reply2->str);
                    freeReplyObject(reply2);
                }


                if (std::string(action) == "AGGIORNA NOME AZIENDAPRODUZIONE")
                {   
                    // Incrementiamo il valore della richiesta del fornitore che verrà scritta nel file di risultato dei test.
                    num_richieste_fornitore++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta fornitore numero: " << num_richieste_fornitore << "\n" <<  action << " (" << nome_utente_fornitore << ", " << nuovaAziendaProduzione << " )\n" << std::endl;

                    std::string aggiornaAziendaProduttrice = fornitore.aggiorna_nome_azienda_produttrice(db1, nome_utente_fornitore,nuovaAziendaProduzione);

                    strcpy(outputs, aggiornaAziendaProduttrice.c_str());

                    // send result to client
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    ////printf("Effettuata azione: %s\n", action);

                    printf("\nElaborazione della richiesta del fornitore dal server con risultato: %s\n", outputs);

                    //printf("Result: %s \n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_FORNITORE, key, value);
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE, key, value, reply2->str);
                    freeReplyObject(reply2);
                    
                }


                if (std::string(action) == "AGGIUNGI PRODOTTO SITO")
                {
                    // Incrementiamo il valore della richiesta del fornitore che verrà scritta nel file di risultato dei test.
                    num_richieste_fornitore++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta fornitore numero: " << num_richieste_fornitore << "\n" <<  action << " (" << nome_utente_fornitore << ", " << nomeProdotto << ", " << categoriaProdotto << ", " << prezzoProdotto << ", " << descrizioneProdotto << ", " << aziendaProduzione << ", " << 
                            numeroCopieDisponibili << " )\n" << std::endl;

                    std::string aggiuntaProdottoSito = prodotto.aggiungi_prodotto_sito(db1, nome_utente_fornitore, nomeProdotto, categoriaProdotto, prezzoProdotto, descrizioneProdotto, aziendaProduzione, numeroCopieDisponibili);

                    strcpy(outputs, aggiuntaProdottoSito.c_str());

                    // send result to client
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    //printf("Effettuata azione: %s\n", action);

                    printf("\nElaborazione della richiesta del fornitore dal server con risultato: %s\n", outputs);

                    //printf("Result: %s \n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_FORNITORE, key, value);
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE, key, value, reply2->str);
                    freeReplyObject(reply2);

                }


                if (std::string(action) == "RIMUOVI PRODOTTO SITO")
                {
                    // Incrementiamo il valore della richiesta del fornitore che verrà scritta nel file di risultato dei test.
                    num_richieste_fornitore++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta fornitore numero: " << num_richieste_fornitore << "\n" <<  action << " (" << nome_utente_fornitore << ", " << codiceProdotto << " )\n" << std::endl;

                    std::string rimozioneProdottoSito = prodotto.rimuovi_prodotto_sito(db1, nome_utente_fornitore, codiceProdotto);

                    strcpy(outputs, rimozioneProdottoSito.c_str());

                    // send result to client
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    //printf("Effettuata azione: %s\n", action);

                    printf("\nElaborazione della richiesta del fornitore dal server con risultato: %s\n", outputs);

                    //printf("Result: %s \n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_FORNITORE, key, value);
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE, key, value, reply2->str);
                    freeReplyObject(reply2);
                }
                

                if (std::string(action) == "EFFETTUA LOGIN FORNITORE")
                {
                    // Incrementiamo il valore della richiesta del fornitore che verrà scritta nel file di risultato dei test.
                    num_richieste_fornitore++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta fornitore numero: " << num_richieste_fornitore << "\n" <<  action << " (" << nome_utente_fornitore << ", " << password << " )\n" << std::endl;

                    // Genero il sessionID
                    std::string sessionID = generateSessionID();

                    // Controllo se il sessionID generato dal server-customer sia univcoco nella tabella dell'Utente
                    bool resultSession = check_sessionID(db1, "creazione sessionID per utente fornitore", statoRequisito::Wait, sessionID);
                    
                    // Se il risultato è false il sessionID già esiste nel database, ed è stato assegnato ad un altro utente, dobbiamo generarlo un'altro finchè non ne abbiamo uno univoco
                    if (resultSession == false)
                    {
                        while (resultSession == false){
                            sessionID = generateSessionID();
                            resultSession = check_sessionID(db1, "creazione sessionID per utente fornitore", statoRequisito::Wait, sessionID);
                        }
                    }

                    std::string effettuaLogin = fornitore.login(db1, nome_utente_fornitore, password, sessionID);

                    strcpy(outputs, effettuaLogin.c_str());

                    // send result to client
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    //printf("Effettuata azione: %s\n", action);

                    printf("\nElaborazione della richiesta del fornitore dal server con risultato: %s\n", outputs);

                    //printf("Result: %s \n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_FORNITORE, key, value);
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE, key, value, reply2->str);
                    freeReplyObject(reply2);

                }


                if (std::string(action) == "EFFETTUA LOGOUT FORNITORE")
                {
                    // Incrementiamo il valore della richiesta del fornitore che verrà scritta nel file di risultato dei test.
                    num_richieste_fornitore++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta fornitore numero: " << num_richieste_fornitore << "\n" <<  action << " (" << nome_utente_fornitore << " )\n" << std::endl;

                    std::string effettuaLogout =  fornitore.logout(db1, nome_utente_fornitore);

                    strcpy(outputs, effettuaLogout.c_str());

                    // send result to client
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    //printf("Effettuata azione: %s\n", action);

                    printf("\nElaborazione della richiesta del fornitore dal server con risultato: %s\n", outputs);

                    //printf("Result: %s \n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_FORNITORE, key, value);
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE, key, value, reply2->str);
                    freeReplyObject(reply2);
                }


                if (std::string(action) == "ELIMINA PROFILO FORNITORE")
                {
                    // Incrementiamo il valore della richiesta del fornitore che verrà scritta nel file di risultato dei test.
                    num_richieste_fornitore++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta fornitore numero: " << num_richieste_fornitore << "\n" <<  action << " (" << nome_utente_fornitore << " )\n" << std::endl;

                    std::string eliminaProfilo = fornitore.elimina_profilo(db1, nome_utente_fornitore);

                    strcpy(outputs, eliminaProfilo.c_str());

                    // send result to client
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    //printf("Effettuata azione: %s\n", action);

                    printf("\nElaborazione della richiesta del fornitore dal server con risultato: %s\n", outputs);

                    //printf("Result: %s \n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_FORNITORE, key, value);
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE, key, value, reply2->str);
                    freeReplyObject(reply2);
                }


                if (std::string(action) == "AGGIORNA NUMERO TELEFONO FORNITORE")
                {
                    // Incrementiamo il valore della richiesta del fornitore che verrà scritta nel file di risultato dei test.
                    num_richieste_fornitore++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta fornitore numero: " << num_richieste_fornitore << "\n" <<  action << " (" << nome_utente_fornitore << ", " << nuovoNumeroTelefono << " )\n" << std::endl;

                    std::string aggiornaNumeroTelefono = fornitore.aggiorna_numero_telefono(db1, nome_utente_fornitore, nuovoNumeroTelefono);

                    strcpy(outputs, aggiornaNumeroTelefono.c_str());

                    // send result to client
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("Effettuata azione: %s\n",action);

                    printf("\nElaborazione della richiesta del fornitore dal server con risultato: %s\n", outputs);

                    //printf("Result: %s \n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_FORNITORE, key, value);
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE, key, value, reply2->str);
                    freeReplyObject(reply2);
                }


                if (std::string(action) == "AGGIORNA PASSWORD FORNITORE")
                {  
                    // Incrementiamo il valore della richiesta del fornitore che verrà scritta nel file di risultato dei test.
                    num_richieste_fornitore++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta fornitore numero: " << num_richieste_fornitore << "\n" <<  action << " (" << nome_utente_fornitore << ", " << vecchiaPassw << " ," << nuovaPassw << " )\n" << std::endl;

                    std::string aggiornaPassword = fornitore.aggiorna_password(db1, nome_utente_fornitore, vecchiaPassw, nuovaPassw);

                    strcpy(outputs, aggiornaPassword.c_str());

                    // send result to client
                    send_counter++;
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    //printf("Effettuata azione: %s\n", action);

                    printf("\nElaborazione della richiesta del fornitore dal server con risultato: %s\n", outputs);

                    //printf("Result: %s \n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_FORNITORE, key, value);
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE, key, value, reply2->str);
                    freeReplyObject(reply2);

                }

            } // for del numero dei messaggi dell'i-esima Stream

        } // for dell'iterazione delle varie Stream

        freeReplyObject(reply);
        

    } // while ()


    outputFile.close(); // Chiudi il file


    redisFree(c2r);
}