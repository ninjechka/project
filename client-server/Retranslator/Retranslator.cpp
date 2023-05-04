#include <QDataStream>
#include <Retranslator.h>
#include <QTextStream>
#include <QFile>
#include <QAbstractSocket>

Retranslator::Retranslator(QString argFile) : m_argsFile(argFile)
{
    m_timer = new QTimer(this);
    readFile();
    if(this->listen(QHostAddress::Any, m_port))
        qDebug() << "start retranslator" << m_id;
    else
        qDebug() << "error retranslator" << m_id;

    for (auto node: m_listenTo.keys()) {
        m_clientSocket = new QTcpSocket(this);
        connect (m_clientSocket, &QTcpSocket::readyRead, this, &Retranslator::slotReady);
        m_clientSocket->connectToHost(node.first, node.second);
        if(m_clientSocket->waitForConnected())
            qDebug() << "Connected to receiver" << " from" << m_id;
        else
        {
            qDebug() << "can`t connect to receiver";
            m_timer->start(5000);
            connect(m_timer, &QTimer::timeout, this, [this, node]() {
                m_clientSocket = new QTcpSocket(this);
                connect (m_clientSocket, &QTcpSocket::readyRead, this, &Retranslator::slotReadyRead);
                m_clientSocket->connectToHost(node.first, node.second);
                if(m_clientSocket->waitForConnected()) {
                    m_timer->stop();
                    qDebug() << "Connected to receiver" << " from" << m_id;
                }
            });
        }
    }
}

void Retranslator::slotReady()
{
    m_clientSocket = reinterpret_cast<QTcpSocket*>(sender());
    QDataStream in(m_clientSocket);
    in.setVersion(QDataStream::Qt_5_9);
    if(in.status() == QDataStream::Ok)
    {
        QString str;
        in >> str;
        QString command = str.split('#').at(0);
        if (command == QString::number(sendGraph))
        {
            QStringList args = str.split('#').at(1).split('_');
            QStringList from = args.at(args.size() - 1).split(":");
            int s = m_listenTo[qMakePair(from.at(0), from.at(1).toInt())];
            QString st = "-" + m_id + "-" + QString::number(s) + "_" + m_id;
            str.append(st);
            sendToClient(str);
        }
        if (command == QString::number(getPackage))
        {
            qDebug() << "get package";
            sendToClient(QString::number(getPackage));
        }
        if (command == QString::number(nodeDisconnected))
        {
            sendToServer(QString::number(getGraph));
            qDebug() << "Node disconnected";
        }
    }
    else
        qDebug() << "DataStream error";
}

void Retranslator::incomingConnection(qintptr socketDescriptor)
{
    m_serverSocket = new QTcpSocket;
    m_serverSocket-> setSocketDescriptor(socketDescriptor);
    connect (m_serverSocket, &QTcpSocket::readyRead, this, &Retranslator::slotReadyRead);
    connect (m_serverSocket, &QTcpSocket::disconnected, m_serverSocket, &QTcpSocket::deleteLater);
    m_clientSockets.push_back(m_serverSocket);
    qDebug() << m_id <<" client connected " << socketDescriptor;
}
void Retranslator::slotReadyRead()
{
    m_serverSocket = (QTcpSocket*)sender();
    QDataStream in(m_serverSocket);
    in.setVersion(QDataStream::Qt_5_9);
    if(in.status() == QDataStream::Ok)
    {
        QString str;
        in >> str;
        qDebug() << str;
        QStringList args;
        int command = str.split('#').at(0).toInt();
        switch (command) {
        case getGraph:
            sendToServer(QString::number(getGraph));
            break;
        case sendPackage:
            args = str.split('#');
            if (args.at(1) == m_id)
            {
                str = QString::number(sendPackage);
                for (int i = 2; i < args.size(); ++i)
                    str += "#" + args[i];
                qDebug() <<  m_id << "send to receiver:" << str;
                sendToServer(str);
            }
            break;
        default:
            break;
        }
    }
    else
        qDebug() << "DataStream error";
}

void Retranslator::sendToClient(QString str)
{
    m_data.clear();
    QDataStream out (&m_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << str;
    m_serverSocket->write(m_data);
}

void Retranslator::sendToServer(QString str){
    m_clientData.clear();
    QDataStream out (&m_clientData, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << str;
    m_clientSocket->write(m_clientData);
}

void Retranslator::readFile()
{
    QFile file(m_argsFile);
    QString data;
    if (!file.open(QIODevice::ReadOnly))
        qDebug() << "error";
    QTextStream in(&file);

    QString line = in.readLine();
    if(line != "\n"){
        m_port = line.split(" ").at(1).toInt();
        m_ip = line.split(" ").at(0);
        m_id = line.split(" ").at(0) + ":" +line.split(" ").at(1);
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

void Retranslator::disconnect(){
    m_clientSocket->disconnectFromHost();
    m_serverSocket->disconnectFromHost();
    if((m_clientSocket->state() == QAbstractSocket::UnconnectedState || m_clientSocket->waitForDisconnected(1000)) && (m_serverSocket->waitForDisconnected(1000) || m_serverSocket->state() == QAbstractSocket::UnconnectedState))
    {
        qDebug() << "disconnected";
        m_clientSocket->deleteLater();
        m_serverSocket->deleteLater();
    }
    else{
        qDebug() << "connected, ploho";
    }
}
