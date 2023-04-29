#include "receiver.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Receiver server(6002, "127:0:0:1");
    return a.exec();
}
