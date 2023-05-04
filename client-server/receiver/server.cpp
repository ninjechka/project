#include "server.h"
#include <QDataStream>
#include <QFile>

Server::Server()
{
    readFile();
    if(this->listen(QHostAddress::Any, 2324))
        qDebug() << "receiver start";
    else
        qDebug() << "error receiver";
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    m_socket = new QTcpSocket;
    m_socket->setSocketDescriptor(socketDescriptor);
    connect (m_socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect (m_socket, &QTcpSocket::disconnected, m_socket, &QTcpSocket::deleteLater);

    m_clientSockets.push_back(m_socket);
    qDebug() << "client connected" << socketDescriptor;

}

void Server::slotReadyRead()
{
    m_socket = (QTcpSocket*)sender();
    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_5_9);
    if(in.status() == QDataStream::Ok)
    {
        QString str;
        in >> str;
        qDebug() << str;
        QString command = str.split('#').at(0);
        if (command == QString::number(getGraph))
        {
            QString st = QString::number(sendGraph) + "#" + m_id;
            qDebug() << st;
            sendToClient(st);
        }
        if (command == QString::number(sendPackage))
        {
            QStringList args = str.split('#');
            qDebug() << args.at(1);
            m_packages.append(args.at(1));
            if (m_packages.size() < 50)
                sendToClient(QString::number(getPackage));
            else qDebug() << "all data received";
        }
    }
    else
        qDebug() << "DataStream error";
}

void Server::sendToClient(QString str)
{
    for (auto socket : m_clientSockets)
    {
        m_data.clear();
        QDataStream out (&m_data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_9);
        out << str;
        socket->write(m_data);
    }
}

void Server::readFile()
{
    QFile file("node_server.txt");
    QString data;
    if (!file.open(QIODevice::ReadOnly))
        qDebug() << "error open file";
    QTextStream in(&file);
    QString line = in.readLine();
    if(line != "\n"){
        m_port = line.split(" ").at(1).toInt();
        m_ip = line.split(" ").at(0);
        m_id = line.split(" ").at(0) + ":" +line.split(" ").at(1);
    }
    file.close();
}
