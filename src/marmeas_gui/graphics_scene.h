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

#ifndef GRAPHICS_SCENE_H_
#define GRAPHICS_SCENE_H_

#include <QGraphicsScene>
#include "graphics_point_item.h"
#include "object_base.h"



typedef enum  {
	AddObject,
	Selection,
	Navigation
} SceneStatus;

/*
 * @description: Scene to operate with in the MARMEAS context
 *               is responsible for status changes,
 *               scene coordinate limits etc
 */

class GraphicsScene : public QGraphicsScene {

    Q_OBJECT

public:
	GraphicsScene ( QObject * parent = 0 );
	void setImage(QImage* image);
	/*
	 * @description: provides the image for intensity analysis
	 * @return: pointer to an image; if zero there is no image set
	 */
	void setStatus(SceneStatus status);
	virtual ~GraphicsScene();
public slots:
	void deleteItem();
	void deleteItem(GeometricObject* object);
	void editItem();
	GeometricObject* emitSelectedObject(void);
	void setObjectFactory(GeometricObjectFactory *);

signals:
    /*
     *  @description: signal that indicates that another object is selected
     *  @param GeometricObject*: the selected object
     */
	void objectSelected(GeometricObject* obj);
	void imageAvailable(bool available);
	/*!
	 * is signaled whenever a graphic item is removed and is is required
	 * to remove a linked geometric object as well
	 */
	void objectRemoved(GeometricObject* obj);

protected:
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


private:

	SceneStatus status_;
	// ObjectTemplateContainer* object_templates_;
	GeometricObjectFactory* object_creator_;
	// item that represents the image
	QGraphicsPixmapItem* pixmap_;




};

#endif /* GRAPHICS_SCENE_H_ */
