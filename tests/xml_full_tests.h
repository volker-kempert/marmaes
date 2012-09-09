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

#ifndef XML_FULL_TESTS_H_
#define XML_FULL_TESTS_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


#include "object_base_read_write.h"
#include "marmeas_core/image_context/image_context_read_write.h"
#include "object_plugin.h"


class XmlStreamReaderMock;
using namespace std;

class XmlFullTests : public CPPUNIT_NS :: TestFixture
{
public:
	CPPUNIT_TEST_SUITE (XmlFullTests);
	CPPUNIT_TEST (readFullOK);
	CPPUNIT_TEST (readEmptyGeometricMeasurementOK);
	CPPUNIT_TEST (readWithEmptyMeasurementsOK);
	CPPUNIT_TEST (readImageContextOnlyOK);
	CPPUNIT_TEST (readObjectsOnlyFail);
	CPPUNIT_TEST_SUITE_END ();

	public:
		void setUp (void);
		void tearDown (void);

	protected:
		void readFullOK (void);
		void readEmptyGeometricMeasurementOK(void);
		void readWithEmptyMeasurementsOK(void);
		void readImageContextOnlyOK (void);
		void readObjectsOnlyFail (void);
	private:
		ObjectInterface* interf_;
		XmlStreamReaderMock* reader_;
		ImageContextReader*  icr_;
};

#endif /* XML_FULL_TESTS_H_ */
