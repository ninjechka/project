#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    QTcpSocket *socket;
    int port;
    QString ip;
private:
    enum Commands {
        getGraph,
        sendGraph,
        sendPackage
    };
    QVector <QTcpSocket*> Sockets;
    QByteArray Data;

    void readFile();
    void SendToClient(QString str);
public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
};

#endif // SERVER_H
