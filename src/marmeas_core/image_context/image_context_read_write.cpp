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

#include <QDebug>
#include "image_context_read_write.h"

ImageContextReadFlags ImageContextReader::assignValue(QString key ,
		QString value) {

	qDebug() << "--ImageContextReader::assignValue-- key:" << key
			<< " value: " << value ;
	ImageContextReadFlags flags;
    flags.unit_of_measurement = false;
	flags.conversion_factor = false;
	flags.image_file_name     = false;
	if (key == "image_file") {
		image_context_->setImageFileName(value);
		flags.image_file_name = true;
	}
	if (key == "conversion_factor") {
		bool tmp;
		image_context_->setConversionFactor(value.toFloat(& tmp));
		flags.conversion_factor = tmp;
	}
	if (key == "unit_of_measurement") {
		image_context_->setUnitOfMeasurement(value);
		flags.unit_of_measurement = true;
	}
	if (key == "param_one_name") {
		image_context_->setParam1Name(value);
	}
	if (key == "param_one_data") {
		image_context_->setParam1Data(value);
	}
	if (key == "param_two_name") {
		image_context_->setParam2Name(value);
	}
	if (key == "param_two_data") {
		image_context_->setParam2Data(value);
	}
	qDebug() << "--ImageContextReader::assignValue-- flags-inside " << (7 & flags.group);
	return flags;
}


bool ImageContextReader::read(QXmlStreamReaderWhitespace& reader) {

	// check for the preconditions
	if (reader.atEnd()) return false;
	if (reader.isEndElement()) return false;
	if (reader.name() != "image_context") return false;

	// record if mandatory items are given
	ImageContextReadFlags flags;
	flags.group = 0;

	image_context_->resetImageContext();
	// ignore the format version for now.
	reader.readNext();
	while (!reader.atEnd()) {
		if (reader.isEndElement())
			if (reader.name() == "image_context") {  break; }
		if (reader.isStartElement()) {
			QString key = reader.name().toString();
			reader.readNext();
			if (!reader.isCharacters()) { return false ; }
			QString value = reader.text().toString();
			reader.readNext();
			if (!reader.isEndElement()) { return false ; }
			else if (reader.name() != key) { return false ;  }
			// we got an item - lets do the assignment
			ImageContextReadFlags tmp;
			tmp =  assignValue(key,value);
			flags.group |= tmp.group;
		}
		reader.readNext();  // move ahead in any case
	}
	if ( reader.atEnd() )	return false;
	reader.readNext();
	return flags.unit_of_measurement &&
		   flags.conversion_factor && flags.image_file_name ;
}

bool ImageContextReader::checkToken(const QXmlStreamReaderWhitespace& reader) {
	return reader.isStartElement() && reader.name() == "image_context";
}

bool ImageContextWriter::write(QXmlStreamWriter& writer) {
	writer.writeStartElement("image_context");
	writer.writeAttribute("version", "1.0"); // the xml format version

	writer.writeTextElement("image_file",
			image_context_->getImageFileName());
	writer.writeTextElement("unit_of_measurement",
			image_context_->getUnitOfMeasurement());
	writer.writeTextElement("conversion_factor",
			QString::number(image_context_->getConversionFactor()));
	if (image_context_->getParam1Name() != "") {
		writer.writeTextElement("param_one_name",
			image_context_->getParam1Name());
		if (image_context_->getParam1Data() != "")
			writer.writeTextElement("param_one_data",
					image_context_->getParam1Data());
	}
	if (image_context_->getParam2Name() != "") {
		writer.writeTextElement("param_two_name",
			image_context_->getParam2Name());
		if (image_context_->getParam2Data() != "")
			writer.writeTextElement("param_two_data",
					image_context_->getParam2Data());
	}
	writer.writeEndElement();
}
