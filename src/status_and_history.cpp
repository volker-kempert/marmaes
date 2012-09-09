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


#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QSettings>

#include "status_and_history.h"

StatusAndHistory* StatusAndHistory::the_status_and_history_ = 0;

StatusAndHistory* StatusAndHistory::getInstance() {
    if (!the_status_and_history_) {
    	qDebug() << "--StatusAndHistory::getInstance--";
        the_status_and_history_ = new StatusAndHistory();
    }
    return the_status_and_history_;
}

StatusAndHistory::StatusAndHistory() :
	max_alloc_history_entries_(8)      // !!hard coded fitting to main.ui!!
{
	max_history_entries_ = 0;
	history_ = new QString[max_alloc_history_entries_];
	readSettings();
	object_data_changed_ = false;
	image_context_changed_ = false;



}

StatusAndHistory::~StatusAndHistory() {
	if (the_status_and_history_ == this) {
		writeSettings();
		delete[] history_;
		the_status_and_history_ = 0;
	}
}

void StatusAndHistory::readSettings() {
	 QSettings settings;
	 max_history_entries_ = settings.beginReadArray("FileHistory");
	 for (int i = 0; i < max_history_entries_; i++) {
		 settings.setArrayIndex(i);
		 history_[i] = settings.value("FileName").toString();
	 }
	 settings.endArray();
	 settings.beginGroup("Files");
	 last_directory_  = settings.value("LastDirectory").toString();
	 settings.endGroup();
}

void StatusAndHistory::writeSettings() {
	 QSettings settings;
	 settings.beginWriteArray("FileHistory");
	 for (int i = 0; i < max_history_entries_; i++) {
	     settings.setArrayIndex(i);
	     settings.setValue("FileName", history_[i]);
	 }
	 settings.endArray();
	 settings.beginGroup("Files");
	 settings.value("LastDirectory", last_directory_);
	 settings.endGroup();
}

//-----------------------------------------------------------------------------

/*!
 * returns the last directory in use either from loading or saving data
 *
 */
QString StatusAndHistory::getLastDir() {
	return last_directory_;
}


/*
 * split off the directory portion of a file name and store it
 * inside the object
 */
void StatusAndHistory::registerDirectory(QString file_name) {
	QFileInfo file_info = QFileInfo(file_name);
	last_directory_ = file_info.absoluteDir().canonicalPath();
}


QString StatusAndHistory::getHistoryFileName(int index) {
	if (index < 0 or index >= max_history_entries_ )
		throw "getHistoryFileType: out of range";
    return history_[index];
}

/*!
 * @brief register a new image to that was loaded to the history
 *        mark the status as unchanged
 */
void StatusAndHistory::imageLoaded(QString file_name){
	addHistoryEntry(file_name);
	registerDirectory(file_name);
	image_context_changed_ = false;
	object_data_changed_ = false;
	emit statusChanged(false);

}

/*!
 * @brief register a new object data file to that was loaded to the history
 *        mark the status as unchanged
 */
void StatusAndHistory::objectDataLoaded(QString file_name){
	addHistoryEntry(file_name);
	registerDirectory(file_name);
	image_context_changed_ = false;
	object_data_changed_ = false;
	emit statusChanged(false);
}

void StatusAndHistory::imageContextChanged(){
	image_context_changed_ = true;
	qDebug() << "--StatusAndHistory::imageContextChanged-- called";
	emit statusChanged(true);
}

void StatusAndHistory::objectDataChanged(){
	qDebug() << "--StatusAndHistory::objectDataChanged-- called";
	object_data_changed_ = true;
	emit statusChanged(true);
}
void StatusAndHistory::objectDataReset(){
	object_data_changed_ = false;
	qDebug() << "--StatusAndHistory::objectDataReset-- called";
	if (!image_context_changed_) emit statusChanged(false);
}

void StatusAndHistory::objectDataSaved(QString file_name){
	addHistoryEntry(file_name);
	registerDirectory(file_name);
	object_data_changed_ = false;
	image_context_changed_ = false;
	if (!object_data_changed_) emit statusChanged(false);
	if (!image_context_changed_) emit statusChanged(false);
}

/*
 * handles new entries to the history
 * if new entry not available:
 *    add the entry to the top (index 0)
 *    the latest might be thrown away
 * if entry is available
 *    get it to 1st rank (index 0)
 */
void StatusAndHistory::addHistoryEntry(QString file) {
	bool rearrange = false;
	int i;
	for (i = 0; i < max_history_entries_ ; i++)
		if ( history_[i] == file ) {
			rearrange = true; break; // entry is there; get it to the top
		}
	if (rearrange)
		for (int j = i ; j > 0 ; j--) history_[j] = history_[j-1];
	else {
		if (max_history_entries_ < max_alloc_history_entries_)
			max_history_entries_++;
		for (int j = max_history_entries_ - 1; j > 0; j--)
			history_[j] = history_[j-1];
	}
	history_[0] = file;
	emit historyChanged();
}
