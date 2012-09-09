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
#include <QImage>
#include "measurement_line.h"
#include <math.h>

ProfileMeasurementOnLine::ProfileMeasurementOnLine()
		: IntensityMeasurement() {

	actual_measures_ = IntensityMeasurement::Undefined ;
	qDebug() << "--ProfileMeasurementOnLine::ProfileMeasurementOnLine--"
			 << " actual measures: " << actual_measures_;

}

ProfileMeasurementOnLine::~ProfileMeasurementOnLine() {

}



QString ProfileMeasurementOnLine::measurementName(MeasurementType m) const {
	QString ret;
	switch (m) {

	case IntensityMeasurement::Self  :

		ret = tr("Line Profile");
		break;


	// otherwise delegate to super class
	default:
		ret = IntensityMeasurement::measurementName(m);
	}
	return ret;
}

QString ProfileMeasurementOnLine::canonicName(MeasurementType m) const {
	QString ret;
	switch (m) {
		case IntensityMeasurement::Self  :
			ret = "profile";		break;


		// otherwise delegate to super class
		default:
			ret = IntensityMeasurement::canonicName(m);
		}
		return ret;
}

void ProfileMeasurementOnLine::computeProfile() {
	GeometricObjectContainer* object_container
		= GeometricObjectContainerSingleton::getInstance();
	QImage* image = object_container->image();
	qDebug() << "--ProfileMeasurementOnLine::computeProfile-- image addr: "
			<< (long)image;

	QPointF start   =  ((GeometricLine*)object_)->getStart();
	QPointF end     =  ((GeometricLine*)object_)->getEnd();
	int x_dist = - start.rx() + end.rx();
	int y_dist = - start.ry() + end.ry();
	int length = sqrt(x_dist * x_dist + y_dist * y_dist);
	if (profile_) delete profile_;
	bool vertical   = ( y_dist == 0 ) ;
	bool horizontal = ( x_dist == 0 ) ;
	// if neither horizontal nor vertical add the end point as fixed end
	int additional_end_point = 1;
	if (vertical || horizontal) additional_end_point = 0;
	profile_ = new QVector<QPointF>(ceil(length) + additional_end_point);
	for (int i = 0; i < (int)ceil(length); i++) {
		// compute position on the image
		int x,y;
		if (vertical || horizontal) {
			if ( vertical   ) {  x = 0; y = i; }
			if ( horizontal ) {  x = i; y = 0; }
		} else {
			x = i * x_dist / length;
			y = i * y_dist / length;
		}
		QPoint pos(x,y);
		pos += start.toPoint();
		// determine the pixel value
		QRgb  rgb = image->pixel(pos);
		int value = qGray(rgb);

		// set the data
		QPointF point((double)i,(double)value);
		profile_->replace(i,point);
	}
	// add the end point
	if (additional_end_point) {
		// determine the pixel value at the end point
		QPoint pos(end.rx(), end.ry());
		QRgb  rgb = image->pixel(pos);
		int value = qGray(rgb);

		// set the data
		QPointF point(ceil(length) + 1.0 ,(double)value);
	}

}



