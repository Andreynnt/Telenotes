
#ifndef SERVER_CONTROLLER_HPP
#define SERVER_CONTROLLER_HPP

#include "json.hpp"
#include "../bd/dbconnector.hpp"
#include "../reply/reply.hpp"
#include "../core/queue.hpp"

#include <iostream>

using json = nlohmann::json;

class Message {
public:

    void setText(const std::string& Text) {text = Text; };
    void setName(const std::string& Name) {name = Name; };

    std::string getText() { return text; };
    std::string getName() { return name; }

private:
    //Текст заметки/ номер заметки
    std::string text;
    std::string name;
};




class Controller {

public:
    explicit Controller() = default;

    bool parseJSON(const std::string& str, int connection_id);
    std::string parseAndAnswer(http::reply& reply_, Queue& clientsQueue);

private:

    DBConnector bd;
    bool messageIsCommand();
    bool messageIsValidCommand();
    std::string returnText() { return textMessage.getText(); }
    std::string getName() { return textMessage.getName(); }

    int chatID = 0;
    int message_id = 0;
    Message textMessage;
};




#endif //SERVER_CONTROLLER_HPP
