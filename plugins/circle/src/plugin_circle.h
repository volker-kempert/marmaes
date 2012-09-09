/*
 *
 *  Created on: 31.07.2011
 *      Author: volker
 */

#ifndef PLUGIN_LINE_H_
#define PLUGIN_LINE_H_

#include "object_interface.h"


class CircleInterface : public QObject , public ObjectInterface {
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
