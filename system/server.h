#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

class Receiver : public QTcpServer
{
    Q_OBJECT

public:
    Receiver(int port, QString ip);

private:
    QTcpSocket* m_clientSocket;
    void sendToServer();
    void connectTo();

    QTcpSocket* m_serverSocket;
    QByteArray m_data;
    void sendToClient(QString str);

    QVector<QTcpSocket*> Sockets;
    QString m_ip;
    int m_port;
public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyReadServer();
    void slotReadyReadClient();

private:
    QTcpServer* m_server;
};

#endif // SERVER_H
