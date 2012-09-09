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

#ifndef MEASUREMENT_LINE_H_
#define MEASUREMENT_LINE_H_

#include <math.h>
#include <xml_reader_writer.h>
#include <object_base.h>
#include "line.h"

class FactoryMeasurementOnLine {
public:
	static MeasurementOnObject* create(GeometricLine* g);
};

class GeometricMeasurementOnLine : public GeometricMeasurement {
public:
	GeometricMeasurementOnLine();
	virtual ~GeometricMeasurementOnLine();
	virtual int getVersion() const { return 1; }
	virtual QVariant measurementValue(MeasurementType m) const;
	virtual QString measurementName(MeasurementType m) const;

private:
	// measures implementations
	QRectF  boundingBox() const ;
	QPointF midpoint() const;
	double maxDimension() const ;
	double maxDimensionAngle() const;
	double roundness() const;

};

class ProfileMeasurementOnLine : public IntensityMeasurement {
public:
	ProfileMeasurementOnLine();
	virtual ~ProfileMeasurementOnLine();
	virtual int getVersion() const { return 1; }
	virtual QString canonicName(MeasurementType m) const;
	virtual QString measurementName(MeasurementType m) const;
	virtual void computeProfile();

};

#endif /* MEASUREMENT_LINE_H_ */
