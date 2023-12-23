#pragma once

#include <QWidget>
#include "ui_category.h"
#include "task.h"
#include "taskbook.h"

class Category : public QWidget
{
	Q_OBJECT

public:
	Category(QWidget* parent = nullptr);
	~Category();

	void addTask(Task*);

private:
	Ui::CategoryClass ui;
};
