
#include "main.h"
#include <array>
#include <string>
#include <string.h>
#include <fstream>

// cc -Wall -g -ggdb -o streams streams.c -lhiredis
// Usage: ./streams <add count> <read count> [block time, default: 1]

#define DEBUG 1000 // Definisci una costante per abilitare il debug

#define READ_STREAM_CUSTOMER "stream2"  // Nome dello stream da cui leggere.
#define WRITE_STREAM_CUSTOMER "stream1" // Nome dello stream su cui scrivere.

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
    char key11[100];
    char value11[100];
    char key12[100];
    char value12[100];
    char key13[100];
    char value13[100];
    char key14[100];
    char value14[100];

    char streamname[100]; // Buffer per il nome dello stream Redis
    char msgid[100];      // Buffer per l'ID del messaggio Redis
    char fval[100];       // Buffer per il valore del campo del messaggio Redis
    int i, k, h;          // Variabili di iterazione

    std::string test1[20] = {

        "EFFETTUA REGISTRAZIONE COMPRATORE",
        "EFFETTUA LOGIN COMPRATORE",

        "AGGIORNA NUMERO TELEFONO COMPRATORE",
        "AGGIORNA PASSWORD COMPRATORE",
        "AGGIORNA RESIDENZA",

        "AGGIUNGI CARTA PAGAMENTO",

        "AGGIUNGI PRODOTTO CARRELLO",
        "AGGIUNGI PRODOTTO LISTADESIDERI",

        "RICERCA PRODOTTO",
        "ACQUISTA PRODOTTO",
        "VISIONA ORDINI EFFETTUATI",

        "EFFETTUA RECENSIONE",

        "ANNULLA ORDINE",

        "EFFETTUA RESO",

        "RIMUOVI RECENSIONE",
        "RIMUOVI CARTA PAGAMENTO",
        "RIMUOVI PRODOTTO CARRELLO",
        "RIMUOVI PRODOTTO LISTADESIDERI",

        "EFFETTUA LOGOUT COMPRATORE",
        "ELIMINA PROFILO COMPRATORE"};

    std::string test2[39] = {
        "EFFETTUA REGISTRAZIONE COMPRATORE",
        "EFFETTUA REGISTRAZIONE COMPRATORE",
        "EFFETTUA REGISTRAZIONE COMPRATORE",
        "EFFETTUA REGISTRAZIONE COMPRATORE",
        "EFFETTUA REGISTRAZIONE COMPRATORE",

        "EFFETTUA LOGIN COMPRATORE",
        "EFFETTUA LOGIN COMPRATORE",
        "EFFETTUA LOGIN COMPRATORE",
        "EFFETTUA LOGIN COMPRATORE",
        "EFFETTUA LOGIN COMPRATORE",

        "AGGIORNA NUMERO TELEFONO COMPRATORE",
        "AGGIORNA PASSWORD COMPRATORE",
        "AGGIORNA RESIDENZA",
        "AGGIORNA NUMERO TELEFONO COMPRATORE",
        "AGGIORNA PASSWORD COMPRATORE",

        "ACQUISTA PRODOTTO",
        "ACQUISTA PRODOTTO",
        "ACQUISTA PRODOTTO",
        "ACQUISTA PRODOTTO",
        "ACQUISTA PRODOTTO",
        "ACQUISTA PRODOTTO",
        "ACQUISTA PRODOTTO",
        "ACQUISTA PRODOTTO",
        "AGGIUNGI PRODOTTO CARRELLO",
        "AGGIUNGI PRODOTTO CARRELLO",
        "AGGIUNGI PRODOTTO CARRELLO",
        "AGGIUNGI PRODOTTO CARRELLO",

        "AGGIUNGI PRODOTTO LISTADESIDERI",
        "AGGIUNGI PRODOTTO LISTADESIDERI",
        "AGGIUNGI PRODOTTO LISTADESIDERI",
        "AGGIUNGI PRODOTTO LISTADESIDERI",
        "RIMUOVI PRODOTTO CARRELLO",

        "RICERCA PRODOTTO",
        "ACQUISTA PRODOTTO",

        "EFFETTUA RESO",
        "EFFETTUA RECENSIONE",
        "EFFETTUA LOGOUT COMPRATORE",
        "ELIMINA PROFILO COMPRATORE"};

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
        date_di_nascita[i] = "01/01/2000"; // Esempio di data di nascita fittizia
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

    // Apre il file in modalità di lettura
    std::ifstream file("../test/test.txt");
    if (!file.is_open())
    {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

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

            printf("XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s \n",
                   WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                   key9, value9, key10, value10, key11, value11, key12, value12, key13, value13, key14, value14);

            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n\n", pid, WRITE_STREAM_CUSTOMER,
                   key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                   key9, value9, key10, value10, key11, value11, key12, value12, key13, value13, key14, value14,
                   reply->str);

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

            // Effettuo un comando di scrittura relativo al logout dell'utente compratore.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("XADD %s * %s %s %s %s\n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s(id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

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

            printf("XADD %s * %s %s %s %s\n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s(id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

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

            printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

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

            printf("XADD %s * %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

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

            printf("XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, reply->str);

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

            printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

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

            printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

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

            printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

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

            printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

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

            printf("XADD %s * %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

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
            sprintf(key3, "numeroCartaPagamento");
            sprintf(value3, line.c_str());

            // Effettuo un comando di scrittura relativo alla rimozione della carta di pagamento.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

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
            sprintf(key3, "codiceProdotto");
            sprintf(value3, line.c_str());

            // Effettuo un comando di scrittura relativo alla rimozione della carta di pagamento.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

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

            printf("XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, reply->str);

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

            printf("XADD %s * %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

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

            printf("XADD %s * %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

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

            printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

        if (line == "EFFETTUA RESO")
        {
            sprintf(key1, "Action");
            sprintf(value1, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key2, "idOrdine");
            sprintf(value2, line.c_str());

            std::getline(file, line); // Passa alla riga successiva
            sprintf(key3, "codiceProdotto");
            sprintf(value3, line.c_str());

            // Effettuo un comando di scrittura relativo all'effettuazione del reso.
            reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

            // Verifica la risposta del comando e termina il programma in caso di errore
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);

            printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
            printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

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

            printf("XADD %s *  %s %s %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5);
            printf("main(): pid =%d: stream %s: Added  %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, reply->str);

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

            printf("XADD %s *  %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
            printf("main(): pid =%d: stream %s: Added  %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

            // Libera la risorsa della risposta
            freeReplyObject(reply);
        }

    } // while scorrimento linee file

    file.close(); // Chiude il file

    printf("\n\nOra di leggere i risultati dal server. \n");

    //  Lettura dei risultati dal server
    read_counter++;

    // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER.
    reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

    printf("Effettuato comando per leggere i messaggi della Streams. \n");

    printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

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
    // Test randomici.
    while (1)
    {
        /* sleep   */
        micro_sleep(10000000); // 10 secondi di attesa

        printf("Attesa\n");

        // send arguments to server
        send_counter++;

        // Itero sulle azioni che effettua il client.
        for (i = 0; i < 39; i++)
        {

            printf("\nValore della variabile i: %d \n", i);

            // Definisco 4 indici casuali per i parametri che passa il client
            int i5 = rand() % 5;
            int i10 = rand() % 10;
            int i100 = rand() % 100;
            int i30 = rand() % 30;

            sprintf(key1, "Action");
            sprintf(value1, test2[i].c_str());

            if (test2[i] == "EFFETTUA REGISTRAZIONE COMPRATORE")
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

                printf("XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s \n",
                       WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                       key9, value9, key10, value10, key11, value11, key12, value12, key13, value13, key14, value14);

                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n\n", pid, WRITE_STREAM_CUSTOMER,
                       key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                       key9, value9, key10, value10, key11, value11, key12, value12, key13, value13, key14, value14,
                       reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "EFFETTUA LOGIN COMPRATORE")
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

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "EFFETTUA LOGOUT COMPRATORE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                // Effettuo un comando di scrittura relativo al logout dell'utente compratore.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s\n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s(id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "ELIMINA PROFILO COMPRATORE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                // Effettuo un comando di scrittura relativo all'eliminazione dell'utente compratore.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "AGGIORNA NUMERO TELEFONO COMPRATORE")
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

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "AGGIORNA PASSWORD COMPRATORE")
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

                printf("XADD %s * %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "AGGIORNA RESIDENZA")
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

                printf("XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "AGGIUNGI CARTA PAGAMENTO")
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

                printf("XADD %s * %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "RIMUOVI CARTA PAGAMENTO")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "numeroCartaPagamento");
                sprintf(value3, carte_pagamento[i30].c_str());

                // Effettuo un comando di scrittura relativo alla rimozione della carta di pagamento.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }

            if (test2[i] == "AGGIUNGI PRODOTTO CARRELLO")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "codiceProdotto");
                std::string codiceProdotto = std::to_string(rand() % 30);
                sprintf(value3, codiceProdotto.c_str());

                // Effettuo un comando di scrittura relativo all'aggiungimento del prodtto nel carrello.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "RIMUOVI PRODOTTO CARRELLO")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "codiceProdotto");
                std::string codiceProdotto = std::to_string(rand() % 30);
                sprintf(value3, codiceProdotto.c_str());

                // Effettuo un comando di scrittura relativo alla rimozione del prodtto nel carrello.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "AGGIUNGI PRODOTTO LISTADESIDERI")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "codiceProdotto");
                std::string codiceProdotto = std::to_string(rand() % 30);
                sprintf(value3, codiceProdotto.c_str());

                // Effettuo un comando di scrittura relativo all'aggiungimento del prodtto nella lista desideri.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "RIMUOVI PRODOTTO LISTADESIDERI")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "codiceProdotto");
                std::string codiceProdotto = std::to_string(rand() % 30);
                sprintf(value3, codiceProdotto.c_str());

                // Effettuo un comando di scrittura relativo alla rimozione del prodotto dalla lista desideri.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "ACQUISTA PRODOTTO")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "codiceProdotto");
                std::string codiceProdotto = std::to_string(rand() % 30);
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

                printf("XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "RICERCA PRODOTTO")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "codiceProdotto");
                std::string codiceProdotto = std::to_string(rand() % 30);
                sprintf(value3, codiceProdotto.c_str());

                // Effettuo un comando di scrittura relativo alla ricerca del prodotto.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "VISIONA ORDINI EFFETTUATI")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                // Effettuo un comando di scrittura relativo alla visione degli ordini effettuati.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "ANNULLA ORDINE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "idOrdine");
                // Dovrei effettuare una selezione del numero ordine compreso tra quelli presenti.
                sprintf(value3, "1");

                // Effettuo un comando di scrittura relativo all'annullamento dell'ordine.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "EFFETTUA RESO")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "idOrdine");
                sprintf(value2, "1");

                // Modificare motivazioneReso
                sprintf(key3, "codiceProdotto");
                sprintf(value3, "1");

                // Effettuo un comando di scrittura relativo all'effettuazione del reso.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "EFFETTUA RECENSIONE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "idOrdine");
                sprintf(value3, "1");

                sprintf(key4, "descrizioneRecensione");
                sprintf(value4, descrizioneProdotti.c_str());

                sprintf(key5, "voto_stella");
                sprintf(value5, "Cinque");

                // Effettuo un comando di scrittura relativo all'effettuazione del reso.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s *  %s %s %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5);
                printf("main(): pid =%d: stream %s: Added  %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }

            if (test2[i] == "RIMUOVI RECENSIONE")
            {
                // Impostazioni chiavi e valori per il comando Redis:

                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i30].c_str());

                sprintf(key3, "idRecensione");
                sprintf(value3, "1");

                // Effettuo un comando di scrittura relativo alla rimozione del reso.
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

                // Verifica la risposta del comando e termina il programma in caso di errore
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s *  %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added  %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                // Libera la risorsa della risposta
                freeReplyObject(reply);
            }
        }

        printf("\n\nOra di leggere i risultati dal server. \n");

        //  Lettura dei risultati dal server
        read_counter++;

        // Effettuo un comando di lettura dei messaggi sulla Stream di lettura READ_STREAM_CUSTOMER.
        reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

        printf("Effettuato comando per leggere i messaggi della Streams. \n");

        printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

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

    } // while ()
#endif
    redisFree(c2r);
}
