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

#ifndef BASIC_TYPE_READ_WRITE_H_
#define BASIC_TYPE_READ_WRITE_H_

#include "xml_reader_writer.h"
#include <QStringRef>



class QVariantWriter: public Writer {
public:
	bool write(QXmlStreamWriter& writer);
	QVariantWriter(const QString name, const QVariant& data) :
			name_(&name) { data_ = &data; }
	virtual ~QVariantWriter();
private:
	bool writePointData(QXmlStreamWriter& writer);
	bool writeRectData(QXmlStreamWriter& writer);
	bool writeQuartile(QXmlStreamWriter& writer);

	QStringRef name_;
	QVariant const *data_;
};


class QVariantReader : public Reader {
public:
	QVariantReader(const QString& name, QVariant& data);
	bool read(QXmlStreamReaderWhitespace& reader);
	bool checkToken(const QXmlStreamReaderWhitespace& reader);
	~QVariantReader() {};
private:
	bool readPoint(QXmlStreamReaderWhitespace& reader);
	bool readRect(QXmlStreamReaderWhitespace& reader);
	bool readUserType(QXmlStreamReaderWhitespace& reader);
	int readArbitraryFloat(QXmlStreamReaderWhitespace& reader,
			double& value, const QStringRef node_names[], int max_nodes);

	QString name_;
	QVariant *data_;
};

#endif /* BASIC_TYPE_READ_WRITE_H_ */
