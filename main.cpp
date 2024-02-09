#include "taskbook.h"
#include "LogSystem.h"
#include <QApplication>

TaskBook* TaskBook::instance = NULL;
std::ofstream* LogSystem::logFile = nullptr;

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	TaskBook* w = TaskBook::getInstance();
	w->show();
	try
	{
		app.exec();
	}
	catch (const std::exception& ex)
	{
		LogSystem::Write("ERROR: " + std::string(ex.what()));
		LogSystem::Close();
	}
    delete w;
    return 0;
}