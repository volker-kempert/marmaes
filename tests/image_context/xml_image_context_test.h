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

#ifndef XML_IMAGE_CONTEXT_TEST_H_
#define XML_IMAGE_CONTEXT_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "marmeas_core/image_context/image_context_read_write.h"

using namespace std;

class XmlImageContextTests : public CPPUNIT_NS :: TestFixture
{
public:
	CPPUNIT_TEST_SUITE (XmlImageContextTests);
	CPPUNIT_TEST (writeImageContext);
	CPPUNIT_TEST (readImageContextOK);
	CPPUNIT_TEST (readImageContextTokenMisMatch);
	CPPUNIT_TEST (readImageContextIncomplete);
	CPPUNIT_TEST (readImageContextNumberConversionError);
	CPPUNIT_TEST (readImageContextEmptyParameterError);
	CPPUNIT_TEST (readImageContextNoEnd);
	CPPUNIT_TEST (readImageContextWrongEnd);
	CPPUNIT_TEST_SUITE_END ();

	public:
		void setUp (void);
		void tearDown (void);

	protected:
		void writeImageContext (void);

		void readImageContextOK (void);
		void readImageContextTokenMisMatch (void);
		void readImageContextIncomplete (void);
		void readImageContextNumberConversionError (void);
		void readImageContextEmptyParameterError(void);
		void readImageContextNoEnd (void);
		void readImageContextWrongEnd (void);
	private:
		ImageContextReader*  icr_;
};

#endif /* XML_IMAGE_CONTEXT_TEST_H_ */
