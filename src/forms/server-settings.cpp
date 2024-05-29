#include "server-settings.hpp"

#include "../config.hpp"
#include "../vrbro.hpp"

ServerSettings::ServerSettings(QWidget *parent) : QDialog(parent), ui(new Ui::ServerSettings) {
	ui->setupUi(this);
	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onFormAccepted()));
	connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(onFormCancel()));
}

void ServerSettings::onFormAccepted() {
	auto conf = GetConfig();
	conf->ListenAddrString = ui->hostInput->text();
	conf->ListenPortNumber = ui->portInput->text();
    conf->AutoBufferBool = ui->autoBufferCheckBox->isChecked();

	conf->Save();
	ToggleShowHide();
}

void ServerSettings::onFormCancel() {
	ToggleShowHide();
}

void ServerSettings::showEvent(QShowEvent *) {
	auto conf = GetConfig();
	ui->hostInput->setText(conf->ListenAddrString);
	ui->portInput->setText(conf->ListenPortNumber);
    ui->autoBufferCheckBox->setChecked(conf->AutoBufferBool);
}

void ServerSettings::ToggleShowHide() {
	setVisible(!isVisible());
}

ServerSettings::~ServerSettings() {
	delete ui;
}
