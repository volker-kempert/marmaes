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


#ifndef OBJECT_DATA_VIEW_H
#define OBJECT_DATA_VIEW_H

#include <QtGui/QDockWidget>
#include <QObject>
#include "ui_object_data_view.h"
#include "object_base.h"
#include "3rdparty/plotter/simple_plotter.h"

class ObjectDataView : public QDockWidget
{
    Q_OBJECT

public:
    ObjectDataView(QWidget *parent = 0, Qt::WindowFlags flags = 0 );
    ~ObjectDataView();

public slots:
	void setObject (GeometricObject * obj );
	void update ();
	void trogglePlot();

private:
	void updateObject();
	void updateGraphics(bool isAllowed);
	void updateMeasurements();
	void updateGeometryMeasurements(GeometricMeasurement *measurements,
			QTreeWidgetItem* item);
	void updateIntensityMeasurements(IntensityMeasurement *measurements,
			QTreeWidgetItem* item);
    Ui::ObjectSourceMarking ui_;
    GeometricObject* object_;

    QTreeWidgetItem *general_;
    QTreeWidgetItem *model_;
    QTreeWidgetItem *measurements_;
    SimplePlotter *plotter_;


};

#endif // OBJECT_DATA_VIEW_H
