#ifndef RETRANSLATOR_H
#define RETRANSLATOR_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QMap>
#include <QTimer>
class Retranslator : public QTcpServer
{
    Q_OBJECT
public:
    Retranslator(QString argFile);

    void disconnect();
private:
    int m_port;
    QString m_ip;
    QString m_argsFile;
    QString m_id;

    enum Commands {
        getGraph,
        sendGraph,
        sendPackage,
        getPackage,
        nodeDisconnected
    };

    QTimer* m_timer;
    QTcpSocket *m_serverSocket;
    QVector <QTcpSocket*> m_clientSockets;
    QByteArray m_data;
    void sendToClient(QString str);

    QTcpSocket* m_clientSocket;
    QByteArray m_clientData;
    QMap<QPair<QString, int>, int> m_listenTo;
    void readFile();
    void sendToServer(QString str);
    void connectTo();

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
    void slotReady();
};


#endif // RETRANSLATOR_H
