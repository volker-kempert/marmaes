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
#include "graphics_point_item.h"

GraphicsPointItem::GraphicsPointItem()
	: cross_axis_length_(8) {
	color_ = Qt::green;
	selected_color_ = Qt::darkGreen;

	setFlags(QGraphicsItem::ItemIsSelectable |
			 QGraphicsItem::ItemIsMovable );
	setZValue(1);  // always on top
	geometric_input_ = 0;
	dependent_shape_ = 0;
	// setAcceptDrops(true);   // be notified in case of a drag/drop
}

GraphicsPointItem::~GraphicsPointItem() {

}

QRectF GraphicsPointItem::boundingRect() const {
	QRectF rect( -cross_axis_length_ ,  - cross_axis_length_,
			2 * cross_axis_length_ ,  2 * cross_axis_length_ );
	return rect;
}

void GraphicsPointItem::paint(QPainter *painter,
		const QStyleOptionGraphicsItem *option, QWidget *widget) {
	QPen pen (color_, 1, Qt::SolidLine, Qt::RoundCap);
	if (option->state & QStyle::State_Selected) {
		pen.setColor(selected_color_);
	}
	painter->setPen(pen);
	painter->drawLine(-cross_axis_length_, 0, -2,                 0); // west
	painter->drawLine(2,                   0, cross_axis_length_, 0); // east
	painter->drawLine(0, -cross_axis_length_, 0,                 -2); // north
	painter->drawLine(0,                   2, 0, cross_axis_length_); // south

}


QVariant GraphicsPointItem::itemChange(GraphicsItemChange change,
		const QVariant &value) {
	qDebug() << "--GraphicsPointItem::itemChange-- " << change;
	if (change == ItemSelectedHasChanged) {
	// if (true ) {
		QPointF position = pos();
		qDebug() << "-GraphicsPointItem::itemChange--"
				 << " Position:  (" << QString::number(position.rx())
				 << QString::number(position.ry()) << ")";
		// position has changed -> inform potential connected objects
		if (geometric_input_ )  geometric_input_->computeModel();
		// propagate the change information to the dependent object
		if (dependent_shape_ )  dependent_shape_->update();
	}
	return QGraphicsItem::itemChange(change, value);
}

void GraphicsPointItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) {
	qDebug() << "--GraphicsPointItem::mouseReleaseEvent--";
	// position has changed -> inform potential connected objects
	if (geometric_input_ )  geometric_input_->computeModel();
	// propagate the change information to the dependent object
	if (dependent_shape_ )  dependent_shape_->update();
	// make the dependent object selected
	// TODO check if other objects need to be deselected
	if (dependent_shape_ )  dependent_shape_->setSelected(true);
	QGraphicsItem::mouseReleaseEvent(event);
}

