#include "eps32rgb.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Eps32RGB w;
    w.show();
    return a.exec();
}
