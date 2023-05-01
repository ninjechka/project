#include <client.h>
#include <server.h>

Client::Client(){
   m_socket = new QTcpSocket(this);
   connect (m_socket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
   m_socket->connectToHost(QHostAddress::LocalHost, 2323);
   //m_socket->connectToHost(QHostAddress::LocalHost, 2325);
   if(m_socket->waitForConnected())
   {
       qDebug() << "Connected to Server";
   }
   else {
       qDebug() << "error";
   }
  /* QString message = "12";
   sendToServer(message);*/
}
void Client::connectTo(){
     m_socket->connectToHost(QHostAddress::LocalHost, 2323);
     if(m_socket->waitForConnected())
     {
         qDebug() << "Connected to Server";
     }
     else {
         qDebug() << "error";
     }
     QString message = "12";
     sendToServer(message);
}
void Client::slotReadyRead()
{
   /* QByteArray buffer;

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
    QString header = buffer.mid(0,128);
    QString fileType = header.split(",")[0].split(":")[1];
    buffer = buffer.mid(128);

    if(fileType=="message"){
        QString message = QString("%1 :: %2").arg(m_socket->socketDescriptor()).arg(QString::fromStdString(buffer.toStdString()));
        qDebug() << message;
    }*/
    m_socket = (QTcpSocket*)sender();
    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status() == QDataStream::Ok)
    {
        qDebug() << "Client read...";
        QString str;
        in >> str;
        qDebug() << str;

    }
    else
    {
        qDebug() << "DataStream error";
    }
}
void Client::sendToServer(QString str){
    m_data.clear();
    QDataStream out (&m_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << str;
    m_socket->write(m_data);

}
