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


#include <QDebug>
#include <QTreeWidgetItem>
#include <assert.h>

#include "object_data_dialog.h"
#include "measurement_on_object.h"

ObjectDataDialog::ObjectDataDialog( QWidget *parent )
	: QDialog(parent)
{
	object_ = 0;

	ui.setupUi(this);
	ui.actionApply->setEnabled(false);

	// set up edit restrictions
	QRegExp word_exp("[a-zA-Z0-9]*");
	QRegExp id_exp("[0-9]*");
	ui.editObjectName->setValidator(new QRegExpValidator(word_exp, this));
	ui.editObjectId->setValidator(new QRegExpValidator(id_exp, this));

	// do the wiring
	connect(ui.editObjectName,SIGNAL(textChanged(QString)),
			this, SLOT(activateApply()));
	connect(ui.editObjectId,SIGNAL(textChanged(QString)),
			this, SLOT(activateApply()));
	connect(ui.treeWidget,SIGNAL(itemChanged(QTreeWidgetItem *, int)),
			this, SLOT(activateApply()));
	connect(ui.actionClose, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.actionApply, SIGNAL(clicked()), this, SLOT(apply()));
}


void ObjectDataDialog::setObject(GeometricObject* object) {
	object_ = object;
	qDebug() << "--ObjectDataDialog::setObject--: " << (long)object_;
	assert(object_);
	ui.editObjectName->setText(object_->getName());
	ui.editObjectId->setText(QString("%1").arg(object_->getId()));
	ui.labelObjectBaseType->setText(object_->getType());

	// clear the list
	ui.treeWidget->clear();   // remove and delete old existing content

	// insert the object data
	MeasurementOnObject* measurements = object_->getMeasurement();

	MeasurementType available = measurements->availableMeasures();
	MeasurementType actual = measurements->actualMeasures();
	qDebug() << "--ObjectDataDialog::setObject-- available: " << available;
	qDebug() << "--ObjectDataDialog::setObject-- actual   : " << actual;
	MeasurementType m;


	for (m = 2  ; m < MeasurementOnObject::AllMeasures; m = m << 1) {
		if (m & available) {

			QTreeWidgetItem *item = new QTreeWidgetItem(ui.treeWidget);
			item->setExpanded(true);
			item->setFlags( Qt::ItemIsUserCheckable | Qt::ItemIsEnabled |
					Qt::ItemIsTristate);

			item->setText(0, measurements->measurementName(m));
			if ( m & actual  )
				item->setCheckState(0, Qt::Checked);
			else
				item->setCheckState(0, Qt::Unchecked);
			QVariant v = measurements->measurementValue(m);

			// do the sub items
			qDebug() << "--ObjectDataView::update-- value retrieved";
			if (v.canConvert<GeometricMeasurement*>()) {
				GeometricMeasurement* gm = v.value<GeometricMeasurement*>();
				updateMeasurements(gm,item);
			}
			if (v.canConvert<IntensityMeasurement*>()) {
				IntensityMeasurement* im = v.value<IntensityMeasurement*>();
				updateMeasurements(im,item);
			}
		}
	}
	ui.actionApply->setEnabled(false);
}

void ObjectDataDialog::updateMeasurements(
		MeasurementOnObject *measurements,
		QTreeWidgetItem* group_header) {

	MeasurementType actual = measurements->actualMeasures();
	MeasurementType available = measurements->availableMeasures();
	qDebug() << "--ObjectDataDialog::updateMeasurements-- actual "
			<< actual << " available " << available ;

	for (MeasurementType k = 2 ; k < MeasurementOnObject::AllMeasures ; k = k << 1) {
		if (k & available) {
			QTreeWidgetItem* item = new QTreeWidgetItem(group_header);
			item->setText(0,measurements->measurementName(k));
			if ( k & actual  )
				item->setCheckState(0, Qt::Checked);
			else
				item->setCheckState(0, Qt::Unchecked);
		}

	}
}


void ObjectDataDialog::apply() {
	qDebug("--ObjectDataDialog::apply--: apply pressed");
	object_->setName(ui.editObjectName->text());
	long id;
	bool tmp_bool;
	QString tmp_id = ui.editObjectId->text();
	id = tmp_id.toLong(&tmp_bool);
	object_->setId(id);

	// determine the measurement settings
	// the digit position in MeasurementType definition
	MeasurementOnObject* measurement = object_->getMeasurement();

	// 2 is before the dynamic type starts for composed measurement
	MeasurementType composed_measurement = 2;
	QTreeWidgetItemIterator it(ui.treeWidget);
	 while (*it) {
		QTreeWidgetItem* item = (*it)->parent();
		int top_level_index = ui.treeWidget->indexOfTopLevelItem(*it);
		if (top_level_index != -1 ) {
			// this is a toplevel item - i.e. always aligned to composed widget
			composed_measurement = composed_measurement << 1;
			qDebug() << "--ObjectDataDialog::apply-- "
		             << " dialog text " << (*it)->text(0)
			         << " index of top level" << top_level_index;
			// handle the item itself
			if ((*it)->checkState(0) == Qt::Checked ||
					(*it)->checkState(0) == Qt::PartiallyChecked ) {
				qDebug() << "--ObjectDataDialog::apply-- set:"
						 << composed_measurement  << " canonic: "
						 << measurement->canonicName(composed_measurement);
				measurement->addActualMeasures(composed_measurement);
			}
			if ((*it)->checkState(0) == Qt::Unchecked) {
				qDebug() << "--ObjectDataDialog::apply-- reset:"
				      	 << composed_measurement  << " canonic: "
				      	 << measurement->canonicName(composed_measurement);
				measurement->removeActualMeasures(composed_measurement);
			}
		} else  {
			// do potential the sub items
			QVariant v = measurement->measurementValue(composed_measurement);
			if (v.canConvert<GeometricMeasurement*>()) {
				GeometricMeasurement* gm = v.value<GeometricMeasurement*>();
				qDebug() << "--ObjectDataDialog::applyMeasurements-- "
						 << "dig into geometry";
				applyMeasurements(it, gm);
			}
			if (v.canConvert<IntensityMeasurement*>()) {
				IntensityMeasurement* im = v.value<IntensityMeasurement*>();
				qDebug() << "--ObjectDataDialog::applyMeasurements-- "
						 << "dig into intensity";
				applyMeasurements(it, im);
			}
		}
		qDebug() << "--ObjectDataDialog::apply-- next iterator";
	    ++it;
	 }

	qDebug() << "--ObjectDataDialog::apply-- finished";
	ui.actionApply->setEnabled(false);
	emit objectDataChanged();
}

/**
 * @description updates the actual measures according to the settings
 *  within the tree list box.
 *  calls itself recursively, if required
 *  @param iterator: of QTreeWidgetItemIteratorrow at the next index to process
 *                   called by reference
 *  @param measurements: pointer to the measurement object
 *  @return : index of the last element that belongs to the group
 *            will be computed during the run.
 */

void ObjectDataDialog::applyMeasurements(QTreeWidgetItemIterator& it
		,MeasurementOnObject *measurement ) {

	MeasurementType available = measurement->availableMeasures();
	qDebug() << "--ObjectDataDialog::applyMeasurements-- group canonic: "
			<< measurement->canonicName(MeasurementOnObject::Self)
			<< " available: " << available;
	QTreeWidgetItem* parent = (*it)->parent(); // must exist
	int child_index = parent->indexOfChild(*it);
	qDebug() << "--ObjectDataDialog::applyMeasurements-- child index: "
			<< child_index;

	// assume that there are no sub items!! - just one level of grouping

	MeasurementType m = 1;
	for (int index = -1; index <  child_index ; m = m << 1 )
		if ( m & available) index++;
	// it is always one shift too much
	m = m >> 1;

	if ((*it)->checkState(0) == Qt::Checked) {
		qDebug() << "--ObjectDataDialog::applyMeasurements-- set: " << m
				 << " canonic: " << measurement->canonicName(m);
		measurement->addActualMeasures(m);
	}
	if ((*it)->checkState(0) == Qt::Unchecked) {
		qDebug() << "--ObjectDataDialog::applyMeasurements-- reset: " << m
				 << " canonic: " << measurement->canonicName(m);
		measurement->removeActualMeasures(m);
	}

}



ObjectDataDialog::~ObjectDataDialog() {
	// TODO Auto-generated destructor stub
	// disconnect - or does it the parent?
}


