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


#ifndef STATUS_AND_HISTORY_H_
#define STATUS_AND_HISTORY_H_

#include <QString>
#include <QObject>

/*!
 * @brief
 *
 * Singleton to take care for application specific states and historical states
 *
 * Responsible for maintaining the recently opened and saved files
 *
 * Responsible for the decision if there are unsaved changes
 *
 */
class StatusAndHistory : public QObject {

	Q_OBJECT

public:
	static StatusAndHistory* getInstance();
	~StatusAndHistory();


	// History access
	int getMaxNumberOfHistoryEntries() {  return max_history_entries_ ; }
	QString getHistoryFileName(int index);

	// Status Queries
	bool isObjectDataChanged() { return object_data_changed_; }
	bool isImageDataChanged()  { return image_context_changed_;  }


	// retrieve information
	QString getLastDir();

public slots:

	// Status changes
	void imageLoaded(QString file_name_);
	void objectDataLoaded(QString file_name_);
	void objectDataSaved(QString file_name_);

	void imageContextChanged();
	void objectDataChanged();
	void objectDataReset();

signals:
	void statusChanged(bool flag);
	void historyChanged();

private:

	// Application close is handled by destroying the object
	// therefore there is no explicit "applicationClose" change method
    static StatusAndHistory* the_status_and_history_;
	StatusAndHistory();
	void readSettings();
	void writeSettings();
	void addHistoryEntry(QString s);
	void registerDirectory(QString file_name);

	QString last_directory_;

	bool object_data_changed_;
	bool image_context_changed_;

	const int max_alloc_history_entries_;
	int max_history_entries_;
	QString* history_;


};

#endif /* STATUS_AND_HISTORY_H_ */
