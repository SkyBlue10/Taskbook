#include "addcategorywindow.h"
#include "LogSystem.h"
#include <QMessageBox>

AddCategoryWindow::AddCategoryWindow(QTabWidget* categories, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(500, 110);
	this->categories = categories;

	connect(ui.pbAccept, &QPushButton::clicked, this, [=]() {
		QString nameCategory = getQLEInputCategory();
		LogSystem::Write("The user adds a new category named " + nameCategory.toStdString());
		if (nameCategory.isEmpty())
		{
			LogSystem::Write("The user tried to add a category without a name");
			QMessageBox msg(QMessageBox::Icon::Warning, "Creating a category without a name", "The field with the category name is empty, fill it in");
			msg.exec();
			return;
		}
		int count = categories->count();
		for (int i = 0; i < count; i++)
		{
			if (nameCategory == categories->widget(i)->objectName()) {
				LogSystem::Write("The user was trying to add an already existing category");
				QMessageBox msg(QMessageBox::Icon::Warning, "Creating an existing category", "A category with that name already exists");
				msg.exec();
				return;
			}
		}
		LogSystem::Write("The new category has been successfully added");
		accept();
		});
	connect(ui.pbReject, SIGNAL(clicked()), this, SLOT(reject()));
}

AddCategoryWindow::~AddCategoryWindow()
{}

QString AddCategoryWindow::getQLEInputCategory()
{
	return ui.qleInput->text();
}