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
#include "measurement_circle.h"
#include <math.h>

HistogramMeasurementOnCircle::HistogramMeasurementOnCircle()
		: IntensityMeasurement() {

	actual_measures_ = IntensityMeasurement::Undefined ;
	qDebug() << "--HistogramMeasurementOnCircle::HistogramMeasurementOnCircle--"
			 << " actual measures: " << actual_measures_;

}

HistogramMeasurementOnCircle::~HistogramMeasurementOnCircle() {

}



QString HistogramMeasurementOnCircle::measurementName(MeasurementType m) const {
	QString ret;
	switch (m) {

	case IntensityMeasurement::Self  :

		ret = tr("Circle Histogram");
		break;


	// otherwise delegate to super class
	default:
		ret = IntensityMeasurement::measurementName(m);
	}
	return ret;
}

QString HistogramMeasurementOnCircle::canonicName(MeasurementType m) const {
	QString ret;
	switch (m) {
		case IntensityMeasurement::Self  :
			ret = "histogram";		break;


		// otherwise delegate to super class
		default:
			ret = IntensityMeasurement::canonicName(m);
		}
		return ret;
}

void HistogramMeasurementOnCircle::computeHistogram() {
	GeometricObjectContainer* object_container
		= GeometricObjectContainerSingleton::getInstance();
	QImage* image = object_container->image();
	qDebug() << "--HistogramMeasurementOnCircle::computeHistogram-- image addr: "
			<< (long)image;

	QPointF midpoint   =  ((GeometricCircle*)object_)->getMidpoint();
	float  radius     =  ((GeometricCircle*)object_)->getRadius();
	if (histogram_) delete histogram_;

	histogram_ = new QVector<QPointF>(256);
	// initialize the histogram
	for (int i = 0; i < 256 ; i++ ) {
		QPointF point ( (double) i , 0.0);
		histogram_->replace(i, point);
	}

	for (float i = ( midpoint.x() - radius -1);
			i <  midpoint.x() + radius + 1 ; i += 1.0 )
		for (float j =  midpoint.y() - radius -1.0 ;
	         j < midpoint.y() + radius + 1.0  ; j++ )
	        	 if ( sqrt(  ( i - midpoint.x() ) * ( i - midpoint.x() ) +
	        			     ( j - midpoint.y() ) * ( j - midpoint.y() )  )
	        			 < radius) {
	        			QRgb  rgb = image->pixel(int(i),int(j));
	        			int value = qGray(rgb);
	        			QPointF val = histogram_->at(value);
	        			histogram_->replace(value, QPointF( 0.0 , 1.0 ) + val );
	        	 }

}





