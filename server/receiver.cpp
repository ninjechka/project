#include "receiver.h"
#include <QDataStream>

Receiver::Receiver(int port, QString ip) : m_port{port}, m_ip{ip}
{
    if (this->listen(QHostAddress::Any, port))
    {
        qDebug() << "start";
    }
    else
    {
        qDebug() << "error";
    }
}

void Receiver::sendToClient(QString str)
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

void Receiver::incomingConnection(qintptr socketDescriptor)
{
    m_serverSocket = new QTcpSocket();
    m_serverSocket->setSocketDescriptor(socketDescriptor);
    connect(m_serverSocket, &QTcpSocket::readyRead, this, &Receiver::slotReadyRead);
    m_sockets.push_back(m_serverSocket);
    sendToClient("hi");
    qDebug() << "client connected";
}

void Receiver::slotReadyRead()
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

    buffer = buffer.mid(128);
    QString message = QString("%1 :: %2").arg(m_serverSocket->socketDescriptor()).arg(QString::fromStdString(buffer.toStdString()));
    qDebug() << message;


}
