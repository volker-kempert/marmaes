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

#include "image_context_dialog.h"

ImageContextDialog::ImageContextDialog(QWidget *parent, ImageContext* input )
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.close, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.apply, SIGNAL(clicked()), this, SLOT(apply()));
	QRegExp word_exp("[a-zA-Z0-9]*");
	ui.Param1Data->setValidator(new QRegExpValidator(word_exp, this));
	connect(ui.Param1Data,SIGNAL(textChanged(QString)),
			this, SLOT(activateApply()));
	ui.Param2Data->setValidator(new QRegExpValidator(word_exp, this));
	connect(ui.Param2Data,SIGNAL(textChanged(QString)),
			this, SLOT(activateApply()));
	input_ = input;

	QRegExp float_exp("[0-9]+(\\.[0-9]+)?");
	ui.editConversionFactor->setValidator(new QRegExpValidator(float_exp, this));
	connect(ui.editConversionFactor,SIGNAL(textChanged(QString)),
			this, SLOT(activateApply()));

	connect(ui.comboUnitOfMeasurement,SIGNAL(currentIndexChanged(int)),
	   	this, SLOT(activateUnit(int)));
	connect(ui.comboUnitOfMeasurement,SIGNAL(currentIndexChanged(int)),
	   	this, SLOT(activateApply()));

	int item = ui.comboUnitOfMeasurement->findText(
			input_->getUnitOfMeasurement());
	ui.comboUnitOfMeasurement->setCurrentIndex(item);
	ui.editConversionFactor->setText(QString::number(
			input_->getConversionFactor()));

	// overwrite the custom data and show what is available
	if (input_->getParam1Name() == "") {
		ui.Param1Name->setVisible(false);
		ui.Param1Data->setVisible(false);
	} else {
		ui.Param1Name->setText(input_->getParam1Name());
		ui.Param1Data->setText(input_->getParam1Data());
	}
	if (input_->getParam2Name() == "" ) {
		ui.Param2Name->setVisible(false);
		ui.Param2Data->setVisible(false);
	} else {
		ui.Param2Name->setText(input_->getParam2Name());
		ui.Param2Data->setText(input_->getParam2Data());
	}
	ui.apply->setEnabled(false);

}


void ImageContextDialog::apply() {
	qDebug("--ImageContextDialog::apply--");


	// save the data to input_
	input_->setConversionFactor(ui.editConversionFactor->text().toFloat());
	input_->setUnitOfMeasurement(ui.comboUnitOfMeasurement->itemText(
			ui.comboUnitOfMeasurement->currentIndex()));
	input_->setParam1Data(ui.Param1Data->text());
	input_->setParam2Data(ui.Param2Data->text());
	ui.apply->setEnabled(false);
	qDebug("--ImageContextDialog::apply-- emit imageContextChanged");
	emit imageContextChanged();
}

ImageContextDialog::~ImageContextDialog() {
	// TODO Auto-generated destructor stub
	// disconnect - or does it the parent?
}
