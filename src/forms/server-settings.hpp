#pragma once

#include <QDialog>

#include "ui_server-settings.h"

class ServerSettings : public QDialog {
	Q_OBJECT
public:
	explicit ServerSettings(QWidget *parent = 0);
	~ServerSettings();

	void showEvent(QShowEvent *event);
	void ToggleShowHide();
    
private slots:
	void onFormAccepted();
	void onFormCancel();

private:
	Ui::ServerSettings *ui;
};
