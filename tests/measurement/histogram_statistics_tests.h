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

#ifndef HISTOGRAM_STATISTICS_TESTS_H_
#define HISTOGRAM_STATISTICS_TESTS_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "marmeas_core/measurement/histogram_statistics.h"

using namespace std;



class HistogramStatisticsTests : public CPPUNIT_NS :: TestFixture {
public:
	CPPUNIT_TEST_SUITE (HistogramStatisticsTests);

	CPPUNIT_TEST (doubleRounding);

    CPPUNIT_TEST_EXCEPTION( noHistogramMeanThrow, NoHistogramError );
    CPPUNIT_TEST_EXCEPTION( noHistogramVarianceThrow, NoHistogramError );
    CPPUNIT_TEST_EXCEPTION( noHistogramMinThrow, NoHistogramError );
    CPPUNIT_TEST_EXCEPTION( noHistogramMaxThrow, NoHistogramError );
    CPPUNIT_TEST_EXCEPTION( noHistogramMedianThrow, NoHistogramError );
    CPPUNIT_TEST_EXCEPTION( noHistogramBinWidthThrow, NoHistogramError );
    CPPUNIT_TEST_EXCEPTION( noHistogramNumberOfBinsThrow, NoHistogramError );

    CPPUNIT_TEST_EXCEPTION( NoEqualSizedBinsThrow, NoEqualSizedBinsError );

    CPPUNIT_TEST_EXCEPTION( profileZeroThrow, InconsistentProfileError );
    CPPUNIT_TEST_EXCEPTION( profileEmptyThrow, InconsistentProfileError );
    CPPUNIT_TEST_EXCEPTION( profileWrongBinsThrow, InconsistentProfileError );

    CPPUNIT_TEST (profileOnePointCorrectBins);
	CPPUNIT_TEST (profileOnePoint);
	CPPUNIT_TEST (profileOneBinTwoPoints);
	CPPUNIT_TEST (profileOneBinSameX);
	CPPUNIT_TEST (profileTwoBins);
	CPPUNIT_TEST (profileThreeBins);


	CPPUNIT_TEST (histogramMeanOneBin);
	CPPUNIT_TEST (histogramVarianceOneBin);
	CPPUNIT_TEST (histogramSlopeOneBin);
	CPPUNIT_TEST (histogramMaxOneBin);
	CPPUNIT_TEST (histogramMinOneBin);
	CPPUNIT_TEST (histogramMedianOneBin);

	CPPUNIT_TEST (histogramMeanTwoBins);
	CPPUNIT_TEST (histogramVarianceTwoBins);
	CPPUNIT_TEST (histogramSlopeTwoBins);
	CPPUNIT_TEST (histogramMaxTwoBins);
	CPPUNIT_TEST (histogramMinTwoBins);
	CPPUNIT_TEST (histogramMedianTwoBins);
	CPPUNIT_TEST (histogramBinWidthTwoBins);
	CPPUNIT_TEST (histogramNumberOfBinsTwoBins);

	CPPUNIT_TEST (histogramMeanThreeBins);
	CPPUNIT_TEST (histogramVarianceThreeBins);
	CPPUNIT_TEST (histogramSlopeThreeBins);
	CPPUNIT_TEST (histogramMaxThreeBins);
	CPPUNIT_TEST (histogramMinThreeBins);
	CPPUNIT_TEST (histogramMedianThreeBins);
	CPPUNIT_TEST (histogramBinWidthThreeBins);
	CPPUNIT_TEST (histogramNumberOfBinsThreeBins);

	CPPUNIT_TEST_SUITE_END ();

public:
	void setUp (void);
	void tearDown (void);

protected:

	void doubleRounding (void);

	void noHistogramMeanThrow (void);
	void noHistogramVarianceThrow(void);
	void noHistogramMaxThrow(void);
	void noHistogramMinThrow(void);
	void noHistogramMedianThrow(void);
	void noHistogramNumberOfBinsThrow(void);
	void noHistogramBinWidthThrow(void);

	void NoEqualSizedBinsThrow(void);

	void  profileZeroThrow (void);
	void  profileEmptyThrow (void);
	void  profileWrongBinsThrow (void);

	void  profileOnePointCorrectBins (void);
	void  profileOnePoint (void);
	void  profileOneBinTwoPoints (void);
	void  profileOneBinSameX(void);
	void  profileTwoBins(void);
	void  profileThreeBins (void);

	void  histogramMeanOneBin (void);
	void  histogramVarianceOneBin (void);
	void  histogramSlopeOneBin(void);
	void  histogramMaxOneBin(void);
	void  histogramMinOneBin(void);
	void  histogramMedianOneBin(void);

	void  histogramMeanTwoBins (void);
	void  histogramVarianceTwoBins (void);
	void  histogramSlopeTwoBins(void);
	void  histogramMaxTwoBins(void);
	void  histogramMinTwoBins(void);
	void  histogramMedianTwoBins(void);
	void  histogramNumberOfBinsTwoBins(void);
	void  histogramBinWidthTwoBins(void);

	void  histogramMeanThreeBins (void);
	void  histogramVarianceThreeBins (void);
	void  histogramSlopeThreeBins(void);
	void  histogramMaxThreeBins(void);
	void  histogramMinThreeBins(void);
	void  histogramMedianThreeBins(void);
	void  histogramNumberOfBinsThreeBins(void);
	void  histogramBinWidthThreeBins(void);
private:

};


#endif /* HISTOGRAM_STATISTICS_TESTS_H_ */
