#include "taskbook.h"
#include <QApplication>

TaskBook* TaskBook::instance = NULL;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    TaskBook* w = TaskBook::getInstance();
    w->show();
    app.exec();
    delete w;
    return 0;
}