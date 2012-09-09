/*
 * graphics_circle.h
 *
 *  Created on: 11.02.2011
 *      Author: volker
 */

#ifndef DEF_OBJECT_CIRCLE_3POINT_H_
#define DEF_OBJECT_CIRCLE_3POINT_H_

#include <QGraphicsView>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>

#include "graphics_point_item.h"
#include "object_base.h"

class FactoryCircle3PointAnnotator : public GeometricObjectFactory {
	// is a factory for GeometricCircle (s)
public:
	FactoryCircle3PointAnnotator() ;
	~FactoryCircle3PointAnnotator() {}

protected:
	GeometricInput*   createInput() ;
	GeometricObject*  finalizeObjects();

	// void createObjects(QGraphicsScene* scene);

};


class GeometricInputCircle3PointAnnotator : public GeometricInput {
	// 3point annotator computation strategy
	// including the model
public:
	GeometricInputCircle3PointAnnotator() ;
	virtual ~GeometricInputCircle3PointAnnotator();
	bool addPoint(GraphicsPointItem * p);
	void connectPoints(QGraphicsItem* qgi);
	void computeModel(void);
private:
	GraphicsPointItem* point_[3];
};


#endif /* DEF_OBJECT_CIRCLE_3POINT_H_ */
