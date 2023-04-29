#include "server.h"

Server::Server()
{
    if(this->listen(QHostAddress::Any, 2324)){
    qDebug() << "start";
    }
    else
    {
        qDebug() << "error server";
    }
}
void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket-> setSocketDescriptor(socketDescriptor);
    connect (socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect (socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    Sockets.push_back(socket);
    qDebug() << "client connected" << socketDescriptor;

}
void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status() == QDataStream::Ok)
    {
        qDebug() << "Server read...";
        QString str;
        in >> str;
        qDebug() << str;
        QString reportC = "message accepted from Server";
        SendToClient(reportC);
    }
    else
    {
        qDebug() << "DataStream error";
    }
}
void Server::SendToClient(QString str)
{
    Data.clear();
    QDataStream out (&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << str;
    socket->write(Data);

}