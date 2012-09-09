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

#include "xml_image_context_test.h"
#include "xml_reader_writer.h"
#include "marmeas_core/image_context/image_context.h"
#include <QDebug>

CPPUNIT_TEST_SUITE_REGISTRATION (XmlImageContextTests);

void XmlImageContextTests :: setUp (void)
{
	ImageContext* input = ImageContext::getInstance();
	icr_ = new ImageContextReader(input);
	CPPUNIT_ASSERT (icr_);
}

void XmlImageContextTests :: tearDown (void)
{
	delete icr_;
}

void XmlImageContextTests::writeImageContext (void) {
	bool result = true;
	CPPUNIT_ASSERT ( result);
	// ToDo
}


void XmlImageContextTests::readImageContextOK (void) {
	qDebug() << "**XmlImageContextTests::readImageContextOK**";
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" // line 0
			"<image_context version=\"1.0\">\n"
			"  <image_file>/home/volker/Desktop/25783_41_017.bmp</image_file>\n"
			"  <unit_of_measurement>m</unit_of_measurement>\n"
			"  <conversion_factor>2</conversion_factor>\n"
			"  <param_one_name>abcd</param_one_name>\n" // line 5
			"  <param_one_data>xyz</param_one_data>\n"
			"  <param_two_name> a</param_two_name>\n"
			"  <param_two_data> b</param_two_data>\n"
			"</image_context>\n"
			);
	reader.readNext();
	reader.readNext();
	CPPUNIT_ASSERT (icr_->checkToken(reader));
	CPPUNIT_ASSERT (icr_->read(reader));

}

void XmlImageContextTests::readImageContextTokenMisMatch (void) {
	qDebug() << "**XmlImageContextTests::readImageContextTokenMisMatch**";
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" // line 0
			"<image_contex version=\"1.0\">\n"
			"  <image_file>/home/volker/Desktop/25783_41_017.bmp</image_file>\n"
			"  <unit_of_measurement>m</unit_of_measurement>\n"
			"  <conversation_factor>2</conversation_factor>\n"
			"  <param_one_name>abcd</param_one_name>\n" // line 5
			"  <param_one_data>xyz</param_one_data>\n"
			"  <param_two_name> a</param_two_name>\n"
			"  <param_two_data> b</param_two_data>\n"
			"</image_contex>\n"
			);
	reader.readNext();
	reader.readNext();
	CPPUNIT_ASSERT (! icr_->checkToken(reader));
}

void XmlImageContextTests::readImageContextIncomplete (void) {
	qDebug() << "**XmlImageContextTests::readImageContextIncomplete**";
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" // line 0
			"<image_context version=\"1.0\">\n"
			"  <image_file>/home/volker/Desktop/25783_41_017.bmp</image_file>\n"
			"  <unit_of_measurement>m</unit_of_measurement>\n"
			"</image_context>\n"
			);
	reader.readNext();
	reader.readNext();
	CPPUNIT_ASSERT (icr_->checkToken(reader));
	CPPUNIT_ASSERT (! icr_->read(reader));
}
void XmlImageContextTests::readImageContextNumberConversionError (void) {
	qDebug() << "**XmlImageContextTests::readImageContextNumberConversionError**";
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" // line 0
			"<image_context version=\"1.0\">\n"
			"  <image_file>/home/volker/Desktop/25783_41_017.bmp</image_file>\n"
			"  <unit_of_measurement>m</unit_of_measurement>\n"
			"  <conversation_factor>2..2</conversation_factor>\n"
			"  <param_one_name>abcd</param_one_name>\n" // line 5
			"  <param_one_data>xyz</param_one_data>\n"
			"</image_context>\n"
			);
	reader.readNext();
	reader.readNext();
	CPPUNIT_ASSERT (icr_->checkToken(reader));
	CPPUNIT_ASSERT (! icr_->read(reader));
}

void XmlImageContextTests::readImageContextEmptyParameterError (void) {
	qDebug() << "**XmlImageContextTests::readImageContextEmptyParameterError**";
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" // line 0
			"<image_context version=\"1.0\">\n"
			"  <image_file>/home/volker/Desktop/25783_41_017.bmp</image_file>\n"
			"  <unit_of_measurement>m</unit_of_measurement>\n"
			"  <conversation_factor>2</conversation_factor>\n"
			"  <param_one_name>abcd</param_one_name>\n" // line 5
			"  <param_one_data>xyz</param_one_data>\n"
			"  <param_two_name></param_two_name>\n"
			"  <param_two_data></param_two_data>\n"
			"</image_context>\n"
			);
	reader.readNext();
	reader.readNext();
	CPPUNIT_ASSERT (icr_->checkToken(reader));
	CPPUNIT_ASSERT (! icr_->read(reader));
	qDebug() << "**XmlImageContextTests::readImageContextEmptyParameterError** error at: " << reader.lineNumber();
	CPPUNIT_ASSERT ( reader.lineNumber() == 6);

}

void XmlImageContextTests::readImageContextNoEnd (void) {
	qDebug() << "**XmlImageContextTests::readImageContextNoEnd**";
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" // line 0
			"<image_context version=\"1.0\">\n"
			"  <image_file>/home/volker/Desktop/25783_41_017.bmp</image_file>\n"
			"  <unit_of_measurement>m</unit_of_measurement>\n"
			"  <conversation_factor>2</conversation_factor>\n"
			"  <param_one_name>abcd</param_one_name>\n" // line 5
			"  <param_one_data>xyz</param_one_data>\n"
			);
	reader.readNext();
	reader.readNext();
	CPPUNIT_ASSERT (icr_->checkToken(reader));
	CPPUNIT_ASSERT (!icr_->read(reader));
	qDebug() << "error at: " << reader.lineNumber();
	CPPUNIT_ASSERT ( reader.lineNumber() == 5);

}

void XmlImageContextTests::readImageContextWrongEnd (void) {
	qDebug() << "**XmlImageContextTests::readImageContextWrongEn**";
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" // line 0
			"<image_context version=\"1.0\">\n"
			"  <image_file>/home/volker/Desktop/25783_41_017.bmp</image_file>\n"
			"  <unit_of_measurement>m</unit_of_measurement>\n"
			"  <conversation_factor>2</conversation_factor>\n"
			"  <param_one_name>abcd</param_one_name>\n" // line 5
			"  <param_one_data>xyz</param_one_data>\n"
			"</image_contex>\n"
			);
	reader.readNext();
	reader.readNext();
	CPPUNIT_ASSERT (icr_->checkToken(reader));
	CPPUNIT_ASSERT (!icr_->read(reader));
}
