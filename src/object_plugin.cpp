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


#include <QObject>
#include <QDir>
#include <QPluginLoader>
#include <QLibrary>
#include <QSettings>
#include <QDebug>
#include "object_plugin.h"
#include "injected_defines.h"



ObjectPluginManager* ObjectPluginManager::the_object_plugin_manager_ = 0;

ObjectPluginManager* ObjectPluginManager::getInstance() {
    if (!the_object_plugin_manager_) {
    	qDebug() << "--ObjectPluginManager::getInstance-- "
    			 << "Create ObjectPluginManager instance";
    	the_object_plugin_manager_ = new ObjectPluginManager();
    }
    return the_object_plugin_manager_;
}

ObjectPluginManager::ObjectPluginManager() {
	plugins_loaded_ = false;
}

/**
 * @brief: register a list of directories where to search for
 *         in case not
 */
bool ObjectPluginManager::registerPluginDir(const QString& plugins_directory) {
	qDebug() << "--ObjectPluginManager::registerPluginDir--"
			<< " try for directory: " << plugins_directory;
	if ( plugins_loaded_ ) {
		qDebug() << "--ObjectPluginManager::registerPluginDir--"
				<< " don't try anything - plugins already loaded";
	} else {
		if ( findAndLoadPlugins(plugins_directory) ) {
			qDebug() << "--ObjectPluginManager::registerPluginDir--"
					<< " plugin(s) successfully loaded ";
			plugins_loaded_ = true;
			return true;
		}
	}
	return false;
}


ObjectPluginManager::~ObjectPluginManager() {
	// nothing to do since all the plugins get unloaded while the application
	// terminates
}

const QStringList& ObjectPluginManager::pluginNames()  {
	if ( ! plugins_loaded_ ) {
		// check if a plugin directory is configured
    	QSettings settings;
    	settings.beginGroup("system");
    	QString plugin_dir = settings.value("pluginDirectory").toString();
    	settings.endGroup();
    	qDebug() << "--ObjectPluginManager::pluginNames-- "
    			 << "settings plugin directory: " << plugin_dir;
    	// if nothing is given by the settings use build in plugin directory
    	if (plugin_dir != "") plugin_dir == __plugin_dir__;
		registerPluginDir(plugin_dir);
	}
	return plugin_names_;
}

ObjectInterface* ObjectPluginManager::plugin(const QString& name) const {
	for (int i = 0; i < plugin_names_.size(); i++)
		if ( plugin_names_[i] == name) return plugins_[i];
	return 0;
}

/**
 * @brief:           try to load plugins located in the given directory
 * @param dir_name:  name of the directory where to search in
 * @return:          true if any plugin was successfully loaded
 */
bool ObjectPluginManager::findAndLoadPlugins(const QString& dir_name) {

	QDir plugin_dir(dir_name);
	bool any_plugin_loaded = false;
	QStringList filters;
	filters << "*.so" << "*.dll";  // handle linux and windows
	plugin_dir.setNameFilters(filters);
	foreach (QString file_name, plugin_dir.entryList(QDir::Files)) {
		qDebug() << "--ObjectPluginManager::findAndLoadPlugins-- "
				 << "probe file" << file_name;
		QPluginLoader loader(plugin_dir.absoluteFilePath(file_name));
		QObject* obj_interface = loader.instance();
		qDebug() << "--ObjectPluginManager::findAndLoadPlugins-- "
				 << "raw interface reference : "
				 << (unsigned long) obj_interface
				 << " error String " << loader.errorString();
		ObjectInterface* interface = qobject_cast<ObjectInterface *>(obj_interface);
		if ( interface != 0 ) {
			qDebug() << "--ObjectPluginManager::findAndLoadPlugins-- "
					 << "loaded plugin - interface class : "
					 << interface->objectName();
			plugins_.append(interface);
			plugin_names_.append(file_name);
			any_plugin_loaded = true;
		} else
			if (loader.isLoaded())
				qDebug() << "--ObjectPluginManager::findAndLoadPlugins-- "
						"plugin signature mismatch (interface cast failed)" ;
	}
	return any_plugin_loaded;
}
