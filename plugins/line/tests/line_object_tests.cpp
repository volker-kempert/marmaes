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
#include <QVariant>
#include <QPointF>
#include <QRectF>
#include <QSettings>
#include <QApplication>

#include <math.h>

#include "../src/marmeas_gui/graphics_scene.h"
#include "line_object_tests.h"

static const char * const TEST_XPM[] = {
     "8 8 15 1",

     /* monochrome definitions with rgb values*/

     "a m #000000", "b m #010101", "c m #020202", "d m #030303", "e m #040404",
     "f m #050505", "g m #060606", "h m #070707", "i m #080808", "j m #090909",
     "k m #0a0a0a", "l m #0b0b0b", "m m #0c0c0c", "n m #0d0d0d", "0 m #0e0d0d",

     /* 8x8 pixels */
     "abcdefgh",
     "bcdefghi",
     "cdefghij",
     "defghijk",
     "efghijkl",
     "fghijklm",
     "ghijklmn",
     "hijklmno"
};


// Mock implementations


CPPUNIT_TEST_SUITE_REGISTRATION (LineObjectTests);

class ObjectTestReader: public ObjectBaseReader {
public:
	ObjectTestReader() : ObjectBaseReader() {}
	GeometricObject* geometricObject() { return object_; }
};

LineObjectTests::LineObjectTests() :
		CPPUNIT_NS :: TestFixture()  {
	qDebug() << "**LineObjectTests::LineObjectTests**" ;
	sut_factory_ = 0;

}

LineObjectTests::~LineObjectTests()  {
	qDebug() << "**LineObjectTests::~LineObjectTests**";

	if ( sut_factory_ ) delete sut_factory_;
	// ToDo nice to have unload interface
}


void LineObjectTests::setUp() {
	qDebug() << "**LineObjectTests::setUp**";
	if ( !sut_factory_ ) {
		object_container_ =
				GeometricObjectContainerSingleton::getInstance();

		QImage* image = new QImage(TEST_XPM);
		object_container_->setImage(image);
		graphics_scene_ = new GraphicsScene();

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

		sut_factory_ = (GeometricObjectFactory*) interf_->objectFactory("line");
		CPPUNIT_ASSERT ( sut_factory_ );
		sut_factory_->setScene(graphics_scene_);


	}
}

void LineObjectTests::tearDown() {
	qDebug() << "**LineObjectTests::tearDown**";
	object_container_->setImage(0);
}

void LineObjectTests::getTheOnlyObject() {
	line_ = 0;
	for (long i = 1; i < 1000; i++) {
		line_ = (GeometricLine*) object_container_->getObjectById(i);
		if ( line_ ) break;
	}
}

void LineObjectTests::startEnd() {
	qDebug() << "**LineObjectTests::startEnd**";

	// inject the line object
	GraphicsPointItem g_start;
	g_start.setPos(QPointF(100,150));
	sut_factory_->addPoint(&g_start);
	GraphicsPointItem g_end;
	g_end.setPos(QPointF(200,250));
	sut_factory_->addPoint(&g_end);

	// retrieve the object from the container
	getTheOnlyObject();
	CPPUNIT_ASSERT ( line_ != 0 );

	// test the values
	QPointF start = line_->getStart();
	QPointF end   = line_->getEnd();
	CPPUNIT_ASSERT ( start.rx() == 100 );
	CPPUNIT_ASSERT ( start.ry() == 150 );
	CPPUNIT_ASSERT ( end.rx() == 200 );
	CPPUNIT_ASSERT ( end.ry() == 250 );
}


void LineObjectTests::horizontalProfile() {
	qDebug() << "**LineObjectTests::horizontalProfile**";
	// inject the line object
	GraphicsPointItem g_start;
	g_start.setPos(QPointF(1,1));
	sut_factory_->addPoint(&g_start);
	GraphicsPointItem g_end;
	g_end.setPos(QPointF(7,1));
	sut_factory_->addPoint(&g_end);

	// retrieve the object from the container
	getTheOnlyObject();
	CPPUNIT_ASSERT ( line_ != 0 );

	MeasurementOnObject* measurement = line_->getMeasurement();
	CPPUNIT_ASSERT ( measurement != 0 );
	QVariant v = measurement->measurementValue(8);
	IntensityMeasurement* im;
	CPPUNIT_ASSERT (v.canConvert<IntensityMeasurement*>());
	im = v.value<IntensityMeasurement*>();
	im->computeProfile();
	QVector<QPointF>* profile = im->profile();

	CPPUNIT_ASSERT ( profile != 0 );
	CPPUNIT_ASSERT( profile->at(0).x() == 0.0 );
	CPPUNIT_ASSERT( profile->at(0).y() == 2.0 );
	CPPUNIT_ASSERT( profile->at(1).x() == 1.0 );
	CPPUNIT_ASSERT( profile->at(1).y() == 3.0 );
	CPPUNIT_ASSERT( profile->at(2).x() == 2.0 );
	CPPUNIT_ASSERT( profile->at(2).y() == 4.0 );
	CPPUNIT_ASSERT( profile->at(3).x() == 3.0 );
	CPPUNIT_ASSERT( profile->at(3).y() == 5.0 );

	CPPUNIT_ASSERT ( profile->size() == 6 );
}

void LineObjectTests::verticalProfile() {
	qDebug() << "**LineObjectTests::verticalProfile**";
	// inject the line object
	GraphicsPointItem g_start;
	g_start.setPos(QPointF(1,1));
	sut_factory_->addPoint(&g_start);
	GraphicsPointItem g_end;
	g_end.setPos(QPointF(1,7));
	sut_factory_->addPoint(&g_end);

	// retrieve the object from the container
	getTheOnlyObject();
	CPPUNIT_ASSERT ( line_ != 0 );

	MeasurementOnObject* measurement = line_->getMeasurement();
	CPPUNIT_ASSERT ( measurement != 0 );
	QVariant v = measurement->measurementValue(8);
	IntensityMeasurement* im;
	CPPUNIT_ASSERT (v.canConvert<IntensityMeasurement*>());
	im = v.value<IntensityMeasurement*>();
	im->computeProfile();
	QVector<QPointF>* profile = im->profile();

	CPPUNIT_ASSERT ( profile != 0 );
	CPPUNIT_ASSERT( profile->at(0).x() == 0.0 );
	CPPUNIT_ASSERT( profile->at(0).y() == 2.0 );
	CPPUNIT_ASSERT( profile->at(1).x() == 1.0 );
	CPPUNIT_ASSERT( profile->at(1).y() == 3.0 );
	CPPUNIT_ASSERT( profile->at(2).x() == 2.0 );
	CPPUNIT_ASSERT( profile->at(2).y() == 4.0 );
	CPPUNIT_ASSERT( profile->at(3).x() == 3.0 );
	CPPUNIT_ASSERT( profile->at(3).y() == 5.0 );

	CPPUNIT_ASSERT ( profile->size() == 6 );
}

void LineObjectTests::onePointProfile() {
	qDebug() << "**LineObjectTests::onePointProfile**";
	// inject the line object
	GraphicsPointItem g_start;
	g_start.setPos(QPointF(3,1));
	sut_factory_->addPoint(&g_start);
	GraphicsPointItem g_end;
	g_end.setPos(QPointF(3,1));
	sut_factory_->addPoint(&g_end);

	// retrieve the object from the container
	getTheOnlyObject();
	CPPUNIT_ASSERT ( line_ != 0 );

	MeasurementOnObject* measurement = line_->getMeasurement();
	CPPUNIT_ASSERT ( measurement != 0 );
	QVariant v = measurement->measurementValue(8);
	IntensityMeasurement* im;
	CPPUNIT_ASSERT (v.canConvert<IntensityMeasurement*>());
	im = v.value<IntensityMeasurement*>();
	im->computeProfile();
	QVector<QPointF>* profile = im->profile();


	// test the model values
	QPointF start = line_->getStart();
	QPointF end   = line_->getEnd();
	CPPUNIT_ASSERT ( start.rx() == 3 );
	CPPUNIT_ASSERT ( start.ry() == 1 );
	qDebug() << "**LineObjectTests::onePointProfile**"
			<< "end(" << end.rx() << " , " << end.ry() << ")";
	CPPUNIT_ASSERT ( end.rx() == 3 );
	CPPUNIT_ASSERT ( end.ry() == 1 );

	// profile will be empty since a line with length 0 does not make sense as
	// line
	CPPUNIT_ASSERT ( profile->size() == 0 );
}

void LineObjectTests::twoPointProfile() {
	qDebug() << "**LineObjectTests::twoPointProfile**";
	// inject the line object
	GraphicsPointItem g_start;
	g_start.setPos(QPointF(3,1));
	sut_factory_->addPoint(&g_start);
	GraphicsPointItem g_end;
	g_end.setPos(QPointF(2,0));
	sut_factory_->addPoint(&g_end);

	// retrieve the object from the container
	getTheOnlyObject();
	CPPUNIT_ASSERT ( line_ != 0 );

	MeasurementOnObject* measurement = line_->getMeasurement();
	CPPUNIT_ASSERT ( measurement != 0 );
	QVariant v = measurement->measurementValue(8);
	IntensityMeasurement* im;
	CPPUNIT_ASSERT (v.canConvert<IntensityMeasurement*>());
	im = v.value<IntensityMeasurement*>();
	im->computeProfile();
	QVector<QPointF>* profile = im->profile();

	// test the model values
	QPointF start = line_->getStart();
	QPointF end   = line_->getEnd();
	CPPUNIT_ASSERT ( start.rx() == 3 );
	CPPUNIT_ASSERT ( start.ry() == 1 );
	qDebug() << "**LineObjectTests::onePointProfile**"
			<< "end(" << end.rx() << " , " << end.ry() << ")";
	CPPUNIT_ASSERT ( end.rx() == 2 );
	CPPUNIT_ASSERT ( end.ry() == 0 );

	qDebug() << "**LineObjectTests::twoPointProfile**"
			<< "at(1): " << profile->at(1).x() << " " << profile->at(1).y()
			<< "len: " << profile->size();
	CPPUNIT_ASSERT ( profile != 0 );
	CPPUNIT_ASSERT ( profile->size() == 3 );
	CPPUNIT_ASSERT( profile->at(0).x() == 0.0 );
	CPPUNIT_ASSERT( profile->at(0).y() == 4.0 );
	CPPUNIT_ASSERT( profile->at(1).x() == 1.0 );
	CPPUNIT_ASSERT( profile->at(1).y() == 2.0 );
	CPPUNIT_ASSERT( profile->at(2).x() == 3.0 );
	CPPUNIT_ASSERT( profile->at(2).y() == 2.0 );

}


void LineObjectTests::diagonalProfile() {
	qDebug() << "**LineObjectTests::diagonalProfile**";
	// inject the line object
	GraphicsPointItem g_start;
	g_start.setPos(QPointF(1,1));
	sut_factory_->addPoint(&g_start);
	GraphicsPointItem g_end;
	g_end.setPos(QPointF(7,7));
	sut_factory_->addPoint(&g_end);

	// retrieve the object from the container
	getTheOnlyObject();
	CPPUNIT_ASSERT ( line_ != 0 );

	MeasurementOnObject* measurement = line_->getMeasurement();
	CPPUNIT_ASSERT ( measurement != 0 );
	QVariant v = measurement->measurementValue(8);
	IntensityMeasurement* im;
	CPPUNIT_ASSERT (v.canConvert<IntensityMeasurement*>());
	im = v.value<IntensityMeasurement*>();
	im->computeProfile();
	QVector<QPointF>* profile = im->profile();

	CPPUNIT_ASSERT ( profile != 0 );
	CPPUNIT_ASSERT( profile->at(0).x() == 0.0 );
	CPPUNIT_ASSERT( profile->at(0).y() == 2.0 );
	CPPUNIT_ASSERT( profile->at(1).x() == 1.0 );
	CPPUNIT_ASSERT( profile->at(1).y() == 2.0 );
	CPPUNIT_ASSERT( profile->at(2).x() == 2.0 );
	CPPUNIT_ASSERT( profile->at(2).y() == 4.0 );
	CPPUNIT_ASSERT( profile->at(3).x() == 3.0 );
	CPPUNIT_ASSERT( profile->at(3).y() == 6.0 );
	CPPUNIT_ASSERT( profile->at(4).x() == 4.0 );
	CPPUNIT_ASSERT( profile->at(4).y() == 8.0 );
	CPPUNIT_ASSERT( profile->at(5).x() == 5.0 );
	CPPUNIT_ASSERT( profile->at(5).y() == 8.0 );

	CPPUNIT_ASSERT( profile->at(8).x() == 8.0 );
	CPPUNIT_ASSERT( profile->at(8).y() == 12.0 );

	CPPUNIT_ASSERT ( profile->size() == 9 );
}

void LineObjectTests::lowReverseSlopeProfile() {
	qDebug() << "**LineObjectTests::lowReverseSlopeProfile**";
	// inject the line object
	GraphicsPointItem g_start;
	g_start.setPos(QPointF(7,0));
	sut_factory_->addPoint(&g_start);
	GraphicsPointItem g_end;
	g_end.setPos(QPointF(3,2));
	sut_factory_->addPoint(&g_end);

	// retrieve the object from the container
	getTheOnlyObject();
	CPPUNIT_ASSERT ( line_ != 0 );

	MeasurementOnObject* measurement = line_->getMeasurement();
	CPPUNIT_ASSERT ( measurement != 0 );
	QVariant v = measurement->measurementValue(8);
	IntensityMeasurement* im;
	CPPUNIT_ASSERT (v.canConvert<IntensityMeasurement*>());
	im = v.value<IntensityMeasurement*>();
	im->computeProfile();
	QVector<QPointF>* profile = im->profile();

	CPPUNIT_ASSERT ( profile != 0 );
	CPPUNIT_ASSERT( profile->at(0).x() == 0.0 );
	CPPUNIT_ASSERT( profile->at(0).y() == 7.0 );
	CPPUNIT_ASSERT( profile->at(1).x() == 1.0 );
	CPPUNIT_ASSERT( profile->at(1).y() == 6.0 );
	CPPUNIT_ASSERT( profile->at(2).x() == 2.0 );
	CPPUNIT_ASSERT( profile->at(2).y() == 6.0 );
	CPPUNIT_ASSERT( profile->at(3).x() == 3.0 );
	CPPUNIT_ASSERT( profile->at(3).y() == 5.0 );

	CPPUNIT_ASSERT ( profile->size() == 5 );
}

