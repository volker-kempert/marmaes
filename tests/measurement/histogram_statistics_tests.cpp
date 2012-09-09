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
#include <cmath>

#include "histogram_statistics_tests.h"
#include "arbitrary_math.h"


CPPUNIT_TEST_SUITE_REGISTRATION (HistogramStatisticsTests);

static QVector<QPointF> DATA_ONE_POINT(1, QPointF(1.0,1.0));
static QVector<QPointF> DATA_TWO_POINTS(2, QPointF(0.0,0.0));
static QVector<QPointF> DATA_SYM_THREE(3,QPointF(0.0,0.0));
static QVector<QPointF> DATA_THE_SAME(5, QPointF(-1.0,-1.0));
static QVector<QPointF> DATA_PERIODIC(9);
static QVector<QPointF> DATA_NO_HIST(4);


void HistogramStatisticsTests::setUp() {
	if ( DATA_TWO_POINTS.at(0).x() == 0.0) {
		QPointF p1(1.0, 1.0);
		QPointF p2(2.0, 2.0);
		QPointF p3(3.0, 3.0);
		QPointF p4(4.0, 4.0);
		QPointF p5(5.0, 5.0);

		QPointF p6(3.0, 1.0);

		DATA_TWO_POINTS.replace(0,p1);
		DATA_TWO_POINTS.replace(1,p2);

		DATA_SYM_THREE.replace(0,p1);
		DATA_SYM_THREE.replace(1,p2);
		DATA_SYM_THREE.replace(2,p6);

		DATA_NO_HIST.replace(0,p1);
		DATA_NO_HIST.replace(1,p2);
		DATA_NO_HIST.replace(2,p4);
		DATA_NO_HIST.replace(3,p5);

		DATA_PERIODIC.replace(0,p1);
		DATA_PERIODIC.replace(1,p2);
		DATA_PERIODIC.replace(2,p3);
		DATA_PERIODIC.replace(3,p1);
		DATA_PERIODIC.replace(4,p2);
		DATA_PERIODIC.replace(5,p3);
		DATA_PERIODIC.replace(6,p1);
		DATA_PERIODIC.replace(7,p2);
		DATA_PERIODIC.replace(8,p3);
	}
}

void HistogramStatisticsTests::tearDown() {
}

//-----------------------------------------------------------------------------

void HistogramStatisticsTests::doubleRounding(void) {
	CPPUNIT_ASSERT(round(21.0 , 0) == 21.0);
	CPPUNIT_ASSERT(round(1.4 , 0) == 1.0);
	CPPUNIT_ASSERT(round(10.6 , 0) == 11.0);

	CPPUNIT_ASSERT(round(110.0  , 1) == 110.0);
	CPPUNIT_ASSERT(round(9.13   , 1) == 10.0);
	CPPUNIT_ASSERT(round(3043.6 , 1) == 3040.0);

	CPPUNIT_ASSERT(round(0.1  , -1) == 0.1);
	CPPUNIT_ASSERT(round(0.13 , -1) == 0.1);
	CPPUNIT_ASSERT(round(0.08 , -1) == 0.1);

	CPPUNIT_ASSERT(round(0.01  , -2) == 0.01);
	CPPUNIT_ASSERT(round(0.013 , -2) == 0.01);
	CPPUNIT_ASSERT(round(-0.008 , -2) == -0.01);
	CPPUNIT_ASSERT(round(-0.013 , -2) == -0.01);

	// does not work so far!!
	//qDebug() << "10.13: " << round(10.13 , -1);
	//CPPUNIT_ASSERT(round(10.13 , -1) == round(10.10 , -1));
	//CPPUNIT_ASSERT(round(-10.13 , -1) == -10.1);

	//CPPUNIT_ASSERT(round(1.13 , -1) == 1.1);
	//CPPUNIT_ASSERT(round(-1.13 , -1) == -1.1);

}

//-----------------------------------------------------------------------------

void HistogramStatisticsTests::noHistogramMeanThrow(void) {
	HistogramStatistics sut;
	double d = sut.mean();
}

void HistogramStatisticsTests::noHistogramVarianceThrow(void) {
	HistogramStatistics sut;
	double d = sut.variance();
}

void HistogramStatisticsTests::noHistogramMaxThrow(void) {
	HistogramStatistics sut;
	double d = sut.max();
}

void HistogramStatisticsTests::noHistogramMinThrow(void) {
	HistogramStatistics sut;
	double d = sut.min();
}

void HistogramStatisticsTests::noHistogramMedianThrow(void) {
	HistogramStatistics sut;
	double d = sut.median();
}

void HistogramStatisticsTests::noHistogramNumberOfBinsThrow(void) {
	HistogramStatistics sut;
	unsigned int bins = sut.numberOfBins();
}

void HistogramStatisticsTests::noHistogramBinWidthThrow(void) {
	HistogramStatistics sut;
	double d = sut.binWidth();
}

void HistogramStatisticsTests::NoEqualSizedBinsThrow(void) {
	HistogramStatistics sut;
	sut.setHistogram(&DATA_NO_HIST);
}

//-----------------------------------------------------------------------------

void  HistogramStatisticsTests::profileZeroThrow (void) {
	qDebug() << "**HistogramStatisticsTests::profileEmpty**" ;
	QVector<QPointF> empty(0);
	HistogramStatistics sut;
	sut.setPoints(0 , 1);
}

void  HistogramStatisticsTests::profileEmptyThrow (void) {
	qDebug() << "**HistogramStatisticsTests::profileEmpty**" ;
	QVector<QPointF> empty(0);
	HistogramStatistics sut;
	sut.setPoints(&empty, 1) ;
}

void  HistogramStatisticsTests::profileWrongBinsThrow (void) {
	qDebug() << "**HistogramStatisticsTests::profileEmpty**" ;
	QVector<QPointF> empty(0);
	HistogramStatistics sut;
	sut.setPoints(&DATA_NO_HIST , 0);
}
//-----------------------------------------------------------------------------

void  HistogramStatisticsTests::profileOnePoint (void) {
	qDebug() << "**HistogramStatisticsTests::profileOnePoint**" ;
	HistogramStatistics sut;
	sut.setPoints(&DATA_ONE_POINT, 1 );
	CPPUNIT_ASSERT(sut.mean() == 1.0 );
	CPPUNIT_ASSERT(sut.binWidth() == 0.0 );
	CPPUNIT_ASSERT(sut.numberOfBins() == 1 );
}

void  HistogramStatisticsTests::profileOnePointCorrectBins (void) {
	qDebug() << "**HistogramStatisticsTests::profileOnePointCorrectBins**" ;
	HistogramStatistics sut;
	sut.setPoints(&DATA_ONE_POINT, 2 );
	CPPUNIT_ASSERT(sut.numberOfBins() == 1 );
	CPPUNIT_ASSERT(sut.binWidth() == 0.0 );
	CPPUNIT_ASSERT(sut.mean() == 1.0 );
}

void  HistogramStatisticsTests::profileOneBinTwoPoints(void) {
	qDebug() << "**HistogramStatisticsTests::profileOneBinTwoPoints**" ;
	HistogramStatistics sut;
	sut.setPoints(&DATA_TWO_POINTS, 1 );
	qDebug() << "round: " << round(sut.mean(),-1);
	CPPUNIT_ASSERT(std::abs(sut.mean() - 1.5 ) < 0.000001 );
	CPPUNIT_ASSERT(sut.numberOfBins() == 1 );
	CPPUNIT_ASSERT(std::abs(sut.binWidth() - 1.0) < 0.0000001 );
}

void  HistogramStatisticsTests::profileOneBinSameX (void) {
	qDebug() << "**HistogramStatisticsTests::profileOneBinSameX**" ;
	HistogramStatistics sut;
	sut.setPoints(&DATA_THE_SAME, 4 );
	CPPUNIT_ASSERT(sut.mean() == -1.0);
	CPPUNIT_ASSERT(sut.numberOfBins() == 1 );
	CPPUNIT_ASSERT(sut.binWidth() == 0.0 );
}

void  HistogramStatisticsTests::profileTwoBins (void) {
	qDebug() << "**HistogramStatisticsTests::profileTwoBins**" ;
	HistogramStatistics sut;
	sut.setPoints(&DATA_TWO_POINTS, 2 );
	CPPUNIT_ASSERT(abs(sut.mean() - 1.5 ) < 0.000001 );
	CPPUNIT_ASSERT(sut.numberOfBins() == 2 );
	CPPUNIT_ASSERT(abs(sut.binWidth() - 0.5) < 0.0000001 );
}

void  HistogramStatisticsTests::profileThreeBins (void) {
	qDebug() << "**HistogramStatisticsTests::profileThreeBins**" ;
	HistogramStatistics sut;
	sut.setPoints(&DATA_PERIODIC, 3 );
	CPPUNIT_ASSERT(2.0000 == round(sut.mean(),0));
}
//-----------------------------------------------------------------------------

void  HistogramStatisticsTests::histogramMeanOneBin (void) {
	qDebug() << "**HistogramStatisticsTests::histogramMeanOneBin**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_ONE_POINT);
	CPPUNIT_ASSERT(sut.mean() == 1.0 );

}

void  HistogramStatisticsTests::histogramVarianceOneBin (void) {
	qDebug() << "**HistogramStatisticsTests::histogramVarianceOneBin**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_ONE_POINT);
	CPPUNIT_ASSERT(sut.variance() == 0.0 );
}

void  HistogramStatisticsTests::histogramSlopeOneBin(void) {
	qDebug() << "**HistogramStatisticsTests::histogramSlopeOneBin**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_ONE_POINT);
	CPPUNIT_ASSERT(sut.slope() == 0.0 );
}

void  HistogramStatisticsTests::histogramMaxOneBin(void) {
	qDebug() << "**HistogramStatisticsTests::histogramMaxOneBin**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_ONE_POINT);
	CPPUNIT_ASSERT(sut.max() == 1.0 );
}

void  HistogramStatisticsTests::histogramMinOneBin(void) {
	qDebug() << "**HistogramStatisticsTests::histogramMinOneBin**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_ONE_POINT);
	CPPUNIT_ASSERT(sut.max() == 1.0 );
}

void  HistogramStatisticsTests::histogramMedianOneBin(void) {
	qDebug() << "**HistogramStatisticsTests::histogramMedianOneBin**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_ONE_POINT);
	CPPUNIT_ASSERT(sut.median() == 1.0 );
}

//-----------------------------------------------------------------------------

void  HistogramStatisticsTests::histogramMeanTwoBins (void) {
	qDebug() << "**HistogramStatisticsTests::histogramMeanTwoBins**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_TWO_POINTS);
	CPPUNIT_ASSERT(round(sut.mean(),4) == round(1.6666666,4) );
}

void  HistogramStatisticsTests::histogramVarianceTwoBins (void) {
	qDebug() << "**HistogramStatisticsTests::histogramVarianceTwoBins**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_TWO_POINTS);
	CPPUNIT_ASSERT(round(sut.variance(),4) == round(0.222222,4) );
}

void  HistogramStatisticsTests::histogramSlopeTwoBins(void) {
	qDebug() << "**HistogramStatisticsTests::histogramSlopeTwoBins**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_TWO_POINTS);
	qDebug() << sut.slope();
	CPPUNIT_ASSERT(abs(sut.slope() + 0.074074074  ) < 0.000001 );
}

void  HistogramStatisticsTests::histogramMaxTwoBins(void) {
	qDebug() << "**HistogramStatisticsTests::histogramMaxTwoBins**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_TWO_POINTS);
	CPPUNIT_ASSERT(sut.max() == 2.0 );
}

void  HistogramStatisticsTests::histogramMinTwoBins(void) {
	qDebug() << "**HistogramStatisticsTests::histogramMinTwoBins**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_TWO_POINTS);
	CPPUNIT_ASSERT(sut.min() == 1.0 );
}

void  HistogramStatisticsTests::histogramMedianTwoBins(void) {
	qDebug() << "**HistogramStatisticsTests::histogramMedianTwoBins**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_TWO_POINTS);
	CPPUNIT_ASSERT(sut.median() == 2.0 );
}

void  HistogramStatisticsTests::histogramNumberOfBinsTwoBins(void) {
	qDebug() << "**HistogramStatisticsTests::histogramNumberOfBinsTwoBins**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_TWO_POINTS);
	CPPUNIT_ASSERT(sut.numberOfBins() == 2 );
}

void  HistogramStatisticsTests::histogramBinWidthTwoBins(void) {
	qDebug() << "**HistogramStatisticsTests::histogramBinWidthTwoBins**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_TWO_POINTS);
	CPPUNIT_ASSERT(sut.binWidth() == 1.0 );
}


//-----------------------------------------------------------------------------

void  HistogramStatisticsTests::histogramMeanThreeBins (void) {
	qDebug() << "**HistogramStatisticsTests::histogramMeanThreeBins**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_SYM_THREE);
	CPPUNIT_ASSERT(round(sut.mean(),0) == 2 );
}

void  HistogramStatisticsTests::histogramVarianceThreeBins (void) {
	qDebug() << "**HistogramStatisticsTests::histogramVarianceThreeBins**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_SYM_THREE);
	CPPUNIT_ASSERT(sut.variance() == 0.5 );
}

void  HistogramStatisticsTests::histogramSlopeThreeBins(void) {
	qDebug() << "**HistogramStatisticsTests::histogramSlopeThreeBins**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_SYM_THREE);
	qDebug() << sut.slope();
	CPPUNIT_ASSERT(round(sut.slope(),-4) == 0);
}

void  HistogramStatisticsTests::histogramMaxThreeBins(void) {
	qDebug() << "**HistogramStatisticsTests::histogramMaxThreeBins**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_SYM_THREE);
	CPPUNIT_ASSERT(sut.max() == 3.0 );
}

void  HistogramStatisticsTests::histogramMinThreeBins(void) {
	qDebug() << "**HistogramStatisticsTests::histogramMinThreeBins**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_SYM_THREE);
	CPPUNIT_ASSERT(sut.min() == 1.0 );
}

void  HistogramStatisticsTests::histogramMedianThreeBins(void) {
	qDebug() << "**HistogramStatisticsTests::histogramMedianThreeBins**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_SYM_THREE);
	CPPUNIT_ASSERT(sut.median() == 2.0 );
}

void  HistogramStatisticsTests::histogramNumberOfBinsThreeBins(void) {
	qDebug() << "**HistogramStatisticsTests::histogramNumberOfBinsThreeBins**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_SYM_THREE);
	CPPUNIT_ASSERT(sut.numberOfBins() == 3 );
}

void  HistogramStatisticsTests::histogramBinWidthThreeBins(void) {
	qDebug() << "**HistogramStatisticsTests::histogramBinWidthThreeBins**" ;
	HistogramStatistics sut;
	sut.setHistogram(&DATA_SYM_THREE);
	CPPUNIT_ASSERT(sut.binWidth() == 1.0 );
}
