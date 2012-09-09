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

#define _USE_MATH_DEFINES

#include <QDebug>
#include <assert.h>

#include "line.h"
#include "measurement_line.h"


//----------------------------------------------------------------------------

FactoryLineMarker::FactoryLineMarker()
	: GeometricObjectFactory() { input_ = 0; }

GeometricObject* FactoryLineMarker::finalizeObjects() {


	GeometricLine* geometric_line = new GeometricLine();
	GraphicsLineItem* graphics_line = new GraphicsLineItem();

	qDebug() << "--FactoryLineMarker::finalizeObjects-- line objects exist";
	assert (input_);
	assert (geometric_line);
	assert (graphics_line);

	geometric_line->setInput(input_);
	input_->setOutput(geometric_line);
	input_->computeModel();
	graphics_line->setGeometricObject((GeometricObject*)geometric_line);
	graphics_line->setPos(computeMidpoint(geometric_line->getStart(),
			geometric_line->getEnd()));

	// setup the measurements
	MeasurementOnObject*  measurement;
	measurement = FactoryMeasurementOnLine::create(geometric_line);
	geometric_line->setMeasurement(measurement);

	qDebug() << "--FactoryLineMarker::finalizeObjects-- cross linking is done";
	assert (scene_);
	scene_->addItem(graphics_line);
	// make all the connections
	input_->connectPoints(graphics_line);

	// initially setup the measures
	measurement->updateMeasures();

	qDebug() << "--FactoryLineMarker::finalizeObjects-- drawing was finished  - do exit";
	return geometric_line;

}

GeometricInput*   FactoryLineMarker::createInput() {
		qDebug() << "--FactoryLineMarker::createInput-- start a new line object";
		input_ = new GeometricInputLineMarker();
		assert ( input_);

}

//----------------------------------------------------------------------------

GeometricInputLineMarker::GeometricInputLineMarker() :
		GeometricInput() {
	point_[0] = 0;
	point_[1] = 0;
}

void GeometricInputLineMarker::computeModel(void) {
	assert (point_[0]); // make sure all data is available
	assert (point_[1]); // make sure all data is available

	// propagate the computed model parameter
	assert (output_);

	QPointF start(point_[0]->x(),point_[0]->y());
	QPointF   end(point_[1]->x(),point_[1]->y());

	((GeometricLine *)output_)->setStart(start);
	((GeometricLine *)output_)->setEnd(end);
}

bool GeometricInputLineMarker::addPoint(GraphicsPointItem * p) {
	assert(p);
	if (point_[0] == 0) { point_[0] = p; return false ; }
	if (point_[1] == 0) { point_[1] = p; return true  ; }
	assert (true); // should never reach this line
	return false;  // just to please the compiler ;-)
}

void GeometricInputLineMarker::connectPoints(QGraphicsItem* qgi) {
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

GeometricInputLineMarker::~GeometricInputLineMarker() {
	// let qt scene delete the points since they are registered over there
}

//---------------------------------------------------------------------------


GeometricLine::GeometricLine() : GeometricObject() {
	start_ = QPointF(0,0);
	type_ = "line";
	end_ = QPointF(0,0);
}

const QString GeometricLine::getModelParameterName(int i) const {
	QString ret("");
	switch ( i ) {
	case 0:
		ret = "start";
		break;
	case 1:
		ret = "end";
		break;
	}
	return ret;
}
const QVariant GeometricLine::getModelParameterData(int i) const {
	QVariant ret = QVariant("");
	switch ( i ) {
	case 0:
		ret = QVariant(start_);
		break;
	case 1:
		ret = QVariant(end_);
		break;
	}
	return ret;

}

//----------------------------------------------------------------------------

QRectF GraphicsLineItem::boundingRect() const {
	QPointF start = ((GeometricLine*)geometric_object_)->getStart();
	QPointF end = ((GeometricLine*)geometric_object_)->getEnd();
	qreal width = abs(start.rx() - end.rx());
	qreal height = abs(start.ry() - end.ry());
	QRectF rect(-width / 2 , - height / 2 ,width , height);
	return rect;
}

void GraphicsLineItem::paint(QPainter *painter,
		const QStyleOptionGraphicsItem *option, QWidget *widget) {
	QPen pen (color_, 1, Qt::SolidLine, Qt::RoundCap);
	if (option->state & QStyle::State_Selected) {
		pen.setColor(selected_color_);
	}
	painter->setPen(pen);
	QPointF start = ((GeometricLine*)geometric_object_)->getStart();
	QPointF end = ((GeometricLine*)geometric_object_)->getEnd();
	QPointF midpoint = computeMidpoint(start,end);

	painter->drawLine (start - midpoint ,  end - midpoint );
}

QVariant GraphicsLineItem::itemChange(GraphicsItemChange change,
		const QVariant &value) {

	qDebug() << "--GraphicsLineItem::itemChange-- change type: " << change;
	if (change == ItemSelectedHasChanged) {
		setPos(computeMidpoint(
				((GeometricLine*)geometric_object_)->getStart(),
				((GeometricLine*)geometric_object_)->getEnd()));

		QPointF position = pos();
		qDebug() << "--GraphicsLineItem::itemChange-- New Midpoint:  "
				 << "(" << QString::number(position.rx())
			     << QString::number(position.ry()) << ")";
	}

	return GraphicsItem::itemChange(change, value);
}

GraphicsLineItem::GraphicsLineItem() :
	GraphicsItem() {
	color_ = Qt::blue;
	selected_color_ = Qt::red;
	setFlags(QGraphicsItem::ItemIsSelectable );
}
GraphicsLineItem::~GraphicsLineItem() {
	;
}


