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
#include <stdexcept>
#include <QDebug>
#include <QVariant>

#include "measurement_on_object.h"




void CompositeMeasurement::updateMeasures() {
	for (int index = 0; index < measurements_.size(); index++ )
			measurements_.at(index)->updateMeasures();
}

void CompositeMeasurement::init(GeometricObject* g) {
	if (g)  {
		object_ = g;
		for (int index = 0; index < measurements_.size(); index++ )
			measurements_.at(index)->init(g);
	}
}

QString CompositeMeasurement::measurementName(MeasurementType m) const {
	qDebug() << "--CompositeMeasurement::measurementName-- type: " << m;
	if  ( m == CompositeMeasurement::Self )	return tr("Measurements");
	if  ( m == CompositeMeasurement::Graphic )	return "Graphics";
	MeasurementType flag = 4;
	for (int index = 0; index < measurements_.size(); index++ ) {
		if ( flag & m )  return measurements_.at(index)->measurementName(
				MeasurementOnObject::Self);
		flag = flag << 1;
	}

	qDebug() << "--CompositeMeasurement::measurementName-- "
			 << "Unhandled measurement type" << m;
	//throw std::logic_error("--CompositeMeasurement::measurementName--"
	//		" Unhandled  measurement type");
	return "";
}


QString CompositeMeasurement::canonicName(MeasurementType m) const {
	qDebug() << "--CompositeMeasurement::canonicName-- type: " << m;
	if  ( m == CompositeMeasurement::Self )	return "measurements";
	if  ( m == CompositeMeasurement::Graphic )	return "graphics";
	MeasurementType flag = 4;
	for (int index = 0; index < measurements_.size(); index++ ) {
		if ( flag & m )  return measurements_.at(index)->canonicName(
				MeasurementOnObject::Self);
		flag = flag << 1;
	}

	qDebug() << "--CompositeMeasurement::canonicName-- "
			 << "Unhandled measurement type" << m;
	//throw std::logic_error("--CompositeMeasurement::canonicName--"
	//		" Unhandled  measurement type");
	return "";
}


/**
 * @return: delivers always a value if nothing given QVariant (double,0)
 *          is returned.
 *          by checking availableMeasurmentType it has to made sure
 *          that the return is indeed meaningful values of 0.0
 *
 *
 *
 */
QVariant CompositeMeasurement::measurementValue(MeasurementType m) const  {
	qDebug() << "--CompositeMeasurement::measurementValue-- type: " << m;
	if  ( m == CompositeMeasurement::Self )
		// next line does not work: see
		// http://developer.qt.nokia.com/forums/viewthread/4636
		//return QVariant::fromValue<CompositeMeasurementStar>(this);
		return qVariantFromValue(const_cast<CompositeMeasurement*>(this));

	MeasurementType flag = 4;
	for (int index = 0; index < measurements_.size(); index++ ) {
		if ( flag & m )
			return measurements_.at(index)->measurementValue(Self);
		flag = flag << 1;
	}

	qDebug() << "--CompositeMeasurement::measurementValue-- "
			 << "Unhandled measurement type" << m;
	throw std::logic_error("--CompositeMeasurement::measurementValue--"
			" Unhandled  measurement type");
}


void CompositeMeasurement::addMeasurement(MeasurementOnObject* m) {
	measurements_.append(m);
	available_measures_ |= 2 << measurements_.size();
}

CompositeMeasurement::CompositeMeasurement() {
	available_measures_ = CompositeMeasurement::AllMeasures;
}

CompositeMeasurement::~CompositeMeasurement() {
	qDebug() << "--CompositeMeasurement::~CompositeMeasurement--" ;
	for (int index = 0; index < measurements_.size(); index++ )
		delete measurements_.at(index);
	qDebug() << "--CompositeMeasurement::~CompositeMeasurement-- done" ;
}




