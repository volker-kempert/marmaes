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
#include "circle_object_tests.h"

static const char * const TEST_XPM[] = {
     "8 8 15 1",

     /* monochrome definitions with rgb values*/

     "a m #000000", "b m #010101", "c m #020202", "d m #030303", "e m #040404",
     "f m #050505", "g m #060606", "h m #070707", "i m #080808", "j m #090909",
     "k m #0a0a0a", "l m #0b0b0b", "m m #0c0c0c", "n m #0d0d0d", "0 m #0e0d0d",

     /* 8x8 pixels */
     "dddedddd",
     "deefeedd",
     "defgfedd",
     "efghgfed",
     "defgfedd",
     "deefeedd",
     "dddedddd",
     "dddddddd"
};


// Mock implementations


CPPUNIT_TEST_SUITE_REGISTRATION (CircleObjectTests);

class ObjectTestReader: public ObjectBaseReader {
public:
	ObjectTestReader() : ObjectBaseReader() {}
	GeometricObject* geometricObject() { return object_; }
};

CircleObjectTests::CircleObjectTests() :
		CPPUNIT_NS :: TestFixture()  {
	qDebug() << "**CircleObjectTests::CircleObjectTests**" ;
	sut_factory_2points_ = 0;
	sut_factory_3points_ = 0;

}

CircleObjectTests::~CircleObjectTests()  {
	qDebug() << "**CircleObjectTests::~CircleObjectTests**";

	if ( sut_factory_2points_ ) delete sut_factory_2points_;
	if ( sut_factory_3points_ ) delete sut_factory_3points_;
	// ToDo nice to have unload interface
}


void CircleObjectTests::setUp() {
	qDebug() << "**CircleObjectTests::setUp**";
	if ( !sut_factory_2points_ ) {
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
		qDebug() << "--CircleObjectTests::setUp-- "
				 << "settings plugin directory: " << plugin_dir;
		manager->registerPluginDir(plugin_dir);

		interf_ = manager->plugin("libmarmeascircle.so");
		CPPUNIT_ASSERT ( interf_ );

		sut_factory_2points_  = (GeometricObjectFactory*)
						interf_->objectFactory("circle_2point");
		CPPUNIT_ASSERT ( sut_factory_2points_ );
		sut_factory_2points_->setScene(graphics_scene_);

		sut_factory_3points_  = (GeometricObjectFactory*)
						interf_->objectFactory("circle_3point");
		CPPUNIT_ASSERT ( sut_factory_3points_ );
		sut_factory_3points_->setScene(graphics_scene_);

	}
}

void CircleObjectTests::tearDown() {
	qDebug() << "**CircleObjectTests::tearDown**";
	// object_container_->setImage(0);  // reset all objects as well
}

void CircleObjectTests::getTheOnlyObject() {
	circle_ = 0;
	for (long i = 1; i < 1000; i++) {
		circle_ = (GeometricCircle*) object_container_->getObjectById(i);
		if ( circle_ ) break;
	}
}

void CircleObjectTests::geometricModel2Point() {
	qDebug() << "**CircleObjectTests::geometricModel2Point**";

	// inject the circle object
	GraphicsPointItem g_start;
	g_start.setPos(QPointF(100,150));
	sut_factory_2points_->addPoint(&g_start);
	GraphicsPointItem g_end;
	g_end.setPos(QPointF(100,160));
	sut_factory_2points_->addPoint(&g_end);

	// retrieve the object from the container
	getTheOnlyObject();
	CPPUNIT_ASSERT ( circle_ != 0 );

	// test the values
	QPointF midpoint = circle_->getMidpoint();
	double radius   = circle_->getRadius();
	qDebug() << "**CircleObjectTests::geometricModel2Point**"
			<< " midpoint: (" << midpoint.rx() << "," << midpoint.ry() << ")"
			<< " radius: " << radius;
	CPPUNIT_ASSERT ( midpoint.rx() == 100 );
	CPPUNIT_ASSERT ( midpoint.ry() == 150 );
	CPPUNIT_ASSERT ( radius == 10 );
}


void CircleObjectTests::geometricModel3Point() {
	qDebug() << "**CircleObjectTests::geometricModel3Point**";
	// inject the circle object
	GraphicsPointItem p1;
	p1.setPos(QPointF(100,10));
	sut_factory_3points_->addPoint(&p1);
	GraphicsPointItem p2;
	p2.setPos(QPointF(100,90));
	sut_factory_3points_->addPoint(&p2);
	GraphicsPointItem p3;
	p3.setPos(QPointF(60,50));
	sut_factory_3points_->addPoint(&p3);


	// retrieve the object from the container
	getTheOnlyObject();
	CPPUNIT_ASSERT ( circle_ != 0 );

	// test the values
	QPointF midpoint = circle_->getMidpoint();
	double radius   = circle_->getRadius();
	qDebug() << "**CircleObjectTests::geometricModel3Point**"
			<< " midpoint: (" << midpoint.rx() << "," << midpoint.ry() << ")"
			<< " radius: " << radius;
	CPPUNIT_ASSERT ( midpoint.rx() == 100 );
	CPPUNIT_ASSERT ( midpoint.ry() == 50 );
	CPPUNIT_ASSERT ( radius == 40 );
}

void CircleObjectTests::zeroPointHistogram() {
	qDebug() << "**CircleObjectTests::zeroPointHistogram**";
	// inject the circle object
	GraphicsPointItem p1;
	p1.setPos(QPointF(3,3));
	sut_factory_2points_->addPoint(&p1);
	GraphicsPointItem p2;
	p2.setPos(QPointF(3,3));
	sut_factory_2points_->addPoint(&p2);

	// retrieve the object from the container
	getTheOnlyObject();
	CPPUNIT_ASSERT ( circle_ != 0 );

	MeasurementOnObject* measurement = circle_->getMeasurement();
	CPPUNIT_ASSERT ( measurement != 0 );
	QVariant v = measurement->measurementValue(8);
	IntensityMeasurement* im;
	CPPUNIT_ASSERT (v.canConvert<IntensityMeasurement*>());
	im = v.value<IntensityMeasurement*>();
	im->computeHistogram();
	QVector<QPointF>* histogram = im->histogram();

	CPPUNIT_ASSERT ( histogram != 0 );
	CPPUNIT_ASSERT ( histogram->size() == 256 );
	qDebug() << "**CircleObjectTests::zeroPointHistogram**"
			<< " at100: " << histogram->at(100).x()
			<< " -> " << histogram->at(100).y();
	CPPUNIT_ASSERT( histogram->at(100).x() == 100.0 );
	CPPUNIT_ASSERT( histogram->at(100).y() == 0.0 );

}

void CircleObjectTests::onePointHistogram() {
	qDebug() << "**CircleObjectTests::onePointHistogram**";
	// inject the circle object
	GraphicsPointItem p1;
	p1.setPos(QPointF(3,3));
	sut_factory_2points_->addPoint(&p1);
	GraphicsPointItem p2;
	p2.setPos(QPointF(4,3));
	sut_factory_2points_->addPoint(&p2);

	// retrieve the object from the container
	getTheOnlyObject();
	CPPUNIT_ASSERT ( circle_ != 0 );

	MeasurementOnObject* measurement = circle_->getMeasurement();
	CPPUNIT_ASSERT ( measurement != 0 );
	QVariant v = measurement->measurementValue(8);
	IntensityMeasurement* im;
	CPPUNIT_ASSERT (v.canConvert<IntensityMeasurement*>());
	im = v.value<IntensityMeasurement*>();
	im->computeHistogram();
	QVector<QPointF>* histogram = im->histogram();

	CPPUNIT_ASSERT ( histogram != 0 );
	CPPUNIT_ASSERT ( histogram->size() == 256 );
	CPPUNIT_ASSERT( histogram->at(100).x() == 100.0 );
	CPPUNIT_ASSERT( histogram->at(100).y() == 0.0 );

	qDebug() << "**CircleObjectTests::onePointHistogram**"
			<< " at 0: " << histogram->at(0).y()
			<< " at 1: " << histogram->at(1).y()
			<< " at 2: " << histogram->at(2).y()
			<< " at 3: " << histogram->at(3).y()
			<< " at 4: " << histogram->at(4).y()
			<< " at 5: " << histogram->at(5).y()
			<< " at 6: " << histogram->at(6).y()
			<< " at 7: " << histogram->at(7).y();
	CPPUNIT_ASSERT( histogram->at(7).x() == 7.0 );
	CPPUNIT_ASSERT( histogram->at(7).y() == 1.0 );
	CPPUNIT_ASSERT( histogram->at(6).x() == 6.0 );
	CPPUNIT_ASSERT( histogram->at(6).y() == 0.0 );
	CPPUNIT_ASSERT( histogram->at(5).x() == 5.0 );
	CPPUNIT_ASSERT( histogram->at(5).y() == 0.0 );
	CPPUNIT_ASSERT( histogram->at(4).x() == 4.0 );
	CPPUNIT_ASSERT( histogram->at(4).y() == 0.0 );

}

void CircleObjectTests::toTheEdgeHistogram() {
	qDebug() << "**CircleObjectTests::toTheEdgeHistogram**";
	// inject the circle object
	GraphicsPointItem p1;
	p1.setPos(QPointF(3,3));
	sut_factory_2points_->addPoint(&p1);
	GraphicsPointItem p2;
	p2.setPos(QPointF(0,3));
	sut_factory_2points_->addPoint(&p2);

	// retrieve the object from the container
	getTheOnlyObject();
	CPPUNIT_ASSERT ( circle_ != 0 );

	MeasurementOnObject* measurement = circle_->getMeasurement();
	CPPUNIT_ASSERT ( measurement != 0 );
	QVariant v = measurement->measurementValue(8);
	IntensityMeasurement* im;
	CPPUNIT_ASSERT (v.canConvert<IntensityMeasurement*>());
	im = v.value<IntensityMeasurement*>();
	im->computeHistogram();
	QVector<QPointF>* histogram = im->histogram();

	CPPUNIT_ASSERT ( histogram != 0 );
	CPPUNIT_ASSERT ( histogram->size() == 256 );
	CPPUNIT_ASSERT( histogram->at(100).x() == 100.0 );
	CPPUNIT_ASSERT( histogram->at(100).y() == 0.0 );

	qDebug() << "**CircleObjectTests::toTheEdgeHistogram**"
			<< " at 0: " << histogram->at(0).y()
			<< " at 1: " << histogram->at(1).y()
			<< " at 2: " << histogram->at(2).y()
			<< " at 3: " << histogram->at(3).y()
			<< " at 4: " << histogram->at(4).y()
			<< " at 5: " << histogram->at(5).y()
			<< " at 6: " << histogram->at(6).y()
			<< " at 7: " << histogram->at(7).y();
	CPPUNIT_ASSERT( histogram->at(7).x() == 7.0 );
	CPPUNIT_ASSERT( histogram->at(7).y() == 1.0 );
	CPPUNIT_ASSERT( histogram->at(6).x() == 6.0 );
	CPPUNIT_ASSERT( histogram->at(6).y() == 4.0 );
	CPPUNIT_ASSERT( histogram->at(5).x() == 5.0 );
	CPPUNIT_ASSERT( histogram->at(5).y() == 8.0 );
	CPPUNIT_ASSERT( histogram->at(4).x() == 4.0 );
	CPPUNIT_ASSERT( histogram->at(4).y() == 12.0 );

}


void CircleObjectTests::radius3Histogram() {
	qDebug() << "**CircleObjectTests::radius3Histogram**";
	// inject the circle object
	GraphicsPointItem p1;
	p1.setPos(QPointF(3,3));
	sut_factory_2points_->addPoint(&p1);
	GraphicsPointItem p2;
	p2.setPos(QPointF(1,3));
	sut_factory_2points_->addPoint(&p2);

	// retrieve the object from the container
	getTheOnlyObject();
	CPPUNIT_ASSERT ( circle_ != 0 );

	MeasurementOnObject* measurement = circle_->getMeasurement();
	CPPUNIT_ASSERT ( measurement != 0 );
	QVariant v = measurement->measurementValue(8);
	IntensityMeasurement* im;
	CPPUNIT_ASSERT (v.canConvert<IntensityMeasurement*>());
	im = v.value<IntensityMeasurement*>();
	im->computeHistogram();
	QVector<QPointF>* histogram = im->histogram();

	CPPUNIT_ASSERT ( histogram != 0 );
	CPPUNIT_ASSERT ( histogram->size() == 256 );
	CPPUNIT_ASSERT( histogram->at(100).x() == 100.0 );
	CPPUNIT_ASSERT( histogram->at(100).y() == 0.0 );

	CPPUNIT_ASSERT( histogram->at(7).x() == 7.0 );
	CPPUNIT_ASSERT( histogram->at(7).y() == 1.0 );
	CPPUNIT_ASSERT( histogram->at(6).x() == 6.0 );
	CPPUNIT_ASSERT( histogram->at(6).y() == 4.0 );
	CPPUNIT_ASSERT( histogram->at(5).x() == 5.0 );
	CPPUNIT_ASSERT( histogram->at(5).y() == 4.0 );
	CPPUNIT_ASSERT( histogram->at(4).x() == 4.0 );
	CPPUNIT_ASSERT( histogram->at(4).y() == 0.0 );

}

