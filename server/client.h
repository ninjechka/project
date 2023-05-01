#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QTcpSocket>


class Client : public QObject
{
    Q_OBJECT
public:
    Client();
void connectTo();
void sendToServer(QString str);
private:
    QTcpSocket* m_socket;
    QByteArray m_data;



public slots:
    void slotReadyRead();
};
#endif // CLIENT_H
