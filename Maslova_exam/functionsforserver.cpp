#include "functionsforserver.h"
#include "dataBase.h"

QByteArray parsing (QString data)
 {
     /* data_from_client = NameOfFunc&Param1&Param2&Param3
      * Цель парсинга:
      * return NameOfFunc(Param1,Param2,Param3)
      */
     QStringList dataList = data.split("&");
     // data_from_client_list  = "NameOfFunc" "Param1" "Param2" Param3"
     int connection_id = dataList.front().toInt();
     dataList.pop_front();
     QString nameOfFunc = dataList.front();
     dataList.pop_front();
     if (nameOfFunc == "auth")
         return auth(connection_id, dataList.at(0),dataList.at(1));
     else if (nameOfFunc == "reg")
         return reg(connection_id, dataList.at(0),dataList.at(1),dataList.at(2));
     else if (nameOfFunc == "unauth")
         return unauth(connection_id);
     else if (nameOfFunc == "start")
         return start(connection_id, dataList.at(0));
     else if (nameOfFunc == "finish")
         return finish(connection_id, dataList.at(0));
     else
         return "error";
 }


QByteArray auth(int connection_id, QString login, QString password) {
    int result = DataBase::getInstance()->dbAuth(connection_id, login, password);
    if (result == 0)
        return "auth_ok\n";
    if (result == 1)
        return "auth_false\n";
    return "auth_err\n";
}

QByteArray reg(int connection_id, QString login, QString password, QString email) {
    int result = DataBase::getInstance()->dbReg(connection_id, login, password, email);
    if (result == 0)
        return "reg_ok\n";
    return "reg_err\n";
}

QByteArray unauth(int connection_id) {
    int result = DataBase::getInstance()->dbUnauth(connection_id);
    if (result == 0)
        return "unauth_ok\n";
    return "unauth_err\n";
}

QByteArray start(int connection_id, QString time_start) {
    if (! QRegularExpression("^\\d\\d_\\d\\d_\\d\\d$").match(time_start).hasMatch())
        return "start_format_err";
    int hours = time_start.section("_", 0, 0).toInt();
    int minutes = time_start.section("_", 1, 1).toInt();
    int seconds = time_start.section("_", 2, 2).toInt();
    if (seconds >= 60 or minutes >= 60 or hours >= 24)
        return "start_format_err\n";
    int result = DataBase::getInstance()->dbStart(connection_id, hours*3600+minutes*60+seconds);
    if (result == 0)
        return "start_ok\n";
    if (result == 1)
        return "start_err\n";
    return "start_reok\n";
}

QByteArray finish(int connection_id, QString time_finish) {
    if (! QRegularExpression("^\\d\\d_\\d\\d_\\d\\d$").match(time_finish).hasMatch())
        return "finish_format_err";
    int hours = time_finish.section("_", 0, 0).toInt();
    int minutes = time_finish.section("_", 1, 1).toInt();
    int seconds = time_finish.section("_", 2, 2).toInt();
    if (seconds >= 60 or minutes >= 60 or hours >= 24)
        return "finish_format_err\n";
    int result = DataBase::getInstance()->dbFinish(connection_id, hours*3600+minutes*60+seconds);
    if (result == 0)
        return "finish_ok\n";
    if (result == 1)
        return "finish_err\n";
    return "finish_reerr\n";
}
