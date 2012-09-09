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

#ifndef HELP_BROWSER_H
#define HELP_BROWSER_H


#include <QTextBrowser>
#include <ui_help.h>


class QHelpEngine;

class HelpBrowser : public QTextBrowser {
public:
	void setHelpEngine(QHelpEngine* help_engine);
	HelpBrowser( QWidget * parent = 0 );
	~HelpBrowser();
	virtual QVariant 	loadResource ( int type, const QUrl & name );
private:
	QHelpEngine* help_engine_;
};


#endif // HELP_BROWSER_H
