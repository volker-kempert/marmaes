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
#include <QSettings>

#include "xml_full_tests.h"
#include "marmeas_core/image_context/image_context.h"
#include "xml_reader_writer.h"
#include "object_base_read_write.h"



CPPUNIT_TEST_SUITE_REGISTRATION (XmlFullTests);


class XmlStreamReaderMock : public XmlStreamReader {
public:
	XmlStreamReaderMock() : XmlStreamReader() { line_ = 0 ; }
	bool readData(const char * data) {
		QXmlStreamReaderWhitespace reader;
		reader.addData(data);
		if (! read(reader) ) {
			line_ = reader.lineNumber();
			qDebug() << "read line: " << line_;
			return false;
		}
		return true;
	}
private:
	unsigned int line_ ;
};

class ObjectMockReader: public ObjectBaseReader {
public:
	ObjectMockReader() : ObjectBaseReader() {}
	GeometricObject* geometricObject() { return object_; }
};


void XmlFullTests :: setUp(void ) {
	ImageContext* input = ImageContext::getInstance();
	icr_ = new ImageContextReader(input);
	reader_ = new XmlStreamReaderMock();
	reader_->registerReader((Reader*)(icr_));
	ObjectPluginManager* manager = ObjectPluginManager::getInstance();

	// figure out where the plugin library is located
	QSettings settings("dobus","marmeas");
	settings.beginGroup("system");
	QString plugin_dir = settings.value("pluginDirectory").toString();
	settings.endGroup();

	qDebug() << "--ObjectPluginManager::pluginNames-- "
			 << "settings plugin directory: " << plugin_dir;
	manager->registerPluginDir(plugin_dir);

	interf_ = manager->plugin("libmarmeasline.so");
	CPPUNIT_ASSERT ( interf_ );
	ObjectMockReader* reader = (ObjectMockReader*) interf_->getXMLReader();
	CPPUNIT_ASSERT ( reader );
	reader_->registerReader( reader );

	QGraphicsScene* graphics_scene = new QGraphicsScene();
	CPPUNIT_ASSERT ( reader_ );
	reader->setScene(graphics_scene);
}

void XmlFullTests :: tearDown(void ) {
}

void XmlFullTests :: readFullOK( void ) {
	CPPUNIT_ASSERT ( reader_->readData(
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" // line 0
			"<marmeas>\n"
			"<image_context version=\"1.0\">\n"
			"  <image_file>/home/volker/Desktop/25783_41_017.bmp</image_file>\n"
			"  <unit_of_measurement>m</unit_of_measurement>\n"
			"  <conversion_factor>2</conversion_factor>\n"
			"  <param_one_name>abcd</param_one_name>\n" // line 5
			"  <param_one_data>xyz</param_one_data>\n"
			"</image_context>\n"
			"<objects>\n"
			"<object id=\"45\" type=\"line\">\n"
			"  <model> \n"
            "     <end type=\"point\">\n"
			"        <x>200</x>\n"
			"        <y>250</y>\n"                  // line 15
			"     </end>\n"
            "     <start type=\"point\">\n"
			"        <x>100</x>\n"
			"        <y>150</y>\n"
			"     </start>\n"                       // line 20
			"  </model>\n"
			"  <measurements>\n"
			"    <geometric_measurements>\n"
			"      <max_dim type=\"double\"> 123.2 </max_dim>"
			"    </geometric_measurements>\n"
			"  </measurements>\n"
			"</object>\n"
			"</objects>\n"
			"</marmeas>\n"
			) );
}

void XmlFullTests :: readEmptyGeometricMeasurementOK( void ) {
	CPPUNIT_ASSERT ( reader_->readData(
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" // line 0
			"<marmeas>\n"
			"<image_context version=\"1.0\">\n"
			"  <image_file>/home/volker/Desktop/25783_41_017.bmp</image_file>\n"
			"  <unit_of_measurement>m</unit_of_measurement>\n"
			"  <conversion_factor>2</conversion_factor>\n"
			"  <param_one_name>abcd</param_one_name>\n" // line 5
			"  <param_one_data>xyz</param_one_data>\n"
			"</image_context>\n"
			"<objects>\n"
			"<object id=\"45\" type=\"line\">\n"
			"  <model> \n"
            "     <end type=\"point\">\n"
			"        <x>200</x>\n"
			"        <y>250</y>\n"                  // line 15
			"     </end>\n"
            "     <start type=\"point\">\n"
			"        <x>100</x>\n"
			"        <y>150</y>\n"
			"     </start>\n"                       // line 20
			"  </model>\n"
			"  <measurements>\n"
			"    <geometric_measurements/>\n"
			"  </measurements>\n"
			"</object>\n"
			"</objects>\n"
			"</marmeas>\n"
			) );
}


void XmlFullTests :: readWithEmptyMeasurementsOK( void ) {
	CPPUNIT_ASSERT ( reader_->readData(
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" // line 0
			"<marmeas>\n"
			"<image_context version=\"1.0\">\n"
			"  <image_file>/home/volker/Desktop/25783_41_017.bmp</image_file>\n"
			"  <unit_of_measurement>m</unit_of_measurement>\n"
			"  <conversion_factor>2</conversion_factor>\n"
			"  <param_one_name>abcd</param_one_name>\n" // line 5
			"  <param_one_data>xyz</param_one_data>\n"
			"</image_context>\n"
			"<objects>\n"
			"<object id=\"45\" type=\"line\">\n"
			"  <model> \n"
            "     <end type=\"point\">\n"
			"        <x>200</x>\n"
			"        <y>250</y>\n"                  // line 15
			"     </end>\n"
            "     <start type=\"point\">\n"
			"        <x>100</x>\n"
			"        <y>150</y>\n"
			"     </start>\n"                       // line 20
			"  </model>\n"
			"  <measurements/>\n"
			"</object>\n"
			"</objects>\n"
			"</marmeas>\n"
			) );
}

void XmlFullTests :: readImageContextOnlyOK( void ) {
	CPPUNIT_ASSERT ( reader_->readData(
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" // line 0
			"<marmeas>\n"
			"<image_context version=\"1.0\">\n"
			"  <image_file>/home/volker/Desktop/25783_41_017.bmp</image_file>\n"
			"  <unit_of_measurement>m</unit_of_measurement>\n"
			"  <conversion_factor>2</conversion_factor>\n"
			"  <param_one_name>abcd</param_one_name>\n" // line 5
			"  <param_one_data>xyz</param_one_data>\n"
			"</image_context>\n"
		    "<objects/>\n"
			"</marmeas>\n"
			) );
}

void XmlFullTests :: readObjectsOnlyFail( void ) {
	CPPUNIT_ASSERT ( ! reader_->readData(
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" // line 0
			"<marmeas>"
			"<objects>\n"                           // line 1
			"<object id=\"45\" type=\"line\">\n"
			"  <model> \n"
            "     <end type=\"point\">\n"
			"        <x>200</x>\n"
			"        <y>250</y>\n"                  // line 5
			"     </end>\n"
            "     <start type=\"point\">\n"
			"        <x>100</x>\n"
			"        <y>150</y>\n"
			"     </start>\n"                       // line 10
			"  </model>\n"
			"  <measurement>\n"
			"    <max_dim type=\"integer\"> 200 </max_dim>\n"
			"    <roundness type=\"integer\"> 0 </roundness>\n"
			"  </measurement>\n"                   // line 15
			"</object>\n"
			"</objects>\n"
			"</marmeas>\n"
			) );
}
