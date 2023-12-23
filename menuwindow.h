#pragma once

#include <QDialog>
#include "ui_menuwindow.h"

class MenuWindow : public QDialog
{
	Q_OBJECT

public:
	MenuWindow(QWidget* parent = nullptr);
	~MenuWindow();

private:
	Ui::MenuWindowClass ui;
};