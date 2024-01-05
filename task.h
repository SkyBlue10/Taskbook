#pragma once

#include <QWidget>
#include "ui_task.h"

class Task : public QWidget
{
	Q_OBJECT

public:
	Task(QString, QString, QString, QString, QString, QWidget *parent = nullptr);
	Task(QWidget *parent = nullptr);
	~Task();

	void setId(int id);
	void setIdPair(int pair_id);

	int getId();
	int getIdPair();

	QString getName();
	QString getText();
	QString getDate();
	QString getImportant();
	QString getCategory();

	void setName(QString);
	void setText(QString);
	void setDate(QString);
	void setImportant(QString);
	void setCategory(QString);

public slots:
	void deleteTask();
	void editTask();
	void onStateChanged(int);

signals:


private:
	Ui::TaskClass ui;
	int id;//id самого задачника
	int pair_id;//id пары (-1, если пара отсуствует)

	void prepare_complete_task();
};