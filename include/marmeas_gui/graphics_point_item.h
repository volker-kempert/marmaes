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

#ifndef GRAPHICSPOINTITEM_H_
#define GRAPHICSPOINTITEM_H_


#include <QGraphicsView>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QGraphicsItem>

#include "object_base.h"


class GraphicsPointItem : public QGraphicsItem {
	// Q_DECLARE_TR_FUNCTIONS(Point)
public:
	GraphicsPointItem();
	virtual ~GraphicsPointItem();

	void setColor(const QColor & color) { color_ = color; }
	void setSelectedColor(const QColor & color) {selected_color_ = color; }

	QRectF boundingRect() const;
	void setGeometricInput(GeometricInput* gi) {geometric_input_ = gi; }
	void setDependentShape(QGraphicsItem* qgi) {dependent_shape_ = qgi; }
	void paint(QPainter *painter,
			const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
	QVariant itemChange(GraphicsItemChange change,
			const QVariant &value);
	void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

private:
	QColor color_;
	QColor selected_color_;

	// displayed as a cross
	const qreal cross_axis_length_;
	GeometricInput* geometric_input_;
	QGraphicsItem* dependent_shape_;

	// link to annotator object to receive the new position
	// ToDo
};

#endif /* GRAPHICSPOINTITEM_H_ */
