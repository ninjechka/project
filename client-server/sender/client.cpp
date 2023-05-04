#include <client.h>
#include <QDataStream>
#include <QHostAddress>
#include <QFile>
#include <QRandom.h>
#include <QTime>
#include <QQueue>
#include <QTimer>

Client::Client(){
    readFile();
    m_timer = new QTimer(this);
    for(auto node : m_listenTo.keys())
    {
        m_socket = new QTcpSocket(this);
        connect (m_socket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
        m_socket->connectToHost(node.first, node.second);
        if(m_socket->waitForConnected())
        {
            qDebug() << "Connected to retranslator";
            m_sockets.append(m_socket);
            m_socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
            connect(m_socket, &QAbstractSocket::errorOccurred, [this]
            {
                qDebug() << "Node disconnected";
            });
        }
        else
        {
            qDebug() << "error connect";
            m_timer->start(5000);
             //пытаемся переподключиться к узлу
            connect(m_timer, &QTimer::timeout, this, [this, node]() {
                m_socket = new QTcpSocket(this);
                connect (m_socket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
                m_socket->connectToHost(node.first, node.second);
                if(m_socket->waitForConnected()) {
                    m_timer->stop();
                    qDebug() << "Connected to retranslator";
                    m_sockets.append(m_socket);
                }
            });
        }
    }

    //отправляем запрос на получение графа пока не получим его, т.к вдруг ретрансляторы не получили сообщение(были отключены)
    m_timer->start(5000);
    connect(m_timer, &QTimer::timeout, this, [this]() {
        if (m_graph.empty())
            sendToServer(QString::number(getGraph));
        else m_timer->stop();

    });

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
        QString command = str.split('#').at(0);
        qDebug() << command;
        if (command == QString::number(sendGraph))
        {
            str = str.split('#').at(1);
            QStringList args;
            args = str.split("_");
            for (int i = 0; i < args.size() - 1; ++i) {
                QStringList pair =  args.at(i).split("-");
                m_graph[qMakePair(pair.at(1), pair.at(0))] =  pair.at(2).toInt();
            }
            QStringList pair = args.at(args.size() - 1).split(":");
            m_graph[qMakePair(m_ip, args.at(args.size() - 1))] =  m_listenTo[qMakePair(pair.at(0), pair.at(1).toInt())];
            sendPackageToServer();
        }
        if (command == QString::number(getPackage))
            sendPackageToServer();
        if (command == QString::number(nodeDisconnected))
        {
            sendToServer(QString::number(getGraph));
            qDebug() << "Node disconnected";
        }
    }
    else
        qDebug() << "DataStream error";
}

void Client::sendToServer(QString str)
{
    for (auto socket : m_sockets)
    {
        if(socket)
        {
            m_data.clear();
            QDataStream out (&m_data, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_9);
            out << str;
            socket->write(m_data);
        }
    }
}


void Client::sendPackageToServer()
{
    QString path = getBestPath();
    QString package = generatePackage() + "_" +QTime::currentTime().toString();
    QString message = QString::number(sendPackage) + path + '#' + package;
    sendToServer(message);
}

void Client::readFile()
{
    QFile file("node_sender.txt");
    QString data;
    if (!file.open(QIODevice::ReadOnly))
        qDebug() << "error";
    QTextStream in(&file);

    QString line = in.readLine();
    if(line != "\n"){
        m_ip = line.split(" ").at(0);
        line = in.readLine();
        int count = line.toInt();
        for (int var = 0; var < count; ++var) {
            data = in.readLine();
            QStringList args = data.split(" ");
            m_listenTo[qMakePair(args[0], args[1].toInt())] = args[2].toInt();
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
    for (auto node: m_graph.keys())
    {
        if (!dist.contains(node.first))
            dist[node.first] = INT_MAX;
        if (!dist.contains(node.second))
            dist[node.second] = INT_MAX;
    }

    QString s = m_ip;
    QString t = "127.0.0.1:2324";
    dist[s] = 0;
    QMap<QString, QString> p;
    p[s] = "";
    QQueue<QString> q;
    q.push_back(m_ip);
    QVector<QString> adj;
    while (!q.empty()) {
        QString v = q.front();
        q.pop_front();
        for (auto node: m_graph.keys()) {
            if (node.first == v)
                adj.append(node.second);
        }
        for (QString u : adj) {
            if (dist[u] > dist[v] + m_graph[qMakePair(v, u)]) {
                p[u] = v;
                dist[u] = dist[v] + m_graph[qMakePair(v, u)];
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

void Client::disconnect(){
    m_socket->disconnectFromHost();
    if(m_socket->waitForDisconnected()){
        qDebug() << "disconnected";
    }
    else{
        qDebug() << "connected ploho";
    }
}
