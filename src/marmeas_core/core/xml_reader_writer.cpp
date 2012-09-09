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


#include <QFile>
#include <QXmlStreamWriter>
#include <QDebug>

#include "xml_reader_writer.h"


XmlStreamReader::XmlStreamReader() {

}


/*
 * @description: register a reader that reads a specific xml portion
 *              the reader must be dynamically created (new) and the
 *              XmlStreamReader object is responsible for Reader deleting
 * @param reader: the reader
 */
void XmlStreamReader::registerReader(Reader* reader) {
	registered_reader_.append(reader);
}

bool XmlStreamReader::readFile(const QString file_name) {
	QFile file(file_name);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		qDebug() << "--XmlStreamReader::readFile-- "
				 << "file: cannot be opened for reading";
		// place a Message box?
		return false;
	}
	QXmlStreamReaderWhitespace reader(&file);
	if ( ! read(reader)) {
		qDebug() << "--XmlStreamReader::readFile-- at line "
				 << reader.lineNumber() << ": error reading xml data";
		return false;
	}
	if (file.error()) {
		qDebug() << "--XmlStreamReader::readFile-- at line "
				 << reader.lineNumber() << ": file error";
		return false;
	}
	return true;
}

bool XmlStreamReader::read(QXmlStreamReaderWhitespace& reader) {
	bool read_ok = true;
	reader.readNext();
	// process all token
	while (!reader.atEnd()) {
		// process registered reader
		for (int i = 0; i < registered_reader_.size(); ++i) {
			 // read in the next token
			if (registered_reader_.at(i)->checkToken(reader))
				 if (!registered_reader_.at(i)->read(reader)) {
					 read_ok = false;
					 qDebug("--XmlStreamReader::read-- "
							 "file: xml processing error");
					 break;
				 }
		}
		reader.readNext();
	}
	return read_ok;
}


XmlStreamReader::~XmlStreamReader() {
	// remove all registered readers
	for (int i = 0; i < registered_reader_.size(); ++i)
    	delete registered_reader_.at(i);
}

//------------------------------------------------------------------------

XmlStreamWriter::XmlStreamWriter()  {

}

void XmlStreamWriter::registerWriter(Writer* writer) {
	registered_writer_.append(writer);
}

bool XmlStreamWriter::writeFile(const QString file_name) {
	QFile file(file_name);
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		qDebug() << "--XmlStreamWriter::writeFile-- "
				 << "file: cannot be opened for writing";
		return false;
		// place a Message box?
	}
	QXmlStreamWriter writer(&file);
	writer.setAutoFormatting(true);
	writer.writeStartDocument();
	writer.writeStartElement("marmeas");
	// process registered writer
	for (int i = 0; i < registered_writer_.size(); ++i) {
	     registered_writer_.at(i)->write(writer);
	 }
	writer.writeEndElement();
	writer.writeEndDocument();
	file.close();
	if (file.error()) {
		qDebug() << "--XmlStreamWriter::writeFile-- "
				    "file: error writing xml data";
		return false;
	}
	return true;
}

XmlStreamWriter::~XmlStreamWriter() {
}

//------------------------------------------------------------------------

QXmlStreamReaderWhitespace::QXmlStreamReaderWhitespace( QIODevice * device ) :
	QXmlStreamReader(device) {
		line_number_ = 0;
}
QXmlStreamReaderWhitespace::QXmlStreamReaderWhitespace() :
	QXmlStreamReader() {
		line_number_ = 0;
}

unsigned int QXmlStreamReaderWhitespace::lineNumber() const {
	return line_number_;
}


QXmlStreamReader::TokenType QXmlStreamReaderWhitespace::readNext () {

	QXmlStreamReader::TokenType token = ((QXmlStreamReader*)this)->readNext();
	// overread whitespaces
	if (token  == QXmlStreamReader::Characters ) {
		QString t;
		t.append(this->text());
		line_number_ += t.count('\n');
		if ( isWhitespace()  ) {
			token = ((QXmlStreamReader*)this)->readNext();
		}
	}

	return token;

}

