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

#include <QtPlugin>
#include "plugin_line.h"
#include "line.h"
#include "xml_line.h"

const QString LineInterface::object_name_ = "line";
const QStringList LineInterface::object_templates_ = ( QStringList() << QString("line"));
const QIcon  LineInterface::object_icons_[] = {
	   QIcon(QString::fromUtf8(":/resources/line_icon_a.svg"))
	,  QIcon(":/resources/line_icon_b.svg")
	,  QIcon(":/resources/line_icon_c.svg")
	,  QIcon(":/resources/line_icon_f.svg")
	,  QIcon(":/resources/line_icon_h.svg")
	,  QIcon() // a zero item at the end
    };


ObjectBaseReader*  LineInterface::getXMLReader() const {
	LineReader* reader = new LineReader();
	return (ObjectBaseReader *)reader;
}

Writer*  LineInterface::getXMLWriter() const {
	return 0;
}

const QStringList& LineInterface::objectTemplates() const {
	return object_templates_;
}


GeometricObjectFactory* LineInterface::objectFactory(const QString& obj_templ) const {
	if ( obj_templ == "line" ) {
		FactoryLineMarker* factory = new FactoryLineMarker();
		return (GeometricObjectFactory*) factory;
	}
	// nothing found
	return 0;
}

const QIcon * LineInterface::icons(const QString& obj_templ) const {
	// return object_icons_;
	if (obj_templ == "line")
		return new QIcon(QString::fromUtf8(":/resources/line_icon_a.svg"));
	else
		return new QIcon();
}

Q_EXPORT_PLUGIN2(marmeasline, LineInterface)
