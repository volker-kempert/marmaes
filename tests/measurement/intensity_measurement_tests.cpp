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

#include "intensity_measurement_tests.h"
#include <QDebug>
#include <QVariant>
#include "measurement_on_object.h"


CPPUNIT_TEST_SUITE_REGISTRATION (IntensityMeasurementTests);

void IntensityMeasurementTests :: setUp (void)
{
	im_ = new IntensityMeasurement();

}

void IntensityMeasurementTests :: tearDown (void)
{
	delete im_;
}

void IntensityMeasurementTests::test_01_canonicName_Undefined (){

	MeasurementType t = IntensityMeasurement::Undefined;
	CPPUNIT_ASSERT ( im_->canonicName(t) == "");

}

void IntensityMeasurementTests::test_02_canonicName_Unspecific (){
	MeasurementType t = 0x80000000;
	CPPUNIT_ASSERT ( im_->canonicName(t) == "");
}

void IntensityMeasurementTests::test_03_canonicName_SelfAndGraphic (){
	MeasurementType t = IntensityMeasurement::Self;
	CPPUNIT_ASSERT ( im_->canonicName(t) == "intensity_measurements");
	 t = IntensityMeasurement::Graphic;
	CPPUNIT_ASSERT ( im_->canonicName(t) == "graphics");
}

void IntensityMeasurementTests::test_04_canonicName (){
	MeasurementType t = IntensityMeasurement::IntensityMean;
	CPPUNIT_ASSERT ( im_->canonicName(t) == "mean");
	t = IntensityMeasurement::IntensityVariance;
	CPPUNIT_ASSERT ( im_->canonicName(t) == "variance");
	t = IntensityMeasurement::IntensitySlope;
	CPPUNIT_ASSERT ( im_->canonicName(t) == "slope");
	t = IntensityMeasurement::IntensityMin;
	CPPUNIT_ASSERT ( im_->canonicName(t) == "minimum");
	t = IntensityMeasurement::IntensityMax ;
	CPPUNIT_ASSERT ( im_->canonicName(t) == "maximum");
	t = IntensityMeasurement::IntensityMedian;
	CPPUNIT_ASSERT ( im_->canonicName(t) == "median");
	t = IntensityMeasurement::IntensityCenterOfGravity;
	CPPUNIT_ASSERT ( im_->canonicName(t) == "center_of_gravity");
	t = IntensityMeasurement::IntensityLinearFit ;
	CPPUNIT_ASSERT ( im_->canonicName(t) == "linear_fitting");
	t = IntensityMeasurement::IntensityQuartiles;
	CPPUNIT_ASSERT ( im_->canonicName(t) == "quartiles");

}

void IntensityMeasurementTests::test_05_type_Undefined (){
	CPPUNIT_ASSERT ( im_->type("") == IntensityMeasurement::Undefined);
}


void IntensityMeasurementTests::test_06_type_SelfAndGraphic (){
	CPPUNIT_ASSERT ( im_->type("intensity_measurements") == IntensityMeasurement::Self);
	CPPUNIT_ASSERT ( im_->type("graphics") == IntensityMeasurement::Graphic);
}

void IntensityMeasurementTests::test_07_type (){
	qDebug() << "--IntensityMeasurementTests::test_07_type-- mean:"
			<< im_->type("mean");
	CPPUNIT_ASSERT ( im_->type("mean") == IntensityMeasurement::IntensityMean);
	CPPUNIT_ASSERT ( im_->type("variance") == IntensityMeasurement::IntensityVariance );
	CPPUNIT_ASSERT ( im_->type("slope") == IntensityMeasurement::IntensitySlope );
	CPPUNIT_ASSERT ( im_->type("minimum") == IntensityMeasurement::IntensityMin);
	CPPUNIT_ASSERT ( im_->type("maximum") == IntensityMeasurement::IntensityMax);
	CPPUNIT_ASSERT ( im_->type("median") == IntensityMeasurement::IntensityMedian);
	CPPUNIT_ASSERT ( im_->type("center_of_gravity") == IntensityMeasurement::IntensityCenterOfGravity);
	CPPUNIT_ASSERT ( im_->type("linear_fitting") == IntensityMeasurement::IntensityLinearFit);
	CPPUNIT_ASSERT ( im_->type("quartiles") == IntensityMeasurement::IntensityQuartiles);
}

void IntensityMeasurementTests::test_08_measurementValue (){
	QVariant v;
	v = im_->measurementValue(IntensityMeasurement::Self);
	CPPUNIT_ASSERT ( v.canConvert<IntensityMeasurement*>());
	CPPUNIT_ASSERT ( v.value<IntensityMeasurement*>() == im_);
	CPPUNIT_ASSERT (! v.canConvert<CompositeMeasurement*>());

}
