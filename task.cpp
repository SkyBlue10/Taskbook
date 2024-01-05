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
	tb->delTask(this->id);
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

		if (pair_id != -1) {
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

	if (checked == Qt::Checked) { //если задачу отмечают как выполненную
		Task* complete_task = new Task(this->ui.qlNameTask->text(),
			this->ui.qlTextTask->text(), this->ui.qlDaetPeriod->text(),
			this->ui.qlImportant->text(), this->ui.qlCategory->text());
		complete_task->setId(id);
		complete_task->setIdPair(pair_id);
		complete_task->prepare_complete_task();

		TaskBook* tb = TaskBook::getInstance();
		tb->addComplete(complete_task);
		tb->delTask(this->id);
		//исправить: при создании в категории таска и отметке выполненным в all ошибка
	}
	else { //если задачу переотмечают как невыполненную
		Task* task_for_category = nullptr;
		Task* task_for_all = new Task(this->ui.qlNameTask->text(),
			this->ui.qlTextTask->text(), this->ui.qlDaetPeriod->text(),
			this->ui.qlImportant->text(), this->ui.qlCategory->text());
		task_for_all->setId(this->id);
		task_for_all->setIdPair(this->pair_id);

		if (task_for_all->getIdPair() != -1) {
			task_for_category = new Task(this->ui.qlNameTask->text(),
				this->ui.qlTextTask->text(), this->ui.qlDaetPeriod->text(),
				this->ui.qlImportant->text(), this->ui.qlCategory->text());
			task_for_category->setId(task_for_all->getIdPair());
			task_for_all->setIdPair(task_for_all->getId());
		}
		if (this != nullptr)
			delete this;

		TaskBook* tb = TaskBook::getInstance();
		tb->addFromComplete(task_for_all, task_for_category);
	}
}

void Task::prepare_complete_task()
{
	ui.pbDelete->hide();
	ui.pbEdit->hide(); 
	bool oldState = ui.qcbIsComplete->blockSignals(true);
	ui.qcbIsComplete->setChecked(true);
	ui.qcbIsComplete->blockSignals(oldState);
	ui.qlNameTask->setStyleSheet("text-decoration: line-through;");
	ui.qlTextTask->setStyleSheet("text-decoration: line-through;");
}