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
#include <QMenu>
#include <QAction>
#include <assert.h>

#include "measurement_on_object.h"




void MeasurementOnObject::init(GeometricObject* g) {
	if (g)  object_ = g;
}

MeasurementType MeasurementOnObject::actualMeasures() const {
	return actual_measures_;
}

MeasurementType MeasurementOnObject::availableMeasures() const {
	return available_measures_;
}

int MeasurementOnObject::version() const { return 1 ; }


MeasurementOnObject::MeasurementOnObject() {
	actual_measures_ = Undefined;
	available_measures_ = Undefined;
	object_ = 0;
}

MeasurementOnObject::~MeasurementOnObject() {
}



MeasurementType MeasurementOnObject::type(const QString& canonic_name) const {
	MeasurementType m = 1 ;
	for (int index = 1; index < sizeof(MeasurementType) * 8; index++) {
		if (m & ( available_measures_ 	| MeasurementOnObject::Self
										| MeasurementOnObject::Graphic ))
			if (canonic_name == canonicName(m)) return m;
		m = m << 1;
	}
	// nothing matching found
	return MeasurementOnObject::Undefined;
}

void MeasurementOnObject::addActualMeasures( MeasurementType  measure) {
	actual_measures_ |= ( measure &
			( available_measures_ 	| MeasurementOnObject::Graphic ) );
}

void MeasurementOnObject::removeActualMeasures( MeasurementType  measure) {
	actual_measures_ &= (  MeasurementOnObject::AllMeasures & ~measure ) ;
}


void MeasurementOnObject::updateMeasures() {
}
