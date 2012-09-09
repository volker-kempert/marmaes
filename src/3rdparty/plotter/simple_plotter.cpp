/*
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

#include <QtGui>
#include <cmath>


#include "simple_plotter.h"


SimplePlotter::SimplePlotter(QWidget *parent) : QWidget(parent) {
	setBackgroundRole(QPalette::Light);
	setAutoFillBackground(true);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setFocusPolicy(Qt::StrongFocus);

	zoom_in_button_ = new QToolButton(this);
	zoom_in_button_->setIcon(QIcon(":/resources/zoom_in.png"));
	zoom_in_button_->adjustSize();
	connect(zoom_in_button_, SIGNAL(clicked()), this, SLOT(zoomIn()));


	zoom_out_button_ = new QToolButton(this);
	zoom_out_button_->setIcon(QIcon(":/resources/zoom_out.png"));
	zoom_out_button_->adjustSize();
	connect(zoom_out_button_, SIGNAL(clicked()), this, SLOT(zoomOut()));

	setPlotSettings(PlotSettings());

}


void SimplePlotter::setPlotSettings(const PlotSettings &settings) {
	zoom_stack_.clear();
	zoom_stack_.append(settings);
	cur_zoom_ = 0;
	zoom_in_button_->hide();
	zoom_out_button_->hide();
	refreshPixmap();
}

void SimplePlotter::zoomOut() {
	--cur_zoom_;
	zoom_out_button_->setEnabled(cur_zoom_ > 0);
	zoom_in_button_->setEnabled(true);
	zoom_in_button_->show();
	refreshPixmap();
}

void SimplePlotter::zoomIn() {
	if (cur_zoom_ < zoom_stack_.count() -1 ) {
		++cur_zoom_;
		zoom_in_button_->setEnabled(cur_zoom_ < zoom_stack_.count() -1 );
		zoom_out_button_->setEnabled(true);
		zoom_out_button_->show();
		refreshPixmap();
	}
}


void SimplePlotter::setCurveData(int id, const QVector<QPointF> &data) {
	curve_map_[id] = data;
	refreshPixmap();
}

void SimplePlotter::clearCurve(int id) {
	curve_map_.remove(id);
	refreshPixmap();
}

QSize SimplePlotter::minimumSizeHint() const {
	return QSize(6 * Margin, 4 * Margin);
}

QSize SimplePlotter::sizeHint() const {
	return QSize(12 * Margin, 8 * Margin);
}

void SimplePlotter::paintEvent(QPaintEvent *e) {
	QStylePainter painter(this);
	painter.drawPixmap(0,0,pixmap_);
	if (rubber_band_is_shown_) {
		painter.setPen(palette().dark().color());
		painter.drawRect(rubber_band_rect_.normalized().adjusted( 0, 0,-1, -1));
	}
	if (hasFocus()) {
		QStyleOptionFocusRect option;
		option.initFrom(this);
		option.backgroundColor = palette().light().color();
		painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
	}
}

void SimplePlotter::resizeEvent(QResizeEvent* e) {
	qDebug() << "--SimplePlotter::resizeEvent-- called";
	int x = width() - ( zoom_in_button_->width() +
			zoom_out_button_->width() + 10);
	zoom_in_button_->move(x,5);
	zoom_out_button_->move(x+ zoom_in_button_->width() + 5 , 5);
	refreshPixmap();
}

void SimplePlotter::mousePressEvent(QMouseEvent *event) {
	QRect rect(Margin, Margin,
			width() - 2 * Margin, height() - 2 * Margin);
	if (event->button() == Qt::LeftButton) {
		rubber_band_is_shown_ = true;
		rubber_band_rect_.setTopLeft(event->pos());
		rubber_band_rect_.setBottomRight(event->pos());
		updateRubberBandRegion();
		setCursor(Qt::CrossCursor);
	}
}

void SimplePlotter::mouseMoveEvent(QMouseEvent *event) {
	if (rubber_band_is_shown_) {
		updateRubberBandRegion();
		rubber_band_rect_.setBottomRight(event->pos());
		updateRubberBandRegion();
	}
}

void SimplePlotter::mouseReleaseEvent(QMouseEvent *event) {
	if((event->button() == Qt::LeftButton) && rubber_band_is_shown_) {
		rubber_band_is_shown_ = false;
		updateRubberBandRegion();
		unsetCursor();

		QRect rect = rubber_band_rect_.normalized();
		if ( rect.width() < 4 || rect.height() < 4 ) return;
		rect.translate(-Margin, -Margin);
		PlotSettings prev_settings = zoom_stack_[cur_zoom_];
		PlotSettings settings;
		double dx = prev_settings.spanX() / (width() - 2 * Margin);
		double dy = prev_settings.spanY() / (height() - 2 * Margin);
		settings.min_x_ = prev_settings.min_x_ + dx * rect.left();
		settings.max_x_ = prev_settings.min_x_ + dx * rect.right();
		settings.min_y_ = prev_settings.max_y_ - dx * rect.bottom();
		settings.max_y_ = prev_settings.max_y_ - dx * rect.top();
		settings.adjust();

		zoom_stack_.resize(cur_zoom_ +1);
		zoom_stack_.append(settings);
		zoomIn();
	}
}

void SimplePlotter::keyPressEvent(QKeyEvent *event) {
	switch (event->key()) {
	case Qt::Key_Plus:
		zoomIn();
		break;
	case Qt::Key_Minus:
		zoomOut();
		break;
	case Qt::Key_Left:
		zoom_stack_[cur_zoom_].scroll(-1, 0);
		break;
	case Qt::Key_Right:
		zoom_stack_[cur_zoom_].scroll(+1, 0);
		break;
	case Qt::Key_Down:
		zoom_stack_[cur_zoom_].scroll(0, -1);
		break;
	case Qt::Key_Up:
		zoom_stack_[cur_zoom_].scroll(0, +1);
		break;
	default:
		QWidget::keyPressEvent(event);
	}
}

void SimplePlotter::wheelEvent(QWheelEvent *event) {
	int numDegrees = event->delta() / 8;
	int numTicks = numDegrees / 15 ;

	if (event->orientation() == Qt::Horizontal) {
		zoom_stack_[cur_zoom_].scroll(numTicks, 0);
	} else {
		zoom_stack_[cur_zoom_].scroll(0, numTicks);
	}
	refreshPixmap();
}

void SimplePlotter::updateRubberBandRegion() {
	QRect rect = rubber_band_rect_.normalized();
	update(rect.left(), rect.top(), rect.width() , 1);
	update(rect.left(), rect.top(), 1 , height());
	update(rect.left(), rect.bottom(), rect.width() , 1);
	update(rect.right(), rect.top(), 1 , height());
}

void SimplePlotter::refreshPixmap() {
	pixmap_ = QPixmap(size());
	pixmap_.fill(this, 0, 0);
	QPainter painter(&pixmap_);
	painter.initFrom(this);
	drawGrid(&painter);
	drawCurves(&painter);
	update();
}

void SimplePlotter::drawGrid(QPainter *painter) {
	QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin );
	if (!rect.isValid()) return;

	PlotSettings settings = zoom_stack_[cur_zoom_];

	for (int i = 0; i <= settings.num_x_ticks_; ++i ) {
		int x = rect.left() + ( i * rect.width() - 1) / ( settings.num_x_ticks_);
		double label = settings.min_x_ + (i * settings.spanX() / settings.num_x_ticks_);
		painter->setPen(QColor(Qt::darkGray));
		painter->drawLine(x, rect.top(), x, rect.bottom());
		painter->setPen(QColor(Qt::black));
		painter->drawLine(x, rect.bottom(), x , rect.bottom() + 5);
		painter->drawText(x -50, rect.bottom() + 5, 100, 15,
				Qt::AlignHCenter | Qt::AlignTop , QString::number(label));
	}

	for (int j = 0; j <= settings.num_y_ticks_; j++) {
		int y = rect.bottom() - (j * (rect.height() -1) / settings.num_y_ticks_);
		double label = settings.min_y_ + (j * settings.spanY() / settings.num_y_ticks_);
		painter->setPen(QColor(Qt::darkGray));
		painter->drawLine(rect.left(), y , rect.right(), y);
		painter->setPen(QColor(Qt::black));
		painter->drawLine(rect.left() - 5, y , rect.left(), y);
		painter->drawText(rect.left() - Margin, y -10, Margin -5, 20 ,
				Qt::AlignVCenter | Qt::AlignRight , QString::number(label));
	}
	painter->drawRect(rect.adjusted( 0, 0, -1, -1));
}

void SimplePlotter::drawCurves(QPainter *painter) {
	static const QColor color_for_ids[6] = {
			Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow };
	PlotSettings settings = zoom_stack_[cur_zoom_];
	QRect rect(Margin, Margin, width() -2 * Margin, height() - 2 * Margin);
	if (!rect.isValid()) return;

	painter->setClipRect(rect.adjusted ( + 1, + 1, -1, -1));
	QMapIterator<int, QVector<QPointF> > i(curve_map_);
	while ( i.hasNext()) {
		i.next();
		int id = i.key();
		const QVector<QPointF> &data = i.value();
		QPolygonF polyline(data.count());
		for (int j = 0; j < data.count(); ++j) {
			double dx = data[j].x() - settings.min_x_;
			double dy = data[j].y() - settings.min_y_;
			double x = rect.left() + (dx * (rect.width() -1 ) / settings.spanX());
			double y = rect.bottom() - (dy * (rect.height() -1 ) / settings.spanY());
			polyline[j] = QPointF(x, y);
		}
		painter->setPen(color_for_ids[uint(id) % 6]);
		painter->drawPolyline(polyline);
	}
}

//----------------------------------------------------------------------------

PlotSettings::PlotSettings() {
	min_x_ = 0.0;
	max_x_ = 640.0;
	num_x_ticks_ = 5;
	min_y_ = 0.0;
	max_y_ = 256.0;
	num_y_ticks_ = 5;
}

void PlotSettings::scroll(int dx, int dy) {
	double step_x = spanX() / num_x_ticks_;
	min_x_ += dx * step_x;
	max_x_ += dx * step_x;
	double step_y = spanY() / num_y_ticks_;
	min_y_ += dy * step_y;
	max_y_ += dy * step_y;
}

void PlotSettings::adjust() {
	adjustAxis(min_x_, max_x_, num_x_ticks_);
	adjustAxis(min_y_, max_y_, num_y_ticks_);
}

void PlotSettings::adjustAxis(double &min, double &max, int &num_ticks) {
	const int min_ticks = 4;
	double gross_step = (max - min)  / min_ticks;
	double step = pow(10.0, floor(log10(gross_step)));

	if ( 5 * step < gross_step) { step *= 5; }
	else if ( 2 * step < gross_step ) { step *= 2; }

	num_ticks = int(ceil(max / step) - floor(min / step));
	if (num_ticks < min_ticks) num_ticks = min_ticks;
	min = floor(min / step ) * step;
	max = ceil(max / step) * step;
}
