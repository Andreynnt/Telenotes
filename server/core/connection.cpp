
#include "../request/request_handler.hpp"
#include "connection.hpp"
#include <iostream>
#include <vector>
#include <boost/bind.hpp>

namespace http {

    connection::connection(boost::asio::io_service& io_service, request_handler& handler, Queue& clientsQueue)
            : strand_(io_service),
              socket_(io_service),
              request_handler_(handler),
              clientsQueue(clientsQueue)
    {
    }


    boost::asio::ip::tcp::socket& connection::socket() {
        return socket_;
    }


    void connection::start() {
        //wrap - создание нового обработчика, который автоматически отправит обработанный обработчик в strand
        socket_.async_read_some(boost::asio::buffer(buffer_), strand_.wrap(
                boost::bind(&connection::handle_read, shared_from_this(),
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred)));
    }


    std::string connection::print_buffer(std::size_t bytes_transferred) {
        std::string my_string(buffer_.data(), buffer_.data() + bytes_transferred);
        std::cout << my_string << std::endl;
    }


    void connection::handle_read(const boost::system::error_code& e, std::size_t bytes_transferred) {
        if (!e) {

            //std::cout << "bytes trans = " << bytes_transferred << std::endl;

            boost::tribool result;
            //tie создает кортеж с неконстантыми ссылками
            boost::tie(result, boost::tuples::ignore) = request_parser_.parse(request_, buffer_.data(), buffer_.data() + bytes_transferred);

            if (result) {

                std::string my_string(buffer_.data(), buffer_.data() + bytes_transferred);
                setContent(my_string);

                Controller controller;
                controller.parseJSON(getContent());
                controller.bd.setID(controller.getID());

                std::string answer;
                // логика бота в этом методе
                controller.parseAndAnswer(reply_, clientsQueue, answer);

                request_handler_.handle_request(request_, reply_, answer);

                boost::asio::async_write(socket_, reply_.to_buffers(),
                                         strand_.wrap(boost::bind(&connection::handle_write, shared_from_this(),
                                                                  boost::asio::placeholders::error)));
            }

            else if (!result) {
                reply_ = reply::stock_reply(reply::bad_request);
                boost::asio::async_write(socket_, reply_.to_buffers(),
                                         strand_.wrap(
                                                 boost::bind(&connection::handle_write, shared_from_this(),
                                                             boost::asio::placeholders::error)));
            } else {
                socket_.async_read_some(boost::asio::buffer(buffer_),
                                        strand_.wrap(
                                                boost::bind(&connection::handle_read, shared_from_this(),
                                                            boost::asio::placeholders::error,
                                                            boost::asio::placeholders::bytes_transferred)));
            }
        }

        // Если возникает ошибка, новые асинхронные операции не запускаются. Это
        // означает, что все shared_ptr на объект класса connection
        // исчезнут, и объекты будет уничтоженны автоматически
        // Деструктор connection закрывает сокет.
    }


    void connection::handle_write(const boost::system::error_code& e) {
        if (!e) {
            boost::system::error_code ignored_ec;
            socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
        }
    }


    std::string connection::getContent() {
        return content;
    }


    void connection::setContent(const std::string& str) {
        size_t pos = str.find('{');
        content = str.substr(pos, std::string::npos);
    }

}

