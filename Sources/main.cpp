#include "widget.h"
#include <QApplication>
#include "filerw.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FileRW file;
    Widget w;
    w.show();
    return a.exec();
}
