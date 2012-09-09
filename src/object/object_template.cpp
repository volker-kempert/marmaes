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
#include <QSettings>

#include "assert.h"
#include <stdexcept>

#include "object_template.h"

ObjectTemplate::ObjectTemplate(GeometricObjectFactory* f,
		QString type, QAction* action, bool is_generic ) {
	object_factory_ = f;
	generic_template_ = is_generic;
	generic_type_ = type;
	name_ = type;
	action_link_ = action;
}

void ObjectTemplate::setName(QString name) {
	if ( ! generic_template_ ) name_ = name;
	else {
		qDebug() << "--ObjectTemplate::setName-- "
				<< "it is not allowed to change the name of a generic template";
		std::logic_error error("setName is invalid for generic template");
		throw error;
	}
}


ObjectTemplate::~ObjectTemplate() {

}

ObjectTemplateContainer::ObjectTemplateContainer(QMainWindow *mw,
		QMenu *m, QComboBox *cb) {
	main_window_ = mw;
	menu_ = m;
	combo_box_ = cb;
	action_group_ = new QActionGroup(menu_);
	action_group_->setExclusive(true);

	connect(this,SIGNAL(newObjectTemplateSelected(const QString& )),
			this,SLOT(selectionChanged(const QString& )));

	connect(combo_box_,SIGNAL(currentIndexChanged (const QString& )),
			this,SLOT(selectionChanged(const QString& )));

}

ObjectTemplateContainer::~ObjectTemplateContainer() {
	 for (int i = 0; i < container_.size(); ++i) {
			// ToDo unlink from menu
			// ToDo delete action
		 delete(container_.at(i));
	 }
}

void ObjectTemplateContainer::registerGenericTemplate(
		GeometricObjectFactory* factory,QString type, const QIcon& icon) {

	QAction* menu_entry = new QAction(main_window_);
	menu_entry->setObjectName(type);
	menu_entry->setText(type);

	menu_entry->setIcon(icon);
    menu_entry->setCheckable(true);
	menu_->addAction(menu_entry);
    if (container_.isEmpty())
    	menu_entry->setChecked(true); // select 1st item
	action_group_->addAction(menu_entry);
	ObjectTemplate* t = new ObjectTemplate(factory, type, menu_entry, true);
	container_.append(t);

	combo_box_->addItem(icon,type,QVariant(1));


	// set the message flow
	// menu --> combo box and obj templ container
	connect(menu_entry,SIGNAL(triggered()),
			this,SLOT(getSelectedFactory()));

}

void ObjectTemplateContainer::newCustomTemplate(
		ObjectTemplate* ref_to_generic, QString custom_type, const QIcon& icon) {
// ToDo for custom templates
}

void ObjectTemplateContainer::removeCustomTemplate(
		ObjectTemplate* remove_subject) {
	// ToDo for custom templates
}

void ObjectTemplateContainer::readSettings() {
	// ToDo for custom templates
}

void ObjectTemplateContainer::writeSettings() {
	// ToDo for custom templates
}

void ObjectTemplateContainer::selectionChanged(const QString& name) {
	// combo box index change
	int index = combo_box_->findText(name);
	if (index != combo_box_->currentIndex())
		combo_box_->setCurrentIndex ( index );

	// update menu selection change and signal new factory
	ObjectTemplate * object_template;
	for (int i = 0; i < container_.size(); i++) {
		object_template = container_.at(i);
		bool check_val = (object_template->name() == name);
		object_template->getAction()->setChecked(check_val);
		// signal new factory
		if (check_val)
			emit newFactorySelected(object_template->getFactory());
	}
}

GeometricObjectFactory* ObjectTemplateContainer::getSelectedFactory() {
	ObjectTemplate * object_template;
	qDebug() << "--ObjectTemplateContainer::getSelectedFactory--";
	for (int i = 0; i < container_.size(); i++) {
		object_template = container_.at(i);
		if (object_template->getAction()->isChecked() ) {
			emit newObjectTemplateSelected(object_template->name());
			return object_template->getFactory();
		}
	}
	// in case nothing is checked return first item
	assert(false); // come to this point is wrong
	assert (container_.size() > 0);
	emit newFactorySelected(container_.at(0)->getFactory());
	return container_.at(0)->getFactory();
}

