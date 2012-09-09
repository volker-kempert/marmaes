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
#include <QHBoxLayout>
#include <QIcon>


#include "image_context_view.h"



void ImageContextView::setupUi(QWidget* parent )
{
	qDebug() << "--ImageContextView::setupUi-- called";

    if (objectName().isEmpty())
        setObjectName(QString::fromUtf8("ImageContextView"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //sizePolicy.setHorizontalStretch(1);
    //sizePolicy.setVerticalStretch(1);
    //sizePolicy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(sizePolicy);
    setMinimumSize(QSize(380, 120));
    setBaseSize(QSize(0, 0));
    QFont font;
    font.setBold(true);
    font.setWeight(75);
    setFont(font);
    setFloating(false);
    setFeatures(QDockWidget::AllDockWidgetFeatures);
    setAllowedAreas(Qt::BottomDockWidgetArea|Qt::TopDockWidgetArea);

    dockWidgetContents = new QWidget();
    dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
    QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    dockWidgetContents->setMinimumSize(QSize(380, 80));
    dockWidgetContents->setBaseSize(QSize(0, 0));

    dockWidgetContents->setSizePolicy(sizePolicy1);
    QFont font_normal;
    font_normal.setWeight(QFont::Normal);
    dockWidgetContents->setFont(font_normal);
    setWidget(dockWidgetContents);

    editObject = new QPushButton(dockWidgetContents);
    editObject->setSizePolicy(sizePolicy1);
    editObject->setMinimumSize(QSize(24, 24));
    QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(editObject->sizePolicy().hasHeightForWidth());
    editObject->setSizePolicy(sizePolicy2);
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/resources/edit.svg"), QSize(), QIcon::Normal, QIcon::On);
    editObject->setIcon(icon1);


    labelImageFileName = new QLabel();
    dataImageFileName = new QLabel();
    //dataFileName->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
    QHBoxLayout *layoutImageFileName = new QHBoxLayout;
    QWidget*  layoutWidgetImageFileName = new QWidget;
    layoutWidgetImageFileName->setLayout(layoutImageFileName);
    layoutImageFileName->addWidget(labelImageFileName);
    layoutImageFileName->addWidget(dataImageFileName);

    labelObjectFileName = new QLabel();
    dataObjectFileName = new QLabel();
    QHBoxLayout *layoutObjectFileName = new QHBoxLayout;
    QWidget*  layoutWidgetObjectFileName = new QWidget;
    layoutWidgetObjectFileName->setLayout(layoutObjectFileName);
    layoutObjectFileName->addWidget(labelObjectFileName);
    layoutObjectFileName->addWidget(dataObjectFileName);

    labelUnitConversation = new QLabel();
    dataUnitConversation = new QLabel();
    QHBoxLayout *layoutUnitConversation = new QHBoxLayout;
    QWidget*  layoutWidgetUnitConversation = new QWidget;
    layoutWidgetUnitConversation->setLayout(layoutUnitConversation);
    layoutUnitConversation->addWidget(labelUnitConversation);
    layoutUnitConversation->addWidget(dataUnitConversation);

    labelCustomParam1 = new QLabel();
    dataCustomParam1 = new QLabel();
    QHBoxLayout *layoutCustomParam1 = new QHBoxLayout;
    QWidget*  layoutWidgetCustomParam1 = new QWidget;
    layoutWidgetCustomParam1->setLayout(layoutCustomParam1);
    layoutCustomParam1->addWidget(labelCustomParam1);
    layoutCustomParam1->addWidget(dataCustomParam1);

    labelCustomParam2 = new QLabel();
    dataCustomParam2 = new QLabel();
    QHBoxLayout *layoutCustomParam2 = new QHBoxLayout;
    QWidget*  layoutWidgetCustomParam2 = new QWidget;
    layoutWidgetCustomParam2->setLayout(layoutCustomParam2);
    layoutCustomParam2->addWidget(labelCustomParam2);
    layoutCustomParam2->addWidget(dataCustomParam2);

    QFont font_bold;
    font_bold.setWeight(QFont::DemiBold);

    dataImageFileName->setFont(font_bold);
    dataObjectFileName->setFont(font_bold);
    dataCustomParam1->setFont(font_bold);
    dataCustomParam2->setFont(font_bold);
    dataUnitConversation->setFont(font_bold);

    flowLayout = new FlowLayout();
    flowLayout->setObjectName(QString::fromUtf8("flowLayout"));
    flowLayout->addWidget(layoutWidgetImageFileName);
    flowLayout->addWidget(layoutWidgetObjectFileName);
    flowLayout->addWidget(layoutWidgetUnitConversation);
    flowLayout->addWidget(layoutWidgetCustomParam1);
    flowLayout->addWidget(layoutWidgetCustomParam2);

    QWidget*  flowWidget = new QWidget;
    flowWidget->setLayout(flowLayout);

    QHBoxLayout *layoutOverall = new QHBoxLayout;
    dockWidgetContents->setLayout(layoutOverall);
    layoutOverall->addWidget(flowWidget);
    layoutOverall->addWidget(editObject);

    retranslateUi();

	connect(editObject, SIGNAL(clicked()), parent, SLOT(editImageContext()));
}

void ImageContextView::retranslateUi()
{
    setWindowTitle(QApplication::translate("ImageContextView", "Image Context Data", 0, QApplication::UnicodeUTF8));
    labelImageFileName->setText(QApplication::translate("ImageContextView", "Image File Name", 0, QApplication::UnicodeUTF8));
    dataImageFileName->setText(QString());
    labelObjectFileName->setText(QApplication::translate("ImageContextView", "Object File Name", 0, QApplication::UnicodeUTF8));
    dataObjectFileName->setText(QString());
    labelCustomParam1->setText(QApplication::translate("ImageContextView", "Custom data 1", 0, QApplication::UnicodeUTF8));
    labelCustomParam2->setText(QApplication::translate("ImageContextView", "Custom data 2", 0, QApplication::UnicodeUTF8));
    labelUnitConversation->setText(QApplication::translate("ImageContextView", "Units", 0, QApplication::UnicodeUTF8));
    dataUnitConversation->setText(QString());
    dataCustomParam1->setText(QString());
    dataCustomParam2->setText(QString());
}



ImageContextView::ImageContextView(QWidget* parent , Qt::WindowFlags flags  )
    : QDockWidget(parent,flags)
{
	setupUi(parent);
	input_ = ImageContext::getInstance();
	updateImageContext();
}


void ImageContextView::updateImageContext() {
	qDebug() << "--ImageContextView::updateImageContext-- called";
	if ( input_->getImageShortName() == "" ) {
		dataImageFileName->setVisible(false);
		labelImageFileName->setVisible(false);
		dataObjectFileName->setVisible(false);
		labelObjectFileName->setVisible(false);
		dataUnitConversation->setVisible(false);
		labelUnitConversation->setVisible(false);
	    editObject->setEnabled(false);
		labelCustomParam1->setVisible(false);
		dataCustomParam1->setVisible(false);
		labelCustomParam2->setVisible(false);
		dataCustomParam2->setVisible(false);
	} else {
	    editObject->setEnabled(true);
		dataImageFileName->setVisible(true);
		labelImageFileName->setVisible(true);
		dataImageFileName->setText(input_->getImageShortName());

		dataUnitConversation->setVisible(true);
		labelUnitConversation->setVisible(true);
		QString unit = QString("1 Pixel equals ");
		unit += QString::number(input_->getConversionFactor());
		unit += " ";
		unit += input_->getUnitOfMeasurement();
		dataUnitConversation->setText(unit);

		if ( input_->getObjectShortName() == "" ) {
			dataObjectFileName->setVisible(false);
			labelObjectFileName->setVisible(false);
		} else {
			dataObjectFileName->setVisible(true);
			labelObjectFileName->setVisible(true);
			dataObjectFileName->setText(input_->getObjectShortName());
		}
		if (input_->getParam1Name() == "") {
			labelCustomParam1->setVisible(false);
			dataCustomParam1->setVisible(false);
		} else {
			labelCustomParam1->setVisible(true);
			dataCustomParam1->setVisible(true);
			labelCustomParam1->setText(input_->getParam1Name());
			dataCustomParam1->setText(input_->getParam1Data());
		}
		if (input_->getParam2Name() == "") {
			labelCustomParam2->setVisible(false);
			dataCustomParam2->setVisible(false);
		} else {
			labelCustomParam2->setVisible(true);
			dataCustomParam2->setVisible(true);
			labelCustomParam2->setText(input_->getParam2Name());
			dataCustomParam2->setText(input_->getParam2Data());
		}
	}
}

ImageContextView::~ImageContextView()
{

}
