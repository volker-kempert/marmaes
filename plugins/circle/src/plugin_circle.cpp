/*
 *
 *  Created on: 31.07.2011
 *      Author: volker
 */

#include <QtPlugin>
#include "plugin_circle.h"
#include "circle.h"
#include "3point_circle.h"
#include "2point_circle.h"
#include "xml_circle.h"

const QString CircleInterface::object_name_ = "circle";
const QStringList CircleInterface::object_templates_ = (
		QStringList() << QString("circle_3point")
					  << QString("circle_2point"));
const QIcon  CircleInterface::object_icons_[] = {
	   QIcon(QString::fromUtf8(":/resources/circle2_icon_a.svg"))
	,  QIcon(":/resources/circle3_icon_b.svg")
	,  QIcon(":/resources/circle3_icon_c.svg")
	,  QIcon(":/resources/circle3_icon_f.svg")
	,  QIcon(":/resources/circle3_icon_h.svg")
	,  QIcon() // a zero item at the end
    };


ObjectBaseReader*  CircleInterface::getXMLReader() const {
	CircleReader* reader = new CircleReader();
	return (ObjectBaseReader *)reader;
}

Writer*  CircleInterface::getXMLWriter() const {
	return 0;
}

const QStringList& CircleInterface::objectTemplates() const {
	return object_templates_;
}


GeometricObjectFactory* CircleInterface::objectFactory(const QString& obj_templ) const {
	if ( obj_templ == "circle_3point" ) {
		FactoryCircle3PointAnnotator* factory =
				new FactoryCircle3PointAnnotator();
		return (GeometricObjectFactory*) factory;
	}
	if ( obj_templ == "circle_2point" ) {
		FactoryCircle2PointMarker* factory =
				new FactoryCircle2PointMarker();
		return (GeometricObjectFactory*) factory;
	}
	// nothing found
	return 0;
}

const QIcon * CircleInterface::icons(const QString& obj_templ) const {
	if ( obj_templ == "circle_3point" ) {
		QIcon *p = new QIcon(":/resources/circle3_icon_a.svg");
		return p;
	}
	if ( obj_templ == "circle_2point" ) {
		QIcon *p = new QIcon(":/resources/circle2_icon_a.svg");
		return p;
	}

	// nothing found -> empty icon
	return new QIcon();


}

Q_EXPORT_PLUGIN2(marmeascircle, CircleInterface)
