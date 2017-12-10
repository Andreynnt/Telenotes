//
// Created by andreynt on 09.12.17.
//

#ifndef QT_DBCONNECTOR_HPP
#define QT_DBCONNECTOR_HPP

#include <QSqlDatabase>

class DBConnector{
public:
    DBConnector(int userid1 = 278);
    ~DBConnector();
    bool connectDB();
    void authorizeUser();
    void insertQuery(char* note_name, char* note_text, char* note_tags);
    std::string selectByNameQuery(char* note_name);
    std::string selectAllQuery();

private:
    int user_id;
    QSqlDatabase db_connector;
    bool checkConnection();
};

#endif //QT_DBCONNECTOR_HPP
