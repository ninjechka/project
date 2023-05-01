
#include <resiver.h>
#include <QTextStream>

Resiver::Resiver()
{
    if(this->listen(QHostAddress::Any, portFrom)){
    qDebug() << "start";
    }
    else
    {
        qDebug() << "error resiver";
    }

    rC_socket = new QTcpSocket(this);
    connect (rC_socket, &QTcpSocket::readyRead, this, &Resiver::slotReady);
    rC_socket->connectToHost(QHostAddress::LocalHost, 2324);
    if(rC_socket->waitForConnected())
    {
        qDebug() << "Connected to Resiver";
    }
    else {
        qDebug() << "error resiver";
    }
}
void Resiver::killNode(){
    rC_socket->disconnectFromHost();
   // rC_socket = nullptr;
    //portTo = 11111;
   // rS_socket->disconnectFromHost();
   // portFrom = 11111
   // rS_socket = nullptr;
    qDebug() << "nod off";
}
void Resiver::reviveNode(){
    portTo = reincornatedPortTo;
    portFrom = reincornatedPortFrom;
    Resiver();
    qDebug() << "nod On";
}
void Resiver::slotReady()
{
    /*QByteArray buffer;

    QDataStream socketStream(rC_socket);
    socketStream.setVersion(QDataStream::Qt_5_9);
    socketStream.startTransaction();
    socketStream >> buffer;
    if(!socketStream.commitTransaction())
    {
        QString message = QString("INFO :: Attachment from sd:%1 discarded").arg(rC_socket->socketDescriptor());
        qDebug() << "Waiting for more data to come.. " << message;;
        return;
    }
    QString header = buffer.mid(0,128);
    QString fileType = header.split(",")[0].split(":")[1];
    buffer = buffer.mid(128);

    if(fileType=="message"){
        QString message = QString("%1 :: %2").arg(rC_socket->socketDescriptor()).arg(QString::fromStdString(buffer.toStdString()));
        qDebug() << message;
    }*/
    rC_socket = (QTcpSocket*)sender();
    QDataStream in(rC_socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status() == QDataStream::Ok)
    {
        qDebug() << "Client-Resiver read...";
        QString str;
        in >> str;
        qDebug() << str;
        SendToClient(str);

    }
    else
    {
        qDebug() << "DataStream error";
    }

}

void Resiver::incomingConnection(qintptr socketDescriptor)
{
    rS_socket = new QTcpSocket;
    rS_socket-> setSocketDescriptor(socketDescriptor);
    connect (rS_socket, &QTcpSocket::readyRead, this, &Resiver::slotReadyRead);
    connect (rS_socket, &QTcpSocket::disconnected, rS_socket, &QTcpSocket::deleteLater);

    rS_Sockets.push_back(rS_socket);
    qDebug() << "client connected" << socketDescriptor;

}
void Resiver::slotReadyRead()
{
    rS_socket = (QTcpSocket*)sender();
    QDataStream in(rS_socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status() == QDataStream::Ok)
    {
        qDebug() << "Resiver read...";
        QString str;
        in >> str;
        qDebug() << str;
        QString reportC = "message accepted from Resiver";
        QTextStream cout (stdout);
        SendToClient(reportC);
        sendToServer(str);

    }
    else
    {
        qDebug() << "DataStream error";
    }
}
void Resiver::SendToClient(QString str)
{
    Data.clear();
    QDataStream out (&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << str;
    rS_socket->write(Data);
}
void Resiver::sendToServer(QString str){
    rC_data.clear();
    QDataStream out (&rC_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << str;
    rC_socket->write(rC_data);

}
