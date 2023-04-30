#include <client.h>
#include <QDataStream>
#include <QHostAddress>
#include <QFile>
#include <QRandom.h>
#include <QTime>
#include <QQueue>

Client::Client(){
    readFile();
    for(auto node : listenTo.keys())
    {
        m_socket = new QTcpSocket(this);
        connect (m_socket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
        m_socket->connectToHost(node.first, node.second);
        if(m_socket->waitForConnected())
            qDebug() << "Connected to Server";
        else
            qDebug() << "error connect";
        QString message = QString::number(getGraph);
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
        QString str;
        in >> str;
        qDebug() << str;

        QString command = str.split('#').at(0);
        if (command == QString::number(sendGraph))
        {
            str = str.split('#').at(1);
            QStringList args;
            args = str.split("_");
            for (int i = 0; i < args.size() - 1; ++i) {
                QStringList pair =  args.at(i).split("-");
                graph[qMakePair(pair.at(1), pair.at(0))] =  pair.at(2).toInt();
            }
            QStringList pair = args.at(args.size() - 1).split(":");
            graph[qMakePair(ip, args.at(args.size() - 1))] =  listenTo[qMakePair(pair.at(0), pair.at(1).toInt())];
            qDebug() <<  getBestPath();
        }
        sendPackageToServer();
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


void Client::sendPackageToServer()
{
    QString package = generatePackage() + "_" +QTime::currentTime().toString();
    QString path = getBestPath();
    QString message = QString::number(sendPackage) + path + '#' + package;
    sendToServer(message);
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

QString Client::generatePackage()
{
    QString package = QString::number(qrand() % 50) + QString::number(qrand() % 50) + QString::number(qrand() % 50);
    return package;
}

QString Client::getBestPath()
{
    QMap<QString, int> dist;
    for (auto node: graph.keys())
    {
        if (!dist.contains(node.first))
            dist[node.first] = INT_MAX;
        if (!dist.contains(node.second))
            dist[node.second] = INT_MAX;
    }

    QString s = ip;
    QString t = "127.0.0.1:2324";
    dist[s] = 0;
    QMap<QString, QString> p;
    p[s] = "";
    QQueue<QString> q;
    q.push_back(ip);
    QVector<QString> adj;
    while (!q.empty()) {
        QString v = q.front();
        q.pop_front();
        for (auto node: graph.keys()) {
            if (node.first == v)
                adj.append(node.second);
        }
        for (QString u : adj) {
            if (dist[u] > dist[v] + graph[qMakePair(v, u)]) {
                p[u] = v;
                dist[u] = dist[v] + graph[qMakePair(v, u)];
                q.push_back(u);
            }
        }
        adj.clear();
    }

    if (dist[t] == INT_MAX)
        return "";

    QVector<QString> path;
    while (t != "")
    {
        path.push_back(t);
        t = p[t];
    }

    QString strPath = "";
    for (int i = path.size() - 2; i > 0; --i)
        strPath += "#" + path.at(i);
    strPath += "#127.0.0.1:2324";
    return strPath;
}
