
-- connettersi al database dbname

\c :dbname postgres

-- user already exists
GRANT ALL PRIVILEGES ON DATABASE :dbname to :username ;


ALTER TABLE UtenteCompratore OWNER TO :username ;
ALTER TABLE UtenteFornitore OWNER TO :username ;
ALTER TABLE UtenteTrasportatore OWNER TO :username ;
ALTER TABLE Carta OWNER TO :username ;
ALTER TABLE Prodotto OWNER TO :username ;
ALTER TABLE Carrello OWNER TO :username ;
ALTER TABLE ListaDesideri OWNER TO :username ;
ALTER TABLE Reso OWNER TO :username ;
ALTER TABLE Recensione OWNER TO :username ;
ALTER TABLE AssistenzaClienti OWNER TO :username ;
ALTER TABLE Ordine OWNER TO :username ;
--ALTER TABLE AziendaProd OWNER TO :username;
--ALTER TABLE DittaSp OWNER TO :username;


GRANT ALL ON SCHEMA public TO :username ;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO :username ;
