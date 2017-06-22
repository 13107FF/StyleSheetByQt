#include "stylesheetdialog.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile f("://default.qss");
    if (f.open(QIODevice::ReadOnly))
        a.setStyleSheet(f.readAll());

    StyleSheetDialog w;
    w.show();

    return a.exec();
}
