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
#include <QVariant>
#include <QColor>
#include "measurement_line.h"
#include <math.h>
#include <QObject>


GeometricMeasurementOnLine::GeometricMeasurementOnLine()
		: GeometricMeasurement() {
	available_measures_ =
			  GeometricMeasurement::GeometricMaxDimensionAngle
			| GeometricMeasurement::GeometricRoundness
			| GeometricMeasurement::GeometricBoundingBox
			| GeometricMeasurement::GeometricMaxDimension  // the diameter
			| GeometricMeasurement::GeometricMidpoint

			;
	actual_measures_ = GeometricMeasurement::Undefined ;
	qDebug() << "--GeometricMeasurementOnLine::GeometricMeasurementOnLine--"
			 << " available measures: " << available_measures_;

}

GeometricMeasurementOnLine::~GeometricMeasurementOnLine() {

}

QVariant GeometricMeasurementOnLine::measurementValue(MeasurementType m) const {
	qDebug() << "--GeometricMeasurementOnLine::measurementValue-- called";

	switch(m ) {
	case GeometricMeasurement::GeometricMaxDimensionAngle  :
		return QVariant(maxDimensionAngle());
		break;
	case GeometricMeasurement::GeometricRoundness :
		return QVariant(roundness());
		break;
	case GeometricMeasurement::GeometricBoundingBox :
		return QVariant(boundingBox());
		break;
	case GeometricMeasurement::GeometricMidpoint :
		return QVariant(midpoint());
		break;
	case GeometricMeasurement::GeometricMaxDimension :
		return QVariant(maxDimension());
		break;

	default:
		qDebug() << "--GeometricMeasurementOnLine::measurementValue-- "
				 << "delegate type super class: " << m;
		return  GeometricMeasurement::measurementValue(m);
		break;
	}
}


QString GeometricMeasurementOnLine::measurementName(MeasurementType m) const {
	QString ret;
	switch (m) {
	// geometry related make sure the meaning is more type specific

	case GeometricMeasurement::GeometricXMaxDimension  :
	case GeometricMeasurement::GeometricYMaxDimension  :
	case GeometricMeasurement::GeometricMaxDimension  :
	case GeometricMeasurement::GeometricMinDimension  :
		ret = tr("Length");
		break;

	case GeometricMeasurement::GeometricMaxDimensionAngle  :
	case GeometricMeasurement::GeometricMinDimensionAngle  :

		ret = tr("Slope");
		break;


	// otherwise delegate to super class
	default:
		ret = GeometricMeasurement::measurementName(m);
	}
	return ret;
}


QRectF GeometricMeasurementOnLine::boundingBox() const {
	// TODO - bugfix
	QPointF start     =  ((GeometricLine*)object_)->getStart();
	QPointF end =  ((GeometricLine*)object_)->getEnd();

	QRectF b_box(start,end);
	return b_box;
}

double GeometricMeasurementOnLine::maxDimension() const {
	QPointF start   =  ((GeometricLine*)object_)->getStart();
	QPointF end     =  ((GeometricLine*)object_)->getEnd();
	double x_dist = - start.rx() + end.rx();
	double y_dist = - start.ry() + end.ry();

	double len = sqrt(x_dist * x_dist + y_dist * y_dist );
	return len;

}

double GeometricMeasurementOnLine::roundness() const {
	return 0;  // since it is a line
}

double GeometricMeasurementOnLine::maxDimensionAngle() const {
	QPointF start   =  ((GeometricLine*)object_)->getStart();
	QPointF end     =  ((GeometricLine*)object_)->getEnd();
	double x_dist = - start.rx() + end.rx();
	double y_dist = - start.ry() + end.ry();
	double angle;
	if (x_dist == 0.0)
		if (y_dist > 0.0) angle = M_PI / 2;           //  90 degree
		else              angle = M_PI * 3 / 2;       // 270 degree
	else
		angle = atan( y_dist / x_dist);
	return angle;

}

QPointF GeometricMeasurementOnLine::midpoint() const {
	QPointF start   =  ((GeometricLine*)object_)->getStart();
	QPointF end     =  ((GeometricLine*)object_)->getEnd();
	double x_dist = - start.rx() + end.rx();
	double y_dist = - start.ry() + end.ry();
	end = QPointF(x_dist * 0.5, y_dist * 0.5);
	start += end;
	return start;

}



