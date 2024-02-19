#include "task.h"
#include "taskbook.h"
#include "edittaskwindow.h"
#include "LogSystem.h"
#include <QCheckBox>
#include <QMessageBox>

Task::Task(QString name, QString text, QString date, QString important, QString category, QWidget* parent) : QWidget(parent)
{
	ui.setupUi(this);
	
	ui.qlNameTask->setText(name);
	ui.qlTextTask->setText(text);
	this->setDate(date);
	ui.qlImportant->setText(important);
	ui.qlCategory->setText(category);
	this->setAttribute(Qt::WA_StyledBackground, true);
	this->check_deadline();

	connect(ui.pbDelete, SIGNAL(clicked()), this, SLOT(deleteTask()));
	connect(ui.pbEdit, SIGNAL(clicked()), this, SLOT(editTask()));
	connect(ui.qcbIsComplete, SIGNAL(stateChanged(int)), this, SLOT(onStateChanged(int)));
	connect(this, SIGNAL(dateChanged()), this, SLOT(check_deadline()));
}

Task::Task(Task* task, QWidget* parent) : Task(task->getName(), task->getText(),
	task->getDate(), task->getImportant(), task->getCategory())
{
	this->setId(task->getId());
	this->setPairId(task->getPairId());
}

Task::Task(QSqlQuery query, QSqlRecord rec) : Task(query.value(rec.indexOf("nameTask")).toString(),
	query.value(rec.indexOf("textTask")).toString(),
	query.value(rec.indexOf("date")).toString(),
	query.value(rec.indexOf("important")).toString(),
	query.value(rec.indexOf("category")).toString())
{
	this->setId(query.value(rec.indexOf("id")).toInt());
	this->setPairId(query.value(rec.indexOf("pair_id")).toInt());
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

void Task::setPairId(int pair_id)
{
	this->pair_id = pair_id;
}

int Task::getId()
{
	return id;
}

int Task::getPairId()
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
	return ui.qlDeadline->text();
}

QString Task::getImportant()
{
	return ui.qlImportant->text();
}

QString Task::getCategory()
{
	return ui.qlCategory->text();
}

QString Task::getEndDate()
{
	return end_date;
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
	if (date != ui.qlDeadline->text()) 
	{
		ui.qlDeadline->setText(date);
		emit dateChanged();
	}
}

void Task::setImportant(QString important)
{
	ui.qlImportant->setText(important);
}

void Task::setCategory(QString category)
{
	ui.qlCategory->setText(category);
}

void Task::setEndDate(QString end_date)
{
	this->end_date = end_date;
}

bool Task::isComplete()
{
	return ui.qcbIsComplete->isChecked();
}

void Task::deleteTask() {
	LogSystem::Write("Call Task::deleteTask() function");
	QMessageBox msg(QMessageBox::Icon::Question, "Are you sure?", "Are you sure you want to delete the task? This action will permanently delete the task");
	msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msg.setDefaultButton(QMessageBox::No);
	if (msg.exec() == QMessageBox::No) {
		LogSystem::Write("msg.exec() return No");
		LogSystem::Write("End of the call Task::deleteTask() function\n");
		return;
	}

	LogSystem::Write("msg.exec() return Yes");
	TaskBook* tb = TaskBook::getInstance();
	tb->delTask(this->id);
	LogSystem::Write("End of the call Task::deleteTask() function\n");
}

void Task::editTask()
{
	LogSystem::Write("Call Task::editTask() function");
	EditTaskWindow dlg(this, this);
	switch (dlg.exec())
	{
	case QDialog::Accepted: {
		LogSystem::Write("dlg.exec() return QDialog::Accepted");
		LogSystem::Write(this, "Editing a task");
		TaskBook* tb = TaskBook::getInstance();

		ui.qlNameTask->setText(dlg.getNewName());
		ui.qlTextTask->setText(dlg.getNewText());
		this->setDate(dlg.getNewDate().toString("dd.MM.yyyy"));
		ui.qlImportant->setText(dlg.getNewInmportant());
		tb->delFromDB(this);
		tb->writeInDB(this);
		LogSystem::Write(this, "The task was edited successfully");

		if (pair_id != -1) {
			tb->editTask(id);
		}
		break;
	}
	case QDialog::Rejected: {
		LogSystem::Write("dlg.exec() return QDialog::Rejected");
		LogSystem::Write("The user rejected the editing of a task");
		break;
	}
	default:
		LogSystem::Write("dlg.exec() return default");
		break;
	}
	LogSystem::Write("End of the call Task::editTask() function\n");
}

void Task::onStateChanged(int checked)
{
	//примечание: если я в styleSheet QLabel`ов добавлю что-нибудь при разработке дизайна
	//то нужно будет переписать код ниже, чтобы пред настройки стилей не пропадали, а только линия

	LogSystem::Write("Call Task::onStateChanged() function");
	if (checked == Qt::Checked) { //если задачу отмечают как выполненную
		LogSystem::Write("checked == Qt::Checked");
		LogSystem::Write(this, "The task was marked as completed");
		QSqlDatabase db = QSqlDatabase::database();
		db.open();
		QSqlQuery query("SELECT MAX(id) AS 'max_id' FROM Complete;"); //получаем самый высокий id среди всех complete задач
		QSqlRecord rec = query.record();
		query.next();

		Task* complete_task = new Task(this);
		complete_task->setId(query.value(rec.indexOf("max_id")).toInt() + 1);
		this->getPairId() == -1 ? complete_task->setPairId(-1) : complete_task->setPairId(0);
		db.close();
		LogSystem::Write(complete_task, "A copy of the task has been created for CompleteTasksWindow");
		complete_task->prepare_complete_task();

		//мы отмечаем задачу не выполненной для того, чтобы при её заносе в БД случайно не удалить complete_task
		bool oldState = ui.qcbIsComplete->blockSignals(true);
		ui.qcbIsComplete->setChecked(false);
		ui.qcbIsComplete->blockSignals(oldState);

		TaskBook* tb = TaskBook::getInstance();
		tb->addComplete(complete_task, true);
		tb->delTask(this->id);
	}
	else { //если задачу переотмечают как невыполненную
		LogSystem::Write("checked != Qt::Checked");
		LogSystem::Write(this, "The task was marked as noncompleted");
		Task* task_for_category = nullptr;
		Task* task_for_all = new Task(this);

		if (task_for_all->getPairId() != -1)
			task_for_category = new Task(this);

		QSqlDatabase db = QSqlDatabase::database();
		db.open();
		QSqlQuery query;
		query.exec("DELETE FROM Complete WHERE id = '" + QString::number(this->id) + "';");
		db.close();
		QString endDate = this->getEndDate();


		LogSystem::Write(this, "Deleting a task from memory");
		if (this != nullptr)
			delete this;

		TaskBook* tb = TaskBook::getInstance();
		tb->addFromComplete(task_for_all, task_for_category, endDate);
	}
	LogSystem::Write("End of the call Task::onStateChanged() function\n");
}

void Task::prepare_complete_task()
{
	this->setEndDate(QDate::currentDate().toString("dd.MM.yyyy"));
	ui.pbDelete->hide();
	ui.pbEdit->hide();
	bool oldState = ui.qcbIsComplete->blockSignals(true);
	ui.qcbIsComplete->setChecked(true);
	ui.qcbIsComplete->blockSignals(oldState);
	ui.qlNameTask->setStyleSheet("text-decoration: line-through;");
	ui.qlTextTask->setStyleSheet("text-decoration: line-through;");
}

void Task::check_deadline()
{
	std::string date_str = ui.qlDeadline->text().toStdString();
	QDate task_deadline(stoi(date_str.substr(6, 4)), stoi(date_str.substr(3, 2)), stoi(date_str.substr(0, 2)));
	if (task_deadline < QDate::currentDate())
		ui.qlDeadline->setStyleSheet("color: red");
	else
		ui.qlDeadline->setStyleSheet("color: black");
}