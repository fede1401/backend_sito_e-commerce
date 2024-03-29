
#include "main.h"
#include <array>
#include <string>
#include <string.h>
#include <fstream>
#include <filesystem>

// cc -Wall -g -ggdb -o streams streams.c -lhiredis
// Usage: ./streams <add count> <read count> [block time, default: 1]

#define DEBUG 1000 // Definisci una costante per abilitare il debug

#define READ_STREAM_CUSTOMER "stream2"  // Nome dello stream da cui leggere.
#define WRITE_STREAM_CUSTOMER "stream1" // Nome dello stream su cui scrivere.

using namespace std; // Consente di utilizzare le funzioni e le classi standard del C++ senza doverle qualificare con std::.
namespace fs = std::filesystem;

int main()
{

    redisContext *c2r;
    redisReply *reply;      // Inizializzazione risposta Redis
    int read_counter = 0;   // Contatore delle letture effettuate
    int send_counter = 0;   // Contatore degli invii effettuati
    int block = 1000000000; // Tempo di blocco per la lettura da stream in nanosecondi
    int pid;                // ID del processo
    
    char username[100];
    char key1[100];
    char value1[100];
    char key2[100];
    char value2[100];
    char key3[100];
    char value3[100];
    char key4[100];
    char value4[100];
    char key5[100];
    char value5[100];
    char key6[100];
    char value6[100];
    char key7[100];
    char value7[100];
    char key8[100];
    char value8[100];
    char key9[100];
    char value9[100];
    char key10[100];
    char value10[100];
    char key11[100];
    char value11[100];
    char key12[100];
    char value12[100];
    char key13[100];
    char value13[100];
    char key14[100];
    char value14[100];

    int num_richieste_client = 0; // Variabile utilizzata per enumerare le richieste del client nel file corrispondente ai risultati del test.
    int num_risposte_server = 0;  // Variabile utilizzata per enumerare le risposte nel file corrispondente ai risultati del test.

    char streamname[100]; // Buffer per il nome dello stream Redis
    char msgid[100];      // Buffer per l'ID del messaggio Redis
    char fval[100];       // Buffer per il valore del campo del messaggio Redis
    int i, k, h;   // Variabili di iterazione

    /*  prg  */

#if (DEBUG > 0)
    setvbuf(stdout, (char *)NULL, _IONBF, 0);
    setvbuf(stderr, (char *)NULL, _IONBF, 0);
#endif

    // Imposta il nome utente.
    strcpy(username, "federico");

    // Ottenimento dell'ID del processo corrente
    pid = getpid();

    // Connessione a Redis utilizzando l'indirizzo "localhost" e la porta di default 6379
    printf("main(): pid %d: user %s: connecting to redis ...\n", pid, username);
    c2r = redisConnect("localhost", 6379);
    // Stampa un messaggio di connessione riuscita.
    printf("main(): pid %d: user %s: connected to redis\n", pid, username);


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


    // Creazione degli stream/gruppi
    initStreams(c2r, READ_STREAM_CUSTOMER);
    initStreams(c2r, WRITE_STREAM_CUSTOMER);

    printf("Creazione Streams\n");


    /* init random number generator  */
    srand((unsigned)time(NULL));


    // Apre il file in modalità di lettura corrispondente al test da effettuare.
    // gestione_acquisiti
    // test_sessioni_gestioneprofilo
    // test_post_acquisto.txt
    std::ifstream file("../test/test_sessioni_gestioneprofilo.txt");
    if (!file.is_open())
    {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }


    // Creo una cartella che conterrà i risultati dei test effettuati per gli utenti compratori.
    std::string folder_path = "../result";

    // Verifica se la cartella esiste già
    if (!fs::exists(folder_path))
    {
        // Se non esiste, crea la cartella
        if (fs::create_directory(folder_path))
        {
            std::cout << "Cartella creata con successo.\n";
        } else
        {
            std::cerr << "Errore durante la creazione della cartella.\n";
        }
    }
    else
    {
        std::cout << "La cartella esiste già.\n";
    }

    // Apre il file corrispondente ai risultati dei test in modalità scrittura (se il file non esiste, lo crea; altrimenti sovrascrive il contenuto)
    std::ofstream outputFile("../result/test-result-compratore.txt", std::ios::app);
    // Verifica se il file è stato aperto correttamente
    if (!outputFile.is_open())
    {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1; // Termina il programma con un codice di errore
    }


    // Leggiamo il file corrispondente ai test una riga per volta. 
    // Se si trova una linea corrispondente all'azione da effettuare , prendiamo tutti i parametri necessari nelle righe seguenti e inviamo la richiesta al server.
    std::string line;
    while (std::getline(file, line)) // Legge una riga per volta
    {

        if (line == "EFFETTUA REGISTRAZIONE COMPRATORE")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "categoriaUtente");
            sprintf(value3, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key4, "nome");
            sprintf(value4, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key5, "cognome");
            sprintf(value5, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key6, "numeroTelefono");
            sprintf(value6, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key7, "email");
            sprintf(value7, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key8, "viaResidenza");
            sprintf(value8, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key9, "numeroCivico");
            sprintf(value9, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key10, "cap");
            sprintf(value10, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key11, "cittàResidenza");
            sprintf(value11, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key12, "password");
            sprintf(value12, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key13, "confermaPassword");
            sprintf(value13, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key14, "dataCompleanno");
            sprintf(value14, line.c_str());

            // Effettuo un comando di scrittura relativo alla registrazione dell'utente compratore
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                                 WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                                 key9, value9, key10, value10, key11, value11, key12, value12, key13, value13, key14, value14);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s, %s: %s, %s: %s, %s: %s, %s: %s, %s: %s, %s: %s, %s: %s, %s: %s, %s: %s, %s: %s, %s: %s)  (id: %s)\n\n",
                   pid, WRITE_STREAM_CUSTOMER,
                   key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8, key9, value9, key10, value10, key11, value11, key12, value12, key13, value13, key14, value14, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << ", " << value3 << ", " << value4 << ", " << value5 << ", " << value6 << ", " << value7 << ", " << value8 << ", " << value9 << ", " << value10 << ", "
                       << value11 << ", " << value12 << ", " << value13 << ", " << value14 << " )\n"
                       << std::endl;

            // Libera la risorsa della risposta
            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);
                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);

                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }


        if (line == "EFFETTUA LOGIN COMPRATORE")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "password");
            sprintf(value3, line.c_str());

            // Effettuo un comando di scrittura relativo al logout dell'utente compratore.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << ", " << value3 << " )\n"
                       << std::endl;

            // Libera la risorsa della risposta
            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);

                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);
                    
                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "EFFETTUA LOGOUT COMPRATORE")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            // Effettuo un comando di scrittura relativo al logout dell'utente compratore.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << " )\n"
                       << std::endl;

            // Libera la risorsa della risposta
            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);
                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);

                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "AGGIORNA NUMERO TELEFONO COMPRATORE")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "nuovoNumeroTelefono");
            sprintf(value3, line.c_str());

            // Effettuo un comando di scrittura relativo all'aggiornamento di numero di telefono.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s (%s: %s, %s: %s)  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << ", " << value3 << " )\n"
                       << std::endl;

            // Libera la risorsa della risposta
            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("\nLettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);

                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);
                    
                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }
            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }


        if (line == "AGGIORNA PASSWORD COMPRATORE")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "vecchiaPassw");
            sprintf(value3, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key4, "nuovaPassw");
            sprintf(value4, line.c_str());

            // Effettuo un comando di scrittura relativo all'aggiornamento della password.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s , %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << ", " << value3 << ", " << value4 << " )\n"
                       << std::endl;

            // Libera la risorsa della risposta
            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);

                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);
                    
                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "AGGIORNA RESIDENZA")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "nuovaViaResidenza");
            sprintf(value3, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key4, "nuovoNumCiv");
            sprintf(value4, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key5, "nuovoCAP");
            sprintf(value5, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key6, "nuovaCittaResidenza");
            sprintf(value6, line.c_str());

            // Effettuo un comando di scrittura relativo all'aggiornamento della residenza.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s, %s: %s, %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << ", " << value3 << ", " << value4 << ", " << value5 << ", " << value6 << " )\n"
                       << std::endl;

            // Libera la risorsa della risposta
            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);

                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);
                    
                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "AGGIUNGI PRODOTTO CARRELLO")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "codiceProdotto");
            sprintf(value3, line.c_str());

            // Effettuo un comando di scrittura relativo all'aggiungimento del prodtto nel carrello.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s)  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << ", " << value3 << " )\n"
                       << std::endl;

            // Libera la risorsa della risposta
            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);

                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);
                    
                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "AGGIUNGI PRODOTTO LISTADESIDERI")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "codiceProdotto");
            sprintf(value3, line.c_str());

            // Effettuo un comando di scrittura relativo all'aggiungimento del prodtto nel carrello.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s)  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << ", " << value3 << " )\n"
                       << std::endl;

            // Libera la risorsa della risposta
            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);

                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);
                    
                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "RIMUOVI PRODOTTO CARRELLO")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "codiceProdotto");
            sprintf(value3, line.c_str());

            // Effettuo un comando di scrittura relativo alla rimozione del prodtto nel carrello.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s)  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << ", " << value3 << " )\n"
                       << std::endl;

            // Libera la risorsa della risposta
            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);

                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);
                    
                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "RIMUOVI PRODOTTO LISTADESIDERI")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "codiceProdotto");
            sprintf(value3, line.c_str());

            // Effettuo un comando di scrittura relativo alla rimozione del prodtto nel carrello.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << ", " << value3 << " )\n"
                       << std::endl;

            // Libera la risorsa della risposta
            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);

                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);
                    
                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "AGGIUNGI CARTA PAGAMENTO")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "numeroCartaPagamento");
            sprintf(value3, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key4, "cvvCartaPagamento");
            sprintf(value4, line.c_str());

            // Effettuo un comando di scrittura relativo all'aggiungimento della carta di pagamento.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s : %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << ", " << value3 << ", " << value4 << " )\n"
                       << std::endl;

            // Libera la risorsa della risposta
            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);

                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);
                    
                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "RIMUOVI CARTA PAGAMENTO")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "idCarta");
            sprintf(value3, line.c_str());

            // Effettuo un comando di scrittura relativo alla rimozione della carta di pagamento.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << ", " << value3 << " )\n"
                       << std::endl;

            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);

                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);
                    
                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "RICERCA PRODOTTO")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "nomeProdotto");
            sprintf(value3, line.c_str());

            // Effettuo un comando di scrittura relativo alla rimozione della carta di pagamento.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << ", " << value3 << " )\n"
                       << std::endl;

            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);

                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);
                    
                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "ACQUISTA PRODOTTO")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "codiceProdotto");
            sprintf(value3, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key4, "via_spedizione");
            sprintf(value4, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key5, "città_spedizione");
            sprintf(value5, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key6, "numero_civico_spedizione");
            sprintf(value6, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key7, "CAP_spedizione");
            sprintf(value7, line.c_str());

            // Effettuo un comando di scrittura relativo all'acquisto del prodotto.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s, %s: %s, %s: %s, %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << ", " << value3 << ", " << value4 << ", " << value5 << ", " << value6 << ", " << value7 << " )\n";

            // Libera la risorsa della risposta
            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);

                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);
                    
                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "VISIONA ORDINI EFFETTUATI")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            // Effettuo un comando di scrittura relativo alla visione degli ordini effettuati.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << " )\n"
                       << std::endl;

            // Libera la risorsa della risposta
            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);

                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);
                    
                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "ELIMINA PROFILO COMPRATORE")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            // Effettuo un comando di scrittura relativo all'eliminazione dell'utente compratore.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << " )\n"
                       << std::endl;

            // Libera la risorsa della risposta
            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);

                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);
                    
                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "ANNULLA ORDINE")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "idOrdine");
            sprintf(value3, line.c_str());

            // Effettuo un comando di scrittura relativo all'annullamento dell'ordine.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << ", " << value3 << " )\n"
                       << std::endl;

            // Libera la risorsa della risposta
            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);

                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);
                    
                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "EFFETTUA RESO")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "idOrdine");
            sprintf(value3, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key4, "motivazioneReso");
            sprintf(value4, line.c_str());

            // Effettuo un comando di scrittura relativo all'effettuazione del reso.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << ", " << value3 << ", " << value4 << " )\n"
                       << std::endl;

            // Libera la risorsa della risposta
            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);

                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);
                    
                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "EFFETTUA RECENSIONE")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "idOrdine");
            sprintf(value3, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key4, "descrizioneRecensione");
            sprintf(value4, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key5, "voto_stella");
            sprintf(value5, line.c_str());

            // Effettuo un comando di scrittura relativo all'effettuazione del reso.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s, %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << ", " << value3 << ", " << value4 << ", " << value5 << " )\n"
                       << std::endl;

            // Libera la risorsa della risposta
            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);

                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);
                    
                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "RIMUOVI RECENSIONE")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_compratore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "idRecensione");
            sprintf(value3, line.c_str());

            // Effettuo un comando di scrittura relativo alla rimozione del reso.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
            printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

            // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
            num_richieste_client++;

            // Scrive nel file
            outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                       << value1 << " ( " << value2 << ", " << value3 << " )\n"
                       << std::endl;

            // Libera la risorsa della risposta
            freeReplyObject(reply);


            //  Lettura dei risultati dal server
            read_counter++;

            // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
            reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

            printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

            printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReply(c2r, reply);

            // Stampa la risposta del comando
            dumpReply(reply, 0);

            // Scorro il numero di Streams nella connessione Redis
            for (k = 0; k < ReadNumStreams(reply); k++)
            {
                ReadStreamName(reply, streamname, k);

                // Scorro il numero di messaggi della Stream Redis
                for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                {
                    ReadStreamNumMsgID(reply, k, i, msgid);

                    printf("Numero messaggio: %d\n", i);

                    printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                    printf("\tElenco dei valori del messaggio numero: %d\n", i);
                    
                    // Scorro il numero di valori del messaggio della Stream Redis
                    for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                    {
                        ReadStreamMsgVal(reply, k, i, h, fval);
                        printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                        if (h == 1)
                        {
                            // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                            num_risposte_server++;

                            // Scrive nel file corrispondente alla risposta del server
                            outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                       << fval << "\n"
                                       << std::endl;
                        }
                    }
                }
            }

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

    } // while scorrimento linee file

    file.close(); // Chiude il file

    /* sleep   */
    // micro_sleep(10000000); // 10 secondi di attesa



    // Esecuzione test randomici:

    std::string azioniCompratore[20] = {"EFFETTUA REGISTRAZIONE COMPRATORE", "EFFETTUA LOGIN COMPRATORE", "EFFETTUA LOGOUT COMPRATORE", "ELIMINA PROFILO COMPRATORE",
                                        "AGGIORNA NUMERO TELEFONO COMPRATORE", "AGGIORNA PASSWORD COMPRATORE", "AGGIORNA RESIDENZA", "AGGIUNGI CARTA PAGAMENTO",
                                        "RIMUOVI CARTA PAGAMENTO", "AGGIUNGI PRODOTTO CARRELLO", "AGGIUNGI PRODOTTO LISTADESIDERI", "RIMUOVI PRODOTTO CARRELLO", "RIMUOVI PRODOTTO LISTADESIDERI",
                                        "RICERCA PRODOTTO", "ACQUISTA PRODOTTO", "VISIONA ORDINI EFFETTUATI", "EFFETTUA RECENSIONE", "ANNULLA ORDINE", "EFFETTUA RESO", "RIMUOVI RECENSIONE"};

    // Array di nomi utente
    std::array<std::string, 30> nomi_utente;
    for (int i = 0; i < 30; ++i)
    {
        nomi_utente[i] = "Utente" + std::to_string(i + 1);
    }

    // Array di nomi
    std::array<std::string, 10> nomi = {"Mario", "Luigi", "Giovanni", "Alessia", "Chiara", "Francesco", "Elena", "Roberto", "Laura", "Paolo"};

    // Array di cognomi
    std::array<std::string, 10> cognomi = {"Rossi", "Bianchi", "Verdi", "Ferrari", "Russo", "Esposito", "Romano", "Gallo", "Conti", "De Luca"};

    // Array di email
    std::array<std::string, 30> email;
    for (int i = 0; i < 30; ++i)
    {
        email[i] = nomi_utente[i] + "@example.com";
    }

    // Array di numeri di telefono
    std::array<std::string, 30> numeri_telefono;
    for (int i = 0; i < 30; ++i)
    {
        numeri_telefono[i] = "123456789" + std::to_string(i);
    }

    // Array di password
    std::array<std::string, 30> password;
    for (int i = 0; i < 30; ++i)
    {
        password[i] = "P.assword" + std::to_string(i + 1);
    }

    // Array di nomi di prodotti
    std::array<std::string, 30> nomi_prodotti;
    for (int i = 0; i < 30; ++i)
    {
        nomi_prodotti[i] = "Prodotto" + std::to_string(i + 1);
    }

    // Array di nomi di prodotti
    std::string descrizioneProdotti = "Prodotto eccellente.";

    // Array di categorie prodotti
    std::array<std::string, 10> categorie_prodotti = {"Abbigliamento", "Elettronica", "Informatica", "Arredamento", "Culinario", "Sportivo", "Illuminazione", "Giardinaggio", "Musicale", "Fotografia"};

    // Array di prezzi e copie disponibili di prodotti
    std::array<std::string, 30> prezzo_copie_disponibili;
    for (int i = 0; i < 30; ++i)
    {
        prezzo_copie_disponibili[i] = std::to_string(i + 1);
    }

    // Array di date di nascita
    std::array<std::string, 30> date_di_nascita;
    for (int i = 0; i < 30; ++i)
    {
        date_di_nascita[i] = "01/01/2000"; // Esempio di data di nascita fittizia }
    }

        // Array di CAP
        std::array<std::string, 5> CAP = {"12345", "54321", "67890", "09876", "45678"};

        // Array di città di residenza
        std::array<std::string, 5> citta_di_residenza = {"Roma", "Milano", "Napoli", "Firenze", "Venezia"};

        // Array di vie di residenza
        std::array<std::string, 5> vie_di_residenza = {"Via Roma", "Via Milano", "Via Napoli", "Via Firenze", "Via Venezia"};

        // Array di numeri civici
        std::array<std::string, 30> numeri_civici;
        for (int i = 0; i < 30; ++i)
        {
            numeri_civici[i] = std::to_string(i + 1);
        }

        // Array di numeri di carte di pagamento:
        std::array<std::string, 30> carte_pagamento;
        for (int i = 0; i < 30; ++i)
        {
            std::string carta = "";

            // Genera le restanti 10 cifre casuali
            for (int j = 0; j < 10; ++j)
            {
                carta += std::to_string(rand() % 10); // Aggiunge una cifra casuale compresa tra 0 e 9
            }

            carte_pagamento[i] = carta;
        }

        // Array di numeri di carte di pagamento:
        std::array<std::string, 30> cvv_cartePagamento;
        for (int i = 0; i < 30; ++i)
        {
            std::string cvv = "";

            // Genera le restanti 10 cifre casuali
            for (int j = 0; j < 3; ++j)
            {
                cvv += std::to_string(rand() % 3); // Aggiunge una cifra casuale compresa tra 0 e 9
            }

            cvv_cartePagamento[i] = cvv;
        }

        // Array per le motivazioni del reso
        std::array<std::string, 4> motivazioni_reso = {"difettoso", "misura errata", "non conforme alle aspettative", "cambio opinione"};

        // Array per i voti delle stelle
        std::array<std::string, 5> voti_stelle = {"1", "2", "3", "4", "5"};

        std::string idOrdine="";
        std::string codiceProdotto="";
        std::string idCarta = "";
        std::string idRecensione="";

        // #if (DEBUG < 0)

    while (1)
    {
            /* sleep   */
            micro_sleep(10000000); // 10 secondi di attesa

            // Invio richieste al server
            send_counter++;

            // Randomicamente scegliamo l'azione che il client effettuerà:
            int i20 = rand() % 20;
            std::string azioneCompratore = azioniCompratore[i20];

            // Utilizziamo degli indici random per selezionare il parametro della richiesta che effettuerà il client.
            int i5 = rand() % 5;
            int i4 = rand() % 4;
            int i10 = rand() % 10;
            //int i100 = rand() % 100;
            int i30 = rand() % 30;

            sprintf(key1, "Action");
            sprintf(value1, azioneCompratore.c_str());

            if (azioneCompratore == "EFFETTUA REGISTRAZIONE COMPRATORE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "categoriaUtente");
                sprintf(value3, "UtenteCompratore");

                sprintf(key4, "nome");
                sprintf(value4, nomi[i10].c_str());

                sprintf(key5, "cognome");
                sprintf(value5, cognomi[i10].c_str());

                sprintf(key6, "numeroTelefono");
                sprintf(value6, numeri_telefono[i30].c_str());

                sprintf(key7, "email");
                sprintf(value7, email[i30].c_str());

                sprintf(key8, "viaResidenza");
                sprintf(value8, vie_di_residenza[i5].c_str());

                sprintf(key9, "numeroCivico");
                sprintf(value9, numeri_civici[i30].c_str());

                sprintf(key10, "cap");
                sprintf(value10, CAP[i5].c_str());

                sprintf(key11, "cittàResidenza");
                sprintf(value11, citta_di_residenza[i5].c_str());

                sprintf(key12, "password");
                sprintf(value12, password[i30].c_str());

                sprintf(key13, "confermaPassword");
                sprintf(value13, password[i30].c_str());

                sprintf(key14, "dataCompleanno");
                sprintf(value14, date_di_nascita[i30].c_str());

                // Effettuo un comando di scrittura relativo alla registrazione dell'utente compratore
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                                     WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                                     key9, value9, key10, value10, key11, value11, key12, value12, key13, value13, key14, value14);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s, %s: %s, %s: %s, %s: %s, %s: %s, %s: %s, %s: %s, %s: %s, %s: %s, %s: %s, %s: %s, %s: %s)  (id: %s)\n\n",
                       pid, WRITE_STREAM_CUSTOMER,
                       key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8, key9, value9, key10, value10, key11, value11, key12, value12, key13, value13, key14, value14, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << ", " << value3 << ", " << value4 << ", " << value5 << ", " << value6 << ", " << value7 << ", " << value8 << ", " << value9 << ", " << value10 << ", "
                           << value11 << ", " << value12 << ", " << value13 << ", " << value14 << " )\n"
                           << std::endl;

                // Libera la risorsa della risposta
                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);
                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);

                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Scrive nel file

                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "EFFETTUA LOGIN COMPRATORE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "password");
                sprintf(value3, password[i30].c_str());

                // Effettuo un comando di scrittura relativo al login dell'utente compratore
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << ", " << value3 << " )\n"
                           << std::endl;

                // Libera la risorsa della risposta
                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);

                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);
                        
                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "EFFETTUA LOGOUT COMPRATORE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                // Effettuo un comando di scrittura relativo al logout dell'utente compratore.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << " )\n"
                           << std::endl;

                // Libera la risorsa della risposta
                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);
                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);

                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "ELIMINA PROFILO COMPRATORE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                // Effettuo un comando di scrittura relativo all'eliminazione dell'utente compratore.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << " )\n"
                           << std::endl;

                // Libera la risorsa della risposta
                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);

                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);
                        
                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "AGGIORNA NUMERO TELEFONO COMPRATORE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "nuovoNumeroTelefono");
                sprintf(value3, numeri_telefono[i30].c_str());

                // Effettuo un comando di scrittura relativo all'aggiornamento di numero di telefono.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s (%s: %s, %s: %s)  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << ", " << value3 << " )\n"
                           << std::endl;

                // Libera la risorsa della risposta
                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("\nLettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);

                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);
                        
                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }
                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "AGGIORNA PASSWORD COMPRATORE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "vecchiaPassw");
                sprintf(value3, password[i30].c_str());

                sprintf(key4, "nuovaPassw");
                sprintf(value4, password[i30].c_str());

                // Effettuo un comando di scrittura relativo all'aggiornamento della password.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s , %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << ", " << value3 << ", " << value4 << " )\n"
                           << std::endl;

                // Libera la risorsa della risposta
                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);

                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);
                        
                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "AGGIORNA RESIDENZA")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "nuovaViaResidenza");
                sprintf(value3, vie_di_residenza[i5].c_str());

                sprintf(key4, "nuovoNumCiv");
                sprintf(value4, numeri_civici[i30].c_str());

                sprintf(key5, "nuovoCAP");
                sprintf(value5, CAP[i5].c_str());

                sprintf(key6, "nuovaCittaResidenza");
                sprintf(value6, citta_di_residenza[i5].c_str());

                // Effettuo un comando di scrittura relativo all'aggiornamento della residenza.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s, %s: %s, %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << ", " << value3 << ", " << value4 << ", " << value5 << ", " << value6 << " )\n"
                           << std::endl;

                // Libera la risorsa della risposta
                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);

                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);
                        
                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "AGGIUNGI CARTA PAGAMENTO")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "numeroCartaPagamento");
                sprintf(value3, carte_pagamento[i30].c_str());

                sprintf(key4, "cvvCartaPagamento");
                sprintf(value4, cvv_cartePagamento[i30].c_str());

                // Effettuo un comando di scrittura relativo all'aggiungimento della carta di pagamento.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s : %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << ", " << value3 << ", " << value4 << " )\n"
                           << std::endl;

                // Libera la risorsa della risposta
                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);

                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);
                        
                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "RIMUOVI CARTA PAGAMENTO")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "idCarta");
                idCarta = std::to_string(rand() % 30);
                sprintf(value3, idCarta.c_str());

                // Effettuo un comando di scrittura relativo alla rimozione della carta di pagamento.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << ", " << value3 << " )\n"
                           << std::endl;

                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);

                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);
                        
                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "AGGIUNGI PRODOTTO CARRELLO")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "codiceProdotto");
                codiceProdotto = std::to_string(rand() % 30);
                sprintf(value3, codiceProdotto.c_str());

                // Effettuo un comando di scrittura relativo all'aggiungimento del prodtto nel carrello.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s)  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << ", " << value3 << " )\n"
                           << std::endl;

                // Libera la risorsa della risposta
                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);

                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);
                        
                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "RIMUOVI PRODOTTO CARRELLO")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "codiceProdotto");
                codiceProdotto = std::to_string(rand() % 30);
                sprintf(value3, codiceProdotto.c_str());

                // Effettuo un comando di scrittura relativo alla rimozione del prodtto nel carrello.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s)  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << ", " << value3 << " )\n"
                           << std::endl;

                // Libera la risorsa della risposta
                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);

                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);
                        
                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "AGGIUNGI PRODOTTO LISTADESIDERI")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "codiceProdotto");
                codiceProdotto = std::to_string(rand() % 30);
                sprintf(value3, codiceProdotto.c_str());

                // Effettuo un comando di scrittura relativo all'aggiungimento del prodtto nella lista desideri.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s)  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << ", " << value3 << " )\n"
                           << std::endl;

                // Libera la risorsa della risposta
                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);

                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);
                        
                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "RIMUOVI PRODOTTO LISTADESIDERI")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "codiceProdotto");
                codiceProdotto = std::to_string(rand() % 30);
                sprintf(value3, codiceProdotto.c_str());

                // Effettuo un comando di scrittura relativo alla rimozione del prodotto dalla lista desideri.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << ", " << value3 << " )\n"
                           << std::endl;

                // Libera la risorsa della risposta
                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);

                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);
                        
                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "ACQUISTA PRODOTTO")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "codiceProdotto");
                codiceProdotto = std::to_string(rand() % 30);
                sprintf(value3, codiceProdotto.c_str());

                sprintf(key4, "via_spedizione");
                sprintf(value4, vie_di_residenza[i5].c_str());

                sprintf(key5, "città_spedizione");
                sprintf(value5, citta_di_residenza[i5].c_str());

                sprintf(key6, "numero_civico_spedizione");
                sprintf(value6, numeri_civici[i30].c_str());

                sprintf(key7, "CAP_spedizione");
                sprintf(value7, CAP[i5].c_str());

                // Effettuo un comando di scrittura relativo all'acquisto del prodotto.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s, %s: %s, %s: %s, %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << ", " << value3 << ", " << value4 << ", " << value5 << ", " << value6 << ", " << value7 << " )\n";

                // Libera la risorsa della risposta
                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);

                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);
                        
                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "RICERCA PRODOTTO")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "nomeProdotto");
                sprintf(value3, nomi_prodotti[i30].c_str());

                // Effettuo un comando di scrittura relativo alla ricerca del prodotto.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << ", " << value3 << " )\n"
                           << std::endl;

                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);

                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);
                        
                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "VISIONA ORDINI EFFETTUATI")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                // Effettuo un comando di scrittura relativo alla visione degli ordini effettuati.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << " )\n"
                           << std::endl;

                // Libera la risorsa della risposta
                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);

                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);
                        
                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "ANNULLA ORDINE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "idOrdine");
                idOrdine = std::to_string(rand() % 30);
                sprintf(value3, idOrdine.c_str());

                // Effettuo un comando di scrittura relativo all'annullamento dell'ordine.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << ", " << value3 << " )\n"
                           << std::endl;

                // Libera la risorsa della risposta
                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);

                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);
                        
                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "EFFETTUA RESO")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "idOrdine");
                idOrdine = std::to_string(rand() % 30);
                sprintf(value3, idOrdine.c_str());

                sprintf(key4, "motivazioneReso");
                sprintf(value4, motivazioni_reso[i4].c_str());

                // Effettuo un comando di scrittura relativo all'effettuazione del reso.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s, %s: %s, %s: %s)  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << ", " << value3 << ", " << value4 << " )\n"
                           << std::endl;

                // Libera la risorsa della risposta
                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);

                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);
                        
                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "EFFETTUA RECENSIONE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "idOrdine");
                idOrdine = std::to_string(rand() % 30);
                sprintf(value3, idOrdine.c_str());

                sprintf(key4, "descrizioneRecensione");
                sprintf(value4, descrizioneProdotti.c_str());

                sprintf(key5, "voto_stella");
                sprintf(value5, voti_stelle[i5].c_str());

                // Effettuo un comando di scrittura relativo all'effettuazione del reso.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s, %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << ", " << value3 << ", " << value4 << ", " << value5 << " )\n"
                           << std::endl;

                // Libera la risorsa della risposta
                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);

                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);
                        
                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (azioneCompratore == "RIMUOVI RECENSIONE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "idRecensione");
                idRecensione = std::to_string(rand() % 30);
                sprintf(value3, idRecensione.c_str());

                // Effettuo un comando di scrittura relativo alla rimozione del reso.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("\n----------------------------------------------------------------------------------\nRichiesta del client compratore. \n");
                printf("main(): pid =%d: stream %s: Added %s: %s ( %s: %s, %s: %s )  (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Incrementiamo il valore della richiesta del client che verrà scritta nel file di risultato dei test.
                num_richieste_client++;

                // Scrive nel file
                outputFile << "\nRichiesta client numero: " << num_richieste_client << "\n"
                           << value1 << " ( " << value2 << ", " << value3 << " )\n"
                           << std::endl;

                // Libera la risorsa della risposta
                freeReplyObject(reply);

                
                //  Lettura dei risultati dal server
                read_counter++;

                // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER per verificare che la richiesta effettuata dal client sia andata a buon fine oppure no.
                reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

                printf("Lettura dei messaggi sulla Streams corrispondente alle risposte del server. \n");

                printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReply(c2r, reply);

                // Stampa la risposta del comando
                dumpReply(reply, 0);

                // Scorro il numero di Streams nella connessione Redis
                for (k = 0; k < ReadNumStreams(reply); k++)
                {
                    ReadStreamName(reply, streamname, k);

                    // Scorro il numero di messaggi della Stream Redis
                    for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
                    {
                        ReadStreamNumMsgID(reply, k, i, msgid);

                        printf("Numero messaggio: %d\n", i);

                        printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n", pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                        printf("\tElenco dei valori del messaggio numero: %d\n", i);
                        
                        // Scorro il numero di valori del messaggio della Stream Redis
                        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                        {
                            ReadStreamMsgVal(reply, k, i, h, fval);
                            printf("\tmain(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                            if (h == 1)
                            {
                                // Incrementiamo il valore della risposta del server che verrà scritta nel file di risultato dei test.
                                num_risposte_server++;

                                // Scrive nel file corrispondente alla risposta del server
                                outputFile << "Risposta server numero: " << num_risposte_server << "\n"
                                           << fval << "\n"
                                           << std::endl;
                            }
                        }
                    }
                }

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

        micro_sleep(10000000); // 10 secondi di attesa

    } // while ()

    // #endif

    outputFile.close(); // Chiudi il file


    redisFree(c2r);
}
