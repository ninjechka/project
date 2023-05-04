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

private:
    QTcpSocket* m_socket;
    int m_port;
    QString m_ip;
    QString m_id;

    enum Commands {
        getGraph,
        sendGraph,
        sendPackage,
        getPackage
    };

    QVector<QTcpSocket*> m_clientSockets;
    QByteArray m_data;
    QVector<QString> m_packages;
    void readFile();
    void sendToClient(QString str);
public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
};

#endif // SERVER_H
