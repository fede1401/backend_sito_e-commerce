
#include "main.h"
#include <array>
#include <string>

// cc -Wall -g -ggdb -o streams streams.c -lhiredis
// Usage: ./streams <add count> <read count> [block time, default: 1]

#define DEBUG 1000

#define READ_STREAM "stream1"
#define WRITE_STREAM "stream2"

using namespace std;

int main()
{

    redisContext *c2r;
    redisReply *reply;
    int read_counter = 0;
    int send_counter = 0;
    int block = 1000000000;
    int pid;
    unsigned seed;
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
    std::string server_types[38] = {
        
                                    "EFFETTUA_REGISTRAZIONE_COMPRATORE", 
                                    "EFFETTUA_REGISTRAZIONE_FORNITORE", 
                                    "EFFETTUA_REGISTRAZIONE_TRASPORTATORE", 

                                    "EFFETTUA_LOGIN_COMPRATORE", 
                                    "EFFETTUA_LOGIN_FORNITORE", 
                                    "EFFETTUA_LOGIN_TRASPORTATORE", 


                                    "AGGIORNA_NUMERO_TELEFONO_COMPRATORE", 
                                    "AGGIORNA_NUMERO_TELEFONO_FORNITORE", 
                                    "AGGIORNA_NUMERO_TELEFONO_TRASPORTATORE", 


                                    "AGGIORNA_PASSWORD_COMPRATORE",
                                    "AGGIORNA_PASSWORD_FORNITORE",
                                    "AGGIORNA_PASSWORD_TRASPORTATORE",


                                    "AGGIORNA_RESIDENZA", 
                                    "AGGIORNA_NOME_AZIENDAPRODUZIONE", 
                                    "AGGIORNA_NOME_DITTASPEDIZIONE", 


                                    "AGGIUNGI_CARTA_PAGAMENTO",


                                    "AGGIUNGI_PRODOTTO_SITO", 
                                    "AGGIUNGI_PRODOTTO_CARRELLO", 
                                    "AGGIUNGI_PRODOTTO_LISTADESIDERI", 


                                    "RICERCA_PRODOTTO", 

                                    "ACQUISTA_PRODOTTO", 

                                    "ASSEGNA_ORDINE_TRASPORTATORE",

                                    "AVVISA_SPEDIZIONE_EFFETTUATA", 

                                    "VISIONA_ORDINI_EFFETTUATI",

                                    "EFFETTUA_RECENSIONE",


                                    "ANNULLA_ORDINE", 

                                    "EFFETTUA_RESO", 


                                    "RIMUOVI_RECENSIONE",


                                    "RIMUOVI_CARTA_PAGAMENTO", 

                                    "RIMUOVI_PRODOTTO_CARRELLO", 

                                    "RIMUOVI_PRODOTTO_LISTADESIDERI", 

                                    "RIMUOVI_PRODOTTO_SITO",


                                    "EFFETTUA_LOGOUT_COMPRATORE", 
                                    "EFFETTUA_LOGOUT_FORNITORE", 
                                    "EFFETTUA_LOGOUT_TRASPORTATORE", 



                                    "ELIMINA_PROFILO_COMPRATORE", 
                                    "ELIMINA_PROFILO_FORNITORE", 
                                    "ELIMINA_PROFILO_TRASPORTATORE", 

        };

    /*  prg  */

#if (DEBUG > 0)
    setvbuf(stdout, (char *)NULL, _IONBF, 0);
    setvbuf(stderr, (char *)NULL, _IONBF, 0);
#endif

    // Inizializzazione del seme per i numeri casuali
    seed = (unsigned)time(NULL);
    srand(seed);

    // Generazione del nome utente basato su un numero casuale
    sprintf(username, "%u", rand());

    // Ottenimento dell'identificatore del processo
    pid = getpid();

    // Connessione a Redis
    printf("main(): pid %d: user %s: connecting to redis ...\n", pid, username);
    c2r = redisConnect("localhost", 6379);
    printf("main(): pid %d: user %s: connected to redis\n", pid, username);

    // Eliminazione degli stream se esistono
    reply = RedisCommand(c2r, "DEL %s", READ_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);

    reply = RedisCommand(c2r, "DEL %s", WRITE_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);


    // Creazione degli stream/gruppi
    initStreams(c2r, READ_STREAM);
    initStreams(c2r, WRITE_STREAM);

    printf("Creazione Streams");


    while (1)
    {
        // send arguments to server
        send_counter++;

        // Itero sulle azioni che può effettuare il client
        for (i=0; i<30; i++){
            sprintf(key1, "Action");
            sprintf(value1, "%s", server_types[i]);

            if (server_types[i]== "EFFETTUA_REGISTRAZIONE_COMPRATORE"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, "marco1");

                sprintf(key3, "categoriaUtente");
                sprintf(value3, "UtenteCompratore");

                sprintf(key4, "nome");
                sprintf(value4, "Marco");

                sprintf(key5, "cognome");
                sprintf(value5, "Rossi");

                sprintf(key6, "numeroTelefono");
                sprintf(value6, "123456789");

                sprintf(key7, "email");
                sprintf(value7, "marco.rossi1@gmail.com");

                sprintf(key8, "viaResidenza");
                sprintf(value8, "Via degli Ulivi");

                sprintf(key9, "numeroCivico");
                sprintf(value9, "123");

                sprintf(key10, "cap");
                sprintf(value10, "01011");

                sprintf(key11, "cittàResidenza");
                sprintf(value11, "Roma");

                sprintf(key12, "password");
                sprintf(value12, "Passsss..121");
                
                sprintf(key13, "confermaPassword");
                sprintf(value13, "Passsss..121");

                sprintf(key14, "dataCompleanno");
                sprintf(value14, "14/01/1999");


                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", 
                                        WRITE_STREAM, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                                        key9, value9, key10, value10, key11, value11, key12, value12, key13, value13, key14, value14);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, 
                                        key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                                        key9, value9, key10, value10, key11, value11, key12, value12, key13, value13, key14, value14, 
                                        reply->str);

                freeReplyObject(reply);


            }



            if (server_types[i] == "EFFETTUA_LOGIN_COMPRATORE"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, "marco1");

                sprintf(key3, "password");
                sprintf(value3, "Passsss..121");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }


            if (server_types[i] == "EFFETTUA_LOGOUT_COMPRATORE"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, "marco1");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, reply->str);

                freeReplyObject(reply);
            }


            if (server_types[i] == "ELIMINA_PROFILO_COMPRATORE"){
            
                reply = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM, key1, value1);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s (id: %s)\n", pid, WRITE_STREAM,  key1, value1, reply->str);

                freeReplyObject(reply);
            }

            if (server_types[i] == "AGGIORNA_NUMERO_TELEFONO_COMPRATORE"){
                sprintf(key2, "nuovoNumeroTelefono");
                sprintf(value2, "3384459873");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, reply->str);

                freeReplyObject(reply);
            }

            if (server_types[i] == "AGGIORNA_PASSWORD_COMPRATORE"){
                sprintf(key2, "vecchiaPassw");
                sprintf(value2, "Passsss..121");

                sprintf(key3, "nuovaPassw");
                sprintf(value3, "Passsss..333");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }

            if (server_types[i] == "AGGIORNA_RESIDENZA"){
                sprintf(key2, "nuovaViaResidenza");
                sprintf(value2, "Via dei Babbuini");

                sprintf(key3, "nuovoNumCiv");
                sprintf(value3, "11");

                sprintf(key4, "nuovoCAP");
                sprintf(value4, "10348");

                sprintf(key5, "nuovaCittaResidenza");
                sprintf(value5, "Torino");

        
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, reply->str);

                freeReplyObject(reply);
            }

            if (server_types[i] == "AGGIUNGI_CARTA_PAGAMENTO"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, "marco1");

                sprintf(key3, "numeroCartaPagamento");
                sprintf(value3, "4536467287372");

                sprintf(key4, "cvvCartaPagamento");
                sprintf(value4, "975");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2, key3, value3, key4, value4);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

                freeReplyObject(reply);
            }

            if (server_types[i] == "RIMUOVI_CARTA_PAGAMENTO"){
                sprintf(key2, "idCarta");
                sprintf(value2, "1");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, reply->str);

                freeReplyObject(reply);
            }


            if (server_types[i] == "AGGIUNGI_PRODOTTO_CARRELLO"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, "marco1");

                sprintf(key3, "codiceProdotto");
                sprintf(value3, "1");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }

            if (server_types[i] == "RIMUOVI_PRODOTTO_CARRELLO"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, "marco1");

                sprintf(key3, "codiceProdotto");
                sprintf(value3, "1");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }

            if (server_types[i] == "AGGIUNGI_PRODOTTO_LISTADESIDERI"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, "marco1");

                sprintf(key3, "codiceProdotto");
                sprintf(value3, "1");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }

            if (server_types[i] == "RIMUOVI_PRODOTTO_LISTADESIDERI"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, "marco1");

                sprintf(key3, "codiceProdotto");
                sprintf(value3, "1");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }

            if (server_types[i] == "ACQUISTA_PRODOTTO"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, "marco1");

                sprintf(key3, "via_spedizione");
                sprintf(value3, "Via dei Babbuini");

                sprintf(key4, "città_spedizione");
                sprintf(value4, "Torino");

                sprintf(key5, "numero_civico_spedizione");
                sprintf(value5, "121");

        
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, reply->str);

                freeReplyObject(reply);
            }

            if (server_types[i] == "RICERCA_PRODOTTO"){
                sprintf(key2, "nomeProdotto");
                sprintf(value2, "Nike Dump");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, reply->str);

                freeReplyObject(reply);
            }

            if (server_types[i] == "VISIONA_ORDINI_EFFETTUATI"){
                sprintf(key2, "nome_utente_compratore");
                sprintf(value2, "marco1");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, reply->str);

                freeReplyObject(reply);
            }

            if (server_types[i] == "ANNULLA_ORDINE"){
                sprintf(key2, "idOrdine");
                sprintf(value2, "1");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, reply->str);

                freeReplyObject(reply);
            }


            if (server_types[i] == "EFFETTUA_RESO"){
                sprintf(key2, "idOrdine");
                sprintf(value2, "1");

                // Modificare motivazioneReso
                sprintf(key3, "codiceProdotto");
                sprintf(value3, "1");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }


            if (server_types[i] == "EFFETTUA_RECENSIONE"){
                sprintf(key2, "idOrdine");
                sprintf(value2, "1");

                sprintf(key3, "descrizioneRecensione");
                sprintf(value3, "Prodotto ottimo");

                sprintf(key4, "voto_stella");
                sprintf(value4, "Cinque");

                        
                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2, key3, value3, key4, value4);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, key3, value3, key4, value4, reply->str);

                freeReplyObject(reply);
            }


            if (server_types[i] == "RIMUOVI_RECENSIONE"){
                sprintf(key2, "idRecensione");
                sprintf(value2, "1");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, reply->str);

                freeReplyObject(reply);
            }



            if (server_types[i]== "EFFETTUA_REGISTRAZIONE_FORNITORE"){
                sprintf(key2, "nome_utente_fornitore");
                sprintf(value2, "luigi2");

                sprintf(key3, "categoriaUtente");
                sprintf(value3, "UtenteFornitore");

                sprintf(key4, "nome");
                sprintf(value4, "Luigi");

                sprintf(key5, "cognome");
                sprintf(value5, "Pesce");

                sprintf(key6, "numeroTelefono");
                sprintf(value6, "3334447771");

                sprintf(key7, "email");
                sprintf(value7, "luigi.pesce@gmail.com");

                sprintf(key8, "password");
                sprintf(value8, "Passsss..121");
                
                sprintf(key9, "confermaPassword");
                sprintf(value9, "Passsss..121");

                sprintf(key10, "aziendaProduzione");
                sprintf(value10, "Nike");


                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", 
                                        WRITE_STREAM, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                                        key9, value9, key10, value10);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, 
                                        key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                                        key9, value9, key10, value10, reply->str);

                freeReplyObject(reply);
            }


            if (server_types[i] == "AGGIORNA_NOME_AZIENDAPRODUZIONE"){
                sprintf(key2, "nuovaAziendaProduzione");
                sprintf(value2, "Adidas");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, reply->str);

                freeReplyObject(reply);
            }


            if (server_types[i]== "AGGIUNGI_PRODOTTO_SITO"){
                sprintf(key2, "nomeProdotto");
                sprintf(value2, "luigi2");

                sprintf(key3, "categoriaProdotto");
                sprintf(value3, "UtenteFornitore");

                sprintf(key4, "prezzoProdotto");
                sprintf(value4, "Luigi");

                sprintf(key5, "descrizioneProdotto");
                sprintf(value5, "Pesce");

                sprintf(key6, "aziendaProduzione");
                sprintf(value6, "3334447771");

                sprintf(key7, "numeroCopieDisponibili");
                sprintf(value7, "luigi.pesce@gmail.com");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s", 
                                        WRITE_STREAM, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, 
                                        key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, reply->str);

                freeReplyObject(reply);
            }


            if (server_types[i] == "RIMUOVI_PRODOTTO_SITO"){
                sprintf(key2, "codiceProdotto");
                sprintf(value2, "1");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, reply->str);

                freeReplyObject(reply);
            }



            if (server_types[i] == "EFFETTUA_LOGIN_FORNITORE"){
                sprintf(key2, "nome_utente_fornitore");
                sprintf(value2, "luigi2");

                sprintf(key3, "password");
                sprintf(value3, "Passsss..121");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }


            if (server_types[i] == "EFFETTUA_LOGOUT_FORNITORE"){
                sprintf(key2, "nome_utente_fornitore");
                sprintf(value2, "luigi2");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, reply->str);

                freeReplyObject(reply);
            }


            if (server_types[i] == "ELIMINA_PROFILO_FORNITORE"){
            
                reply = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM, key1, value1);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s (id: %s)\n", pid, WRITE_STREAM,  key1, value1, reply->str);

                freeReplyObject(reply);
            }

            if (server_types[i] == "AGGIORNA_NUMERO_TELEFONO_FORNITORE"){
                sprintf(key2, "nuovoNumeroTelefono");
                sprintf(value2, "1119992228");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, reply->str);

                freeReplyObject(reply);
            }

            if (server_types[i] == "AGGIORNA_PASSWORD_FORNITORE"){
                sprintf(key2, "vecchiaPassw");
                sprintf(value2, "Passsss..121");

                sprintf(key3, "nuovaPassw");
                sprintf(value3, "Passsss..555");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }



            if (server_types[i]== "EFFETTUA_REGISTRAZIONE_TRASPORTATORE"){
                sprintf(key2, "nome_utente_trasportatore");
                sprintf(value2, "andrea3");

                sprintf(key3, "categoriaUtente");
                sprintf(value3, "UtenteTrasportatore");

                sprintf(key4, "nome");
                sprintf(value4, "Andrea");

                sprintf(key5, "cognome");
                sprintf(value5, "Lancia");

                sprintf(key6, "numeroTelefono");
                sprintf(value6, "4445558889");

                sprintf(key7, "email");
                sprintf(value7, "andrea.lancia@gmail.com");

                sprintf(key8, "password");
                sprintf(value8, "Passsss..121");
                
                sprintf(key9, "confermaPassword");
                sprintf(value9, "Passsss..121");

                sprintf(key10, "dittaSpedizione");
                sprintf(value10, "FedEx");


                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", 
                                        WRITE_STREAM, key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                                        key9, value9, key10, value10);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, 
                                        key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6, key7, value7, key8, value8,
                                        key9, value9, key10, value10, reply->str);

                freeReplyObject(reply);
            }

            if (server_types[i] == "AGGIORNA_NOME_DITTASPEDIZIONE"){
                sprintf(key2, "nuovaDittaSpedizione");
                sprintf(value2, "Bartolini");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, reply->str);

                freeReplyObject(reply);
            }


            if (server_types[i] == "AVVISA_SPEDIZIONE_EFFETTUATA"){
                sprintf(key2, "idSpedizione");
                sprintf(value2, "1");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, reply->str);

                freeReplyObject(reply);
            }

            if (server_types[i] == "ASSEGNA_ORDINE_TRASPORTATORE"){
        
                reply = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM, key1, value1);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, reply->str);

                freeReplyObject(reply);
            }



            if (server_types[i] == "EFFETTUA_LOGIN_TRASPORTATORE"){
                sprintf(key2, "nome_utente_trasportatore");
                sprintf(value2, "andrea3");

                sprintf(key3, "password");
                sprintf(value3, "Passsss..121");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }


            if (server_types[i] == "EFFETTUA_LOGOUT_TRASPORTATORE"){
                sprintf(key2, "nome_utente_trasportatore");
                sprintf(value2, "andrea3");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, reply->str);

                freeReplyObject(reply);
            }


            if (server_types[i] == "ELIMINA_PROFILO_TRASPORTATORE"){
            
                reply = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM, key1, value1);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s (id: %s)\n", pid, WRITE_STREAM,  key1, value1, reply->str);

                freeReplyObject(reply);
            }

            if (server_types[i] == "AGGIORNA_NUMERO_TELEFONO_TRASPORTATORE"){
                sprintf(key2, "nuovoNumeroTelefono");
                sprintf(value2, "576768474738");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, reply->str);

                freeReplyObject(reply);
            }

            if (server_types[i] == "AGGIORNA_PASSWORD_TRASPORTATORE"){
                sprintf(key2, "vecchiaPassw");
                sprintf(value2, "Passsss..121");

                sprintf(key3, "nuovaPassw");
                sprintf(value3, "Passsss..777");

                reply = RedisCommand(c2r, "XADD %s * %s %s %s %s %s %s", WRITE_STREAM, key1, value1, key2, value2, key3, value3);
                assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                printf("main(): pid =%d: stream %s: Added %s %s %s %s %s %s (id: %s)\n", pid, WRITE_STREAM, key1, value1, key2, value2, key3, value3, reply->str);

                freeReplyObject(reply);
            }



        }


        //  read result from server
        read_counter++;
        reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >",
                             username, block, READ_STREAM);

        printf("main(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM);

        assertReply(c2r, reply);
        dumpReply(reply, 0);

        // Scorro il numero di Streams nella connessione Redis
        for (k = 0; k < ReadNumStreams(reply); k++)
        {
            ReadStreamName(reply, streamname, k);

            // Scorro il numero di messaggi della Streams Redis
            for (i = 0; i < ReadStreamNumMsg(reply, k); i++)
            {
                ReadStreamNumMsgID(reply, k, i, msgid);

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
        micro_sleep(5000000);
    } // while ()

    redisFree(c2r);
}
