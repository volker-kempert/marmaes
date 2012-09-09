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

#ifndef CONFIG_INPUT_CHARACTERISTICS_DIALOG_H
#define CONFIG_INPUT_CHARACTERISTICS_DIALOG_H

#include <QtGui/QWidget>
#include "ui_image_context_templates.h"
#include "marmeas_core/image_context/image_context_template.h"


class ConfigInputCharacteristicsDialog : public QDialog
{
    Q_OBJECT

public:
    ConfigInputCharacteristicsDialog(QWidget *parent = 0);
    ~ConfigInputCharacteristicsDialog();

private slots:
	void apply();           // take all the changes and update settings
	void browseDirectory(); // determine the image context storage directory
	void activateUnit(int unit_index);  // allow/set edits of unit field
	void activateApply() { ui.Apply->setEnabled(true); }

private:
	void updateFromSettings();
    Ui::InputCharacteristics ui;
    ImageContextTemplate config_input_;

};




#endif // CONFIG_INPUT_CHARACTERISTICS_DIALOG_H
