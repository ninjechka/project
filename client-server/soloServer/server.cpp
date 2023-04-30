#include "server.h"
#include <QDataStream>
#include <QFile>

Server::Server()
{
    readFile();
    if(this->listen(QHostAddress::Any, 2324))
        qDebug() << "start";
    else
        qDebug() << "error server";
}


void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect (socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect (socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    Sockets.push_back(socket);
    qDebug() << "client connected" << socketDescriptor;

}
void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_9);
    if(in.status() == QDataStream::Ok)
    {
        QString str;
        in >> str;
        qDebug() << str;
        if (str == QString::number(getGraph))
        {
            QString st = QString::number(sendGraph) + "#" + ip + ":" + QString::number(port);
            SendToClient(st);
        }
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
    out.setVersion(QDataStream::Qt_5_9);
    out << str;
    socket->write(Data);

}

void Server::readFile()
{
    QFile file("node.txt");
    QString data;
    if (!file.open(QIODevice::ReadOnly))
        qDebug() << "error open file";
    QTextStream in(&file);
    QString line = in.readLine();
    if(line != "\n"){
        port = line.split(" ").at(1).toInt();
        ip = line.split(" ").at(0);
    }
    file.close();
}
