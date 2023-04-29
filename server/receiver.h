#ifndef RECEIVER_H
#define RECEIVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QVector>


class Receiver : public QTcpServer
{
    Q_OBJECT

public:
    Receiver(int port, QString ip);

private:
    QTcpSocket* m_serverSocket;
    QByteArray m_data;
    void sendToClient(QString str);

    QVector<QTcpSocket*> m_sockets;
    QString m_ip;
    int m_port;
   // QVector<int, QVector<int>> graph2 = {1:{(2, 10) ,(3, 5)}};
   // QVector<QPair<int, int>> graph = { (1, 2), (1, 3), (2, 4), (3, 4)};
public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();

private:
    QTcpServer* m_server;
};

#endif // RECEIVER_H
