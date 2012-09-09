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

#include <assert.h>
#include <QDebug>
#include <QVariant>
 #include <QListIterator>

#include "measurement_on_object.h"
#include "basic_type_read_write.h"
#include "measurement_read_write.h"

MeasurementReader::MeasurementReader(MeasurementOnObject* measurement) {
	assert(measurement);
	measurement_ = measurement;
	MeasurementType available = measurement_->availableMeasures();
	qDebug() << "--MeasurementReader::MeasurementReader--" << " c'tor self: "
			 << measurement_->canonicName(MeasurementOnObject::Self)
			 << " available: " << available;
	if (measurement_->canonicName(MeasurementOnObject::Self) == "measurements") {
		// it is a composite measurement
		MeasurementType m;
		for (m = 2  ; m < MeasurementOnObject::AllMeasures; m = m << 1) {
			if (m & available) {
				QVariant v = measurement->measurementValue(m);
				MeasurementOnObject* moo;
				if (v.canConvert<GeometricMeasurement*>())
					moo = (MeasurementOnObject*) v.value<GeometricMeasurement*>();
				if (v.canConvert<IntensityMeasurement*>())
					moo = (MeasurementOnObject*) v.value<IntensityMeasurement*>();
				MeasurementReader* mr = new MeasurementReader(moo);
				sub_readers_.append(mr);
				qDebug() << "--MeasurementReader::MeasurementReader--"
						 << " add aggregated";
			}
		}
	}
}

MeasurementReader::~MeasurementReader() {
	 QList<MeasurementReader*>::iterator it;
	 for (it = sub_readers_.begin(); it != sub_readers_.end(); ++it)
		delete *it;
}



bool MeasurementReader::checkToken(const QXmlStreamReaderWhitespace& reader) {
	QString token = measurement_->canonicName(MeasurementOnObject::Self);
	qDebug() << "--MeasurementReader::checkToken--"
			 << " token: " << token;
	return reader.isStartElement() && reader.name() == token ;
}

bool MeasurementReader::read(QXmlStreamReaderWhitespace& reader) {
	if ( sub_readers_.size() > 0 ) {
		bool result =  readComposite(reader);
		if (result) setActualFlags();
		return result;
	} else
		return readLeaf(reader);
}

void MeasurementReader::setActualFlags(void) {
	MeasurementType available = measurement_->availableMeasures();
	qDebug() << "--MeasurementReader::setActualFlags--"
			 << " available: " << available;
	MeasurementType m;
	for (m = 2  ; m < MeasurementOnObject::AllMeasures; m = m << 1) {
		if (m & available) {
			QVariant v = measurement_->measurementValue(m);
			MeasurementOnObject* moo;
			if (v.canConvert<GeometricMeasurement*>())
				moo = (MeasurementOnObject*) v.value<GeometricMeasurement*>();
			if (v.canConvert<IntensityMeasurement*>())
				moo = (MeasurementOnObject*) v.value<IntensityMeasurement*>();
			if (moo->actualMeasures()) measurement_->addActualMeasures(m);
		}
	}
}

bool MeasurementReader::readComposite(QXmlStreamReaderWhitespace& reader) {
	QString token = measurement_->canonicName(MeasurementOnObject::Self);
	qDebug() << "--MeasurementReader::readComposite--"
			 << " token: " << token;
	reader.readNext();   // overcome the start element
	int loop_count = 33; // there are not more than 32 measures
	measurement_->removeActualMeasures(MeasurementOnObject::AllMeasures);
	while ( !reader.isEndDocument() && loop_count-- > 0) { // emergency exit
		bool read_flag = false;
		if ( reader.isEndElement() &&  reader.name() == token) {
			qDebug() << "--MeasurementReader::readComposite--"
					 << "regularly leave the loop";
			reader.readNext() ;
			return true;
		}
		 QList<MeasurementReader*>::iterator it;
		 for (it = sub_readers_.begin(); it != sub_readers_.end(); ++it) {
			if ((*it)->checkToken(reader)) {
				qDebug() << "--MeasurementReader::readComposite--"
						 << " dig into the aggregation";
				(*it)->read(reader);
				read_flag = true;
			}
		}
		// very restrictive we were not able to read the token
		if (! read_flag ) { reader.readNext(); return false ; }
	}
	reader.readNext(); // overcome the end element
	return false;
}


bool MeasurementReader::readLeaf(QXmlStreamReaderWhitespace& reader) {
	QString token = measurement_->canonicName(MeasurementOnObject::Self);
	qDebug() << "--MeasurementReader::readLeaf--"
			 << " token: " << token;
	reader.readNext();   // overcome the start element
	int loop_count = 33; // there are not more than 32 measures
	measurement_->removeActualMeasures(MeasurementOnObject::AllMeasures);
	while ( !reader.isEndDocument() && loop_count-- > 0) { // emergency exit
		if ( reader.isEndElement() &&  reader.name() == token) {
			qDebug() << "--MeasurementReader::readLeaf-- "
					 << "regularly leave the loop";
			break;  // normal condition to exit the loop
		}
		if ( reader.isStartElement() ) {
			QString node_name;
			node_name.append(reader.name());
			qDebug() << "--MeasurementReader::readLeaf--"
					 << " measure name: " << node_name
					 << " loop count " << loop_count;
			MeasurementType m = measurement_->type(node_name);
			if ( m & measurement_->availableMeasures() ||
					m & MeasurementOnObject::Graphic) {
				qDebug() << "--MeasurementReader::readLeaf--"
						 << " test for type : " << m;
				QVariant value;
				QVariantReader measure_reader(node_name, value);
				if (! measure_reader.read(reader)) return false;
				// values of measures are always computed but never read
				// however, the measures desired get registered
				measurement_->addActualMeasures(m);
			}
		}
	}
	bool retval = reader.isEndElement() ;
	retval &=   reader.name() == token ;
	reader.readNext(); // overcome the end element
	return retval;
}

