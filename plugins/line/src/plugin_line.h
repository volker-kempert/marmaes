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

#ifndef PLUGIN_LINE_H_
#define PLUGIN_LINE_H_

#include "object_interface.h"


class LineInterface : public QObject , public ObjectInterface {
	Q_OBJECT
	Q_INTERFACES(ObjectInterface)

public:

	const QString& objectName() const { return object_name_; }
	ObjectBaseReader*  getXMLReader() const;
	Writer*  getXMLWriter() const ;
	const QStringList& objectTemplates() const ;
	GeometricObjectFactory* objectFactory(const QString& obj_templ) const ;
	const QIcon * icons(const QString& obj_templ) const ;
private:
	static const QStringList object_templates_;
	static const QIcon object_icons_[];
	static const QString object_name_;
};


#endif /* PLUGIN_LINE_H_ */
