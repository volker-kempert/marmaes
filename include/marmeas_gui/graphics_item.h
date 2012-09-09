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

#ifndef GRAPHICS_ITEM_H_
#define GRAPHICS_ITEM_H_

#include <QGraphicsScene>
#include <QGraphicsItem>
#include "geometric_object.h"


/*!
 * @class       GraphicsItem
 * @description
 */
class GraphicsItem : public QGraphicsItem {
public:
	GraphicsItem() : QGraphicsItem() {geometric_object_ = 0; }
	virtual ~GraphicsItem();
	void setGeometricObject(GeometricObject* go) {geometric_object_ = go; }
	GeometricObject* getGeometricObject() {return geometric_object_; }

	// set the custom type for all derived classes
	enum { Type = UserType + 1 };
	// Enable the use of qgraphicsitem_cast with this item.
    int type() const  {  return Type; }

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	GeometricObject* geometric_object_;
};


#endif /* GRAPHICS_ITEM_H_ */
