//
// Created by andreynt on 28.12.17.
//

#ifndef SERVER_CONF_READER_HPP
#define SERVER_CONF_READER_HPP


#include <iostream>

class serverConfig {
public:
    void readConfigFile(const std::string& file);

    int getPort() { return  port; }
    int getThreads() { return threads; }
    std::string getAddress() { return address; }

private:
    std::string address;
    int port = 8000;
    int threads = 1;
};



class databaseConfig {
public:
    void readConfigFile(const std::string& file);

    std::string getHostName() { return  hostName; }
    std::string getDataBaseName() { return dataBaseName; }
    std::string getUserName() { return userName; }
    std::string getPassword() { return password; }

private:
    std::string hostName;
    std::string dataBaseName;
    std::string userName;
    std::string password;
};


extern databaseConfig database;

#endif //SERVER_CONF_READER_HPP
