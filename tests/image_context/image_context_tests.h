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

#ifndef IMAGE_CONTEXT_TESTS_H_
#define IMAGE_CONTEXT_TESTS_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;


class ImageContextTests : public CPPUNIT_NS :: TestFixture {
public:
	CPPUNIT_TEST_SUITE (ImageContextTests);

	CPPUNIT_TEST (test_01_useImageFileName);
	CPPUNIT_TEST (test_02_useImageFileName);

	CPPUNIT_TEST (test_01_doNotUpdateObjectFileNames);

	CPPUNIT_TEST (test_01_useTemplateDefinition);
	CPPUNIT_TEST (test_02_useTemplateDefinition);
	CPPUNIT_TEST (test_03_useTemplateDefinition);

	CPPUNIT_TEST_SUITE_END ();

public:
	void setUp (void);
	void tearDown (void);

protected:
	void test_01_useImageFileName();
	void test_02_useImageFileName();

	void test_01_doNotUpdateObjectFileNames();

	void test_01_useTemplateDefinition();
	void test_02_useTemplateDefinition();
	void test_03_useTemplateDefinition();
private:

};

#endif /* IMAGE_CONTEXT_TESTS_H_ */
