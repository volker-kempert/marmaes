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

#include "assert.h"

#include <QtHelp/QHelpEngine>
#include <QtHelp/QHelpContentModel>
#include <QtHelp/QHelpContentWidget>
#include <QtHelp/QHelpIndexModel>
#include <QtHelp/QHelpIndexWidget>
#include <QFileInfo>

#include <QDebug>

#include <QDockWidget>

#include "help.h"
#include "injected_defines.h"
#include "help_browser.h"


QHelpEngine* HelpBrowserDialog::help_engine_ = 0;

HelpBrowserDialog::HelpBrowserDialog(QWidget *parent)   : QWidget(parent) {
	qDebug() << "--HelpBrowserDialog::HelpBrowserDialog-- start";
	ui.setupUi(this);
	connect(ui.close, SIGNAL(clicked()), this, SLOT(close()));
	if (help_engine_ == 0) {
		qDebug() << "--HelpBrowserDialog::HelpBrowserDialog-- help file name: "
				<<  __help_dir__ "/marmeas.qhc";
		QFileInfo file_info_qhc(__help_dir__ "/marmeas.qhc");
		if (file_info_qhc.exists())   {
			qDebug() << "--HelpBrowserDialog::HelpBrowserDialog-- Help File is available";
			help_engine_ = new QHelpEngine(file_info_qhc.absoluteFilePath());
			assert(help_engine_);

			qDebug() << "--HelpBrowserDialog::HelpBrowserDialog-- Registered Docs: "
			         << help_engine_->registeredDocumentations()
					 << "compressed doc file: "
					 << help_engine_->documentationFileName("marmeas") ;
			QString registered_docs(__help_dir__ "marmeas.qch");
			if (help_engine_->registerDocumentation (registered_docs))
					qDebug() << "--HelpBrowserDialog::HelpBrowserDialog-- Documentation file is registered: "
							<< registered_docs;
			if (help_engine_->setupData())
				qDebug () << "--HelpBrowserDialog::HelpBrowserDialog-- Data setup successfully";
			qDebug() << "--HelpBrowserDialog::HelpBrowserDialog-- Available filter attributes: "
					<< help_engine_->filterAttributes()
					<< "current filter is: " << help_engine_->currentFilter();
			help_engine_->setCurrentFilter("marmeas");
		}
	}

	// redirect the source for the browser to the help_engine
	ui.textBrowser->setHelpEngine(help_engine_);

	// fill toc and index tabs
	QHelpContentWidget *contentWidget = help_engine_->contentWidget();
	QHelpContentModel *contentModel = help_engine_->contentModel();

	QHelpIndexModel* indexModel = help_engine_->indexModel();
	QHelpIndexWidget* indexWidget = help_engine_->indexWidget();

	// do some layouting

	ui.toc_layout->addWidget((QWidget*) contentWidget);
	ui.index_layout->addWidget((QWidget*) indexWidget);

	indexWidget->setModel(indexModel);
	contentWidget->setModel(contentModel);

	// do the signal wiring

    connect(contentWidget, SIGNAL(linkActivated(const QUrl &)),
            ui.textBrowser, SLOT(setSource(const QUrl &)));
    connect(ui.textBrowser, SIGNAL(sourceChanged(const QUrl &)),
    		this, SLOT(updateWindowTitle()));
    connect(ui.home, SIGNAL(clicked()),  ui.textBrowser,SLOT(home()));
    connect(ui.back, SIGNAL(clicked()), ui.textBrowser,SLOT(backward()));
}

HelpBrowserDialog::~HelpBrowserDialog()
{
	// do not delete help_engine object
}

void HelpBrowserDialog::updateWindowTitle()
{
	setWindowTitle(tr("Help: %1").arg(ui.textBrowser->documentTitle()));
}



void HelpBrowserDialog::showPage(const QString &page) {

	QString tmp = "qthelp://marmeas/doc/";
	if (page != "")
		tmp = tmp + page;
	else
		tmp = tmp + "index.html";
	QUrl url(tmp);
    qDebug() << "--HelpBrowserDialog::showPage-- " << url;
    ui.textBrowser->setSource(url);
}


