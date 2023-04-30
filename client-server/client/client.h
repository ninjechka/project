#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QVector>


class Client : public QObject
{
    Q_OBJECT
public:
    Client();

private:
     QString ip;
    enum Commands {
        getGraph,
        sendGraph,
        isAllConnect
    };

    QMap<QPair<QString, int>, int> listenTo;
    QMap<QPair<QString, QString>, int> graph;
    QTcpSocket* m_socket;
    QByteArray m_data;

    void readFile();
    void sendToServer(QString str);
    void connectTo();

public slots:
    void slotReadyRead();
};
#endif // CLIENT_H
