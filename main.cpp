#include "mainwindow.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    /*QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LD_LIBRARY_PATH", "/usr/lib64/");
    QStringList listEnv = env.toStringList();
    for(int i = 0; i < listEnv.size(); ++i)
    {
        std::cout << listEnv.at(i).toStdString() << std::endl;
    }*/

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    qputenv("LD_LIBRARY_PATH", "/usr/lib64/");
    return a.exec();
}
