
-- database already exists

\c :dbname 


CREATE TYPE motivazioneReso AS ENUM ('difettoso', 'misura errata', 'non conforme alle aspettative', 'cambio opinione');

CREATE TYPE votoStelle AS ENUM ('1', '2', '3', '4', '5');

CREATE TYPE statoOrdine AS ENUM ('in elaborazione', 'spedito', 'annullato');

CREATE TYPE statoSpedizione AS ENUM ('in transito', 'consegnato');

CREATE TYPE statoLog AS ENUM ('INFO', 'WARNING', 'ERROR');



CREATE TABLE IF NOT EXISTS UtenteCompratore (
    nome_utente_compratore VARCHAR(50) NOT NULL UNIQUE,
    session_id_c VARCHAR(50) UNIQUE,
    categoriaUtente VARCHAR(50) NOT NULL,
    nome VARCHAR(50) NOT NULL,
    cognome VARCHAR(50) NOT NULL,
    indirizzo_mail VARCHAR(100) NOT NULL UNIQUE,
    numero_di_telefono VARCHAR(20),
    password VARCHAR(100) NOT NULL,
    data_compleanno DATE,
    via_di_residenza VARCHAR(100),
    numero_civico VARCHAR(10),
    CAP VARCHAR(10),
    citta_di_residenza VARCHAR(50),
    saldo DECIMAL(10, 2),
    stato INTEGER,
    PRIMARY KEY(nome_utente_compratore)
);



CREATE TABLE IF NOT EXISTS UtenteFornitore (
    nome_utente_fornitore VARCHAR(50) NOT NULL UNIQUE,
    session_id_f VARCHAR(50) UNIQUE,
    categoriaUtente VARCHAR(50) NOT NULL,
    nome VARCHAR(50) NOT NULL,
    cognome VARCHAR(50) NOT NULL,
    indirizzo_mail VARCHAR(100) NOT NULL UNIQUE,
    numero_di_telefono VARCHAR(20),
    password VARCHAR(100) NOT NULL,
    nome_AziendaProduttrice VARCHAR(50) NOT NULL,
    stato INTEGER,
    PRIMARY KEY(nome_utente_fornitore)
);


CREATE TABLE IF NOT EXISTS UtenteTrasportatore (
    nome_utente_trasportatore VARCHAR(50) NOT NULL UNIQUE,
    session_id_t VARCHAR(50) UNIQUE,
    categoriaUtente VARCHAR(50) NOT NULL,
    nome VARCHAR(50) NOT NULL,
    cognome VARCHAR(50) NOT NULL,
    indirizzo_mail VARCHAR(100) NOT NULL UNIQUE,
    numero_di_telefono VARCHAR(20),
    password VARCHAR(100) NOT NULL,
    nome_DittaSpedizione VARCHAR(50) NOT NULL,
    stato INTEGER,
    dispo INTEGER,
    PRIMARY KEY(nome_utente_trasportatore)
);


CREATE TABLE IF NOT EXISTS Carta (
    idCarta SERIAL ,
    nome_utente_compratore VARCHAR(50) NOT NULL,
    numeroCarta VARCHAR(20) NOT NULL UNIQUE,
    cvv VARCHAR(3) NOT NULL,
    PRIMARY KEY(idCarta),
    CONSTRAINT fk_nome_utente_compratore
      FOREIGN KEY(nome_utente_compratore)
      REFERENCES UtenteCompratore(nome_utente_compratore)
      ON DELETE CASCADE
);


CREATE TABLE IF NOT EXISTS Prodotto (
	codProdotto SERIAL,
	nome VARCHAR(50) NOT NULL,
	categoria VARCHAR(50) NOT NULL,
  descrizione TEXT,
	prezzoEuro DECIMAL(10, 2),
	nome_AziendaProduttrice VARCHAR(50) NOT NULL,
	num_copie_dispo INTEGER,
	PRIMARY KEY(codProdotto)
  
);


CREATE TABLE IF NOT EXISTS Carrello (
	nome_utente_compratore VARCHAR(50) NOT NULL,
	codProdotto SERIAL,
  quantitàProd INTEGER,
	CONSTRAINT fk_nome_utente_compratore
      FOREIGN KEY(nome_utente_compratore)
      REFERENCES UtenteCompratore(nome_utente_compratore)
      ON DELETE CASCADE,
    CONSTRAINT fk_codProdotto
      FOREIGN KEY(codProdotto)
      REFERENCES Prodotto(codProdotto)
      ON DELETE CASCADE
);


CREATE TABLE IF NOT EXISTS ListaDesideri (
	nome_utente_compratore VARCHAR(50) NOT NULL,
	codProdotto SERIAL,
  quantitàProd INTEGER,
	CONSTRAINT fk_nome_utente_compratore
      FOREIGN KEY(nome_utente_compratore)
      REFERENCES UtenteCompratore(nome_utente_compratore)
      ON DELETE CASCADE,
    CONSTRAINT fk_codProdotto
      FOREIGN KEY(codProdotto)
      REFERENCES Prodotto(codProdotto)
      ON DELETE CASCADE
);



CREATE TABLE IF NOT EXISTS AssistenzaClienti (
	idChiamata SERIAL,
  nome_utente_compratore VARCHAR(50) NOT NULL,
  nome_utente_fornitore VARCHAR(50) NOT NULL,
  nome_utente_trasportatore VARCHAR(50) NOT NULL,
  motivazioneChiamata VARCHAR(50) NOT NULL,
  numeroTelefonoAssistenza VARCHAR(20) NOT NULL,
  PRIMARY KEY(idChiamata),
  CONSTRAINT fk_nome_utente_compratore
      FOREIGN KEY(nome_utente_compratore)
      REFERENCES UtenteCompratore(nome_utente_compratore)
      ON DELETE CASCADE,
  CONSTRAINT fk_nome_utente_trasportatore
      FOREIGN KEY(nome_utente_trasportatore)
      REFERENCES UtenteTrasportatore(nome_utente_trasportatore)
      ON DELETE CASCADE,
  CONSTRAINT fk_nome_utente_fornitore
      FOREIGN KEY(nome_utente_fornitore)
      REFERENCES UtenteFornitore(nome_utente_fornitore)
      ON DELETE CASCADE
);



CREATE TABLE IF NOT EXISTS Ordine (
    idOrdine SERIAL,
    codProdotto SERIAL,
    nome_utente_compratore VARCHAR(50) NOT NULL,
    dataOrdineEffettuato DATE,
    statoOrdine statoOrdine,
    viaSpedizione VARCHAR(50) NOT NULL,
    cittaSpedizione VARCHAR(50) NOT NULL,
    numCivSpedizione VARCHAR(10) NOT NULL, 
    PRIMARY KEY(idOrdine),
    CONSTRAINT fk_codProdotto
      FOREIGN KEY(codProdotto)
      REFERENCES Prodotto(codProdotto)
      ON DELETE CASCADE,
    CONSTRAINT fk_nome_utente_compratore
      FOREIGN KEY(nome_utente_compratore)
      REFERENCES UtenteCompratore(nome_utente_compratore)
      ON DELETE CASCADE
);



CREATE TABLE IF NOT EXISTS Spedizione (
    idSpedizione SERIAL,
    idOrdine SERIAL,
    nome_utente_trasportatore VARCHAR(50) NOT NULL,
    statoSpedizione statoSpedizione,
    nome_DittaSpedizione VARCHAR(50) NOT NULL,
    PRIMARY KEY(idSpedizione),
    CONSTRAINT fk_idOrdine
      FOREIGN KEY(idOrdine)
      REFERENCES Ordine(idOrdine)
      ON DELETE CASCADE,
    CONSTRAINT fk_nome_utente_trasportatore
      FOREIGN KEY(nome_utente_trasportatore)
      REFERENCES UtenteTrasportatore(nome_utente_trasportatore)
      ON DELETE CASCADE
);


CREATE TABLE IF NOT EXISTS Reso (
        idReso SERIAL,
        nome_utente_compratore VARCHAR(50) NOT NULL,
        idOrdine SERIAL,
        motivazioneReso motivazioneReso,
        PRIMARY KEY(idReso),
        UNIQUE(nome_utente_compratore, idOrdine),
        CONSTRAINT fk_nome_utente_compratore
      FOREIGN KEY(nome_utente_compratore)
      REFERENCES UtenteCompratore(nome_utente_compratore)
      ON DELETE CASCADE,
    CONSTRAINT fk_idOrdine
      FOREIGN KEY(idOrdine)
      REFERENCES Ordine(idOrdine)
      ON DELETE CASCADE
);



CREATE TABLE IF NOT EXISTS Recensione (
        idRec SERIAL,
        nome_utente_compratore VARCHAR(50) NOT NULL,
        idOrdine SERIAL,
        descrizione TEXT,
        votoStelle votoStelle, 
        PRIMARY KEY(idRec),
        UNIQUE(nome_utente_compratore, idOrdine),
        CONSTRAINT fk_nome_utente_compratore
      FOREIGN KEY(nome_utente_compratore)
      REFERENCES UtenteCompratore(nome_utente_compratore)
      ON DELETE CASCADE,
    CONSTRAINT fk_idOrdine
      FOREIGN KEY(idOrdine)
      REFERENCES Ordine(idOrdine)
      ON DELETE CASCADE
);


CREATE TABLE IF NOT EXISTS LogTable (
    timevalue VARCHAR NOT NULL,		      -- time of reading YYYY-MM-DD hh-mm-ss
    pid int NOT NULL,                     -- pid of process logged
    statoLog statoLog NOT NULL,
    messaggio TEXT,
    sessionId VARCHAR(50) NOT NULL--,
    -- PRIMARY KEY(timevalue, pid, sessionId)
);

