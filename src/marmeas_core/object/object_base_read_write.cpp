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
#include <QString>
#include <assert.h>
#include "object_base_read_write.h"
#include "basic_type_read_write.h"
#include "measurement/measurement_read_write.h"


ObjectBaseReader::ObjectBaseReader() {
		object_ = 0;
		measurement_ = 0;
		name_ = "";
		source_ = "";
		id_ = 0;
}

void ObjectBaseReader::setScene(QGraphicsScene* scene) {
	scene_ = scene;
	qDebug() << "--ObjectBaseReader::setScene-- scene is set: " << (long)scene_;
	assert ( scene_) ;
}

bool ObjectBaseReader::readName(QXmlStreamReaderWhitespace& reader) {

	if (! reader.isStartElement() and reader.name() != "name" ) return false;
	reader.readNext();
	if (! reader.isCharacters()) return false;
	name_ = ""; name_ += reader.text();
	reader.readNext();
	if ( reader.isEndElement() && reader.name() == "name"  ) {
		reader.readNext(); return true ;
	} else return false;
}

bool ObjectBaseReader::readSource(QXmlStreamReaderWhitespace& reader) {

	if (! reader.isStartElement() and reader.name() != "source" ) return false;
	reader.readNext();
	if (! reader.isCharacters()) return false;
	source_ = ""; source_ += reader.text();
	reader.readNext();
	if ( reader.isEndElement() && reader.name() == "source" ) {
		reader.readNext(); return true ;
	} else return false;
}


bool ObjectBaseReader::read(QXmlStreamReaderWhitespace& reader) {

	if (! reader.isStartElement() and reader.name() != "object" ) return false;
	QStringRef id = reader.attributes().value("id");
	QString tmp; tmp.append(id);
	bool isOk;
	id_ = tmp.toLong(&isOk);
	if (!isOk) return false;
	initObjects();
	MeasurementReader measurement_reader(measurement_);
	reader.readNext();

	while (! reader.isEndDocument()) {
		qDebug () << "--ObjectBaseReader::read-- "
				 << "current element: " << reader.name();
		if (reader.isStartElement() && reader.name() == "name") {
			readName(reader); continue; }
		if (reader.isStartElement() && reader.name() == "source") {
			readSource(reader); continue; }
		if (reader.isStartElement() && reader.name() == "model") {
			readGeometricObject(reader); continue;
		}
		if (measurement_reader.checkToken(reader)) {
			measurement_reader.read(reader); continue;
		}
		break;
	}
	if (reader.isEndElement() && reader.name() == "object" )  {
		// compose the data
		object_->setId(id_);
		object_->setName(name_);
		object_->setSource(source_);
		measurement_->init(object_);
		object_->setMeasurement(measurement_);
		finalize();

		// register the object
		qDebug() << "--ObjectBaseReader::read-- "
				 << "Register new Object: " << name_ << " id: " << id_;
		GeometricObjectContainer* container =
				GeometricObjectContainerSingleton::getInstance();
		container->addObject( object_);
		return true;
	}
	// something went wrong -> some cleanup
	qDebug() << "--ObjectBaseReader::read-- "
			 << "Error in reading at line: " << reader.lineNumber()
			 << " object: " << name_ << " id: " << id_;
	delete object_;
	delete measurement_;
	return false;
}


// -------------------------------------------------------------------------

bool ObjectBaseWriter::write(QXmlStreamWriter& writer) {
	writer.writeStartElement("object");
	writer.writeAttribute("version", "1.0"); // the object format version
	writer.writeAttribute("type", object_->getType());
	// which type of object is it; required as attribute to be independent
	// upon who is first model or measurement (to use a simple reader)
	QString tmp;
	tmp = QString("%1").arg(object_->getId());
	writer.writeAttribute("id", tmp);

	// general (but optional) data
	if (object_->getName() != "")
		writer.writeTextElement("name", object_->getName());
	if (object_->getSource() != "")
		writer.writeTextElement("source",object_->getSource());

	// geometric model (mandatory)
	writer.writeStartElement("model");
	for (int i = 0 ; ; i++) {
		QString s = object_->getModelParameterName(i);
		if (s == "") break;
		QVariant data = object_->getModelParameterData(i);
		QVariantWriter variant_writer(s, data);
		if ( ! variant_writer.write(writer) ) return false;
	}
	writer.writeEndElement();

	const MeasurementOnObject* measurement = object_->getMeasurement();
	bool result = writeMeasurement(writer, measurement) ;

	// end of object
	writer.writeEndElement();
	return result;
}

bool ObjectBaseWriter::writeMeasurement(QXmlStreamWriter& writer,
		const MeasurementOnObject* measurement) {

	assert(measurement);

	MeasurementType actual = measurement->actualMeasures();
	qDebug() << "--ObjectBaseWriter::write--"  << " canonic: "
			 << measurement->canonicName(MeasurementOnObject::Self)
			 << " actual: " << (int)actual;

	writer.writeStartElement(
			measurement->canonicName(MeasurementOnObject::Self));

	if ( MeasurementOnObject::Graphic & actual) {
		writer.writeStartElement(measurement->canonicName(
				MeasurementOnObject::Graphic));
		writer.writeEndElement();
	}

	MeasurementType k;
	for (k = 4 ;  k < MeasurementOnObject::AllMeasures ; k = k << 1)
		if (k & actual) {
			QString s = measurement->canonicName(k);
			QVariant data = measurement->measurementValue(k);
			if (data.canConvert<IntensityMeasurement*>()) {
				IntensityMeasurement* m = data.value<IntensityMeasurement*>();
				writeMeasurement(writer, m);
				continue;
			}
			if (data.canConvert<GeometricMeasurement*>()) {
				GeometricMeasurement* m = data.value<GeometricMeasurement*>();
				writeMeasurement(writer, m);
				continue;
			}
			// it is an ordinary value
			QVariantWriter variant_writer(s, data);
			if ( ! variant_writer.write(writer) ) return false;
		}

	writer.writeEndElement();
	return true;
}

// -------------------------------------------------------------------------

bool ObjectContainerWriter::write(QXmlStreamWriter& writer) {
	bool ret_val;
	writer.writeStartElement("objects");
	ret_val = container_->write(writer);
	writer.writeEndElement();
	return ret_val;

}
