
#include "queue.hpp"


int Queue::getClientStatus(int id) {

    auto iter = clients.find(id);

    if (iter == clients.end()) {
        return newClient;
    }

    std::string client = clients[id];

    if (client == "/add") {
        return waitingForNewNote;
    }
    if (client == "/show") {
        return waitingToShowNote;
    }
    if (client == "/show") {
        return waitingToShowAll;
    }
    if (client == "/delete") {
        return waitingForDelete;
    }

    return gotNameWaitingText;
}


void Queue::addClient(int id, const std::string& command) {
    clients[id] = command;
}


void Queue::deleteClient(int id){
    clients.erase(id);
}

std::string  Queue::getCommand(int chatID) {
    return clients[chatID];
}