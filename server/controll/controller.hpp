
#ifndef SERVER_CONTROLLER_HPP
#define SERVER_CONTROLLER_HPP

#include <iostream>
#include<cassert>
#include "json.hpp"
#include "../bd/dbconnector.hpp"

using json = nlohmann::json;
//enum Command{NotValid, AddNote, DeleteNote, ShowAllNotes};
class Message {
public:
    //Возвращает true, если все поля Message заполнены
    bool isFull();
    //Возвращает true, если команда верна (удалось записать команду)
    bool setCommand(std::string com);
    //Текст заметки для команды  /AddNote или аргумент команды /DeleteNote
    void setText(std::string text);
    //Ожидание ввода текста/номера
    bool isWaitingText();
    std::string getCommand();
    std::string getText();

private:
    //Имя заметки
    //std::string name;

    std::string command;

    //Если хранить список команд как enum
    //Command command;

    //Текст заметки/ номер заметки
    std::string text;
    //Message сформирован
    bool fullness = false;
    //Ожидание ввода текста (после того, как ввели команду /AddNote или /DeleteNote)

    // ХАРД КОД

    bool waitingText = true;

};

class Controller {

public:
    explicit Controller() = default;
    void parseJSON(const std::string& str);
    json reparseAnswer();

    int getID() { return chatID; }
    std::string returnText() { return  textMessage.getText(); }


    DBConnector bd;
private:
    int chatID = 0;
    Message textMessage;

    void setID(int id = 0) { chatID = id; }
    //Класс для связи с БД
    //Расшифровка команд в запросы SQL
    //string SwitchCommand()

};




#endif //SERVER_CONTROLLER_HPP
