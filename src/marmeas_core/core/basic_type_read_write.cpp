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

#include <QVariant>
#include <QDebug>
#include <QPointF>
#include <QRectF>
#include <QXmlStreamAttributes>


#include "basic_type_read_write.h"
#include "measurement/quartile.h"

class XMLType {
public:
	const char * operator()(QVariant::Type t) const {
		int i;
		for ( i = 0 ; i < size_ ; i++)
			if (type_lut_[i] == t) break;
		return string_lut_[i];
	}
	QVariant::Type operator()(QStringRef s) const {
		int i;
		for ( i = 0 ; i < size_ ; i++)
			if (string_lut_[i] == s) break;
		return type_lut_[i];
	}
private:

	static const int size_;
	static const QVariant::Type type_lut_[];
	static const char * string_lut_[];
};

const int XMLType::size_ = 6; // one less than the actual lut sizes
                              // used to model unsupported types regularly

const QVariant::Type XMLType::type_lut_[] = {
		QVariant::Bool,
		QVariant::Int,
		QVariant::Double,
		QVariant::String,
		QVariant::PointF,
		QVariant::RectF,
		QVariant::UserType,
		QVariant::Invalid,

};

const char * XMLType::string_lut_[] = {
		"bool" ,
		"integer",
		"double",
		"string",
		"point",
		"box",
		"quartile",
		""
};


bool QVariantWriter::writePointData(QXmlStreamWriter& writer) {
	if ( data_->type() != QVariant::PointF ) return false;
	QPointF p = data_->toPointF();
	QString s;

	writer.writeStartElement("x");
	s = QString("%1").arg(p.rx());
	writer.writeCharacters(s);
	writer.writeEndElement();

	writer.writeStartElement("y");
	s = QString("%1").arg(p.ry());
	writer.writeCharacters(s);
	writer.writeEndElement();
	return true;
}

bool QVariantWriter::writeRectData(QXmlStreamWriter& writer) {
	if ( data_->type() != QVariant::RectF ) return false;
	QRectF r = data_->toRectF();
	QString s;

	writer.writeStartElement("left");
	s = QString("%1").arg(r.left());
	writer.writeCharacters(s);
	writer.writeEndElement();

	writer.writeStartElement("top");
	s = QString("%1").arg(r.top());
	writer.writeCharacters(s);
	writer.writeEndElement();


	writer.writeStartElement("width");
	s = QString("%1").arg(r.width());
	writer.writeCharacters(s);
	writer.writeEndElement();

	writer.writeStartElement("height");
	s = QString("%1").arg(r.height());
	writer.writeCharacters(s);
	writer.writeEndElement();

	return true;
}

bool QVariantWriter::writeQuartile(QXmlStreamWriter& writer) {
	if ( data_->type() != QVariant::UserType ) return false;
	if ( !data_->canConvert<Quartile>() ) return false;
	Quartile q = data_->value<Quartile>();
	QString s;

	writer.writeStartElement("minimum");
	s = QString("%1").arg(q.minimum());
	writer.writeCharacters(s);
	writer.writeEndElement();

	writer.writeStartElement("first");
	s = QString("%1").arg(q.first());
	writer.writeCharacters(s);
	writer.writeEndElement();

	writer.writeStartElement("median");
	s = QString("%1").arg(q.median());
	writer.writeCharacters(s);
	writer.writeEndElement();

	writer.writeStartElement("third");
	s = QString("%1").arg(q.third());
	writer.writeCharacters(s);
	writer.writeEndElement();

	writer.writeStartElement("maximum");
	s = QString("%1").arg(q.maximum());
	writer.writeCharacters(s);
	writer.writeEndElement();

}

bool QVariantWriter::write(QXmlStreamWriter& writer) {
	double d;
	int i;
	bool b;
	bool result = true;
	writer.writeStartElement(*(name_.string()));
	QVariant::Type type = data_->type();
	XMLType type_convert;
	const char *xml_type = type_convert(type);
	writer.writeAttribute("type", xml_type);
	QString s;
	switch (type) {
	case QVariant::Bool:
		b = data_->toBool();
		if (b) s = "true"; else s = "false";
		writer.writeCharacters(s);
		break;
	case QVariant::Int:
		i = data_->toInt(&result);
		s = QString("%1").arg(i);
		writer.writeCharacters(s);
		break;
	case QVariant::Double:
		d = data_->toFloat(&result);
		s = QString("%1").arg(d);
		writer.writeCharacters(s);
		break;
	case QVariant::String:
		s = data_->toString();
		writer.writeCharacters(s);
		break;
	case QVariant::PointF:
		result = writePointData(writer);
		break;
	case QVariant::RectF:
		result = writeRectData(writer);
		break;
	case QVariant::UserType:
		result = false;
		if (data_->canConvert<Quartile>())
			result = writeQuartile(writer);
		break;

	default:
		// unknown type
		result = false;
	}
	writer.writeEndElement();
	return result;

}

QVariantWriter::~QVariantWriter() {}

//----------------------------------------------------------------------------

QVariantReader::QVariantReader(const QString&  name, QVariant& data) :
	// name_ gets a deep copy in order to prevent segmentation fault
	// if there is access to symbols (refs) which are based
	// on the plugin shared lib
	Reader(), name_(name) {
	data_ = &data;
}

bool QVariantReader::checkToken(const QXmlStreamReaderWhitespace& reader) {
	qDebug() << "--QVariantReader::checkToken-- name_: " << name_;
	if (reader.isStartElement())
		if (reader.name() == name_) return true;
	return false;
}


/*
 * read a structure like <node>float_val</node> + point to the next
 * node must be within the
 * @param node_names: list of expected node names
 * @param value: the double value to be read (reference parameter)
 * @param reader: XML stream reader object
 * @return: index of the node that matches the name
 *          negative value indicating any kind of failure
 */
int QVariantReader::readArbitraryFloat(QXmlStreamReaderWhitespace& reader,
		double& value, const QStringRef node_names[], int max_nodes) {


	bool convert;
	QStringRef s;
	QString t;
	int node_index = -1;

	if (!reader.isStartElement()) return -2;
	s = reader.name();
	for (int j = 0; j < max_nodes; j++)
		if (s == node_names[j])  { node_index = j; break; }
	if ( node_index == -1) return -1;
	reader.readNext();
	if (!reader.isCharacters()) return -3;
	t  = reader.text().toString();
	value = t.toFloat(&convert);
	if ( !convert) return -4;

	reader.readNext();
	if (!reader.isEndElement()) return -5;
	if (reader.name() != node_names[node_index]) return -6;

	// to the next; skip white chars
	reader.readNext();
	return node_index;
}


bool QVariantReader::readPoint(QXmlStreamReaderWhitespace& reader) {

	double val, x, y;
	const QString node_x("x");
	const QString node_y("y");

	const QStringRef node_names[] =  { &node_x , &node_y };
	const int max_nodes = sizeof node_names / sizeof (node_names[0]);
	for (int i = 0; i < max_nodes; i++ ) {
		int node_index = readArbitraryFloat(reader,val,node_names,max_nodes);
		switch(node_index) {
		case 0: // x
			x = val;
			break;
		case 1: // y
			y = val;
			break;
		default: // must be an error
			qDebug() << "--QVariantReader::readPoint--"
					 << " read error " << node_index ;
			return false;
		}
	}

	QPointF point( x, y);
	*data_ = point;
	return true;
}

bool QVariantReader::readRect(QXmlStreamReaderWhitespace& reader) {

	double val, left, top, width, height;
	const QString node_left("left");
	const QString node_top("top");
	const QString node_width("width");
	const QString node_height("height");

	const QStringRef node_names[] =  { &node_left , &node_top,
			                           &node_width , &node_height };
	const int max_nodes = sizeof node_names / sizeof (node_names[0]);
	for (int i = 0; i < max_nodes; i++ ) {
		int node_index = readArbitraryFloat(reader,val,node_names,max_nodes);
		switch(node_index) {
		case 0: // x
			left = val;
			break;
		case 1: // y
			top = val;
			break;
		case 2: // y
			width = val;
			break;
		case 3: // y
			height = val;
			break;

		default: // must be an error
			qDebug() << "--QVariantReader::readRect--"
					<< " read error " << node_index ;
			return false;
		}
	}

	QRectF rect(left,top,width,height);
	*data_ = rect;
	return true;
}

/*!
 * @brief read a user type, i.e. a quartile
 *        does not actually read the data at the moment since current contract
 *        says that data is recomputed anyway. Thus just sets a valid empty
 *        qVariant (if the end element appears)
 */
bool QVariantReader::readUserType(QXmlStreamReaderWhitespace& reader) {

	if (!reader.isStartElement()) return false;
	QStringRef s = reader.name();

	// overread until the end element
	while (reader.isEndElement() && reader.name() == s ) {
		reader.readNext();
		if (reader.isEndDocument() ) return false;
	}
	return true;
}

bool QVariantReader::read(QXmlStreamReaderWhitespace& reader) {

	// check for the preconditions
	if (!reader.isStartElement()) return false;
	if (reader.name() != name_) return false;

	bool syntax_ok = false;

	// figure out what to do
	QXmlStreamAttributes attributes = reader.attributes();
	QVariant::Type type;
	QStringRef tmp;
	if ( attributes.hasAttribute("type") ) {
		XMLType type_convert;
		tmp = attributes.value("type");
		type = type_convert(tmp);
	} else {
		// no type attribute -> // simple flag just set the flag
		*data_ = QVariant(true);
		syntax_ok = true;
	}


	if (! reader.isEndElement())
		reader.readNext();  // this is not simple flag - move ahead

	// read the data
	if (reader.isCharacters()) {
		// handle simple types
		QString value = reader.text().toString();
		bool b = false;
		int i;
		double d;
		switch (type) {
			case QVariant::Bool:
				if (value == "True" or value == "true") b = true;
				*data_ = QVariant(b);
				syntax_ok = true;
				break;
			case QVariant::Int:
				i = value.toInt(&b);
				if (b) {
					*data_ = QVariant(i);
					syntax_ok = true;
				} else
					// mal-formatted int
					syntax_ok = false;
				break;
			case QVariant::Double:
				d = value.toFloat(&b);
				if (b) {
					*data_ = QVariant(d);
					syntax_ok = true;
				} else
					// mal-formatted double
					syntax_ok = false;
				break;
			case QVariant::String:
				*data_ = QVariant(value);
				syntax_ok = true;
				break;

			default:
				// unknown type
				qDebug() << "--QVariantReader::read-- " << name_ << " type "
						 << tmp << " unkown variant type";
				qDebug() << "--QVariantReader::read-- Qtype" << type;
				syntax_ok = false;
				break;
		}
		reader.readNext();
	}
	if (reader.isStartElement())
		// complex types
		switch (type) {
			case QVariant::PointF:
				syntax_ok = readPoint(reader);
				break;
			case QVariant::RectF:
				syntax_ok = readRect(reader);
				break;
			case QVariant::UserType:
				syntax_ok = readUserType(reader);
				break;

			default:
				// unknown type
				syntax_ok = false;
				break;
		}
	// check for the correct ending
	bool ending_ok = true;
	if (reader.isEndElement())
		if (reader.name() != name_ )  ending_ok = false;
	// to the next; skip white chars
	reader.readNext();
	return syntax_ok && ending_ok;
}
