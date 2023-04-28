#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(int port, QString ip);

private:
    QTcpSocket* m_clientSocket;
    void sendToServer();
    void connectTo();

    QTcpSocket* m_serverSocket;
    QByteArray m_data;
    void sendToClient(QString str);

    QVector<QTcpSocket*> Sockets;
    QString m_ip;
    int m_port;
public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
signals:
    void newMessage(QString);
private slots:
    void newConnection();
    void appendToSocketList(QTcpSocket* socket);

    void readSocket();
    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);

    void displayMessage(const QString& str);
    void sendMessage(QTcpSocket* socket);
    void sendAttachment(QTcpSocket* socket, QString filePath);
private:
    QTcpServer* m_server;
    QSet<QTcpSocket*> connection_set;

};

#endif // SERVER_H
