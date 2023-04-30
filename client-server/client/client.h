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
        getGraph, //запрос получения графа
        sendGraph, //отправляется граф
        sendPackage// отправляется пакет
    };

    bool waitGraph = false;
    QMap<QPair<QString, int>, int> listenTo; // кого слушаем ключ пара - айпи - порт, значение - задержка до узла
    QMap<QPair<QString, QString>, int> graph; // граф, состоит из пар - {айпи:порт,айпи:порт}, значение - задержка между узлами
    QTcpSocket* m_socket;
    QByteArray m_data;

    QString generatePackage();
    void readFile();
    void sendToServer(QString str);
    void connectTo();

    void sendPackageToServer();

public slots:
    void slotReadyRead();
};
#endif // CLIENT_H
