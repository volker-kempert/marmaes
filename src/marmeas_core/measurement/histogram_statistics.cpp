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

#include <QDebug>
#include <assert.h>

#include <math.h>
#include "histogram_statistics.h"

#define EPSILON 0.0000000001

HistogramStatistics::HistogramStatistics() {
	histogram_ = 0;
	delete_histogram_ = false;
}

HistogramStatistics::~HistogramStatistics() {
	if (delete_histogram_ && histogram_ != 0 ) delete histogram_;
}

/*
 * set the histogram data
 * @param histogram is a pointer to a QVector of QPointF
 *        it is assumed that the pointer points to a correct histogram
 *        a correct histogram consist of QPointF where the y coordinate is
 *        always zero or positive and the x coordinate is monoton in relation
 *        to the vector index
 */
void HistogramStatistics::setHistogram(QVector<QPointF>* histogram) {
	if ( delete_histogram_ && histogram != 0) delete histogram_;
	histogram_ = histogram;
	delete_histogram_ = false;

	normalize_correction_ = 0.0;
	for (int i = 0; i < histogram_->count(); i++)
		normalize_correction_ += histogram_->at(i).y();
	normalize_correction_ = 1 / normalize_correction_;
	computeMean();
	computeBinWidth();

	qDebug() << "--HistogramStatistics::setHistogram--: (normalized)"
			 << " Bins: " << histogram_->count() << " Mean: " << mean_;
	for (int i = 0; i < histogram_->count(); i++)
		qDebug() << "   " << histogram_->at(i);
}

unsigned int HistogramStatistics::numberOfBins() const  {
	if (!histogram_) throw NoHistogramError() ;
	unsigned int no_of_bins = 0;
	if (histogram_) no_of_bins = histogram_->count();
	return no_of_bins;
}

double HistogramStatistics::binWidth() const  {
	if (!histogram_) throw NoHistogramError() ;
	return bin_width_;
}

/**
 * @brief
 * compute the histogram data based pm 'y' values of points
 * the x values are just ignored.
 * the assignment to bins is made such that the range with equal sized bins
 * whereby the last bin takes also the highest value (adding an epsilon)
 *
 * @param points: a pointer to a QVector of QPointF
 *        it is assumed that the pointer points of  ( x, y ) pairs.
 * @param no_of_bins: how many bins will be created to calculate the histogram
 *
 */
void HistogramStatistics::setPoints(QVector<QPointF>* points, int no_of_bins)
{
	if ( no_of_bins <  1) throw InconsistentProfileError();
	if ( points == 0 ) throw InconsistentProfileError();
	if ( points->count() == 0 ) throw InconsistentProfileError();

	// compute min and max
	double min =  1.0e307;
	double max = -1.0e307;
	for (int i = 0; i < points->count(); i++) {
		if ( points->at(i).y() < min ) min = points->at(i).y();
		if ( points->at(i).y() > max ) max = points->at(i).y();
	}
	qDebug() << "--HistogramStatistics::setPoints--: "
			<<  " min: " << min <<  " max: " << max;

	if (max == min ) { bin_width_ = 0.0; no_of_bins = 1; }
	else {
		max += EPSILON; // to make sure the max is inside the last bin
		if ( no_of_bins == 1 ) bin_width_ = max - min;
		else bin_width_ = ( max - min ) /  (double) (no_of_bins  );
	}


	if ( delete_histogram_ && histogram_ != 0) delete histogram_;
	histogram_ = new QVector<QPointF>(no_of_bins);
	delete_histogram_ = true;

	// setup the zero values and bin midpoints
	for (int i = 0; i < no_of_bins; i++) {
		histogram_->replace(i,QPointF(
				min + bin_width_ * ( 0.5 +  (double)i ) , 0.0) );
	}

	// fill the histogram data
	QPointF* data = histogram_->data();
	for (int i = 0; i < points->count(); i++) {
		// figure out the bin the point belongs too
		int bin ;
		if (bin_width_ == 0.0) bin = 0;
		else bin = floor ( ( points->at(i).y() - min ) 	/ bin_width_ );

		// increase the bin collector
		// qDebug() << "--HistogramStatistics::setPoints-- update bin: " << bin;
		data[bin].ry() += 1.0 ;
	}
	normalize_correction_ = 1 / (double) points->count();
	computeMean();
}

void HistogramStatistics::computeMean() {
	mean_ = 0.0;
	for (int i = 0; i < histogram_->count(); i++)
		mean_ += histogram_->at(i).x() * histogram_->at(i).y();
	mean_ *= normalize_correction_;
}
/**
 * @brief computes the width of the bins
 *        the contract is that all bins are of the same width
 *        if this does not hold a NoEqualSizedBinsError exception is thrown
 */
void HistogramStatistics::computeBinWidth() {
	qDebug() << "--HistogramStatistics::computeBinWidth-- elems: "
			<< histogram_->count();
	if (histogram_->count() < 2) bin_width_ = 0.0;
	else {
		bin_width_ = histogram_->at(1).x() - histogram_->at(0).x();
		for (int i = 1; i < histogram_->count(); i++) {
			double bin_width = histogram_->at(i).x() - histogram_->at(i-1).x();
			if( bin_width != bin_width_) throw NoEqualSizedBinsError();
		}
	}
}

double HistogramStatistics::mean()   const  {
	if (!histogram_) throw NoHistogramError() ;
	return mean_ ;
}

double HistogramStatistics::centralMoment(int level ) const  {
	if (!histogram_) throw NoHistogramError() ;
	double moment = 0.0;
	for (int i = 0; i < histogram_->count(); i++)
		moment += pow((histogram_->at(i).x() - mean_) , level )
		           * histogram_->at(i).y();
	return moment * normalize_correction_;
}

double HistogramStatistics::min() const  {
	if (!histogram_) throw NoHistogramError() ;
	return histogram_->at(0).x();
}

double HistogramStatistics::max() const {
	if (!histogram_) throw NoHistogramError() ;
	return histogram_->at( histogram_->count() -1 ).x();
}

double HistogramStatistics::quadril(double portion) const {
	if (!histogram_) throw NoHistogramError() ;
	double no_of_median = portion / normalize_correction_;
	double count = 0.0;
	int i;
	for (i = 0; i < histogram_->count(); i++) {
		count += histogram_->at(i).y() ;
		if ( count >= no_of_median ) return histogram_->at(i).x();
	}
	return histogram_->at(i - 1).x();   // the last index ; highest bin

}

