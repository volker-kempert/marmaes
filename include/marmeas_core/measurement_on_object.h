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

#ifndef MEASUREMENT_OBJECT_H_
#define MEASUREMENT_OBJECT_H_

#include <QObject>
#include <QList>
#include <QString>
#include <QPointF>
#include <QVector>
#include <QImage>
#include <QVariant>

class GeometricObject;   // forward declaration
class HistogramStatistics; // forward declaration

typedef unsigned long MeasurementType;

class MeasurementOnObject   : public QObject {

	Q_OBJECT

public:
	enum
	 {
		// begin - same for all measurement on objects
		// to safely handle exceptions explicitly w/o exception
		Undefined                           =  0x0000,
		// reference to the group/set or measurements of the object itself
		Self                                =  0x0001,
		// boolean identifier if the there is any graphics, plot available
		Graphic                             =  0x0002,
		// end  - same for all measurement on objects - superset of _all_ flags
		AllMeasures                         =  0x2ffe
	};


	MeasurementOnObject();              // measurements are only on Objects
	virtual void init(GeometricObject* g = 0);
	virtual ~MeasurementOnObject();
	virtual int version() const;
	MeasurementType actualMeasures() const;

	/*!
	 * @returns the measure for the respective measurement type
	 * the default implementation just throws an exception
	 * the derived implementations have to check and if they cannot handle
	 * delegate the call to the default method.
	 */
	MeasurementType availableMeasures() const;
	/*!
	 * extends the actual measures; if a measure is given that is not
	 * available, it is ignored
	 * @param 'or-ed' combination of measurment types
	 */
	void addActualMeasures( MeasurementType  measure);
	/*!
	 * remove the set of measures
	 * @param 'or-ed' combination of measurement types
	 */
	void removeActualMeasures( MeasurementType  measure);
	/*!
	 * overwrite and handle the specific measures as provided by the
	 * geometric object specific available measures
	 */
	virtual QVariant measurementValue(MeasurementType m) const = 0;
	/*!
	 * overwrite if extension is required for extended MeasurementType(s)
	 * specifically if the name gets a slightly modified meaning according
	 * to the context of the geometric object
	 *
	 * the name might be subject to translation whereby the canonic name
	 * is fixed for xml persistence and arbitrary handling purposes
	 */
    virtual QString measurementName(MeasurementType m) const = 0;
    virtual QString canonicName(MeasurementType m) const = 0;
    virtual MeasurementType type(const QString& canonic_name) const;


public slots:
	virtual void updateMeasures();


protected:
	MeasurementType   actual_measures_;
	MeasurementType   available_measures_;
	GeometricObject*  object_;


};

class GeometricMeasurement : public MeasurementOnObject {
public:
	enum
	 {
		// begin - same for all measurement on objects
		// to safely handle exceptions explicitly w/o exception
		Undefined                           =  0x0000,
		// reference to the group/set or measurements of the object itself
		Self                                =  0x0001,
		// boolean identifier if the there is any graphics, plot available
		Graphic                             =  0x0002,
		// end  - same for all measurement on objects

		GeometricMidpoint                   =  0x0004,
		GeometricXMaxDimension              =  0x0008,
		GeometricYMaxDimension              =  0x0010,
		GeometricBoundingBox                =  0x0020,
		GeometricMaxDimension               =  0x0040,
		GeometricMinDimension               =  0x0080,
		GeometricMaxDimensionAngle          =  0x0100,
		GeometricMinDimensionAngle          =  0x0200,

		GeometricCircumFerence              =  0x0400,
		GeometricSurface                    =  0x0800,

		// surface bounding box divided by surface
		GeometricRoundness                  =  0x1000,

		reserved2                           =  0x2000,
		reserved3                           =  0x4000,
		reserved4                           =  0x8000,
		// or-ed combination of all detailed measures
		AllMeasures                         =  0x1ffc

		};
	GeometricMeasurement();
	virtual ~GeometricMeasurement();
	/*!
	 * overwrite and handle the specific measures as provided by the
	 * geometric object specific available measures
	 */
	virtual QVariant measurementValue(MeasurementType m) const ;
	/*!
	 * overwrite if extension is required for extended MeasurementType(s)
	 * specifically if the name gets a slightly modified meaning according
	 * to the context of the geometric object
	 *
	 * the name might be subject to translation whereby the canonic name
	 * is fixed for xml persistence and arbitrary handling purposes
	 */
    virtual QString measurementName(MeasurementType m) const ;
    virtual QString canonicName(MeasurementType m) const ;

};

Q_DECLARE_METATYPE(GeometricMeasurement*)


class IntensityMeasurement : public MeasurementOnObject {
	Q_OBJECT
public:
	enum
	 {
		// begin - same for all measurement on objects
		// to safely handle exceptions explicitly w/o exception
		Undefined                           =  0x0000,
		// reference to the group/set or measurements of the object itself
		Self                                =  0x0001,
		// boolean identifier if the there is any graphics, plot available
		Graphic                             =  0x0002,
		// end  - same for all measurement on objects

		IntensityMean                       =  0x0004,
		IntensityVariance                   =  0x0008,
		IntensitySlope                      =  0x0010,

		IntensityMin                        =  0x0020,
		IntensityMax                        =  0x0040,
		IntensityMedian                     =  0x0080,
		// array of doubles for min, 1st quartile median, 3rd quartile, max
		IntensityQuartiles                  =  0x0100,

		// intensity weighted midpoint of the object
		IntensityCenterOfGravity            =  0x0200,

		// fit a linear function - delivers fix, slope and quality of fit
		IntensityLinearFit                  =  0x0400,


		// or-ed combination of all detailed measures
		AllMeasures                         =  0x07fe,
	};
	IntensityMeasurement();
	virtual ~IntensityMeasurement();
	/*!
	 * overwrite and handle the specific measures as provided by the
	 * geometric object specific available measures
	 */
	virtual QVariant measurementValue(MeasurementType m) const ;
	/*!
	 * overwrite if extension is required for extended MeasurementType(s)
	 * specifically if the name gets a slightly modified meaning according
	 * to the context of the geometric object
	 *
	 * the name might be subject to translation whereby the canonic name
	 * is fixed for xml persistence and arbitrary handling purposes
	 */
    virtual QString measurementName(MeasurementType m) const ;
    virtual QString canonicName(MeasurementType m) const ;
	QVector<QPointF>* histogram() const { return histogram_; }
	QVector<QPointF>* profile()   const { return profile_; }
	/*!
	 * @description: Interface to compute the histogram of a specific object
	 *               The result will be written to the local variable
	 *               histogram_ which is a pointer to the data structure;
	 *               if the pointer is zero, the data structure has to be
	 *               created first.
	 *               If no histogram is available the pointer is set to zero.
	 */
	virtual void computeHistogram() { histogram_ = 0; }
	/*!
	 * @description: Interface to compute the profile of a specific object
	 *               The result will be written to the local variable
	 *               histogram_ which is a pointer to the data structure;
	 *               if the pointer is zero, the data structure has to be
	 *               created first.
	 *               If no profile is available the pointer is set to zero.
	 */
	virtual void computeProfile() { profile_ = 0; }
public slots:
	virtual void updateMeasures();
protected:
	QVector<QPointF>* histogram_;
	QVector<QPointF>* profile_;
	QVector<QPointF>* histogram_alternate_;
	QVector<QPointF>* profile_alternate_;
private:
	HistogramStatistics* determineDistribution(MeasurementType m) const;
};


Q_DECLARE_METATYPE(IntensityMeasurement*)

/**
 * @description: deliver the name of the attached group:
 * the measurement types are here or-able but dynamically generated
 * following the 2^n fuction whereby n the n-th element in the list is
 */
class CompositeMeasurement : public MeasurementOnObject {
	Q_OBJECT
public:
	enum
	 {
		// begin - same for all measurement on objects
		// to safely handle exceptions explicitly w/o exception
		Undefined                           =  0x0000,
		// reference to the group/set or measurements of the object itself
		Self                                =  0x0001,
		// boolean identifier if the there is any graphics, plot available
		Graphic                             =  0x0002,
		// end  - same for all measurement on objects

		AllMeasures                         =  0x0000
	};
	CompositeMeasurement();
	virtual ~CompositeMeasurement();
	virtual void init(GeometricObject* g = 0);
	/*!
	 * overwrite and handle the specific measures as provided by the
	 * geometric object specific available measures
	 */
	virtual QVariant measurementValue(MeasurementType m) const ;
	/*!
	 * overwrite if extension is required for extended MeasurementType(s)
	 * specifically if the name gets a slightly modified meaning according
	 * to the context of the geometric object
	 *
	 * the name might be subject to translation whereby the canonic name
	 * is fixed for xml persistence and arbitrary handling purposes
	 */
    virtual QString measurementName(MeasurementType m) const ;
    virtual QString canonicName(MeasurementType m) const ;
    /**
     * @description: add a measurement to the already available measurements
     * 				 takes care for deletion later on
     * @parameter m: pointer to an abstract MeasurementOnObject object
     */
    void addMeasurement(MeasurementOnObject* m);
public slots:
	virtual void updateMeasures();

private:
    QList<MeasurementOnObject*> measurements_;

};

Q_DECLARE_METATYPE(CompositeMeasurement*)

#endif /* MEASUREMENT_OBJECT_H_ */
