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

#ifndef INPUT_IMAGE_DIALOG_H_
#define INPUT_IMAGE_DIALOG_H_

#include "marmeas_core/image_context/image_context.h"
#include "ui_image_context.h"

class ImageContextDialog : public QDialog {

	Q_OBJECT

public:
	ImageContextDialog(QWidget *parent = 0, ImageContext*  input= 0 );
	~ImageContextDialog();

private slots:
	void apply();      // take all the changes and the model
	void activateApply() { ui.apply->setEnabled(true); }

signals:
	void imageContextChanged();

private:
	ImageContext* input_;
    Ui::InputImageData ui;
};

#endif /* INPUT_IMAGE_DIALOG_H_ */
