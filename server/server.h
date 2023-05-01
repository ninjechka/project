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
    std::map <quint16, quint16> ports = {{2323, 2323},
                                         {2324,2324},
                                         {2325,2325}};
    quint16 port;
    void SendToClient(QString str);
private:
    QVector <QTcpSocket*> Sockets;
    QByteArray Data;

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
};

#endif // SERVER_H
