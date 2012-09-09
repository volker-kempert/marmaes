/*
   Copyright (C) 2012, Volker Kempert <volker.kempert@gmx.net>

   This file is part of an MARMEAS library

   The MARMEAS library is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of
   the License, or (at your option) any later version.

   The MARMEAS library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with MARMEAS. If not, see <http://www.gnu.org/licenses/>.

*/

#include <QSettings>

#include "image_context_template.h"


//-------------------------------------------------------------------

ImageContextTemplate::ImageContextTemplate(QString n ) {
	name = n;
	readSettings();
}

// read the settings of a single input type configuration
void ImageContextTemplate::readSettings() {
	QSettings settings;
	// todo check settings validity; outside manipulation is possible
	settings.beginGroup(name);
	unit_of_measurement =
			settings.value("UnitOfMeasurement", "Pixel").toString();
	conversation_factor =
			settings.value("ConversationFactor", 1.0).toReal();
	custom_param_1      = settings.value("CustomParamOne", "").toString();
	custom_param_2      = settings.value("CustomParamTwo", "").toString();

	file_pattern                  = settings.value(
			"FilePattern", "$i$").toString();
	directory      = settings.value(
			"Directory", "").toString();
	settings.endGroup();
}

// write the settings of a single input configuration
//
void ImageContextTemplate::writeSettings() {
	QSettings settings;
	// precondition all values are valid and consistent
	settings.beginGroup(name);
	settings.setValue("UnitOfMeasurement", unit_of_measurement);
	settings.setValue("ConversationFactor", conversation_factor);
	settings.setValue("CustomParamOne", custom_param_1);
	settings.setValue("CustomParamTwo", custom_param_2);
	settings.setValue("Directory",directory);
	settings.setValue("FilePattern",file_pattern);
	settings.endGroup();
	settings.sync();
}


