#include <QCoreApplication>
#include <server.h>
#include <client.h>
#include <resiver.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server s;
    Resiver r;
    Client c;
   // QString message = "test";
   // c.sendToServer(message);
    return a.exec();
}
