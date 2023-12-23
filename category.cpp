#include "category.h"
#include <QTabWidget>

Category::Category(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

}

Category::~Category()
{}

void Category::addTask(Task* task)
{
	QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui.qsaArea->widget()->layout());
	layout->insertWidget(0, task);
}