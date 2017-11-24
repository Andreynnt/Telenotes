#include "request_handler.hpp"
#include "../reply/mime_types.hpp"
#include "../reply/reply.hpp"
#include "request.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <iostream>


namespace http {

        request_handler::request_handler(const std::string& doc_root) : doc_root_(doc_root) {

        }

        void request_handler::handle_request(const request& req, reply& rep) {
            // Декодинг пути к папке
            std::string request_path;
            request_path = "/index.html";

            ///Сюда можно вставить парсинг входящей папки

            // Открываем файл для отправки обатно
            std::string full_path = doc_root_ + request_path;
            std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
            if (!is) {
                //Папка не открыта
                rep = reply::stock_reply(reply::not_found);
                return;
            }

            // Заполнение ответа для клиента
            rep.status = reply::ok;
            char buf[512];
            // Поблочное считывание
            while (is.read(buf, sizeof(buf)).gcount() > 0) {
                rep.content.append(buf, is.gcount());
            }

            rep.headers.resize(2);
            rep.headers[0].name = "Content-Length";
            rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());

            rep.headers[1].name = "Content-Type";
            rep.headers[1].value = mime_types::extension_to_type("json");
        }
}
