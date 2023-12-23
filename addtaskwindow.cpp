#include "addtaskwindow.h"

AddTaskWindow::AddTaskWindow(QString category, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(500, 300);
	if (category == "qwAll")
		ui.qlCurrentCategory->setText("All");
	else
		ui.qlCurrentCategory->setText(category);

	connect(ui.pbAccept, SIGNAL(clicked()), this, SLOT(accept()));
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
	return ui.qdtPeriodEnd->dateTime().toString();
}

QString AddTaskWindow::getCBImportant()
{
	return ui.qleImportant->text();
}
