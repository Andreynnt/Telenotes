#ifndef CLASS_DBCONNECTOR_H
#define CLASS_DBCONNECTOR_H
#include <QSqlDatabase>

class DBConnector{
public:
    DBConnector(int userid1);
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


#endif // CLASS_DBCONNECTOR_H
