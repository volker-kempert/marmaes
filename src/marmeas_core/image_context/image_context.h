/*
   Copyright (C) 2012, Volker Kempert <volker.kempert@gmx.net>

   This file is part of an MARMEAS library

   The MARMEAS library is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of
   the License, or (at your option) any later version.

   The MARMEAS library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with MARMEAS. If not, see <http://www.gnu.org/licenses/>.

*/


#ifndef INPUT_IMAGE_H_
#define INPUT_IMAGE_H_

#include <QObject>
#include <QUrl>

/*
 * by nature ImageContext class is a
 *  * Model (MVC pattern because there are at least two views
 *    (Modify dialog, and view of data) are connected to
 *  * models do not emit signals, but might update themselves by
 *    receiving signals
 *  * models keep their internal consistency by every single call
 *  * it is a singleton, since only one image will be processed after
 *    each other (no Multiple document handling for ease of use)
 *
 *  ImageContext handles the (business) context data of an image
 *
 */
class ImageContext : public QObject {

	Q_OBJECT

public:
	~ImageContext();
	static ImageContext* getInstance();
	/*
	 *  determine if any input parameter have changed after
	 *  reloading from file or initial setup
	 *
	 *  @return true if data needs to be saved otherwise false
	 */
	bool isModified() { return is_modified_; }
	/*!
	 *  if the image is just an image with pixel coordinates
	 *  and no other parameter the image is handled like an image
	 *  any input characteristics will not be setup.
	 *
	 *  the input characteristics must be default!
	 *  this mode makes sense for quick assessments of images w/o
	 *  the intension to store the results.
	 *
	 *  @return true if it is an plain image otherwise false
	 */
	bool isPlainImage() { return is_plain_image_; }
	bool requiresEdits() { return requires_edits_; }


	QString getImageFileName() { return image_file_name_; }
	QString getObjectFileName() { return object_file_name_; }
	QString getImageShortName();
	QString getObjectShortName();
	QString getParam1Name() { return param_1_name_; }
	QString getParam2Name() { return param_2_name_; }
	QString getParam1Data() { return param_1_data_; }
	QString getParam2Data() { return param_2_data_; }
	QString getUnitOfMeasurement() { return unit_of_measurement_; }
	qreal getConversionFactor() { return conversion_factor_; }

	void setUnitOfMeasurement();
	void setConversationFactor();

	void loadImageTemplateSettings(QString image_context_template);

public slots:
	void resetImageContext();
	void setImageFileName(QString  s);
	void setParam1Data(QString s);
	void setParam2Data(QString s);
	void setParam1Name(QString s);
	void setParam2Name(QString s);
	void setUnitOfMeasurement(QString s);
	void setConversionFactor(qreal f);
	void setObjectFileName(QString s);
	void setDirectory(QString s);
	void setFilePattern(QString s);


private:
    static ImageContext* the_input_image_;
    ImageContext(QObject *parent = 0);

	void computeFlags();
	void computeObjectFileName();

	bool is_modified_;
	bool is_plain_image_;
	bool requires_edits_;
	bool freeze_object_file_name_;

	// copy of input data
	QString image_context_template_;
	QString file_pattern_;
	QString directory_;
	QString object_file_name_;
	QString param_1_name_;
	QString param_1_data_;
	QString param_2_name_;
	QString param_2_data_;
	QString unit_of_measurement_;
	qreal   conversion_factor_;
	QString image_file_name_;



};

#endif /* INPUT_IMAGE_H_ */
