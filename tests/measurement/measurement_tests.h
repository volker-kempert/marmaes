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

#ifndef MEASUREMENT_TESTS_H_
#define MEASUREMENT_TESTS_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "measurement_on_object.h"

using namespace std;



class MeasurementTests : public CPPUNIT_NS :: TestFixture {
public:
	CPPUNIT_TEST_SUITE (MeasurementTests);

	CPPUNIT_TEST (profileMean);
	CPPUNIT_TEST (profileVariance);
	CPPUNIT_TEST (profileSlope);
	CPPUNIT_TEST (profileMax);
	CPPUNIT_TEST (profileMin);
	CPPUNIT_TEST (profileMedian);
	CPPUNIT_TEST (profileQuartiles);

	CPPUNIT_TEST (histogramMean);
	CPPUNIT_TEST (histogramVariance);
	CPPUNIT_TEST (histogramSlope);
	CPPUNIT_TEST (histogramMax);
	CPPUNIT_TEST (histogramMin);
	CPPUNIT_TEST (histogramMedian);
	CPPUNIT_TEST (histogramQuartiles);

	CPPUNIT_TEST_SUITE_END ();

public:
	void setUp (void);
	void tearDown (void);

protected:
	void  profileMean (void);
	void  profileVariance (void);
	void  profileSlope(void);
	void  profileMax(void);
	void  profileMin(void);
	void  profileMedian(void);
	void  profileQuartiles(void);

	void  histogramMean (void);
	void  histogramVariance (void);
	void  histogramSlope(void);
	void  histogramMax(void);
	void  histogramMin(void);
	void  histogramMedian(void);
	void  histogramQuartiles (void);

private:

};


#endif /* XML_MEASUREMENT_TESTS_H_ */
