#include <QCoreApplication>
#include <server.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server mServer(6000, "127.0.0.1");

    return a.exec();
}