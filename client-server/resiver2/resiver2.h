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
    QTcpSocket *rS_socket;
    quint16 port = 2325;
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
#endif // RESIVER2_H
