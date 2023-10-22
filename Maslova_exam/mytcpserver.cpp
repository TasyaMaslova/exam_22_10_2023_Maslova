#include "mytcpserver.h"


MyTcpServer::~MyTcpServer() {
    for (QTcpSocket* s : mTcpSocket)
            s->close();
    mTcpServer->close();
    serverStatus=0;
}
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
   // DataBase::getInstance();
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "server is not started";
    } else {
        serverStatus = 1;
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){
    QTcpSocket* socket = mTcpServer->nextPendingConnection();
    if(serverStatus == 1){
        socket->write("Сервер запущен.\r\n");
        connect(socket, &QTcpSocket::readyRead,
                this,&MyTcpServer::slotServerRead);
        connect(socket,&QTcpSocket::disconnected,
                this,&MyTcpServer::slotClientDisconnected);
    }
    mTcpSocket.insert(socket->socketDescriptor(), socket);
}

void MyTcpServer::slotServerRead(){
    QTcpSocket* socket = (QTcpSocket*)sender();
    QString data;
    QByteArray response;
    while(socket->bytesAvailable()>0) {
        data.append(socket->readLine());
    }
    if (data.size() > 0) {
        if (data.endsWith("\r\n"))
            data = data.split("\r\n")[0];
        else if (data.endsWith("\n"))
            data = data.split("\n")[0];
        if (data == "")
            return;
        data = QString::number(socket->socketDescriptor()) + "&" + data;
        qDebug() << "data: " << data << '\n';
        response = parsing(data);
        qDebug() << "response: "<< response << '\n';
        socket->write(response+"\n");
    }
}

void MyTcpServer::slotClientDisconnected(){
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->close();
}
