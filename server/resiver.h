#ifndef RESIVER_H
#define RESIVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QMap>
class Resiver : public QTcpServer
{
    Q_OBJECT
public:
    Resiver();
    QTcpSocket *rS_socket;
    quint16 reincornatedPortTo = 2324;
    quint16 reincornatedPortFrom = 2323;
    quint16 portTo = 2324;
    quint16 portFrom = 2323;
    std::map <quint16, quint16> ports = {{2323, 2323}};
    void killNode();
    void reviveNode();

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
