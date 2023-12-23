#include "edittaskwindow.h"

EditTaskWindow::EditTaskWindow(Task* edited, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(650, 330);

	connect(ui.pbAccept, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.pbReject, SIGNAL(clicked()), this, SLOT(reject()));

	ui.qlOldName->setText("Old name: " + edited->getName());
	ui.qlOldText->setText("Old text: " + edited->getText());
	ui.qlOldDate->setText("Old date: " + edited->getDate());
	ui.qlOldImportant->setText("Old important: " + edited->getImportant());

	ui.qleNameTask->setText(edited->getName());
	ui.qleTextTask->setText(edited->getText());
	//ui.qdteDate->setDateTime(edited->getDate());
	//нужно чтото придумать в будущем с этим, пока что пусть там по умолчанию будет всё
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
	return ui.qdteDate->dateTime();
}

QString EditTaskWindow::getNewInmportant()
{
	return ui.qleImportantTask->text();
}