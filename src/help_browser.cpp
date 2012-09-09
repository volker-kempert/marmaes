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

#include "help.h"

#include "assert.h"

#include <QDebug>
#include <QtHelp/QHelpEngine>
#include <QFileInfo>
#include "help_browser.h"


HelpBrowser::HelpBrowser( QWidget * parent  ) : QTextBrowser(parent)  {
	help_engine_ = 0;
}

HelpBrowser::~HelpBrowser() {}

void HelpBrowser::setHelpEngine(QHelpEngine* help_engine) {
	help_engine_ = help_engine;
}

QVariant HelpBrowser::loadResource(int type,
                                   const QUrl &url)
{
    if (url.scheme() == "qthelp") {
    	qDebug() << "--HelpBrowser::loadResource-- redirect to help engine "
    			<< url;
        return QVariant(help_engine_->fileData(url));
    }  else
        return QTextBrowser::loadResource(type, url);
}
