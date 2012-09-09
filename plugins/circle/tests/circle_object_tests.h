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

#ifndef CIRCLE_OBJECT_TESTS_H_
#define CIRCLE_OBJECT_TESTS_H_



#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "object_base.h"
#include "object_plugin.h"
#include "../src/circle.h"

using namespace std;

class ObjectTestReader;

class CircleObjectTests : public CPPUNIT_NS :: TestFixture {
public:
	CPPUNIT_TEST_SUITE (CircleObjectTests);

	CPPUNIT_TEST (geometricModel2Point);
	CPPUNIT_TEST (geometricModel3Point);

	CPPUNIT_TEST (zeroPointHistogram);
	CPPUNIT_TEST (onePointHistogram);
	CPPUNIT_TEST (toTheEdgeHistogram);
	CPPUNIT_TEST (radius3Histogram);

	CPPUNIT_TEST_SUITE_END ();

public:
	CircleObjectTests();
	~CircleObjectTests();
	void setUp (void);
	void tearDown (void);

protected:
	// test on the model definition (aka factory)
	void geometricModel2Point(void);
	void geometricModel3Point(void);

	// tests on geometric characteristics

	// tests on image intensities
	void  zeroPointHistogram (void);
	void  onePointHistogram (void);
	void  toTheEdgeHistogram (void);
	void  radius3Histogram(void);


private:
	void getTheOnlyObject();

	ObjectInterface* interf_;
	GeometricObjectFactory* sut_factory_2points_;
	GeometricObjectFactory* sut_factory_3points_;
	GeometricObjectContainer* object_container_;
	GraphicsScene* graphics_scene_;
	GeometricCircle* circle_;

	//QApplication* the_app_;
	QWidget* the_widget_;
};

#endif /* CIRCLE_OBJECT_TESTS_H_ */
