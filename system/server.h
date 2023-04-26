#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server();   

private:
    QTcpSocket* m_socket;
    QByteArray m_data;
    void sendToClient();

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();

};

#endif // SERVER_H
