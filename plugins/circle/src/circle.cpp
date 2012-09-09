/*
 * graphics_circle.cpp
 *
 *  Created on: 11.02.2011
 *      Author: volker
 */

#define _USE_MATH_DEFINES


#include <QDebug>
#include <math.h>
#include <assert.h>

#include "circle.h"
#include "measurement_circle.h"



//----------------------------------------------------------------------------

#include <QVariant>
#include "basic_type_read_write.h"



GeometricCircle::GeometricCircle() : GeometricObject() {
	radius_ = 0;
	type_ = "Circle";
	midpoint_ = QPointF(0,0);
}

const QString GeometricCircle::getModelParameterName(int i) const {
	QString ret("");
	switch ( i ) {
	case 0:
		ret = "Midpoint";
		break;
	case 1:
		ret = "Radius";
		break;
	}
	return ret;
}
const QVariant GeometricCircle::getModelParameterData(int i) const {
	QVariant ret = QVariant("");
	switch ( i ) {
	case 0:
		ret = QVariant(midpoint_);
		break;
	case 1:
		ret = QVariant((double)radius_);
		break;
	}
	return ret;

}

//----------------------------------------------------------------------------

QRectF GraphicsCircleItem::boundingRect() const {
	float radius = ((GeometricCircle*)geometric_object_)->getRadius();
	QRectF rect(-radius, - radius, 2* radius, 2*radius);
	return rect;
}

void GraphicsCircleItem::paint(QPainter *painter,
		const QStyleOptionGraphicsItem *option, QWidget *widget) {
	QPen pen (color_, 1, Qt::SolidLine, Qt::RoundCap);
	if (option->state & QStyle::State_Selected) {
		pen.setColor(selected_color_);
	}
	painter->setPen(pen);
	setPos(((GeometricCircle*)geometric_object_)->getMidpoint());
	QPointF position = pos();
	float radius = ((GeometricCircle*)geometric_object_)->getRadius();
	// QPointF m = ge_c->getMidpoint();
	painter->drawEllipse (- radius, -radius, 2*radius, 2* radius);
}

QVariant GraphicsCircleItem::itemChange(GraphicsItemChange change,
		const QVariant &value) {

	qDebug() << "circle itemChange: " << change;
	if (change == ItemSelectedHasChanged) {
		setPos(((GeometricCircle*)geometric_object_)->getMidpoint());
		QPointF position = pos();
		qDebug() << " CirlceItem - New Midpoint:  (" << QString::number(position.rx())
			 << QString::number(position.ry()) << ")";
		qDebug() << " CirlceItem New Radius:  (" << QString::number(
				((GeometricCircle*)geometric_object_)->getRadius());
	}

	return GraphicsItem::itemChange(change, value);
}

GraphicsCircleItem::GraphicsCircleItem() :
	GraphicsItem() {
	color_ = Qt::blue;
	selected_color_ = Qt::red;
	setFlags(QGraphicsItem::ItemIsSelectable );
}
GraphicsCircleItem::~GraphicsCircleItem() {
	;
}


