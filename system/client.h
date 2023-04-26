#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>

class Client
{
    Q_OBJECT

public:
    Client();

private:
    QTcpSocket* m_socket;
    QByteArray m_data;
    void sendToServer();
    void connect();

public slots:
    void slotReadyRead();
};

#endif // CLIENT_H
