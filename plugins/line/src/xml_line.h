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

#ifndef XML_LINE_H_
#define XML_LINE_H_

#include <object_base_read_write.h>

#include "line.h"


class LineReader : public ObjectBaseReader {
public:
	LineReader() : ObjectBaseReader() { }
	bool checkToken(const QXmlStreamReaderWhitespace& reader);
	virtual ~LineReader() {}
protected:
	virtual void initObjects();
	virtual bool readGeometricObject(QXmlStreamReaderWhitespace& reader);
	void finalize();
};


#endif /* XML_LINE_H_ */
