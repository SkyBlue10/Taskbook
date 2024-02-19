#include "completetaskswindow.h"
#include <QDateTime>
#include <QMessageBox>
#include "LogSystem.h"
#include "taskbook.h"

CompleteTasksWindow::CompleteTasksWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setMinimumSize(700, 400);

	connect(ui.qpbDeleteTasks, &QPushButton::clicked, this, [=]() {
		if (cTasks.size() == 0) {
			QMessageBox msg(QMessageBox::Icon::Warning, "Lack of tasks", "No completed tasks, nothing to delete");
			msg.exec();
			return;
		}
		deleteAllTasks();
		});
}

CompleteTasksWindow::~CompleteTasksWindow()
{}

void CompleteTasksWindow::addTask(Task* task)
{
	LogSystem::Write("Call CompleteTasksWindow::addTask(Task* task)");
	LogSystem::Write(task, "Adding a task to CompleteTasksWindow");
	for (int i = 0; i < cTasks.size(); i++)
	{
		if (cTasks[i]->getDate() == task->getEndDate()) {
			LogSystem::Write("A BlockOfCompletedTasks with a similar date was found (" + task->getEndDate().toStdString() + ")");
			cTasks[i]->addTask(task);
			LogSystem::Write("Adding a task to BlockOfCompletedTasks was successful");
			return;
		}
	}

	LogSystem::Write("Creating a new BlockOfCompletedTasks");
	BlockOfCompletedTasks* newblock = new BlockOfCompletedTasks(task->getEndDate());
	newblock->addTask(task);
	cTasks.push_back(newblock);

	QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui.qsaBlocks->layout());
	layout->insertWidget(0, newblock);

	LogSystem::Write("Creating a new BlockOfCompletedTasks was successful");
	LogSystem::Write("End of the call CompleteTasksWindow::addTask(Task* task)\n");
}

void CompleteTasksWindow::del(QString date)
{
	LogSystem::Write("Call CompleteTasksWindow::del(QString date)");
	LogSystem::Write("Deleting a task from a BlockOfCompletedTasks with a date " + date.toStdString());
	for (int i = 0; i < cTasks.size(); i++)
	{
		if (cTasks[i]->getDate() == date) {
			cTasks[i]->setCount(cTasks[i]->getCount() - 1);
			check(date);
			LogSystem::Write("Deleting a task from a BlockOfCompletedTasks with a date " + date.toStdString() + " was successful");
			return;
		}
	}
	LogSystem::Write("End of the call CompleteTasksWindow::del(QString date)\n");
}

void CompleteTasksWindow::deleteAllTasks() 
{
	TaskBook* tb = TaskBook::getInstance();
	LogSystem::Write("Call CompleteTasksWindow::deleteAllTasks()");
	for (auto iter = cTasks.begin(); iter < cTasks.end();)
	{
		BlockOfCompletedTasks* temp = *iter;
		std::vector<Task*> tasks = temp->getTasks();
		for (auto task : tasks) 
		{
			tb->delFromDB(task);
		}
		iter = cTasks.erase(iter);
		delete temp;
	}
	LogSystem::Write("End of the call CompleteTasksWindow::deleteAllTasks()");
}

void CompleteTasksWindow::check(QString date)
{
	LogSystem::Write("Call CompleteTasksWindow::check(QString date)");
	LogSystem::Write("Checking for empty BlocksOfCompletedTasks");
	for (int i = 0; i < cTasks.size(); i++)
	{
		if (cTasks[i]->getCount() == 0) {
			LogSystem::Write("Deleting a BlockOfCompletedTasks with a date " + date.toStdString());
			BlockOfCompletedTasks* temp = cTasks[i];
			cTasks.erase(cTasks.begin() + i);
			delete temp;
			LogSystem::Write("Deleting a BlockOfCompletedTasks with a date " + date.toStdString() + " was successful");
			return;
		}
	}
	LogSystem::Write("Empty BlocksOfCompletedTasks not found");
	LogSystem::Write("End of the call CompleteTasksWindow::del(QString date)\n");
}