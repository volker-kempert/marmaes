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

#ifndef GEOMETRIC_MEASUREMENT_TESTS_H_
#define GEOMETRIC_MEASUREMENT_TESTS_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "object_base.h"
#include "measurement_on_object.h"

using namespace std;

class GeometricMeasurementTests : public CPPUNIT_NS :: TestFixture
{
public:
	CPPUNIT_TEST_SUITE (GeometricMeasurementTests);
	CPPUNIT_TEST (test_01_canonicName_Undefined);
	CPPUNIT_TEST (test_02_canonicName_Unspecific);
	CPPUNIT_TEST (test_03_canonicName_SelfAndGraphic);
	CPPUNIT_TEST (test_04_canonicName);
	CPPUNIT_TEST (test_05_type_Undefined);
	CPPUNIT_TEST (test_06_type_SelfAndGraphic);
	CPPUNIT_TEST (test_07_type);
	CPPUNIT_TEST (test_08_measurementValue);
	CPPUNIT_TEST_SUITE_END ();

	public:
		void setUp (void);
		void tearDown (void);

	protected:
		void test_01_canonicName_Undefined (void);
		void test_02_canonicName_Unspecific (void);
		void test_03_canonicName_SelfAndGraphic (void);
		void test_04_canonicName (void);

		void test_05_type_Undefined (void);
		void test_06_type_SelfAndGraphic (void);
		void test_07_type (void);

		void test_08_measurementValue (void);
	private:
		GeometricMeasurement* gm_;
};

#endif
