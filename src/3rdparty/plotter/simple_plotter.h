/*
 * simple_plotter.h
 *
 * copyright (c) 2010 by Jasmin Blanchette, Mark Summerfield
 *               Book "C++ GUI Programming with Qt4, 2nd Edition"
 *               online ref: http://www.informit.com/title/0132354160
 *               Chapter 05
 *
 * This material may be distributed only subject to the terms and conditions
 * set forth in the Open Publication License, v1.0 or later
 *
 * the latest version is presently available at
 *      http://www.opencontent.org/openpub/).
 *
 * Modification:
 *   by Volker Kempert <volker.kempert@gmx.net>
 *      description of modification:
 *       - identifier names
 *       - color settings
 */


#ifndef SIMPLE_PLOTTER_H_
#define SIMPLE_PLOTTER_H_

#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>
#include <QRect>
#include <QPixmap>
#include <QToolButton>

class QToolButtons;

class PlotSettings {
public:
	PlotSettings();
	void scroll(int dx, int dy);
	void adjust();
	double spanX() const { return max_x_ - min_x_; }
	double spanY() const { return max_y_ - min_y_; }

	double min_x_;
	double max_x_;
	int num_x_ticks_;
	double min_y_;
	double max_y_;
	int num_y_ticks_;
private:
	static void adjustAxis(double &min, double &max, int &num_ticks);

};

class SimplePlotter : public QWidget {
	Q_OBJECT

public:
	SimplePlotter(QWidget *parent = 0);
	void setPlotSettings(const PlotSettings &settings);
	void setCurveData(int id, const QVector<QPointF> & data);
	void clearCurve(int id);
	QSize minimumSizeHint() const;
	QSize sizeHint() const;

public slots:
	void zoomIn();
	void zoomOut();

protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void wheelEvent(QWheelEvent *event);

private:
	void updateRubberBandRegion();
	void refreshPixmap();
	void drawGrid(QPainter *painter);
	void drawCurves(QPainter *painter);

	enum { Margin = 50 } ;
	QToolButton *zoom_in_button_;
	QToolButton *zoom_out_button_;
	QMap<int, QVector<QPointF> > curve_map_;
	QVector<PlotSettings> zoom_stack_;
	int cur_zoom_;
	bool rubber_band_is_shown_;
	QRect rubber_band_rect_;
	QPixmap pixmap_;
};

#endif /* SIMPLE_PLOTTER_H_ */
