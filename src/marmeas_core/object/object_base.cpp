/*
   Copyright (C) 2012, Volker Kempert <volker.kempert@gmx.net>

   This file is part of an MARMEAS library

   The MARMEAS library is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of
   the License, or (at your option) any later version.

   The MARMEAS library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with MARMEAS. If not, see <http://www.gnu.org/licenses/>.

*/

#include <assert.h>
#include <QDebug>
#include "object_base.h"
#include "object_base_read_write.h"

static IdCounter id_counter;


IdCounter::IdCounter()         { id_ =      0; }
long IdCounter::getId()        { return ++id_; }
void IdCounter::setId(long id) { id_ =     id; }

//----------------------------------------------------------------------------

QPointF computeMidpoint(QPointF p1, QPointF p2) {
	QPointF ret = p1 + p2;
	ret *= 0.5;
	return ret;
}


//-------------------------------------------------------------------------

GeometricObjectFactory::GeometricObjectFactory() {
	creator_ = "-- not set --";
	input_ = 0;
}

void GeometricObjectFactory::setScene(QGraphicsScene* scene) {
	scene_ = scene;
	qDebug() << "--GeometricObjectFactory::setScene-- scene is set";
	assert ( scene_) ;
}

bool GeometricObjectFactory::addPoint(GraphicsPointItem * p) {
	if (input_ == 0) {
		input_ = createInput();
		assert (input_) ;
	}
	if (input_->addPoint(p))  {
		GeometricObject* obj = finalizeObjects();
		GeometricObjectContainer* c =
				GeometricObjectContainerSingleton::getInstance();
		c->addObject(obj);
		input_ = 0;
		return true;
	} else {
		return false;
	}
}

void GeometricObjectFactory::rollBack() {
	if (input_ ) {
		qDebug() << "--GeometricObjectFactory::rollBack--"
				 << " undo half way object creation";
		delete ( input_ );
		input_ = 0;
	}
}

//-------------------------------------------------------------------------

GeometricObject::GeometricObject () {
	qDebug() << "--GeometricObject::GeometricObject--";
	type_= "--not defined --";
	name_ = "";
	id_ = id_counter.getId();
	source_ = "";
	input_ = 0;
	measurement_ = 0;
}

/*!
 * removes itself
 */
GeometricObject::~GeometricObject () {
	qDebug() << "--GeometricObject::~GeometricObject--";
	if (input_ ) delete input_;
	qDebug() << "--GeometricObject::~GeometricObject-- 2";
	if (measurement_) delete measurement_;
	qDebug() << "--GeometricObject::~GeometricObject-- done";

}



