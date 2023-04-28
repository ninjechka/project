#include "client.h"

#include <QDataStream>
#include <QHostAddress>

Client::Client()
{
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
    //connect(m_socket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
    m_socket->connectToHost(QHostAddress::LocalHost, 6002);
    if(m_socket->waitForConnected())
        qDebug() << "Connected to Server";
    else
        qDebug() << "error";
}

void Client::sendToServer()
{
    if(m_socket)
    {
        if(m_socket->isOpen())
        {
            QString str = "hi";
            QDataStream socketStream(m_socket);
            socketStream.setVersion(QDataStream::Qt_5_9);
            QByteArray header;
            header.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(str.size()).toUtf8());
            header.resize(128);
            QByteArray byteArray = str.toUtf8();
            byteArray.prepend(header);
            socketStream << byteArray;
        }
        else
           qDebug() << "Socket doesn't seem to be opened";
    }
    else
        qDebug() << "Not connected";
}

void Client::connectTo()
{

}

void Client::slotReadyRead()
{
    QByteArray buffer;

    QDataStream socketStream(m_socket);
    socketStream.setVersion(QDataStream::Qt_5_9);
    socketStream.startTransaction();
    socketStream >> buffer;
    if(!socketStream.commitTransaction())
    {
        QString message = QString("INFO :: Attachment from sd:%1 discarded").arg(m_socket->socketDescriptor());
        qDebug() << "Waiting for more data to come.. " << message;;
        return;
    }
   // QString header = buffer.mid(0,128);
    //QString fileType = header.split(",")[0].split(":")[1];
   // buffer = buffer.mid(128);


        QString message = QString("%1 :: %2").arg(m_socket->socketDescriptor()).arg(QString::fromStdString(buffer.toStdString()));
        qDebug() << message;

   // sendToServer();
}
