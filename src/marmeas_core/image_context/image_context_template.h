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

#ifndef CONFIG_INPUT_H_
#define CONFIG_INPUT_H_


class ImageContextTemplate   {
public:
	ImageContextTemplate(QString name_ );
	~ImageContextTemplate() { writeSettings(); }
	QString name;
	QString unit_of_measurement;
	qreal  conversation_factor;
	QString custom_param_1;
	QString custom_param_2;
	QString directory;
	QString file_pattern;
	void readSettings();
	void writeSettings();
};

#endif /* CONFIG_INPUT_H_ */
