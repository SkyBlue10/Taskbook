#pragma once

#include "ui_task.h"
#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <qsqlrecord.h>

class Task : public QWidget
{
	Q_OBJECT

public:
	Task(QString, QString, QString, QString, QString, QWidget *parent = nullptr);
	Task(Task*, QWidget *parent = nullptr);
	Task(QSqlQuery query, QSqlRecord rec);
	Task(QWidget *parent = nullptr);
	~Task();

	void setId(int id);
	void setPairId(int pair_id);

	int getId();
	int getPairId();

	QString getName();
	QString getText();
	QString getDate();
	QString getImportant();
	QString getCategory();
	QString getEndDate();

	void setName(QString);
	void setText(QString);
	void setDate(QString);
	void setImportant(QString);
	void setCategory(QString);
	void setEndDate(QString);

	bool isComplete();

	void prepare_complete_task();

public slots:
	void deleteTask();
	void editTask();
	void onStateChanged(int);
	void check_deadline();

signals:
	void dateChanged();

private:
	Ui::TaskClass ui;
	int id;//id самого задачника
	int pair_id;//id пары (-1, если пара отсуствует)
	QString end_date; //дата завершения задачи (только для complete задач)
};