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
    quint16 port = 2324;
    std::map <quint16, quint16> ports = {{2323, 2323}};

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
