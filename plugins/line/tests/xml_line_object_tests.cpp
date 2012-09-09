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

#include <QSettings>
#include <QVariant>
#include <QXmlStreamReader>
#include <QDebug>
#include <QPointF>
#include <QRectF>




#include <math.h>


#include "xml_line_object_tests.h"
#include "../src/line.h"

// Mock implementations


CPPUNIT_TEST_SUITE_REGISTRATION (XMLLineObjectTests);

class ObjectTestReader: public ObjectBaseReader {
public:
	ObjectTestReader() : ObjectBaseReader() {}
	GeometricObject* geometricObject() { return object_; }
};

void XMLLineObjectTests::setUp() {
	ObjectPluginManager* manager = ObjectPluginManager::getInstance();
	qDebug() << "--XMLLineObjectTests::setUp-- do scene";
	QGraphicsScene* graphics_scene;
	qDebug() << "--XMLLineObjectTests::setUp-- do2 scene";
	graphics_scene = new QGraphicsScene();
	qDebug() << "--XMLLineObjectTests::setUp-- do3 scene";
	CPPUNIT_ASSERT (graphics_scene);
	qDebug() << "--XMLLineObjectTests::setUp-- done scene";

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
	sut_ = (ObjectTestReader*) interf_->getXMLReader();
	CPPUNIT_ASSERT ( sut_ );
	sut_->setScene(graphics_scene);
}

void XMLLineObjectTests::tearDown() {
	// ToDo nice to have unload interface
}

void XMLLineObjectTests::readLineModel() {
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<object id=\"45\" type=\"line\">"
			"  <model> "
            "     <end type=\"point\">"
			"        <x>200</x>"
			"        <y>250</y>"
			"     </end>"
            "     <start type=\"point\">"
			"        <x>100</x>"
			"        <y>150</y>"
			"     </start>"
			"  </model>"
			"</object>"
			);
	reader.readNext();
	reader.readNext();
	CPPUNIT_ASSERT ( sut_->checkToken(reader));
	CPPUNIT_ASSERT ( sut_->read(reader));
	GeometricLine* data;
	data = (GeometricLine* ) sut_->geometricObject();
	CPPUNIT_ASSERT ( data != 0 );
	QPointF start = data->getStart();
	QPointF end   = data->getEnd();
	CPPUNIT_ASSERT ( start.rx() == 100 );
	CPPUNIT_ASSERT ( start.ry() == 150 );
	CPPUNIT_ASSERT ( end.rx() == 200 );
	CPPUNIT_ASSERT ( end.ry() == 250 );
}

void XMLLineObjectTests::readLineModel2() {
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<object id=\"45\" type=\"line\">"
			"  <model name=\"Line\">"
            "     <start type=\"point\">"
			"        <x>100</x>"
			"        <y>150</y>"
			"     </start>"
			"     <end type=\"point\">"
			"        <y>250</y>"
			"        <x>200</x>"
			"     </end>"
			"  </model>"
			"</object>"
			);
	reader.readNext();
	reader.readNext();
	GeometricLine* data;
	CPPUNIT_ASSERT ( sut_->checkToken(reader));
	CPPUNIT_ASSERT ( sut_->read(reader));
	data = (GeometricLine* ) sut_->geometricObject();
	CPPUNIT_ASSERT ( data != 0 );
	QPointF start = data->getStart();
	QPointF end   = data->getEnd();
	CPPUNIT_ASSERT ( start.rx() == 100 );
	CPPUNIT_ASSERT ( start.ry() == 150 );
	CPPUNIT_ASSERT ( end.rx() == 200 );
	CPPUNIT_ASSERT ( end.ry() == 250 );
}

void XMLLineObjectTests::readLineModelFail() {
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<object id=\"45\" type=\"line\">"
			"  <model>"
            "     <start type=\"point\">"
			"        <x>100</x>"
			"        <y>150</y>"
			"     </start>"
			"  </model>"
			"</object>"
			);
	reader.readNext();
	reader.readNext();
	GeometricLine* data;
	CPPUNIT_ASSERT ( sut_->checkToken(reader));
	CPPUNIT_ASSERT (! sut_->read(reader));
}


void XMLLineObjectTests::readLineModelFail2() {

 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<object id=\"45\" type=\"line\">"
			"  <model>"
            "     <end type=\"point\">"
			"        <x>100</x>"
			"        <y>150</y>"
			"     </end>"
            "     <start type=\"point\">"
			"        <x>100</x>"
			"        <y>150</y>"
			"     </start>"
            "     <start type=\"point\">"
			"        <x>100</x>"
			"        <y>150</y>"
			"     </start>"
			"  </model>"
			"</object>"
			);
	reader.readNext();
	reader.readNext();
	GeometricLine* data;
	CPPUNIT_ASSERT ( sut_->checkToken(reader));
	CPPUNIT_ASSERT ( ! sut_->read(reader));
}

void XMLLineObjectTests::readLineModelFail3() {
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<object id=\"45\" type=\"line\">"
			"  <model>"
            "     <end type=\"point\">"
			"        <x>100</x>"
			"        <y>150</y>"
			"     </end>"
			"  </model>"
			"</object>"
			);
	reader.readNext();
	reader.readNext();
	GeometricLine* data;
	CPPUNIT_ASSERT ( sut_->checkToken(reader));
	CPPUNIT_ASSERT (! sut_->read(reader));
}


void XMLLineObjectTests::readLineModelFail4() {
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<object id=\"45\" type=\"Line\">"
			"  <model>"
            "     <end type=\"point\">"
			"        <x>100</x>"
			"        <y>150</y>"
			"     </end>"
			"  </model>"
			"</object>"
			);
	reader.readNext();
	reader.readNext();
	GeometricLine* data;
	CPPUNIT_ASSERT (! sut_->checkToken(reader));
}


void XMLLineObjectTests::readLineModelFail5() {
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<object id=\"45\" type=\"line\">"
			"  <mmodel>"
            "     <end type=\"point\">"
			"        <x>100</x>"
			"        <y>150</y>"
			"     </end>"
			"  </mmodel>"
			"</object>"
			);
	reader.readNext();
	reader.readNext();
	GeometricLine* data;
	CPPUNIT_ASSERT (sut_->checkToken(reader));
	CPPUNIT_ASSERT (! sut_->read(reader));
}

void XMLLineObjectTests::readLineModelFail6() {
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<object id=\"45\" type=\"line\">"
			"  <model>"
            "     <end type=\"point\">"
			"        <x>100</x>"
			"        <y>150</y>"
			"     </end>"
			"  </amodel>"
			"</object>"
			);
	reader.readNext();
	reader.readNext();
	GeometricLine* data;
	CPPUNIT_ASSERT (sut_->checkToken(reader));
	CPPUNIT_ASSERT (! sut_->read(reader));
}

void XMLLineObjectTests::readLineModelFail7() {
 	QXmlStreamReaderWhitespace reader;
	reader.addData(
			"<object id=\"45\" type=\"line\">"
			"  <model>"
            "     <end type=\"point\">"
			"        <x>100</x>"
			"        <y>150</y>"
			"     </end>"
			"  </model>"
			"</aobject>"
			);
	reader.readNext();
	reader.readNext();
	GeometricLine* data;
	CPPUNIT_ASSERT (sut_->checkToken(reader));
	CPPUNIT_ASSERT (! sut_->read(reader));
}

void XMLLineObjectTests::writeLine() {
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

void XMLLineObjectTests::readLine() {
	CPPUNIT_ASSERT ( true );
}

