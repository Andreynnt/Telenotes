#include "controller.hpp"

//Проверка команды на правильность
bool isValidCommand(std::string com) {
    if(com == "/AddNote" || com == "/DeleteNote" || com == "/ShowAllNotes") {
        return true;
    }
    return false;
}

//Для DeleteNote - является ли строка числом(номером заметки)
bool isNumber(std::string str) {
    for(int i=0;str[i];i++) {
        if(!(str[i]>='0' && str[i]<='9'))
            return false;
    }
    return true;
}

std::string Message::getCommand() {
    if(isValidCommand(command))return command;
    return "None";
}

std::string Message::getText() {
    assert(isFull());
    return text;
}

bool Message::setCommand(std::string com) {

    if(isValidCommand(com)) {
        this->command = com;
        this->waitingText = true;
        if(com =="/ShowAllNotes") {
            this->waitingText = false;
            this->fullness = true;
        }
        return true;
    }
    return false;
}

bool Message::isFull() {
    return fullness;
}

bool Message::isWaitingText() {
    return waitingText;
}

void Message::setText(std::string text) {
    if(!(command=="/DeleteNote")||isNumber(text)) {
        this->text = text;
        this->fullness = true;
        this->waitingText = false;
    }

}



void Controller::parseJSON(const std::string& str) {
    json jsonToParse = json::parse(str);

    std::string gotText = jsonToParse["message"]["text"];
    int id = jsonToParse["message"]["chat"]["id"];
    setID(id);

    //Если флаг ожидания текста = false, значит была введена команда
    //т.к изначально isWaitingText = false
    if(!textMessage.isWaitingText()) {
        textMessage.setCommand(gotText);
    } else {
        textMessage.setText(gotText);
    }

}

json Controller::reparseAnswer() {
    json jsonAnswer;
    jsonAnswer["chat_id"]=chatID;
    if(textMessage.getCommand()=="/AddNote" && textMessage.isWaitingText()) {
        jsonAnswer["text"]="Вы ввели команду /AddNote. Введите текст заметки";
    }
    if(textMessage.getCommand()=="/DeleteNote" && textMessage.isWaitingText()) {
        jsonAnswer["text"]="Вы ввели команду /DeleteNote. Введите номер заметки";
    }
    if(textMessage.getCommand()=="None") {
        jsonAnswer["text"]="Вы ввели неврную команду. Список доступных команд: /AddNote, /DeleteNote, /ShowAllNotes";
    }
    //Команда введена верно, текст/номер заметки введен
    if(textMessage.isFull()) {
        jsonAnswer["text"]="Выполнено успешно! "+textMessage.getText();
    }
    return jsonAnswer;

};