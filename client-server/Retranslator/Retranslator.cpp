#include <QDataStream>
#include <Retranslator.h>
#include <QTextStream>
#include <QFile>

Retranslator::Retranslator(QString argFile) : argsFile(argFile)
{
    readFile();
    if(this->listen(QHostAddress::Any, port)){
        qDebug() << "start";
    }
    else
    {
        qDebug() << "error resiver";
    }
    for (auto node: listenTo.keys()) {
        rC_socket = new QTcpSocket(this);
        connect (rC_socket, &QTcpSocket::readyRead, this, &Retranslator::slotReady);
        rC_socket->connectToHost(node.first, node.second);

        if(rC_socket->waitForConnected())
            qDebug() << "Connected to receiver";
        else
            qDebug() << "error resiver";
    }
}
void Retranslator::slotReady()
{
    rC_socket = (QTcpSocket*)sender();
    QDataStream in(rC_socket);
    in.setVersion(QDataStream::Qt_5_9);
    if(in.status() == QDataStream::Ok)
    {
        QString str;
        in >> str;
        QString command = str.split('#').at(0);
        if (command == QString::number(sendGraph))
        {
            str = str.split('#').at(1);
            QStringList args = str.split('_');
            QStringList from = args.at(args.size() - 1).split(":");
            int s = listenTo[qMakePair(from.at(0), from.at(1).toInt())];
             QString st = "-" + ip + ":" + QString::number(port) + "-" + QString::number(s) + "_" + ip + ":" + QString::number(port);
            str.append(st);
            qDebug() << str;
            SendToClient(str);
        }
    }
    else
    {
        qDebug() << "DataStream error";
    }

}

void Retranslator::incomingConnection(qintptr socketDescriptor)
{
    rS_socket = new QTcpSocket;
    rS_socket-> setSocketDescriptor(socketDescriptor);
    connect (rS_socket, &QTcpSocket::readyRead, this, &Retranslator::slotReadyRead);
    connect (rS_socket, &QTcpSocket::disconnected, rS_socket, &QTcpSocket::deleteLater);

    rS_Sockets.push_back(rS_socket);
    qDebug() << "client connected" << socketDescriptor;

}
void Retranslator::slotReadyRead()
{
    rS_socket = (QTcpSocket*)sender();
    QDataStream in(rS_socket);
    in.setVersion(QDataStream::Qt_5_9);
    if(in.status() == QDataStream::Ok)
    {
        QString str;
        in >> str;
        qDebug() << str;
        if (str == QString::number(getGraph))
        {

            //SendToClient("told to receiver about this");
            sendToServer(QString::number(getGraph));
        }
    }
    else
    {
        qDebug() << "DataStream error";
    }
}

void Retranslator::SendToClient(QString str)
{
    Data.clear();
    QDataStream out (&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << str;
    rS_socket->write(Data);
}

void Retranslator::sendToServer(QString str){
    rC_data.clear();
    QDataStream out (&rC_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << str;
    rC_socket->write(rC_data);

}

void Retranslator::readFile()
{
    QFile file(argsFile);
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
