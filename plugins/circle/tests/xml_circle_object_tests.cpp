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
#include <QSettings>

#include <math.h>

#include "xml_circle_object_tests.h"
#include "../src/circle.h"



CPPUNIT_TEST_SUITE_REGISTRATION (XMLCircleObjectTests);

class ObjectTestCircleReader: public ObjectBaseReader {
public:
	ObjectTestCircleReader() : ObjectBaseReader() {}
	GeometricObject* geometricObject() { return object_; }
};

void XMLCircleObjectTests::setUp() {
	ObjectPluginManager* manager = ObjectPluginManager::getInstance();

	// figure out where the plugin library is located
	QSettings settings("dobus","marmeas");
	settings.beginGroup("system");
	QString plugin_dir = settings.value("pluginDirectory").toString();
	settings.endGroup();
	qDebug() << "--XMLCircleObjectTests::setUp-- "
			 << "settings plugin directory: " << plugin_dir;
	manager->registerPluginDir(plugin_dir);

	interf_ = manager->plugin("libmarmeascircle.so");
	CPPUNIT_ASSERT ( interf_ );
	sut_ = (ObjectTestCircleReader*) interf_->getXMLReader();
	CPPUNIT_ASSERT ( sut_ );
}


void XMLCircleObjectTests::tearDown() {
	// ToDo nice to have unload interface
}

void XMLCircleObjectTests::readCircle() {

	ObjectPluginManager* manager = ObjectPluginManager::getInstance();
	ObjectInterface* interf = manager->plugin("libmarmeascircle.so");
	CPPUNIT_ASSERT ( interf );

 	QXmlStreamReaderWhitespace reader;
	reader.addData("<object version=\"1.0\" id=\"0815\">"
			"  <name> test_object </name>"
			"  <source> user one </source>"
			"  <model name=\"circle\">"
            "     <Midpoint type=\"point\">"
			"        <x>100</x>"
			"        <y>100</y>"
			"     </Midpoint>"
			"     <Radius type=\"float\">25</Radius>"
			"  </model>"
			"</object>");
	reader.readNext();
	reader.readNext();

	// ToDo
	GeometricObject* data;
	CPPUNIT_ASSERT ( true );
}

void XMLCircleObjectTests::writeCircle() {
	/*
	// create a line Object
	GeometricLine line;
	line.setStart(QPointF(100,100));
	line.setEnd(QPointF(200,200));
	line.setSource("user one");
	line.setName("test line");
	line.setId(4812);
	GeometricObject* reference = (GeometricObject *)(&line);

	QString  tmp;
	QXmlStreamWriter writer(&tmp);
	ObjectBaseWriter object_writer(reference);
	object_writer.write(writer);

	// qDebug() << tmp;
	*/

	CPPUNIT_ASSERT ( true );
}



