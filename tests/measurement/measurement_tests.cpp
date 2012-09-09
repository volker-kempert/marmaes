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


#include <QVariant>
#include <QXmlStreamReader>
#include <QDebug>
#include <cmath>

#include "measurement_tests.h"
#include "measurement_on_object.h"
#include "marmeas_core/measurement/quartile.h"

CPPUNIT_TEST_SUITE_REGISTRATION (MeasurementTests);

static QVector<QPointF> DATA_HISTOGRAM(5, QPointF( 0.0, 0.0 ));
static QVector<QPointF> DATA_PROFILE(6, QPointF( 0.0, 0.0 ));


class MockProfileMeasurement : public IntensityMeasurement {
public:
	void computeProfile() { profile_ = &DATA_PROFILE; }
	~MockProfileMeasurement() {
		profile_ = 0;  // prevent implicit free of data pointers
		histogram_ = 0; // as of the parent class
	}

};

class MockHistogramMeasurement : public IntensityMeasurement {
public:
	void computeHistogram() { histogram_ = &DATA_HISTOGRAM; }
	~MockHistogramMeasurement() {
		profile_ = 0;  // prevent implicit free of data pointers
		histogram_ = 0; // as of the parent class
	}

};

void MeasurementTests::setUp() {
	if ( DATA_HISTOGRAM.at(0).x() == 0.0) {
		QPointF p1(1.0, 1.0);
		QPointF p2(2.0, 2.0);
		QPointF p3(3.0, 3.0);
		QPointF p4(4.0, 4.0);
		QPointF p5(5.0, 5.0);

		DATA_HISTOGRAM.replace(0,p1);
		DATA_HISTOGRAM.replace(1,p2);
		DATA_HISTOGRAM.replace(2,p3);
		DATA_HISTOGRAM.replace(3,p4);
		DATA_HISTOGRAM.replace(4,p5);

		DATA_PROFILE.replace(5,p1);
		DATA_PROFILE.replace(4,p1);
		DATA_PROFILE.replace(3,p1);
		DATA_PROFILE.replace(2,p2);
		DATA_PROFILE.replace(1,p2);
		DATA_PROFILE.replace(0,p3);

	}
}

void MeasurementTests::tearDown() {
}

//-----------------------------------------------------------------------------

void  MeasurementTests::profileMean (void) {
	qDebug() << "**MeasurementTests::profileMean**" ;
	MockProfileMeasurement measure;
	measure.computeProfile();
	QVariant value;
	bool convert_ok;
	qDebug() << "**MeasurementTests::profileMean** after assign" ;
	value = measure.measurementValue(IntensityMeasurement::IntensityMean);
	qDebug() << "**MeasurementTests::profileMean** value" << value ;
	CPPUNIT_ASSERT(abs( value.toDouble(&convert_ok) - 1.66666) < 0.01 );
	CPPUNIT_ASSERT(convert_ok);
}

void  MeasurementTests::profileVariance (void) {
	qDebug() << "**MeasurementTests::profileVariance**" ;
	MockProfileMeasurement measure;
	measure.computeProfile();
	QVariant value;
	bool convert_ok;
	value = measure.measurementValue(IntensityMeasurement::IntensityVariance);
	CPPUNIT_ASSERT(abs( value.toDouble(&convert_ok) - 0.555555) < 0.01 );
	CPPUNIT_ASSERT(convert_ok);
}

void  MeasurementTests::profileSlope(void) {
	qDebug() << "**MeasurementTests::profileSlope**" ;
	MockProfileMeasurement measure;
	measure.computeProfile();
	QVariant value;
	bool convert_ok;
	value = measure.measurementValue(IntensityMeasurement::IntensitySlope);
	CPPUNIT_ASSERT(abs( value.toDouble(&convert_ok) - 0.259259) < 0.01 );
	CPPUNIT_ASSERT(convert_ok);
}

void  MeasurementTests::profileMax(void) {
	qDebug() << "**MeasurementTests::profileMax**" ;
	MockProfileMeasurement measure;
	measure.computeProfile();
	QVariant value;
	bool convert_ok;
	value = measure.measurementValue(IntensityMeasurement::IntensityMax);
	CPPUNIT_ASSERT(abs( value.toDouble(&convert_ok) - 3.0 ) < 0.01 );
	CPPUNIT_ASSERT(convert_ok);
}

void  MeasurementTests::profileMin(void) {
	qDebug() << "**MeasurementTests::profileMin**" ;
	MockProfileMeasurement measure;
	measure.computeProfile();
	QVariant value;
	bool convert_ok;
	value = measure.measurementValue(IntensityMeasurement::IntensityMin);
	CPPUNIT_ASSERT(abs( value.toDouble(&convert_ok) - 1.0 ) < 0.01 );
	CPPUNIT_ASSERT(convert_ok);
}


void  MeasurementTests::profileMedian (void) {
	qDebug() << "**MeasurementTests::profileMedian**" ;
	MockProfileMeasurement measure;
	measure.computeProfile();
	QVariant value;
	bool convert_ok;

	value = measure.measurementValue(IntensityMeasurement::IntensityMedian);
	qDebug() << "**MeasurementTests::profileMedian**" << value ;
	CPPUNIT_ASSERT(abs( value.toDouble(&convert_ok) - 1.0 ) < 0.01 );
	CPPUNIT_ASSERT(convert_ok);
}

void  MeasurementTests::profileQuartiles (void) {
	qDebug() << "**MeasurementTests::profileQuartiles**" ;
	MockProfileMeasurement measure;
	measure.computeProfile();
	QVariant value;
	bool convert_ok;
	value = measure.measurementValue(IntensityMeasurement::IntensityQuartiles);
	CPPUNIT_ASSERT(value.canConvert<Quartile>());
	Quartile q = value.value<Quartile>();
	CPPUNIT_ASSERT(abs( q.minimum() - 1.0l ) < 0.01l );
	CPPUNIT_ASSERT(abs( q.median()  - 1.0 ) < 0.01 );
	CPPUNIT_ASSERT(abs( q.third()   - 2.0 ) < 0.01 );
	CPPUNIT_ASSERT(abs( q.maximum() - 3.0 ) < 0.01 );

}

//-----------------------------------------------------------------------------



void  MeasurementTests::histogramMean (void) {
	qDebug() << "**MeasurementTests::histogramMean**" ;
	MockHistogramMeasurement measure;
	measure.computeHistogram();
	QVariant value;
	bool convert_ok;

	value = measure.measurementValue(IntensityMeasurement::IntensityMean);
	qDebug() << "**MeasurementTests::histogramMeanOnePoint** value" << value ;
	CPPUNIT_ASSERT(abs(value.toDouble(&convert_ok) - 3.6666666) < 0.00001);
	CPPUNIT_ASSERT(convert_ok);
}

void  MeasurementTests::histogramVariance (void) {
	qDebug() << "**MeasurementTests::histogramVariance**" ;
	MockHistogramMeasurement measure;
	measure.computeHistogram();
	QVariant value;
	bool convert_ok;
	value = measure.measurementValue(IntensityMeasurement::IntensityVariance);
	CPPUNIT_ASSERT(abs(value.toDouble(&convert_ok) - 1.555555) < 0.00001);
	CPPUNIT_ASSERT(convert_ok);
}

void  MeasurementTests::histogramSlope(void) {
	qDebug() << "**MeasurementTests::histogramSlope**" ;
	MockHistogramMeasurement measure;
	measure.computeHistogram();
	QVariant value;
	bool convert_ok;
	value = measure.measurementValue(IntensityMeasurement::IntensitySlope);
	CPPUNIT_ASSERT(abs(value.toDouble(&convert_ok) + 1.140740) < 0.00001);
	CPPUNIT_ASSERT(convert_ok);
}

void  MeasurementTests::histogramMax(void) {
	qDebug() << "**MeasurementTests::histogramMax**" ;
	MockHistogramMeasurement measure;
	measure.computeHistogram();
	QVariant value;
	bool convert_ok;
	value = measure.measurementValue(IntensityMeasurement::IntensityMax);
	CPPUNIT_ASSERT(value.toDouble(&convert_ok) == 5.0);
	CPPUNIT_ASSERT(convert_ok);
}

void  MeasurementTests::histogramMin(void) {
	qDebug() << "**MeasurementTests::histogramMin**" ;
	MockHistogramMeasurement measure;
	measure.computeHistogram();
	QVariant value;
	bool convert_ok;
	value = measure.measurementValue(IntensityMeasurement::IntensityMin);
	CPPUNIT_ASSERT(value.toDouble(&convert_ok) == 1.0);
	CPPUNIT_ASSERT(convert_ok);
}

void  MeasurementTests::histogramMedian(void) {
	MockHistogramMeasurement measure;
	measure.computeHistogram();
	QVariant value;
	bool convert_ok;
	value = measure.measurementValue(IntensityMeasurement::IntensityMedian);
	qDebug() << "**MeasurementTests::histogramMedian** " << value;
	CPPUNIT_ASSERT(value.toDouble(&convert_ok) == 4.0);
	CPPUNIT_ASSERT(convert_ok);
}

void  MeasurementTests::histogramQuartiles (void) {
	qDebug() << "**MeasurementTests::histogramQuartiles**" ;
	MockHistogramMeasurement measure;
	measure.computeHistogram();
	QVariant value;
	bool convert_ok;

	value = measure.measurementValue(IntensityMeasurement::IntensityQuartiles);
	CPPUNIT_ASSERT(value.canConvert<Quartile>());
	Quartile q = value.value<Quartile>();
	CPPUNIT_ASSERT(abs( q.minimum() - 1.0 ) < 0.01 );
	CPPUNIT_ASSERT(abs( q.first()   - 3.0 ) < 0.01 );
	CPPUNIT_ASSERT(abs( q.median()  - 4.0 ) < 0.01 );
	CPPUNIT_ASSERT(abs( q.third()   - 5.0 ) < 0.01 );
	CPPUNIT_ASSERT(abs( q.maximum() - 5.0 ) < 0.01 );
}
