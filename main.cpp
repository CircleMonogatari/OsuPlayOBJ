#include "mainosu.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);




    Mainosu w;


    //w.resize(availableGeometry.width() / 3, availableGeometry.height() / 1);
    w.resize(400, 500);
    w.show();

    return a.exec();
}
