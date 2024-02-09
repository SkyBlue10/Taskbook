#include "category.h"
#include <QTabWidget>
#include <fstream>

Category::Category(QString category_name, bool isNew, QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	LogSystem::Write("Creating a new category named " + category_name.toStdString());
	setObjectName(category_name);
	LogSystem::Write("Creating a new category named " + category_name.toStdString() + " was successful");

	if (isNew)
		write_to_file();
}

Category::~Category()
{}

void Category::addTask(Task* task)
{
	LogSystem::Write(task, "Adding a task to a category " + this->objectName().toStdString());
	QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui.qsaArea->widget()->layout());
	layout->insertWidget(0, task);
	LogSystem::Write(task, "Adding a task to a category " + this->objectName().toStdString() + " was successful");
}

void Category::write_to_file()
{
	LogSystem::Write("Adding a category " + this->objectName().toStdString() + " to a file");
	std::ofstream file;
	file.open("./data/categories", std::ios::app);
	file << this->objectName().toStdString() << std::endl;
	file.close();
	LogSystem::Write("Adding a category " + this->objectName().toStdString() + " to a file was successful");
}