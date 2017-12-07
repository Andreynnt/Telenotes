//
// Created by andreynt on 24.11.17.
//

#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP


#include "connection.hpp"
#include "../request/request_handler.hpp"
#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace http {

    class server {

    public:
        ///Коснструктор, настраиваем на прослушивание тсп адреса и порта
        ///и отдачу файлов из заданной директории
        explicit server(const std::string& address, const std::string& port, const std::string& doc_root, std::size_t thread_pool_size);

        void run();

    private:
        /// Инициируем операцию асинхронного приема
        void start_accept();

        /// Обработка завершения операции асинхронного приема
        void handle_accept(const boost::system::error_code& e);

        /// Обработка запроса на остановку сервера
        void handle_stop();

        /// Количестов потоков, которые вызовуться run()
        std::size_t thread_pool_size_;

        /// Io_service для выполнения асинхронных операций
        boost::asio::io_service io_service_;

        /// Signal_set используется для уведомлений о завершении процессов
        boost::asio::signal_set signals_;

        ///Acceptor для прослушивания входящих соединений
        boost::asio::ip::tcp::acceptor acceptor_;

        /// Следующее соедениние
        connection_ptr new_connection_;

        /// Обработчик входящих запросов
        request_handler request_handler_;
    };
}



#endif //SERVER_SERVER_HPP
