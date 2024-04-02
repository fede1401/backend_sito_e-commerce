#include "include.h"


// cc -Wall -g -ggdb -o streams streams.c -lhiredis
// Usage: ./streams <add count> <read count> [block time, default: 1]

#define DEBUG 1000                          // Definizione di una costante per il debugging

#define READ_STREAM_CUSTOMER "stream1"      // Nome dello stream da cui leggere.
#define WRITE_STREAM_CUSTOMER "stream2"     // Nome dello stream su cui scrivere.

using namespace std;      // Consente di utilizzare le funzioni e le classi standard del C++ senza doverle qualificare con std::.
namespace fs = std::filesystem;


int main()
{

    redisContext *c2r;
    redisReply *reply;          // Inizializzazione risposta Redis
    redisReply *reply2;         // Inizializzazione risposta2 Redis

    int read_counter = 0;       // Contatore delle letture effettuate

    int num_richieste_client = 0;      // Variabile utilizzata per enumerare le richieste del client nel file corrispondente ai risultati del test.
    int num_risposte_server = 0;       // Variabile utilizzata per enumerare le risposte nel file corrispondente ai risultati del test.
    
    int block = 1000000000;     // Tempo di blocco per la lettura da stream in nanosecondi
    int pid;                    // ID del processo
    
    char username[100];
    char key[200];              // Buffer per la chiave da utilizzare in Redis
    char value[200];            // Buffer per il valore da utilizzare in Redis
    char streamname[100];       // Buffer per il nome dello stream Redis
    char msgid[100];            // Buffer per l'ID del messaggio Redis
    char fval[100];             // Buffer per il valore del campo del messaggio Redis
    int i, k, h;                // Variabili di iterazione
    char action[100];           // Buffer per l'azione da eseguire

    // Inizializzazioni variabili per compiere le vari azioni dell'utente.
    char nome_utente_compratore[100];
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

    char viaResidenza[100];
    char numeroCivico[100];
    char cap[100];
    char cittàResidenza[100];

    char nuovaViaResidenza[100];
    char nuovoNumCiv[100];
    char nuovoCAP[100];
    char nuovaCittaResidenza[100];

    char via_spedizione[100];
    char città_spedizione[100];
    char numero_civico_spedizione[100];
    char CAP_spedizione[100];
    
    char dataCompleanno[100];

    char numeroCartaPagamento[100];
    char cvvCartaPagamento[100];
    int idCarta;

    char nomeProdotto[100];
    int codiceProdotto;
    
    int idOrdine;
    
    char descrizioneRecensione[100];
    int idRecensione;
    char voto_stelle_str[100];

    char motivazione_reso_str[100];

    char outputs[100];


    UtenteCompratore compratore;
    Prodotto prodotto;
    Ordine ordine;
    Carta carta;
    Carrello carrello;
    ListaDesideri listadesideri;
    Reso reso;
    Recensione recensione;


    // Utilizzo di un std::unordered_map per associare il session ID all'oggetto UtenteCompratore;
    std::unordered_map<std::string, UtenteCompratore> sessionMapIDUtenteCompratore;


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
    reply = RedisCommand(c2r, "DEL %s", READ_STREAM_CUSTOMER);
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReply(c2r, reply);
    // Stampa la risposta del comando
    dumpReply(reply, 0);

    // Eliminazione stream di scrittura se esiste.
    reply = RedisCommand(c2r, "DEL %s", WRITE_STREAM_CUSTOMER);
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReply(c2r, reply);
    // Stampa la risposta del comando
    dumpReply(reply, 0);
    

    // Inizializza gli stream/gruppi
    initStreams(c2r, READ_STREAM_CUSTOMER);
    initStreams(c2r, WRITE_STREAM_CUSTOMER);

    printf("\nStreams create!\n");

    // Connessione al database.
    Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");
    // Stampa un messaggio di conferma della connessione al database
    printf("Connessione al database avvenuta con successo.\n\n");


    // Creo una cartella che conterrà i risultati dei test effettuati per gli utenti compratori.
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
    std::ofstream outputFile("../result/test-result-compratore.txt", std::ios::app);
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
        printf("Lettura dei messaggi sulla Streams corrispondente alle richieste del client. \n ");

        // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER corrispondenti alle richieste del client compratore.
        // Imposto COUNT a -1 per leggere tutti i messaggi disponibili nello stream
        reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

        printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

        // Verifica la risposta del comando e termina il programma in caso di errore
        assertReply(c2r, reply);

        // Stampa la risposta del comando
        dumpReply(reply, 0);


        // Elaborazione dei messaggi letti:

        // Scorro il numero di Streams nella connessione Redis
        for (k = 0; k < ReadNumStreams(reply); k++)
        {
            ReadStreamName(reply, streamname, k);
            
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


                    // Una volta estrapolata l'azione seleziono tutti i parametri necessari per effettuarla.
                    if (strcmp(fval, "nome_utente_compratore") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(nome_utente_compratore, fval);
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
                    if (strcmp(fval, "viaResidenza") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(viaResidenza, fval);
                    }
                    if (strcmp(fval, "numeroCivico") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(numeroCivico, fval);
                    }
                    if (strcmp(fval, "cap") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(cap, fval);
                    }
                    if (strcmp(fval, "cittàResidenza") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(cittàResidenza, fval);
                    }
                    if (strcmp(fval, "confermaPassword") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(confermaPassword, fval);
                    }
                    if (strcmp(fval, "dataCompleanno") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(dataCompleanno, fval);
                    }
                    if (strcmp(fval, "numeroCartaPagamento") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(numeroCartaPagamento, fval);
                    }
                    if (strcmp(fval, "cvvCartaPagamento") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(cvvCartaPagamento, fval);
                    }

                    if (strcmp(fval, "idCarta") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        idCarta = atoi(fval);
                    }

                    if (strcmp(fval, "nuovaViaResidenza") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(nuovaViaResidenza, fval);
                    }
                    if (strcmp(fval, "nuovoNumCiv") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(nuovoNumCiv, fval);
                    }
                    if (strcmp(fval, "nuovoCAP") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(nuovoCAP, fval);
                    }
                    if (strcmp(fval, "nuovaCittaResidenza") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(nuovaCittaResidenza, fval);
                    }
                    if (strcmp(fval, "codiceProdotto") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        codiceProdotto = atoi(fval);
                    }

                    if (strcmp(fval, "idOrdine") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        idOrdine = atoi(fval);
                    }
                    if (strcmp(fval, "nomeProdotto") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(nomeProdotto, fval);
                    }
                    if (strcmp(fval, "via_spedizione") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(via_spedizione, fval);
                    }
                    if (strcmp(fval, "città_spedizione") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(città_spedizione, fval);
                    }
                    if (strcmp(fval, "numero_civico_spedizione") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(numero_civico_spedizione, fval);
                    }
                    if (strcmp(fval, "CAP_spedizione") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(CAP_spedizione, fval);
                    }
                    if (strcmp(fval, "descrizioneRecensione") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(descrizioneRecensione, fval);
                    }
                    if (strcmp(fval, "idRecensione") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        idRecensione = atoi(fval);
                    }
                    if (strcmp(fval, "motivazioneReso") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(motivazione_reso_str, fval);
                    }
                    if (strcmp(fval, "voto_stella") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(voto_stelle_str, fval);
                    }
                

                } // for dei valori dell'i-esimo messaggio dell'i-esima Stream.


                // Effettuiamo l'azione all'i-esima iterazione del messaggio
                if (std::string(action) == "EFFETTUA REGISTRAZIONE COMPRATORE")
                {
                    
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << ", " << categoriaUtente << ", " << nome << ", " << cognome << ", " << numeroTelefono
                            << ", " << email << ", " << viaResidenza << ", " << numeroCivico << ", " << cap  << ", " << cittàResidenza  << ", " << password  << ", " << confermaPassword << ", " << dataCompleanno << " )\n" << std::endl;
                    

                    // Genero il sessionID
                    std::string sessionID = generateSessionID();

                    // Controllo se il sessionID generato dal server-customer sia univcoco nella tabella dell'Utente.
                    bool resultSession = check_sessionID(db1, "creazione sessionID per utente compratore", statoRequisito::Wait, sessionID);
                    
                    // Se il risultato è false il sessionID già esiste nel database, ed è stato assegnato ad un altro utente, devo generarlo un'altro finchè non ne se ne ha uno univoco.
                    if (resultSession == false)
                    {
                        // Genero il sessionID finchè non sia univoco il suo valore.
                        while (resultSession == false){
                            sessionID = generateSessionID();
                            resultSession = check_sessionID(db1, "creazione sessionID per utente compratore", statoRequisito::Wait, sessionID);
                        }
                    }

                    // Effettuo la registrazione dell'utente compratore e ottengo la risposta.
                    std::string outputRegistrazione =  compratore.registrazione(db1, nome_utente_compratore, categoriaUtente, nome, cognome, sessionID, numeroTelefono, email, 
                                                    viaResidenza, numeroCivico, cap,
                                                    cittàResidenza, password, confermaPassword, dataCompleanno);

                    sessionMapIDUtenteCompratore[sessionID] = compratore;

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs,  outputRegistrazione.c_str());


                    // Invio la risposta al client.                    
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;


                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;


                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta alla registrazione dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);

                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);

                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);

                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);   
                }


                if (std::string(action) == "EFFETTUA LOGIN COMPRATORE")
                {
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << ", " << password << " )\n" << std::endl;
                            
                    // Genero il sessionID
                    std::string sessionID = generateSessionID();

                    // Controllo se il sessionID generato dal server-customer sia univcoco nella tabella dell'Utente
                    bool resultSession = check_sessionID(db1, "creazione sessionID per utente compratore", statoRequisito::Wait, sessionID);
                    
                    // Se il risultato è false il sessionID già esiste nel database, ed è stato assegnato ad un altro utente, dobbiamo generarlo un'altro finchè non ne abbiamo uno univoco
                    if (resultSession == false)
                    {
                        // Genero il sessionID finchè non sia univoco il suo valore.
                        while (resultSession == false){
                            sessionID = generateSessionID();
                            resultSession = check_sessionID(db1, "creazione sessionID per utente compratore", statoRequisito::Wait, sessionID);
                        }
                    }
                    
                    // Effettuo il login dell'utente compratore e ottengo la risposta.
                    std::string outputLogin = compratore.login(db1, nome_utente_compratore, password, sessionID);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, outputLogin.c_str());

                    // Invio la risposta al client.      
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta al login dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);

                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);
                }


                if (std::string(action) == "EFFETTUA LOGOUT COMPRATORE")
                {
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << " )\n" << std::endl;

                    // Effettuo il logout dell'utente compratore e ottengo la risposta.
                    std::string outputLogout = compratore.logout(db1, nome_utente_compratore);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, outputLogout.c_str());

                    // Invio la risposta al client. 
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta al logout dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);

                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);

                }


                if (std::string(action) == "ELIMINA PROFILO COMPRATORE")
                {
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << " )\n" << std::endl;
                    
                    std::string eliminazioneProfilo = compratore.elimina_profilo(db1, nome_utente_compratore);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, eliminazioneProfilo.c_str());

                    // Invio la risposta al client. 
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);
                    
                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta all'eliminazione del profilo dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);


                }


                if (std::string(action) == "AGGIORNA NUMERO TELEFONO COMPRATORE")
                {
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << ", " << nuovoNumeroTelefono << " )\n" << std::endl;
                            
                    std::string aggiornamentoNumeroTelefono = compratore.aggiorna_numero_telefono(db1, nome_utente_compratore, nuovoNumeroTelefono);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, aggiornamentoNumeroTelefono.c_str());   

                    // Invio la risposta al client. 
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);
                    
                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta all'aggiornamento del numero di telefono dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);

                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);
              
                }


                if (std::string(action) == "AGGIORNA PASSWORD COMPRATORE")
                {
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << ", " << vecchiaPassw << ", " << nuovaPassw << " )\n" << std::endl;

                    std::string aggiornamentoPassword =  compratore.aggiorna_password(db1, nome_utente_compratore ,vecchiaPassw, nuovaPassw);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, aggiornamentoPassword.c_str());  

                    // Invio la risposta al client. 
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);                    

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta all'aggiornamento della password dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);

                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);
   
                }


                if (std::string(action) == "AGGIORNA RESIDENZA")
                {
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << ", " << nuovaViaResidenza << ", " << nuovoNumCiv << ", " << nuovoCAP << ", " << nuovaCittaResidenza << " )\n" << std::endl;   

                    std::string aggiornamentoResidenza = compratore.aggiorna_residenza(db1, nome_utente_compratore, nuovaViaResidenza, nuovoNumCiv, nuovoCAP, nuovaCittaResidenza);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, aggiornamentoResidenza.c_str());

                    // Invio la risposta al client. 
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);
                
                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta all'aggiornamento della residenza dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);

                }


                if (std::string(action) == "AGGIUNGI CARTA PAGAMENTO")
                {    
                     // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << ", " << numeroCartaPagamento << ", " << cvvCartaPagamento << " )\n" << std::endl;   

                    std::string aggiuntaCartaPagamento = carta.aggiungi_carta_pagamento(db1, nome_utente_compratore, numeroCartaPagamento, cvvCartaPagamento);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, aggiuntaCartaPagamento.c_str());

                    // Invio la risposta al client. 
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta all'aggiungimento della carta di pagamento dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);
                }


                if (std::string(action) == "RIMUOVI CARTA PAGAMENTO")
                {
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << ", " << idCarta  << " )\n" << std::endl;   

                    std::string rimozioneCartaPagamento = carta.rimuovi_carta_pagamento(db1, nome_utente_compratore, idCarta);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, rimozioneCartaPagamento.c_str());

                    // Invio la risposta al client. 
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    
                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta alla rimozionde della carta di pagamento dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);

                }


                if (std::string(action) == "AGGIUNGI PRODOTTO CARRELLO")
                {
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << ", " << codiceProdotto << " )\n" << std::endl;   

                    std::string aggiuntaProdottoCarrello = carrello.aggiungi_prodotto_carrello(db1, nome_utente_compratore, codiceProdotto );

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, aggiuntaProdottoCarrello.c_str());

                    // Invio la risposta al client. 
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta all'aggiungimento del prodotto nel carrello dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);
                }


                if (std::string(action) == "RIMUOVI PRODOTTO CARRELLO")
                {
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << ", " << codiceProdotto << " )\n" << std::endl; 

                    std::string rimozioneProdottoCarrello = carrello.rimuovi_prodotto_carrello(db1, nome_utente_compratore, codiceProdotto);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, rimozioneProdottoCarrello.c_str());

                    // Invio la risposta al client. 
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta alla rimozionde del prodotto dal carrello dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);
                }


                if (std::string(action) == "AGGIUNGI PRODOTTO LISTADESIDERI")
                {
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << ", " << codiceProdotto << " )\n" << std::endl; 

                    std::string aggiuntaProdottoListaDesideri = listadesideri.aggiungi_prodotto_lista_desideri(db1, nome_utente_compratore, codiceProdotto);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, aggiuntaProdottoListaDesideri.c_str());

                    // Invio la risposta al client. 
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta all'aggiungimento del prodotto nella lista desideri dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);
                }


                if (std::string(action) == "RIMUOVI PRODOTTO LISTADESIDERI")
                {
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << ", " << codiceProdotto << " )\n" << std::endl; 

                    std::string rimozioneProdottoListaDesideri = listadesideri.rimuovi_prodotto_lista_desideri(db1, nome_utente_compratore, codiceProdotto);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, rimozioneProdottoListaDesideri.c_str());

                    // Invio la risposta al client. 
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta alla rimozionde del prodotto dalla lista desideri dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);

                }


                if (std::string(action) == "ACQUISTA PRODOTTO")
                {
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << ", " << codiceProdotto << ", " << via_spedizione << ", " << città_spedizione << ", " << numero_civico_spedizione <<
                            CAP_spedizione << " )\n" << std::endl;   

                    std::string acquistoProdotto = prodotto.acquistaProdotto(db1, nome_utente_compratore, codiceProdotto, via_spedizione, città_spedizione, numero_civico_spedizione, CAP_spedizione);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, acquistoProdotto.c_str());

                    // Invio la risposta al client. 
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta all'acquisto del prodotto da parte dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);
                }


                if (std::string(action) == "RICERCA PRODOTTO")
                {
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << ", " << nomeProdotto << " )\n" << std::endl;   

                    std::string ricercaProdotto = prodotto.ricerca_mostra_Prodotto(db1, nome_utente_compratore, nomeProdotto);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, ricercaProdotto.c_str());

                    // Invio la risposta al client. 
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);           

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta alla ricerca del prodotto da parte dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);

                }


                if (std::string(action) == "VISIONA ORDINI EFFETTUATI")
                {
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << " )\n" << std::endl;  

                    std::string visionaOrdiniEffettuati = ordine.visione_ordini_effettuati(db1, nome_utente_compratore);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, visionaOrdiniEffettuati.c_str());

                    // Invio la risposta al client. 
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);
                    
                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta alla visione degli ordini effettuati dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);
                }


                if (std::string(action) == "ANNULLA ORDINE")
                {    
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << ", " << idOrdine << " )\n" << std::endl;  

                    std::string annullaOrdine = ordine.annulla_ordine(db1, nome_utente_compratore, idOrdine);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, annullaOrdine.c_str());

                    // Invio la risposta al client. 
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta all'annullamento dell'ordine dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);
                }


                if (std::string(action) == "EFFETTUA RESO")
                {    
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << ", " << idOrdine << ", " << motivazione_reso_str << " )\n" << std::endl;   

                    motivazioneReso motivazione_reso_enum = stringToStatoMotivazioneReso( motivazione_reso_str);
                    std::string effettuaReso = reso.effettua_reso(db1,nome_utente_compratore, idOrdine, motivazione_reso_enum);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, effettuaReso.c_str());

                    // Invio la risposta al client. 
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta all'effettuamento del reso dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);
                }


                if (std::string(action) == "EFFETTUA RECENSIONE")
                {
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << ", " << idOrdine << ", " << descrizioneRecensione << ", " << voto_stelle_str << " )\n" << std::endl;   

                    votoStelle voto_stelle_enum = stringToVotoStelle(voto_stelle_str);
                    std::string effettuaRecensione = recensione.effettua_recensione(db1, nome_utente_compratore, idOrdine, descrizioneRecensione, voto_stelle_enum);
                    
                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, effettuaRecensione.c_str());

                    // Invio la risposta al client. 
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta all'effettuamento della recensione dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);
                    
                    // Libera la risorsa della risposta
                    freeReplyObject(reply2);

                }


                if (std::string(action) == "RIMUOVI RECENSIONE")
                {
                    // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                    num_richieste_client++;
                    
                    // Scrive nel file
                    outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n" <<  action << " (" << nome_utente_compratore << ", " << idRecensione << " )\n" << std::endl;   

                    std::string rimuoviRecensione =  recensione.rimuovi_recensione(db1, nome_utente_compratore, idRecensione);

                    // Assegno alla variabile outputs la risposta dell'operazione.
                    strcpy(outputs, rimuoviRecensione.c_str());

                    // Invio la risposta al client. 
                    sprintf(key, "Result");
                    sprintf(value, "%s", outputs);

                    printf("\nElaborazione della richiesta del client dal server con risultato: %s\n", outputs);

                    // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                    num_risposte_server++;

                    // Scrivo nel file la risposta del server.
                    outputFile << "Risposta server numero: " << num_risposte_server << "\n" <<  outputs << "\n" << std::endl;

                    // Effettuo un comando di scrittura relativo all'elaborazione del server in risposta alla rimozionde della recensione dell'utente compratore.
                    reply2 = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key, value);
                    
                    // Verifica la risposta del comando e termina il programma in caso di errore
                    assertReplyType(c2r, reply2, REDIS_REPLY_STRING);
                    
                    printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key, value, reply2->str);
                    
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