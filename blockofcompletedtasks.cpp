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