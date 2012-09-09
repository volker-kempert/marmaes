/*
 * measurement_circle.h
 *
 *  Created on: 28.08.2011
 *      Author: volker
 */

#ifndef MEASUREMENT_CIRCLE_H_
#define MEASUREMENT_CIRCLE_H_

#include <math.h>
#include <xml_reader_writer.h>
#include <object_base.h>
#include "circle.h"


class FactoryMeasurementOnCircle {
public:
	static MeasurementOnObject* create(GeometricCircle* g);
};

class GeometricMeasurementOnCircle : public GeometricMeasurement {
public:
	GeometricMeasurementOnCircle();
	virtual ~GeometricMeasurementOnCircle();
	int getVersion() const { return 1; }
	QVariant measurementValue(MeasurementType m) const;
	QString measurementName(MeasurementType m) const;

private:
	// measures implementations
	double surface() const;
	double circumFerence() const;
	QRectF  boundingBox() const ;
	double maxDimension() const ;
	double roundness() const;

};


class HistogramMeasurementOnCircle : public IntensityMeasurement {
public:
	HistogramMeasurementOnCircle();
	virtual ~HistogramMeasurementOnCircle();
	virtual int getVersion() const { return 1; }
	virtual QString canonicName(MeasurementType m) const;
	virtual QString measurementName(MeasurementType m) const;
	virtual void computeHistogram();

};



#endif /* MEASUREMENT_CIRCLE_H_ */
