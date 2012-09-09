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

#ifndef GRAPHICS_VIEW_H
#define GRAPHICS_VIEW_H


#include <QWidget>
#include <QGraphicsView>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QMainWindow>
#include <QWheelEvent>

#include "graphics_point_item.h"
#include "graphics_scene.h"


class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsView(QWidget *parent = 0);
    ~GraphicsView();
    void setScene(GraphicsScene* s) { scene_ = s; QGraphicsView::setScene(
    		(QGraphicsScene* ) s);
    }
protected:
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent ( QKeyEvent * event );
    void keyReleaseEvent ( QKeyEvent * event );

private slots:
	void zoomIn();
	void zoomOut();
	void selectMode() { scene_->setStatus(Selection);
						setDragMode(QGraphicsView::RubberBandDrag);}
	void navigateMode() { 	scene_->setStatus(Navigation);
							setDragMode(QGraphicsView::ScrollHandDrag);}
	void addMode() { scene_->setStatus(AddObject);
					 setDragMode(QGraphicsView::RubberBandDrag);}

private:
    GraphicsScene *scene_;


};

#endif
