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

#include "measurement_circle.h"

MeasurementOnObject* FactoryMeasurementOnCircle::create(GeometricCircle* g) {
	GeometricMeasurementOnCircle* geom_measure= new GeometricMeasurementOnCircle();
	geom_measure->addActualMeasures(geom_measure->availableMeasures());

	HistogramMeasurementOnCircle* histogram_measure= new HistogramMeasurementOnCircle();
	histogram_measure->addActualMeasures(histogram_measure->availableMeasures());

	CompositeMeasurement* measurement = new CompositeMeasurement();
	measurement->addMeasurement(geom_measure);
	measurement->addMeasurement(histogram_measure);
	measurement->addActualMeasures(measurement->availableMeasures());
	measurement->init(g);

	qDebug() << "--FactoryMeasurementOnCircle::create-- available"
			<< measurement->availableMeasures();

	return measurement;
}




