#include <QApplication>
#include "mainw.h"
#include <QTranslator>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resource);
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load(":CN.qm");
    a.installTranslator(&translator);

    mainw w;
    w.setStyleSheet("font:bold 16pt");
    w.show();

    return a.exec();
}
