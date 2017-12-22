#include "controller.hpp"

struct Command {
    const char* command;
} Commands[] =
        {
                {"/add"},
                {"/delete"},
                {"/show"},
                {"/all"},
                {"/start"}
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
    name = text;
}

void Controller::setMessageName(const std::string& text) {
    textMessage.setName(text);
}

void Controller::parseJSON(const std::string& str) {

    std::string gotText;

    json jsonToParse = json::parse(str);

    int id = jsonToParse["message"]["chat"]["id"];
    setID(id);

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






void Controller::parseAndAnswer(http::reply& reply_, Queue& clientsQueue, std::string& answer) {

    reply_.chatID = chatID;

    if (textMessage.getText().empty()) {
        answer = "Oh, I can work only with text notes.";
        clientsQueue.deleteClient(chatID);
        return;
    }

    if (messageIsCommand()) {

        // если пользователь отправил любое сообщение, которое начинается с '/'
        if (!messageIsValidCommand()) {
            answer = "This command isn't valid.\nTry /add, /delete, /all, /show";
            return;
        }

        // если пользователь ввел команду из списка команд
        std::string command = textMessage.getText();
        int status = clientsQueue.getClientStatus(chatID);

        if (status == newClient) {
            if (command == "/all") {
                bd.connectDB();
                answer = bd.selectAllQuery();
            }  else if (command == "/start") {
                answer = "Welcome to Telenotes bot! Here you can create, delete and watch your text notes.";
            } else {
                clientsQueue.addClient(chatID, command);
                if (command == "/add") {
                    answer = "Enter note's name";
                    return;
                }
                answer = "Enter note's name";
            }
        } else if (status == waitingForDelete){
            answer = "Previous command was /delete.\n Enter note's name";
        } else if (status == waitingToShowNote) {
            answer = "Previous command was /show.\n Enter note's name";
        } else if (status == gotNameWaitingText){
            answer = "Previous command was /add.\n Enter note's text";
        } else {
            answer = "Previous command was /add.\n Enter note's name";
        }
    } else {

        // если пользователь ввел текст
        int status = clientsQueue.getClientStatus(chatID);
        switch (status) {
            case newClient:
                // когда нам отправляют картинку, видео или стикер
                answer = "Oh, you should try command from this list: /add, /delete, /show, /all";
                break;
            case waitingForNewNote:
                clientsQueue.addClient(chatID, textMessage.getText());
                answer = "Now enter note";
                break;

            case gotNameWaitingText:
                bd.connectDB();
                bd.insertQuery(clientsQueue.getCommand(chatID), returnText(), getTags());
                answer = "Got it!";
                clientsQueue.deleteClient(chatID);
                break;

            case waitingToShowNote:
                bd.connectDB();
                answer = bd.selectByNameQuery(returnText());
                clientsQueue.deleteClient(chatID);
                break;
            case waitingForDelete:
                bd.connectDB();

            std::string name = textMessage.getText();
                if (!bd.deleteByName(name)) {
                    answer = R"(You haven't got a note with name ")";
                    answer += name;
                    answer.append(R"(")");
                } else {
                    answer = R"(Deleted!)";
                }
                clientsQueue.deleteClient(chatID);
                break;
        }
    }

};
