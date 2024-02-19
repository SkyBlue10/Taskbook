#pragma once

#include <vector>
#include <QDialog>
#include <QLabel>
#include "ui_completetaskswindow.h"
#include "task.h"
#include "blockofcompletedtasks.h"

class CompleteTasksWindow : public QDialog
{
	Q_OBJECT

public:
	CompleteTasksWindow(QWidget *parent = nullptr);
	~CompleteTasksWindow();

	void addTask(Task* task);
	void del(QString date);

public slots:
	void deleteAllTasks();

private:
	Ui::CompleteTasksWindowClass ui;
	std::vector<BlockOfCompletedTasks*> cTasks;

	void check(QString date);
};