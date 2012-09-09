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

#ifndef IMAGE_CONTEXT_READ_WRITE_H_
#define IMAGE_CONTEXT_READ_WRITE_H_

#include "xml_reader_writer.h"
#include "marmeas_core/image_context/image_context.h"


typedef union {
	unsigned int group ;
	struct  {
		bool unit_of_measurement : 1 ;
		bool conversion_factor : 1 ;
		bool image_file_name     : 1 ;
	};
} ImageContextReadFlags;

class ImageContextReader : public Reader {
public:
	ImageContextReader(ImageContext* context) { image_context_ = context; }
	bool read(QXmlStreamReaderWhitespace& reader);
	bool checkToken(const QXmlStreamReaderWhitespace& reader);
	~ImageContextReader() {};
private:
	ImageContextReadFlags assignValue(QString key , QString value);
	ImageContext* image_context_;
};

class ImageContextWriter : public Writer {
public:
	ImageContextWriter(ImageContext* context) { image_context_ = context; }
	bool write(QXmlStreamWriter& writer);
	~ImageContextWriter() {};
private:
	ImageContext* image_context_;
};


#endif /* IMAGE_CONTEXT_READ_WRITE_H_ */
