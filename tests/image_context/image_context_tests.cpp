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

#include <QDebug>

#include "image_context/image_context_tests.h"
#include "../src/marmeas_core/image_context/image_context.h"



CPPUNIT_TEST_SUITE_REGISTRATION (ImageContextTests);

void ImageContextTests::setUp() {
}

void ImageContextTests::tearDown() {
}

void ImageContextTests::test_01_useImageFileName() {
	qDebug() << "**ImageContextTests::test_01_useImageFileName**";
	ImageContext* ic = ImageContext::getInstance();
	ic->resetImageContext();
	ic->setImageFileName("abcde.png");
	CPPUNIT_ASSERT ( "abcde.xod" == ic->getObjectShortName() );
	CPPUNIT_ASSERT ( "abcde.png" == ic->getImageShortName() );

}

void ImageContextTests::test_02_useImageFileName() {
	qDebug() << "**ImageContextTests::test_02_useImageFileName**";
	ImageContext* ic = ImageContext::getInstance();
	ic->resetImageContext();
	ic->setImageFileName("/abc/abcde.png");
	ic->setDirectory("/dir/dir");
	CPPUNIT_ASSERT ( "abcde.xod" == ic->getObjectShortName() );
	CPPUNIT_ASSERT ( "/abc/abcde.xod" == ic->getObjectFileName() );
}


void ImageContextTests::test_01_doNotUpdateObjectFileNames() {
	qDebug() << "**ImageContextTests::test_01_doNotUpdateObjectFileNames**";
	ImageContext* ic = ImageContext::getInstance();
	ic->resetImageContext();
	ic->setObjectFileName("/def/def.xml");
	ic->setImageFileName("/abc/abcde.jpg");
	CPPUNIT_ASSERT ( "def.xml" == ic->getObjectShortName() );
	CPPUNIT_ASSERT ( "/def/def.xml" == ic->getObjectFileName() );
	CPPUNIT_ASSERT ( "abcde.jpg" == ic->getImageShortName() );
	CPPUNIT_ASSERT ( "/abc/abcde.jpg" == ic->getImageFileName() );
}


void ImageContextTests::test_01_useTemplateDefinition() {
	qDebug() << "**ImageContextTests::test_01_useTemplateDefinition**";
	ImageContext* ic = ImageContext::getInstance();
	ic->resetImageContext();
	ic->setImageFileName("/abc/abc/abcde.png");
	ic->setDirectory("/dir/dir");
	ic->setParam1Data("is");
	ic->setParam2Data("ok");
	ic->setFilePattern("a-$i$-$p1$-$p2$-b");
	CPPUNIT_ASSERT ( "/dir/dir/a-abcde-is-ok-b.xod" == ic->getObjectFileName() );
	CPPUNIT_ASSERT ( "a-abcde-is-ok-b.xod" == ic->getObjectShortName() );
	CPPUNIT_ASSERT ( "abcde.png" == ic->getImageShortName() );

}

void ImageContextTests::test_02_useTemplateDefinition() {
	qDebug() << "**ImageContextTests::test_02_useTemplateDefinition**";
	ImageContext* ic = ImageContext::getInstance();
	ic->resetImageContext();
	ic->setImageFileName("/abc/abc/def.jpg.png");
	ic->setFilePattern("a-$i$-$p1$-b");
	CPPUNIT_ASSERT ( "a-def--b.xod" == ic->getObjectShortName() );
	CPPUNIT_ASSERT ( "/a-def--b.xod" == ic->getObjectFileName() );
	CPPUNIT_ASSERT ( "def.jpg.png" == ic->getImageShortName() );
	CPPUNIT_ASSERT ( "/abc/abc/def.jpg.png" == ic->getImageFileName() );
}


void ImageContextTests::test_03_useTemplateDefinition() {
	qDebug() << "**ImageContextTests::test_03_useTemplateDefinition**";
	ImageContext* ic = ImageContext::getInstance();
	ic->resetImageContext();
	ic->setImageFileName("/abc/abc/abcde.png");
	ic->setDirectory("/base");
	ic->setParam1Data("is");
	ic->setParam2Data("ok");
	ic->setFilePattern("$p1$/$p2$/pre-$i$-suf");
	CPPUNIT_ASSERT ( "/base/is/ok/pre-abcde-suf.xod" == ic->getObjectFileName() );
	CPPUNIT_ASSERT ( "abcde.png" == ic->getImageShortName() );
	CPPUNIT_ASSERT ( "/abc/abc/abcde.png" == ic->getImageFileName() );
}
