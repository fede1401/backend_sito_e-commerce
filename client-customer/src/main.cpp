
#include "main.h"
#include <array>
#include <string>
#include <string.h>
#include <fstream>

// cc -Wall -g -ggdb -o streams streams.c -lhiredis
// Usage: ./streams <add count> <read count> [block time, default: 1]

#define DEBUG 1000                              // Definisci una costante per abilitare il debug

#define READ_STREAM_CUSTOMER "stream2"          // Nome dello stream da cui leggere.
#define WRITE_STREAM_CUSTOMER "stream1"         // Nome dello stream su cui scrivere.

using namespace std;        // Consente di utilizzare le funzioni e le classi standard del C++ senza doverle qualificare con std::.


int main()
{

    redisContext *c2r;
    redisReply *reply;                  // Inizializzazione risposta Redis
    int read_counter = 0;               // Contatore delle letture effettuate
    int send_counter = 0;               // Contatore degli invii effettuati
    int block = 1000000000;             // Tempo di blocco per la lettura da stream in nanosecondi
    int pid;                            // ID del processo
    //unsigned seed;
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

    
    char streamname[100];               // Buffer per il nome dello stream Redis
    char msgid[100];                    // Buffer per l'ID del messaggio Redis
    char fval[100];                     // Buffer per il valore del campo del messaggio Redis
    int i, k, h;                        // Variabili di iterazione


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
                                    "ELIMINA PROFILO COMPRATORE"
        };


        std::string test2[39] ={
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
                            "ELIMINA PROFILO COMPRATORE"
        };


    // Array di nomi utente
    std::array<std::string, 30> nomi_utente;
    for (int i = 0; i < 30; ++i) {
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
    for (int i = 0; i < 30; ++i) {
        std::string carta = ""; 
        
        // Genera le restanti 10 cifre casuali
        for (int j = 0; j < 10; ++j) {
            carta += std::to_string(rand() % 10); // Aggiunge una cifra casuale compresa tra 0 e 9
        }
        
        carte_pagamento[i] = carta;
    }

    // Array di numeri di carte di pagamento:
    std::array<std::string, 30> cvv_cartePagamento;
    for (int i = 0; i < 30; ++i) {
        std::string cvv = ""; 
        
        // Genera le restanti 10 cifre casuali
        for (int j = 0; j < 3; ++j) {
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


    // Esegui test registrazioneUtenteCompratoreSenzaErrori

    // Apre il file in modalità di lettura
    std::ifstream file("../test/registrazioneUtenteCompratoreSenzaErrori.txt"); 
    if (!file.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    std::string line;
    std::getline(file, line);

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
    }


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

    file.close(); // Chiude il file

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Esegui test registrazioneUtenteCompratoreErroreEmail

    // Apre il file in modalità di lettura
    std::ifstream file2("../test/registrazioneUtenteCompratoreErroreEmail.txt"); 
    if (!file2.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file2, line);

    if (line == "EFFETTUA REGISTRAZIONE COMPRATORE")
    {
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file2, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file2, line); // Passa alla riga successiva
        sprintf(key3, "categoriaUtente");
        sprintf(value3, line.c_str());

        std::getline(file2, line); // Passa alla riga successiva
        sprintf(key4, "nome");
        sprintf(value4, line.c_str());

        std::getline(file2, line); // Passa alla riga successiva
        sprintf(key5, "cognome");
        sprintf(value5, line.c_str());

        std::getline(file2, line); // Passa alla riga successiva
        sprintf(key6, "numeroTelefono");
        sprintf(value6, line.c_str());

        std::getline(file2, line); // Passa alla riga successiva
        sprintf(key7, "email");
        sprintf(value7, line.c_str());

        std::getline(file2, line); // Passa alla riga successiva
        sprintf(key8, "viaResidenza");
        sprintf(value8, line.c_str());

        std::getline(file2, line); // Passa alla riga successiva
        sprintf(key9, "numeroCivico");
        sprintf(value9, line.c_str());

        std::getline(file2, line); // Passa alla riga successiva
        sprintf(key10, "cap");
        sprintf(value10, line.c_str());

        std::getline(file2, line); // Passa alla riga successiva
        sprintf(key11, "cittàResidenza");
        sprintf(value11, line.c_str());


        std::getline(file2, line); // Passa alla riga successiva
        sprintf(key12, "password");
        sprintf(value12, line.c_str());


        std::getline(file2, line); // Passa alla riga successiva
        sprintf(key13, "confermaPassword");
        sprintf(value13, line.c_str());


        std::getline(file2, line); // Passa alla riga successiva
        sprintf(key14, "dataCompleanno");
        sprintf(value14, line.c_str());
    }

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
    
    file2.close(); // Chiude il file


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // Esegui test registrazioneUtenteCompratoreErrorePasswordCorta

    // Apre il file in modalità di lettura
    std::ifstream file3("../test/registrazioneUtenteCompratoreErrorePasswordCorta.txt"); 
    if (!file3.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file3, line);

    if (line == "EFFETTUA REGISTRAZIONE COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file3, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file3, line); // Passa alla riga successiva
        sprintf(key3, "categoriaUtente");
        sprintf(value3, line.c_str());

        std::getline(file3, line); // Passa alla riga successiva
        sprintf(key4, "nome");
        sprintf(value4, line.c_str());

        std::getline(file3, line); // Passa alla riga successiva
        sprintf(key5, "cognome");
        sprintf(value5, line.c_str());

        std::getline(file3, line); // Passa alla riga successiva
        sprintf(key6, "numeroTelefono");
        sprintf(value6, line.c_str());

        std::getline(file3, line); // Passa alla riga successiva
        sprintf(key7, "email");
        sprintf(value7, line.c_str());

        std::getline(file3, line); // Passa alla riga successiva
        sprintf(key8, "viaResidenza");
        sprintf(value8, line.c_str());

        std::getline(file3, line); // Passa alla riga successiva
        sprintf(key9, "numeroCivico");
        sprintf(value9, line.c_str());

        std::getline(file3, line); // Passa alla riga successiva
        sprintf(key10, "cap");
        sprintf(value10, line.c_str());

        std::getline(file3, line); // Passa alla riga successiva
        sprintf(key11, "cittàResidenza");
        sprintf(value11, line.c_str());

        std::getline(file3, line); // Passa alla riga successiva
        sprintf(key12, "password");
        sprintf(value12, line.c_str());

        std::getline(file3, line); // Passa alla riga successiva
        sprintf(key13, "confermaPassword");
        sprintf(value13, line.c_str());

        std::getline(file3, line); // Passa alla riga successiva
        sprintf(key14, "dataCompleanno");
        sprintf(value14, line.c_str());
    }

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
    
    file3.close(); // Chiude il file


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////7


    // Esegui test registrazioneUtenteCompratoreErrorePasswordSenzaCarSpec

    // Apre il file in modalità di lettura
    std::ifstream file4("../test/registrazioneUtenteCompratoreErrorePasswordSenzaCarSpec.txt"); 
    if (!file4.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file4, line);

    if (line == "EFFETTUA REGISTRAZIONE COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file4, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file4, line); // Passa alla riga successiva
        sprintf(key3, "categoriaUtente");
        sprintf(value3, line.c_str());

        std::getline(file4, line); // Passa alla riga successiva
        sprintf(key4, "nome");
        sprintf(value4, line.c_str());

        std::getline(file4, line); // Passa alla riga successiva
        sprintf(key5, "cognome");
        sprintf(value5, line.c_str());

        std::getline(file4, line); // Passa alla riga successiva
        sprintf(key6, "numeroTelefono");
        sprintf(value6, line.c_str());

        std::getline(file4, line); // Passa alla riga successiva
        sprintf(key7, "email");
        sprintf(value7, line.c_str());

        std::getline(file4, line); // Passa alla riga successiva
        sprintf(key8, "viaResidenza");
        sprintf(value8, line.c_str());

        std::getline(file4, line); // Passa alla riga successiva
        sprintf(key9, "numeroCivico");
        sprintf(value9, line.c_str());

        std::getline(file4, line); // Passa alla riga successiva
        sprintf(key10, "cap");
        sprintf(value10, line.c_str());

        std::getline(file4, line); // Passa alla riga successiva
        sprintf(key11, "cittàResidenza");
        sprintf(value11, line.c_str());

        std::getline(file4, line); // Passa alla riga successiva
        sprintf(key12, "password");
        sprintf(value12, line.c_str());

        std::getline(file4, line); // Passa alla riga successiva
        sprintf(key13, "confermaPassword");
        sprintf(value13, line.c_str());

        std::getline(file4, line); // Passa alla riga successiva
        sprintf(key14, "dataCompleanno");
        sprintf(value14, line.c_str());
    }

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
    
    file4.close(); // Chiude il file



    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Esegui test registrazioneUtenteCompratoreErrorePasswordSenzaNumero

    // Apre il file in modalità di lettura
    std::ifstream file5("../test/registrazioneUtenteCompratoreErrorePasswordSenzaNumero.txt"); 
    if (!file5.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file5, line);

    if (line == "EFFETTUA REGISTRAZIONE COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file5, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file5, line); // Passa alla riga successiva
        sprintf(key3, "categoriaUtente");
        sprintf(value3, line.c_str());

        std::getline(file5, line); // Passa alla riga successiva
        sprintf(key4, "nome");
        sprintf(value4, line.c_str());

        std::getline(file5, line); // Passa alla riga successiva
        sprintf(key5, "cognome");
        sprintf(value5, line.c_str());

        std::getline(file5, line); // Passa alla riga successiva
        sprintf(key6, "numeroTelefono");
        sprintf(value6, line.c_str());

        std::getline(file5, line); // Passa alla riga successiva
        sprintf(key7, "email");
        sprintf(value7, line.c_str());

        std::getline(file5, line); // Passa alla riga successiva
        sprintf(key8, "viaResidenza");
        sprintf(value8, line.c_str());

        std::getline(file5, line); // Passa alla riga successiva
        sprintf(key9, "numeroCivico");
        sprintf(value9, line.c_str());

        std::getline(file5, line); // Passa alla riga successiva
        sprintf(key10, "cap");
        sprintf(value10, line.c_str());

        std::getline(file5, line); // Passa alla riga successiva
        sprintf(key11, "cittàResidenza");
        sprintf(value11, line.c_str());

        std::getline(file5, line); // Passa alla riga successiva
        sprintf(key12, "password");
        sprintf(value12, line.c_str());

        std::getline(file5, line); // Passa alla riga successiva
        sprintf(key13, "confermaPassword");
        sprintf(value13, line.c_str());

        std::getline(file5, line); // Passa alla riga successiva
        sprintf(key14, "dataCompleanno");
        sprintf(value14, line.c_str());
    }


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
    
    file5.close(); // Chiude il file


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Esegui test registrazioneUtenteCompratoreErrorePassworddiversaConfermaPassowrd

    // Apre il file in modalità di lettura
    std::ifstream file6("../test/registrazioneUtenteCompratoreErrorePassworddiversaConfermaPassowrd.txt"); 
    if (!file6.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file6, line);

    if (line == "EFFETTUA REGISTRAZIONE COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file6, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file6, line); // Passa alla riga successiva
        sprintf(key3, "categoriaUtente");
        sprintf(value3, line.c_str());

        std::getline(file6, line); // Passa alla riga successiva
        sprintf(key4, "nome");
        sprintf(value4, line.c_str());

        std::getline(file6, line); // Passa alla riga successiva
        sprintf(key5, "cognome");
        sprintf(value5, line.c_str());

        std::getline(file6, line); // Passa alla riga successiva
        sprintf(key6, "numeroTelefono");
        sprintf(value6, line.c_str());

        std::getline(file6, line); // Passa alla riga successiva
        sprintf(key7, "email");
        sprintf(value7, line.c_str());

        std::getline(file6, line); // Passa alla riga successiva
        sprintf(key8, "viaResidenza");
        sprintf(value8, line.c_str());

        std::getline(file6, line); // Passa alla riga successiva
        sprintf(key9, "numeroCivico");
        sprintf(value9, line.c_str());

        std::getline(file6, line); // Passa alla riga successiva
        sprintf(key10, "cap");
        sprintf(value10, line.c_str());

        std::getline(file6, line); // Passa alla riga successiva
        sprintf(key11, "cittàResidenza");
        sprintf(value11, line.c_str());

        std::getline(file6, line); // Passa alla riga successiva
        sprintf(key12, "password");
        sprintf(value12, line.c_str());

        std::getline(file6, line); // Passa alla riga successiva
        sprintf(key13, "confermaPassword");
        sprintf(value13, line.c_str());

        std::getline(file6, line); // Passa alla riga successiva
        sprintf(key14, "dataCompleanno");
        sprintf(value14, line.c_str());
    }

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
    
    file6.close(); // Chiude il file



    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Esegui test registrazioneUtenteCompratoreNomeUtenteEsistente

    // Apre il file in modalità di lettura
    std::ifstream file7("../test/registrazioneUtenteCompratoreNomeUtenteEsistente.txt"); 
    if (!file7.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file7, line);

    if (line == "EFFETTUA REGISTRAZIONE COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file7, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file7, line); // Passa alla riga successiva
        sprintf(key3, "categoriaUtente");
        sprintf(value3, line.c_str());

        std::getline(file7, line); // Passa alla riga successiva
        sprintf(key4, "nome");
        sprintf(value4, line.c_str());

        std::getline(file7, line); // Passa alla riga successiva
        sprintf(key5, "cognome");
        sprintf(value5, line.c_str());

        std::getline(file7, line); // Passa alla riga successiva
        sprintf(key6, "numeroTelefono");
        sprintf(value6, line.c_str());

        std::getline(file7, line); // Passa alla riga successiva
        sprintf(key7, "email");
        sprintf(value7, line.c_str());

        std::getline(file7, line); // Passa alla riga successiva
        sprintf(key8, "viaResidenza");
        sprintf(value8, line.c_str());

        std::getline(file7, line); // Passa alla riga successiva
        sprintf(key9, "numeroCivico");
        sprintf(value9, line.c_str());

        std::getline(file7, line); // Passa alla riga successiva
        sprintf(key10, "cap");
        sprintf(value10, line.c_str());

        std::getline(file7, line); // Passa alla riga successiva
        sprintf(key11, "cittàResidenza");
        sprintf(value11, line.c_str());

        std::getline(file7, line); // Passa alla riga successiva
        sprintf(key12, "password");
        sprintf(value12, line.c_str());

        std::getline(file7, line); // Passa alla riga successiva
        sprintf(key13, "confermaPassword");
        sprintf(value13, line.c_str());

        std::getline(file7, line); // Passa alla riga successiva
        sprintf(key14, "dataCompleanno");
        sprintf(value14, line.c_str());
    }

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
    
    file7.close(); // Chiude il file



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Esegui test registrazioneUtenteCompratoreEmailUtenteEsistente

    // Apre il file in modalità di lettura
    std::ifstream file8("../test/registrazioneUtenteCompratoreEmailUtenteEsistente.txt"); 
    if (!file8.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file8, line);

    if (line == "EFFETTUA REGISTRAZIONE COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file8, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file8, line); // Passa alla riga successiva
        sprintf(key3, "categoriaUtente");
        sprintf(value3, line.c_str());

        std::getline(file8, line); // Passa alla riga successiva
        sprintf(key4, "nome");
        sprintf(value4, line.c_str());

        std::getline(file8, line); // Passa alla riga successiva
        sprintf(key5, "cognome");
        sprintf(value5, line.c_str());

        std::getline(file8, line); // Passa alla riga successiva
        sprintf(key6, "numeroTelefono");
        sprintf(value6, line.c_str());

        std::getline(file8, line); // Passa alla riga successiva
        sprintf(key7, "email");
        sprintf(value7, line.c_str());

        std::getline(file8, line); // Passa alla riga successiva
        sprintf(key8, "viaResidenza");
        sprintf(value8, line.c_str());

        std::getline(file8, line); // Passa alla riga successiva
        sprintf(key9, "numeroCivico");
        sprintf(value9, line.c_str());

        std::getline(file8, line); // Passa alla riga successiva
        sprintf(key10, "cap");
        sprintf(value10, line.c_str());

        std::getline(file8, line); // Passa alla riga successiva
        sprintf(key11, "cittàResidenza");
        sprintf(value11, line.c_str());

        std::getline(file8, line); // Passa alla riga successiva
        sprintf(key12, "password");
        sprintf(value12, line.c_str());

        std::getline(file8, line); // Passa alla riga successiva
        sprintf(key13, "confermaPassword");
        sprintf(value13, line.c_str());

        std::getline(file8, line); // Passa alla riga successiva
        sprintf(key14, "dataCompleanno");
        sprintf(value14, line.c_str());
    }

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
    
    file8.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test effettuaLogoutSenzaErrori.txt


    // Apre il file in modalità di lettura
    std::ifstream file31("../test/effettuaLogoutSenzaErrori.txt"); 
    if (!file31.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file31, line);

    if (line == "EFFETTUA LOGIN COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file31, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo al logout dell'utente compratore.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                
    printf("XADD %s * %s %s %s %s\n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s(id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file31.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test effettuaLogoutUtenteNonAttivo.txt


    // Apre il file in modalità di lettura
    std::ifstream file32("../test/effettuaLogoutUtenteNonAttivo.txt"); 
    if (!file32.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file32, line);

    if (line == "EFFETTUA LOGOUT COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file32, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo al logout dell'utente compratore.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                
    printf("XADD %s * %s %s %s %s\n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s(id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file32.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test effettuaLogoutUtenteNonRegistrato.txt


    // Apre il file in modalità di lettura
    std::ifstream file33("../test/effettuaLogoutUtenteNonRegistrato†.txt"); 
    if (!file33.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file33, line);

    if (line == "EFFETTUA LOGOUT COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file33, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo al logout dell'utente compratore.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                
    printf("XADD %s * %s %s %s %s\n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s(id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file33.close(); // Chiude il file




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Esegui test effettuaLoginCompratoreSenzaErrori

    // Apre il file in modalità di lettura
    std::ifstream file9("../test/effettuaLoginCompratoreSenzaErrori.txt"); 
    if (!file9.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file9, line);

    if (line == "EFFETTUA LOGIN COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file9, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file9, line); // Passa alla riga successiva
        sprintf(key3, "password");
        sprintf(value3, line.c_str());
    }

    // Effettuo un comando di scrittura relativo al login dell'utente compratore
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);
    
    file9.close(); // Chiude il file



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test effettuaLoginCompratoreUtenteNonEsistente

    // Apre il file in modalità di lettura
    std::ifstream file10("../test/effettuaLoginCompratoreUtenteNonEsistente.txt"); 
    if (!file10.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file10, line);

    if (line == "EFFETTUA LOGIN COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file10, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file10, line); // Passa alla riga successiva
        sprintf(key3, "password");
        sprintf(value3, line.c_str());

    }

    // Effettuo un comando di scrittura relativo al login dell'utente compratore
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);
    
    file10.close(); // Chiude il file



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test effettuaLoginCompratorePasswordErrata

    // Apre il file in modalità di lettura
    std::ifstream file11("../test/effettuaLoginCompratorePasswordErrata.txt"); 
    if (!file11.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file11, line);

    if (line == "EFFETTUA LOGIN COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file11, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file11, line); // Passa alla riga successiva
        sprintf(key3, "password");
        sprintf(value3, line.c_str());

    }

    // Effettuo un comando di scrittura relativo al login dell'utente compratore
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);
    
    file11.close(); // Chiude il file



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test effettuaLoginUtenteAttivo.txt


    // Apre il file in modalità di lettura
    std::ifstream file12("../test/effettuaLoginUtenteAttivo.txt"); 
    if (!file12.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file12, line);

    if (line == "EFFETTUA LOGIN COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file12, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file12, line); // Passa alla riga successiva
        sprintf(key3, "password");
        sprintf(value3, line.c_str());

    }

    // Effettuo un comando di scrittura relativo al login dell'utente compratore
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);
    
    file12.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiornaNumeroTelefonoCompratoreSenzaErrori.txt


    // Apre il file in modalità di lettura
    std::ifstream file13("../test/aggiornaNumeroTelefonoCompratoreSenzaErrori.txt"); 
    if (!file13.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file13, line);

    if (line == "AGGIORNA NUMERO TELEFONO COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file13, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file13, line); // Passa alla riga successiva
        sprintf(key3, "nuovoNumeroTelefono");
        sprintf(value3, line.c_str());

    }

    // Effettuo un comando di scrittura relativo all'aggiornamento di numero di telefono.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file13.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiornaNumeroTelefonoCompratoreNonRegistrato.txt


    // Apre il file in modalità di lettura
    std::ifstream file14("../test/aggiornaNumeroTelefonoCompratoreNonRegistrato.txt"); 
    if (!file14.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file14, line);

    if (line == "AGGIORNA NUMERO TELEFONO COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file14, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file14, line); // Passa alla riga successiva
        sprintf(key3, "nuovoNumeroTelefono");
        sprintf(value3, line.c_str());

    }

    // Effettuo un comando di scrittura relativo all'aggiornamento di numero di telefono.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

file14.close(); // Chiude il file



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiornaPasswordUtenteCompratoreSenzaErrori.txt


    // Apre il file in modalità di lettura
    std::ifstream file15("../test/aggiornaPasswordUtenteCompratoreSenzaErrori.txt"); 
    if (!file15.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file15, line);

    if (line == "AGGIORNA PASSWORD COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file15, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file15, line); // Passa alla riga successiva
        sprintf(key3, "vecchiaPassw");
        sprintf(value3, line.c_str());

        std::getline(file15, line); // Passa alla riga successiva
        sprintf(key4, "nuovaPassw");
        sprintf(value4, line.c_str());

    }

    // Effettuo un comando di scrittura relativo all'aggiornamento della password.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file15.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiornaPasswordUtenteCompratoreNonRegistrato.txt


    // Apre il file in modalità di lettura
    std::ifstream file16("../test/aggiornaPasswordUtenteCompratoreNonRegistrato.txt"); 
    if (!file16.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file16, line);

    if (line == "AGGIORNA PASSWORD COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file16, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file16, line); // Passa alla riga successiva
        sprintf(key3, "vecchiaPassw");
        sprintf(value3, line.c_str());

        std::getline(file16, line); // Passa alla riga successiva
        sprintf(key4, "nuovaPassw");
        sprintf(value4, line.c_str());

    }

    // Effettuo un comando di scrittura relativo all'aggiornamento della password.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file16.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiornaPasswordUtenteCompratoreVecchiaPasswordDiversa.txt


    // Apre il file in modalità di lettura
    std::ifstream file17("../test/aggiornaPasswordUtenteCompratoreVecchiaPasswordDiversa.txt"); 
    if (!file17.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file17, line);

    if (line == "AGGIORNA PASSWORD COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file17, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file17, line); // Passa alla riga successiva
        sprintf(key3, "vecchiaPassw");
        sprintf(value3, line.c_str());

        std::getline(file17, line); // Passa alla riga successiva
        sprintf(key4, "nuovaPassw");
        sprintf(value4, line.c_str());

    }

    // Effettuo un comando di scrittura relativo all'aggiornamento della password.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file17.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiornaPasswordUtenteCompratoreNuovaPasswordLunCorta.txt


    // Apre il file in modalità di lettura
    std::ifstream file18("../test/aggiornaPasswordUtenteCompratoreNuovaPasswordLunCorta.txt"); 
    if (!file18.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file18, line);

    if (line == "AGGIORNA PASSWORD COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file18, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file18, line); // Passa alla riga successiva
        sprintf(key3, "vecchiaPassw");
        sprintf(value3, line.c_str());

        std::getline(file18, line); // Passa alla riga successiva
        sprintf(key4, "nuovaPassw");
        sprintf(value4, line.c_str());

    }

    // Effettuo un comando di scrittura relativo all'aggiornamento della password.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file18.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiornaPasswordUtenteCompratoreNuovaPasswordSenzaCarSpeciale.txt


    // Apre il file in modalità di lettura
    std::ifstream file19("../test/aggiornaPasswordUtenteCompratoreNuovaPasswordSenzaCarSpeciale.txt"); 
    if (!file19.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file19, line);

    if (line == "AGGIORNA PASSWORD COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file19, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file19, line); // Passa alla riga successiva
        sprintf(key3, "vecchiaPassw");
        sprintf(value3, line.c_str());

        std::getline(file19, line); // Passa alla riga successiva
        sprintf(key4, "nuovaPassw");
        sprintf(value4, line.c_str());

    }


    // Effettuo un comando di scrittura relativo all'aggiornamento della password.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file19.close(); // Chiude il file



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiornaPasswordUtenteCompratoreNuovaPasswordSenzaNumero.txt


    // Apre il file in modalità di lettura
    std::ifstream file20("../test/aggiornaPasswordUtenteCompratoreNuovaPasswordSenzaNumero.txt"); 
    if (!file20.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file20, line);

    if (line == "AGGIORNA PASSWORD COMPRATORE"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file20, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file20, line); // Passa alla riga successiva
        sprintf(key3, "vecchiaPassw");
        sprintf(value3, line.c_str());

        std::getline(file20, line); // Passa alla riga successiva
        sprintf(key4, "nuovaPassw");
        sprintf(value4, line.c_str());

    }

    // Effettuo un comando di scrittura relativo all'aggiornamento della password.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file20.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiornaResidenzaUtenteCompratoreSenzaErrori.txt


    // Apre il file in modalità di lettura
    std::ifstream file21("../test/aggiornaResidenzaUtenteCompratoreSenzaErrori.txt"); 
    if (!file21.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file21, line);

    if (line == "AGGIORNA RESIDENZA"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file21, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file21, line); // Passa alla riga successiva
        sprintf(key3, "nuovaViaResidenza");
        sprintf(value3, line.c_str());

        std::getline(file21, line); // Passa alla riga successiva
        sprintf(key4, "nuovoNumCiv");
        sprintf(value4, line.c_str());

        std::getline(file21, line); // Passa alla riga successiva
        sprintf(key5, "nuovoCAP");
        sprintf(value5, line.c_str());

        std::getline(file21, line); // Passa alla riga successiva
        sprintf(key6, "nuovaCittaResidenza");
        sprintf(value6, line.c_str());

    }

    // Effettuo un comando di scrittura relativo all'aggiornamento della residenza.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf( "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file21.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiornaResidenzaUtenteCompratoreNonRegistrato.txt


    // Apre il file in modalità di lettura
    std::ifstream file22("../test/aggiornaResidenzaUtenteCompratoreNonRegistrato.txt"); 
    if (!file22.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file22, line);

    if (line == "AGGIORNA RESIDENZA"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file22, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file22, line); // Passa alla riga successiva
        sprintf(key3, "nuovaViaResidenza");
        sprintf(value3, line.c_str());

        std::getline(file22, line); // Passa alla riga successiva
        sprintf(key4, "nuovoNumCiv");
        sprintf(value4, line.c_str());

        std::getline(file22, line); // Passa alla riga successiva
        sprintf(key5, "nuovoCAP");
        sprintf(value5, line.c_str());

        std::getline(file22, line); // Passa alla riga successiva
        sprintf(key6, "nuovaCittaResidenza");
        sprintf(value6, line.c_str());

    }


    // Effettuo un comando di scrittura relativo all'aggiornamento della residenza.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf( "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file22.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiungiProdottoCarrelloUtenteCompratoreSenzaErrori.txt


    // Apre il file in modalità di lettura
    std::ifstream file23("../test/aggiungiProdottoCarrelloUtenteCompratoreSenzaErrori.txt"); 
    if (!file23.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file23, line);

    if (line == "AGGIUNGI PRODOTTO CARRELLO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file23, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file23, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo all'aggiungimento del prodtto nel carrello.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file23.close(); // Chiude il file

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiungiProdottoCarrelloUtenteCompratoreNonRegistrato.txt


    // Apre il file in modalità di lettura
    std::ifstream file24("../test/aggiungiProdottoCarrelloUtenteCompratoreNonRegistrato.txt"); 
    if (!file24.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file24, line);

    if (line == "AGGIUNGI PRODOTTO CARRELLO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file24, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file24, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo all'aggiungimento del prodtto nel carrello.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file24.close(); // Chiude il file



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiungiProdottoCarrelloNonEsistente.txt


    // Apre il file in modalità di lettura
    std::ifstream file25("../test/aggiungiProdottoCarrelloNonEsistente.txt"); 
    if (!file25.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file25, line);

    if (line == "AGGIUNGI PRODOTTO CARRELLO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file25, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file25, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo all'aggiungimento del prodtto nel carrello.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file25.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiungiProdottoCarrelloEsistente.txt


    // Apre il file in modalità di lettura
    std::ifstream file26("../test/aggiungiProdottoCarrelloEsistente.txt"); 
    if (!file26.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file26, line);

    if (line == "AGGIUNGI PRODOTTO CARRELLO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file26, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file26, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo all'aggiungimento del prodtto nel carrello.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file26.close(); // Chiude il file



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiungiProdottoListaDesideriUtenteCompratoreSenzaErrori.txt


    // Apre il file in modalità di lettura
    std::ifstream file27("../test/aggiungiProdottoListaDesideriUtenteCompratoreSenzaErrori.txt"); 
    if (!file27.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file27, line);

    if (line == "AGGIUNGI PRODOTTO LISTADESIDERI"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file27, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file27, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

    }

    // Effettuo un comando di scrittura relativo all'aggiungimento del prodtto nel carrello.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file27.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiungiProdottoListaDesideriUtenteCompratoreNonRegistrato.txt


    // Apre il file in modalità di lettura
    std::ifstream file28("../test/aggiungiProdottoListaDesideriUtenteCompratoreNonRegistrato.txt"); 
    if (!file28.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file28, line);

    if (line == "AGGIUNGI PRODOTTO LISTADESIDERI"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file28, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file28, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo all'aggiungimento del prodtto nel carrello.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file28.close(); // Chiude il file



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiungiProdottoListaDesideriNonEsistente.txt


    // Apre il file in modalità di lettura
    std::ifstream file29("../test/aggiungiProdottoListaDesideriNonEsistente.txt"); 
    if (!file29.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file29, line);

    if (line == "AGGIUNGI PRODOTTO LISTADESIDERI"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file29, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file29, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo all'aggiungimento del prodtto nel carrello.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file29.close(); // Chiude il file

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiungiProdottoListaDesideriEsistente.txt


    // Apre il file in modalità di lettura
    std::ifstream file30("../test/aggiungiProdottoListaDesideriEsistente.txt"); 
    if (!file30.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file30, line);

    if (line == "AGGIUNGI PRODOTTO LISTADESIDERI"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file30, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file30, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

    }

    // Effettuo un comando di scrittura relativo all'aggiungimento del prodtto nel carrello.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file30.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test rimozioneProdottoCarrelloSenzaErrori.txt


    // Apre il file in modalità di lettura
    std::ifstream file34("../test/rimozioneProdottoCarrelloSenzaErrori.txt"); 
    if (!file34.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file34, line);

    if (line == "RIMUOVI PRODOTTO CARRELLO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file34, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file34, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo alla rimozione del prodtto nel carrello.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);
    
    file34.close(); // Chiude il file



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test rimozioneProdottoCarrelloUtenteCompratoreNonRegistrato.txt


    // Apre il file in modalità di lettura
    std::ifstream file35("../test/rimozioneProdottoCarrelloUtenteCompratoreNonRegistrato.txt"); 
    if (!file35.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file35, line);

    if (line == "RIMUOVI PRODOTTO CARRELLO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file35, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file35, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo alla rimozione del prodtto nel carrello.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);
    
    file35.close(); // Chiude il file
 
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test rimozioneProdottoCarrelloNonEsistente.txt


    // Apre il file in modalità di lettura
    std::ifstream file36("../test/rimozioneProdottoCarrelloNonEsistente.txt"); 
    if (!file36.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file36, line);

    if (line == "RIMUOVI PRODOTTO CARRELLO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file36, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file36, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo alla rimozione del prodtto nel carrello.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);
    
    file36.close(); // Chiude il file

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test rimozioneProdottoListaDesideriSenzaErrori.txt


    // Apre il file in modalità di lettura
    std::ifstream file37("../test/rimozioneProdottoListaDesideriSenzaErrori.txt"); 
    if (!file37.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file37, line);

    if (line == "RIMUOVI PRODOTTO LISTADESIDERI"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file37, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file37, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo alla rimozione del prodtto nel carrello.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file37.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test rimozioneProdottoListaDesideriUtenteCompratoreNonRegistrato.txt


    // Apre il file in modalità di lettura
    std::ifstream file38("../test/rimozioneProdottoListaDesideriUtenteCompratoreNonRegistrato.txt"); 
    if (!file38.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file38, line);

    if (line == "RIMUOVI PRODOTTO LISTADESIDERI"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file38, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file38, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo alla rimozione del prodtto nel carrello.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file38.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test rimozioneProdottoListaDesideriNonEsistente.txt


    // Apre il file in modalità di lettura
    std::ifstream file39("../test/rimozioneProdottoListaDesideriNonEsistente.txt"); 
    if (!file39.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file39, line);

    if (line == "RIMUOVI PRODOTTO LISTADESIDERI"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file39, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file39, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo alla rimozione del prodtto nel carrello.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file39.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiuntaCartaPagamentoSenzaErrori.txt


    // Apre il file in modalità di lettura
    std::ifstream file40("../test/aggiuntaCartaPagamentoSenzaErrori.txt"); 
    if (!file40.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file40, line);

    if (line == "AGGIUNGI CARTA PAGAMENTO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file40, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file40, line); // Passa alla riga successiva
        sprintf(key3, "numeroCartaPagamento");
        sprintf(value3, line.c_str());

        std::getline(file40, line); // Passa alla riga successiva
        sprintf(key4, "cvvCartaPagamento");
        sprintf(value4, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo all'aggiungimento della carta di pagamento.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file40.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiuntaCartaPagamentoUtenteNonRegistrato.txt


    // Apre il file in modalità di lettura
    std::ifstream file41("../test/aggiuntaCartaPagamentoUtenteNonRegistrato.txt"); 
    if (!file41.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file41, line);

    if (line == "AGGIUNGI CARTA PAGAMENTO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file41, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file41, line); // Passa alla riga successiva
        sprintf(key3, "numeroCartaPagamento");
        sprintf(value3, line.c_str());

        std::getline(file41, line); // Passa alla riga successiva
        sprintf(key4, "cvvCartaPagamento");
        sprintf(value4, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo all'aggiungimento della carta di pagamento.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file41.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiuntaCartaPagamentoUtenteNonCompratore.txt


    // Apre il file in modalità di lettura
    std::ifstream file42("../test/aggiuntaCartaPagamentoUtenteNonCompratore.txt"); 
    if (!file42.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file42, line);

    if (line == "AGGIUNGI CARTA PAGAMENTO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file42, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file42, line); // Passa alla riga successiva
        sprintf(key3, "numeroCartaPagamento");
        sprintf(value3, line.c_str());

        std::getline(file42, line); // Passa alla riga successiva
        sprintf(key4, "cvvCartaPagamento");
        sprintf(value4, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo all'aggiungimento della carta di pagamento.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file42.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test aggiuntaCartaPagamentoEsistente.txt


    // Apre il file in modalità di lettura
    std::ifstream file43("../test/aggiuntaCartaPagamentoEsistente.txt"); 
    if (!file43.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file43, line);

    if (line == "AGGIUNGI CARTA PAGAMENTO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file43, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file43, line); // Passa alla riga successiva
        sprintf(key3, "numeroCartaPagamento");
        sprintf(value3, line.c_str());

        std::getline(file43, line); // Passa alla riga successiva
        sprintf(key4, "cvvCartaPagamento");
        sprintf(value4, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo all'aggiungimento della carta di pagamento.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file43.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test rimozioneCartaPagamentoSenzaErrori.txt


    // Apre il file in modalità di lettura
    std::ifstream file44("../test/rimozioneCartaPagamentoSenzaErrori.txt"); 
    if (!file44.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file44, line);

    if (line == "RIMUOVI CARTA PAGAMENTO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file44, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file44, line); // Passa alla riga successiva
        sprintf(key3, "numeroCartaPagamento");
        sprintf(value3, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo alla rimozione della carta di pagamento.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    freeReplyObject(reply);

    file44.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test rimozioneCartaPagamentoUtenteNonRegistrato.txt


    // Apre il file in modalità di lettura
    std::ifstream file45("../test/rimozioneCartaPagamentoUtenteNonRegistrato.txt"); 
    if (!file45.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file45, line);

    if (line == "RIMUOVI CARTA PAGAMENTO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file45, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file45, line); // Passa alla riga successiva
        sprintf(key3, "numeroCartaPagamento");
        sprintf(value3, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo alla rimozione della carta di pagamento.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    freeReplyObject(reply);

    file45.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test rimozioneCartaPagamentoNonEsistente.txt


    // Apre il file in modalità di lettura
    std::ifstream file46("../test/rimozioneCartaPagamentoNonEsistente.txt"); 
    if (!file46.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file46, line);

    if (line == "RIMUOVI CARTA PAGAMENTO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file46, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file46, line); // Passa alla riga successiva
        sprintf(key3, "numeroCartaPagamento");
        sprintf(value3, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo alla rimozione della carta di pagamento.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    freeReplyObject(reply);

    file46.close(); // Chiude il file

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test ricercaProdottoSenzaErrori.txt


    // Apre il file in modalità di lettura
    std::ifstream file47("../test/ricercaProdottoSenzaErrori.txt"); 
    if (!file47.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file47, line);

    if (line == "RICERCA PRODOTTO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file47, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file47, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo alla rimozione della carta di pagamento.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    freeReplyObject(reply);

    file47.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test ricercaProdottoUtenteCompratoreNonRegistrato.txt


    // Apre il file in modalità di lettura
    std::ifstream file48("../test/ricercaProdottoUtenteCompratoreNonRegistrato.txt"); 
    if (!file48.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file48, line);

    if (line == "RICERCA PRODOTTO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file48, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file48, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo alla rimozione della carta di pagamento.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    freeReplyObject(reply);

    file48.close(); // Chiude il file

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test ricercaProdottoNonEsistente.txt


    // Apre il file in modalità di lettura
    std::ifstream file49("../test/ricercaProdottoNonEsistente.txt"); 
    if (!file49.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file49, line);

    if (line == "RICERCA PRODOTTO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file49, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file49, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo alla rimozione della carta di pagamento.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

    freeReplyObject(reply);

    file49.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test acquistaProdottoSenzaErrori.txt


    // Apre il file in modalità di lettura
    std::ifstream file50("../test/acquistaProdottoSenzaErrori.txt"); 
    if (!file50.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file50, line);

    if (line == "ACQUISTA PRODOTTO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file50, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file50, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

        std::getline(file50, line); // Passa alla riga successiva
        sprintf(key4, "via_spedizione");
        sprintf(value4, line.c_str());

        std::getline(file50, line); // Passa alla riga successiva
        sprintf(key5, "città_spedizione");
        sprintf(value5, line.c_str());

        std::getline(file50, line); // Passa alla riga successiva
        sprintf(key6, "numero_civico_spedizione");
        sprintf(value6, line.c_str());

        std::getline(file50, line); // Passa alla riga successiva
        sprintf(key7, "CAP_spedizione");
        sprintf(value7, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo all'acquisto del prodotto.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6,  key7, value7);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file50.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test acquistoProdottoUtenteNonRegistrato.txt


    // Apre il file in modalità di lettura
    std::ifstream file51("../test/acquistoProdottoUtenteNonRegistrato.txt"); 
    if (!file51.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file51, line);

    if (line == "ACQUISTA PRODOTTO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file51, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file51, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

        std::getline(file51, line); // Passa alla riga successiva
        sprintf(key4, "via_spedizione");
        sprintf(value4, line.c_str());

        std::getline(file51, line); // Passa alla riga successiva
        sprintf(key5, "città_spedizione");
        sprintf(value5, line.c_str());

        std::getline(file51, line); // Passa alla riga successiva
        sprintf(key6, "numero_civico_spedizione");
        sprintf(value6, line.c_str());

        std::getline(file51, line); // Passa alla riga successiva
        sprintf(key7, "CAP_spedizione");
        sprintf(value7, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo all'acquisto del prodotto.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6,  key7, value7);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file51.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test acquistoProdottoUtenteNonEsistente.txt


    // Apre il file in modalità di lettura
    std::ifstream file52("../test/acquistoProdottoUtenteNonEsistente.txt"); 
    if (!file52.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file52, line);

    if (line == "ACQUISTA PRODOTTO"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file52, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

        std::getline(file52, line); // Passa alla riga successiva
        sprintf(key3, "codiceProdotto");
        sprintf(value3, line.c_str());

        std::getline(file52, line); // Passa alla riga successiva
        sprintf(key4, "via_spedizione");
        sprintf(value4, line.c_str());

        std::getline(file52, line); // Passa alla riga successiva
        sprintf(key5, "città_spedizione");
        sprintf(value5, line.c_str());

        std::getline(file52, line); // Passa alla riga successiva
        sprintf(key6, "numero_civico_spedizione");
        sprintf(value6, line.c_str());

        std::getline(file52, line); // Passa alla riga successiva
        sprintf(key7, "CAP_spedizione");
        sprintf(value7, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo all'acquisto del prodotto.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7);
                
    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6,  key7, value7);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);

    file52.close(); // Chiude il file


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test visionaOrdiniSenzaErrori.txt


    // Apre il file in modalità di lettura
    std::ifstream file53("../test/visionaOrdiniSenzaErrori.txt"); 
    if (!file53.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file53, line);

    if (line == "VISIONA ORDINI EFFETTUATI"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file53, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo alla visione degli ordini effettuati.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);
    
    file53.close(); // Chiude il file

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Esegui test visionaOrdiniUtenteNonRegistrato.txt


    // Apre il file in modalità di lettura
    std::ifstream file54("../test/visionaOrdiniUtenteNonRegistrato.txt"); 
    if (!file54.is_open()) {
        std::cerr << "Impossibile aprire il file!" << std::endl;
        return 1;
    }

    line;
    std::getline(file54, line);

    if (line == "VISIONA ORDINI EFFETTUATI"){
        sprintf(key1, "Action");
        sprintf(value1, line.c_str());

        std::getline(file54, line); // Passa alla riga successiva
        sprintf(key2, "nome_utente_compratore");
        sprintf(value2, line.c_str());

    }
    
    // Effettuo un comando di scrittura relativo alla visione degli ordini effettuati.
    reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);

    // Verifica la risposta del comando e termina il programma in caso di errore
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    printf("XADD %s * %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);
    printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

    // Libera la risorsa della risposta
    freeReplyObject(reply);
    
    file54.close(); // Chiude il file














































































































    while (1)
    {
        /* sleep   */
        micro_sleep(10000000); // 10 secondi di attesa

        printf("Attesa\n");

        // send arguments to server
        send_counter++;

        // Itero sulle azioni che effettua il client.
        for (i=0; i<39; i++){

            printf("\nValore della variabile i: %d \n", i);

            // Definisco 4 indici casuali per i parametri che passa il client
            int i5 = rand()%5;
            int i10 = rand()%10;
            int i100 = rand()%100;
            int i30 = rand()%30;

            sprintf(key1, "Action");
            sprintf(value1, test2[i].c_str());

            if (test2[i]== "EFFETTUA REGISTRAZIONE COMPRATORE")
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

                printf( "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6);
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
                std::string codiceProdotto = std::to_string(rand()%30);
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
                std::string codiceProdotto = std::to_string(rand()%30);
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
                std::string codiceProdotto = std::to_string(rand()%30);
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
                std::string codiceProdotto = std::to_string(rand()%30);
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
                std::string codiceProdotto = std::to_string(rand()%30);
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

                printf("XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6,  key7, value7);
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
                std::string codiceProdotto = std::to_string(rand()%30);
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
                sprintf(value2,  nomi_utente[i30].c_str());

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
                printf("main(): pid =%d: stream %s: Added  %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4,  key5, value5, reply->str);

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

    redisFree(c2r);
}
