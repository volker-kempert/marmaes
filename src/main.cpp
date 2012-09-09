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

#include <QApplication>
#include <QDebug>
#include <QString>
#include <QSettings>

#include <iostream>

#include "main_interaction.h"
#include "object_plugin.h"



int main(int argc, char *argv[])
{
	int ret_val;



    try {

        QApplication the_app(argc, argv);

        // context for settings to refer to
        the_app.setOrganizationName("pixmeter");
        the_app.setOrganizationDomain("pixmeter.com");
        the_app.setApplicationName("marmeas");

        QStringList cmdline_args = QCoreApplication::arguments();
    	qDebug() << "--main-- "
    	         <<  "arguments: " << cmdline_args
    	         <<  "application dir path "
    	         << QCoreApplication::applicationDirPath ();

    	/*
    	QString plugin_dir ; // command line parameter
    	qDebug() << "--main-- settings plugin directory: " << plugin_dir;
    	if (plugin_dir != "") {
    		ObjectPluginManager* plugin_manager = ObjectPluginManager::getInstance();
    		plugin_manager->registerPluginDir(plugin_dir);
    	}
    	*/

        MainInterAction *widget = new MainInterAction;
        widget->show();
        ret_val = the_app.exec();

    }
    catch (std::exception& e) {
    	std::cerr << "exception caught: " << e.what() << endl;
    	return 1;
    }

    return ret_val;
}
