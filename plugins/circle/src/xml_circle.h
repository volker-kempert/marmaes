/*
 * xml_line.h
 *
 *  Created on: 27.08.2011
 *      Author: volker
 */

#ifndef XML_LINE_H_
#define XML_LINE_H_

#include <object_base_read_write.h>

#include "circle.h"


class CircleReader : public ObjectBaseReader {
public:
	CircleReader() : ObjectBaseReader() {}
	bool checkToken(const QXmlStreamReaderWhitespace& reader);
	virtual ~CircleReader() {}
protected:
	virtual void initObjects();
	virtual bool readGeometricObject(QXmlStreamReaderWhitespace& reader);
	void finalize();
};


#endif /* XML_LINE_H_ */
