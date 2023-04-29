#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <server.h>

class Client : public QObject
{
    Q_OBJECT
public:
    Client();

private:
    QTcpSocket* m_socket;
    QByteArray m_data;
    void sendToServer(QString str);
    void connectTo();

public slots:
    void slotReadyRead();
};
#endif // CLIENT_H
