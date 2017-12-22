#ifndef SERVER_QUEUE_HPP
#define SERVER_QUEUE_HPP

#include <iostream>
#include <unordered_map>


enum {
    newClient,
    waitingForNewNote,
    waitingForDelete,
    waitingToShowNote,
    waitingToShowAll,
    gotNameWaitingText
};


class Queue {

private:
    std::unordered_map<int, std::string> clients;

public:
    int getClientStatus(int id);
    void addClient(int id, const std::string& command);
    void deleteClient(int id);
    std::string getCommand(int chatID);
};


#endif //SERVER_QUEUE_HPP
