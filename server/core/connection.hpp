//
// Created by andreynt on 24.11.17.
//

#ifndef SERVER_CONNECTION_HPP
#define SERVER_CONNECTION_HPP

#include "../reply/reply.hpp"
#include "../request/request.hpp"
#include "../request/request_handler.hpp"
#include "../request/request_parser.hpp"

#include "../controll/controller.hpp"
#include "queue.hpp"

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>


namespace http {

    ///Одно соединение от клиента
    class connection : public boost::enable_shared_from_this<connection>, private boost::noncopyable {

    public:
        explicit connection(boost::asio::io_service& io_service, request_handler& handler, Queue& clientsQueue);

        /// Получение сокета, связанного с соединением
        boost::asio::ip::tcp::socket& socket();


        /// Запуск асинхронной операции соедиения
        void start();
        std::string getContent();

    private:
        void setContent(const std::string& str);


        /// Обработка завершения операции чтения
        void handle_read(const boost::system::error_code& e, std::size_t bytes_transferred);

        /// Обработка завершения операции записи
        void handle_write(const boost::system::error_code& e);

        /// Strand для обеспечения того, чтобы обработчики соединения не вызывались одновременно.
        boost::asio::io_service::strand strand_;

        /// Сокет для соединения
        boost::asio::ip::tcp::socket socket_;

        /// Обработчик входящего соединеия
        request_handler& request_handler_;

        /// Buffer для входящих данных
        boost::array<char, 32768> buffer_;

        std::string print_buffer(std::size_t bytes_transferred);

        /// Входящий запрос (request)
        request request_;

        /// Парсер для входящего запроса
        request_parser request_parser_;

        /// Ответ для клиента
        reply reply_;

        std::string content;

        Queue& clientsQueue;
    };

    typedef boost::shared_ptr<connection> connection_ptr;
}

#endif //SERVER_CONNECTION_HPP
