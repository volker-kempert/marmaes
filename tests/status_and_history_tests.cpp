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

#include "status_and_history_tests.h"

#include "status_and_history.h"


CPPUNIT_TEST_SUITE_REGISTRATION (StatusAndHistoryTests);

void StatusAndHistoryTests :: setUp (void)
{
}

void StatusAndHistoryTests :: tearDown (void)
{
}

void StatusAndHistoryTests::testHistoryAdd (void){
	bool result = true;
	StatusAndHistory* h = StatusAndHistory::getInstance();
	h->imageLoaded("aa");
	result &= (h->getHistoryFileName(0) == "aa");
	// result &= (h->getMaxNumberOfHistoryEntries() == 1);
	delete h;
	CPPUNIT_ASSERT ( result);
}


void StatusAndHistoryTests::testHistoryAddUnknown (void){
	bool result = true;
	StatusAndHistory* h = StatusAndHistory::getInstance();
	h->imageLoaded("aa");
	h->imageLoaded("bb");
	result &= (h->getHistoryFileName(0) == "bb");
	result &= (h->getHistoryFileName(1) == "aa");
	delete h;
	CPPUNIT_ASSERT ( result);
}

void StatusAndHistoryTests::testHistoryAddKnown (void) {
	bool result = true;
	StatusAndHistory* h = StatusAndHistory::getInstance();
	h->imageLoaded("aa");
	h->imageLoaded("bb");
	h->imageLoaded("cc");
	h->imageLoaded("aa");
	result &= (h->getHistoryFileName(0) == "aa");
	result &= (h->getHistoryFileName(1) == "cc");
	result &= (h->getHistoryFileName(2) == "bb");
	delete h;
	CPPUNIT_ASSERT ( result);

}

void StatusAndHistoryTests::testHistoryOverLimit (void) {
	bool result = true;
	StatusAndHistory* h = StatusAndHistory::getInstance();
	h->imageLoaded("aa");
	h->imageLoaded("bb");
	h->imageLoaded("cc");
	h->imageLoaded("dd");
	h->imageLoaded("ee");
	h->imageLoaded("ff");
	h->imageLoaded("gg");
	h->imageLoaded("hh");
	h->imageLoaded("ii");

	result &= (h->getHistoryFileName(0) == "ii");
	result &= (h->getHistoryFileName(1) == "hh");
	result &= (h->getHistoryFileName(2) == "gg");
	result &= (h->getHistoryFileName(3) == "ff");
	result &= (h->getHistoryFileName(4) == "ee");
	result &= (h->getHistoryFileName(5) == "dd");
	result &= (h->getHistoryFileName(6) == "cc");
	result &= (h->getHistoryFileName(7) == "bb");

	result &= (h->getMaxNumberOfHistoryEntries() == 8);
	delete h;
	CPPUNIT_ASSERT ( result);

}

void StatusAndHistoryTests::testHistoryToLimit (void) {
	bool result = true;
	StatusAndHistory* h = StatusAndHistory::getInstance();
	h->imageLoaded("aa");
	h->imageLoaded("bb");
	h->imageLoaded("cc");
	h->imageLoaded("dd");
	h->imageLoaded("ee");
	h->imageLoaded("ff");
	h->imageLoaded("gg");
	h->imageLoaded("hh");
	h->imageLoaded("aa");

	result &= (h->getHistoryFileName(0) == "aa");
	result &= (h->getHistoryFileName(1) == "hh");
	result &= (h->getHistoryFileName(2) == "gg");
	result &= (h->getHistoryFileName(3) == "ff");
	result &= (h->getHistoryFileName(4) == "ee");
	result &= (h->getHistoryFileName(5) == "dd");
	result &= (h->getHistoryFileName(6) == "cc");
	result &= (h->getHistoryFileName(7) == "bb");

	result &= (h->getMaxNumberOfHistoryEntries() == 8);
	delete h;
	CPPUNIT_ASSERT ( result);

}
