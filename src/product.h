// Product.h
#ifndef PRODUCT_H
#define PRODUCT_H


#include <iostream>
#include <string>

class Product {
    public:
    std::string cod_product;
    std::string nome;
    std::string categoria;
    float prezzo_euro;
    std::string descrizione;
    std::string azienda_produzione;
    int numero_copie_disponibili;

    Product(std::string cod_product, std::string nome, std::string categoria, 
           float prezzo_euro, std::string descrizione, std::string azienda_produzione, int numero_copie_disponibili)
        : cod_product(cod_product), nome(nome), categoria(categoria),
          prezzo_euro(prezzo_euro), descrizione(descrizione), azienda_produzione(azienda_produzione), numero_copie_disponibili(numero_copie_disponibili) {}

    void mostraInformazioni() {
        std::cout << "Codice prodotto: " << cod_product << std::endl;
        std::cout << "Nome: " << nome << std::endl;
        std::cout << "Categoria: " << categoria << std::endl;
        std::cout << "Prezzo in euro: " << prezzo_euro << std::endl;
        std::cout << "Descrizione: " << descrizione << std::endl;
        std::cout << "Azienda produzione: " << azienda_produzione << std::endl;
        std::cout << "Numero copie disponibili: " << numero_copie_disponibili << std::endl;
    }
};


/*
int main() {
    // Esempi di utilizzo delle classi derivate

    Product prodotto1("A-01", "Iphone 15", "Telefonia", 859.99, "Iphone color nero, 8 GB Ram, 512 GB archiviazione", "Apple", 150);

    // Visualizzazione delle informazioni specifiche per ciascun tipo di utente
    prodotto1.mostraInformazioni();

    return 0;
}
*/

#endif // PRODUCT_H
