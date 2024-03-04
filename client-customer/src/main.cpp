
#include "main.h"
#include <array>
#include <string>
#include <string.h>

// cc -Wall -g -ggdb -o streams streams.c -lhiredis
// Usage: ./streams <add count> <read count> [block time, default: 1]

#define DEBUG 1000

#define READ_STREAM_CUSTOMER "stream2"
#define WRITE_STREAM_CUSTOMER "stream1"

using namespace std;

int main()
{

    redisContext *c2r;
    redisReply *reply;
    int read_counter = 0;
    int send_counter = 0;
    int block = 1000000000;
    int pid;
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

    
    char streamname[100];
    char msgid[100];
    char fval[100];
    int i, k, h;
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


        std::string test2[18] ={
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

                            "RICERCA PRODOTTO", 
                            //"ACQUISTA PRODOTTO", 
                            "RICERCA PRODOTTO", 
                            //"ACQUISTA PRODOTTO", 
                            "RICERCA PRODOTTO", 
                            //"ACQUISTA PRODOTTO", 

                            "EFFETTUA RESO", 
                            "EFFETTUA RECENSIONE"
        };


    // Array di nomi utente
    std::array<std::string, 100> nomi_utente;
    for (int i = 0; i < 100; ++i) {
        nomi_utente[i] = "Utente" + std::to_string(i + 1);
    }

    // Array di nomi
    std::array<std::string, 10> nomi = {"Mario", "Luigi", "Giovanni", "Alessia", "Chiara", "Francesco", "Elena", "Roberto", "Laura", "Paolo"};

    // Array di cognomi
    std::array<std::string, 10> cognomi = {"Rossi", "Bianchi", "Verdi", "Ferrari", "Russo", "Esposito", "Romano", "Gallo", "Conti", "De Luca"};

    // Array di email
    std::array<std::string, 100> email;
    for (int i = 0; i < 100; ++i)
    {
        email[i] = nomi_utente[i] + "@example.com";
    }

    // Array di numeri di telefono
    std::array<std::string, 100> numeri_telefono;
    for (int i = 0; i < 100; ++i)
    {
        numeri_telefono[i] = "123456789" + std::to_string(i);
    }

    // Array di password
    std::array<std::string, 100> password;
    for (int i = 0; i < 100; ++i)
    {
        password[i] = "P.assword" + std::to_string(i + 1);
    }


    // Array di nomi di prodotti
    std::array<std::string, 100> nomi_prodotti;
    for (int i = 0; i < 100; ++i)
    {
        nomi_prodotti[i] = "Prodotto" + std::to_string(i + 1);
    }

    // Array di nomi di prodotti
    std::string descrizioneProdotti = "Prodotto eccellente.";

    // Array di categorie prodotti
    std::array<std::string, 10> categorie_prodotti = {"Abbigliamento", "Elettronica", "Informatica", "Arredamento", "Culinario", "Sportivo", "Illuminazione", "Giardinaggio", "Musicale", "Fotografia"};


    // Array di prezzi e copie disponibili di prodotti
    std::array<std::string, 100> prezzo_copie_disponibili;
    for (int i = 0; i < 100; ++i)
    {
        prezzo_copie_disponibili[i] = std::to_string(i + 1);
    }

    // Array di date di nascita
    std::array<std::string, 100> date_di_nascita;
    for (int i = 0; i < 100; ++i)
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
    std::array<std::string, 100> numeri_civici;
    for (int i = 0; i < 100; ++i)
    {
        numeri_civici[i] = std::to_string(i + 1);
    }

    // Array di numeri di carte di pagamento:
    std::array<std::string, 100> carte_pagamento;
    for (int i = 0; i < 100; ++i) {
        std::string carta = ""; 
        
        // Genera le restanti 10 cifre casuali
        for (int j = 0; j < 10; ++j) {
            carta += std::to_string(rand() % 10); // Aggiunge una cifra casuale compresa tra 0 e 9
        }
        
        carte_pagamento[i] = carta;
    }

    // Array di numeri di carte di pagamento:
    std::array<std::string, 100> cvv_cartePagamento;
    for (int i = 0; i < 100; ++i) {
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

    // Inizializzazione del seme per i numeri casuali
    //seed = (unsigned)time(NULL);
    //srand(seed);

    // Generazione del nome utente basato su un numero casuale
    //sprintf(username, "%u", rand());

    strcpy(username, "federico");

    // Ottenimento dell'identificatore del processo
    pid = getpid();

    // Connessione a Redis
    printf("main(): pid %d: user %s: connecting to redis ...\n", pid, username);
    c2r = redisConnect("localhost", 6379);
    printf("main(): pid %d: user %s: connected to redis\n", pid, username);

    // Eliminazione degli stream se esistono
    reply = RedisCommand(c2r, "DEL %s", READ_STREAM_CUSTOMER);
    assertReply(c2r, reply);
    dumpReply(reply, 0);

    reply = RedisCommand(c2r, "DEL %s", WRITE_STREAM_CUSTOMER);
    assertReply(c2r, reply);
    dumpReply(reply, 0);


    // Creazione degli stream/gruppi
    initStreams(c2r, READ_STREAM_CUSTOMER);
    initStreams(c2r, WRITE_STREAM_CUSTOMER);

    printf("Creazione Streams\n");

    /* init random number generator  */
    srand((unsigned)time(NULL));


    while (1)
    {
        // send arguments to server
        send_counter++;

        // Itero sulle azioni che può effettuare il client
        for (i=0; i<21; i++){

            printf("\nValore della variabile i: %d \n", i);

            // Definisco 3 indici casuali
            int i5 = rand()%5;
            int i10 = rand()%10;
            int i100 = rand()%100;

            sprintf(key1, "Action");
            sprintf(value1, test2[i].c_str());

            if (test2[i]== "EFFETTUA REGISTRAZIONE COMPRATORE"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i100].c_str());

                sprintf(key3, "categoriaUtente");
                sprintf(value3, "UtenteCompratore");

                sprintf(key4, "nome");
                sprintf(value4, nomi[i10].c_str());

                sprintf(key5, "cognome");
                sprintf(value5, cognomi[i10].c_str());

                sprintf(key6, "numeroTelefono");
                sprintf(value6, numeri_telefono[i100].c_str());

                sprintf(key7, "email");
                sprintf(value7, email[i100].c_str());

                sprintf(key8, "viaResidenza");
                sprintf(value8, vie_di_residenza[i5].c_str());

                sprintf(key9, "numeroCivico");
                sprintf(value9, numeri_civici[i100].c_str());

                sprintf(key10, "cap");
                sprintf(value10, CAP[i5].c_str());

                sprintf(key11, "cittàResidenza");
                sprintf(value11, citta_di_residenza[i5].c_str());

                sprintf(key12, "password");
                sprintf(value12, password[i100].c_str());
                
                sprintf(key13, "confermaPassword");
                sprintf(value13, password[i100].c_str());

                sprintf(key14, "dataCompleanno");
                sprintf(value14, date_di_nascita[i100].c_str());


                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", 
                                        WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                                        key9, value9, key10, value10, key11, value11, key12, value12, key13, value13, key14, value14);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);


                printf("XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s \n", 
                        WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                        key9, value9, key10, value10, key11, value11, key12, value12, key13, value13, key14, value14);

                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n\n", pid, WRITE_STREAM_CUSTOMER, 
                                        key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                                        key9, value9, key10, value10, key11, value11, key12, value12, key13, value13, key14, value14, 
                                        reply->str);

                freeReplyObject(reply);


            }



            if (test2[i] == "EFFETTUA LOGIN COMPRATORE"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i100].c_str());

                sprintf(key3, "password");
                sprintf(value3, password[i100].c_str());

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }


            if (test2[i] == "EFFETTUA LOGOUT COMPRATORE"){
                
                reply = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key1, value1);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                
                printf("XADD %s * %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1);
                printf("main(): pid =%d: stream %s: Added %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, reply->str);

                freeReplyObject(reply);
            }


            if (test2[i] == "ELIMINA PROFILO COMPRATORE"){
            
                reply = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_CUSTOMER, key1, value1);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1);
                printf("main(): pid =%d: stream %s: Added %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER,  key1, value1, reply->str);

                freeReplyObject(reply);
            }

            if (test2[i] == "AGGIORNA NUMERO TELEFONO COMPRATORE"){
                sprintf(key2, "nuovoNumeroTelefono");
                sprintf(value2, numeri_telefono[i100].c_str());

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

                freeReplyObject(reply);
            }

            if (test2[i] == "AGGIORNA PASSWORD COMPRATORE"){
                sprintf(key2, "vecchiaPassw");
                sprintf(value2, password[i100].c_str());

                sprintf(key3, "nuovaPassw");
                sprintf(value3, password[i100].c_str());

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }

            if (test2[i] == "AGGIORNA RESIDENZA"){
                sprintf(key2, "nuovaViaResidenza");
                sprintf(value2, vie_di_residenza[i5].c_str());

                sprintf(key3, "nuovoNumCiv");
                sprintf(value3, numeri_civici[i100].c_str());

                sprintf(key4, "nuovoCAP");
                sprintf(value4, CAP[i5].c_str());

                sprintf(key5, "nuovaCittaResidenza");
                sprintf(value5, citta_di_residenza[i5].c_str());

        
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf( "XADD %s * %s %s %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, reply->str);

                freeReplyObject(reply);
            }

            if (test2[i] == "AGGIUNGI CARTA PAGAMENTO"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i100].c_str());

                sprintf(key3, "numeroCartaPagamento");
                sprintf(value3, carte_pagamento[i100].c_str());

                sprintf(key4, "cvvCartaPagamento");
                sprintf(value4, cvv_cartePagamento[i100].c_str());

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

                freeReplyObject(reply);
            }

            if (test2[i] == "RIMUOVI CARTA PAGAMENTO"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i100].c_str());

                sprintf(key3, "numeroCartaPagamento");
                sprintf(value3, carte_pagamento[i100].c_str());

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }


            if (test2[i] == "AGGIUNGI PRODOTTO CARRELLO"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i100].c_str());

                sprintf(key3, "nomeProdotto");
                sprintf(value3, nomi_prodotti[i100].c_str());

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }

            if (test2[i] == "RIMUOVI PRODOTTO CARRELLO"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i100].c_str());

                sprintf(key3, "nomeProdotto");
                sprintf(value3, nomi_prodotti[i100].c_str());

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }

            if (test2[i] == "AGGIUNGI PRODOTTO LISTADESIDERI"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i100].c_str());

                sprintf(key3, "nomeProdotto");
                sprintf(value3, nomi_prodotti[i100].c_str());


                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }

            if (test2[i] == "RIMUOVI PRODOTTO LISTADESIDERI"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i100].c_str());

                sprintf(key3, "nomeProdotto");
                sprintf(value3, nomi_prodotti[i100].c_str());


                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }

            if (test2[i] == "ACQUISTA PRODOTTO"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i100].c_str());

                sprintf(key3, "nomeProdotto");
                sprintf(value3, nomi_prodotti[i100].c_str());

                sprintf(key4, "via_spedizione");
                sprintf(value4, vie_di_residenza[i5].c_str());

                sprintf(key5, "città_spedizione");
                sprintf(value5, citta_di_residenza[i5].c_str());

                sprintf(key6, "numero_civico_spedizione");
                sprintf(value6, numeri_civici[i100].c_str());

                sprintf(key7, "CAP_spedizione");
                sprintf(value7, CAP[i100].c_str());

        
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6,  key7, value7);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, reply->str);

                freeReplyObject(reply);
            }

            if (test2[i] == "RICERCA PRODOTTO"){
                sprintf(key2, "nomeProdotto");
                sprintf(value2, nomi_prodotti[i100].c_str());

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

                freeReplyObject(reply);
            }

            if (test2[i] == "VISIONA ORDINI EFFETTUATI"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i100].c_str());

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, reply->str);

                freeReplyObject(reply);
            }

            if (test2[i] == "ANNULLA ORDINE"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i100].c_str());

                sprintf(key3, "idOrdine");
                // Dovrei effettuare una selezione del numero ordine compreso tra quelli presenti.
                sprintf(value3, "1");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }


            if (test2[i] == "EFFETTUA RESO"){
                sprintf(key2, "idOrdine");
                sprintf(value2, "1");

                // Modificare motivazioneReso
                sprintf(key3, "codiceProdotto");
                sprintf(value3, "1");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s * %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }


            if (test2[i] == "EFFETTUA RECENSIONE"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2,  nomi_utente[i100].c_str());

                sprintf(key3, "idOrdine");
                sprintf(value3, "1");

                sprintf(key4, "descrizioneRecensione");
                sprintf(value4, descrizioneProdotti.c_str());

                sprintf(key5, "voto_stella");
                sprintf(value5, "Cinque");

                        
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s *  %s %s %s %s %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5);
                printf("main(): pid =%d: stream %s: Added  %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, key4, value4,  key5, value5, reply->str);

                freeReplyObject(reply);
            }


            if (test2[i] == "RIMUOVI RECENSIONE"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, nomi_utente[i100].c_str());

                sprintf(key3, "idRecensione");
                sprintf(value3, "1");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);

                printf("XADD %s *  %s %s %s %s %s %s \n", WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3);
                printf("main(): pid =%d: stream %s: Added  %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM_CUSTOMER, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }

        }


        printf("\n\nOra di leggere i risultati dal server. \n");

        //  read result from server
        read_counter++;
        reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_CUSTOMER);

        printf("Effettuato comando per leggere i messaggi della Streams. \n");

        printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_CUSTOMER);

        assertReply(c2r, reply);
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

        freeReplyObject(reply);

        /* sleep   */
        micro_sleep(10000000); // 10 secondi di attesa
    } // while ()

    redisFree(c2r);
}
