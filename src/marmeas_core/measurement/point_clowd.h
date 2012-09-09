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


#ifndef POINT_CLOWD_H_
#define POINT_CLOWD_H_

#include <QVector>
#include <QPointF>

class PointClowd {
public:
	PointClowd(QVector<QPointF>* point_set = 0 );
	void setPoints(QVector<QPointF>* point_set);
	double minX() const;
	double minY() const;
	double maxX() const;
	double maxY() const;
	virtual ~PointClowd();
private:
	QVector<QPointF>* point_set_;
};

#endif /* POINT_CLOWD_H_ */
