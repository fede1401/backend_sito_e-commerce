// Recensione.h
#ifndef RECENSIONE_H
#define RECENSIONE_H

#include <string>


enum class votoStelle {
    Uno,
    Due,
    Tre,
    Quattro,
    Cinque
  };


class Recensione {
public:
    int idRecensione;
    std::string nome_utente_compratore;
    int idOrdine;
    std::string descrizione;
    votoStelle voto_stella;

    // Costruttori:
    Recensione():
      idRecensione(-1),
      nome_utente_compratore(""),
      idOrdine(-1),
      descrizione(""),
      voto_stella(){}

    Recensione(int idRecensione, std::string nome_utente_compratore, int idOrdine, 
            std::string descrizione, votoStelle voto_stella):
         
            idRecensione(idRecensione), nome_utente_compratore(nome_utente_compratore),
            idOrdine(idOrdine), 
            descrizione(descrizione), voto_stella(voto_stella) {}


    
    
    void effettuaRecensione(int idOrdine, std::string descrizione, votoStelle voto_stella){

      // Connession al database:
      Con2DB db1("localhost", "5432", "sito_ecommerce", "47002", "backend_sito_ecommerce1");

      std::string stato_spedizione;
        
      // Innanzitutto controllo se l'ordine è stato spedito e arrivato correttamente
      sprintf(sqlcmd, "SELECT statoSpedizione FROM Spedizione WHERE idOrdine = '%d'", idOrdine);
      res = db1.ExecSQLtuples(sqlcmd);
      rows = PQntuples(res);

      if (rows==1){
          stato_spedizione = PQgetvalue(res, 0, PQfnumber(res, "statoSpedizione"));

          if (stato_spedizione == "consegnato"){
                
              // Selezioniamo il nome del'utente compratore:
              std::string nome_utente_compratore;
              sprintf(sqlcmd, "SELECT nome_utente_compratore FROM Ordine WHERE idOrdine = '%d'", idOrdine);
              res = db1.ExecSQLtuples(sqlcmd);
              rows = PQntuples(res);
              if (rows == 1){
                  nome_utente_compratore = PQgetvalue(res, 0, PQfnumber(res, "nome_utente_compratore"));
                    
                  std::string votoStelleStr = statoVotoStelleToString(voto_stella);
                    
                  sprintf(sqlcmd, "INSERT INTO Recensione (idRec, nome_utente_compratore, idOrdine, descrizione, votoStelle) VALUES (DEFAULT, '%s', '%d', '%s', '%s')", 
                  nome_utente_compratore.c_str(), idOrdine, descrizione.c_str(),  votoStelleStr.c_str());
                  res = db1.ExecSQLcmd(sqlcmd);
                  PQclear(res); 
              }
              else{
                  std::cout << "L'ordine non è stato trovato!" << std::endl;
                  return;
              }
          }
          else{
              std::cout << "L'ordine è stato spedito, ma non è ancora arrivato, perciò non può essere effettuata la recensione!" << std::endl;
              return;
            }
      }
      else{
          std::cout << "L'ordine non è stato ancora spedito, perciò non può essere effettuata la recensione!" << std::endl;
          return;
      }
    std::cout << "Recensione effettuata" << std::endl;
    return;
    }


    std::string statoVotoStelleToString(votoStelle stato) {
        switch (stato) {
            case votoStelle::Uno :
                return "1";
            case votoStelle::Due :
                return "2";
            case votoStelle::Tre :
                return "3";
            case votoStelle::Quattro :
                return "4";
            case votoStelle::Cinque :
                return "5";
            default:
                return ""; // gestione degli errori o valori non validi
        }
    }





};

#endif // RECENSIONE_H
