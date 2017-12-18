#include "request_handler.hpp"
#include "../reply/mime_types.hpp"
#include "../reply/reply.hpp"
#include "request.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <iomanip>


namespace http {

        request_handler::request_handler(const std::string& doc_root) : doc_root_(doc_root) {

        }


        void request_handler::handle_request(const request& req, reply& rep, const std::string& answer) {

            // Заполнение ответа для клиента
            rep.status = reply::ok;

            std::string jsonContent = "{";
            jsonContent += R"("method":"sendMessage","chat_id":")";
            //std::string jsonContent="{\"method\":\"sendMessage\",\"chat_id\":\"";

            jsonContent += std::to_string(rep.chatID);

            std::stringstream newAnswer;
            newAnswer << std::quoted(answer);

            jsonContent += R"(","text":)";

            jsonContent += newAnswer.str();
            jsonContent += "}";

            rep.content.append(jsonContent);

            //std::cout << "rep.content = " << rep.content << std::endl;

            rep.headers.resize(2);
            rep.headers[0].name = "Content-Length";
            rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());

            rep.headers[1].name = "Content-Type";
            rep.headers[1].value = mime_types::extension_to_type("json");
        }
}
