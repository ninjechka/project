#ifndef SENDER_H
#define SENDER_H

#include <QObject>
#include <QTcpSocket>

class Sender : public QObject
{
    Q_OBJECT
public:
    Sender();

private:
    QTcpSocket* m_socket;
    QByteArray m_data;
    void sendToServer();
    void connectTo();

public slots:
    void slotReadyRead();
};

#endif // SENDER_H
