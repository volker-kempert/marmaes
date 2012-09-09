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

#include "composite_measurement_tests.h"
#include <QDebug>
#include <QVariant>
#include "measurement_on_object.h"


CPPUNIT_TEST_SUITE_REGISTRATION (CompositeMeasurementTests);

void CompositeMeasurementTests :: setUp (void)
{
	cm_ = new CompositeMeasurement();
	gm_ = new GeometricMeasurement();
	im_ = new IntensityMeasurement();
	cm_->addMeasurement(gm_);
	cm_->addMeasurement(im_);

}

void CompositeMeasurementTests :: tearDown (void)
{
	delete cm_;
}

void CompositeMeasurementTests::test_01_canonicName_Undefined (){

	MeasurementType t = CompositeMeasurement::Undefined;
	CPPUNIT_ASSERT ( cm_->canonicName(t) == "");
}

void CompositeMeasurementTests::test_02_canonicName_Unspecific (){
	MeasurementType t = 0x80000000;
	CPPUNIT_ASSERT ( cm_->canonicName(t) == "");
}

void CompositeMeasurementTests::test_03_canonicName_SelfAndGraphic (){
	MeasurementType t = CompositeMeasurement::Self;
	CPPUNIT_ASSERT ( cm_->canonicName(t) == "measurements");
	 t = CompositeMeasurement::Graphic;
	CPPUNIT_ASSERT ( cm_->canonicName(t) == "graphics");
}

void CompositeMeasurementTests::test_04_canonicName (){
	qDebug() << "--CompositeMeasurementTests::test_04_canonicName-- at 4:"
			<< cm_->canonicName(4) << " at 8: " << cm_->canonicName(8);
	CPPUNIT_ASSERT ( cm_->canonicName(4) == "geometric_measurements");
	CPPUNIT_ASSERT ( cm_->canonicName(8) == "intensity_measurements");
}

void CompositeMeasurementTests::test_05_type_Undefined (){
	CPPUNIT_ASSERT ( cm_->type("") == CompositeMeasurement::Undefined);
}


void CompositeMeasurementTests::test_06_type_SelfAndGraphic (){
	CPPUNIT_ASSERT ( cm_->type("measurements") == CompositeMeasurement::Self);
	CPPUNIT_ASSERT ( cm_->type("graphics") == CompositeMeasurement::Graphic);
}

void CompositeMeasurementTests::test_07_type (){
	CPPUNIT_ASSERT ( cm_->type("geometric_measurements") == 4);
	CPPUNIT_ASSERT ( cm_->type("intensity_measurements") == 8);
}

void CompositeMeasurementTests::test_08_measurementValue (){
	QVariant v;
	v = cm_->measurementValue(CompositeMeasurement::Self);
	CPPUNIT_ASSERT ( v.canConvert<CompositeMeasurement*>());
	CPPUNIT_ASSERT ( v.value<CompositeMeasurement*>() == cm_);
	CPPUNIT_ASSERT (! v.canConvert<GeometricMeasurement*>());

	v = cm_->measurementValue(4);
	CPPUNIT_ASSERT ( v.canConvert<GeometricMeasurement*>());
	CPPUNIT_ASSERT ( v.value<GeometricMeasurement*>() == gm_);

	v = cm_->measurementValue(8);
	CPPUNIT_ASSERT ( v.canConvert<IntensityMeasurement*>());
	CPPUNIT_ASSERT ( v.value<IntensityMeasurement*>() == im_);
}


void CompositeMeasurementTests::test_09_availableMeasures (void) {
	qDebug() << "--CompositeMeasurementTests::test_09_availableMeasures-- "
			<< " available Measures: " << cm_->availableMeasures();
	// 1 is self (reserved) and 2 is graphic (reserved)
	// Geometry measure is coded as 4  (added first)
	// intensity measure is coded as 8 (added second)
	CPPUNIT_ASSERT ( cm_->availableMeasures() == ( 4 | 8 ));

}
