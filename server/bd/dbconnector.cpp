//
// Created by andreynt on 09.12.17.
//


#include "dbconnector.hpp"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDate>
#include <ostream>

void masterScript(){
    //Script for creating database.
    //Making DB on localhost
    //Server should contain:
    //-PostgreSQL Server.
    //-Postgres master user with name 'host1' and password 'host1'
    //Postgres DB named 'mydb'
    QSqlDatabase connector = QSqlDatabase::addDatabase("QPSQL");

    connector.setHostName("localhost");
    connector.setDatabaseName("mydb");
    connector.setUserName("host1");
    connector.setPassword("host1");

    if( !connector.open() ) {
        qDebug() <<"conn error"<< connector.lastError().text();
    }
    else{
        QSqlQuery query(connector);
        if(!query.exec("CREATE TABLE users( user_id int primary key);"))
            qDebug() <<"conn error"<< connector.lastError().text();
        if(!query.exec("CREATE TABLE notes(user_id int references users,note_name text,note_text text,note_tags text,note_date date);"))
            qDebug() <<"conn error"<< connector.lastError().text();
    }
    connector.close();

}

DBConnector::DBConnector(int userid1){
    user_id = userid1;
    QSqlDatabase newConnector;
    db_connector = newConnector;
}

DBConnector::~DBConnector(){
    db_connector.close();
}

bool DBConnector::connectDB(){
    db_connector = QSqlDatabase::addDatabase("QPSQL");

    db_connector.setHostName("localhost");
    db_connector.setDatabaseName("telenotesdb");
    db_connector.setUserName("host1");
    db_connector.setPassword("host1");

    if( !db_connector.open() ) {
        qDebug() <<"connection error"<< db_connector.lastError().text();
        return false;
    }
    return true;
}

void DBConnector::authorizeUser(){
    if(checkConnection()){
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

void DBConnector::insertQuery(char* note_name, char* note_text, char* note_tags){
    if(checkConnection()){
        QSqlQuery query(db_connector);
        QDate date = QDate::currentDate();

        query.prepare("INSERT INTO notes VALUES(:usrid, :name, :note, :tags, :date);");
        query.bindValue( ":usrid", user_id );
        query.bindValue( ":name", note_name );
        query.bindValue( ":note", note_text);
        query.bindValue( ":tags", note_tags);
        query.bindValue(":date",date);
        if( !query.exec() ){
            qDebug() <<"Insert error"<<query.lastError().text();
        }
    }
}

std::string DBConnector::selectByNameQuery( char* note_name){
    if(checkConnection()){
        std::string savedNote;
        QSqlQuery query(db_connector);

        query.prepare(
                "SELECT * FROM notes WHERE user_id=:userid AND note_name=:name;"
        );
        query.bindValue( ":name", note_name );
        query.bindValue( ":userid", user_id );
        if( !query.exec() ){
            qDebug() <<"Select err"<<db_connector.lastError().text();
        }
        while( query.next() ) {
            savedNote += query.value(1).toString().toStdString();
            savedNote += "\n\n";
            savedNote += query.value(2).toString().toStdString();
            savedNote += "\n TAGS: \n";
            savedNote += query.value(3).toString().toStdString();
            savedNote += "\n";
        }
        return savedNote;
    }
    else
        return "\n";

}

std::string DBConnector::selectAllQuery(){
    if(checkConnection()){
        QSqlQuery query(db_connector);
        query.prepare(
                "SELECT note_name FROM notes WHERE user_id=:userid;"
        );
        query.bindValue(":userid",user_id);
        if( !query.exec() ){
            qDebug() <<"Select err"<<db_connector.lastError().text();
        }
        std::string names;
        while( query.next() ) {
            std::string name = query.value(0).toString().toStdString();
            names += name;
            names += ", ";
        }
        names += "\n\n";
        return names;
    }
}