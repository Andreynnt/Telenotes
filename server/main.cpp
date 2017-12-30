#include "core/server.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/xml_parser.hpp>



int main() {

    /// чтение конфига сервера из xml файла
    serverConfig serverConf;
    serverConf.readConfigFile("../config/server");

    /// чтение конфига базы данных из xml файла
    databaseConfig databaseConf;
    databaseConf.readConfigFile("../config/database");

    try {
        http::server s(serverConf.getAddress(), serverConf.getPort(), serverConf.getThreads());
        s.run();
    } catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    }

    return 0;
}