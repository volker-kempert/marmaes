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


#ifndef OBJECT_DATA_DIALOG_H_
#define OBJECT_DATA_DIALOG_H_

#include "object_base.h"
#include "ui_object_data_edit.h"

class ObjectDataDialog : public QDialog {

	Q_OBJECT

public:
	ObjectDataDialog(QWidget *parent = 0 );
	void setObject(GeometricObject* object);
	~ObjectDataDialog();

private slots:
	void apply();      // take all the changes and the model
	void activateApply() { ui.actionApply->setEnabled(true); }

signals:
	void objectDataChanged();

private:
	void applyMeasurements(QTreeWidgetItemIterator& it
			,MeasurementOnObject *measurement  );
	void updateMeasurements(MeasurementOnObject *measurements,
			QTreeWidgetItem* item);

	GeometricObject* object_;
    Ui::ObjectDataDialog ui;
};


#endif /* OBJECT_DATA_DIALOG_H_ */
