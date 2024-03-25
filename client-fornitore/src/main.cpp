
#include "main.h"
#include <array>
#include <string>
#include <string.h>
#include <fstream>

// cc -Wall -g -ggdb -o streams streams.c -lhiredis
// Usage: ./streams <add count> <read count> [block time, default: 1]

#define DEBUG 1000 // Definisci una costante per abilitare il debug

#define READ_STREAM_FORNITORE "stream4"  // Nome dello stream da cui leggere.
#define WRITE_STREAM_FORNITORE "stream3" // Nome dello stream su cui scrivere.

using namespace std; // Consente di utilizzare le funzioni e le classi standard del C++ senza doverle qualificare con std::.

int main()
{

    redisContext *c2r;
    redisReply *reply;      // Inizializzazione risposta Redis
    int read_counter = 0;   // Contatore delle letture effettuate
    int send_counter = 0;   // Contatore degli invii effettuati
    int block = 1000000000; // Tempo di blocco per la lettura da stream in nanosecondi
    int pid;                // ID del processo
    // unsigned seed;
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

    char streamname[100]; // Buffer per il nome dello stream Redis
    char msgid[100];      // Buffer per l'ID del messaggio Redis
    char fval[100];       // Buffer per il valore del campo del messaggio Redis
    int i, k, h;          // Variabili di iterazione

    std::string test1[9] = {
        "EFFETTUA REGISTRAZIONE FORNITORE", "EFFETTUA LOGIN FORNITORE", "AGGIORNA NUMERO TELEFONO FORNITORE",
        "AGGIORNA PASSWORD FORNITORE", "AGGIORNA NOME AZIENDAPRODUZIONE", "AGGIUNGI PRODOTTO SITO", //"RIMUOVI PRODOTTO SITO", //"EFFETTUA LOGOUT FORNITORE",  //"ELIMINA PROFILO FORNITORE"
    };

    std::string test2[29] = {
        "EFFETTUA REGISTRAZIONE FORNITORE",
        "EFFETTUA REGISTRAZIONE FORNITORE",
        "EFFETTUA REGISTRAZIONE FORNITORE",
        "EFFETTUA REGISTRAZIONE FORNITORE",

        "EFFETTUA LOGIN FORNITORE",
        "EFFETTUA LOGIN FORNITORE",
        "EFFETTUA LOGIN FORNITORE",
        "EFFETTUA LOGIN FORNITORE",

        "AGGIORNA NOME AZIENDAPRODUZIONE",

        "AGGIUNGI PRODOTTO SITO",
        "AGGIUNGI PRODOTTO SITO",
        "AGGIUNGI PRODOTTO SITO",
        "AGGIUNGI PRODOTTO SITO",
        "AGGIUNGI PRODOTTO SITO",
        "AGGIUNGI PRODOTTO SITO",
        "AGGIUNGI PRODOTTO SITO",
        "AGGIUNGI PRODOTTO SITO",
        "AGGIUNGI PRODOTTO SITO",
        "AGGIUNGI PRODOTTO SITO",
        "AGGIUNGI PRODOTTO SITO",
        "AGGIUNGI PRODOTTO SITO",
        "RIMUOVI PRODOTTO SITO",
        "RIMUOVI PRODOTTO SITO"
        "AGGIORNA NUMERO TELEFONO FORNITORE",
        "AGGIORNA PASSWORD FORNITORE",
        "AGGIORNA NUMERO TELEFONO FORNITORE",
        //"RIMUOVI PRODOTTO SITO",
        "AGGIORNA NOME AZIENDAPRODUZIONE",
        "EFFETTUA LOGOUT FORNITORE",
        "ELIMINA PROFILO FORNITORE"};

    // Array di nomi utente
    std::array<std::string, 30> nomi_utente;
    for (int i = 0; i < 30; ++i)
    {
        nomi_utente[i] = "Utente" + std::to_string(i + 1);
    }

    // Array di nomi aziende produttrici
    std::array<std::string, 5> nomi_aziende_produttrici = {"Azienda1", "Azienda2", "Azienda3", "Azienda4", "Azienda5"};

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
    // std::array<std::string, 10> categorie_prodotti = {"Abbigliamento", "Elettronica", "Informatica", "Arredamento", "Culinario", "Sportivo", "Illuminazione", "Giardinaggio", "Musicale", "Fotografia"};
    std::array<std::string, 2> categorie_prodotti = {"Abbigliamento", "Elettronica"};

    // Array di nomi di prodotti
    std::array<std::string, 30> prezzo_copie_disponibili;
    for (int i = 0; i < 30; ++i)
    {
        prezzo_copie_disponibili[i] = std::to_string(i + 1);
    }

    /*  prg  */

#if (DEBUG > 0)
    setvbuf(stdout, (char *)NULL, _IONBF, 0);
    setvbuf(stderr, (char *)NULL, _IONBF, 0);
#endif

    // Imposta il nome utente.
    strcpy(username, "federico");

    // Ottenimento dell'ID del processo corrente
    pid = getpid();

    // Connessione a Redis
    printf("main(): pid %d: user %s: connecting to redis ...\n", pid, username);
    c2r = redisConnect("localhost", 6379);
    // Stampa un messaggio di connessione riuscita.
    printf("main(): pid %d: user %s: connected to redis\n", pid, username);

    // Eliminazione degli stream se esistono
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

    // Creazione degli stream/gruppi
    initStreams(c2r, READ_STREAM_FORNITORE);
    initStreams(c2r, WRITE_STREAM_FORNITORE);

    printf("Creazione Streams\n");

    /* init random number generator  */
    srand((unsigned)time(NULL));

    // Apre il file corrispondente al test da effettuare in modalità di lettura
    // test_sessioni_gestioneprofilo
    // gestione_prodotti_sito
    std::ifstream file("../test/gestione_prodotti_sito.txt");
    if (!file.is_open())
    {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    // Leggiamo una riga per volta e se si trova una linea corrispondente all'azione da effettuare , prendiamo tutti i parametri necessari nelle righe seguenti e inviamo la richiesta al server. 
    std::string line;
    while (std::getline(file, line))
    { // Legge una riga per volta

        if (line == "EFFETTUA REGISTRAZIONE FORNITORE")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_fornitore");
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
            sprintf(key8, "password");
            sprintf(value8, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key9, "confermaPassword");
            sprintf(value9, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key10, "aziendaProduzione");
            sprintf(value10, line.c_str());

            // Effettuo un comando di scrittura relativo alla registrazione dell'utente fornitore
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                                 WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                                 key9, value9, key10, value10);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s \n",
                   WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                   key9, value9, key10, value10);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE,
                   key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                   key9, value9, key10, value10, reply->str);

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "EFFETTUA LOGIN FORNITORE")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_fornitore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "password");
            sprintf(value3, line.c_str());

            // Effettuo un comando di scrittura relativo al login dell'utente fornitore.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("XADD %s * %s %s %s %s %s %s\n", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s(id: %s)\n", pid, WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, reply->str);

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "EFFETTUA LOGOUT FORNITORE")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_fornitore");
            sprintf(value2, line.c_str());

            // Effettuo un comando di scrittura relativo al logout dell'utente fornitore.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_FORNITORE, key1, value1, key2, value2);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("XADD %s * %s %s %s %s\n", WRITE_STREAM_FORNITORE, key1, value1, key2, value2);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s(id: %s)\n", pid, WRITE_STREAM_FORNITORE, key1, value1, key2, value2, reply->str);

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "ELIMINA PROFILO FORNITORE")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_fornitore");
            sprintf(value2, line.c_str());

            // Effettuo un comando di scrittura relativo all'eliminazione dell'utente fornitore.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_FORNITORE, key1, value1, key2, value2);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("XADD %s * %s %s %s %s\n", WRITE_STREAM_FORNITORE, key1, value1, key2, value2);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s(id: %s)\n", pid, WRITE_STREAM_FORNITORE, key1, value1, key2, value2, reply->str);

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "AGGIORNA NOME AZIENDAPRODUZIONE")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_fornitore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "nuovaAziendaProduzione");
            sprintf(value3, line.c_str());

            // Effettuo un comando di scrittura relativo all'effettuazione del login dell'utente fornitore.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, reply->str);

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "AGGIORNA NUMERO TELEFONO FORNITORE")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_fornitore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "nuovoNumeroTelefono");
            sprintf(value3, line.c_str());

            // Effettuo un comando di scrittura relativo all'effettuazione del login dell'utente fornitore.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, reply->str);

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "AGGIORNA PASSWORD FORNITORE")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_fornitore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "vecchiaPassw");
            sprintf(value3, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key4, "nuovaPassw");
            sprintf(value4, line.c_str());

            // Effettuo un comando di scrittura relativo all'aggiornamento di password dell'utente fornitore.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, key4, value4);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("XADD %s * %s %s %s %s %s %s %s %s \n", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, key4, value4);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "AGGIUNGI PRODOTTO SITO")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_fornitore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "nomeProdotto");
            sprintf(value3, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key4, "categoriaProdotto");
            sprintf(value4, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key5, "prezzoProdotto");
            sprintf(value5, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key6, "descrizioneProdotto");
            sprintf(value6, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key7, "aziendaProduzione");
            sprintf(value7, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key8, "numeroCopieDisponibili");
            sprintf(value8, line.c_str());

            // Effettuo un comando di scrittura relativo all'aggiunta di un prodotto da parte dell'utente fornitore.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                                 WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s \n",
                   WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE,
                   key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8, reply->str);

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "RIMUOVI PRODOTTO SITO")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "nome_utente_fornitore");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "codiceProdotto");
            sprintf(value3, line.c_str());

            // Effettuo un comando di scrittura relativo alla rimozione del prodotto dell'utente fornitore.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, reply->str);

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

    } // while scorrimento linee file

    file.close(); // Chiude il file

    printf("\n\nOra di leggere i risultati dal server. \n");

    //  Lettura dei risultati dal server
    read_counter++;

    // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER.
    reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_FORNITORE);

    printf("Effettuato comando per leggere i messaggi della Streams. \n");

    printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_FORNITORE);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReply(c2r, reply);

    // Stampa la risposta del comando
    dumpReply(reply, 0);

    // Scorro il numero di Streams nella connessione Redis
    for (k = 0; k < ReadNumStreams(reply); k++)
    {
        ReadStreamName(reply, streamname, k);

        printf("Numero stream: %d\n", k);

        // Scorro il numero di messaggi della Streams Redis
        for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
        {
            ReadStreamNumMsgID(reply, k, i, msgid);

            printf("Numero messaggio: %d della Stream %d\n", i, k);

            printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n",
                   pid, username, streamname,
                   k, i, msgid,
                   ReadStreamMsgNumVal(reply, k, i));

            // Scorro il numero di valori del messaggio della Streams Redis
            for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
            {
                ReadStreamMsgVal(reply, k, i, h, fval);
                printf("main(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n",
                       pid, username, k, i, msgid, h, fval);
            }
        }
    }

    // Libera la risorsa della risposta
    freeReplyObject(reply);

#if (DEBUG < 0)

    // Test randomici
    while (1)
    {
        // send arguments to server
        send_counter++;

        // Itero sulle azioni che può effettuare il client
        for (i = 0; i < 13; i++)
        {

            // Definisco 3 indici casuali
            int i5 = rand() % 5;
            int i10 = rand() % 10;
            int i100 = rand() % 100;
            int i30 = rand() % 30;
            int i2 = rand() % 2;

            printf("\n\nValore della variabile i: %d \n", i);
            printf("Valore di i5: %d \n", i5);
            printf("Valore di i10: %d  \n", i10);
            printf("Valore di i100: %d \n\n", i100);

            sprintf(key1, "Action");
            sprintf(value1, test2[i].c_str());

            if (test2[i] == "EFFETTUA REGISTRAZIONE FORNITORE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_fornitore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "categoriaUtente");
                sprintf(value3, "UtenteFornitore");

                sprintf(key4, "nome");
                sprintf(value4, nomi[i10].c_str());

                sprintf(key5, "cognome");
                sprintf(value5, cognomi[i10].c_str());

                sprintf(key6, "numeroTelefono");
                sprintf(value6, numeri_telefono[i30].c_str());

                sprintf(key7, "email");
                sprintf(value7, email[i30].c_str());

                sprintf(key8, "password");
                sprintf(value8, password[i30].c_str());

                sprintf(key9, "confermaPassword");
                sprintf(value9, password[i30].c_str());

                sprintf(key10, "aziendaProduzione");
                sprintf(value10, nomi_aziende_produttrici[i5].c_str());

                // Effettuo un comando di scrittura relativo alla registrazione dell'utente fornitore
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                                     WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                                     key9, value9, key10, value10);
                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s \n",
                       WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                       key9, value9, key10, value10);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE,
                       key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                       key9, value9, key10, value10, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "AGGIORNA NOME AZIENDAPRODUZIONE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_fornitore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "nuovaAziendaProduzione");
                sprintf(value3, nomi_aziende_produttrici[i5].c_str());

                // Effettuo un comando di scrittura relativo all'aggiornamento dell'azienda di produzione.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "AGGIUNGI PRODOTTO SITO")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_fornitore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "nomeProdotto");
                sprintf(value3, nomi_prodotti[i30].c_str());

                sprintf(key4, "categoriaProdotto");
                sprintf(value4, categorie_prodotti[i2].c_str());

                sprintf(key5, "prezzoProdotto");
                sprintf(value5, prezzo_copie_disponibili[i30].c_str());

                sprintf(key6, "descrizioneProdotto");
                sprintf(value6, descrizioneProdotti.c_str());

                sprintf(key7, "aziendaProduzione");
                sprintf(value7, nomi_aziende_produttrici[i5].c_str());

                sprintf(key8, "numeroCopieDisponibili");
                sprintf(value8, prezzo_copie_disponibili[i30].c_str());

                // Effettuo un comando di scrittura relativo all'aggiunta di un prodotto da parte dell'utente fornitore.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                                     WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s \n",
                       WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE,
                       key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "RIMUOVI PRODOTTO SITO")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_fornitore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "codiceProdotto");
                std::string codiceProdotto = std::to_string(rand() % 30);
                sprintf(value3, codiceProdotto.c_str());

                // Effettuo un comando di scrittura relativo alla rimozione del prodotto dell'utente fornitore.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "EFFETTUA LOGIN FORNITORE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_fornitore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "password");
                sprintf(value3, password[i30].c_str());

                // Effettuo un comando di scrittura relativo all'effettuazione del login dell'utente fornitore.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "EFFETTUA LOGOUT FORNITORE")
            {

                sprintf(key2, "nome_utente_fornitore");
                sprintf(value2, nomi_utente[i30].c_str());

                // Effettuo un comando di scrittura relativo al logout dell'utente fornitore.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_FORNITORE, key1, value1, key2, value2);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s\n", WRITE_STREAM_FORNITORE, key1, value1, key2, value2);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s(id: %s)\n", pid, WRITE_STREAM_FORNITORE, key1, value1, key2, value2, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "ELIMINA PROFILO FORNITORE")
            {

                sprintf(key2, "nome_utente_fornitore");
                sprintf(value2, nomi_utente[i30].c_str());

                // Effettuo un comando di scrittura relativo all'eliminazione dell'utente fornitore.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_FORNITORE, key1, value1, key2, value2);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s\n", WRITE_STREAM_FORNITORE, key1, value1, key2, value2);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s(id: %s)\n", pid, WRITE_STREAM_FORNITORE, key1, value1, key2, value2, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "AGGIORNA NUMERO TELEFONO FORNITORE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_fornitore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "nuovoNumeroTelefono");
                sprintf(value3, numeri_telefono[i100].c_str());

                // Effettuo un comando di scrittura relativo all'aggiornamento di numero telefono dell'utente fornitore.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "AGGIORNA PASSWORD FORNITORE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_fornitore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "vecchiaPassw");
                sprintf(value3, password[i30].c_str());

                sprintf(key4, "nuovaPassw");
                sprintf(value4, password[i30].c_str());

                // Effettuo un comando di scrittura relativo all'aggiornamento di password dell'utente fornitore.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, key4, value4);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s %s %s \n", WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, key4, value4);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_FORNITORE, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }
        }

        printf("\n\nOra di leggere i risultati dal server. \n");

        //  Lettura dei risultati dal server
        read_counter++;

        // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER.
        reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_FORNITORE);

        printf("Effettuato comando per leggere i messaggi della Streams. \n");

        printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_FORNITORE);

        // Verifica la risposta del comando e termina il programma in caso di errore
        assertReply(c2r, reply);

        // Stampa la risposta del comando
        dumpReply(reply, 0);

        // Scorro il numero di Streams nella connessione Redis
        for (k = 0; k < ReadNumStreams(reply); k++)
        {
            ReadStreamName(reply, streamname, k);

            printf("Numero stream: %d\n", k);

            // Scorro il numero di messaggi della Streams Redis
            for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
            {
                ReadStreamNumMsgID(reply, k, i, msgid);

                printf("Numero messaggio: %d della Stream %d\n", i, k);

                printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n",
                       pid, username, streamname,
                       k, i, msgid,
                       ReadStreamMsgNumVal(reply, k, i));

                // Scorro il numero di valori del messaggio della Streams Redis
                for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
                {
                    ReadStreamMsgVal(reply, k, i, h, fval);
                    printf("main(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n",
                           pid, username, k, i, msgid, h, fval);
                }
            }
        }

        // Libera la risorsa della risposta
        freeReplyObject(reply);

        /* sleep   */
        micro_sleep(10000000); // 10 secondi di attesa
    }                          // while ()
#endif


    redisFree(c2r);
}
