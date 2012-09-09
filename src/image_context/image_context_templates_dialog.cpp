/*
   Copyright (C) 2012, Volker Kempert <volker.kempert@gmx.net>

   This file is part of MARMEAS application

   MARMEAS is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of
   the License, or (at your option) any later version.

   MARMEAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public
   License along with MARMEAS. If not, see <http://www.gnu.org/licenses/>.

*/

#include <QDebug>
#include <QRegExp>
#include <QString>
#include <QFileDialog>
#include "image_context_templates_dialog.h"

ConfigInputCharacteristicsDialog::ConfigInputCharacteristicsDialog(QWidget *parent)
    : QDialog(parent), config_input_("default")
{
	ui.setupUi(this);
	connect(ui.Close, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.Apply, SIGNAL(clicked()), this, SLOT(apply()));
	connect(ui.actionBrowse_Directory, SIGNAL(clicked()),
			this, SLOT(browseDirectory()));

	QRegExp float_exp("[0-9]+(\\.[0-9]+)?");
	ui.ConversationFactor->setValidator(new QRegExpValidator(float_exp, this));
	connect(ui.UnitOfMeasurement,SIGNAL(currentIndexChanged(int)),
	   	this, SLOT(activateUnit(int)));
	QRegExp small_word_exp("[a-z]*");
	ui.CustomParameter1->setValidator(new QRegExpValidator(small_word_exp, this));
	ui.CustomParameter2->setValidator(new QRegExpValidator(small_word_exp, this));
	connect(ui.CustomParameter1,SIGNAL(textChanged(QString)),
			this, SLOT(activateApply()));
	connect(ui.CustomParameter2,SIGNAL(textChanged(QString)),
			this, SLOT(activateApply()));
	connect(ui.ConversationFactor,SIGNAL(textChanged(QString)),
			this, SLOT(activateApply()));
	connect(ui.ImageContextDirectory, SIGNAL(textChanged(QString)),
			this, SLOT(activateApply()));
	connect(ui.FilePattern, SIGNAL(textChanged(QString)),
			this, SLOT(activateApply()));
	connect(ui.UnitOfMeasurement,SIGNAL(currentIndexChanged(int)),
	   	this, SLOT(activateApply()));
	QRegExp file_pattern_exp("(\\$p1\\$|\\$p2\\$|\\$i\\$|[/_-0-9a-zA-Z])+");
	ui.FilePattern->setValidator(
			new QRegExpValidator(file_pattern_exp, this));

	// fill in the fields with the appropriate settings
	updateFromSettings();
}

void ConfigInputCharacteristicsDialog::updateFromSettings() {
	ui.CustomParameter1->setText(config_input_.custom_param_1);
	ui.CustomParameter2->setText(config_input_.custom_param_2);
	int item = ui.UnitOfMeasurement->findText(
			config_input_.unit_of_measurement);
	ui.UnitOfMeasurement->setCurrentIndex(item);
	ui.ConversationFactor->setText(QString::number(
			config_input_.conversation_factor));
	ui.ImageContextDirectory->setText(
			config_input_.directory);
	ui.FilePattern->setText(
			config_input_.file_pattern);
	ui.Apply->setEnabled(false);
}
ConfigInputCharacteristicsDialog::~ConfigInputCharacteristicsDialog()
{

}


void ConfigInputCharacteristicsDialog::apply()
{
	qDebug() << "--ConfigInputCharacteristicsDialog:apply-- ";
	ui.Apply->setEnabled(false);

	// retrieve the date and update the settings
	config_input_.conversation_factor =
		ui.ConversationFactor->text().toFloat();
	config_input_.unit_of_measurement =
		ui.UnitOfMeasurement->itemText(
				ui.UnitOfMeasurement->currentIndex());
	config_input_.custom_param_1      = ui.CustomParameter1->text();
	config_input_.custom_param_2      = ui.CustomParameter2->text();

	config_input_.directory   = 	ui.ImageContextDirectory->text();
	config_input_.file_pattern  =
			ui.FilePattern->text();
	// make the settings persistent
	config_input_.writeSettings();
}

// determine the image context storage directory
void ConfigInputCharacteristicsDialog::browseDirectory() {
	qDebug() << "--ConfigInputCharacteristicsDialog::browseDirectory--";
	QString dir = QFileDialog::getExistingDirectory(this,
			tr("Select Image Context Directory"),
			ui.ImageContextDirectory->text(),
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	ui.ImageContextDirectory->setText(dir);
}


void ConfigInputCharacteristicsDialog::activateUnit(int unit_index )
{
	qDebug() << "--ConfigInputCharacteristicsDialog::activateUnit--";
	switch (unit_index) {
	case 0:  // Pixel
		ui.ConversationFactor->setEnabled(false);
		ui.ConversationFactor->setText("1.0");
		break;
	default:
		ui.ConversationFactor->setEnabled(true);
		break;
	}
}



