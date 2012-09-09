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

#include "histogram_statistics.h"
#include "measurement_on_object.h"
#include "measurement/quartile.h"



IntensityMeasurement::IntensityMeasurement() : MeasurementOnObject() {
	qDebug() << "--IntensityMeasurement::IntensityMeasurement--";
	histogram_ = 0;
	profile_ = 0;
	available_measures_ = IntensityMeasurement::AllMeasures;
}

IntensityMeasurement::~IntensityMeasurement() {
	qDebug() << "--IntensityMeasurement::~IntensityMeasurement--";
	if (histogram_) delete histogram_;
	if (profile_)   delete profile_;
}


QString IntensityMeasurement::measurementName(MeasurementType m) const {
	QString ret = tr("anything-else");
	qDebug() << "--IntensityMeasurement::measurementName-- type: " << m;
	switch ( m  ) {
	case IntensityMeasurement::Self  :
		ret = tr("Intensity measurements");
		break;
	case IntensityMeasurement::Graphic  :
		ret = tr("Graphic Plot");
		break;

	case IntensityMeasurement::IntensityMean  :
		ret = tr("Mean");
		break;
	case IntensityMeasurement::IntensityVariance  :
		ret = tr("Variance");
		break;
	case IntensityMeasurement::IntensitySlope  :
		ret = tr("Slope");
		break;
	case IntensityMeasurement::IntensityMin  :
		ret = tr("Minimum");
		break;
	case IntensityMeasurement::IntensityMax  :
		ret = tr("Maximum");
		break;
	case IntensityMeasurement::IntensityMedian  :
		ret = tr("Median");
		break;
	case IntensityMeasurement::IntensityCenterOfGravity  :
		ret = tr("Center of Gravity");
		break;
	case IntensityMeasurement::IntensityLinearFit  :
		ret = tr("Linear Fitting");
		break;
	case IntensityMeasurement::IntensityQuartiles  :
		ret = tr("Quartiles");
		break;


	// there must be something
	default:
		qDebug() << "--IntensityMeasurement::measurementName-- "
				 << "Unhandled measurement type" << m;
		throw std::logic_error("--IntensityMeasurement::measurementName-- Unhandled  measurement type");
	}
	return ret;
}

QString IntensityMeasurement::canonicName(MeasurementType m) const {
	QString ret = "";
	qDebug() << "--IntensityMeasurement::canonicName-- type: " << m;
	switch ( m  ) {
	case IntensityMeasurement::Self  :
		ret = "intensity_measurements"; 	break;
	case IntensityMeasurement::Graphic  :
		ret = "graphics"; 	break;

	case IntensityMeasurement::IntensityMean  :
		ret = "mean"; 	break;
	case IntensityMeasurement::IntensityVariance  :
		ret = "variance"; break;
	case IntensityMeasurement::IntensitySlope  :
		ret = "slope"; 	break;
	case IntensityMeasurement::IntensityMin  :
		ret = "minimum"; break;
	case IntensityMeasurement::IntensityMax  :
		ret = "maximum"; break;
	case IntensityMeasurement::IntensityMedian  :
		ret = "median"; break;
	case IntensityMeasurement::IntensityCenterOfGravity  :
		ret = "center_of_gravity"; 	break;
	case IntensityMeasurement::IntensityLinearFit  :
		ret = "linear_fitting"; 	break;
	case IntensityMeasurement::IntensityQuartiles  :
		ret = "quartiles"; 	break;

	default:
		qDebug() << "--IntensityMeasurement::canonicName-- "
				 << "Unhandled measurement type" << m;
		//throw std::logic_error("--IntensityMeasurement::canonicName--"
		//		" Unhandled  measurement type");
		ret = ""; 	break;
	}
	return ret;
}



/**
 * @brief: figure out which distribution data set is to be used
 *         if none a zero pointer is used
 * @return: the pointer to a histogram statistics distribution
 *          the consumer of this return object
 *          is responsible for object deletion after consuming
 */
HistogramStatistics* IntensityMeasurement::determineDistribution(MeasurementType m) const {
	HistogramStatistics* histogram = 0;
	qDebug() << "--IntensityMeasurement::determineDistribution-- " << m;
	if (profile_) {
			qDebug() << "--IntensityMeasurement::determineDistribution-- Profile ";
			histogram = new HistogramStatistics();
			histogram->setPoints(profile_, 256);
	}
	if (histogram_) {
			qDebug() << "--IntensityMeasurement::determineDistribution-- Histogram ";
			histogram = new HistogramStatistics();
			qDebug() << "--IntensityMeasurement::determineDistribution-2 ";
			histogram->setHistogram(histogram_);
			qDebug() << "--IntensityMeasurement::determineDistribution-3 ";
	}
	qDebug() << "--IntensityMeasurement::determineDistribution-- addr: "
			<< (long) histogram;
	return histogram;
}
/**
 * @return: delivers always a value if nothing given QVariant (double,0)
 *          is returned.
 *          by checking availableMeasurmentType it has to made sure
 *          that the return is indeed meaningful values of 0.0 and not
 *          accidentyl 0.0 tha do not mean anything because the measure is not
 *          defined
 *
 */
QVariant IntensityMeasurement::measurementValue(MeasurementType m) const  {
	qDebug() << "--IntensityMeasurement::measurementValue-- called";
	if  ( m == CompositeMeasurement::Self )
		// next line does not work: see
		// http://developer.qt.nokia.com/forums/viewthread/4636
		// return QVariant::fromValue<IntensityMeasurementStar>(this);
		return qVariantFromValue(const_cast<IntensityMeasurement*>(this));

	HistogramStatistics* histogram = determineDistribution(m);
	double ret_val = 0.0;
	qDebug() << "--IntensityMeasurement::measurementValue-- "
			<< " type: " <<  m
			<< " hist addr: " << (long) histogram;
	if (histogram) {
		switch ( m ) {
		case 	IntensityMean:
			ret_val =  histogram->mean();
			break;
		case 	IntensityVariance:
			ret_val =  histogram->variance();
			break;
		case	IntensitySlope:
			ret_val =  histogram->slope();
			break;
		case	IntensityMin :
			ret_val =  histogram->min();
			break;
		case 	IntensityMax :
			ret_val =  histogram->max();
			break;
		case	IntensityMedian:
			ret_val =  histogram->median();
			break;
		case	IntensityQuartiles:
		{
			Quartile q(histogram->min(),
					   histogram->firstQuadrille(),
					   histogram->median(),
					   histogram->thirdQuadrille(),
					   histogram->max()) ;
			QVariant v;
			v.setValue<Quartile>(q);
			return v;
		}
		default:
			// provide anything at all
			return QVariant((double)(0.0));
		}
		delete histogram;
	}
	return QVariant(ret_val);
}

/**
 * @brief slot to make sure internally any measurement cache is updated
 *        or (if not done initially set); this holds specifically for
 *        intensity measures
 */
void IntensityMeasurement::updateMeasures() {
	computeHistogram();
	computeProfile();
}
