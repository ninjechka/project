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

void Server::incomingConnection(qintptr socketDescriptor)
{
    m_serverSocket = new QTcpSocket();
}
