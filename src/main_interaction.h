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

#ifndef MAIN_INTERACTION_H
#define MAIN_INTERACTION_H

#include <QtGui/QMainWindow>
#include <QString>
#include "ui_main.h"
#include "image_context/image_context_view.h"
#include "image_context/image_context_templates_dialog.h"
#include "image_context/image_context_dialog.h"
#include "marmeas_core/image_context/image_context.h"
#include "status_and_history.h"
#include "marmeas_gui/graphics_view.h"
#include "marmeas_gui/graphics_scene.h"
#include "object/object_data_view.h"
#include "object/object_data_dialog.h"
#include "object/object_template.h"

#include "xml_reader_writer.h"


class MainInterAction : public QMainWindow
{
    Q_OBJECT

public:
    MainInterAction(QWidget *parent = 0);
    ~MainInterAction();

public slots:

	void editObjectData();
	void deleteObject();

private slots:
	void openFile();
	bool saveFile(QString s = "");
	void troggleSaveFile(bool enable);
	bool saveFileAs();
	bool closeData();
	void openHistoryFile1();
	void openHistoryFile2();
	void openHistoryFile3();
	void openHistoryFile4();
	void openHistoryFile5();
	void openHistoryFile6();
	void openHistoryFile7();
	void openHistoryFile8();
	void updateHistoryMenuEntries();
	void exit();

	void navigateMode();
	void selectMode();
	void addObjectMode();

	void editImageContext();


	bool configInputCharacteristics();
	void help();
	void aboutIcomm();
	void aboutQt();

signals:
	void imageContextChanged();


private:
	bool okToContinue();
	bool loadFile(const QString &file_name);
	bool loadImageFile(const QString &file_name);
	bool loadImageFileAndInteraction(const QString &file_name);
	void setupObjectTemplates();
	void setupObjectReaders(XmlStreamReader& reader);
	QString determineImageContextFileName();
	QString determineObjectOfInterestFileName();
	QAction* getHistoryEntry(int i);

	void wireInitialEvents();
	void setupWidgets();

	void writeSettings();
	void readSettings();

    Ui_MainWindow ui_;

    QString image_file_name_;
    ImageContext* image_context_;
    StatusAndHistory* status_and_history_;
    ObjectTemplateContainer* object_template_container_;

    ConfigInputCharacteristicsDialog* config_input_dialog_;
    ObjectDataDialog* object_data_dialog_;

    ObjectDataView* object_data_view_;
    ImageContextView* image_data_view_;
    GraphicsView* graphics_view_;
    GraphicsScene* graphics_scene_;
};

#endif // MAIN_INTERACTION_H
