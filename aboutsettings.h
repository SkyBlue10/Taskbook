#pragma once

#include <QWidget>
#include "ui_aboutsettings.h"

class AboutSettings : public QWidget
{
	Q_OBJECT

public:
	AboutSettings(QWidget* parent = nullptr);
	~AboutSettings();

private:
	Ui::AboutSettingsClass ui;
};
