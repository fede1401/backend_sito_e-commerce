
-- database already exists

\c :dbname 


CREATE TYPE motivazioneReso AS ENUM ('difettoso', 'misura errata', 'non conforme alle aspettative', 'cambio opinione');

CREATE TYPE votoStelle AS ENUM ('1', '2', '3', '4', '5');

CREATE TYPE statoConsegna AS ENUM ('in elaborazione', 'spedito', 'in transito', 'consegnato');

/*CREATE TYPE categoriaUtente AS ENUM ('UtenteCompratore', 'UtenteFornitore', 'UtenteTrasportatore');*/


/*
Eliminazione tabelle:
DROP TABLE UtenteCompratore CASCADE;
DROP TABLE AziendaProd CASCADE;
DROP TABLE DittaSp CASCADE;
DROP TABLE UtenteFornitore CASCADE;
DROP TABLE UtenteTrasportatore CASCADE;
DROP TABLE Carta CASCADE;
DROP TABLE Prodotto CASCADE;
DROP TABLE Carrello CASCADE;
DROP TABLE ListaDesideri CASCADE;
DROP TABLE Reso CASCADE;
DROP TABLE Recensione CASCADE;
DROP TABLE AssistenzaClienti CASCADE;
DROP TABLE Ordine CASCADE;
*/


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
    nome_utente_compratore VARCHAR(50) NOT NULL UNIQUE,
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
    categoriaUtente VARCHAR(50) NOT NULL,
    nome VARCHAR(50) NOT NULL,
    cognome VARCHAR(50) NOT NULL,
    indirizzo_mail VARCHAR(100) NOT NULL UNIQUE,
    numero_di_telefono VARCHAR(20),
    password VARCHAR(100) NOT NULL,
    nome_DittaSpedizione VARCHAR(50) NOT NULL,
    stato INTEGER,
    PRIMARY KEY(nome_utente_trasportatore)
);


CREATE TABLE IF NOT EXISTS Carta (
    idCarta SERIAL ,
    nome_utente_compratore VARCHAR(50) NOT NULL UNIQUE,
    numeroCarta VARCHAR(20) NOT NULL,
    cvv VARCHAR(3) NOT NULL,
    PRIMARY KEY(idCarta),
    CONSTRAINT fk_nome_utente_compratore
      FOREIGN KEY(nome_utente_compratore)
      REFERENCES UtenteCompratore(nome_utente_compratore)
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
	CONSTRAINT fk_nome_utente_compratore
      FOREIGN KEY(nome_utente_compratore)
      REFERENCES UtenteCompratore(nome_utente_compratore),
    CONSTRAINT fk_codProdotto
      FOREIGN KEY(codProdotto)
      REFERENCES Prodotto(codProdotto)
);


CREATE TABLE IF NOT EXISTS ListaDesideri (
	nome_utente_compratore VARCHAR(50) NOT NULL,
	codProdotto SERIAL,
	CONSTRAINT fk_nome_utente_compratore
      FOREIGN KEY(nome_utente_compratore)
      REFERENCES UtenteCompratore(nome_utente_compratore),
    CONSTRAINT fk_codProdotto
      FOREIGN KEY(codProdotto)
      REFERENCES Prodotto(codProdotto)
);


CREATE TABLE IF NOT EXISTS Reso (
        idReso SERIAL,
        nome_utente_compratore VARCHAR(50) NOT NULL,
        codProdotto SERIAL,
        motivazioneReso motivazioneReso,
        PRIMARY KEY(idReso),
        CONSTRAINT fk_nome_utente_compratore
      FOREIGN KEY(nome_utente_compratore)
      REFERENCES UtenteCompratore(nome_utente_compratore),
    CONSTRAINT fk_codProdotto
      FOREIGN KEY(codProdotto)
      REFERENCES Prodotto(codProdotto)
);



CREATE TABLE IF NOT EXISTS Recensione (
        idRec SERIAL,
        nome_utente_compratore VARCHAR(50) NOT NULL,
        codProdotto SERIAL,
        descrizione TEXT,
        votoStelle votoStelle, 
        PRIMARY KEY(idRec),
        CONSTRAINT fk_nome_utente_compratore
      FOREIGN KEY(nome_utente_compratore)
      REFERENCES UtenteCompratore(nome_utente_compratore),
    CONSTRAINT fk_codProdotto
      FOREIGN KEY(codProdotto)
      REFERENCES Prodotto(codProdotto)
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
      REFERENCES UtenteCompratore(nome_utente_compratore),
  CONSTRAINT fk_nome_utente_trasportatore
      FOREIGN KEY(nome_utente_trasportatore)
      REFERENCES UtenteTrasportatore(nome_utente_trasportatore),
  CONSTRAINT fk_nome_utente_fornitore
      FOREIGN KEY(nome_utente_fornitore)
      REFERENCES UtenteFornitore(nome_utente_fornitore)
  
);



CREATE TABLE IF NOT EXISTS Ordine (
    idOrdine SERIAL,
    codProdotto SERIAL,
    nome_utente_compratore VARCHAR(50) NOT NULL,
    nome_utente_trasportatore VARCHAR(50) NOT NULL,
    dataOrdineEffettuato DATE,
    statoConsegna statoConsegna,
    nome_DittaSpedizione VARCHAR(50) NOT NULL,
    viaSpedizione VARCHAR(50) NOT NULL,
    cittaSpedizione VARCHAR(50) NOT NULL,
    numCivSpedizione VARCHAR(10) NOT NULL,    
    PRIMARY KEY(idOrdine),
    CONSTRAINT fk_codProdotto
      FOREIGN KEY(codProdotto)
      REFERENCES Prodotto(codProdotto),
    CONSTRAINT fk_nome_utente_compratore
      FOREIGN KEY(nome_utente_compratore)
      REFERENCES UtenteCompratore(nome_utente_compratore),
    CONSTRAINT fk_nome_utente_trasportatore
      FOREIGN KEY(nome_utente_trasportatore)
      REFERENCES UtenteTrasportatore(nome_utente_trasportatore)
);

/*CREATE TABLE IF NOT EXISTS UtenteCompratore (
    idUtComp SERIAL,
    categoriaUtente VARCHAR(50) NOT NULL,
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
    nome VARCHAR(50) NOT NULL UNIQUE,
    PRIMARY KEY(idAziendaProd)
);


CREATE TABLE IF NOT EXISTS DittaSp (
    idDittaSp SERIAL,
    nome VARCHAR(50) NOT NULL UNIQUE,
    PRIMARY KEY(idDittaSp)
);


CREATE TABLE IF NOT EXISTS UtenteFornitore (
    idUtForn SERIAL,
    categoriaUtente VARCHAR(50) NOT NULL,
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
    categoriaUtente VARCHAR(50) NOT NULL,
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


CREATE TABLE IF NOT EXISTS AssistenzaClienti (
	idChiamata SERIAL,
  idUtComp SERIAL,
  idUtForn SERIAL,
  idUtTrasp SERIAL,
  motivazioneChiamata VARCHAR(50) NOT NULL,
  numeroTelefonoAssistenza VARCHAR(20) NOT NULL
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
*/

