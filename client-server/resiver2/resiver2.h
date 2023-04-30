#ifndef RESIVER2_H
#define RESIVER2_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
class Resiver2 : public QTcpServer
{
    Q_OBJECT
public:
    Resiver2();

private:
    QTcpSocket *rS_socket;
    int port;
    QString ip;

    QVector <QTcpSocket*> rS_Sockets;
    QByteArray Data;
    void SendToClient(QString str);

    enum Commands {
        getGraph,
        sendGraph,
        isAllConnect
    };

    QMap<QPair<QString, int>, int> listenTo;
    QTcpSocket* rC_socket;
    QByteArray rC_data;
    void sendToServer(QString str);
    void readFile();
    void connectTo();
public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
    void slotReady();
};
#endif // RESIVER2_H
