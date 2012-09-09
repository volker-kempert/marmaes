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

#include "image_context.h"
#include "image_context_template.h"
#include <QFileInfo>
#include <QDir>
#include <QObject>
#include <QDebug>


ImageContext* ImageContext::the_input_image_ = 0;

ImageContext *ImageContext::getInstance() {
    if (!the_input_image_) {
    	qDebug() << "--ImageContext::getInstance--";
        the_input_image_ = new ImageContext();
    }
    return the_input_image_;
}

ImageContext::~ImageContext()  {
        if (the_input_image_ == this)
            the_input_image_ = 0;
    }

ImageContext::ImageContext(QObject *parent)
	: QObject(parent) {
	resetImageContext();
}

//-----------------------------------------------------------------------------

QString ImageContext::getImageShortName() {
	return QFileInfo(image_file_name_).fileName();
}

QString ImageContext::getObjectShortName() {
	return QFileInfo(object_file_name_).fileName();
}

/*!
 * @brief load the settings from a template
 */
void ImageContext::loadImageTemplateSettings(QString image_context_template) {
	ImageContextTemplate config(image_context_template);
	config.readSettings();
	// make a copy to be independent from changes of the template
	param_1_name_ = config.custom_param_1;
	param_2_name_ = config.custom_param_2;
	unit_of_measurement_ = config.unit_of_measurement;
	conversion_factor_ = config.conversation_factor;
	image_context_template_ = config.name;
	directory_ = config.directory;
	file_pattern_ = config.file_pattern;
}



/*!
 * @brief take current constellation of base dir, param1, param2 and match
 *        pattern as well as the image file name to calculate the object file
 *        name
 *
 *        only member variables are used
 *
 *        method: if pattern, param1, param2 is given the the image file name
 *        is ignored and the instructions are used instead
 *        otherwise the image file name is used and the extension is replaced
 *        by xod
 *
 * @note  if object_file_name_ is already set (!= "") nothing is done
 *
 */
void ImageContext::computeObjectFileName() {
	qDebug() << "--ImageContext::computeObjectFileName--"
			<< " pattern: " << file_pattern_;

	// do not overwrite if name is already set directly
	if (freeze_object_file_name_) return;

	if ( file_pattern_  == "" ) {
		// use the image file name
		QFileInfo file_info(image_file_name_);
		object_file_name_ =  file_info.absolutePath();
		object_file_name_ += QDir::separator ();
		object_file_name_ += file_info.baseName();
		object_file_name_ += ".xod";
		qDebug() << "--ImageContext::computeObjectFileName--: (from image) "
				<< object_file_name_ ;
	} else {
		// use the template information

		QString base_name;
		base_name = file_pattern_;
		QFileInfo img_base(image_file_name_);
		base_name.replace ( "$i$", img_base.baseName());
		base_name.replace ( "$p1$", param_1_data_);
		base_name.replace ( "$p2$", param_2_data_);

		object_file_name_ = directory_;
		object_file_name_ += QDir::separator ();
		object_file_name_ += base_name;
		object_file_name_ += ".xod";
		qDebug() << "--ImageContext::computeObjectFileName--: (from template) "
				<< object_file_name_ ;
	}
}


/*!
 * @brief
 * releases the input definition and throws all data away
 * it will not checked if there are modifications.
 */
void ImageContext::resetImageContext() {
	is_modified_ = false;
	is_plain_image_ = true;
	param_1_name_ = "";
	param_1_data_ = "";
	param_2_name_ = "";
	param_2_data_ = "";
	unit_of_measurement_ = "Pixel";
	conversion_factor_ = 1.0;
	image_file_name_ = "";
	directory_ = "";
	file_pattern_ = "";
	object_file_name_ = "";
	freeze_object_file_name_ = false;
}

void ImageContext::computeFlags() {
	is_plain_image_ = true;
	requires_edits_ = true;
	if (image_context_template_ != "default") is_plain_image_ = false;
	if (param_1_name_ != "") requires_edits_ = false;
	if (param_2_name_ != "") requires_edits_ = false;
	if (unit_of_measurement_ != "Pixel" ) is_plain_image_ = false;
	if (conversion_factor_ != 1.0 ) is_plain_image_ = false;
	is_plain_image_ &= requires_edits_;
}



void ImageContext::setImageFileName(QString s ) {
	bool emitChange = (image_file_name_ != s);
	image_file_name_ = s;
    if (emitChange) {
    	computeFlags();
    	computeObjectFileName();
    }
}

void ImageContext::setParam1Data(QString s) {
	bool emitChange = (param_1_data_ != s);
	param_1_data_ = s ;
    if (emitChange) {
    	computeFlags();
    	computeObjectFileName();
    }
}
void ImageContext::setParam2Data(QString s) {
	bool emitChange = (param_2_data_ != s);
	param_2_data_ = s ;
    if (emitChange) {
    	computeFlags();
    	computeObjectFileName();
    }
}

void ImageContext::setParam1Name(QString s) {
	bool emitChange = (param_1_name_ != s);
	param_1_name_ = s ;
    if (emitChange) {
    	computeFlags();
    }
}

void ImageContext::setParam2Name(QString s) {
	bool emitChange = (param_2_name_ != s);
	param_2_name_ = s ;
    if (emitChange) {
    	computeFlags();
    }
}

void ImageContext::setUnitOfMeasurement(QString s) {
	bool emitChange = (unit_of_measurement_ != s);
	unit_of_measurement_ = s ;
    if (emitChange) {
    	computeFlags();
    }
}

void ImageContext::setConversionFactor(qreal f) {
	bool emitChange = (conversion_factor_ != f);
	conversion_factor_ = f ;
    if (emitChange) {
    	computeFlags();
    }
}

void ImageContext::setObjectFileName(QString s) {
	bool emitChange = (object_file_name_ != s);
	object_file_name_ = s;
	freeze_object_file_name_ = true;
    if (emitChange) {
    	computeFlags();
    }
}
void ImageContext::setDirectory(QString s) {
	bool emitChange = (directory_ != s);
	directory_ = s;
    if (emitChange) {
    	computeFlags();
    	computeObjectFileName();
    }
}

void ImageContext::setFilePattern(QString s) {
	bool emitChange = (file_pattern_ != s);
	file_pattern_ = s;
    if (emitChange) {
    	computeFlags();
    	computeObjectFileName();
    }
}

