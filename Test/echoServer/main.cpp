#include <QCoreApplication>
#include "echoserver.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    EchoServer server;
    server.listen(QHostAddress::Any, 7);
    return a.exec();
}
