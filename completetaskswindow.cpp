#include "completetaskswindow.h"

CompleteTasksWindow::CompleteTasksWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setMinimumSize(700, 400);
}

CompleteTasksWindow::~CompleteTasksWindow()
{}

void CompleteTasksWindow::addTask(Task* task)
{
	for (int i = 0; i < cTasks.size(); i++)
	{
		if (cTasks[i]->getDate() == task->getDate()) {
			cTasks[i]->addTask(task);
			return;
		}
	}

	BlockOfCompletedTasks* newblock = new BlockOfCompletedTasks(task->getDate());
	newblock->addTask(task);
	cTasks.push_back(newblock);
	QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui.qsaBlocks->layout());
	layout->insertWidget(0, newblock);
}

void CompleteTasksWindow::del(QString date)
{
	for (int i = 0; i < cTasks.size(); i++)
	{
		if (cTasks[i]->getDate() == date) {
			cTasks[i]->setCount(cTasks[i]->getCount() - 1);
			check(date);
			return;
		}
	}
}

void CompleteTasksWindow::check(QString date)
{
	for (int i = 0; i < cTasks.size(); i++)
	{
		if (cTasks[i]->getCount() == 0) {
			BlockOfCompletedTasks* temp = cTasks[i];
			cTasks.erase(cTasks.begin() + i);
			delete temp;			
			return;
		}
	}
}