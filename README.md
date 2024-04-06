# Backend sito e-commerce.
Il progetto si concentra sulla realizzazione di un backend per un sito e-commerce, progettato per gestire tre tipologie di utenti: Compratori, Fornitori e Trasportatori. 

Per garantire un'efficace gestione delle varie categorie di utenti, sono stati implementati tre tipi di server, ciascuno dedicato a una specifica categoria.

La comunicazione tra i vari processi avviene attraverso un middleware che sfrutta il database/framework NoSQL Redis, mentre i dati elaborati dal sito e-commerce sono archiviati nel database PostgreSQL.

Il sistema prevede che i diversi tipi di utenti possano inviare richieste ai server di categoria tramite streams Redis dedicate, utilizzando un'interfaccia web. I server, a loro volta, elaborano le richieste e inviano le risposte agli utenti tramite le appropriate streams Redis.

Per eseguire il programma è necessario trovarsi in un ambiente Unix, come Linux o macOS, che offre una vasta gamma di strumenti e utilità da riga di comando, fondamentali per lo sviluppo e l'esecuzione del software.


## Installazione Redis e PostgreSQL.
È inoltre richiesta l'installazione dei database NoSQL Redis e PostgreSQL. Una volta installati, è possibile procedere con l'esecuzione del programma.

Per l'installazione di Redis , in base all'ambiente in cui ci troviamo
.
.
.
.
.

Una volta installati i due framework possiamo procedere con l'esecuzione del programma.


## Configurazione pre-esecuzione del programma
Considerando che le cartelle dei client sono associate alle categorie di utenti che inviano richieste al server, se desideriamo eseguire test specifici, è necessario modificare i file main.cpp dei client per selezionare i test da eseguire. Inoltre, per decidere se avviare anche test casuali, è importante commentare l'if debug nella sezione corrispondente.

Per avviare il programma, è necessario aprire sei terminali e posizionarsi nelle cartelle bin dei server e dei client.

Prima di avviare i server, è necessario avviare il server Redis utilizzando il comando: ``` redis-server ```  


## Esecuzione del programma.
Successivamente, è possibile avviare i server e i client eseguendo il comando ``` ./main ``` . (prima avviare i server che restano in ascolto delle richieste dei client)

Durante l'esecuzione del programma, le comunicazioni tra client e server verranno visualizzate nei terminali. 


## Controllo dei risultati.
È possibile controllare i risultati delle operazioni in diversi modi: 

1. Ci connettiamo al database tramite terminale con il comando: ```psql -U sito_ecommerce -h localhost -d backend_sito_ecommerce1```. Dove :

    a. -U sito_ecommerce: specifica il nome utente (sito_ecommerce) con cui connettersi al database.

    b. -h localhost: specifica l'host (localhost) su cui si trova il database. In questo caso, il database è presumibilmente in esecuzione sulla stessa macchina locale.
  
    c. -d backend_sito_ecommerce1: specifica il nome del database (backend_sito_ecommerce1) a cui connettersi.

3. Controlliamo le richieste e le risposte del server nelle cartelle result dei vari client e server.

4. Monitorando direttamente i terminali in cui vengono eseguiti client e server.


## Modifiche codice programma.
Se intendi apportare modifiche al codice, assicurati sempre di compilare il programma prima di eseguirlo nuovamente. Ad esempio, se devi modificare il codice del server dei Compratori, posizionati nella cartella src e utilizza il comando make per compilare il programma. Allo stesso modo, se desideri apportare modifiche al codice del client dei Fornitori, posizionati nella cartella src e utilizza il comando make per compilare il programma.
