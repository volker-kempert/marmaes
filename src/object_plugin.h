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

#ifndef OBJECT_PLUGIN_H_
#define OBJECT_PLUGIN_H_

#include "object_interface.h"
#include <QStringList>
#include <QString>

class ObjectPluginManager {
public:
	static ObjectPluginManager* getInstance();
	bool registerPluginDir(const QString& directory_name);
	const QStringList& pluginNames();
	ObjectInterface* plugin(const QString& name) const;
	~ObjectPluginManager();
private:
	static ObjectPluginManager* the_object_plugin_manager_;
	ObjectPluginManager();
	bool findAndLoadPlugins(const QString& dir_name);
	QList<ObjectInterface *> plugins_;
	QStringList plugin_names_;
	bool plugins_loaded_;
};



#endif /* OBJECT_PLUGIN_H_ */
