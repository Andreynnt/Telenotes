#ifndef BOOSTING_REPLY_HPP
#define BOOSTING_REPLY_HPP

#include "../request/header.hpp"
#include <string>
#include <vector>
#include <boost/asio.hpp>

namespace http {

        /// Ответ клиенту
        struct reply {
            /// Статус ответа
            enum status_type {
                ok = 200,
                created = 201,
                accepted = 202,
                no_content = 204,
                multiple_choices = 300,
                moved_permanently = 301,
                moved_temporarily = 302,
                not_modified = 304,
                bad_request = 400,
                unauthorized = 401,
                forbidden = 403,
                not_found = 404,
                internal_server_error = 500,
                not_implemented = 501,
                bad_gateway = 502,
                service_unavailable = 503
            } status;

            /// HTTP headers для ответа
            std::vector<header> headers;

            /// Данные для ответа
            std::string content;

            int chatID;

            std::string get_content();

            /// Преобразование ответа в вектор <buffers>
            std::vector<boost::asio::const_buffer> to_buffers();

            /// Получение стандартного ответа
            static reply stock_reply(status_type status);
        };
}

#endif //BOOSTING_REPLY_HPP
