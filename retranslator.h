#ifndef RETRANSLATOR_H
#define RETRANSLATOR_H

//#include <client.h>
#include <QObject>
#include <server.h>

class Retranslator //: public Server
{
    //Q_OBJECT
public:
    Retranslator();

private:
    int port;
    QString ip;

    struct Node
    {
        int port; // будем считать что порт - id
        QString ip;
    };

    QMap<int, Node> listenTo;
    QMap<int, Node> connectedTo;

    bool tryToConnect(int port);


};

#endif // RETRANSLATOR_H
