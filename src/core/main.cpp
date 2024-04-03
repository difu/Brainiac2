#include "../gui/mainwindow.h"

#include <QApplication>
#include "agent.h"

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{file}:%{line} %{function} -> %{if-category}%{category}: %{endif}%{message}");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Agent agent;
    return a.exec();
}
