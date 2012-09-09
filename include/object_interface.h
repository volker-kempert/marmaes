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


#ifndef OBJECT_INTERFACE_H_
#define OBJECT_INTERFACE_H_

#include <QIcon>
#include "object_base.h"
#include "object_base_read_write.h"

/*!
 *  @brief Interface that all object plugins must implement
 */
class ObjectInterface  {
public:
	virtual  ~ObjectInterface() {}
	/*!
	 * one plugin provides one to many Templates for exactly one type of object
	 * that is specified by ObjectName()
	 */
	virtual const QString& objectName() const = 0;
	/*!
	 * the xml portion reader for the object type under consideration
	 */
	virtual ObjectBaseReader*  getXMLReader() const = 0;
	virtual Writer*  getXMLWriter() const = 0;
	/*!
	 * enumerate all object templates that are provided to create the
	 * object type of that plugin
	 *
	 */
	virtual const QStringList& objectTemplates() const = 0;
	/*!
	 * @return pointer to a factory object
	 *         zero if factory is found, that matches the object template name
	 */
	virtual GeometricObjectFactory* objectFactory(const QString& obj_templ) const = 0;
	/*!
	 * @return pointer to array of QIcons;
	 *         if the pointer is to a zero icon , no more Icons are provided
	 *         by the plugin
	 *         the first item is the default for the default template,
	 *         thus there must be at least one icon given that is a non-zero
	 *         icon
	 */
	virtual const QIcon* icons(const QString& obj_templ) const = 0;
};

Q_DECLARE_INTERFACE(ObjectInterface,"org.dobus.marmeas.ObjectInterface/2.2")

#endif /* OBJECT_INTERFACE_H_ */
