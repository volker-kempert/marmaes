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

#ifndef GEOMETRIC_OBJECT_CONTAINER_H_
#define GEOMETRIC_OBJECT_CONTAINER_H_

#include <QObject>
#include <QList>
#include <QString>
#include <QImage>

#include "geometric_object.h"


/*!
 * @brief container for geometric object and related to manage the data storage
 *
 * the object container is responsible for maintaining lifetime, i.e. deletion
 * of contained objects; thus, objects that are added to the container must be
 * created using the "new operator" in one or the other way
 *
 * The object container is also responsible for maintaining the image where all
 * objects refer to.
 *
 * Business constraint:
 * objects can only be added if an image is set.
 * if an image is removed all objects will be removed as well
 *
 */

class GeometricObjectContainer : public QObject {
    Q_OBJECT
public:
	virtual ~GeometricObjectContainer();
	GeometricObjectContainer();

	void addObject(GeometricObject* object);
	void removeAllObjects();
	GeometricObject* getObjectById(long id);
	bool write(QXmlStreamWriter& writer);

	void setImage(QImage* image);
	/*
	 * @description: provides the image for intensity analysis
	 * @return: pointer to an image; if zero there is no image set
	 */
	QImage* image() const { return image_; }

public slots:
	void changeObject(GeometricObject* object);
	void removeObject(GeometricObject* object);

signals:
	// emitted whenever a object is added, removed of anything of the object
 	// has changed
    void objectsChanged();
    // emitted after the last object has been removed
    void objectsEmptied();
    // whenever a new image is is set
    void imageChanged(QImage*);
    // emitted whenever an object gets removed and deleted
    // directly after the delete call
    void objectDeleted(GeometricObject* object);
    // emitted whenever the container gets destroyed
    void objectContainerDeleted(GeometricObjectContainer* container);
private:
	QList<GeometricObject*> list_;
	// image to reference to
	QImage* image_;

};


/*!
 * @brief convenience factory that allows to access object container as
 *        singletons
 *
 *
 */

class GeometricObjectContainerSingleton : public GeometricObjectContainer {
public:
	static GeometricObjectContainer* getInstance();
	virtual ~GeometricObjectContainerSingleton() ;
private:
	GeometricObjectContainerSingleton() : 	GeometricObjectContainer() {}
	static GeometricObjectContainerSingleton* the_container_;
};



#endif /* GEOMETRIC_OBJECT_CONTAINER_H_ */
