//
// Created by andreynt on 09.12.17.
//

#ifndef QT_DBCONNECTOR_HPP
#define QT_DBCONNECTOR_HPP


#include <QSqlDatabase>
#include "../config_reader/conf_reader.hpp"


class DBConnector {
public:
    DBConnector();
    ~DBConnector();
    bool connectDB();
    void authorizeUser();

    void setID(const int id) { user_id = id; };
    void setMessageID(const int id) { message_id = id; };

    void insertQuery(const std::string& note_name, const std::string& note_text);
    std::string selectByNameQuery(const std::string& note_name);
    std::string selectAllQuery();
    bool deleteByName(const std::string& note_name);

    databaseConfig* config = &database;
private:
    // уникальный номер запроса для соедения с базой данных
    int message_id = 0;
    // уникальный id клиента
    int user_id = 0;
    QSqlDatabase db_connector;
    bool checkConnection();
};

#endif //QT_DBCONNECTOR_HPP
