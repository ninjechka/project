#include "server.h"
#include <QDataStream>
#include <QTimer>

Receiver::Receiver(int port, QString ip) : m_port{port}, m_ip{ip}
{
    if (this->listen(QHostAddress::Any, 6001))
    {
        qDebug() << "start";
    }
    else
    {
        qDebug() << "error";
    }
    QTimer* timer = new QTimer();
    timer->start(10000);
     connect(timer, &QTimer::timeout, this, &Receiver::connectTo);
   // connectTo();
}

void Receiver::sendToClient(QString str)
{
    m_data.clear();
    QDataStream out(&m_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << str;
    m_serverSocket->write(m_data);
}

void Receiver::incomingConnection(qintptr socketDescriptor)
{
    m_serverSocket = new QTcpSocket();
    m_serverSocket->setSocketDescriptor(socketDescriptor);
    connect(m_serverSocket, &QTcpSocket::readyRead, this, &Receiver::slotReadyReadServer);
    Sockets.push_back(m_serverSocket);
    sendToClient("hi");
    qDebug() << "client connected";

}

void Receiver::slotReadyReadServer()
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

void Receiver::slotReadyReadClient()
{QByteArray buffer;

    QDataStream socketStream(m_clientSocket);
    socketStream.setVersion(QDataStream::Qt_5_9);
    socketStream.startTransaction();
    socketStream >> buffer;
    if(!socketStream.commitTransaction())
    {
        QString message = QString("INFO :: Attachment from sd:%1 discarded").arg(m_clientSocket->socketDescriptor());
        qDebug() << "Waiting for more data to come.. " << message;;
        return;
    }
   // QString header = buffer.mid(0,128);
    //QString fileType = header.split(",")[0].split(":")[1];
   // buffer = buffer.mid(128);


        QString message = QString("%1 :: %2").arg(m_clientSocket->socketDescriptor()).arg(QString::fromStdString(buffer.toStdString()));
        qDebug() << message;
    sendToServer();

}

void Receiver::sendToServer()
{
    if(m_clientSocket)
    {
        if(m_clientSocket->isOpen())
        {
            QString str = "hi";
            QDataStream socketStream(m_clientSocket);
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

void Receiver::connectTo()
{
    m_clientSocket = new QTcpSocket(this);
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &Receiver::slotReadyReadClient);
    //connect(m_socket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
    m_clientSocket->connectToHost(QHostAddress::LocalHost, 6002);
    if(m_clientSocket->waitForConnected())
        qDebug() << "Connected to Server";
    else
        qDebug() << "error";
}
