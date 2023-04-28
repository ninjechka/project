#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    Client();

private:
    QTcpSocket* m_socket;
    QByteArray m_data;
    void sendToServer();
    void connectTo();

public slots:
    void slotReadyRead();
};

#endif // CLIENT_H
