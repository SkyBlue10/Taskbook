#pragma once

#include <QDialog>
#include "ui_addtaskwindow.h"

class AddTaskWindow : public QDialog
{
	Q_OBJECT

public:
	AddTaskWindow(QString category, QWidget* parent = nullptr);
	~AddTaskWindow();

	QString getQLENameTask();
	QString getQLETextTask();
	QString getDatePeriodEnding();
	QString getCBImportant();

private:
	Ui::AddTaskWindowClass ui;
};