#include <QCoreApplication>
#include <server.h>
#include <client.h>
#include <resiver.h>
#include <QTimer>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server s;
    Resiver r;
    Client c;
    QString str = "hi";
   QTimer *timer1 = new QTimer();
  // Client::connect(timer1, &QTimer::timeout, [&r](){r.killNode();});
   timer1->singleShot(1,[&r](){r.killNode();});
   // timer1->start();
    c.sendToServer(str);
    //s.SendToClient(str);
    //r.killNode();
   // c.sendToServer(str);
    /*c.connectTo();
    r.reviveNode();
    c.connectTo();*/
   // QString message = "test";
   // c.sendToServer(message);
    return a.exec();
}
