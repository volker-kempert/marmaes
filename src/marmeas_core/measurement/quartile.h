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

#ifndef QUARTILE_H_
#define QUARTILE_H_

#include <QObject>
#include <QString>
#include <QVariant>

class Quartile : public QObject {
	Q_OBJECT
public:
	Quartile(double min = 0, double first = 0 , double median = 0,
			 double third = 0, double max = 0 );
	Quartile(const Quartile& q);

	virtual ~Quartile();
	QString toString(int precision= 0) const;

	double minimum() const  { return  min_ ; }
	double first()   const  { return  first_ ; }
	double median()  const  { return  median_ ; }
	double third()   const  { return  third_ ; }
	double maximum()     const  { return  max_ ; }
private:
	double min_ ;
	double first_ ;
	double median_ ;
	double third_ ;
	double max_ ;
};


Q_DECLARE_METATYPE(Quartile)

#endif /* QUARTILE_H_ */
