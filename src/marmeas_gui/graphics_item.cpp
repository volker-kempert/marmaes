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


#include <QDebug>
#include <QMenu>
#include <QAction>
#include "graphics_item.h"

GraphicsItem::~GraphicsItem() {
}

void GraphicsItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event) {
	qDebug() << "--GraphicsItem::mouseReleaseEvent--";
	// handle the selection of background objects
	if (isSelected()) {

	    QList<QGraphicsItem *> overlap_items;
	    overlap_items = collidingItems(Qt::IntersectsItemShape);

	    foreach (QGraphicsItem *item, overlap_items) {
	        if (item->zValue() !=  -1 &&  // background image
	            item->zValue() !=  1 )    // marking crosses
	            item->setZValue( item->zValue()+0.01);
	              // one of the underlying gets ontop
	              // good enough to embed 99 objects ;-)
	    }
	    setZValue(-0.0);  // this item moves to the back
	}

	QGraphicsItem::mouseReleaseEvent(event);
}

void GraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QMenu menu;
	menu.addAction("Delete");
	menu.addAction("Edit");
	QAction *a = menu.exec();
    qDebug("--GraphicsItem::contextMenuEvent-- User clicked %s", qPrintable(a->text()));
	//connect(menu, SIGNAL(triggered(QAction *)),this, SLOT(triggered(QAction *)));
}

QVariant GraphicsItem::itemChange(GraphicsItemChange change,
		const QVariant &value) {

	qDebug() << "--GraphicItem::itemChange--: " << change;
	return QGraphicsItem::itemChange(change, value);
}



