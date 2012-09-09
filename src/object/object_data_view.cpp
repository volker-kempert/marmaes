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


#define _USE_MATH_DEFINES

#include <math.h>

#include <QDebug>
#include <QObject>
#include <QMetaProperty>
#include <QString>

#include <QVector>
#include <QPointF>


#include "object_data_view.h"
#include "marmeas_core/measurement/point_clowd.h"
#include "marmeas_core/measurement/quartile.h"
#include "marmeas_core/image_context/image_context.h"

/*
 * convert a qvariant into a real measure considering units of measurement
 * @param QVariant to convert to a QString
 * @param precision number of digits after the .  -1 for not considered
 *        default is zero
 * @param Conversation factor (optional) if not given 1.0 is used
 * @param Unit of measurement (optional) if not given ""
 *
 * @return the converted string "" if not handled correctly
 */
const QString convertQVariant(QVariant v,int precision = 0, double conv= 1.0, QString unit= "") {
	QString converted = "";
	bool flag;
	double x,y,d;
	double b,t,l,r;
	switch (v.type()) {
	case QVariant::PointF:
		x = double(v.toPointF().x()) * conv;
		y = double(v.toPointF().y()) * conv;
		converted = QString("(%1 %2,%3 %4)").arg(x,0,'f',precision).arg(unit).arg(y,0,'f',precision).arg(unit);
		break;
	case QVariant::UserType:
		converted = v.value<Quartile>().toString();
		break;
	case QVariant::RectF:
		l = double(v.toRectF().left()) * conv;
		r = double(v.toRectF().right()) * conv;
		t = double(v.toRectF().top()) * conv;
		b = double(v.toRectF().bottom()) * conv;

		converted = QString("(%1 %2,%3 %4)-(%5 %6,%7 %8)").arg(t,0,'f',precision).arg(unit).arg(l,0,'f',precision).arg(unit).arg(b,0,'f',precision).arg(unit).arg(r,0,'f',precision).arg(unit);
		break;
	case QVariant::Double:
		d = v.toReal(&flag) * conv;
		converted = QString("%1 %2").arg(d,0,'f',precision).arg(unit);
		break;
	case QVariant::String:
		converted = v.toString();
		break;
	}
	return converted;
}




ObjectDataView::ObjectDataView(QWidget *parent, Qt::WindowFlags flags )
: QDockWidget(parent,flags)
{
	qDebug() << "--ObjectDataView::ObjectDataView-- called";
	ui_.setupUi(this);
	object_ = 0;

	// wire the connections
	connect(ui_.plot,SIGNAL(clicked()),this, SLOT(trogglePlot()));
	connect(ui_.removeObject,SIGNAL(clicked()), parent, SLOT(deleteObject()));
	connect(ui_.editObject,SIGNAL(clicked()), parent, SLOT(editObjectData()));

	update();

}

ObjectDataView::~ObjectDataView()
{
	// no treewidget to delete since it is done by clear automatically
}

/*
 * sets the object to view and updates the view
 * zero indicates an empty object
 */
void ObjectDataView::setObject (GeometricObject * object) {
	qDebug() << "--ObjectDataView::setObject-- "
			 << "receive new object "  <<	(unsigned long)object;
	object_ = object;
	update();
}

void ObjectDataView::update () {
	ui_.treeWidget->clear();   // remove and delete old existing content
	general_ = new QTreeWidgetItem(ui_.treeWidget);
	general_->setText(0,tr("General Data"));
	general_->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);

	model_ = new QTreeWidgetItem(ui_.treeWidget);
	model_->setText(0,tr("Geometric Model"));
	model_->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	measurements_ = new QTreeWidgetItem(ui_.treeWidget);
	measurements_->setText(0,tr("Measurements"));
	measurements_->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);



	updateGraphics (false);

	if (object_ == 0) {
		ui_.editObject->setEnabled(false);
		ui_.removeObject->setEnabled(false);
	} else {
		ui_.editObject->setEnabled(true);
		ui_.removeObject->setEnabled(true);

		updateObject();
		updateMeasurements();
	}

	general_->setExpanded(true);
	model_->setExpanded(true);
	measurements_->setExpanded(true);
	ui_.treeWidget->resizeColumnToContents( 0 );
	ui_.treeWidget->resizeColumnToContents( 1 );
}

void ObjectDataView::updateObject() {

	QString tmp;
	QTreeWidgetItem *item;
	item = new QTreeWidgetItem(general_);
	item->setText(0,tr("Identifier"));
	item->setText(1,QString::number (object_->getId()));

	tmp = object_->getName();
	if (tmp != "") {
		item = new QTreeWidgetItem(general_);
		item->setText(0,tr("Name"));
		item->setText(1,tmp);
	}

	tmp = object_->getSource();
	if (tmp != "") {
		item = new QTreeWidgetItem(general_);
		item->setText(0,tr("Source"));
		item->setText(1,tmp);
	}

	item = new QTreeWidgetItem(general_);
	item->setText(0,tr("Type"));
	item->setText(1,object_->getType());

	// geometric model data

	ImageContext* context = ImageContext::getInstance();

	for (int i = 0; i < 4; i++)
		if ((tmp = object_->getModelParameterName(i)) != "" ) {
			item = new QTreeWidgetItem(model_);
			item->setText(0,tmp);
			QVariant variant = object_->getModelParameterData(i);
			item->setText(1,convertQVariant(variant,1,
				context->getConversionFactor(),
				context->getUnitOfMeasurement()));
		}
		else
			break;

}

void ObjectDataView::updateMeasurements() {
		MeasurementOnObject *measurements = object_->getMeasurement();
		int measures = measurements->actualMeasures();
		qDebug() << "--ObjectDataView::updateMeasurements-- act. Measures"
				 << measures ;

		MeasurementType k;
		for (k = 4 ;  k < MeasurementOnObject::AllMeasures ; k = k << 1)
			if (k & measures) {
				QTreeWidgetItem* item = new QTreeWidgetItem(measurements_);
				item->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
				item->setExpanded(true);
				item->setText(0,measurements->measurementName(k));
				qDebug() << "--ObjectDataView::update-- name retrieved";
				QVariant v = measurements->measurementValue(k);
				qDebug() << "--ObjectDataView::update-- value retrieved";
				if (v.canConvert<GeometricMeasurement*>()) {
					GeometricMeasurement* gm = v.value<GeometricMeasurement*>();
					updateGeometryMeasurements(gm,item);
				}
				if (v.canConvert<IntensityMeasurement*>()) {
					IntensityMeasurement* im = v.value<IntensityMeasurement*>();
					updateIntensityMeasurements(im,item);
				}
			}

}

void ObjectDataView::updateGeometryMeasurements(
		GeometricMeasurement *measurements,
		QTreeWidgetItem* group_header) {
	int measures = measurements->actualMeasures();
	qDebug() << "--ObjectDataView::update-- act. Measures" << measures ;
	ImageContext* context = ImageContext::getInstance();

	if (IntensityMeasurement::Graphic & measures)	updateGraphics (true);

	for (MeasurementType k = 4 ;  k < MeasurementOnObject::AllMeasures; k = k << 1) {
		if (k & measures) {
			QTreeWidgetItem* item = new QTreeWidgetItem(group_header);
			item->setText(0,measurements->measurementName(k));
			qDebug() << "--ObjectDataView::update-- name retrieved";
			QVariant variant = measurements->measurementValue(k);
			qDebug() << "--ObjectDataView::update-- value retrieved";

			switch (k) {
			// basic unit
			case GeometricMeasurement::GeometricMidpoint :
			case GeometricMeasurement::GeometricXMaxDimension :
			case GeometricMeasurement::GeometricYMaxDimension :
			case GeometricMeasurement::GeometricBoundingBox :
			case GeometricMeasurement::GeometricMaxDimension :
			case GeometricMeasurement::GeometricMinDimension :
			case GeometricMeasurement::GeometricCircumFerence :

				item->setText(1,convertQVariant(variant,1,
					context->getConversionFactor(),
					context->getUnitOfMeasurement()));

				break;
			// square unit
			case GeometricMeasurement::GeometricSurface :
				{
				QString tmp = context->getUnitOfMeasurement();
				QString unit = tmp + " x " + tmp;
				double conversation = context->getConversionFactor();
				conversation *= conversation;
				item->setText(1,convertQVariant(variant,1,
					conversation, unit));
				}
				break;
			// angle measurement in degree
			case GeometricMeasurement::GeometricMaxDimensionAngle  :
			case GeometricMeasurement::GeometricMinDimensionAngle  :
				{
				QString unit("degree");
				double conversation = 180.0 / M_PI;
				item->setText(1,convertQVariant(variant,1,
					conversation, unit));
				}
				break;

			default:
				item->setText(1,"default not implemented yet");
			}
		}

	}
}

void ObjectDataView::updateGraphics(bool isAllowed) {
	if (!isAllowed) {
		ui_.plot->setEnabled(false);
		ui_.plotarea->hide();
	} else {
		ui_.plot->setEnabled(true);
		ui_.plotarea->show();
	}
}

void ObjectDataView::updateIntensityMeasurements(
		IntensityMeasurement *measurements,
		QTreeWidgetItem* group_header) {

	int measures = measurements->actualMeasures();
	qDebug() << "--ObjectDataView::updateIntensityMeasurements-- act. Measures"
			<< measures ;
	measurements->computeProfile();
	measurements->computeHistogram();

	if (IntensityMeasurement::Graphic & measures) {
		updateGraphics (true);
		qDebug() << "--ObjectDataView::update-- set plot data - profile: "
				<< " address " << (long)(measurements->profile());

		PointClowd point_clowd(measurements->profile());
		PlotSettings plot_settings;
		plot_settings.max_x_ = point_clowd.maxX();
		plot_settings.max_y_ = point_clowd.maxY();
		plot_settings.min_x_ = point_clowd.minX();
		plot_settings.min_y_ = point_clowd.minY();
		ui_.plotarea->setPlotSettings(plot_settings);
		ui_.plotarea->setCurveData(2, *(measurements->profile()));
	}

	for (MeasurementType k = 4 ;  k < MeasurementOnObject::AllMeasures ; k = k << 1) {
		if (k & measures) {
			QTreeWidgetItem* item = new QTreeWidgetItem(group_header);
			item->setText(0,measurements->measurementName(k));
			qDebug() << "--ObjectDataView::update-- name retrieved";
			QVariant variant = measurements->measurementValue(k);
			qDebug() << "--ObjectDataView::update-- value retrieved";

			switch (k) {

			case IntensityMeasurement::IntensityMean :
			case IntensityMeasurement::IntensityVariance   :
			case IntensityMeasurement::IntensitySlope  :
			case IntensityMeasurement::IntensityMin :
			case IntensityMeasurement::IntensityMax :
			case IntensityMeasurement::IntensityMedian :
			case IntensityMeasurement::IntensityQuartiles :
				{
				QString unit("Grey Level");
				double conversation = 1;
				item->setText(1,convertQVariant(variant,1,
					conversation, unit));
				}
				break;

			default:
				item->setText(1,"default not implemented yet");
			}
		}

	}
}


/**
 * @brief switch on/off of the plot area
 */
void ObjectDataView::trogglePlot() {
	qDebug() << "--ObjectDataView::trogglePlot-- called";
	ui_.plotarea->setVisible(!ui_.plotarea->isVisible());
}

