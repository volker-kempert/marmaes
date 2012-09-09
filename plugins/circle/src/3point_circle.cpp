/*
 * graphics_circle.cpp
 *
 *  Created on: 11.02.2011
 *      Author: volker
 */

#include <QDebug>
#include <math.h>
#include <assert.h>

#include "circle.h"
#include "measurement_circle.h"
#include "3point_circle.h"

FactoryCircle3PointAnnotator::FactoryCircle3PointAnnotator()
	: GeometricObjectFactory() { input_ = 0; }

GeometricObject* FactoryCircle3PointAnnotator::finalizeObjects() {


	GeometricCircle* geometric_circle = new GeometricCircle();
	GraphicsCircleItem* graphics_circle = new GraphicsCircleItem();


	qDebug() << "--FactoryCircle3PointAnnotator::finalizeObjects--"
			 << " circle objects exist: " << ((long)geometric_circle);
	assert (input_);
	assert (geometric_circle);
	assert (graphics_circle);

	geometric_circle->setInput(input_);
	input_->setOutput(geometric_circle);
	input_->computeModel();
	graphics_circle->setGeometricObject((GeometricObject*)geometric_circle);
	graphics_circle->setPos(geometric_circle->getMidpoint());

	// setup the measurements
	MeasurementOnObject*  measurement;
	measurement = FactoryMeasurementOnCircle::create(geometric_circle);
	geometric_circle->setMeasurement(measurement);

	qDebug() << "--FactoryCircle3PointAnnotator::finalizeObjects--"
			 << " cross linking is done";
	assert (scene_);
	scene_->addItem(graphics_circle);
	// make all the connections
	input_->connectPoints(graphics_circle);

	qDebug() << "--FactoryCircle3PointAnnotator::finalizeObjects--"
			 << " drawing was finished";
	return geometric_circle;

}

GeometricInput*   FactoryCircle3PointAnnotator::createInput() {
		qDebug() << "--FactoryCircle3PointAnnotator::createInput--";
		input_ = new GeometricInputCircle3PointAnnotator();
		assert ( input_);

}



GeometricInputCircle3PointAnnotator::GeometricInputCircle3PointAnnotator() :
		GeometricInput() {
	qDebug() << "--GeometricInputCircle3PointAnnotator"
			"::GeometricInputCircle3PointAnnotator--";
	point_[0] = 0;
	point_[1] = 0;
	point_[2] = 0;
}

void GeometricInputCircle3PointAnnotator::computeModel(void) {
	qDebug() << "--GeometricInputCircle3PointAnnotator::computeModel--";
	assert (point_[0]); // make sure all data is available
	assert (point_[1]); // make sure all data is available
	assert (point_[2]); // make sure all data is available

	// see http://mathforum.org/library/drmath/view/55239.html
	// for the computation

	qreal x1= point_[0]->x();
	qreal x2= point_[1]->x();
	qreal x3= point_[2]->x();
	qreal y1= point_[0]->y();
	qreal y2= point_[1]->y();
	qreal y3= point_[2]->y();


	// compute the mid point first
	float mx, my;
    //    |x1^2+y1^2  y1  1|        |x1  x1^2+y1^2  1|
    //    |x2^2+y2^2  y2  1|        |x2  x2^2+y2^2  1|
    //    |x3^2+y3^2  y3  1|        |x3  x3^2+y3^2  1|
    // mx = ------------------,  my = ------------------
    //      |x1  y1  1|               |x1  y1  1|
    //    2*|x2  y2  1|             2*|x2  y2  1|
    //      |x3  y3  1|               |x3  y3  1|

	float det_2 = (x1 * y2 + y1 * x3  + x2 * y3
			    - x1 * y3 - y1 * x2  - y2 * x3 ) * 2;

	mx = ( (x1*x1 + y1*y1) * y2 + y1 * (x3*x3 + y3*y3 ) + (x2*x2 + y2*y2)*y3
	   - (x1*x1 + y1*y1) * y3 - y1 * ( x2*x2 + y2*y2) - y2 * (x3*x3 + y3*y3 ))
	   / det_2;

	my = ( x1 * (x2*x2 + y2*y2) + (x1*x1 + y1*y1) * x3 + x2 * ( x3*x3 + y3*y3)
	   - x1 * (x3*x3 + y3*y3) - (x1*x1 + y1*y1) * x2 - (x2*x2 + y2*y2) * x3)
	   / det_2;

	QPointF midpoint = QPointF(mx, my);
	float radius = sqrt( (x1 - mx)* (x1 - mx) + (y1 - my) * (y1 - my));


	// propagate the computed model parameter
	assert (output_);
	((GeometricCircle *)output_)->setMidpoint(midpoint);
	((GeometricCircle *)output_)->setRadius(radius);
}

bool GeometricInputCircle3PointAnnotator::addPoint(GraphicsPointItem * p) {
	assert(p);
	if (point_[0] == 0) { point_[0] = p; return false ; }
	if (point_[1] == 0) { point_[1] = p; return false ; }
	if (point_[2] == 0) { point_[2] = p; return true  ; }
	assert (true); // should never reach this line
	return false;  // just to please the compiler ;-)
}

void GeometricInputCircle3PointAnnotator::connectPoints(QGraphicsItem* qgi) {
	assert (point_[0]);
	assert (point_[1]);
	assert (point_[2]);
	// need to be updated in case of a position change
	point_[0]->setGeometricInput(this);
	point_[1]->setGeometricInput(this);
	point_[2]->setGeometricInput(this);
	point_[0]->setDependentShape(qgi);
	point_[1]->setDependentShape(qgi);
	point_[2]->setDependentShape(qgi);

	// also relocate the points as childs to the GraphicsView
	// --> will be removed when the object is removed


}


GeometricInputCircle3PointAnnotator::~GeometricInputCircle3PointAnnotator() {
	qDebug() << "--GeometricInputCircle3PointAnnotator::"
			"~GeometricInputCircle3PointAnnotator--";
	// ToDo rework deletion
	//if (point_[0]) delete point_[0];
	//if (point_[1]) delete point_[1];
	//if (point_[2]) delete point_[2];
}


