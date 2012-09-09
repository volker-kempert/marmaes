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

#include <QVariant>
#include <QXmlStreamReader>
#include <QDebug>
#include <QPointF>
#include <QRectF>

#include <math.h>

#include "xml_basic_tests.h"
#include <basic_type_read_write.h>


CPPUNIT_TEST_SUITE_REGISTRATION (XMLBasicTests);

void XMLBasicTests::setUp() {
}

void XMLBasicTests::tearDown() {
}

void XMLBasicTests::readFloat() {
	QVariant data;
	QXmlStreamReaderWhitespace reader;
	reader.addData("<float_test type=\"double\">0.345</float_test>");
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	const QString tag("float_test");
	QVariantReader sut(tag, data);
	CPPUNIT_ASSERT ( sut.checkToken(reader));
	CPPUNIT_ASSERT ( sut.read(reader));
	bool result = true;
	double dVar1 = 0.345;
	double dVar2 = data.toDouble(&result);
	CPPUNIT_ASSERT ( result );
	//double epsilon = DBL_EPSILON *100 ;
	double epsilon = 0.0001;
	// qDebug() << "v1= " << dVar1 << "v2= " << dVar2 << "epsilon= " << epsilon;
	bool bEqual = fabs(dVar1 - dVar2) < epsilon;
	CPPUNIT_ASSERT ( bEqual );

}

void XMLBasicTests::readFloatFail() {
	QVariant data;
	QVariant ref(0.311);
	QXmlStreamReaderWhitespace reader;
	reader.addData("<float_test type=\"double\">0.345</float_test>");
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	const QString tag("float_test");
	QVariantReader sut(tag, data);
	CPPUNIT_ASSERT ( sut.checkToken(reader));
	CPPUNIT_ASSERT ( sut.read(reader));
	CPPUNIT_ASSERT( data != ref);
}

void XMLBasicTests::readInt() {
	QVariant data;
	QVariant ref((int)12);
	QXmlStreamReaderWhitespace reader;
	reader.addData("<int_test type=\"integer\">12</int_test>");
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	const QString tag("int_test");
	QVariantReader sut(tag, data);
	CPPUNIT_ASSERT ( sut.checkToken(reader));
	CPPUNIT_ASSERT ( sut.read(reader));
	CPPUNIT_ASSERT( data == ref);
}

void XMLBasicTests::readIntFail() {
	QVariant data;
	QVariant ref(12);
	QXmlStreamReaderWhitespace reader;
	reader.addData("<int_test type=\"integer\">-13</int_test>");
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	const QString tag("int_test");
	QVariantReader sut(tag, data);
	CPPUNIT_ASSERT ( sut.checkToken(reader));
	CPPUNIT_ASSERT ( sut.read(reader));
	CPPUNIT_ASSERT( data != ref);
}

void XMLBasicTests::readBool_1() {
	QVariant data;
	QVariant ref(true);
	QXmlStreamReaderWhitespace reader;
	reader.addData("<bool_test type=\"bool\">True</bool_test>");
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	const QString tag("bool_test");
	QVariantReader sut(tag, data);
	CPPUNIT_ASSERT ( sut.checkToken(reader));
	CPPUNIT_ASSERT ( sut.read(reader));
	CPPUNIT_ASSERT( data == ref);
}

void XMLBasicTests::readBool_2() {
	QVariant data;
	QVariant ref(true);
	QXmlStreamReaderWhitespace reader;
	reader.addData("<bool_test type=\"bool\">true</bool_test>");
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	const QString tag("bool_test");
	QVariantReader sut(tag, data);
	CPPUNIT_ASSERT ( sut.checkToken(reader));
	CPPUNIT_ASSERT ( sut.read(reader));
	CPPUNIT_ASSERT( data == ref);
}

void XMLBasicTests::readBoolFlag_1() {
	QVariant data;
	QVariant ref(true);
	QXmlStreamReaderWhitespace reader;
	reader.addData("<bool_test></bool_test>");
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	const QString tag("bool_test");
	QVariantReader sut(tag, data);
	CPPUNIT_ASSERT ( sut.checkToken(reader));
	CPPUNIT_ASSERT ( sut.read(reader));
	CPPUNIT_ASSERT( data == ref);
}

void XMLBasicTests::readBoolFlag_2() {
	QVariant data;
	QVariant ref(true);
	QXmlStreamReaderWhitespace reader;
	reader.addData("<bool_test/>");
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	const QString tag("bool_test");
	QVariantReader sut(tag, data);
	CPPUNIT_ASSERT ( sut.checkToken(reader));
	CPPUNIT_ASSERT ( sut.read(reader));
	CPPUNIT_ASSERT( data == ref);
}


void XMLBasicTests::readBoolFail() {
	QVariant data;
	QVariant ref(false);
	QXmlStreamReaderWhitespace reader;
	reader.addData("<bool_test type=\"bool\">false</bool_test>");
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	const QString tag("bool_test");
	QVariantReader sut(tag, data);
	CPPUNIT_ASSERT ( sut.checkToken(reader));
	CPPUNIT_ASSERT ( sut.read(reader));
	CPPUNIT_ASSERT( data == ref);
}

void XMLBasicTests::readString() {
	QVariant data;
	QVariant ref(QString("test text"));
	QXmlStreamReaderWhitespace reader;
	reader.addData("<string_test type=\"string\">test text</string_test>");
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	const QString tag("string_test");
	QVariantReader sut(tag, data);
	CPPUNIT_ASSERT ( sut.checkToken(reader));
	CPPUNIT_ASSERT ( sut.read(reader));
	CPPUNIT_ASSERT( data == ref);
}

void XMLBasicTests::readPoint1() {
	QVariant data;
	QVariant ref(QPointF(435,100));
	QXmlStreamReaderWhitespace reader;
	reader.addData("<point_test type=\"point\">"
			      "<x>435</x> <y>100</y></point_test>");
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	const QString tag("point_test");
	QVariantReader sut(tag, data);
	CPPUNIT_ASSERT ( sut.checkToken(reader));
	CPPUNIT_ASSERT ( sut.read(reader));
	CPPUNIT_ASSERT( data == ref);
}

void XMLBasicTests::readPoint2() {
	QVariant data;
	QVariant ref(QPointF(435.5,100.5));
	QXmlStreamReaderWhitespace reader;
	reader.addData("<point_test type=\"point\"><x>435.5</x><y>100.5</y></point_test>");
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	const QString tag("point_test");
	QVariantReader sut(tag, data);
	CPPUNIT_ASSERT ( sut.checkToken(reader));
	CPPUNIT_ASSERT ( sut.read(reader));
	CPPUNIT_ASSERT( data == ref);
}

void XMLBasicTests::readPoint3() {
	QVariant data;
	QVariant ref(QPointF(435.5,100.5));
	QXmlStreamReaderWhitespace reader;
	reader.addData("<point_test type=\"point\"><y>100.5</y><x>435.5</x></point_test>");
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	const QString tag("point_test");
	QVariantReader sut(tag, data);
	CPPUNIT_ASSERT ( sut.checkToken(reader));
	CPPUNIT_ASSERT ( sut.read(reader));
	CPPUNIT_ASSERT( data == ref);
}

void XMLBasicTests::readPointFail() {
	QVariant data;
	QVariant ref(QPointF(435.5,99.5));
	QXmlStreamReaderWhitespace reader;
	reader.addData("<point_test type=\"point\"><x>435.5</x><y>100.5</y></point_test>");
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	const QString tag("point_test");
	QVariantReader sut(tag, data);
	CPPUNIT_ASSERT ( sut.checkToken(reader));
	CPPUNIT_ASSERT ( sut.read(reader));
	CPPUNIT_ASSERT( data != ref);
}

void XMLBasicTests::readRect1() {
	QVariant data;
	QVariant ref(QRectF(435,100,20,40));
	QXmlStreamReaderWhitespace reader;
	reader.addData("<rect_test type=\"box\"><left>435</left><top>100</top><width>20</width><height>40</height></rect_test>");
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	const QString tag("rect_test");
	QVariantReader sut(tag, data);
	CPPUNIT_ASSERT ( sut.checkToken(reader));
	CPPUNIT_ASSERT ( sut.read(reader));
	CPPUNIT_ASSERT( data == ref);
}

void XMLBasicTests::readRect2() {
	QVariant data;
	QVariant ref(QRectF(435,100,20,40));
	QXmlStreamReaderWhitespace reader;
	reader.addData("<rect_test type=\"box\"><width>20</width><top>100</top><height>40</height><left>435</left></rect_test>");
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	const QString tag("rect_test");
	QVariantReader sut(tag, data);
	CPPUNIT_ASSERT ( sut.checkToken(reader));
	CPPUNIT_ASSERT ( sut.read(reader));
	CPPUNIT_ASSERT( data == ref);
}

void XMLBasicTests::readRectFail1() {
	QVariant data;
	QVariant ref(QRectF(435,100,20,40));
	QXmlStreamReaderWhitespace reader;
	reader.addData("<rect_test type=\"box\"><left>435</left><top>100</top><height>40</height></rect_test>");
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	const QString tag("rect_test");
	QVariantReader sut(tag, data);
	CPPUNIT_ASSERT ( sut.checkToken(reader));
	CPPUNIT_ASSERT ( !sut.read(reader));

}

void XMLBasicTests::readRectFail2() {
	QVariant data;
	QVariant ref(QRectF(435,100,200,40));
	QXmlStreamReaderWhitespace reader;
	reader.addData("<rect_test type=\"box\"><left>435</left><top>100</top><width>20</width><height>40</height></rect_test>");
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	const QString tag("rect_test");
	QVariantReader sut(tag, data);
	CPPUNIT_ASSERT ( sut.checkToken(reader));
	CPPUNIT_ASSERT ( sut.read(reader));
	CPPUNIT_ASSERT( data != ref);
}


void XMLBasicTests::writeFloat() {
	QVariant data;
	QString  tmp;
	QVariant ref((double)0.345);
	const QString tag("float_test");
	QVariantWriter sut(tag,ref);
	QXmlStreamWriter writer(&tmp);
	CPPUNIT_ASSERT ( sut.write(writer));

	// qDebug() << "xml = " << tmp;
	// read back for test
	QXmlStreamReaderWhitespace reader;
	reader.addData(tmp);
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();
	QVariantReader read_variant(tag, data);
	CPPUNIT_ASSERT ( read_variant.checkToken(reader));
	CPPUNIT_ASSERT ( read_variant.read(reader));
	bool result = true;
	double dVar1 = 0.345;
	double dVar2 = data.toDouble(&result);
	CPPUNIT_ASSERT ( result );
	//double epsilon = DBL_EPSILON *100 ;
	double epsilon = 0.0001;
	// qDebug() << "v1= " << dVar1 << "v2= " << dVar2 << "epsilon= " << epsilon;
	bool bEqual = fabs(dVar1 - dVar2) < epsilon;
	CPPUNIT_ASSERT ( bEqual );
}

void XMLBasicTests::writeInt() {
	QVariant data;
	QVariant ref((int)12);
	QString  tmp;
	const QString tag("int_test");

	QVariantWriter sut(tag, ref);
	QXmlStreamWriter writer(&tmp);
	CPPUNIT_ASSERT ( sut.write(writer));
	// qDebug() << "xml = " << tmp;

	QXmlStreamReaderWhitespace reader;
	reader.addData(tmp);
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();

	QVariantReader read_variant(tag, data);
	CPPUNIT_ASSERT ( read_variant.checkToken(reader));
	CPPUNIT_ASSERT ( read_variant.read(reader));
	CPPUNIT_ASSERT( data == ref);
	}

void XMLBasicTests::writeBool() {
	QVariant data;
	QVariant ref(true);
	QString  tmp;
	const QString tag("bool_test");

	QVariantWriter sut(tag,ref);
	QXmlStreamWriter writer(&tmp);
	CPPUNIT_ASSERT ( sut.write(writer));
	// qDebug() << "xml = " << tmp;

	QXmlStreamReaderWhitespace reader;
	reader.addData(tmp);
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();

	QVariantReader read_variant(tag, data);
	CPPUNIT_ASSERT ( read_variant.checkToken(reader));
	CPPUNIT_ASSERT ( read_variant.read(reader));
	CPPUNIT_ASSERT( data == ref);
	}

void XMLBasicTests::writeString() {
	QVariant data;
	QVariant ref("dos palomas negras");
	QString  tmp;
	const QString tag("string_test");

	QVariantWriter sut(tag,ref);
	QXmlStreamWriter writer(&tmp);
	CPPUNIT_ASSERT ( sut.write(writer));
	// qDebug() << "xml = " << tmp;

	QXmlStreamReaderWhitespace reader;
	reader.addData(tmp);
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();

	QVariantReader read_variant(tag, data);
	CPPUNIT_ASSERT ( read_variant.checkToken(reader));
	CPPUNIT_ASSERT ( read_variant.read(reader));
	CPPUNIT_ASSERT( data == ref);
	}


void XMLBasicTests::writePoint() {
	QVariant data;
	QPointF point_ref(133,-133);
	QVariant ref(point_ref);
	QString  tmp;
	const QString tag("point_test");

	QVariantWriter sut(tag,ref);
	QXmlStreamWriter writer(&tmp);
	CPPUNIT_ASSERT ( sut.write(writer));
	// qDebug() << "xml = " << tmp;

	QXmlStreamReaderWhitespace reader;
	reader.addData(tmp);
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();

	QVariantReader read_variant(tag, data);
	CPPUNIT_ASSERT ( read_variant.checkToken(reader));
	CPPUNIT_ASSERT ( read_variant.read(reader));
	CPPUNIT_ASSERT( data == ref);
	}

void XMLBasicTests::writeRect() {
	QVariant data;
	QRectF box_ref(133,-133, 44,55);
	QVariant ref(box_ref);
	QString  tmp;
	const QString tag("box_test");

	QVariantWriter sut(tag,ref);
	QXmlStreamWriter writer(&tmp);
	CPPUNIT_ASSERT ( sut.write(writer));
	// qDebug() << "xml = " << tmp;

	QXmlStreamReaderWhitespace reader;
	reader.addData(tmp);
	reader.readNext();  // read.next has to happen twice, don't know why
	reader.readNext();

	QVariantReader read_variant(tag, data);
	CPPUNIT_ASSERT ( read_variant.checkToken(reader));
	CPPUNIT_ASSERT ( read_variant.read(reader));
	CPPUNIT_ASSERT( data == ref);
	}
