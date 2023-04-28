#include "server.h"

Server::Server(int port, QString ip) : m_port{port}, m_ip{ip}
{
    if (this->listen(QHostAddress::Any, 2404))
    {
        qDebug() << "start";
    }
    else
    {
        qDebug() << "error";
    }
}

void Server::sendToClient(QString str)
{
    m_data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << str;
    m_serverSocket->write(m_data);
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    m_serverSocket = new QTcpSocket();
    m_serverSocket->setSocketDescriptor(socketDescriptor);
    connect(m_serverSocket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    Sockets.push_back(m_serverSocket);
    qDebug() << "client connected";
    
}

void Server::slotReadyRead()
{
    m_serverSocket = reinterpret_cast<QTcpSocket*>sender();
    QDataStream in(m_serverSocket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status() == QDataStream::Ok)
    {
        qDebug() <<"read...";
        QString str;
        in>>syr;
        qDebug() << str;
    }
    else
    {
        qDebug() << "DataStream error";
    }
}
