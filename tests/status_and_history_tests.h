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

#ifndef STATUS_AND_HISTORY_TESTS_H_
#define STATUS_AND_HISTORY_TESTS_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class StatusAndHistoryTests : public CPPUNIT_NS :: TestFixture
{
public:
	CPPUNIT_TEST_SUITE (StatusAndHistoryTests);
	CPPUNIT_TEST (testHistoryAdd);
	CPPUNIT_TEST (testHistoryAddUnknown);
	CPPUNIT_TEST (testHistoryAddKnown);
	CPPUNIT_TEST (testHistoryOverLimit);
	CPPUNIT_TEST (testHistoryToLimit);
	CPPUNIT_TEST_SUITE_END ();

	public:
		void setUp (void);
		void tearDown (void);

	protected:
		void testHistoryAdd (void);
		void testHistoryAddUnknown (void);
		void testHistoryAddKnown (void);
		void testHistoryToLimit (void);
		void testHistoryOverLimit (void);

	private:

};



#endif /* STATUS_AND_HISTORY_TESTS_H_ */

