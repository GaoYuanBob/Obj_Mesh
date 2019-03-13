#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
	w.setMinimumSize(800, 600);
	w.resize(1200, 800);

    w.show();
	// w.showMaximized(); // »´∆¡œ‘ æ

    return a.exec();
}
