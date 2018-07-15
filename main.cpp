#include "mainwindow.hh"
#include <QApplication>
#include <QStyleFactory>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setStyle("fusion");
    //a.setStyle(QStyleFactory::create("Fusion"));
    qDebug() << QStyleFactory::keys();
    MainWindow w;
    w.show();

    return a.exec();
}
