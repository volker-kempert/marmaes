/*
 * graphics_circle.h
 *
 *  Created on: 11.02.2011
 *      Author: volker
 */

#ifndef DEF_OBJECT_CIRCLE_2POINT_H_
#define DEF_OBJECT_CIRCLE_2POINT_H_

#include <QGraphicsView>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>

#include "graphics_point_item.h"
#include "object_base.h"

class FactoryCircle2PointMarker : public GeometricObjectFactory {
	// is a factory for GeometricCircle (s)
public:
	FactoryCircle2PointMarker() ;
	~FactoryCircle2PointMarker() {}

protected:
	GeometricInput*   createInput() ;
	GeometricObject*  finalizeObjects();

	// void createObjects(QGraphicsScene* scene);

};


class GeometricInputCircle2PointMarker : public GeometricInput {
	// midpoint + one point on the circumference  computation strategy
	// including the model
public:
	GeometricInputCircle2PointMarker() ;
	virtual ~GeometricInputCircle2PointMarker();
	bool addPoint(GraphicsPointItem * p);
	void connectPoints(QGraphicsItem* qgi);
	void computeModel(void);
private:
	GraphicsPointItem* point_[2];
};


#endif /* DEF_OBJECT_CIRCLE_2POINT_H_ */
