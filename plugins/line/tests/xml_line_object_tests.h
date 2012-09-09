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

#ifndef XML_OBJECT_TESTS_H_
#define XML_OBJECT_TESTS_H_



#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "object_base_read_write.h"
#include "object_plugin.h"

using namespace std;

class ObjectTestReader;

class XMLLineObjectTests : public CPPUNIT_NS :: TestFixture {
public:
	CPPUNIT_TEST_SUITE (XMLLineObjectTests);

	CPPUNIT_TEST (readLine);
	CPPUNIT_TEST (readLineModel);
	CPPUNIT_TEST (readLineModel2);
	CPPUNIT_TEST (readLineModelFail);
	CPPUNIT_TEST (readLineModelFail2);
	CPPUNIT_TEST (readLineModelFail3);
	CPPUNIT_TEST (readLineModelFail4);
	CPPUNIT_TEST (readLineModelFail5);
	CPPUNIT_TEST (readLineModelFail6);
	CPPUNIT_TEST (readLineModelFail7);
	CPPUNIT_TEST (writeLine);

	CPPUNIT_TEST_SUITE_END ();

public:
	void setUp (void);
	void tearDown (void);

protected:
	void  writeLine (void);
	void  readLineModel (void);
	void  readLineModel2 (void);
	void  readLineModelFail (void);
	void  readLineModelFail2 (void);
	void  readLineModelFail3 (void);
	void  readLineModelFail4 (void);
	void  readLineModelFail5 (void);
	void  readLineModelFail6 (void);
	void  readLineModelFail7 (void);
	void  readLine      (void);

private:
	ObjectInterface* interf_;
	ObjectTestReader* sut_;
};

#endif /* XML_OBJECT_TESTS_H_ */
