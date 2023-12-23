#pragma once

#include <QDialog>
#include "ui_edittaskwindow.h"
#include "task.h"

class EditTaskWindow : public QDialog
{
	Q_OBJECT

public:
	EditTaskWindow(Task* edited, QWidget* parent = nullptr);
	~EditTaskWindow();

	QString getNewName();
	QString getNewText();
	QDateTime getNewDate();
	QString getNewInmportant();

private:
	Ui::EditTaskWindowClass ui;
};
