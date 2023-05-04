#include <QCoreApplication>
#include <Retranslator.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Retranslator r1("node2.txt");
    //Retranslator r2("node2.txt");

    return a.exec();
}
