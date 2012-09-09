/*
 * measurement_circle.cpp
 *
 *  Created on: 28.08.2011
 *      Author: volker
 */
#include <QVariant>
#include <QDebug>
#include "measurement_circle.h"


GeometricMeasurementOnCircle::GeometricMeasurementOnCircle()
: GeometricMeasurement() {
	available_measures_ =
			  GeometricMeasurement::GeometricSurface
			| GeometricMeasurement::GeometricCircumFerence
			| GeometricMeasurement::GeometricRoundness
			| GeometricMeasurement::GeometricBoundingBox
			| GeometricMeasurement::GeometricMaxDimension  // the diameter
			;
	actual_measures_ = GeometricMeasurement::Undefined ;
	qDebug() << "GeometricMeasurementOnCircle: Init Measures: " << actual_measures_;
}

GeometricMeasurementOnCircle::~GeometricMeasurementOnCircle() {

}

QVariant GeometricMeasurementOnCircle::measurementValue(MeasurementType m) const {
	if (!(m & actual_measures_) )
		// maybe the superclass can handle
		return GeometricMeasurement::measurementValue(m);

	switch(m) {
	case GeometricMeasurement::GeometricSurface  :
		return QVariant(surface());
		break;
	case GeometricMeasurement::GeometricCircumFerence :
		return QVariant(circumFerence());
		break;
	case GeometricMeasurement::GeometricRoundness :
		return QVariant(roundness());
		break;
	case GeometricMeasurement::GeometricBoundingBox :
		return QVariant(boundingBox());
		break;
	case GeometricMeasurement::GeometricMaxDimension :
		return QVariant(maxDimension());
		break;

	default:
		throw "Measurement is not available";
		break;
	}
}


QString GeometricMeasurementOnCircle::measurementName(MeasurementType m) const {
	QString ret;
	switch (m) {
	// geometry related make sure the meaning is more type specific

	case GeometricMeasurement::GeometricXMaxDimension  :
	case GeometricMeasurement::GeometricYMaxDimension  :
	case GeometricMeasurement::GeometricMaxDimension  :
	case GeometricMeasurement::GeometricMinDimension  :

		ret = tr("Diameter");
		break;


	// otherwise delegate to super class
	default:
		ret = GeometricMeasurement::measurementName(m);
	}
	return ret;
}


double GeometricMeasurementOnCircle::surface() const {
	double radius = ((GeometricCircle*)object_)->getRadius();
	return radius * radius * M_PI;
}

double GeometricMeasurementOnCircle::circumFerence() const {
	return ((GeometricCircle*)object_)->getRadius() * M_PI;
}

QRectF GeometricMeasurementOnCircle::boundingBox() const {
	// TODO - bugfix
	double radius = ((GeometricCircle*)object_)->getRadius();
	QPointF midpoint =  ((GeometricCircle*)object_)->getMidpoint();
	QRectF b_box(midpoint.x()- radius, midpoint.y() - radius,
			2 * radius, 2 * radius);

	return b_box;
}

double GeometricMeasurementOnCircle::maxDimension() const {
	return ((GeometricCircle*)object_)->getRadius() * 2.0;
}

double GeometricMeasurementOnCircle::roundness() const {
	return 1;  // since it is a circle
}

