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

#include "measurement_line.h"



MeasurementOnObject* FactoryMeasurementOnLine::create(GeometricLine* g) {
	GeometricMeasurementOnLine* geom_measure= new GeometricMeasurementOnLine();
	geom_measure->addActualMeasures(geom_measure->availableMeasures());
	//geom_measure->init(g);

	ProfileMeasurementOnLine* profile_measure= new ProfileMeasurementOnLine();
	profile_measure->addActualMeasures(profile_measure->availableMeasures());
	//profile_measure->init(g);

	CompositeMeasurement* measurement = new CompositeMeasurement();
	measurement->addMeasurement(geom_measure);
	measurement->addMeasurement(profile_measure);
	measurement->addActualMeasures(measurement->availableMeasures());
	measurement->init(g);

	qDebug() << "--FactoryMeasurementOnLine::create-- available"
			<< measurement->availableMeasures();

	return measurement;
}




