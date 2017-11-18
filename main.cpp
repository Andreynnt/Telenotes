#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDate>

void connectDB(QSqlDatabase connector){
    connector = QSqlDatabase::addDatabase("QPSQL");
    connector.setHostName("localhost");
    connector.setDatabaseName("mydb");
    connector.setUserName("host1");
    connector.setPassword("host1");
    if( !connector.open() ) {
        qDebug() <<"conn error"<< connector.lastError().text();

    }
}
void insertQuery(QSqlDatabase connector, int userid, char* text){
    QSqlQuery query(connector);
    query.prepare(
               "INSERT INTO test( some_id, some_text) VALUES( :usrid, :note );"
    );
    query.bindValue( ":usrid", userid );
    query.bindValue( ":note", text);
    if( !query.exec() ) {
        qDebug() <<"Insert error"<<connector.lastError().text();
    }
}

void selectQuery(QSqlDatabase connector, int userid){
    QSqlQuery query(connector);
    query.prepare(
               "SELECT * FROM test ;"//WHERE some_id=:usrid
    );
    //query.bindValue( ":usrid", userid );
    if( !query.exec() ){
        qDebug() <<"Select err"<<connector.lastError().text();
    }
    QSqlRecord rec = query.record();
    //const int loginIndex = rec.indexOf( "some_id" );
    while( query.next() ) {
        int id = query.value(0).toInt();
        QString note = query.value(1).toString();
        qDebug()<< id <<"\t"<< note;
    }
}


int main() {
    QSqlDatabase db;
    connectDB(db);
    insertQuery(db,11011,"lala lalala");
    selectQuery(db, 12345);
    return 0;
}


