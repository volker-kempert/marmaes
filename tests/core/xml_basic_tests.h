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

#ifndef XML_BASIC_TESTS_H_
#define XML_BASIC_TESTS_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;


class XMLBasicTests : public CPPUNIT_NS :: TestFixture {
public:
	CPPUNIT_TEST_SUITE (XMLBasicTests);

	CPPUNIT_TEST (readFloat);
	CPPUNIT_TEST (readString);
	CPPUNIT_TEST (readInt);
	CPPUNIT_TEST (readBool_1);
	CPPUNIT_TEST (readBool_2);
	CPPUNIT_TEST (readBoolFlag_1);
	CPPUNIT_TEST (readBoolFlag_2);

	CPPUNIT_TEST (readFloatFail);
	CPPUNIT_TEST (readIntFail);
	CPPUNIT_TEST (readBoolFail);

	CPPUNIT_TEST (readPoint1);
	CPPUNIT_TEST (readPoint2);
	CPPUNIT_TEST (readPoint3);
	CPPUNIT_TEST (readPointFail);
	CPPUNIT_TEST (readRect1);
	CPPUNIT_TEST (readRect2);
	CPPUNIT_TEST (readRectFail1);
	CPPUNIT_TEST (readRectFail2);

	CPPUNIT_TEST (writeFloat);
	CPPUNIT_TEST (writeString);
	CPPUNIT_TEST (writeInt);
	CPPUNIT_TEST (writeBool);
	CPPUNIT_TEST (writePoint);
	CPPUNIT_TEST (writeRect);

	CPPUNIT_TEST_SUITE_END ();

public:
	void setUp (void);
	void tearDown (void);

protected:
	void readFloat (void);
	void readInt (void);
	void readString (void);
	void readBool_1 (void);
	void readBool_2 (void);
	void readBoolFlag_1 (void);
	void readBoolFlag_2 (void);

	void readFloatFail (void);
	void readIntFail (void);
	void readBoolFail (void);

	void readPoint1 (void);
	void readPoint2 (void);
	void readPoint3 (void);
	void readPointFail (void);
	void readRect1 (void);
	void readRect2 (void);
	void readRectFail1 (void);
	void readRectFail2 (void);

	void writeFloat (void);
	void writeInt (void);
	void writeString (void);
	void writeBool (void);
	void writePoint (void);
	void writeRect (void);

private:

};

#endif /* XML_BASIC_TESTS_H_ */
