#pragma once

#include <QWidget>
#include "ui_blockofcompletedtasks.h"
#include "task.h"

class BlockOfCompletedTasks : public QWidget
{
	Q_OBJECT

public:
	BlockOfCompletedTasks(QString date, QWidget *parent = nullptr);
	~BlockOfCompletedTasks();

	void addTask(Task* task);

	void setDate(QString date);
	QString getDate();
	void setCount(int count);
	int getCount();

private:
	Ui::BlockOfCompletedTasksClass ui;
	int count_tasks_in_block;
};