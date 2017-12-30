
#include "dbconnector.hpp"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDate>
#include <iostream>

#define MAX_NAME_SIZE 200


DBConnector::DBConnector(){
    QSqlDatabase newConnector;
    db_connector = newConnector;
}


DBConnector::~DBConnector(){
    QString conn;
    conn = db_connector.connectionName();
    db_connector.close();
    db_connector = QSqlDatabase();
    db_connector.removeDatabase(conn);
}


bool DBConnector::connectDB(){
    std::string id = std::to_string(message_id);
    db_connector = QSqlDatabase::addDatabase("QPSQL", id.c_str());

    db_connector.setHostName((config->getHostName()).c_str());
    db_connector.setDatabaseName((config->getDataBaseName()).c_str());
    db_connector.setUserName((config->getUserName()).c_str());
    db_connector.setPassword((config->getPassword()).c_str());

    if( !db_connector.open() ) {
        qDebug() <<"connection error"<< db_connector.lastError().text();
        return false;
    }
    return true;
}


void DBConnector::authorizeUser() {
    if(checkConnection()) {
        QSqlQuery query(db_connector);
        query.prepare(
                "SELECT * FROM users WHERE user_id=:userid;"
        );
        query.bindValue( ":userid", user_id );
        if( !query.exec() ){
            qDebug() <<"Select err"<<db_connector.lastError().text();
        }
        if(query.next() == false){//if there's on such user
            query.prepare(
                    "INSERT INTO users VALUES(:userid);"
            );
            query.bindValue( ":userid", user_id );
            if( !query.exec() ){
                qDebug() <<"Creating err"<<db_connector.lastError().text();
            }
        }
    }
}


bool DBConnector::checkConnection(){//checking the connection. If smth is wrong trying to reconnect.
    if(!db_connector.isOpen()){
        if(!connectDB()){
            qDebug()<<"Database connection error. Please, check the database server";
            return false;
        }
        return false;
    }
    return true;
}


void DBConnector::insertQuery(const std::string& note_name, const std::string& note_text) {
    if(checkConnection()) {
        QSqlQuery query(db_connector);
        QDate date = QDate::currentDate();

        query.prepare("INSERT INTO notes VALUES(:usrid, :name, :note, :date);");
        query.bindValue( ":usrid", user_id );
        query.bindValue( ":name", note_name.c_str());
        query.bindValue( ":note", note_text.c_str());
        query.bindValue(":date",date);

        if( !query.exec() ){
            qDebug() <<"Insert error"<<query.lastError().text();
        }
    }
}


std::string DBConnector::selectByNameQuery(const std::string& note_name) {
    if (checkConnection()){
        std::string savedNote;
        QSqlQuery query(db_connector);

        query.prepare(
                "SELECT * FROM notes WHERE user_id=:userid AND note_name=:name;"
        );
        query.bindValue( ":name", note_name.c_str() );
        query.bindValue( ":userid", user_id );

        if( !query.exec() ){
            qDebug() <<"Select err"<<db_connector.lastError().text();
        }

        bool noteInBd = false;
        while (query.next()) {
            savedNote += "Note's name: ";
            savedNote += query.value(1).toString().toStdString();
            savedNote += "\n\nText:\n";
            savedNote += query.value(2).toString().toStdString();
            savedNote += "\n\n";
            noteInBd = true;
        }
        if (!noteInBd) {
            // заметки с таким именем не было
            savedNote = R"(You haven't got a note with name: ")";
            if (note_name.length() > MAX_NAME_SIZE) {
                std::string shortName;
                shortName.assign(note_name, 0, MAX_NAME_SIZE);
                savedNote += shortName;
                savedNote.append(R"(...")");
            } else {
                savedNote += note_name;
                savedNote.append(R"(")");
            }

        }
        return savedNote;
    }
    else {
        return "\n";
    }

}


std::string DBConnector::selectAllQuery(){
    if(checkConnection()){
        QSqlQuery query(db_connector);
        query.prepare(
                "SELECT note_name FROM notes WHERE user_id=:userid;"
        );
        query.bindValue(":userid",user_id);
        if( !query.exec() ) {
            qDebug() <<"Select err"<<db_connector.lastError().text();
        }
        std::string names;

        int noteNumber = 1;
        while( query.next() ) {
            std::string name = std::to_string(noteNumber);
            name += ". ";
            name += query.value(0).toString().toStdString();
            names += name;
            names += "\n";
            noteNumber++;
        }
        names += "\n\n";
        return names;
    }
}


bool DBConnector::deleteByName(const std::string& note_name){
    if(checkConnection()){
        QSqlQuery query(db_connector);

        query.prepare("DELETE FROM notes WHERE note_name = :note_name");
        query.bindValue( ":note_name", note_name.c_str());

        if (!query.exec()) {
            qDebug() <<"Delete error"<<query.lastError().text();
            return false;
        }

        if (!query.numRowsAffected()) {
            return false;
        }

        return true;
    }
    return false;
}