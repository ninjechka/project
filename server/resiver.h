#ifndef RESIVER_H
#define RESIVER_H
#include <server.h>
#include <client.h>
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
class Resiver : public QTcpServer
{
    Q_OBJECT
public:
    Resiver();
    QTcpSocket *rS_socket;

private:
    QVector <QTcpSocket*> rS_Sockets;
    QByteArray Data;
    void SendToClient(QString str);

    QTcpSocket* rC_socket;
    QByteArray rC_data;
    void sendToServer(QString str);
    void connectTo();
public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
    void slotReady();
};


#endif // RESIVER_H
