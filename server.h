#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(int port, QString ip);

private:
    QTcpSocket* m_serverSocket;
    QByteArray m_data;
    void sendToClient(QString str);

    QVector<QTcpSocket*> m_sockets;
    QString m_ip;
    int m_port;
public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();

private:
    QTcpServer* m_server;
};

#endif // SERVER_H
