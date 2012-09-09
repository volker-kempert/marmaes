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

#ifndef HISTOGRAM_STATISTICS_H_
#define HISTOGRAM_STATISTICS_H_

#include <QVector>
#include <QPointF>
#include <stdexcept>

class NoHistogramError : public std::runtime_error
{
public:
	NoHistogramError() : runtime_error( "No Histogram Set" )  { }
};

class NoEqualSizedBinsError : public std::runtime_error
{
public:
	NoEqualSizedBinsError() :
		runtime_error( "Bins of the Histogram are of the same width" )  { }
};

class InconsistentProfileError : public std::runtime_error
{
public:
	InconsistentProfileError() : runtime_error( "Inconsistent Profile Data" )  { }
};

/* @brief
 * volatile class that does not care about the histogram life time
 * it takes a pointer to the histogram and computes statistic figures on demand
 *
 * a Histogram is is considered as a QVector of QPointF.
 * each index of the vector represents a bin; The x value of of QPointF
 * represents the mid -point of the bin the y value of QPointF is the absolute
 * number of occurrences of values falling into bin x
 *
 * The accuracy of statistical moments and ranks
 * depends on the considered bin_width;
 * that is the difference of x value of any two subsequent bins.
 *
 */
class HistogramStatistics {
public:
	HistogramStatistics();
	void setHistogram(QVector<QPointF>* histogram);
	void setPoints(QVector<QPointF>* point_set, int no_of_bins);

	double mean()   const  ;
	double variance() const    { return centralMoment(2); }
	double slope()    const    { return centralMoment(3); }

	double max() const;
	double min() const;
	double binWidth() const ;
	unsigned int numberOfBins() const ;
	double firstQuadrille() const { return quadril(0.25); }
	double thirdQuadrille() const { return quadril(0.75); }
	double median()         const { return quadril(0.50); }
	virtual ~HistogramStatistics();
private:
	double quadril(double portion) const;
	double centralMoment(int level) const;
	void computeMean();
	void computeBinWidth();
	QVector<QPointF>* histogram_;
	bool delete_histogram_;
	double normalize_correction_;
	double mean_;
	double bin_width_;
};

#endif /* HISTOGRAM_STATISTICS_H_ */
