#include <QtGui/QApplication>
#include "PlayerShell.hpp"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    PlayerShell w;
    w.show();

    return a.exec();
}
