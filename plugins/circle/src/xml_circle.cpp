/*
 * xml_circle.cpp
 *
 *  Created on: 28.08.2011
 *      Author: volker
 */

#include <QDebug>
#include <assert.h>
#include <QVariant>

#include "xml_circle.h"
#include "circle.h"
#include "measurement_circle.h"
#include "basic_type_read_write.h"


void CircleReader::initObjects() {
	GeometricCircle* geometric_circle = new GeometricCircle();
	object_ = (GeometricObject*) geometric_circle;
	measurement_ = FactoryMeasurementOnCircle::create(geometric_circle);
}

bool CircleReader::readGeometricObject(QXmlStreamReaderWhitespace& reader) {
	reader.readNext();
	QVariant var ;
	QPointF midpoint;
	double radius;
	QVariantReader midpoint_reader("midpoint", var);
	QVariantReader radius_reader("radius", var);
	for (int i = 0 ; i < 2 ; i++) { // it is expected to read exactly two nodes
		bool successful_read = false;
		if (midpoint_reader.checkToken(reader))
			if ( midpoint_reader.read(reader)) {
				midpoint = var.toPointF();
				successful_read = true;
			}
		if (radius_reader.checkToken(reader))
			if ( radius_reader.read(reader)) {
				radius = var.toDouble(&successful_read);
			}
		if (!successful_read ) return false;
	}
	if (reader.isEndElement() && reader.name() == "model" ) return false ;
	// create the object
	GeometricCircle* geometric_circle = new GeometricCircle();
	geometric_circle->setMidpoint(midpoint);
	geometric_circle->setRadius(radius);
	object_ = (GeometricObject*) geometric_circle;
	reader.readNext();
	return true;

}

bool CircleReader::checkToken(const QXmlStreamReaderWhitespace& reader) {
	if (! reader.isStartElement()) return false;
	if ( reader.name() != "object" ) return false;
	QStringRef type = reader.attributes().value("type");
	if ( type != "circle") return false;
	return true;
}


void CircleReader::finalize() {

	GraphicsCircleItem* graphics_circle = new GraphicsCircleItem();
	assert (graphics_circle);

	graphics_circle->setGeometricObject(object_);
	qDebug() << "--CircleReader::finalize-- incomplete";
	assert(false);
	graphics_circle->setPos(((GeometricCircle*)object_)->getMidpoint());
	qDebug() << "--CircleReader::finalize-- set Scene" << (long)scene_;
	scene_->addItem(graphics_circle);
	qDebug() << "--CircleReader::finalize-- done";

}


