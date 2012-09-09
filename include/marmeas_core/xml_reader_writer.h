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

#ifndef XML_READER_WRITER_H_
#define XML_READER_WRITER_H_

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QList>

class GeometricObject;   // forward declaration

class QXmlStreamReaderWhitespace : public QXmlStreamReader {
public:
	QXmlStreamReaderWhitespace( QIODevice * device );
	QXmlStreamReaderWhitespace();
	QXmlStreamReader::TokenType readNext ();
	unsigned int lineNumber() const;
private:
	unsigned int line_number_;
};

// just the interface to to snap into the Stream Reader
class Reader {
public:
	virtual bool read(QXmlStreamReaderWhitespace& reader) = 0;
	virtual bool checkToken(const QXmlStreamReaderWhitespace& reader) = 0;
	virtual ~Reader() {};
};



class XmlStreamReader {
public:
	XmlStreamReader();
	~XmlStreamReader();
	bool readFile(const QString file_name);
	void registerReader(Reader* reader);
protected:
	bool read(QXmlStreamReaderWhitespace& reader);
private:
	QList<Reader*> registered_reader_;
};

// just the interface to to snap into the Stream Writer
class Writer {
public:
	virtual bool write(QXmlStreamWriter& writer) = 0;
	virtual ~Writer() {};
};

class XmlStreamWriter {
public:
	XmlStreamWriter();
	~XmlStreamWriter();
	void registerWriter(Writer* writer);
	bool writeFile(const QString file_name);
private:
	QList<Writer*> registered_writer_;

};

#endif /* XML_READER_WRITER_H_ */
