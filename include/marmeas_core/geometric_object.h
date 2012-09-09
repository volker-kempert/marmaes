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

#ifndef GEOMETRIC_OBJECT_H_
#define GEOMETRIC_OBJECT_H_

#include <QObject>
#include <QGraphicsItem>
#include <QString>


#include "measurement_on_object.h"
#include "xml_reader_writer.h"

class GraphicsPointItem;

class IdCounter {
public:
	IdCounter();
	long getId();
	void setId(long id);
private:
	long id_;
};


QPointF computeMidpoint(QPointF p1, QPointF p2);


class GeometricInput ;


class GeometricObject : public QObject {
	// allow properties
	Q_OBJECT
	Q_PROPERTY(QString name   READ getName   WRITE setName )
	Q_PROPERTY(QString type   READ getType   WRITE setType )
	Q_PROPERTY(QString source READ getSource WRITE setSource )
	Q_PROPERTY(long    id     READ getId     WRITE setId )

public:
	GeometricObject ();
	virtual ~GeometricObject ();
	void setType(const QString s)    { type_ = s; }
	void setName(const QString s)    { name_ = s; }
	void setSource(const QString s)  { source_ = s; }
	void setInput(GeometricInput* i) { input_ = i; }
	void setMeasurement(MeasurementOnObject* m) { measurement_ = m; }
	void setId(long id)              { id_ = id; }

	const QString getType() const { return type_; }
	const QString getName() const { return name_; } ;
	const QString getSource() const { return source_; }

	long getId() const { return id_; }
	MeasurementOnObject* getMeasurement() { return measurement_; }

	// to handle in/out of derived classes
	// index runs from zero to xxx
	// for the first invalid index "" is returned as parameter
	// and "" (as QString) for value

	virtual const QString getModelParameterName(int i) const { return  ""; }
	virtual const QVariant getModelParameterData(int i) const { return  QVariant(QString("")); }

signals:
	void objectChanged();

protected:
	QString type_;     // this is a name from the usage domain
	QString name_;     // an optional name (composed of type and id)
	                   // if not set
	long id_;          // an generated unique id
	QString source_;   // link to the data source context (i.e.
	                   // an image and related reference data
	GeometricInput*  input_;
	                   // identifier of the creator / modifier
	                   // could be a name or an algorithm/version
	MeasurementOnObject*  measurement_;
	                   // identifier of the associated measurement

};


/*!
 * @description Interface that handles annotation points, required to
 *              define a specific geometric object uniquely; based on the
 *              given set of points the geometric object paramter are computed
 */

class GeometricInput {
public:
	GeometricInput() { output_ = 0; }
	virtual ~GeometricInput() { ;}
	void setOutput(GeometricObject* o) { output_ = o; }
	/*!
	 * @description interface method that computes the parameter of a
	 *              specific geometric object using the given marking points
	 */
	virtual void computeModel(void) = 0;
	/*!
	 * @description interface method that handles more points that are
	 *              subject to define a new geometric object
	 * @return      indication about the success of adding more points
	 */

	virtual bool addPoint(GraphicsPointItem * p) = 0;
	virtual void connectPoints(QGraphicsItem* qgi) = 0;
protected:
	GeometricObject* output_;
};


/*!
 * @description
 * abstract factory that creates geometric objects.
 * it defines the factory interface for concrete geometric object create
 * the create is a workflow whereby at the beginnig createInput() is called
 * and at the and finalizeObjects(); those methods must be implemented
 * on the derived classes
 */

class GeometricObjectFactory {
public:
	GeometricObjectFactory();
	void setScene(QGraphicsScene* scene) ;
	void setCreator(QString s) { creator_ = s; }
	bool addPoint(GraphicsPointItem * p);
	void rollBack();   // undo a potential half way created object
protected:
	/*!
	 * @description  initialize the creation of a new geometric object
	 *               also resets all so far collected data if required
	 */
	virtual GeometricInput*   createInput() = 0;
	/*!
	 * @description  actually create all desired objects
	 *               at the end, add the newly created geometric object
	 *               to the object container
	 */
	virtual GeometricObject*  finalizeObjects() = 0;
	QString creator_;    // identifier of the creator
	                     // could be a name or an algorithm/version
	bool new_input_;     // create a new input or not
	GeometricInput* input_; // the actual input object
	QGraphicsScene* scene_; // to be used for creation
};


#endif /* OBJECT_BASE_H_ */
