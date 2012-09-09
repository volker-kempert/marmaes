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

#ifndef IMAGE_DATA_VIEW_H
#define IMAGE_DATA_VIEW_H


#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>
#include <QtGui/QPushButton>

#include "marmeas_core/image_context/image_context.h"
#include "3rdparty/flowlayout/flowlayout.h"


class ImageContextView : public QDockWidget {

	Q_OBJECT

public:
    ImageContextView(QWidget *parent = 0, Qt::WindowFlags flags = 0 );
    ~ImageContextView();

signals:
    void editImageContext();

private slots:
	void updateImageContext ();

private:
    ImageContext* input_;
    QPushButton *editObject;
    QWidget *dockWidgetContents;
    QLabel *labelImageFileName;
    QLabel *dataImageFileName;
    QLabel *labelObjectFileName;
    QLabel *dataObjectFileName;
    QLabel *labelCustomParam1;
    QLabel *labelCustomParam2;
    QLabel *labelUnitConversation;
    QLabel *dataUnitConversation;
    QLabel *dataCustomParam1;
    QLabel *dataCustomParam2;
    FlowLayout *flowLayout;
    void setupUi(QWidget* parent );
    void retranslateUi();
};

#endif // IMAGE_DATA_VIEW_H
