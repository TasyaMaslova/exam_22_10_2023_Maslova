#ifndef DATABASE_H
#define DATABASE_H

#include "singleton.h"
#include <QVector>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

class DataBase : public Singleton<DataBase>
{
    friend class Singleton<DataBase>;
    private:
        QSqlDatabase db;
        DataBase(){
            this->db = QSqlDatabase::addDatabase("QSQLITE");
            this->db.setDatabaseName("Test.db");

            if(!this->db.open())
                qDebug()<<this->db.lastError().text();

            QSqlQuery query(this->db);
            query.exec("CREATE TABLE IF NOT EXISTS Users("
                       "id INTEGER PRIMARY KEY, "
                       "login VARCHAR(64) NOT NULL, "
                       "password VARCHAR(64) NOT NULL, "
                       "email VARCHAR(64) NOT NULL, "
                       "start_time INTEGER DEFAULT NULL, "
                       "finish_time INTEGER DEFAULT NULL, "
                       "stats INTEGER DEFAULT NULL, "
                       "connection_id INTEGER DEFAULT NULL "
                       ")");
        }
        DataBase(const DataBase& ) = delete;
        DataBase& operator = (DataBase &) = delete;
        ~DataBase() {db.close();}
    public:
        ///здесь пишем функцию сделать запрос к БД
        QVector<QMap<QString, QString>> sendQuerry(QString str){
            QSqlQuery query(this->db);
            query.exec(str);            // исполняем запрос

            QVector<QMap<QString, QString>> result; // таблица с запрашиваемыми данными

            if (query.lastError().isValid()) {
                qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
                return result;
            }

            QSqlRecord record = query.record();
            QVector<QString> columns;       // хранит столбцы возвращаемой таблицы

            for (int i = 0; i < record.count(); i++) {
                QString columnName = record.fieldName(i);
                columns.append(columnName);
            }

            while (query.next()) {
                QMap<QString, QString> row;
                for (int i = 0; i < columns.count(); i++) {
                    // строки таблицы заполняются, каждая строка - хэшмап, ключ - название столбца, значение - то, что в ячейке
                    QString columnName = query.record().fieldName(i);
                    QString value = query.value(i).toString();
                    row.insert(columnName, value);
                }
                result.append(row);
            }
            return result;
        }
        int dbAuth(int connectionID, QString login, QString password) {
            QVector<QMap<QString, QString>> users = sendQuerry(QString("select * from Users where "
                       "login = '%1' and password = '%2'").arg(login).arg(password));
            if (users.size() != 1)
                return 1;
            else if (users[0]["connection_id"] != "")
                return 2;
            sendQuerry(QString("update Users set connection_id = %1 "
                               "where login = '%2' and password = '%3'").arg(QString::number(connectionID), login, password));
            return 0;
        }
        int dbReg(int connectionID, QString login, QString password, QString email) {
            QVector<QMap<QString, QString>> users = sendQuerry(QString("select * from Users where "
                       "login = '%1' and password = '%2'").arg(login).arg(password));
            if (users.size() != 0)
                return 1;
            sendQuerry(QString("insert into Users(login, password, email) values ('%1', '%2', '%3')").arg(login, password, email));
            return 0;
        }
        int dbUnauth(int connectionID) {
            QVector<QMap<QString, QString>> users = sendQuerry(QString("select * from Users where "
                       "connection_id = %1").arg(QString::number(connectionID)));
            if (users.size() != 1)
                return 1;
            sendQuerry(QString("update Users set connection_id = '' "
                               "where connection_id = %1").arg(QString::number(connectionID)));
            return 0;
        }
        int dbStart(int connectionID, int time) {
            QVector<QMap<QString, QString>> users = sendQuerry(QString("select * from Users where "
                       "connection_id = %1").arg(QString::number(connectionID)));
            if (users.size() != 1)
                return 1;
            if (users[0]["start_time"] == "") {
                sendQuerry(QString("update Users set start_time = '%1' "
                                   "where connection_id = %2").arg(QString::number(time), QString::number(connectionID)));
                return 0;
            }
            int old_time = users[0]["start_time"].toInt();
            sendQuerry(QString("update Users set stats = '%1' "
                               "where connection_id = %2").arg(QString::number(time-old_time), QString::number(connectionID)));
            sendQuerry(QString("update Users set start_time = '%1' "
                               "where connection_id = %2").arg(QString::number(time), QString::number(connectionID)));
            return 2;
        }
        int dbFinish(int connectionID, int time) {
            QVector<QMap<QString, QString>> users = sendQuerry(QString("select * from Users where "
                       "connection_id = %1").arg(QString::number(connectionID)));
            if (users.size() != 1)
                return 1;
            if (users[0]["start_time"] == "") {
                return 2;
            }
            int start_time = users[0]["start_time"].toInt();
            sendQuerry(QString("update Users set stats = '%1' "
                               "where connection_id = %2").arg(QString::number(time-start_time), QString::number(connectionID)));
            sendQuerry(QString("update Users set finish_time = '%1' "
                               "where connection_id = %2").arg(QString::number(time), QString::number(connectionID)));
            return 0;
        }
};

#endif // DATABASE_H
