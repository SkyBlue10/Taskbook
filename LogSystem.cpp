#include "LogSystem.h"

bool LogSystem::Open()
{
	std::string path = "./logs/log_" + QDateTime::currentDateTime().toString("ddMMyyyy_hhmmss").toStdString() + ".txt";
	logFile = new std::ofstream;
	logFile->open(path, std::ios::app);
	return logFile->is_open();
}

void LogSystem::Close()
{
	if (logFile->is_open())
		logFile->close();
	delete logFile;
}

void LogSystem::Write(std::string str)
{
	if (logFile->is_open()) {
		*logFile << "[" << QDateTime::currentDateTime().toString("dd_MM_yyyy:hh_mm_ss").toStdString() << "]\t" << str << "\n";
	}
}

void LogSystem::Write(Task* task, std::string str)
{
	if (logFile->is_open()) {
		*logFile << "[" << QDateTime::currentDateTime().toString("dd_MM_yyyy:hh_mm_ss").toStdString() << "]\t" << str << "\n\tnametask: " <<
			task->getName().toStdString() << "\n\ttexttask: " << task->getText().toStdString() << "\n\tdeadline: " << task->getDate().toStdString()
			<< "\n\timportant: " << task->getImportant().toStdString() << "\n\tcategory: " << task->getCategory().toStdString() << "\n\tid: "
			<< task->getId() << "\n\tpair id: " << task->getPairId() << "\n\tcomplete: " << task->isComplete() << "\n";
	}
}

//не создаётся файл, разобраться. Возможно дело в в функции open(), а именно в std::ios::app