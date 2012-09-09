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

#include "quartile.h"


Quartile::Quartile(double min, double first, double median,
		 	 	 	 double third, double max) {

	min_ = min;
	first_ = first;
	median_ = median;
	third_ = third;
	max_ = max;
}

Quartile::Quartile(const Quartile& q) {
	min_ = q.min_;
	first_ = q.first_;
	median_ = q.median_;
	third_ = q.third_;
	max_ = q.max_;
}

Quartile::~Quartile() {
}

QString Quartile::toString(int precision) const {
	return QString("( %1 %2 %3 %4 %5 )").arg(min_,0,'f',precision)\
			.arg(first_,0,'f',precision).arg(median_,0,'f',precision)\
			.arg(third_,0,'f',precision).arg(max_,0,'f',precision);
}
