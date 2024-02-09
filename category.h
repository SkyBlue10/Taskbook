#pragma once

#include <QWidget>
#include "ui_category.h"
#include "task.h"
#include "taskbook.h"
#include "LogSystem.h"

class Category : public QWidget
{
	Q_OBJECT

public:
	Category(QString category_name, bool isNew = false, QWidget* parent = nullptr);
	~Category();

	void addTask(Task*);

private:
	Ui::CategoryClass ui;
	void write_to_file();
};
