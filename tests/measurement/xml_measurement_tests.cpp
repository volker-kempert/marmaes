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

#include "xml_measurement_tests.h"


CPPUNIT_TEST_SUITE_REGISTRATION (XMLMeasurementTests);


class GeometricMeasruementMock : public GeometricMeasurement {
public:
	GeometricMeasruementMock() : GeometricMeasurement() {
		available_measures_ =
				  GeometricMeasurement::GeometricMaxDimensionAngle
				| GeometricMeasurement::GeometricRoundness
				| GeometricMeasurement::GeometricBoundingBox
				| GeometricMeasurement::GeometricMaxDimension
				| GeometricMeasurement::GeometricMidpoint
				;
		actual_measures_ = GeometricMeasurement::Undefined ;
	}
	virtual ~GeometricMeasruementMock() {}
};

class  GeometricMeasurementMockReader: public ObjectBaseReader {
public:
	// we risk memory leaks here in order to safe development time
	GeometricMeasurementMockReader() : ObjectBaseReader() {}
	~GeometricMeasurementMockReader() { delete measurement_; delete object_; };
	bool checkToken(const QXmlStreamReaderWhitespace& reader) { return true; }
	MeasurementOnObject* measurement() { return measurement_; }
protected:
	void initObjects() {
		measurement_ = (GeometricMeasurement*) new GeometricMeasruementMock();
		object_ = (GeometricObject*) new GeometricObject();
	}
	void finalize() {};
	virtual bool readGeometricObject(QXmlStreamReaderWhitespace& reader) {
		QString tmp = reader.readElementText();
		reader.readNext();
	}
};

//-----------------------------------------------------------------------------


class IntensityMeasruementMock : public IntensityMeasurement {
public:
	IntensityMeasruementMock() : IntensityMeasurement() {
		available_measures_ =
				  IntensityMeasurement::IntensityMedian
				| IntensityMeasurement::IntensityMean
				| IntensityMeasurement::IntensityMin
				| IntensityMeasurement::IntensityMax
				| IntensityMeasurement::IntensityVariance
				;
		actual_measures_ = IntensityMeasurement::Undefined ;
	}
	virtual ~IntensityMeasruementMock() {}
};

class  IntensityMeasurementMockReader: public ObjectBaseReader {
public:
	// we risk memory leaks here in order to safe development time
	IntensityMeasurementMockReader() : ObjectBaseReader() {}
	~IntensityMeasurementMockReader() { delete measurement_; delete object_; };
	bool checkToken(const QXmlStreamReaderWhitespace& reader) { return true; }
	MeasurementOnObject* measurement() { return measurement_; }
protected:
	void initObjects() {
		measurement_ = (IntensityMeasurement*) new IntensityMeasruementMock();
		object_ = (GeometricObject*) new GeometricObject();
	}
	void finalize() {};
	virtual bool readGeometricObject(QXmlStreamReaderWhitespace& reader) {
		QString tmp = reader.readElementText();
		reader.readNext();
	}
};


//-----------------------------------------------------------------------------


class CompositeMeasruementMock : public CompositeMeasurement {
public:
	CompositeMeasruementMock() : CompositeMeasurement() {
		available_measures_ = CompositeMeasurement::Undefined ;
		actual_measures_ = CompositeMeasurement::Undefined ;
	}
	virtual ~CompositeMeasruementMock() {}
};

class  CompositeMeasurementMockReader: public ObjectBaseReader {
public:
	// we risk memory leaks here in order to safe development time
	CompositeMeasurementMockReader() : ObjectBaseReader() {}
	~CompositeMeasurementMockReader() { delete measurement_; delete object_; };
	bool checkToken(const QXmlStreamReaderWhitespace& reader) { return true; }
	MeasurementOnObject* measurement() { return measurement_; }
protected:
	void initObjects() {
		CompositeMeasruementMock* cm = new CompositeMeasruementMock();
		GeometricMeasruementMock* gm = new GeometricMeasruementMock();
		IntensityMeasruementMock* im = new IntensityMeasruementMock();
		cm->addMeasurement(gm);
		cm->addMeasurement(im);
		measurement_ =  (MeasurementOnObject*) cm;
		object_ = (GeometricObject*) new GeometricObject();
	}
	void finalize() {};
	virtual bool readGeometricObject(QXmlStreamReaderWhitespace& reader) {
		QString tmp = reader.readElementText();
		reader.readNext();
	}
};


//-----------------------------------------------------------------------------


void XMLMeasurementTests::setUp() {
	geometric_sut_ = 0;
}

void XMLMeasurementTests::tearDown() {
	// it crashes so rather deal with a memory leak
	// if ( geometric_sut_ != 0 ) delete sut_;
}



void XMLMeasurementTests::readGeometricMeasurement() {
	qDebug() << "**XMLMeasurementTests::readGeometricMeasurement**" ;
	geometric_sut_ = new GeometricMeasurementMockReader();
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<object id=\"123\">"
			"  <model> "
			"  </model>"
			"  <geometric_measurements>"
			"    <max_dim type=\"integer\"> 123 </max_dim>"
			"    <roundness type=\"integer\"> 123 </roundness>"
			"    <midpoint type=\"integer\"> 123 </midpoint>"
			"  </geometric_measurements>"
			"</object> "
			);
	reader.readNext();
	reader.readNext();
	CPPUNIT_ASSERT ( geometric_sut_->checkToken(reader));
	CPPUNIT_ASSERT ( geometric_sut_->read(reader));
	GeometricMeasurement* m = (GeometricMeasurement*)geometric_sut_->measurement();

	CPPUNIT_ASSERT ( m != 0 );
	MeasurementType t = m->actualMeasures();
	CPPUNIT_ASSERT ( t & GeometricMeasurement::GeometricMaxDimension );
	CPPUNIT_ASSERT ( t & GeometricMeasurement::GeometricRoundness );
	CPPUNIT_ASSERT ( t & GeometricMeasurement::GeometricMidpoint );
	CPPUNIT_ASSERT ( ~ t |  ~GeometricMeasurement::GeometricYMaxDimension );
}

void XMLMeasurementTests::readGeometricMeasurementNotAvailable() {
	qDebug() << "**XMLMeasurementTests::readGeometricMeasurementNotAvailable**" ;
	geometric_sut_ = new GeometricMeasurementMockReader();
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<object id=\"123\">"
			"  <model> "
			"  </model>"
			"  <geometric_measurements>"
			"    <max_dim type=\"integer\"> 123 </max_dim>"
			"    <variance type=\"integer\"> 123 </variance>"
			"  </geometric_measurements>"
			"</object>"
			);
	reader.readNext();
	reader.readNext();
	// variance is not allowed for geometric measurement mock
	CPPUNIT_ASSERT ( ! geometric_sut_->read(reader));
}

//-----------------------------------------------------------------------------

void XMLMeasurementTests::readIntensityMeasurement() {
	qDebug() << "**XMLMeasurementTests::readIntensityMeasurement**" ;
	intensity_sut_ = new IntensityMeasurementMockReader();
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<object id=\"123\">"
			"  <model> "
			"  </model>"
			"  <intensity_measurements>"
			"    <graphics>    </graphics>"
			"    <mean     type=\"double\"> 5.7   </mean>"
			"    <median   type=\"double\"> 123.2 </median>"
			"    <minimum  type=\"double\"> 1.1   </minimum>"
			"    <maximum  type=\"double\"> 2.2   </maximum>"
			"    <variance type=\"double\"> 3.3   </variance>"
			"  </intensity_measurements>"
			"</object> "
			);
	reader.readNext();
	reader.readNext();
	CPPUNIT_ASSERT ( intensity_sut_->checkToken(reader));
	CPPUNIT_ASSERT ( intensity_sut_->read(reader));
	IntensityMeasurement* m = (IntensityMeasurement*)intensity_sut_->measurement();

	CPPUNIT_ASSERT ( m != 0 );
	MeasurementType t = m->actualMeasures();
	CPPUNIT_ASSERT ( t & IntensityMeasurement::Graphic );
	CPPUNIT_ASSERT ( t & IntensityMeasurement::IntensityMean );
	CPPUNIT_ASSERT ( t & IntensityMeasurement::IntensityMedian );
	CPPUNIT_ASSERT ( t & IntensityMeasurement::IntensityMax );
	CPPUNIT_ASSERT ( t & IntensityMeasurement::IntensityMin );
	CPPUNIT_ASSERT ( ~ t |  ~IntensityMeasurement::IntensitySlope );
}

void XMLMeasurementTests::readIntensityMeasurementNotAvailable() {
	qDebug() << "**XMLMeasurementTests::readIntensityMeasurementNotAvailable**" ;
	intensity_sut_ = new IntensityMeasurementMockReader();
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<object id=\"123\">"
			"  <model> "
			"  </model>"
			"  <intensity_measurements>"
			"    <max_dim type=\"integer\"> 123 </max_dim>"
			"    <variance type=\"integer\"> 123 </variance>"
			"  </intensity_measurements>"
			"</object>"
			);
	reader.readNext();
	reader.readNext();
	// max_dim is not allowed for intensity mock
	CPPUNIT_ASSERT ( ! intensity_sut_->read(reader));
}

//-----------------------------------------------------------------------------


void XMLMeasurementTests::readCompositeMeasurement_1() {
	qDebug() << "**XMLMeasurementTests::readCompositeMeasurement**" ;
	composite_sut_ = new CompositeMeasurementMockReader();
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<object id=\"123\">"
			"  <model> "
			"  </model>"
			"  <measurements>"
			"    <geometric_measurements>"
			"      <max_dim type=\"integer\"> 123 </max_dim>"
			"    </geometric_measurements>"
			"    <intensity_measurements>"
			"      <mean     type=\"double\"> 5.7   </mean>"
			"    </intensity_measurements>"
			"  </measurements>"
			"</object> "
			);
	reader.readNext();
	reader.readNext();
	CPPUNIT_ASSERT ( composite_sut_->checkToken(reader));
	CPPUNIT_ASSERT ( composite_sut_->read(reader));
	CompositeMeasurement* m = (CompositeMeasurement*)composite_sut_->measurement();

	CPPUNIT_ASSERT ( m != 0 );
	MeasurementType t = m->actualMeasures();
	CPPUNIT_ASSERT ( m->availableMeasures() == t   );

	QVariant v = m->measurementValue(4);
	CPPUNIT_ASSERT ( v.canConvert<GeometricMeasurement*>() );
	GeometricMeasurement* gm = v.value<GeometricMeasurement*>();
	t = gm->actualMeasures();
	CPPUNIT_ASSERT ( t & GeometricMeasurement::GeometricMaxDimension );

	v = m->measurementValue(8);
	CPPUNIT_ASSERT ( v.canConvert<IntensityMeasurement*>() );
	IntensityMeasurement* im = v.value<IntensityMeasurement*>();
	t = im->actualMeasures();
	CPPUNIT_ASSERT ( t & IntensityMeasurement::IntensityMean );

}



void XMLMeasurementTests::readCompositeMeasurement_2() {
	qDebug() << "**XMLMeasurementTests::readCompositeMeasurement**" ;
	composite_sut_ = new CompositeMeasurementMockReader();
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<object id=\"123\">"
			"  <model> "
			"  </model>"
			"  <measurements>"
			"    <geometric_measurements>"
			"      <max_dim type=\"integer\"> 123 </max_dim>"
			"    </geometric_measurements>"
			"    <intensity_measurements>"
			"    </intensity_measurements>"
			"  </measurements>"
			"</object> "
			);
	reader.readNext();
	reader.readNext();
	CPPUNIT_ASSERT ( composite_sut_->checkToken(reader));
	CPPUNIT_ASSERT ( composite_sut_->read(reader));
	CompositeMeasurement* m = (CompositeMeasurement*)composite_sut_->measurement();

	CPPUNIT_ASSERT ( m != 0 );
	MeasurementType t = m->actualMeasures();
	CPPUNIT_ASSERT ( 4 == t   );

	QVariant v = m->measurementValue(4);
	CPPUNIT_ASSERT ( v.canConvert<GeometricMeasurement*>() );
	GeometricMeasurement* gm = v.value<GeometricMeasurement*>();
	t = gm->actualMeasures();
	CPPUNIT_ASSERT ( t & GeometricMeasurement::GeometricMaxDimension );


}


void XMLMeasurementTests::readCompositeMeasurement_3() {
	qDebug() << "**XMLMeasurementTests::readCompositeMeasurement**" ;
	composite_sut_ = new CompositeMeasurementMockReader();
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<object id=\"123\">"
			"  <model> "
			"  </model>"
			"  <measurements>"
			"    <intensity_measurements>"
			"      <mean     type=\"double\"> 5.7   </mean>"
			"    </intensity_measurements>"
			"  </measurements>"
			"</object> "
			);
	reader.readNext();
	reader.readNext();
	CPPUNIT_ASSERT ( composite_sut_->checkToken(reader));
	CPPUNIT_ASSERT ( composite_sut_->read(reader));
	CompositeMeasurement* m = (CompositeMeasurement*)composite_sut_->measurement();

	CPPUNIT_ASSERT ( m != 0 );
	MeasurementType t = m->actualMeasures();
	CPPUNIT_ASSERT ( 8 == t   );

	QVariant v = m->measurementValue(8);
	CPPUNIT_ASSERT ( v.canConvert<IntensityMeasurement*>() );
	IntensityMeasurement* im = v.value<IntensityMeasurement*>();
	t = im->actualMeasures();
	CPPUNIT_ASSERT ( t & IntensityMeasurement::IntensityMean );

}


void XMLMeasurementTests::readCompositeMeasurementNotAvailable() {
	qDebug() << "**XMLMeasurementTests::readCompositeMeasurementNotAvailable**" ;
	composite_sut_ = new CompositeMeasurementMockReader();
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<object id=\"123\">"
			"  <model> "
			"  </model>"
			"  <measurements>"
			"    <profile_measurements>"
			"      <mean     type=\"double\"> 5.7   </mean>"
			"    </profile_measurements>"
			"  </measurements>"
			"</object>"
			);
	reader.readNext();
	reader.readNext();
	// max_dim is not allowed for composite mock
	CPPUNIT_ASSERT ( ! composite_sut_->read(reader));
}

//-----------------------------------------------------------------------------


void XMLMeasurementTests::writeMeasure() {
	CPPUNIT_ASSERT ( true);
}


