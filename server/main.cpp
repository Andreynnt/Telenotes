#include "core/server.hpp"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/lexical_cast.hpp>

int main(int argc, char* argv[]) {
    try {
        if (argc != 5) {
            std::cerr << "Usage: <address> <port> <threads> <doc_root>\n";
            std::cerr << "  For IPV4, try:\n";
            return 1;
        }

        //инициализируем сервер
        std::size_t num_threads = boost::lexical_cast<std::size_t>(argv[3]);
        http::server s(argv[1], argv[2], argv[4], num_threads);
        //http::server& ser =  http::server::getInstance(argv[1], argv[2], argv[4], num_threads);

        //запуск сервера
        s.run();

    } catch (std::exception& e) {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}