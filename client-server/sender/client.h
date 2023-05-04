#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QTimer>
#include <QVector>


class Client : public QObject
{
    Q_OBJECT
public:
    Client();

private:
     QString m_ip;
    enum Commands {
        getGraph, //запрос получения графа
        sendGraph, //отправляется граф
        sendPackage,// отправляется пакет,
        getPackage
    };

    QMap<QPair<QString, int>, int> m_listenTo; // кого слушаем ключ пара - айпи - порт, значение - задержка до узла
    QMap<QPair<QString, QString>, int> m_graph; // граф, состоит из пар - {айпи:порт,айпи:порт}, значение - задержка между узлами
    QTcpSocket* m_socket;
    QByteArray m_data;
    QTimer* m_timer;
    QVector<QTcpSocket*> m_sockets;
    QString generatePackage();
    void readFile();
    void sendToServer(QString str);
    void connectTo();
    QString getBestPath();
    void sendPackageToServer();
signals:
void tryToConnect();
public slots:
    void slotReadyRead();
};
#endif // CLIENT_H
