#include "server.h"
#include <QDataStream>

Server::Server(int port, QString ip) : m_port{port}, m_ip{ip}
{
    if (this->listen(QHostAddress::Any, 6002))
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
    QDataStream out(&m_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << str;
    m_serverSocket->write(m_data);
    if (m_serverSocket->waitForReadyRead())
    {
        qDebug("read");
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    m_serverSocket = new QTcpSocket();
    m_serverSocket->setSocketDescriptor(socketDescriptor);
    connect(m_serverSocket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    Sockets.push_back(m_serverSocket);
    sendToClient("hi");
    qDebug() << "client connected";

}

void Server::slotReadyRead()
{
    QByteArray buffer;

    QDataStream socketStream(m_serverSocket);
    socketStream.setVersion(QDataStream::Qt_5_9);
    socketStream.startTransaction();
    socketStream >> buffer;
    if(!socketStream.commitTransaction())
    {
        QString message = QString("%1 :: Waiting for more data to come..").arg(m_serverSocket->socketDescriptor());
        qDebug() << message;
    }
    QString header = buffer.mid(0,128);
    QString fileType = header.split(",")[0].split(":")[1];
    buffer = buffer.mid(128);

    if(fileType=="message"){
        QString message = QString("%1 :: %2").arg(m_serverSocket->socketDescriptor()).arg(QString::fromStdString(buffer.toStdString()));
        qDebug() << message;
    }
//    m_serverSocket = reinterpret_cast<QTcpSocket*>(sender());
//    QDataStream in(m_serverSocket);
//    in.setVersion(QDataStream::Qt_5_9);
//    if(in.status() == QDataStream::Ok)
//    {
//        qDebug() <<"read...";
//        QString str;
//        in>>str;
//        qDebug() << str;
//    }
//    else
//    {
//        qDebug() << "DataStream error";
//    }
}
