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

#ifndef DEF_OBJECT_LINE_H_
#define DEF_OBJECT_LINE_H_

#include <QGraphicsView>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>

#include <graphics_point_item.h>
#include <object_base.h>

class FactoryLineMarker : public GeometricObjectFactory {
public:
	FactoryLineMarker() ;
	~FactoryLineMarker() {}

protected:
	GeometricInput*   createInput() ;
	GeometricObject*  finalizeObjects();

};


class GeometricInputLineMarker : public GeometricInput {
public:
	GeometricInputLineMarker() ;
	virtual ~GeometricInputLineMarker();
	bool addPoint(GraphicsPointItem * p);
	void connectPoints(QGraphicsItem* qgi);
	void computeModel(void);
private:
	GraphicsPointItem* point_[2];
};


class GeometricLine : public GeometricObject {
public:

	GeometricLine();
	~GeometricLine() {};
	void setStart(QPointF s) {start_ = s; }
	void setEnd(QPointF e)     {end_ = e;   }

	QPointF getStart()      { return start_;  }
	QPointF getEnd()        { return end_;  }

	const QString getModelParameterName(int i) const ;
	const QVariant getModelParameterData(int i) const;

private:
	QPointF start_;
	QPointF end_;
};


class GraphicsLineItem : public GraphicsItem {
public:
	GraphicsLineItem();
	virtual ~GraphicsLineItem();
	void setColor(const QColor & color) { color_ = color; }
	void setSelectedColor(const QColor & color) {selected_color_ = color; }

	QRectF boundingRect() const;
	void paint(QPainter *painter,
			const QStyleOptionGraphicsItem *option, QWidget *widget);
	// void setGeometricCircle(GeometricCircle *gc) {geometric_circle_ = gc; }
protected:
	QVariant itemChange(GraphicsItemChange change,
			const QVariant &value);
	QColor color_;
	QColor selected_color_;
	// GeometricCircle* geometric_circle_;
};


#endif /* DEF_OBJECT_LINE_H_ */
