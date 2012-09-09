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
#include "geometric_object_container.h"
#include "object_base_read_write.h"


GeometricObjectContainerSingleton::~GeometricObjectContainerSingleton() {
	qDebug() << "--GeometricObjectContainerSingleton"
			"::~GeometricObjectContainerSingleton" << (long)the_container_;
	the_container_ = 0 ;
}


GeometricObjectContainerSingleton*
	GeometricObjectContainerSingleton::the_container_ = 0;

GeometricObjectContainer*
	GeometricObjectContainerSingleton::getInstance() {
    if (!the_container_) {
    	qDebug() << "--GeometricObjectContainerSingleton"
    			"::getInstance-- create new instance";
        the_container_ = new GeometricObjectContainerSingleton();
    }
    return the_container_;
}


//-----------------------------------------------------------------------------

GeometricObjectContainer::GeometricObjectContainer() : QObject() {
	image_ = 0;
}

GeometricObjectContainer::~GeometricObjectContainer() {
	qDebug() << "--GeometricObjectContainer"
			"::~GeometricObjectContainer" ;
	removeAllObjects();
	setImage(0);
	emit objectContainerDeleted(this);
}

void GeometricObjectContainer::addObject(GeometricObject* object) {
	assert(object != 0);
	qDebug() << "--GeometricObjectContainer::addObject-- addr: "
			 << ((long)object) << " Id: " << object->getId();
	// do nothing if no image is set
	if (image_) {
		list_.append(object);
		emit objectsChanged();
	} else
		qDebug() << "--GeometricObjectContainer::addObject-- "
					"empty image - do nothing";
}

void GeometricObjectContainer::removeAllObjects() {
	qDebug() << "--GeometricObjectContainer::removeAllObjects--";
	while ( ! list_.isEmpty() ) 	{
		// removeObject(list_.at(0));
		GeometricObject* obj;
		obj = list_.takeFirst();
		emit objectDeleted(obj);
		delete obj;
	}
	emit objectsEmptied();
	qDebug() << "--GeometricObjectContainer::removeAllObjects-- done";
}

void GeometricObjectContainer::removeObject(GeometricObject* object) {
	qDebug() << "--GeometricObjectContainer::removeObject-- addr: "
			 << ((long)object) << " Id: " << object->getId();
	list_.removeOne(object);
	qDebug() << "--GeometricObjectContainer::removeObject-- 2 ";
	delete object;
	qDebug() << "--GeometricObjectContainer::removeObject-- 3 ";
	emit objectDeleted(object);
	emit objectsChanged();
	if (list_.size() == 0) emit objectsEmptied();
}

GeometricObject* GeometricObjectContainer::getObjectById(long id) {
	qDebug() << "--GeometricObjectContainer::getObjectById--"
			 << " Id: " << id;
	for (int i = 0 ; i < list_.size(); i++ )
		if (list_[i]->getId() == id ) return list_[i];
	return 0;
}

bool GeometricObjectContainer::write(QXmlStreamWriter& writer) {
	qDebug() << "--GeometricObjectContainer::write--"
			 << " Number of objects to write: " << list_.size();
	for (int i = 0 ; i < list_.size(); i++ ) {
		qDebug() << "write object: " << ((long)list_[i]);
		ObjectBaseWriter w(list_[i]);
		if ( !w.write(writer)) return false;

	}
	return true;
}

/*!
 * @brief handle the changed definition (new model parameters) of an object.
 *        i.e. update all measurement and notify the rest of the world
 *        that the set of objects changed
 */
void GeometricObjectContainer::changeObject(GeometricObject* object) {
	qDebug() << "--GeometricObjectContainer::changeObject--";
	object->getMeasurement()->updateMeasures();
	emit objectsChanged();
}

/*!
 * @description: set an image removes all objects because
 *               objects only make sense if there is an image they refer to
 * @param image is an pointer to a QImage;
 *         if zero it reset's the image is reset
 */
void GeometricObjectContainer::setImage(QImage* image) {
	qDebug() << "--GeometricObjectContainer::setImage--";
	// remove any existing objects
	removeAllObjects();
	// if (image_) delete image_;
	image_ = image;
	qDebug() << "--GeometricObjectContainer::setImage-- emit imageAvailable(image_)";
	emit imageChanged(image_);
}
