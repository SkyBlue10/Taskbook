#include "task.h"
#include "taskbook.h"
#include "edittaskwindow.h"
#include <QCheckBox>

Task::Task(QString name, QString text, QString date, QString important, QString category, QWidget* parent) : QWidget(parent)
{
	ui.setupUi(this);
	
	ui.qlNameTask->setText(name);
	ui.qlTextTask->setText(text);
	ui.qlDaetPeriod->setText(date);
	ui.qlImportant->setText(important);
	ui.qlCategory->setText(category);

	connect(ui.pbDelete, SIGNAL(clicked()), this, SLOT(deleteTask()));
	connect(ui.pbEdit, SIGNAL(clicked()), this, SLOT(editTask()));
	connect(ui.qcbIsComplete, SIGNAL(stateChanged(int)), this, SLOT(onStateChanged(int)));

}

Task::Task(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

Task::~Task()
{}

void Task::setId(int id)
{
	this->id = id;
}

void Task::setIdPair(int pair_id)
{
	this->pair_id = pair_id;
}

int Task::getId()
{
	return id;
}

int Task::getIdPair()
{
	return pair_id;
}

QString Task::getName()
{
	return ui.qlNameTask->text();
}

QString Task::getText()
{
	return ui.qlTextTask->text();
}

QString Task::getDate()
{
	return ui.qlDaetPeriod->text();
}

QString Task::getImportant()
{
	return ui.qlImportant->text();
}

QString Task::getCategory()
{
	return ui.qlCategory->text();
}

void Task::setName(QString name)
{
	ui.qlNameTask->setText(name);
}

void Task::setText(QString text)
{
	ui.qlTextTask->setText(text);
}

void Task::setDate(QString date)
{
	ui.qlDaetPeriod->setText(date);
}

void Task::setImportant(QString important)
{
	ui.qlImportant->setText(important);
}

void Task::setCategory(QString category)
{
	ui.qlCategory->setText(category);
}

void Task::deleteTask() {
	TaskBook* tb = TaskBook::getInstance();
	tb->delTask(this->getId());
}

void Task::editTask()
{
	EditTaskWindow dlg(this, this);
	switch (dlg.exec())
	{
	case QDialog::Accepted: {
		ui.qlNameTask->setText(dlg.getNewName());
		ui.qlTextTask->setText(dlg.getNewText());
		ui.qlDaetPeriod->setText(dlg.getNewDate().toString());
		ui.qlImportant->setText(dlg.getNewInmportant());

		if (pair_id != NULL) {
			TaskBook* tb = TaskBook::getInstance();
			tb->editTask(id);
		}
		break;
	}
	case QDialog::Rejected: {
		break;
	}
	default:
		break;
	}
}

void Task::onStateChanged(int checked)
{
	//примечание: если я в styleSheet QLabel`ов добавлю что-нибудь при разработке дизайна
	//то нужно будет переписать код ниже, чтобы пред настройки стилей не пропадали, а только линия
	if (checked == Qt::Checked) {
		ui.qlNameTask->setStyleSheet("text-decoration: line-through;");
		ui.qlTextTask->setStyleSheet("text-decoration: line-through;");

	}
	else {
		ui.qlNameTask->setStyleSheet("");
		ui.qlTextTask->setStyleSheet("");

	}
}