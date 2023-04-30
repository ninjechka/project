#ifndef RETRANSLATOR_H
#define RETRANSLATOR_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QMap>
class Retranslator : public QTcpServer
{
    Q_OBJECT
public:
    Retranslator(QString argFile);
    QTcpSocket *rS_socket;

private:
    int port;
    QString ip;
    QString argsFile;
    QString id;

    enum Commands {
        getGraph,
        sendGraph,
        sendPackage
    };

    QVector <QTcpSocket*> rS_Sockets;
    QByteArray Data;
    void SendToClient(QString str);

    QTcpSocket* rC_socket;
    QByteArray rC_data;
    QMap<QPair<QString, int>, int> listenTo;
    void readFile();
    void sendToServer(QString str);
    void connectTo();

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
    void slotReady();
};


#endif // RETRANSLATOR_H