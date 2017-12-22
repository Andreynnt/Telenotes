
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

    //Текст заметки для команды  /AddNote или аргумент команды /DeleteNote
    void setText(const std::string& text);
    void setName(const std::string& text);
    void setTags(const std::string& tags);

    std::string getText();
    std::string getName() { return name; }
    std::string getTags() { return tags; }

private:
    //Текст заметки/ номер заметки
    std::string text;
    std::string name;
    std::string tags;

};





class Controller {

public:
    explicit Controller() = default;

    void parseJSON(const std::string& str);
    int getID() { return chatID; }
    void parseAndAnswer(http::reply& reply_, Queue& clientsQueue, std::string& answer);

    DBConnector bd;

private:

    void setMessageName(const std::string& text);
    bool messageIsCommand();
    bool messageIsValidCommand();

    std::string returnText() { return textMessage.getText(); }
    std::string getTags() { return textMessage.getTags(); }
    std::string getName() { return textMessage.getName(); }

    int chatID = 0;
    Message textMessage;

    void setID(int id = 0) { chatID = id; }
};




#endif //SERVER_CONTROLLER_HPP
