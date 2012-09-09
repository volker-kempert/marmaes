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

#ifndef LINE_OBJECT_TESTS_H_
#define LINE_OBJECT_TESTS_H_



#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "object_base.h"
#include "object_plugin.h"
#include "../src/line.h"

using namespace std;

class ObjectTestReader;

class LineObjectTests : public CPPUNIT_NS :: TestFixture {
public:
	CPPUNIT_TEST_SUITE (LineObjectTests);

	CPPUNIT_TEST (startEnd);


	CPPUNIT_TEST (horizontalProfile);
	CPPUNIT_TEST (verticalProfile);
	CPPUNIT_TEST (onePointProfile);
	CPPUNIT_TEST (twoPointProfile);
	CPPUNIT_TEST (diagonalProfile);
	CPPUNIT_TEST (lowReverseSlopeProfile);

	CPPUNIT_TEST_SUITE_END ();

public:
	LineObjectTests();
	~LineObjectTests();
	void setUp (void);
	void tearDown (void);

protected:
	// tests on geometric characteristics
	void startEnd(void);

	// tests on image intensities
	void  horizontalProfile (void);
	void  verticalProfile (void);
	void  onePointProfile (void);
	void  twoPointProfile(void);
	void  diagonalProfile (void);
	void  lowReverseSlopeProfile (void);


private:
	void getTheOnlyObject();

	ObjectInterface* interf_;
	GeometricObjectFactory* sut_factory_;
	GeometricObjectContainer* object_container_;
	GraphicsScene* graphics_scene_;
	GeometricLine* line_;

	//QApplication* the_app_;
	QWidget* the_widget_;
};

#endif /* LINE_OBJECT_TESTS_H_ */
