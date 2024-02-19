#include "addtaskwindow.h"
#include "LogSystem.h"
#include <QMessageBox>
#include <QRegularExpression>

AddTaskWindow::AddTaskWindow(QString category, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(500, 300);
	if (category == "qwAll")
		ui.qlCurrentCategory->setText("All");
	else
		ui.qlCurrentCategory->setText(category);
	ui.qdePeriodEnd->setDate(QDate::currentDate().addDays(1));

	connect(ui.pbAccept, &QPushButton::clicked, this, [=]() {
		ui.qleNameTask->setText(ui.qleNameTask->text().replace(QRegularExpression("^\\s+"), QString(""))); //заменяет пробел в начале qleNameTask на пустоту (если далее есть символы - пропускаме)
		ui.qleTextTask->setText(ui.qleTextTask->text().replace(QRegularExpression("^\\s+"), QString(""))); //заменяет пробел в начале qleTextTask на пустоту (если далее есть символы - пропускаме)
		ui.qleImportant->setText(ui.qleImportant->text().replace(QRegularExpression("^\\s+"), QString(""))); //заменяет пробел в начале qleImportant на пустоту (если далее есть символы - пропускаме)
		if (!ui.qleNameTask->text().isEmpty() && !ui.qleTextTask->text().isEmpty() && !ui.qleImportant->text().isEmpty())
			accept(); //если все поля не пустые
		else 
		{
			LogSystem::Write("The user did not fill in some field when creating a new task");
			QMessageBox msg(QMessageBox::Icon::Warning, "An empty field", "Some field is empty, fill in all fields");
			msg.exec();
		}
		});
	connect(ui.pbReject, SIGNAL(clicked()), this, SLOT(reject()));
}

AddTaskWindow::~AddTaskWindow()
{}

QString AddTaskWindow::getQLENameTask()
{
	return ui.qleNameTask->text();
}

QString AddTaskWindow::getQLETextTask()
{
	return ui.qleTextTask->text();
}

QString AddTaskWindow::getDatePeriodEnding()
{
	return ui.qdePeriodEnd->dateTime().toString("dd.MM.yyyy");
}

QString AddTaskWindow::getCBImportant()
{
	return ui.qleImportant->text();
}
