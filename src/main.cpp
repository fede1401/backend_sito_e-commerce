#include "svolgiAzione.h"

void initUSER(Con2DB db1, UtenteCompratore compratore, UtenteFornitore fornitore, UtenteTrasportatore trasportatore)
{

    // Array di nomi utente
    std::array<std::string, 100> nomi_utente;
    for (int i = 0; i < 100; ++i)
    {
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

    // Array di conferma password
    std::array<std::string, 100> confermaPassword;
    for (int i = 0; i < 100; ++i)
    {
        confermaPassword[i] = "P.assword" + std::to_string(i + 1);
    }

    // Array di nomi aziende produttrici
    std::array<std::string, 5> nomi_aziende_produttrici = {"Azienda1", "Azienda2", "Azienda3", "Azienda4", "Azienda5"};

    // Array di nomi ditte di spedizioni
    std::array<std::string, 5> nomi_ditte_spedizioni = {"Ditta1", "Ditta2", "Ditta3", "Ditta4", "Ditta5"};

    // Array di vie di residenza
    std::array<std::string, 5> vie_di_residenza = {"Via Roma", "Via Milano", "Via Napoli", "Via Firenze", "Via Venezia"};

    // Array di numeri civici
    std::array<std::string, 100> numeri_civici;
    for (int i = 0; i < 100; ++i)
    {
        numeri_civici[i] = std::to_string(i + 1);
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

    int i_100C = rand() % 100;
    int i_10C = rand() % 10;
    int i_5C = rand() % 5;

    compratore.effettuaRegistrazione(db1, nomi_utente[i_100C], "UtenteCompratore",
                                     nomi[i_10C], cognomi[i_10C],
                                     numeri_telefono[i_100C], email[i_100C],
                                     vie_di_residenza[i_5C], numeri_civici[i_100C], CAP[i_5C],
                                     citta_di_residenza[i_5C],
                                     password[i_100C], confermaPassword[i_100C],
                                     date_di_nascita[i_100C]);

    int i_100F = rand() % 100;
    int i_10F = rand() % 10;
    int i_5F = rand() % 5;

    fornitore.effettuaRegistrazione(db1, nomi_utente[i_100F], "UtenteFornitore",
                                    nomi[i_10F], cognomi[i_10F],
                                    numeri_telefono[i_100F], email[i_100F],
                                    password[i_100F], confermaPassword[i_100F],
                                    nomi_aziende_produttrici[i_5F]);

    int i_100T = rand() % 100;
    int i_10T = rand() % 10;
    int i_5T = rand() % 5;

    trasportatore.effettuaRegistrazione(db1, nomi_utente[i_100T], "UtenteTrasportatore",
                                        nomi[i_10T], cognomi[i_10T],
                                        numeri_telefono[i_100T], email[i_100T],
                                        password[i_100T], confermaPassword[i_100T],
                                        nomi_ditte_spedizioni[i_5T]);

    
    std::cout << "Registrazione effettuata" << std::endl;

    return;
}

int main()
{
    Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");
    std::cout << "Connessione al database avvenuta con successo." << std::endl;

    // PGresult *res;

    // char sqlcmd[1000];

    // test_generator testUtenti;

    // testUtenti.TestAzioniUtenti();
    // test_generator test2;
    // test2.TestCompletoNoUtenti();

    /* init random number generator  */
    srand((unsigned)time(NULL));

    // server_types x;

    // definito per il numero di iterazioni del while:
    int t = 0;

    int i = 0;

    // std::array<UtenteCompratore, 100> utentiCompratori;
    //  std::array<UtenteFornitore, 100> utentiFornitori;
    // std::array<UtenteTrasportatore, 100> utentiTrasportatori;

    UtenteCompratore compratore;
    UtenteFornitore fornitore;
    UtenteTrasportatore trasportatore;

    while (i < 20)
    {

         initUSER(db1, compratore, fornitore, trasportatore);

        // utentiCompratori[i] = compratore;
        // utentiFornitori[i] = fornitore;
        // utentiTrasportatori[i] = trasportatore;

        i++;

        /* sleep   */
        micro_sleep(1000);
    }

    print_select("UtenteCompratore");
    print_select("UtenteFornitore");
    print_select("UtenteTrasportatore");

    UtenteCompratore compratore1;
    UtenteFornitore fornitore1;
    UtenteTrasportatore trasportatore1;

    int n;
    while (t <= HORIZON)
    {

        n = rand() % 5;

        std::cout << "RAND" << n << std::endl;

        sprintf(sqlcmd, "SELECT * FROM UtenteCompratore");
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        compratore1.nome_utente = PQgetvalue(res, n, 0);
        compratore1.session_id = PQgetvalue(res, n, 1);
        compratore1.categoria = PQgetvalue(res, n, 2);
        compratore1.nome = PQgetvalue(res, n, 3);
        compratore1.cognome = PQgetvalue(res, n, 4);
        compratore1.email = PQgetvalue(res, n, 5);
        compratore1.numero_telefono = PQgetvalue(res, n, 6);
        compratore1.password = PQgetvalue(res, n, 7);
        compratore1.data_compleanno = PQgetvalue(res, n, 8);
        compratore1.via_residenza = PQgetvalue(res, n, 9);
        compratore1.numero_civico = PQgetvalue(res, n, 10);
        compratore1.CAP = PQgetvalue(res, n, 11);
        compratore1.città_residenza = PQgetvalue(res, n, 12);
        compratore1.saldo = atof(PQgetvalue(res, n, 13));
        compratore1.stato = atoi(PQgetvalue(res, n, 14));

        std::cout << compratore1.nome_utente << " " << compratore1.session_id << " " << compratore1.categoria << " " << compratore1.nome << " " << compratore1.cognome << " " << compratore1.email << " " << compratore1.numero_telefono << " " << compratore1.password << " " << compratore1.data_compleanno << " " << compratore1.via_residenza << " " << compratore1.numero_civico << " " << compratore1.CAP << " " << compratore1.città_residenza << " " << compratore1.saldo << " " << compratore1.stato << std::endl;


        sprintf(sqlcmd, "SELECT * FROM UtenteFornitore");
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        fornitore1.nome_utente = PQgetvalue(res, n, 0);
        fornitore1.session_id = PQgetvalue(res, n, 1);
        fornitore1.categoria = PQgetvalue(res, n, 2);
        fornitore1.nome = PQgetvalue(res, n, 3);
        fornitore1.cognome = PQgetvalue(res, n, 4);
        fornitore1.email = PQgetvalue(res, n, 5);
        fornitore1.numero_telefono = PQgetvalue(res, n, 6);
        fornitore1.password = PQgetvalue(res, n, 7);
        fornitore1.azienda_produzione = PQgetvalue(res, n, 8);
        fornitore1.stato = atoi(PQgetvalue(res, n, 9));

        sprintf(sqlcmd, "SELECT * FROM UtenteTrasportatore");
        res = db1.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);
        PQclear(res);

        trasportatore1.nome_utente = PQgetvalue(res, n, 0);
        trasportatore1.session_id = PQgetvalue(res, n, 1);
        trasportatore1.categoria = PQgetvalue(res, n, 2);
        trasportatore1.nome = PQgetvalue(res, n, 3);
        trasportatore1.cognome = PQgetvalue(res, n, 4);
        trasportatore1.email = PQgetvalue(res, n, 5);
        trasportatore1.numero_telefono = PQgetvalue(res, n,6);
        trasportatore1.password = PQgetvalue(res, n, 7);
        trasportatore1.ditta_spedizione = PQgetvalue(res, n, 8);
        trasportatore1.stato = atoi(PQgetvalue(res, n, 9));
        trasportatore1.disponibilità = atoi(PQgetvalue(res, n, 10));

        int index_azioneDaSvolgere = rand() % 34;

        svolgiAzione(db1, compratore1, trasportatore1, fornitore1, index_azioneDaSvolgere);

        std::cout << "t: " << t << std::endl;

        // update time
        t++;

        /* sleep   */
        micro_sleep(500);
    }

    print_select("LogTable");
}