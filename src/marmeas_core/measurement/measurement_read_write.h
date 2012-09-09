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

#ifndef MEASUREMENT_READ_WRITE_H_
#define MEASUREMENT_READ_WRITE_H_

#include "xml_reader_writer.h"
#include "measurement_on_object.h"

class MeasurementReader: public Reader {
public:
	MeasurementReader(MeasurementOnObject* measurement);
	virtual ~MeasurementReader();
	bool read(QXmlStreamReaderWhitespace& reader);
	bool checkToken(const QXmlStreamReaderWhitespace& reader);

private:
	bool readLeaf(QXmlStreamReaderWhitespace& reader);
	bool readComposite(QXmlStreamReaderWhitespace& reader);
	void setActualFlags(void) ;

	MeasurementOnObject* measurement_;
	QList<MeasurementReader*> sub_readers_;
};

#endif /* GEOMETRIC_MEASUREMENT_READ_WRITE_H_ */
