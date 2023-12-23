#include "addcategorywindow.h"

AddCategoryWindow::AddCategoryWindow(QTabWidget* categories, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(500, 110);
	this->categories = categories;

	connect(ui.pbAccept, SIGNAL(clicked()), this, SLOT(pbAcceptClicked()));
	connect(ui.pbReject, SIGNAL(clicked()), this, SLOT(reject()));
}

AddCategoryWindow::~AddCategoryWindow()
{}

QString AddCategoryWindow::getQLEInputCategory()
{
	return ui.qleInput->text();
}

void AddCategoryWindow::setQLError(QString error)
{
	ui.qlError->setText(error);
}

void AddCategoryWindow::pbAcceptClicked() {
	QString nameCategory = getQLEInputCategory();
	int count = categories->count();
	for (int i = 0; i < count; i++)
	{
		if (nameCategory == categories->widget(i)->objectName()) {
			setQLError("A category with that name already exists");
			return;
		}
	}
	accept();
}
