/*
   Copyright (C) 2012, Volker Kempert <volker.kempert@gmx.net>

   This file is part of an MARMEAS library

   The MARMEAS library is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of
   the License, or (at your option) any later version.

   The MARMEAS library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with MARMEAS. If not, see <http://www.gnu.org/licenses/>.

*/

#include <assert.h>
#include <stdexcept>
#include <QDebug>
#include <QMenu>
#include <QAction>


#include "measurement_on_object.h"


GeometricMeasurement::GeometricMeasurement() : MeasurementOnObject() {
	available_measures_ = GeometricMeasurement::AllMeasures;
}

GeometricMeasurement::~GeometricMeasurement() {
}


QString GeometricMeasurement::measurementName(MeasurementType m) const {
	QString ret = tr("anything-else");
	qDebug() << "--GeometricMeasurement::measurementName-- type: " << m;
	switch ( m ) {
	case GeometricMeasurement::Self  :
		ret = tr("Geometric measurements");
		break;
	// geometry related
	case GeometricMeasurement::GeometricMidpoint  :
		ret = tr("Midpoint of Object");
		break;
	case GeometricMeasurement::GeometricXMaxDimension  :
		ret = tr("Maximum X Dimension");
		break;
	case GeometricMeasurement::GeometricYMaxDimension  :
		ret = tr("Maximum Y Dimension");
		break;
	case GeometricMeasurement::GeometricBoundingBox  :
		ret = tr("Bounding Box");
		break;
	case GeometricMeasurement::GeometricMaxDimension  :
		ret = tr("Maximum Dimension (after PCA)");
		break;
	case GeometricMeasurement::GeometricMinDimension  :
		ret = tr("Minimum Dimension (after PCA)");
		break;
	case GeometricMeasurement::GeometricMaxDimensionAngle  :
		ret = tr("Maximum Dimension Angle (after PCA");
		break;
	case GeometricMeasurement::GeometricMinDimensionAngle  :
		ret = tr("Minimum Dimension Angle (after PCA");
		break;
	case GeometricMeasurement::GeometricSurface  :
		ret = tr("Surface Area");
		break;
	case GeometricMeasurement::GeometricCircumFerence  :
		ret = tr("Circumference");
		break;
	case GeometricMeasurement::GeometricRoundness  :
		ret = tr("Roundness");
		break;

	default:
		qDebug() << "--GeometricMeasurement::measurementName-- "
				 << "Unhandled measurement type" << m;
		throw std::logic_error("--GeometricMeasurement::measurementName--"
				" Unhandled  measurement type");
	}
	return ret;
}


QVariant GeometricMeasurement::measurementValue(MeasurementType m) const  {
	qDebug() << "--GeometricMeasurement::measurementValue-- type: " << m;
	if  ( m == GeometricMeasurement::Self ) {
		// next line does not work: see
		// http://developer.qt.nokia.com/forums/viewthread/4636
		//return QVariant::fromValue<GeometricMeasurement*>(this);
		return qVariantFromValue(const_cast<GeometricMeasurement*>(this));
	}

	qDebug() << "--GeometricMeasurement::measurementValue-- "
			 << "Unhandled measurement type" << m;
	throw std::logic_error("--GeometricMeasurement::measurementValue--"
			" Unhandled  measurement type");
}

QString GeometricMeasurement::canonicName(MeasurementType m) const {
	QString ret = tr("");
	qDebug() << "--GeometricMeasurement::canonicName-- type: " << m;
	switch ( m ) {
	case GeometricMeasurement::Self  :
		ret = "geometric_measurements"; break;
	case GeometricMeasurement::Graphic  :
		ret = "graphics"; break;

	case GeometricMeasurement::GeometricMidpoint  :
		ret = "midpoint"; break;
	case GeometricMeasurement::GeometricXMaxDimension  :
		ret = "maximum_x_dimension"; break;
	case GeometricMeasurement::GeometricYMaxDimension  :
		ret = "maximum_y_dimension"; 	break;
	case GeometricMeasurement::GeometricBoundingBox  :
		ret = "bounding_box"; 	break;
	case GeometricMeasurement::GeometricMaxDimension  :
		ret = "max_dim"; break;
	case GeometricMeasurement::GeometricMinDimension  :
		ret = "min_dim"; break;
	case GeometricMeasurement::GeometricMaxDimensionAngle  :
		ret = "max_dim_angle"; break;
	case GeometricMeasurement::GeometricMinDimensionAngle  :
		ret = "min_dim_angle"; break;
	case GeometricMeasurement::GeometricSurface  :
		ret = "surface"; 	break;
	case GeometricMeasurement::GeometricCircumFerence  :
		ret = "circumference"; 	break;
	case GeometricMeasurement::GeometricRoundness  :
		ret = "roundness"; 	break;


	default:
		qDebug() << "--GeometricMeasurement::canonicName-- "
				 << "Unhandled measurement type" << m;
		//throw std::logic_error("--GeometricMeasurement::canonicName--"
		//		" Unhandled  measurement type");
		ret = ""; 	break;
	}
	return ret;
}




