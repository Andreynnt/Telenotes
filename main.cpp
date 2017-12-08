#include <iostream>
#include<cassert>
#include "json.hpp"
using json = nlohmann::json;
//enum Command{NotValid, AddNote, DeleteNote, ShowAllNotes};
class Message
{
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
    bool waitingText = false;

};

//Проверка команды на правильность
bool isValidCommand(std::string com)
{

    if(com == "/AddNote" || com == "/DeleteNote" || com == "/ShowAllNotes") return true;
    return false;

}

//Для DeleteNote - является ли строка числом(номером заметки)
bool isNumber(std::string str)
{
    for(int i=0;str[i];i++)
    {
        if(!(str[i]>='0' && str[i]<='9'))
            return false;
    }
    return true;
}

std::string Message::getCommand()
{
    if(isValidCommand(command))return command;
    return "None";
}

std::string Message::getText()
{
    assert(isFull());
    return text;
}

bool Message::setCommand(std::string com)
{


        if(isValidCommand(com))
        {
            this->command = com;
            this->waitingText = true;
            if(com =="/ShowAllNotes")
            {
                this->waitingText = false;
                this->fullness = true;
            }
            return true;
        }
    return false;
}

bool Message::isFull()
{
    return fullness;
}

bool Message::isWaitingText()
{
    return waitingText;
}

void Message::setText(std::string text)
{
    if(!(command=="/DeleteNote")||isNumber(text))
    {
        this->text = text;
        this->fullness = true;
        this->waitingText = false;
    }

}

class Controller
{

public:
    explicit Controller(int id)
    {
        chatID = id;
    }

    void parseJSON(json jsonToParse);
    json reparseAnswer();

private:
    int chatID;
    Message textMessage;

    //Класс для связи с БД
    //bdConn bd;
    //Расшифровка команд в запросы SQL
    //string SwitchCommand()

};

//
void Controller::parseJSON(json jsonToParse)
{
    std::string gotText = jsonToParse["message"]["text"];

    //Если флаг ожидания текста = false, значит была введена команда
    //т.к изначально isWaitingText = false
    if(!textMessage.isWaitingText())
    {
        textMessage.setCommand(gotText);
    }
    else
    {
        textMessage.setText(gotText);
    }

}
json Controller::reparseAnswer()
{
    json jsonAnswer;
    jsonAnswer["chat_id"]=chatID;
    if(textMessage.getCommand()=="/AddNote" && textMessage.isWaitingText())
    {
        jsonAnswer["text"]="Вы ввели команду /AddNote. Введите текст заметки";
    }
    if(textMessage.getCommand()=="/DeleteNote" && textMessage.isWaitingText())
    {
        jsonAnswer["text"]="Вы ввели команду /DeleteNote. Введите номер заметки";
    }
    if(textMessage.getCommand()=="None")
    {
        jsonAnswer["text"]="Вы ввели неврную команду. Список доступных команд: /AddNote, /DeleteNote, /ShowAllNotes";
    }
    //Команда введена верно, текст/номер заметки введен
    if(textMessage.isFull())
    {
        jsonAnswer["text"]="Выполнено успешно! "+textMessage.getText();
    }
    return jsonAnswer;

};

int main()
{
    json j1 = R"({"update_id":869449690,
                    "message":
                 {"message_id":1081,
                    "from":
                 {"id":225462148,
                  "is_bot":false,
                  "first_name":"Andrey\ud83c\ude35",
                  "username":"Andreynnt",
                  "language_code":"en"},
                     "chat":
                  {"id":225462148,
                   "first_name":"Andrey\ud83c\ude35",
                   "username":"Andreynnt",
                   "type":"private"},

                   "date":1510990738,
                   "text":"/Add"}})"_json;

    std::cout<<j1["message"]["text"]<<std::endl;
    int chatID = j1["message"]["chat"]["id"];

    Controller controller(chatID);
    controller.parseJSON(j1);
    std::cout<<controller.reparseAnswer()<<std::endl;

    json j2 = R"({"update_id":869449690,
                    "message":
                 {"message_id":1081,
                    "from":
                 {"id":225462148,
                  "is_bot":false,
                  "first_name":"Andrey\ud83c\ude35",
                  "username":"Andreynnt",
                  "language_code":"en"},
                     "chat":
                  {"id":225462148,
                   "first_name":"Andrey\ud83c\ude35",
                   "username":"Andreynnt",
                   "type":"private"},

                   "date":1510990738,
                   "text":"/ShowAllNotes"}})"_json;

    std::cout<<j2["message"]["text"]<<std::endl;
    controller.parseJSON(j2);
    std::cout<<controller.reparseAnswer()<<std::endl;

    json j3 = R"({"update_id":869449690,
                    "message":
                 {"message_id":1081,
                    "from":
                 {"id":225462148,
                  "is_bot":false,
                  "first_name":"Andrey\ud83c\ude35",
                  "username":"Andreynnt",
                  "language_code":"en"},
                     "chat":
                  {"id":225462148,
                   "first_name":"Andrey\ud83c\ude35",
                   "username":"Andreynnt",
                   "type":"private"},

                   "date":1510990738,
                   "text":"Text of the note"}})"_json;

    std::cout<<j3["message"]["text"]<<std::endl;
    controller.parseJSON(j3);
    std::cout<<controller.reparseAnswer()<<std::endl;

    return 0;
}