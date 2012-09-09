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

#ifndef ARBITRARY_MATH_H_
#define ARBITRARY_MATH_H_
/**
 * @brief round a double using the scientific method
 * @param r: the double number presentation that needs to be rounded
 * @param precision: number of digits after the .
 *                   negative values allow rounding to to bigger numbers
 *                   default is 1
 */
double round(double r, int precision = 1);


#endif /* ARBITRARY_MATH_H_ */
