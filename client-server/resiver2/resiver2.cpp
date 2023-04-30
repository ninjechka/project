#include "resiver2.h"
#include <QDataStream>
#include <QFile>
#include <QTextStream>

Resiver2::Resiver2()
{
    readFile();
    if(this->listen(QHostAddress::Any, port))
        qDebug() << "start";
    else
        qDebug() << "error resiver";

    for (auto node: listenTo.keys()) {
        rC_socket = new QTcpSocket(this);
        connect (rC_socket, &QTcpSocket::readyRead, this, &Resiver2::slotReady);
        rC_socket->connectToHost(node.first, node.second);
        if(rC_socket->waitForConnected())
            qDebug() << "Connected to Resiver2";
        else
            qDebug() << "error resiver";
    }
}
void Resiver2::slotReady()
{
    rC_socket = reinterpret_cast<QTcpSocket*>(sender());
    QDataStream in(rC_socket);
    in.setVersion(QDataStream::Qt_5_9);
    if(in.status() == QDataStream::Ok)
    {
        qDebug() << "Client-Resiver2 read...";
        QString str;
        in >> str;
        QStringList args = str.split('_');
        QStringList from = args.at(args.size() - 1).split(":");
        int s = listenTo[qMakePair(from.at(0), from.at(1).toInt())];
        QString st = "-" + ip + ":" + QString::number(port) + "-" + QString::number(s) + "_" + ip + ":" + QString::number(port);// QString("-%1:%2-%3_%1:%2").arg(ip, port, s);
        str.append(st);
        SendToClient(str);

    }
    else
        qDebug() << "DataStream error";
}

void Resiver2::incomingConnection(qintptr socketDescriptor)
{
    rS_socket = new QTcpSocket;
    rS_socket-> setSocketDescriptor(socketDescriptor);
    connect (rS_socket, &QTcpSocket::readyRead, this, &Resiver2::slotReadyRead);
    connect (rS_socket, &QTcpSocket::disconnected, rS_socket, &QTcpSocket::deleteLater);

    rS_Sockets.push_back(rS_socket);
    qDebug() << "client connected Resiver2" << socketDescriptor;

}
void Resiver2::slotReadyRead()
{
    rS_socket = (QTcpSocket*)sender();
    QDataStream in(rS_socket);
    in.setVersion(QDataStream::Qt_5_9);
    if(in.status() == QDataStream::Ok)
    {
        qDebug() << "Resiver2 read...";
        QString str;
        in >> str;
        qDebug() << str;
        if (str == QString::number(getGraph))
        {
            SendToClient("told to receiver about this");
            sendToServer(QString::number(getGraph));
        }

    }
    else
        qDebug() << "DataStream error";
}
void Resiver2::SendToClient(QString str)
{
    Data.clear();
    QDataStream out (&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << str;
    rS_socket->write(Data);
}
void Resiver2::sendToServer(QString str){
    rC_data.clear();
    QDataStream out (&rC_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << str;
    rC_socket->write(rC_data);

}

void Resiver2::readFile()
{
    QFile file("node.txt");
    QString data;
    if (!file.open(QIODevice::ReadOnly))
        qDebug() << "error";
    QTextStream in(&file);

    QString line = in.readLine();
    if(line != "\n"){
        port = line.split(" ").at(1).toInt();
        ip = line.split(" ").at(0);
        line = in.readLine();
        int count = line.toInt();
        for (int var = 0; var < count; ++var) {
            data = in.readLine();
            QStringList args = data.split(" ");
            listenTo[qMakePair(args[0], args[1].toInt())] = args[2].toInt();
        }
    }

    file.close();
}
