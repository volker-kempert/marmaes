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
#include <values.h>

#include "point_clowd.h"

PointClowd::PointClowd(QVector<QPointF>* point_set) {
	point_set_ = point_set;
}

PointClowd::~PointClowd() {

}


void PointClowd::setPoints(QVector<QPointF>* point_set) {
	point_set_ = point_set;
}

double PointClowd::minX() const {
	assert( point_set_ );
	double min_x = DBL_MAX;
	for (int i = 0; i < point_set_->size(); i++)
		if ( point_set_->at(i).x() < min_x ) min_x = point_set_->at(i).x();
	return min_x;
}

double PointClowd::minY() const {
	assert( point_set_ );
	double min_y = DBL_MAX;
	for (int i = 0; i < point_set_->size(); i++)
		if ( point_set_->at(i).y() < min_y ) min_y = point_set_->at(i).y();
	return min_y;
}

double PointClowd::maxX() const {
	assert( point_set_ );
	double max_x = DBL_MIN;
	for (int i = 0; i < point_set_->size(); i++)
		if ( point_set_->at(i).x() > max_x ) max_x = point_set_->at(i).x();
	return max_x;
}

double PointClowd::maxY() const {
	assert( point_set_ );
	double max_y = DBL_MIN;
	for (int i = 0; i < point_set_->size(); i++)
		if ( point_set_->at(i).y() > max_y ) max_y = point_set_->at(i).y();
	return max_y;
}
