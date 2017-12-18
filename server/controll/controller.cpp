#include "controller.hpp"

struct Command {
    const char* command;
} Commands[] =
        {
                {"/add"},
                {"/delete"},
                {"/show"},
                {"/all"}
        };


//Проверка команды на правильность
bool isValidCommand(const std::string& com) {
    for (Command* i = Commands; i->command; i++) {
        if (com == i->command) {
            return true;
        }
    }
    return false;
}


std::string Message::getText() {
    return text;
}

void Message::setText(const std::string& Text) {
    text = Text;
}


void Message::setTags(const std::string& Tags) {
    tags = "";
}

void Message::setName(const std::string& text) {
    std::stringstream  stream(text);

    std::string  word, result;
    for (int i = 0; i < 3; i++) {
        stream >> word;
        if (word.length()) {
            word += " ";
        }
        result += word;
        word.clear();
    }

    // удаляем последний пробел
    result.pop_back();
    name = result;
}

void Controller::setMessageName() {
    textMessage.setName(textMessage.getText());
}

void Controller::parseJSON(const std::string& str) {
    json jsonToParse = json::parse(str);

    int id = jsonToParse["message"]["chat"]["id"];
    setID(id);

    std::string gotText;

    try {
        gotText = jsonToParse["message"]["text"];
    } catch (nlohmann::json::exception& a){
        gotText = "";
    }

    textMessage.setText(gotText);
}



bool Controller::messageIsCommand() {
    std::string fullMessage = textMessage.getText();
    if (fullMessage[0] == '/') {
        return true;
    }
    return false;
}

bool Controller::messageIsValidCommand() {
    for (Command* i = Commands; i->command; i++) {
        if (textMessage.getText() == i->command) {
            return true;
        }
    }
    return false;
}






std::string Controller::parseAndAnswer(http::reply& reply_, Queue& clientsQueue, std::string& answer) {

    reply_.chatID = chatID;

    if (messageIsCommand()) {
        // если пользователь отправил любое сообщение, которое начинается с '/'

        if (messageIsValidCommand()) {
            // если пользователь ввел команду из списка команд

            std::string command = textMessage.getText();
            int status = clientsQueue.getClientStatus(chatID);

            switch (status) {
                case newClient:
                    if (command == "/all") {
                        answer = bd.selectAllQuery();
                    }  else {
                        clientsQueue.addClient(chatID, command);
                    }
                    break;

                case waitingForNewNote:
                    answer = "Previous command was /add.\n Enter note";
                    break;

                case waitingToShowNote:
                    answer = "Previous command was /show.\n Enter valid note's name";
                    break;

                case waitingForDelete:
                    answer = "Previous command was /delete.\n Enter notes' name";
                    break;

            }
        } else {
            answer = "This command isn't valid.\nTry /add, /delete, /all, /show";
        }

    } else {

        // если пользователь ввел текст
        int status = clientsQueue.getClientStatus(chatID);
        switch (status) {
            case newClient:
                // когда нам отправляют картинку, видео или стикер
                if (textMessage.getText().empty()) {
                    answer = "Oh, I can work only with text notes.";
                } else {
                    answer = "Oh, you should try command from this list: /add, /delete, /show, /all";
                }
                break;
            case waitingForNewNote:
                setMessageName();
                bd.insertQuery(getName(), returnText(), getTags());
                answer = "Got it!";
                clientsQueue.deleteClient(getID());
                break;
            case waitingToShowNote:
                /// надо  проверку сделать
                answer = bd.selectByNameQuery(returnText());
                clientsQueue.deleteClient(getID());
                break;
            case waitingForDelete:
                /// надо  проверку сделать
                setMessageName();
                bd.deleteByName(getName());
                clientsQueue.deleteClient(getID());
                break;
        }
    }

    return answer;
};
