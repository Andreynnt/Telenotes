//
// Created by andreynt on 09.12.17.
//

#ifndef QT_DBCONNECTOR_HPP
#define QT_DBCONNECTOR_HPP

#include <QSqlDatabase>

class DBConnector {
public:
    DBConnector();
    ~DBConnector();
    bool connectDB();
    void authorizeUser();

    void setID(const int id) { user_id = id; };
    void insertQuery(const std::string& note_name, const std::string& note_text, const std::string& note_tags);
    std::string selectByNameQuery(const std::string& note_name);
    std::string selectAllQuery();
    bool deleteByName(const std::string& note_name);

private:
    int user_id = 0;
    QSqlDatabase db_connector;
    bool checkConnection();
};

#endif //QT_DBCONNECTOR_HPP
