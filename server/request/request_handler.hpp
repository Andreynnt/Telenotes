#ifndef BOOSTING_REQUEST_HANDLER_HPP
#define BOOSTING_REQUEST_HANDLER_HPP

#include <string>
#include <boost/noncopyable.hpp>

namespace http {

        struct reply;
        struct request;

        class request_handler : private boost::noncopyable {
        public:

            explicit request_handler(const std::string& doc_root);

            /// Обрабатка запроса и отдача ответа
            void handle_request(const request& req, reply& rep);

        private:
            /// Дирректория с файлами для отдачи
            std::string doc_root_;
        };
}


#endif //BOOSTING_REQUEST_HANDLER_HPP
