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

#include <QDebug>
#include <cmath>
#include "graphics_view.h"



GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
	qDebug() << "--GraphicsView::GraphicsView--";
	setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	setDragMode(QGraphicsView::RubberBandDrag);
	setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	setContextMenuPolicy(Qt::ActionsContextMenu);
}

void  GraphicsView::wheelEvent(QWheelEvent *event) {
	double num_degrees = -event->delta() / 8.0;
	double num_steps = num_degrees / 15.0;
	double factor = std::pow(1.125 , num_steps);
	scale(factor, factor);
}

void GraphicsView::keyPressEvent ( QKeyEvent * event ) {
	QGraphicsView::keyPressEvent(event);

	if( event->key() == Qt::Key_Shift ) {
		setDragMode(QGraphicsView::ScrollHandDrag);
	}
}

void GraphicsView::keyReleaseEvent ( QKeyEvent * event ) {
		QGraphicsView::keyPressEvent(event);

		if( event->key() == Qt::Key_Shift ) {
			setDragMode(QGraphicsView::RubberBandDrag);
		}
}

void GraphicsView::zoomIn() {
	double factor = std::pow(1.125, 1);
	scale (factor, factor);
}

void GraphicsView::zoomOut() {
	double factor = std::pow(1.125, -1);
	scale (factor, factor);
}


GraphicsView::~GraphicsView()
{

}
