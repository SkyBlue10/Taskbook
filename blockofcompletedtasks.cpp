#include "blockofcompletedtasks.h"

BlockOfCompletedTasks::BlockOfCompletedTasks(QString date, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.qlDate->setText(date);
	count_tasks_in_block = 0;
}

BlockOfCompletedTasks::~BlockOfCompletedTasks()
{}

void BlockOfCompletedTasks::addTask(Task* task)
{
	ui.verticalLayout->insertWidget(0, task);
	count_tasks_in_block++;
}

std::vector<Task*> BlockOfCompletedTasks::getTasks()
{
	QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui.verticalLayout->layout());
	std::vector<Task*> tasks;
	for (int i = 0; i < layout->count(); i++)
	{
		tasks.push_back(qobject_cast<Task*>(layout->itemAt(i)->widget()));
	}
	tasks.erase(tasks.end() - 1); //избавляемся от null значения (от spacer`а)
	return tasks;
}

void BlockOfCompletedTasks::setDate(QString date)
{
	ui.qlDate->setText(date);
}

QString BlockOfCompletedTasks::getDate() 
{
	return ui.qlDate->text();
}

void BlockOfCompletedTasks::setCount(int count)
{
	this->count_tasks_in_block = count;
}

int BlockOfCompletedTasks::getCount()
{
	return count_tasks_in_block;
}