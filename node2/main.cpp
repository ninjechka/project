#include "server.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server mServer(6003, "127.0.0.1");
    return a.exec();
}
