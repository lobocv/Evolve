#include "evolveui.h"
#include <QApplication>
#include <evolve.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EvolveUI w;
    w.show();

    return a.exec();
}
