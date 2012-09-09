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


#include <QMessageBox>
#include <QFileDialog>
#include <QAction>
#include <QActionGroup>
#include <QDebug>
#include <QString>
#include <QComboBox>
#include <QSettings>
#include <assert.h>

#include "main_interaction.h"
#include "marmeas_gui/graphics_scene.h"
#include "help.h"
#include "image_context/image_context_view.h"
#include "object_base_read_write.h"
#include "object_plugin.h"

#include "marmeas_core/image_context/image_context_read_write.h"

#include "injected_defines.h"

MainInterAction::MainInterAction(QWidget *parent)
    : QMainWindow(parent)
{

	setupWidgets();

	status_and_history_ = StatusAndHistory::getInstance();
	updateHistoryMenuEntries(); // read in the history settings

	wireInitialEvents();

	setupObjectTemplates();
	troggleSaveFile(false);  // there is nothing to save at startup

	readSettings(); // adapt to previous window size

}

void MainInterAction::setupWidgets() {
	ui_.setupUi(this);

	config_input_dialog_ = 0;
	object_data_dialog_ = 0;
	image_context_ = ImageContext::getInstance();


	setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
	setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

	object_data_view_ = new ObjectDataView(this);
	addDockWidget(Qt::RightDockWidgetArea,object_data_view_);
	// extend the menu dynamically to allow visibility control via menu
	ui_.menuView->addAction(object_data_view_->toggleViewAction());

	image_data_view_ = new ImageContextView(this);
	addDockWidget(Qt::BottomDockWidgetArea,image_data_view_);
	// extend the menu dynamically to allow visibility control via menu
	ui_.menuView->addAction(image_data_view_->toggleViewAction());


	graphics_scene_ = new GraphicsScene();
    graphics_view_ = ui_.graphicsView;
    graphics_view_->setScene(graphics_scene_);

}

void MainInterAction::wireInitialEvents() {
	connect(ui_.actionOpen,SIGNAL(triggered()),this, SLOT(openFile()));
	connect(ui_.actionSave,SIGNAL(triggered()),this, SLOT(saveFile()));
	connect(ui_.actionSave_As,SIGNAL(triggered()),this, SLOT(saveFileAs()));
	connect(ui_.actionClose,SIGNAL(triggered()),this, SLOT(closeData()));

	connect(ui_.actionAboutIcomm,SIGNAL(triggered()),this, SLOT(aboutIcomm()));
	connect(ui_.actionAboutQt,SIGNAL(triggered()),this, SLOT(aboutQt()));
	connect(ui_.actionHelp,SIGNAL(triggered()),this, SLOT(help()));
	connect(ui_.actionExit,SIGNAL(triggered()),this, SLOT(exit()));


	// history files
	connect(ui_.action1file,SIGNAL(triggered()),this, SLOT(openHistoryFile1()));
	connect(ui_.action2file,SIGNAL(triggered()),this, SLOT(openHistoryFile2()));
	connect(ui_.action3file,SIGNAL(triggered()),this, SLOT(openHistoryFile3()));
	connect(ui_.action4file,SIGNAL(triggered()),this, SLOT(openHistoryFile4()));
	connect(ui_.action5file,SIGNAL(triggered()),this, SLOT(openHistoryFile5()));
	connect(ui_.action6file,SIGNAL(triggered()),this, SLOT(openHistoryFile6()));
	connect(ui_.action7file,SIGNAL(triggered()),this, SLOT(openHistoryFile7()));
	connect(ui_.action8file,SIGNAL(triggered()),this, SLOT(openHistoryFile8()));


	connect(ui_.actionImage_Context, SIGNAL(triggered()), this, SLOT(editImageContext()));
	connect(ui_.actionEdit_Object_Data, SIGNAL(triggered()), this, SLOT(editObjectData()));
	connect(ui_.actionDelete,SIGNAL(triggered()),this, SLOT(deleteObject()));
	connect(ui_.actionSelect,SIGNAL(triggered()),graphics_view_, SLOT(selectMode()));
	connect(ui_.actionNavigate,SIGNAL(triggered()),graphics_view_, SLOT(navigateMode()));
	connect(ui_.actionNavigate,SIGNAL(triggered()),this, SLOT(navigateMode()));
	connect(ui_.actionAdd_Object,SIGNAL(triggered()),this, SLOT(addObjectMode()));
	connect(ui_.actionSelect,SIGNAL(triggered()),this, SLOT(selectMode()));
	connect(ui_.actionAdd_Object,SIGNAL(triggered()),graphics_view_, SLOT(addMode()));

	connect(ui_.actionZoom_In,SIGNAL(triggered()),graphics_view_, SLOT(zoomIn()));
	connect(ui_.actionZoom_Out,SIGNAL(triggered()),graphics_view_, SLOT(zoomOut()));


	// edit menue
	QActionGroup* myActionGroup = new QActionGroup(this);
	// add the qt designer actions
	myActionGroup->addAction(ui_.actionAdd_Object);
	myActionGroup->addAction(ui_.actionSelect);
	myActionGroup->addAction(ui_.actionNavigate);
	// set the properties to exclusive
	myActionGroup->setExclusive(true);

	// options menue
	connect(ui_.actionImage_Context_Templates,SIGNAL(triggered()),
			this,SLOT(configInputCharacteristics()));
	connect(ui_.actionObject_Templates,SIGNAL(triggered()),
			this,SLOT(configInputCharacteristics()));

	connect(status_and_history_,SIGNAL(statusChanged(bool)),this,
			SLOT(troggleSaveFile(bool)));
	connect(status_and_history_,SIGNAL(historyChanged()),
			this, SLOT(updateHistoryMenuEntries()));

	connect(graphics_scene_,SIGNAL(objectSelected(GeometricObject*)),
			object_data_view_, SLOT(setObject(GeometricObject*)));

	connect(graphics_scene_,SIGNAL(imageAvailable(bool)),
			ui_.actionAdd_Object, SLOT(setEnabled(bool)));

	// notify the save button
	GeometricObjectContainer* goc =
			GeometricObjectContainerSingleton::getInstance();
	connect(goc,SIGNAL(objectsChanged()),
			status_and_history_, SLOT(objectDataChanged()));

	connect(this,SIGNAL(imageContextChanged()),
			image_data_view_, SLOT(updateImageContext()));


}



void MainInterAction::setupObjectTemplates() {

	QComboBox* object_templates = new QComboBox();

	object_template_container_ = new ObjectTemplateContainer(this,
			ui_.menuNext_Object, object_templates);
	GeometricObjectFactory *factory = 0;


	ObjectPluginManager* plugin_manager = ObjectPluginManager::getInstance();
	foreach (QString plugin_name, plugin_manager->pluginNames()) {

		qDebug() << "--MainInterAction::setupObjectTemplates--"
				 <<" plugin name" << plugin_name;


		ObjectInterface* plugin = plugin_manager->plugin(plugin_name);
		if (plugin) {
			const QStringList tmp = plugin->objectTemplates();
			foreach (const QString obj_templ, tmp) {
				qDebug() << "--MainInterAction::setupObjectTemplates--"
						 << " object template name: " << obj_templ;


				// get the factories
				factory = plugin->objectFactory(obj_templ);


				// set the message flow


				// make sure one factory is set; since it gets selected anyway
				graphics_scene_->setObjectFactory(factory);

				// register icons, menu entries, etc

				// as per convention the first icon is the default for
				// the object_template; the others are just derivatives
				// for custom object templates
				const QIcon* icon = plugin->icons(obj_templ);
				if ( icon[0].isNull())
					qDebug() << "--MainInterAction::setupObjectTemplates--"
					       << "Plugin has zero default Item: " << obj_templ;

				// register the template
				object_template_container_->registerGenericTemplate(
						factory,obj_templ ,icon[0]);
			}
		}
	}
	if (factory != 0)   graphics_scene_->setObjectFactory(factory);

    ui_.toolBar->addSeparator();
	ui_.toolBar->addWidget(object_templates);

	connect(object_template_container_,
			SIGNAL(newFactorySelected(GeometricObjectFactory*)),
			graphics_scene_,
			SLOT(setObjectFactory(GeometricObjectFactory*)));
	
}

void MainInterAction::setupObjectReaders(XmlStreamReader& reader) {
	ObjectPluginManager* plugin_manager = ObjectPluginManager::getInstance();
	foreach (QString plugin_name, plugin_manager->pluginNames()) {
		qDebug() << "--MainInterAction::setupObjectReaders--"
				 << " plugin name" << plugin_name;
		ObjectInterface* plugin = plugin_manager->plugin(plugin_name);
		if (plugin) {
			const QStringList tmp = plugin->objectTemplates();
			ObjectBaseReader*  object_reader = plugin->getXMLReader();
			object_reader->setScene(graphics_scene_);
			reader.registerReader((Reader*) object_reader);
			}
		}
}


MainInterAction::~MainInterAction()
{
	// cleanup
	if (object_data_dialog_) delete object_data_dialog_;
	// todo all the rest
}

void MainInterAction::navigateMode() {
//	ui_.actionNavigate->setChecked();
}

void MainInterAction::selectMode() {
//	ui_.actionNavigate->setChecked();
}

void MainInterAction::addObjectMode() {
//	ui_.actionNavigate->setChecked();
}

void MainInterAction::troggleSaveFile(bool enable) {
	ui_.actionSave->setEnabled(enable);
}

bool MainInterAction::closeData() {
	qDebug() << "--MainInterAction::closeData--" ;
	bool data_saved_flag = okToContinue(); // store date if required
	if (true) {

		image_context_->resetImageContext();
		image_context_->loadImageTemplateSettings("default");

		GeometricObjectContainer* oc =
				GeometricObjectContainerSingleton::getInstance();
		oc->removeAllObjects();

		graphics_scene_->setImage(0); // reset the image in the scene
		qDebug() << "--MainInterAction::closeData-- emit image context change";
		emit imageContextChanged();
	}
	return data_saved_flag;
}


void MainInterAction::exit() {
	delete (status_and_history_);  // i.e. save settings
	writeSettings();    // save the window size
	if (okToContinue()) close();
}
/*!
 * @brief present a dialogbox to decide if unsaved data shall be saved
 * 		  in case of yes, it saves the data
 * @return cancel is pressed --> false
 *         yes or no is pressed --> true
 */
bool MainInterAction::okToContinue() {
	if (status_and_history_->isImageDataChanged() ||
			status_and_history_->isObjectDataChanged() ) {
		int r = QMessageBox::warning (this, tr("MARMEAS"),
				tr("There are some modifications.\n"
				   "Do you want to save your changes?"),
				   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if ( r == QMessageBox::Yes )    return saveFile();
		if ( r == QMessageBox::No )     return true;
		if ( r == QMessageBox::Cancel ) return false;
	}
	return true; // nothing changed
}

void MainInterAction::openFile() {
	if (closeData()) {
		QString file_name = QFileDialog::getOpenFileName(this,
				tr("Open File"), ".",
				tr("Images (*.bmp *.jpg *.png);; Objects (*.xod *.xml)"));
		if (!file_name.isEmpty()) {
			if ( loadFile(file_name)) 	image_file_name_ = file_name;
		}
	}
}

bool MainInterAction::loadFile(const QString &file_name) {
	if (closeData()) {

		qDebug ("--MainInterAction::loadFile-- called");
		// could be either xml, xod or bmp, png, jpg
		QFileInfo file_info(file_name);
		if (!file_info.isFile() || !file_info.isReadable()) {
			QMessageBox::warning (this, tr("MARMEAS"),
					tr("The file does not exist or there are insufficient access rights."
					   ),
					   QMessageBox::Yes);
			// most likely this file comes from history
			// - still we do try to remove the entry
			qDebug ("--MainInterAction::loadFile-- file not readable");
			return false;
		}

		QString extension = file_info.completeSuffix();

		if ( ( extension == "bmp" ) ||
			 ( extension == "jpg" ) ||
			 ( extension == "png" ) ) {
			// this is an image
			qDebug () << "--MainInterAction::loadFile-- Load Image" << extension;
			return loadImageFileAndInteraction(file_name);
		}
		if ( ( extension == "xod" ) ||
			 ( extension == "xml") ) {
			// this is either a image context of object measurement file
			qDebug () << "--MainInterAction::loadFile-- Load Data" << extension;
			// register image context reader
			XmlStreamReader reader;
			ImageContextReader* icr = new ImageContextReader(image_context_);
			reader.registerReader((Reader*)icr);
			// register all plugin object reader
			setupObjectReaders(reader);
			if ( reader.readFile(file_name)) {
				qDebug() << "--MainInterAction::loadFile-- reading was successful";
				// register File name and directory
				image_context_->setObjectFileName(file_name);
				// read the image file as well
				if (!loadImageFile(image_context_->getImageFileName())) {
					qDebug() << "--MainInterAction::loadFile-- failed image read "
							<< image_context_->getImageFileName()
							<< " " << file_name;
					// image file reading was not successful
					QMessageBox::warning (this, tr("MARMEAS"),
							tr("Reading of image file failed - "
							   "File does not exist or insufficient access rights"
							   ),
							   QMessageBox::Ok);
					return false;
				}

				qDebug() << "--MainInterAction::loadFile-- emit image context change";
				emit imageContextChanged();

				status_and_history_->objectDataLoaded(file_name);
				statusBar()->showMessage(tr("File(s) loaded"), 5000);
				return true;
			}
			qDebug() << "--MainInterAction::loadFile-- reading failed";

			// reading was not successful
			QMessageBox::warning (this, tr("MARMEAS"),
					tr("Reading of Image Context/Object Data file failed - "
					   "read error"
					   ),
					   QMessageBox::Ok);
			return false;
		}
		// unhandled extension
		qDebug () << "--MainInterAction::loadFile-- unknown extension:"
				<< extension;
		QMessageBox::warning (this, tr("MARMEAS"),
				tr("Reading of Image Context/Object Data file failed - "
				   "unknown file extension"
				   ),
				   QMessageBox::Ok);
		return false;
	}
	return false; // closeData
}

bool MainInterAction::loadImageFile(const QString &file_name) {
	// actually load the image file
	// set the background
	// and enable interaction
	qDebug() << "--MainInterAction::loadImageFile--"
			 << " called with: "
			 << file_name;
	QImage* image = new QImage;
	if (image->load(file_name)) {
		// pass on the image
		qDebug() << "--MainInterAction::loadImageFile-- passed";

		assert (image);
		graphics_scene_->setImage(image);
		return true;
	}
	qDebug() << "--MainInterAction::loadImageFile-- failed";
	return false;
}

	/*
	 * Load the image file
	 * update the status and
	 * fire the image context dialog
	 */
bool MainInterAction::loadImageFileAndInteraction(const QString &file_name) {

	qDebug() << "--MainInterAction::loadImageFileAndInteraction-- called";
	if (loadImageFile(file_name)) {
		qDebug() << "--MainInterAction::loadImageFileAndInteraction--"
				 << " handle new image file"
				 << file_name;
		statusBar()->showMessage(tr("Image File loaded"), 5000);
		status_and_history_->imageLoaded(file_name);
		// assumption is that the image context was already reset (if required)
		image_context_->setImageFileName(file_name);
		if (!image_context_->requiresEdits()) 	editImageContext();
		else emit imageContextChanged();
		return true;
	} else {
		qDebug() << "--MainInterAction::loadImageFileAndInteraction--"
				<< " failed image read "
				<< file_name;
		// image file reading was not successful
		QMessageBox::warning (this, tr("MARMEAS"),
				tr("Reading of image file failed - "
				   "File does not exist or insufficient access rights"
				   ),
				   QMessageBox::Ok);
		return false;
	}
}

void MainInterAction::editImageContext() {

		// invoke the input dialog
		ImageContextDialog*  input_image_dialog;
		input_image_dialog = new ImageContextDialog(this, image_context_);
		connect(input_image_dialog,SIGNAL(imageContextChanged()),
				status_and_history_, SLOT(imageContextChanged()));
		connect(input_image_dialog,SIGNAL(imageContextChanged()),image_data_view_,
				SLOT(updateImageContext()));
		input_image_dialog->show();
		// ToDo cleanup - where is it done - certainly not here
		// because it does not do anything

		/*
		disconnect(input_image_dialog,SIGNAL(imageContextChanged()),
				status_and_history_, SLOT(imageContextChanged()));
		disconnect(this,SIGNAL(imageContextChanged()),image_data_view_,
				SLOT(updateImageContext()));
		delete input_image_dialog;
		*/

}

void MainInterAction::editObjectData() {
	GeometricObject* object = graphics_scene_->emitSelectedObject();
	if (object) {
		// invoke the input dialog
		if ( !object_data_dialog_ ) {
			object_data_dialog_ = new ObjectDataDialog(this);
			connect(object_data_dialog_, SIGNAL(objectDataChanged()),
					object_data_view_ , SLOT(update()));
		}
		object_data_dialog_->setObject(object);
		object_data_dialog_->show();
	} else {
		statusBar()->showMessage(tr("no item selected - select first"), 5000);
	}
}


void MainInterAction::deleteObject() {
	GeometricObject* object = graphics_scene_->emitSelectedObject();
	if (object) {
		graphics_scene_->deleteItem();
	} else {
		statusBar()->showMessage(tr("no item selected for deletion- select first"), 5000);
	}
}


bool MainInterAction::saveFileAs() {
	//QFileDialog dialog;
	//dialog.setAcceptMode(QFileDialog::AcceptSave);
	QString file_name = QFileDialog::getSaveFileName(this,
			tr("Write Data"), ".",
			tr("Image Context File (*xic);;Object File (*.xod)" )
			);

	bool result =  saveFile(file_name);
	image_context_->setObjectFileName(file_name);
	qDebug() << "--MainInterAction::saveFileAs-- emit image context change";
	emit imageContextChanged();
	return result;
}

bool MainInterAction::saveFile(QString file_name) {
	bool force_saving = true;
	QString objects_file_name;

	if (file_name == "" ) {
		ImageContext* ic = ImageContext::getInstance();
		objects_file_name = ic->getObjectFileName();
	} else {
		objects_file_name = file_name;
		QFileInfo file_info(file_name);
		// complete the names if required
		if ( file_info.completeSuffix() != "xod" )
			objects_file_name += ".xod";
	}
	qDebug() << "--MainInterAction::saveFile-- filename: "
			 << objects_file_name;

	// write the annotated objects
	XmlStreamWriter writer;
	// write the image context data
	ImageContextWriter icw(image_context_);
	writer.registerWriter((Writer*)(&icw));
	// write the object data
	ObjectContainerWriter ocw;
	writer.registerWriter((Writer*)(&ocw));
	if (!writer.writeFile(objects_file_name)) {
		QMessageBox::warning (this, tr("MARMEAS"),
				tr("Writing of File failed: %1").arg(
				   objects_file_name ),
				   QMessageBox::Yes);
		qDebug() << "--MainInterAction::saveFile-- saving failed: "
				 << objects_file_name;
		return false;
	} else {
		statusBar()->showMessage(tr("Object File saved"), 2000);
		status_and_history_->objectDataSaved(objects_file_name);
	}
	return true;
}


bool MainInterAction::configInputCharacteristics() {
	if (config_input_dialog_ == 0) {
		qDebug() <<"--MainInterAction::configInputCharacteristics--";
		config_input_dialog_ = new ConfigInputCharacteristicsDialog(this);
	}
	config_input_dialog_->show();
	config_input_dialog_->raise();
	config_input_dialog_->activateWindow();
	qDebug("--MainInterAction::configInputCharacteristics-- done");
}



QAction* MainInterAction::getHistoryEntry(int index) {
	switch (index) {
	case 0:
		return ui_.action1file;
	case 1:
		return ui_.action2file;
	case 2:
		return ui_.action3file;
	case 3:
		return ui_.action4file;
	case 4:
		return ui_.action5file;
	case 5:
		return ui_.action6file;
	case 6:
		return ui_.action7file;
	case 7:
		return ui_.action8file;
	}
    throw "wrong history file index";   // index failure
}

void MainInterAction::openHistoryFile1() {
	qDebug() << "MainInterAction::openHistoryFile1";
	QString file_name = status_and_history_->getHistoryFileName(0);
	loadFile(file_name);
	return;
}

void MainInterAction::openHistoryFile2() {
	qDebug() << "MainInterAction::openHistoryFile2";
	QString file_name = status_and_history_->getHistoryFileName(1);
	loadFile(file_name);
	return;
}

void MainInterAction::openHistoryFile3() {
	qDebug() << "MainInterAction::openHistoryFile3";
	QString file_name = status_and_history_->getHistoryFileName(2);
	loadFile(file_name);
	return;
}

void MainInterAction::openHistoryFile4() {
	qDebug() << "MainInterAction::openHistoryFile4";
	QString file_name = status_and_history_->getHistoryFileName(3);
	loadFile(file_name);
	return;
}

void MainInterAction::openHistoryFile5() {
	qDebug() << "MainInterAction::openHistoryFile5";
	QString file_name = status_and_history_->getHistoryFileName(4);
	loadFile(file_name);
	return;
}

void MainInterAction::openHistoryFile6() {
	qDebug() << "MainInterAction::openHistoryFile6";
	QString file_name = status_and_history_->getHistoryFileName(5);
	loadFile(file_name);
	return;
}

void MainInterAction::openHistoryFile7() {
	qDebug() << "MainInterAction::openHistoryFile7";
	QString file_name = status_and_history_->getHistoryFileName(6);
	loadFile(file_name);
	return;
}

void MainInterAction::openHistoryFile8() {
	qDebug() << "MainInterAction::openHistoryFile8";
	QString file_name = status_and_history_->getHistoryFileName(7);
	loadFile(file_name);
	return;
}

void MainInterAction::MainInterAction::updateHistoryMenuEntries() {
	int i;
	for (i= 0; i < 8; i++ )
		if ( i < status_and_history_->getMaxNumberOfHistoryEntries() ) {
			getHistoryEntry(i)->setVisible(true);
			QString text = tr("&%1 %2")
					.arg(i+1)
					.arg(status_and_history_->getHistoryFileName(i));
			getHistoryEntry(i)->setText(text);
			getHistoryEntry(i)->setData(
					status_and_history_->getHistoryFileName(i));
		}
		else
			getHistoryEntry(i)->setVisible(false);
}

void MainInterAction::help() {
	HelpBrowserDialog* help_browser_dialog = new HelpBrowserDialog();
	help_browser_dialog->showPage("");
	help_browser_dialog->show();
	// ToDo figure out where the signal gets disconnected
}

void MainInterAction::aboutIcomm() {
	QMessageBox::about(this, tr("About MARMEAS"),
			tr("<h2>MARMEAS </h2>"
			   "<p>Version %1 </p>"
			   "<p><table>"
			   "<tr><td>Copyright &copy;</td>"
			   "<td>2012 Volker Kempert</td></tr>"
			   "<tr><td> </td><td>All rights reserved</td></tr>"
			   "</table></p>"
			   "<p/>"
			   "<p><b>MARMEAS - MARking and MEASurement of objects on images</b> "
			   "is an application allowing to define objects on images manually "
			   "by marking them on those images and allowing to derive "
			   "geometric and intensity measures about the objects. "
			   "It is specifically helpful in industrial automation projects.</p>"
			   "<p/>"
			   "<p>This program is licensed under the "
				"GNU General Public License (GPL). "
				"See the <a href=\"http://www.gnu.org/licenses/gpl.html\">License Terms</a> for details.</p>"
               "<p/>"

			 ).arg(QString(__version__)));


}

void MainInterAction::aboutQt() {
	QMessageBox::aboutQt(this,tr("About Qt"));


}

void MainInterAction::readSettings() {
	qDebug() << "--MainInterAction::readSettings-- called";
	QSettings settings;
	settings.beginGroup("mainWindow");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("windowState").toByteArray());
	settings.endGroup();
}

void MainInterAction::writeSettings() {
	qDebug() << "--MainInterAction::writeSettings-- called";
	QSettings settings;
	settings.beginGroup("mainWindow");
	settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());

	settings.endGroup();
}


