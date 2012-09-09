/*
 *
 *  Created on: 11.02.2011
 *      Author: volker
 */

#ifndef DEF_OBJECT_CIRCLE_H_
#define DEF_OBJECT_CIRCLE_H_

#include <QGraphicsView>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>

#include "graphics_point_item.h"
#include <object_base.h>
//#include "xml_reader_writer.h"



class GeometricCircle : public GeometricObject {

	Q_PROPERTY(QPointF midpoint  READ getMidpoint  WRITE setMidpoint )
public:

	GeometricCircle();
	~GeometricCircle() {};
	void setMidpoint(QPointF m) {midpoint_ = m; }
	void setRadius(float r)     {radius_ = r;   }
	float getRadius()     const   { return radius_;  }
	QPointF getMidpoint() const   { return midpoint_;  }

	const QString getModelParameterName(int i) const ;
	const QVariant getModelParameterData(int i) const;

private:
	QPointF midpoint_;
	float radius_;
};


class GraphicsCircleItem : public GraphicsItem {
public:
	GraphicsCircleItem();
	virtual ~GraphicsCircleItem();
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



#endif /* DEF_OBJECT_CIRCLE_H_ */
