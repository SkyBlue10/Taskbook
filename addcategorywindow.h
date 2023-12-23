#pragma once

#include <QDialog>
#include "ui_addcategorywindow.h"
#include <QTabWidget>

class AddCategoryWindow : public QDialog
{
	Q_OBJECT

public:
	AddCategoryWindow(QTabWidget* categories, QWidget* parent = nullptr);
	~AddCategoryWindow();

	QString getQLEInputCategory();
	void setQLError(QString error);

public slots:
	void pbAcceptClicked();

private:
	Ui::AddCategoryWindowClass ui;
	QTabWidget* categories;
};