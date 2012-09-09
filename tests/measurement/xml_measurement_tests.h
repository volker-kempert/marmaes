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

#ifndef XML_MEASUREMENT_TESTS_H_
#define XML_MEASUREMENT_TESTS_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "object_base_read_write.h"

using namespace std;

class GeometricMeasurementMockReader;
class IntensityMeasurementMockReader;
class CompositeMeasurementMockReader;


class XMLMeasurementTests : public CPPUNIT_NS :: TestFixture {
public:
	CPPUNIT_TEST_SUITE (XMLMeasurementTests);

	CPPUNIT_TEST (readGeometricMeasurement);
	CPPUNIT_TEST (readGeometricMeasurementNotAvailable);

	CPPUNIT_TEST (readIntensityMeasurement);
	CPPUNIT_TEST (readIntensityMeasurementNotAvailable);

	CPPUNIT_TEST (readCompositeMeasurement_1);
	CPPUNIT_TEST (readCompositeMeasurement_2);
	CPPUNIT_TEST (readCompositeMeasurement_3);
	CPPUNIT_TEST (readCompositeMeasurementNotAvailable);

	CPPUNIT_TEST (writeMeasure);


	CPPUNIT_TEST_SUITE_END ();

public:
	void setUp (void);
	void tearDown (void);

protected:
	void  readGeometricMeasurement (void);
	void  readGeometricMeasurementNotAvailable(void);

	void  readIntensityMeasurement (void);
	void  readIntensityMeasurementNotAvailable(void);

	void  readCompositeMeasurement_1 (void);
	void  readCompositeMeasurement_2 (void);
	void  readCompositeMeasurement_3 (void);
	void  readCompositeMeasurementNotAvailable(void);

	void  writeMeasure (void);
private:
	GeometricMeasurementMockReader* geometric_sut_;
	IntensityMeasurementMockReader* intensity_sut_;
	CompositeMeasurementMockReader* composite_sut_;
};


#endif /* XML_MEASUREMENT_TESTS_H_ */
