#pragma once
#include <string>
#include <fstream>
#include <QDateTime>
#include "task.h"

class LogSystem
{
private:
	static std::ofstream* logFile;

public:
	static bool Open();
	static void Close();
	static void Write(std::string str);
	static void Write(Task* task, std::string str);
};
