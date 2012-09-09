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

#include "geometric_measurement_tests.h"
#include <QDebug>
#include <QVariant>
#include "measurement_on_object.h"


CPPUNIT_TEST_SUITE_REGISTRATION (GeometricMeasurementTests);

void GeometricMeasurementTests :: setUp (void)
{
	gm_ = new GeometricMeasurement();

}

void GeometricMeasurementTests :: tearDown (void)
{
	delete gm_;
}

void GeometricMeasurementTests::test_01_canonicName_Undefined (){

	MeasurementType t = GeometricMeasurement::Undefined;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "");
	 t = GeometricMeasurement::reserved2;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "");
	 t = GeometricMeasurement::reserved3;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "");
	 t = GeometricMeasurement::reserved4;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "");
}

void GeometricMeasurementTests::test_02_canonicName_Unspecific (){
	MeasurementType t = 0x80000000;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "");
}

void GeometricMeasurementTests::test_03_canonicName_SelfAndGraphic (){
	MeasurementType t = GeometricMeasurement::Self;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "geometric_measurements");
	 t = GeometricMeasurement::Graphic;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "graphics");
}

void GeometricMeasurementTests::test_04_canonicName (){
	MeasurementType t = GeometricMeasurement::GeometricMidpoint;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "midpoint");
	t = GeometricMeasurement::GeometricXMaxDimension;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "maximum_x_dimension");
	t = GeometricMeasurement::GeometricYMaxDimension;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "maximum_y_dimension");
	t = GeometricMeasurement::GeometricBoundingBox;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "bounding_box");
	t = GeometricMeasurement::GeometricMaxDimension;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "max_dim");
	t = GeometricMeasurement::GeometricMinDimension;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "min_dim");
	t = GeometricMeasurement::GeometricMaxDimensionAngle;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "max_dim_angle");
	t = GeometricMeasurement::GeometricMinDimensionAngle;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "min_dim_angle");
	t = GeometricMeasurement::GeometricCircumFerence;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "circumference");
	t = GeometricMeasurement::GeometricSurface;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "surface");
	t = GeometricMeasurement::GeometricRoundness;
	CPPUNIT_ASSERT ( gm_->canonicName(t) == "roundness");

}

void GeometricMeasurementTests::test_05_type_Undefined (){
	CPPUNIT_ASSERT ( gm_->type("") == GeometricMeasurement::Undefined);
}


void GeometricMeasurementTests::test_06_type_SelfAndGraphic (){
	CPPUNIT_ASSERT ( gm_->type("geometric_measurements") == GeometricMeasurement::Self);
	CPPUNIT_ASSERT ( gm_->type("graphics") == GeometricMeasurement::Graphic);
}

void GeometricMeasurementTests::test_07_type (){
	CPPUNIT_ASSERT ( gm_->type("midpoint") == GeometricMeasurement::GeometricMidpoint);
	CPPUNIT_ASSERT ( gm_->type("maximum_x_dimension") == GeometricMeasurement::GeometricXMaxDimension);
	CPPUNIT_ASSERT ( gm_->type("maximum_y_dimension") == GeometricMeasurement::GeometricYMaxDimension );
	CPPUNIT_ASSERT ( gm_->type("bounding_box") == GeometricMeasurement::GeometricBoundingBox );
	CPPUNIT_ASSERT ( gm_->type("max_dim") == GeometricMeasurement::GeometricMaxDimension);
	CPPUNIT_ASSERT ( gm_->type("min_dim") == GeometricMeasurement::GeometricMinDimension);
	CPPUNIT_ASSERT ( gm_->type("max_dim_angle") == GeometricMeasurement::GeometricMaxDimensionAngle);
	CPPUNIT_ASSERT ( gm_->type("min_dim_angle") == GeometricMeasurement::GeometricMinDimensionAngle);
	CPPUNIT_ASSERT ( gm_->type("surface") == GeometricMeasurement::GeometricSurface);
	CPPUNIT_ASSERT ( gm_->type("circumference") == GeometricMeasurement::GeometricCircumFerence);
	CPPUNIT_ASSERT ( gm_->type("roundness") == GeometricMeasurement::GeometricRoundness);
}

void GeometricMeasurementTests::test_08_measurementValue (){
	QVariant v;
	v = gm_->measurementValue(GeometricMeasurement::Self);
	CPPUNIT_ASSERT ( v.canConvert<GeometricMeasurement*>());
	CPPUNIT_ASSERT ( v.value<GeometricMeasurement*>() == gm_);
	CPPUNIT_ASSERT (! v.canConvert<CompositeMeasurement*>());

}
