#include <client.h>
#include <QDataStream>
#include <QHostAddress>
#include <QFile>

Client::Client(){
    readFile();
    for(auto node : listenTo.keys())
    {
        m_socket = new QTcpSocket(this);
        connect (m_socket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
        m_socket->connectToHost(node.first, node.second);
        if(m_socket->waitForConnected())
        {
            qDebug() << "Connected to Server";
        }
        else {
            qDebug() << "error connect";
        }
        QString message = QString::number(getGraph);;
        sendToServer(message);
    }
}


void Client::slotReadyRead()
{
    m_socket = (QTcpSocket*)sender();
    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_5_9);
    if(in.status() == QDataStream::Ok)
    {
        qDebug() << "Client read...";
        QString str;
        in >> str;
        qDebug() << str;
        QStringList args;
        args = str.split(" ");
        qDebug() <<  args.size();//args.at(0);
//        if (args.at(0) == "101")
//        {
//            for (int i = 1; i < args.size() - 1; i++)
//            {
//                QStringList pair =  args.at(i).split("-");
//                graph.append(qMakePair(pair.at(0), pair.at(1)));

//            }
//        }
//        qDebug() <<  graph.size();
    }
    else
    {
        qDebug() << "DataStream error";
    }
}

void Client::sendToServer(QString str)
{
    m_data.clear();
    QDataStream out (&m_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << str;
    m_socket->write(m_data);

}

void Client::readFile()
{
    QFile file("node.txt");
    QString data;
    if (!file.open(QIODevice::ReadOnly))
        qDebug() << "error";
    QTextStream in(&file);

    QString line = in.readLine();
    if(line != "\n"){
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
