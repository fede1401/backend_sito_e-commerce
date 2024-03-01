// Connessione c++ con postgreSQL

#include <iostream>
#include <pqxx/pqxx>

/*int main(){
    try{
        pqxx::connection c("host=localhost port=5432 dbname=backend_e_commerce user=postgres password=1414");
        if (c.is_open()){
            std::cout << "Opened database successfully: " << c.dbname() << std::endl;
        } else {
            std::cout << "Can't open database" << std::endl;
            return 1;
        }
        c.disconnect();
    } catch (const std::exception &e){
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
*/
