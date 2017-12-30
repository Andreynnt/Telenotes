//
// Created by andreynt on 24.11.17.
//


#include "server.hpp"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>


namespace http {

    server::server(const std::string& address, int port, std::size_t thread_pool_size)
            : thread_pool_size_(thread_pool_size),
              signals_(io_service_),
              acceptor_(io_service_),
              new_connection_()

    {

        //Сигналы SIGTERM (завершить программу), SIGINT (прерывание сигнала),  SIGQUIT (сигнал, для остановки процесса пользователем)
        signals_.add(SIGINT);
        signals_.add(SIGTERM);
        signals_.add(SIGQUIT);
        signals_.async_wait(boost::bind(&server::handle_stop, this));

        //resolver нужен для того, чтобы преобразововывать человекочитаемый формат в
        //в endpoint, например 127.0.0.1 в 0x7F000001.
        boost::asio::ip::tcp::resolver resolver(io_service_);
        boost::asio::ip::tcp::resolver::query query(address, std::to_string(port));
        //Делаем "конечную точку", которую можно привязать к конкретному сокету
        boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

        acceptor_.open(endpoint.protocol());
        // Делаем acceptor с возможностью переиспользования
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();

        start_accept();
    }

    void server::run() {
        //Создание пула потоков
        std::vector<boost::shared_ptr<boost::thread>> threads;

        for (std::size_t i = 0; i < thread_pool_size_; ++i) {
            boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
            threads.push_back(thread);
        }

        for (std::size_t i = 0; i < threads.size(); ++i) {
            threads[i]->join();
        }
    }

    void server::start_accept() {
        new_connection_.reset(new connection(io_service_, request_handler_, clientsQueue));
        acceptor_.async_accept(new_connection_->socket(), boost::bind(&server::handle_accept, this, boost::asio::placeholders::error));
    }

    /// Обработчик, вызываемый при завершении операции accept
    void server::handle_accept(const boost::system::error_code& e) {
        if (!e) {
            /// уникальный id для каждого соединения
            new_connection_->start(connection_id);
            connection_id++;
        }

        start_accept();
    }

    void server::handle_stop() {
        io_service_.stop();
    }

}

