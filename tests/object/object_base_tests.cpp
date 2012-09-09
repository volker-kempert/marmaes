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

#include "object_base_tests.h"
#include <QDebug>


CPPUNIT_TEST_SUITE_REGISTRATION (ObjectTests);

void ObjectTests :: setUp (void)
{
	gm_ = new GeometricMeasurement();
	im_ = new IntensityMeasurement();

}

void ObjectTests :: tearDown (void)
{
	delete im_;
	delete gm_;
}

void ObjectTests::test_01_canonicType (){

	MeasurementType t = MeasurementOnObject::Undefined;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "");
	CPPUNIT_ASSERT ( im_->canonicName(t) == "");
}

void ObjectTests::test_02_canonicType (){
	MeasurementType t = 0x80000000;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "");
	CPPUNIT_ASSERT ( im_->canonicName(t) == "");
}

void ObjectTests::test_03_canonicType (){
	MeasurementType t = GeometricMeasurement::GeometricSurface;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "surface");

}


void ObjectTests::test_04_canonicType (){
	MeasurementType t = IntensityMeasurement::IntensitySlope;
	CPPUNIT_ASSERT ( im_->canonicName(t) == "slope");
}

void ObjectTests::test_05_canonicType (){
	CPPUNIT_ASSERT ( im_->type("slope") == IntensityMeasurement::IntensitySlope);
}

void ObjectTests::test_06_canonicType (){
	CPPUNIT_ASSERT ( im_->type("") == MeasurementOnObject::Undefined);

}

void ObjectTests::test_07_canonicType (){
	CPPUNIT_ASSERT ( im_->type("taratata") == MeasurementOnObject::Undefined);
}


void ObjectTests::test_08_canonicType (){
	CPPUNIT_ASSERT ( gm_->type("min_dim") == GeometricMeasurement::GeometricMinDimension);
}

void ObjectTests::test_09_canonicType (){
	CPPUNIT_ASSERT ( gm_->type("") == MeasurementOnObject::Undefined);

}

void ObjectTests::test_10_canonicType (){
	CPPUNIT_ASSERT ( gm_->type("taratata") == MeasurementOnObject::Undefined);
}
