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
#include "2point_circle.h"

FactoryCircle2PointMarker::FactoryCircle2PointMarker()
	: GeometricObjectFactory() { input_ = 0; }

GeometricObject* FactoryCircle2PointMarker::finalizeObjects() {


	GeometricCircle* geometric_circle = new GeometricCircle();
	GraphicsCircleItem* graphics_circle = new GraphicsCircleItem();

	qDebug() << "--FactoryCircle2PointMarker::finalizeObjects--"
			 << " circle objects exist";
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

	qDebug() << "--FactoryCircle2PointMarker::finalizeObjects--"
			 << " cross linking is done";
	assert (scene_);
	scene_->addItem(graphics_circle);
	// make all the connections
	input_->connectPoints(graphics_circle);

	qDebug() << "--FactoryCircle2PointMarker::finalizeObjects--"
	         << " drawing was finished";
	return geometric_circle;

}

GeometricInput*   FactoryCircle2PointMarker::createInput() {
		qDebug() << "--FactoryCircle2PointMarker::createInput--";
		input_ = new GeometricInputCircle2PointMarker();
		assert ( input_);

}


GeometricInputCircle2PointMarker::GeometricInputCircle2PointMarker() :
		GeometricInput() {
	qDebug() << "--GeometricInputCircle2PointMarker::"
			"GeometricInputCircle2PointMarker--";
	point_[0] = 0;
	point_[1] = 0;
}

void GeometricInputCircle2PointMarker::computeModel(void) {
	qDebug() << "--GeometricInputCircle2PointMarker::computeModel--";
	assert (point_[0]); // make sure all data is available
	assert (point_[1]); // make sure all data is available

	float mx, my;
	mx = point_[0]->x(); // first point is the midpoint
	my = point_[0]->y();

	float x1, y1;
	x1 = point_[1]->x();
	y1 = point_[1]->y(); // second point is somewhere on the circumference


	QPointF midpoint = QPointF(mx, my);
	float radius = sqrt( (x1 - mx)* (x1 - mx) + (y1 - my) * (y1 - my));


	// propagate the computed model parameter
	assert (output_);
	((GeometricCircle *)output_)->setMidpoint(midpoint);
	((GeometricCircle *)output_)->setRadius(radius);
}

bool GeometricInputCircle2PointMarker::addPoint(GraphicsPointItem * p) {
	assert(p);
	if (point_[0] == 0) { point_[0] = p; return false ; }
	if (point_[1] == 0) { point_[1] = p; return true  ; }
	assert (true); // should never reach this line
	return false;  // just to please the compiler ;-)
}

void GeometricInputCircle2PointMarker::connectPoints(QGraphicsItem* qgi) {
	assert (point_[0]);
	assert (point_[1]);
	// need to be updated in case of a position change
	point_[0]->setGeometricInput(this);
	point_[1]->setGeometricInput(this);
	point_[0]->setDependentShape(qgi);
	point_[1]->setDependentShape(qgi);

	// also relocate the points as childs to the GraphicsView
	// --> will be removed when the object is removed


}


GeometricInputCircle2PointMarker::~GeometricInputCircle2PointMarker() {
	qDebug() << "--GeometricInputCircle2PointMarker::"
			"~GeometricInputCircle2PointMarker--";
	// ToDo rework deletion model of graphic objects
	//if (point_[0]) delete point_[0];
	//if (point_[1]) delete point_[1];
}


