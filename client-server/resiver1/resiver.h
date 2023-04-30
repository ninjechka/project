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


private:
    int port;
    QString ip;

    enum Commands {
        getGraph,
        sendGraph,
        isAllConnect
    };
    QVector <QTcpSocket*> rS_Sockets;
    QByteArray Data;
    void SendToClient(QString str);

    QMap<QPair<QString, int>, int> listenTo;
    void readFile();
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
