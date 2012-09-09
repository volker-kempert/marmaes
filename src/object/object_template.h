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


#ifndef OBJECT_TEMPLATE_H_
#define OBJECT_TEMPLATE_H_

#include <QMainWindow>
#include <QAction>
#include <QActionGroup>
#include <QString>
#include <QIcon>
#include <QMenu>
#include <QComboBox>

#include "object_base.h"

/*
 * there are generic templates and customized templates
 *
 * generic templates are those as provided by the application the type is
 * according to their behavior
 *
 * customized templates facilitate the generic templates. They are based in
 * the business domain to reflect the language and problem space of the user
 * The differentiate from generic templates by (custom type, source,
 * colors and icons
 *
 * The custom type is the name of the template; which is identical to the
 * base type for generic templates
 *
 * generic templates are not covered by settings (all data is predetermined
 * and not subject to modification
 */
class ObjectTemplate {
public:
	ObjectTemplate(GeometricObjectFactory* f, QString type, QAction* action,
			bool is_generic );
	virtual ~ObjectTemplate();
	bool isGenericTemplate() const { return generic_template_; }
	GeometricObjectFactory* getFactory() const {return object_factory_; }
	const QString& getGenericType() const { return generic_type_; }
	const QString& name() const { return name_; }
	void setName(QString name);

	QAction* getAction() { return action_link_; }


private:
	QString name_;
	GeometricObjectFactory* object_factory_;
	QString generic_type_;
	bool generic_template_;

	// it is a compromise to store presentation related links into
	// this object; needs to be replaced once a second view for factory picking
	// comes along
	QAction* action_link_;
};

class ObjectTemplateContainer : public QObject {

	Q_OBJECT

public:
	ObjectTemplateContainer(QMainWindow *mw, QMenu *m, QComboBox *cb);
	virtual ~ObjectTemplateContainer();


	void registerGenericTemplate(GeometricObjectFactory* factory,
			QString type, const QIcon& icon);
	void newCustomTemplate(ObjectTemplate* ref_to_generic,
			QString custom_type, const QIcon& icon);

	void removeCustomTemplate(ObjectTemplate* remove_subject);
	ObjectTemplate* getObjectTemplate(QString type);

public slots:
	// a controller function facing the "sub menu view
	// need to be refactored once a second view comes along
	// emits the new factory signal as well
	GeometricObjectFactory* getSelectedFactory();
	void selectionChanged(const QString& name);

signals:
    void newFactorySelected(GeometricObjectFactory* factory);
    void newObjectTemplateSelected(const QString& name);

private:
	void readSettings();
	void writeSettings();
	QList<ObjectTemplate *> container_;

	// place holder to be able registering new object template in menu
	QMainWindow* main_window_;   // parent for new actions
	QMenu* menu_;                // hook for the menu entries
	QComboBox* combo_box_;       // hook for entries
	QActionGroup* action_group_; // another handler for radio box behavior

};


#endif /* OBJECT_TEMPLATE_H_ */
