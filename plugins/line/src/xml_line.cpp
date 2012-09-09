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
#include <assert.h>
#include <QVariant>

#include "xml_line.h"
#include "line.h"
#include "measurement_line.h"
#include "basic_type_read_write.h"

void LineReader::initObjects() {
	GeometricLine* geometric_line = new GeometricLine();
	object_ = (GeometricObject*) geometric_line;
	measurement_ = FactoryMeasurementOnLine::create(geometric_line);
}

bool LineReader::readGeometricObject(QXmlStreamReaderWhitespace& reader) {
	reader.readNext();
	QVariant var_start ;
	QVariant var_end ;
	QPointF start;
	QPointF end;
	QVariantReader start_reader("start", var_start);
	QVariantReader end_reader("end", var_end);
	bool successful_read_start = false;
	bool successful_read_end = false;
	int loop_count = 0;
	while (! reader.isEndDocument()) {
		qDebug() << "--LineReader::readGeometricObject-- sub node " << reader.name();
		if (start_reader.checkToken(reader))
		//if (reader.isStartElement() && reader.name() == "start")
			if ( start_reader.read(reader)) {
				start = var_start.toPointF();
				successful_read_start = true;
			}
		if (end_reader.checkToken(reader))
			if ( end_reader.read(reader)) {
				end = var_end.toPointF();
				successful_read_end = true;
			}
		if ( successful_read_start && successful_read_end ) break;
		// emergency exit in case no sub items match
		if (loop_count++ > 1 ) return false ;
	}

	qDebug() << "--LineReader::readGeometricObject-- finished sub nodes" ;
	if (!successful_read_start || !successful_read_end ) return false;
	qDebug() << "--LineReader::readGeometricObject-- end tag?? " << reader.name();
	if (! reader.isEndElement()) return false;
	else if ( reader.name() != "model" ) return false ;
	qDebug() << "--LineReader::readGeometricObject-- successfully read" ;
	((GeometricLine*)object_)->setStart(start);
	((GeometricLine*)object_)->setEnd(end);
	reader.readNext();
	qDebug() << "--LineReader::readGeometricObject-- successfully init" ;
	return true;
}

bool LineReader::checkToken(const QXmlStreamReaderWhitespace& reader) {
	if (! reader.isStartElement()) return false;
	if ( reader.name() != "object" ) return false;
	QStringRef type = reader.attributes().value("type");
	if ( type != "line") return false;
	return true;
}

void LineReader::finalize() {

	GraphicsLineItem* graphics_line = new GraphicsLineItem();
	assert (graphics_line);

	graphics_line->setGeometricObject(object_);
	qDebug() << "--LineReader::finalize--  imcomplete";
	graphics_line->setPos(computeMidpoint(((GeometricLine*)object_)->getStart(),
			((GeometricLine*)object_)->getEnd()));
	qDebug() << "--LineReader::finalize-- add to scene: " << long(scene_);
	scene_->addItem(graphics_line);
	qDebug() << "--LineReader::finalize-- done";
}

