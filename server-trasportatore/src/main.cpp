#include "svolgiAzione.h"
// #include "main.h"

#include "/home/federico/sito_ecommerce/github/backend_sito_e-commerce/con2redis/src/con2redis.h"
#include <string.h>

// cc -Wall -g -ggdb -o streams streams.c -lhiredis
// Usage: ./streams <add count> <read count> [block time, default: 1]

#define DEBUG 1000

#define READ_STREAM_TRASPORTATORE "stream5"
#define WRITE_STREAM_TRASPORTATORE "stream6"

using namespace std;


int main()
{

    redisContext *c2r;
    redisReply *reply;
    redisReply *reply2;
    int read_counter = 0;
    int send_counter = 0;
    int block = 1000000000;
    int pid;
    //unsigned seed;
    char username[100];
    char key[100];
    char value[100];
    char streamname[100];
    char msgid[100];
    char fval[100];
    int i, k, h;
    char action[100];
    char nome_utente_compratore[100];
    char nome_utente_fornitore[100];
    char nome_utente_trasportatore[100];
    char nome[100];
    char cognome[100];
    char categoriaUtente[100];
    char password[100];
    char nuovoNumeroTelefono[100];
    char vecchiaPassw[100];
    char nuovaPassw[100];
    char numeroTelefono[100];
    char email[100];
    char viaResidenza[100];
    char numeroCivico[100];
    char cap[100];
    char cittàResidenza[100];
    char confermaPassword[100];
    char dataCompleanno[100];
    int codiceProdotto;
    char numeroCartaPagamento[100];
    char cvvCartaPagamento[100];
    int idCarta;
    int idOrdine;
    char nuovaViaResidenza[100];
    char nuovoNumCiv[100];
    char nuovoCAP[100];
    char nuovaCittaResidenza[100];
    char via_spedizione[100];
    char città_spedizione[100];
    char numero_civico_spedizione[100];
    char descrizioneRecensione[100];
    int voto_stella;
    int idRecensione;
    char motivazione_reso[100];
    char nomeProdotto[100];
    char categoriaProdotto[100];
    char aziendaProduzione[100];
    char nuovaAziendaProduzione[100];
    char descrizioneProdotto[100];
    int numeroCopieDisponibili;
    char dittaSpedizione[100];
    char nuovaDittaSpedizione[100];
    int idSpedizione;
    float prezzoProdotto;


    char outputs[100];

    UtenteCompratore compratore;
    UtenteFornitore fornitore;
    UtenteTrasportatore trasportatore;
    Product prodotto;
    /*  prg  */

#if (DEBUG > 0)
    setvbuf(stdout, (char *)NULL, _IONBF, 0);
    setvbuf(stderr, (char *)NULL, _IONBF, 0);
#endif

    strcpy(username, "federico");

    // Ottenimento dell'identificatore del processo
    pid = getpid();

    // Connessione a Redis
    printf("main(): pid %d: user %s: connecting to redis ...\n", pid, username);
    c2r = redisConnect("localhost", 6379);
    if (c2r == NULL || c2r->err) {
        if (c2r) {
            printf("Errore di connessione: %s\n", c2r->errstr);
            redisFree(c2r);
        } else {
            printf("Errore di allocazione del contesto Redis\n");
        }
    // Gestisci l'errore e termina il programma o riprova la connessione
    }
    printf("main(): pid %d: user %s: connected to redis, %d\n", pid, username, c2r->err);



    
    // Eliminazione degli stream se esistono
    reply = RedisCommand(c2r, "DEL %s", READ_STREAM_TRASPORTATORE);
    assertReply(c2r, reply);
    dumpReply(reply, 0);

    reply = RedisCommand(c2r, "DEL %s", WRITE_STREAM_TRASPORTATORE);
    assertReply(c2r, reply);
    dumpReply(reply, 0);
    
    

    //printf("Eliminazione vecchie streams!");


    /* Create streams/groups */
    initStreams(c2r, READ_STREAM_TRASPORTATORE);
    initStreams(c2r, WRITE_STREAM_TRASPORTATORE);

    printf("Streams create!\n");

    Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");
    printf("Connessione al database avvenuta con successo");

    while (1)
    {
        //  read
        read_counter++;

        micro_sleep(7000000); // 7 secondi di attesa necessari per far sì che il client mandi tutte le richieste

        // Imposto COUNT a -1 per leggere tutti i messaggi disponibili nello stream
        reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT -1 NOACK STREAMS %s >", username, block, READ_STREAM_TRASPORTATORE);

        printf("\n\nmain(): pid %d: user %s: Read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM_TRASPORTATORE);

        assertReply(c2r, reply);
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
            for (i = 0; i < ReadStreamNumMsg(reply, k); i++)  // Il problema dell'errore di segmentazione è qua!!!
            {
                printf("\n\n\n\nPROSSIMO MESSAGGIO NELLA STREAM.");

                ReadStreamNumMsgID(reply, k, i, msgid); 

                printf("Message number %d from Stream: %d\n", i, k );

                printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n",
                       pid, username, streamname, k, i, msgid, ReadStreamMsgNumVal(reply, k, i));

                
                
                // Scorro il numero di valori del messaggio della Streams Redis
                // h deve partire da 0, altrimenti non troverà mai fval == "Action"
                for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h = h + 1)
                {
                    ReadStreamMsgVal(reply, k, i, h, fval);

                    printf("\nValue %d from message number %d from Stream: %d\n", h, i, k );

                    printf("main(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);

                    printf("Fval: %s\n", fval);

                    // Qui bisogna estrapolare l'azione da effettuare:
                    if (strcmp(fval, "Action") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        printf("\nFval: %s\n", fval);

                        strcpy(action, fval);
                        
                        printf("Action: %s\n\n", action);
                    }


                    // una volta estrapolata selezionare tutti i parametri necessari per effettuarla
                    if (strcmp(fval, "nome_utente_compratore") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(nome_utente_compratore, fval);
                    }
                    if (strcmp(fval, "nome_utente_fornitore") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(nome_utente_fornitore, fval);
                    }
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
                    if (strcmp(fval, "codiceProdotto") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        codiceProdotto = atoi(fval);
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
                    
                    if (strcmp(fval, "idOrdine") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        idOrdine = atoi(fval);
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
                    if (strcmp(fval, "descrizioneRecensione") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(descrizioneRecensione, fval);
                    }
                    if (strcmp(fval, "voto_stella") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        voto_stella = atoi(fval);
                    }
                    if (strcmp(fval, "idRecensione") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        idRecensione = atoi(fval);
                    }
                    if (strcmp(fval, "motivazione_reso") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(motivazione_reso, fval);
                    }
                    if (strcmp(fval, "nomeProdotto") == 0)
                    {
                        ReadStreamMsgVal(reply, k, i, h+1, fval);
                        strcpy(nomeProdotto, fval);
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

                }
                //printf("main(): pid %d: user %s: sum = %d\n", pid, username, sum);

                printf("\nAction: %s\n", action);
                printf("nome_utente_compratore: %s\n", nome_utente_compratore);
                printf("nome_utente_fornitore: %s\n", nome_utente_fornitore);
                printf("nome_utente_trasportatore: %s\n", nome_utente_trasportatore);
                printf("nome: %s\n", nome);
                printf("cognome: %s\n", cognome);
                printf("categoriaUtente: %s\n", categoriaUtente);
                printf("password: %s\n", password);
                printf("nuovoNumeroTelefono: %s\n", nuovoNumeroTelefono);
                printf("vecchiaPassw: %s\n", vecchiaPassw);
                printf("nuovaPassw: %s\n", nuovaPassw);
                printf("numeroTelefono: %s\n", numeroTelefono);
                printf("email: %s\n", email);
                printf("viaResidenza: %s\n", viaResidenza);
                printf("numeroCivico: %s\n", numeroCivico);
                printf("cap: %s\n", cap);
                printf("cittàResidenza: %s\n", cittàResidenza);
                printf("confermaPassword: %s\n", confermaPassword);
                printf("dataCompleanno: %s\n", dataCompleanno);
                printf("codiceProdotto: %d\n", codiceProdotto);
                printf("numeroCartaPagamento: %s\n", numeroCartaPagamento);
                printf("cvvCartaPagamento: %s\n", cvvCartaPagamento);
                printf("idCarta: %d\n", idCarta);
                printf("idOrdine: %d\n", idOrdine);
                printf("nuovaViaResidenza: %s\n", nuovaViaResidenza);
                printf("nuovoNumCiv: %s\n", nuovoNumCiv);
                printf("nuovoCAP: %s\n", nuovoCAP);
                printf("nuovaCittaResidenza: %s\n", nuovaCittaResidenza);
                printf("via_spedizione: %s\n", via_spedizione);
                printf("città_spedizione: %s\n", città_spedizione);
                printf("numero_civico_spedizione: %s\n", numero_civico_spedizione);
                printf("descrizioneRecensione: %s\n", descrizioneRecensione);
                printf("voto_stella: %d\n", voto_stella);
                printf("idRecensione: %d\n", idRecensione);
                printf("motivazione_reso: %s\n", motivazione_reso);
                printf("nomeProdotto: %s\n", nomeProdotto);
                printf("categoriaProdotto: %s\n", categoriaProdotto);
                printf("aziendaProduzione: %s\n", aziendaProduzione);
                printf("nuovaAziendaProduzione: %s\n", nuovaAziendaProduzione);
                printf("descrizioneProdotto: %s\n", descrizioneProdotto);
                printf("prezzoProdotto: %.2f\n", prezzoProdotto); // Print float with 2 decimal places
                printf("numeroCopieDisponibili: %d\n", numeroCopieDisponibili);
                printf("dittaSpedizione: %s\n", dittaSpedizione);
                printf("nuovaDittaSpedizione: %s\n", nuovaDittaSpedizione);
                printf("idSpedizione: %d\n", idSpedizione);

                // Qui bisogna svolgere l'azione:
                

                printf("Azione: %s\n", action);

                // Utente trasportatore
                 if (std::string(action) == "EFFETTUA REGISTRAZIONE TRASPORTATORE"){

                    //UtenteTrasportatore trasportatore;

                    trasportatore.effettuaRegistrazione(db1, nome_utente_trasportatore, categoriaUtente, nome, cognome, numeroTelefono, email, password, confermaPassword, dittaSpedizione);

                    strcpy(outputs, "Effettuata registrazione trasportatore");

                    //freeReplyObject(reply);

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

                    /* sleep   */
                    micro_sleep(1000000);
                }

                if (std::string(action) == "AGGIORNA NOME DITTASPEDIZIONE"){

                    //UtenteTrasportatore trasportatore;
                    trasportatore.aggiornaNomeDittaSpedizione(db1, nuovaDittaSpedizione);

                    strcpy(outputs, "Aggiornamento ditta spedizione");

                    //freeReplyObject(reply);

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

                    /* sleep   */
                    //micro_sleep(5000000);
                }

                if (std::string(action) == "AVVISA SPEDIZIONE EFFETTUATA"){
                    Spedizione spedizione;
                    spedizione.spedizioneConsegnata(db1, nome_utente_trasportatore, idSpedizione);
                    
                    strcpy(outputs, "Spedizione effettuata");

                    //freeReplyObject(reply);

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

                    /* sleep   */
                    //micro_sleep(5000000);
                }

                if (std::string(action) == "ASSEGNA ORDINE TRASPORTATORE"){

                    Spedizione spedizione;
                    spedizione.assegnaOrdineTrasportatore(db1);

                    strcpy(outputs, "Spedizione assegnata");

                    //freeReplyObject(reply);

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

                    /* sleep   */
                    //micro_sleep(5000000);
                }


                

                if (std::string(action) == "EFFETTUA LOGIN TRASPORTATORE"){
                    //UtenteTrasportatore trasportatore;

                    trasportatore.effettua_login(db1, nome_utente_trasportatore, password);

                    strcpy(outputs, "Login utente trasportatore avvenuta");

                    //freeReplyObject(reply);

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

                    /* sleep   */
                    //micro_sleep(5000000);
                }

                if (std::string(action) == "EFFETTUA LOGOUT TRASPORTATORE"){
                    //UtenteTrasportatore trasportatore;

                    trasportatore.effettua_logout(db1);

                    strcpy(outputs, "Logout utente trasportatore avvenuta");

                    //freeReplyObject(reply);

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

                    /* sleep   */
                    //micro_sleep(5000000);
                }

                if (std::string(action) == "ELIMINA PROFILO TRASPORTATORE"){
                    //UtenteTrasportatore trasportatore;

                    trasportatore.elimina_profilo(db1);

                    strcpy(outputs, "Eliminazione utente trasportatore avvenuta");

                    //freeReplyObject(reply);

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

                    /* sleep   */
                    //micro_sleep(5000000);
                }

                if (std::string(action) == "AGGIORNA NUMERO TELEFONO TRASPORTATORE"){
                    //UtenteTrasportatore trasportatore;

                    trasportatore.aggiornaNumeroDiTelefono(db1, nuovoNumeroTelefono);

                    strcpy(outputs, "Aggiornamento numero di telefono trasportatore");

                    //freeReplyObject(reply);

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

                    /* sleep   */
                    //micro_sleep(5000000);
                }

                if (std::string(action) == "AGGIORNA PASSWORD TRASPORTATORE"){
                    //UtenteTrasportatore trasportatore;

                    trasportatore.aggiornaPassword(db1, vecchiaPassw, nuovaPassw);

                    strcpy(outputs, "Aggiornamento password utente trasportatore");

                    //freeReplyObject(reply);

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

                    /* sleep   */
                    //micro_sleep(5000000);
                }

            }
        }

        freeReplyObject(reply);

        // send result to client
                    // send_counter++;
                    // sprintf(key, "Result");
                    // sprintf(value, "%s", outputs);

                    // printf("Result: %s \n", outputs);

                    // reply = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
                    // assertReplyType(c2r, reply, REDIS_REPLY_STRING);
                    // printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply->str);
                    // freeReplyObject(reply);

        // freeReplyObject(reply);


        // // send result to client
        // send_counter++;
        // sprintf(key, "Result");
        // sprintf(value, "%s", outputs);

        // reply = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM_TRASPORTATORE, key, value);
        // assertReplyType(c2r, reply, REDIS_REPLY_STRING);
        // printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM_TRASPORTATORE, key, value, reply->str);
        // freeReplyObject(reply);

        // /* sleep   */
        // micro_sleep(5000000);
        

    } // while ()


    redisFree(c2r);
}