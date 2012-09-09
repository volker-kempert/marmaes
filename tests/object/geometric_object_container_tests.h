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

#ifndef GEOMETRIC_OBJECT_CONTAINER_TESTS_H_
#define GEOMETRIC_OBJECT_CONTAINER_TESTS_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


using namespace std;

class GeometricObjectContainerTests : public CPPUNIT_NS :: TestFixture
{
public:
	CPPUNIT_TEST_SUITE (GeometricObjectContainerTests);
	CPPUNIT_TEST (test_01_simpleCreateAndDelete);

	CPPUNIT_TEST (test_02_singletonCreateCreate);
	CPPUNIT_TEST (test_02_singletonCreateDeleteCreate);


	CPPUNIT_TEST (test_03_addOneObjectAndRemove);
	CPPUNIT_TEST (test_03_addManyObjectAndRemoveOne);
	CPPUNIT_TEST (test_03_addManyObjectAndRemoveAll);

	CPPUNIT_TEST (test_03_addManyObjectAndRemoveAnNoneExistingObject);
	CPPUNIT_TEST (test_03_removeAnNoneExistingObject);

	CPPUNIT_TEST (test_03_AccessASpecificObjectById);

	CPPUNIT_TEST_SUITE_END ();

	public:
		void setUp (void);
		void tearDown (void);

	protected:
		void test_01_simpleCreateAndDelete (void);

		void test_02_singletonCreateCreate (void);
		void test_02_singletonCreateDeleteCreate (void);

		void test_03_addOneObjectAndRemove (void);
		void test_03_addManyObjectAndRemoveOne (void);
		void test_03_addManyObjectAndRemoveAll (void);

		void test_03_addManyObjectAndRemoveAnNoneExistingObject (void);
		void test_03_removeAnNoneExistingObject (void);

		void test_03_AccessASpecificObjectById (void);

	private:

};

#endif
