#ifndef FUNCTIONSFORSERVER_H
#define FUNCTIONSFORSERVER_H
#include <QString>
#include "dataBase.h"
#include <QStringList>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
/**
 * \brief parsing - функция, которая по введенному названию
 * и параметрам функции возращает результат.
 * \param [in] data_from_client - строка, которая приходит от клиента
 * в формате....
 * \return Возвращает результат работы функции, которая была подана на вход.
 *
 * \example указать_файл.txt
 */
QByteArray parsing (QString data_from_client);

QByteArray auth(int connection_id, QString log, QString pass);

QByteArray reg(int connection_id, QString log, QString pass, QString mail);

QByteArray unauth(int connection_id);

QByteArray start(int connection_id, QString time_start);

QByteArray finish(int connection_id, QString time_finish);

#endif
// FUNCTIONSFORSERVER_H
