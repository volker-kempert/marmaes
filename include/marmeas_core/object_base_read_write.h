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

#ifndef OBJECT_BASE_READ_WRITE_H_
#define OBJECT_BASE_READ_WRITE_H_


#include "xml_reader_writer.h"
#include "object_base.h"

class ObjectBaseReader : public Reader {
public:
	ObjectBaseReader();
	bool read(QXmlStreamReaderWhitespace& reader);
	void setScene(QGraphicsScene* scene) ;
	~ObjectBaseReader() {};
protected:
	/*
	 * create measurement and geometric object, such that they met
	 * the appropriate types
	 * is called after the object type is determined
	 * (i.e. right after the beginning  of the read)
	 */
	virtual void initObjects() = 0;
	virtual bool readGeometricObject(QXmlStreamReaderWhitespace& reader) = 0;
	/*
	 * put all the objects into the right order
	 * is called at the end of the read method (if everything was
	 * successful so far
	 */
	virtual void finalize() = 0;
	MeasurementOnObject* measurement_;
	GeometricObject* object_;
	long id_;
	QString name_;
	QString source_;
	QGraphicsScene* scene_; // to be used for creation
private:
	bool readName(QXmlStreamReaderWhitespace& reader);
	bool readSource(QXmlStreamReaderWhitespace& reader);




};

// ---------------------------------------------------------------------------

class ObjectBaseWriter : public Writer {
public:
	ObjectBaseWriter(GeometricObject* object) { object_ = object; }
	bool write(QXmlStreamWriter& writer);
	~ObjectBaseWriter() {};
private:
	GeometricObject* object_;
	bool writeMeasurement(QXmlStreamWriter& writer,
			const MeasurementOnObject* measurement);
};


// ---------------------------------------------------------------------------


class ObjectContainerWriter : public Writer {
public:
	ObjectContainerWriter() : Writer() {
		container_ = GeometricObjectContainerSingleton::getInstance(); }
	bool write(QXmlStreamWriter& writer);
	~ObjectContainerWriter() {};
private:
	GeometricObjectContainer* container_;
};

#endif /* OBJECT_BASE_READ_WRITE_H_ */
