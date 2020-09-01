#include "lobby.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    lobby w;
    w.show();
    return a.exec();
}
