#include "UpdateTools.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UpdateTools w;
    w.show();
    return a.exec();
}
