#include "menuwindow.h"
#include "aboutsettings.h"
#include <QStackedWidget>

MenuWindow::MenuWindow(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setFixedSize(800, 600);

	QStackedWidget* stack = new QStackedWidget;
	AboutSettings* about = new AboutSettings();

	stack->setFrameStyle(1);
	stack->addWidget(new QWidget());
	stack->addWidget(about);

	QHBoxLayout* layout = qobject_cast<QHBoxLayout*>(this->layout());
	layout->addWidget(stack);

	layout->setStretch(0, 1);
	layout->setStretch(1, 8);


	connect(ui.qpbAboutApp, &QPushButton::clicked, stack, [stack]() {
		stack->setCurrentIndex(1);
		});


}

MenuWindow::~MenuWindow()
{}