#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H
//#include "dataBase.h"
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include <list>
#include <QtNetwork>
#include <QByteArray>
#include <QDebug>
//#include "functions.h"
#include "functionsforserver.h"

/**
 * \authors авторы
 * \date дата написания
 * \bug описание багов
 * \todo что доделать
 * \version указание версии
 * \example example_file.txt
 * \warning ограничения
 * \brief The MyTcpServer class - класс для описания работы сервера
 *
 * Класс на основе QTcpServer, который ....
 */

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();
public slots:
    /**
     * \brief slotNewConnection - сло для ...
     *
     * подробное описание слота...
     */
    void slotNewConnection();
    void slotClientDisconnected();
    void slotServerRead();

private:
    QTcpServer * mTcpServer;
    QMap<int, QTcpSocket*> mTcpSocket;
    int serverStatus;
};
#endif // MYTCPSERVER_H







