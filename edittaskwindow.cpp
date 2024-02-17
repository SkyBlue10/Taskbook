#include "edittaskwindow.h"

EditTaskWindow::EditTaskWindow(Task* edited, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(650, 330);

	connect(ui.pbAccept, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.pbReject, SIGNAL(clicked()), this, SLOT(reject()));

	ui.qlOldName->setText("Old value: " + edited->getName());
	ui.qlOldText->setText("Old value: " + edited->getText());
	ui.qlOldDate->setText("Old value: " + edited->getDate());
	ui.qlOldImportant->setText("Old value: " + edited->getImportant());

	ui.qleNameTask->setText(edited->getName());
	ui.qleTextTask->setText(edited->getText());

	int d = stoi(edited->getDate().toStdString().substr(0, 2)), m = stoi(edited->getDate().toStdString().substr(3, 2)), 
		y = stoi(edited->getDate().toStdString().substr(6, 4));
	ui.qdeDate->setDateTime(QDateTime(QDate(y, m, d+1), QTime()));

	ui.qleImportantTask->setText(edited->getImportant());
}

EditTaskWindow::~EditTaskWindow()
{}

QString EditTaskWindow::getNewName()
{
	return ui.qleNameTask->text();
}

QString EditTaskWindow::getNewText()
{
	return ui.qleTextTask->text();
}

QDateTime EditTaskWindow::getNewDate()
{
	return ui.qdeDate->dateTime();
}

QString EditTaskWindow::getNewInmportant()
{
	return ui.qleImportantTask->text();
}