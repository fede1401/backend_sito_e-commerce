
-- database already exists

\c :dbname 


CREATE TYPE motivazioneReso AS ENUM ('difettoso', 'misura errata', 'non conforme alle aspettative', 'cambio opinione');

CREATE TYPE votoStelle AS ENUM ('1', '2', '3', '4', '5');

CREATE TYPE statoConsegna AS ENUM ('in elaborazione', 'spedito', 'in transito', 'consegnato');




/*
CREATE TABLE IF NOT EXISTS UtenteCompratore (
    idUtComp SERIAL,
    nome_utente VARCHAR(50) NOT NULL UNIQUE,
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
    stato BOOLEAN,
    PRIMARY KEY(idUtComp)
);


CREATE TABLE IF NOT EXISTS UtenteFornitore (
    idUtForn SERIAL,
    nome_utente VARCHAR(50) NOT NULL UNIQUE,
    nome VARCHAR(50) NOT NULL,
    cognome VARCHAR(50) NOT NULL,
    indirizzo_mail VARCHAR(100) NOT NULL UNIQUE,
    numero_di_telefono VARCHAR(20),
    password VARCHAR(100) NOT NULL,
    idAziendaProduzione VARCHAR(100) NOT NULL,
    stato BOOLEAN,
    PRIMARY KEY(idUtForn)
);


CREATE TABLE IF NOT EXISTS UtenteTrasportatore (
    idUtTrasp SERIAL,
    nome_utente VARCHAR(50) NOT NULL UNIQUE,
    nome VARCHAR(50) NOT NULL,
    cognome VARCHAR(50) NOT NULL,
    indirizzo_mail VARCHAR(100) NOT NULL UNIQUE,
    numero_di_telefono VARCHAR(20),
    password VARCHAR(100) NOT NULL,
    dittaSpedizione VARCHAR(100) NOT NULL,
    stato BOOLEAN,
    PRIMARY KEY(idUtTrasp)
);


CREATE TABLE IF NOT EXISTS Carta (
    idCarta SERIAL ,
    idUtComp SERIAL,
    numeroCarta VARCHAR(20) NOT NULL,
    cvv VARCHAR(3) NOT NULL,
    PRIMARY KEY(idCarta),
    CONSTRAINT fk_idUtComp FOREIGN KEY(idUtComp) REFERENCES UtenteCompratore(idUtComp)
);


CREATE TABLE IF NOT EXISTS Prodotto (
	codProdotto SERIAL,
	nome VARCHAR(50) NOT NULL,
	categoria VARCHAR(50) NOT NULL,
	prezzoEuro DECIMAL(10, 2),
	aziendaProduzione VARCHAR(50),
	num_copie_dispo INTEGER,
	PRIMARY KEY(codProdotto)
);


CREATE TABLE IF NOT EXISTS Carrello (
	idUtComp SERIAL,
	codProdotto SERIAL,
	CONSTRAINT fk_idUtComp FOREIGN KEY(idUtComp) REFERENCES UtenteCompratore(idUtComp),
    CONSTRAINT fk_codProdotto FOREIGN KEY(codProdotto) REFERENCES Prodotto(codProdotto)
);


CREATE TABLE IF NOT EXISTS ListaDesideri (
	idUtComp SERIAL,
	codProdotto SERIAL,
	CONSTRAINT fk_idUtComp FOREIGN KEY(idUtComp) REFERENCES UtenteCompratore(idUtComp),
    CONSTRAINT fk_codProdotto FOREIGN KEY(codProdotto) REFERENCES Prodotto(codProdotto)
);



CREATE TABLE IF NOT EXISTS Reso (
        idReso SERIAL,
        idUtComp SERIAL,
        codProdotto SERIAL,
        motivazioneReso motivazioneReso,
        PRIMARY KEY(idReso),
        CONSTRAINT fk_idUtComp FOREIGN KEY(idUtComp) REFERENCES UtenteCompratore(idUtComp),
        CONSTRAINT fk_codProdotto FOREIGN KEY(codProdotto) REFERENCES Prodotto(codProdotto)
);




CREATE TABLE IF NOT EXISTS Recensione (
        idRec SERIAL,
        idUtComp SERIAL,
        codProdotto SERIAL,
        descrizione TEXT,
        votoStelle votoStelle, 
        PRIMARY KEY(idRec),
        CONSTRAINT fk_idUtComp FOREIGN KEY(idUtComp) REFERENCES UtenteCompratore(idUtComp),
        CONSTRAINT fk_codProdotto FOREIGN KEY(codProdotto) REFERENCES Prodotto(codProdotto)
);




CREATE TABLE IF NOT EXISTS AssistenzaClienti (
	numeroTelefono VARCHAR(20) NOT NULL
);



CREATE TABLE IF NOT EXISTS Ordine (
        idOrdine SERIAL,
        codProdotto SERIAL,
        idUtComp SERIAL,
        idUtTrasp SERIAL,
        dataOrdineEffettuato DATE,
        statoConsegna statoConsegna,
        dittaSpedizione VARCHAR(100),
        viaSpedizione VARCHAR(50) NOT NULL,
        cittaSpedizione VARCHAR(50) NOT NULL,
        numCivSpedizione VARCHAR(10) NOT NULL,
        PRIMARY KEY(idOrdine),
        CONSTRAINT fk_codProdotto FOREIGN KEY(codProdotto) REFERENCES Prodotto(codProdotto),
        CONSTRAINT fk_idUtComp FOREIGN KEY(idUtComp) REFERENCES UtenteCompratore(idUtComp),
        CONSTRAINT fk_idUtTrasp FOREIGN KEY(idUtTrasp) REFERENCES UtenteTrasportatore(idUtTrasp) 
);

*/



CREATE TABLE IF NOT EXISTS UtenteCompratore (
    idUtComp SERIAL,
    nome_utente VARCHAR(50) NOT NULL UNIQUE,
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
    PRIMARY KEY(idUtComp)
);

CREATE TABLE IF NOT EXISTS AziendaProd (
    idAziendaProd SERIAL,
    nome VARCHAR(50) NOT NULL,
    PRIMARY KEY(idAziendaProd)
);


CREATE TABLE IF NOT EXISTS DittaSp (
    idDittaSp SERIAL,
    nome VARCHAR(50) NOT NULL,
    PRIMARY KEY(idDittaSp)
);


CREATE TABLE IF NOT EXISTS UtenteFornitore (
    idUtForn SERIAL,
    nome_utente VARCHAR(50) NOT NULL UNIQUE,
    nome VARCHAR(50) NOT NULL,
    cognome VARCHAR(50) NOT NULL,
    indirizzo_mail VARCHAR(100) NOT NULL UNIQUE,
    numero_di_telefono VARCHAR(20),
    password VARCHAR(100) NOT NULL,
    idAziendaProd SERIAL,
    stato INTEGER,
    PRIMARY KEY(idUtForn),
    CONSTRAINT fk_idAziendaProd
      FOREIGN KEY(idAziendaProd)
      REFERENCES AziendaProd(idAziendaProd)
);


CREATE TABLE IF NOT EXISTS UtenteTrasportatore (
    idUtTrasp SERIAL,
    nome_utente VARCHAR(50) NOT NULL UNIQUE,
    nome VARCHAR(50) NOT NULL,
    cognome VARCHAR(50) NOT NULL,
    indirizzo_mail VARCHAR(100) NOT NULL UNIQUE,
    numero_di_telefono VARCHAR(20),
    password VARCHAR(100) NOT NULL,
    idDittaSp SERIAL,
    stato INTEGER,
    PRIMARY KEY(idUtTrasp),
    CONSTRAINT fk_idDittaSp
      FOREIGN KEY(idDittaSp)
      REFERENCES DittaSp(idDittaSp)
);


CREATE TABLE IF NOT EXISTS Carta (
    idCarta SERIAL ,
    idUtComp SERIAL,
    numeroCarta VARCHAR(20) NOT NULL,
    cvv VARCHAR(3) NOT NULL,
    PRIMARY KEY(idCarta),
    CONSTRAINT fk_idUtComp
      FOREIGN KEY(idUtComp)
      REFERENCES UtenteCompratore(idUtComp)
);


CREATE TABLE IF NOT EXISTS Prodotto (
	codProdotto SERIAL,
	nome VARCHAR(50) NOT NULL,
	categoria VARCHAR(50) NOT NULL,
	prezzoEuro DECIMAL(10, 2),
	idAziendaProd SERIAL,
	num_copie_dispo INTEGER,
	PRIMARY KEY(codProdotto),
    CONSTRAINT fk_idAziendaProd
      FOREIGN KEY(idAziendaProd)
      REFERENCES AziendaProd(idAziendaProd)
);


CREATE TABLE IF NOT EXISTS AziendaProd (
    idAziendaProd SERIAL,
    nome VARCHAR(50) NOT NULL,
    PRIMARY KEY(idAziendaProd)
);


CREATE TABLE IF NOT EXISTS Carrello (
	idUtComp SERIAL,
	codProdotto SERIAL,
	CONSTRAINT fk_idUtComp
      FOREIGN KEY(idUtComp)
      REFERENCES UtenteCompratore(idUtComp),
    CONSTRAINT fk_codProdotto
      FOREIGN KEY(codProdotto)
      REFERENCES Prodotto(codProdotto)
);


CREATE TABLE IF NOT EXISTS ListaDesideri (
	idUtComp SERIAL,
	codProdotto SERIAL,
	CONSTRAINT fk_idUtComp
      FOREIGN KEY(idUtComp)
      REFERENCES UtenteCompratore(idUtComp),
    CONSTRAINT fk_codProdotto
      FOREIGN KEY(codProdotto)
      REFERENCES Prodotto(codProdotto)
);


CREATE TABLE IF NOT EXISTS Reso (
        idReso SERIAL,
        idUtComp SERIAL,
        codProdotto SERIAL,
        motivazioneReso motivazioneReso,
        PRIMARY KEY(idReso),
        CONSTRAINT fk_idUtComp
      FOREIGN KEY(idUtComp)
      REFERENCES UtenteCompratore(idUtComp),
    CONSTRAINT fk_codProdotto
      FOREIGN KEY(codProdotto)
      REFERENCES Prodotto(codProdotto)
);



CREATE TABLE IF NOT EXISTS Recensione (
        idRec SERIAL,
        idUtComp SERIAL,
        codProdotto SERIAL,
        descrizione TEXT,
        votoStelle votoStelle, 
        PRIMARY KEY(idRec),
        CONSTRAINT fk_idUtComp
      FOREIGN KEY(idUtComp)
      REFERENCES UtenteCompratore(idUtComp),
    CONSTRAINT fk_codProdotto
      FOREIGN KEY(codProdotto)
      REFERENCES Prodotto(codProdotto)
);


\d

CREATE TABLE IF NOT EXISTS AssistenzaClienti (
	numeroTelefono VARCHAR(20) NOT NULL
);



CREATE TABLE IF NOT EXISTS Ordine (
        idOrdine SERIAL,
        codProdotto SERIAL,
        idUtComp SERIAL,
        idUtTrasp SERIAL,
        dataOrdineEffettuato DATE,
        statoConsegna statoConsegna,
        idDittaSp SERIAL,
        viaSpedizione VARCHAR(50) NOT NULL,
        cittaSpedizione VARCHAR(50) NOT NULL,
        numCivSpedizione VARCHAR(10) NOT NULL,
        PRIMARY KEY(idOrdine),
        CONSTRAINT fk_codProdotto
      FOREIGN KEY(codProdotto)
      REFERENCES Prodotto(codProdotto),
    CONSTRAINT fk_idUtComp
      FOREIGN KEY(idUtComp)
      REFERENCES UtenteCompratore(idUtComp),
    CONSTRAINT fk_idUtTrasp
      FOREIGN KEY(idUtTrasp)
      REFERENCES UtenteTrasportatore(idUtTrasp),
    CONSTRAINT fk_idDittaSp
      FOREIGN KEY(idDittaSp)
      REFERENCES DittaSp(idDittaSp) 
);