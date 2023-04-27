#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(int port, QString ip);

private:
    QTcpSocket* m_clientSocket;
    void sendToServer();
    void connect();

    QTcpSocket* m_serverSocket;
    QByteArray m_data;
    void sendToClient();

    QString m_ip;
    int m_port;
public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();

};

#endif // SERVER_H
