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
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QList>

#include "graphics_scene.h"
#include "object_base.h"


GraphicsScene::GraphicsScene(QObject * parent ) : QGraphicsScene( parent )  {
	qDebug() << "--GraphicsScene::GraphicsScene--";
	status_ = Selection;
	object_creator_ = 0;
	pixmap_ = 0;
	qDebug () << "--GraphicsScene::GraphicsScene--";

	// if the selection has changed emit the aligned geometric object
	connect((QGraphicsScene*)this,SIGNAL(selectionChanged()),
			this,SLOT(emitSelectedObject()));
}

GraphicsScene::~GraphicsScene() {
	if (pixmap_) delete pixmap_;
}
void GraphicsScene::setObjectFactory(GeometricObjectFactory* factory) {
	qDebug () << "--GraphicsScene::setObjectFactory--";
	if (object_creator_) // in case there was object creation started
		object_creator_->rollBack();
	assert( factory != 0 );         // must be meaningful
	object_creator_ = factory;
	object_creator_->setScene(this);
}


GeometricObject* GraphicsScene::emitSelectedObject(void) {
	qDebug() << "--GraphicsScene::emitSelectedObject-- got selection change";
	GeometricObject* obj = 0;
	QList<QGraphicsItem *> selected_items = QGraphicsScene::selectedItems ();
    foreach (QGraphicsItem *item, selected_items) {
		if (item->type() == QGraphicsItem::UserType + 1) {
			// valid cast; since we checked the type upfront
			obj = ((GraphicsItem*)item)->getGeometricObject();
			break;
		}
    }
	qDebug() << "--GraphicsScene::emitSelectedObject-- emit objectSelected: "
			 << (unsigned long)obj;
	emit objectSelected(obj);
    return obj;
}

/*!
 * @brief delete selected item or items
 */
void GraphicsScene::deleteItem() {
	GeometricObject* obj = 0;
	QList<QGraphicsItem *> items = QGraphicsScene::selectedItems();
    foreach (QGraphicsItem *item, items) {
		if (item->type() == QGraphicsItem::UserType + 1) {
			// valid cast; since we checked the type upfront
			obj = ((GraphicsItem*)item)->getGeometricObject();
			qDebug() << "--GraphicsScene::deleteItem-- Id: "
					 << (unsigned long)obj;
			// signal the removal
			emit objectRemoved(obj);
			emit objectSelected(0); // indicate that nothing is selected anymore
		}
		delete item;
    }
}

/*!
 * @brief delete the item that is linked to a geometric object
 * @note there is only one, still the implementation is safe in checking all
 *
 */
void GraphicsScene::deleteItem(GeometricObject* object) {
	qDebug() << "--GraphicsScene::deleteItem-- "
			<< " Target Geometric Object: " << (unsigned long)object ;
	QList<QGraphicsItem *> items = QGraphicsScene::selectedItems();
	GeometricObject* obj;
    foreach (QGraphicsItem *item, items) {
		if (item->type() == QGraphicsItem::UserType + 1) {
			// valid cast; since we checked the type upfront
			obj = ((GraphicsItem*)item)->getGeometricObject();
			qDebug() << "--GraphicsScene::deleteItem-- probe object: "
					 << (unsigned long)obj;
			if ( obj == object ) {
				qDebug() << "--GraphicsScene::deleteItem-- remove now ";
				// signal the removal
				if (item->isSelected())
					// indicate that nothing is selected anymore
					emit objectSelected(0);

				delete item;
				emit objectRemoved(obj);

			}

		}
    }
}

void GraphicsScene::editItem() {
	GeometricObject* obj = emitSelectedObject();
}


/*
 * @description: set an image and the coordinates of the Scene
 * 				 removes all objects because
 *               objects only make sense if there is an image they refer to
 * @param image is an pointer to a QImage;
 *         if zero it reset's the image is reset
 */
void GraphicsScene::setImage(QImage* image) {
	// register the new image
	GeometricObjectContainer* object_container =
			GeometricObjectContainerSingleton::getInstance();
	object_container->setImage(image);
	qDebug() << "--GraphicsScene::setImage-- image address: "
			 << (unsigned long)image;

	if (pixmap_){
		removeItem(pixmap_);
		// free old pixmap??
		delete pixmap_;
	}
	if (!image) pixmap_ = 0;
	else pixmap_ = new QGraphicsPixmapItem();
	if (pixmap_) {
		// set the scene size
		setSceneRect(0,0,image->width(),image->height());
		pixmap_ = addPixmap(QPixmap::fromImage(*image));
		assert(pixmap_);
		pixmap_->setZValue(-1);  // always in the background
	}
	qDebug() << "--GraphicsScene::setImage-- emit imageAvailable() ";
	emit imageAvailable(image != 0);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {


	QPointF position = event->scenePos();
    qDebug() << "--GraphicsScene::mouseReleaseEvent-- at :  ("
    		 << QString::number(position.rx())
			 << "," << QString::number(position.ry()) << ")";

	switch ( status_) {
		case Selection: {
			QGraphicsScene::mouseReleaseEvent(event);
			object_creator_->rollBack();
			return;
		}
		case Navigation: {
			object_creator_->rollBack();
			return;
		}
		case  AddObject: {
			qDebug() << "--GraphicsScene::mouseReleaseEvent-- AddObject";
			GraphicsPointItem* point = new GraphicsPointItem;
			point->setZValue(1); // annotation points should always be on top
			point->setPos(position);
			addItem(point);   // must be visible somehow before the
			                  // object creation is finished
			// assert (object_creator_); //must be different from null
			object_creator_->addPoint(point);
			clearSelection();
			point->setSelected(true);
			break;
		}

	}
	event->accept();
	update();
}

void GraphicsScene::setStatus(SceneStatus status) {
	// do not allow Add status if no image is set
	if (status == AddObject and !pixmap_ ) {
		qDebug() << "--GraphicsScene::setStatus--"
				" forbidden AddObject status at empty image/pixmap";
		assert(false);
	}
	status_ = status;
	switch (status_) {
		case Selection:
		case Navigation:
			object_creator_->rollBack();
	}
}


