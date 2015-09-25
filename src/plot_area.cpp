/*
 * PlotArea - 2d plotting widget based on Qwt
 * File: "plot_area.cpp"
 */
//----------------------------------------------------------------------------
#include <qpicture.h>
#include <qpainter.h>
#include <qfiledialog.h>
#include <qprinter.h>
#include <qprintdialog.h>
#include <qfileinfo.h>
#include <qwt_plot_canvas.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_renderer.h>
//----------------------------------------------------------------------------
#include <QKeyEvent>
//----------------------------------------------------------------------------
#ifdef QT_SVG_LIB
#  include <qsvggenerator.h>
#endif
//----------------------------------------------------------------------------
#include "plot_area.h" // plot_area
//----------------------------------------------------------------------------
// по документации на qwt-6.1.2 работает только с Qt версии >= 4.4 (Qt5)
#if QT_VERSION < 0x040400
#  error "QT_VERSION < 0x040400"
#endif
//----------------------------------------------------------------------------
// вспомагательная функция для отладочной печати
static const char *_b2s(bool on)
{
  static const char *str_true  = "true";
  static const char *str_false = "false";
  return on ? str_true : str_false;
}
//----------------------------------------------------------------------------
class PlotAreaZoomer: public QwtPlotZoomer
{
public:
  PlotAreaZoomer(int xAxis, int yAxis, QWidget *canvas):
    QwtPlotZoomer(xAxis, yAxis, canvas)
  {
    // отключить вывод (x, y)
    setTrackerMode(QwtPicker::AlwaysOff);

    // отключить прямоугольник выделения
    setRubberBand(QwtPicker::NoRubberBand);

    // RightButton: zoom back out by 1
    setMousePattern(QwtEventPattern::MouseSelect3,
                    Qt::RightButton, Qt::NoModifier);

    // Shift+RightButton: zoom out to full size
    setMousePattern(QwtEventPattern::MouseSelect2,
                    Qt::RightButton, Qt::ShiftModifier);
  }
};
//----------------------------------------------------------------------------
class PlotAreaTracker: public QObject
{
public:
  PlotAreaTracker(QWidget *parent) : QObject(parent)
  {
    if (parent)
    {
      parent->installEventFilter(this);
      parent->setMouseTracking(true);
    }
    d_pos = QPoint(0, 0);
  }

  virtual bool eventFilter(QObject *object, QEvent *event)
  {
    if (object && object == parent())
      if (event->type() == QEvent::MouseMove)
      {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        d_pos = mouseEvent->pos();
        //qDebug(">>> MouseMoveEvent: x=%i, y=%i", d_pos.x(), d_pos.y());
      }
    return false;
  }

  QPoint position() const { return d_pos; }

private:
  QPoint d_pos;
};
//----------------------------------------------------------------------------
PlotArea::PlotArea(QWidget *parent) : QwtPlot(parent)
{
  qDebug("PlotArea::PlotArea(QWidget *parent=%p)", parent);

  setAutoReplot(false);

  //!!! FIXME
  const int margin = 3;
  this->setContentsMargins( margin, margin, margin, 0);
  setContextMenuPolicy( Qt::NoContextMenu );

  // canvas
  QwtPlotCanvas *canvas = new QwtPlotCanvas();
  canvas->setBorderRadius(7);
  canvas->setLineWidth(1);
  canvas->setFrameStyle(0); // QFrame::Box | QFrame::Plain
  setCanvas(canvas);

  // tracker
  d_tracker = new PlotAreaTracker(this->canvas());

  // zoomer (if zoom off)
  d_zoomer[0] = new PlotAreaZoomer(QwtPlot::xBottom, QwtPlot::yLeft,
                                   this->canvas());
  d_zoomer[0]->setRubberBand(QwtPicker::RectRubberBand);
  //d_zoomer[0]->setRubberBandPen(QColor(Qt::green)); // set in setConf()
  d_zoomer[0]->setTrackerMode(QwtPicker::ActiveOnly);
  d_zoomer[1] = new PlotAreaZoomer(QwtPlot::xBottom, QwtPlot::yLeft,
                                   this->canvas());

  // panner
  d_panner = new QwtPlotPanner(this->canvas());
  d_panner->setMouseButton(Qt::MidButton);

  // picker
  d_picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
                               QwtPlotPicker::CrossRubberBand,
                               QwtPicker::ActiveOnly, // set in setConf()
                               this->canvas());
  d_picker->setStateMachine(new QwtPickerDragPointMachine());
  //d_picker->setRubberBandPen(QColor(Qt::green)); // set in setConf()
  d_picker->setRubberBand(QwtPicker::CrossRubberBand);
  //d_picker->setTrackerPen(QColor(Qt::white)); // set in setConf()

  // magnifier
  d_magnifier = new QwtPlotMagnifier(this->canvas());
  d_magnifier->setMouseButton(Qt::RightButton, Qt::ControlModifier);
  d_magnifier->setWheelModifiers(Qt::ControlModifier); // zoom with Control

  // установки по-умолчанию
  d_legend = (QwtLegend*)     0;
  d_grid   = (QwtPlotGrid*)   0;
  d_vLine  = (QwtPlotMarker*) 0;
  d_hLine  = (QwtPlotMarker*) 0;
  d_marker = (QwtPlotMarker*) 0;

  setConf(d_conf);

  // выделене с помощью zoom'а
  connect(d_zoomer[0], SIGNAL(zoomed(const QRectF &)),
                       SLOT(zoomed(const QRectF &)));
}
//----------------------------------------------------------------------------
PlotArea::~PlotArea()
{
  qDebug("PlotArea::~PlotArea()");

  clear();
  
  if (checkVLine())  delete d_vLine;
  if (checkHLine())  delete d_hLine;
  if (checkMarker()) delete d_marker;
  if (checkGrid())   delete d_grid;
  if (checkLegend()) delete d_legend;

  delete d_magnifier;
  delete d_picker;
  delete d_panner;
  delete d_zoomer[1];
  delete d_zoomer[0];
  delete d_tracker;
}
//----------------------------------------------------------------------------
PlotAreaConf PlotArea::getConf() const
{
  qDebug("PlotArea::getConf()");
  return d_conf;
}
//----------------------------------------------------------------------------
void PlotArea::setConf(const PlotAreaConf &newConf)
{
  qDebug("PlotArea::setConf()");

  d_conf = newConf;

  // цвет фона
  setCanvasBackground(d_conf.background);
  
  //!!! FIXME
  //setAutoFillBackground(true);
  //setPalette(QPalette(d_conf.background));

  // zoom
  d_zoomer[0]->setRubberBandPen(d_conf.zoomPen);
  d_zoomer[0]->setTrackerPen(d_conf.trackerPen);
  d_zoomer[0]->setTrackerFont(d_conf.trackerFont);
  d_zoomer[0]->setAxis(d_conf.trackerXAxis, d_conf.trackerYAxis);
  d_zoomer[1]->setAxis(d_conf.trackerXAxis != xBottom ? xBottom : xTop,
                       d_conf.trackerYAxis != yLeft   ? yLeft   : yRight);
  enableZoom(d_conf.zoom);

  // legend
  enableLegend(d_conf.legend);

  // grid
  enableGrid(d_conf.grid);
  d_grid->setMajorPen(d_conf.gridMajorPen);
  d_grid->setMinorPen(d_conf.gridMinorPen);

  // picker
  d_picker->setRubberBandPen(d_conf.pickerPen);
  d_picker->setTrackerPen(d_conf.trackerPen);
  d_picker->setTrackerFont(d_conf.trackerFont);
  d_picker->setAxis(d_conf.trackerXAxis,
                    d_conf.trackerYAxis);
  if (d_conf.pickerAlwaysOn)
  {
     d_zoomer[0]->setTrackerMode(QwtPicker::AlwaysOn);
     d_picker->setTrackerMode(QwtPicker::AlwaysOn);
  }
  else
  {
    d_zoomer[0]->setTrackerMode(QwtPicker::ActiveOnly);
    d_picker->setTrackerMode(QwtPicker::ActiveOnly);
  }
}
//----------------------------------------------------------------------------
void PlotArea::resetZoom()
{
  qDebug("PlotArea::resetZoom()");

  d_zoomer[0]->zoom(0);
  d_zoomer[1]->zoom(0);

  replot();
}
//----------------------------------------------------------------------------
void PlotArea::enableZoom(bool on)
{
  qDebug("PlotArea::enableZoom(bool on=%s)", _b2s(on));

  d_zoomer[0]->setEnabled(on);
  d_zoomer[1]->setEnabled(on);

  bool old = !d_picker->isEnabled();
  d_picker->setEnabled(!on);

  if (on != old)
  {
    replot();
    emit zoomOn(on);
  }
}
//----------------------------------------------------------------------------
bool PlotArea::checkZoom() const
{
  qDebug("PlotArea::checkZoom()");
  return !d_picker->isEnabled();
}
//----------------------------------------------------------------------------
void PlotArea::enableLegend(bool on)
{
  qDebug("PlotArea::enableLegend(bool on=%s)", _b2s(on));

  if (on && d_legend == (QwtLegend*) 0)
  {
    d_legend = new QwtLegend(this);

    if (d_conf.legendBox)
      d_legend->setFrameStyle(QFrame::Box | QFrame::Sunken);

    insertLegend(d_legend, d_conf.legendPosition);

    replot();
    emit legendOn(true);
  }
  else if (!on && d_legend != (QwtLegend*) 0)
  {
    delete d_legend;
    d_legend = (QwtLegend*) 0;
    updateLayout();

    replot();
    emit legendOn(false);
  }
}
//----------------------------------------------------------------------------
bool PlotArea::checkLegend() const
{
  qDebug("PlotArea::checkLegend()");
  return d_legend != (QwtLegend*) 0 ? true : false;
}
//----------------------------------------------------------------------------
void PlotArea::enableGrid(bool on)
{
  qDebug("PlotArea::enableGrid(bool on=%s)", _b2s(on));

  if (on && d_grid == (QwtPlotGrid*) 0)
  {
    d_grid = new QwtPlotGrid();
    d_grid->enableXMin(d_conf.gridXMin);
    d_grid->enableYMin(d_conf.gridYMin);
    d_grid->setMajorPen(d_conf.gridMajorPen);
    d_grid->setMinorPen(d_conf.gridMinorPen);
    d_grid->attach(this);

    replot();
    emit gridOn(on);
  }
  else if (!on && d_grid != (QwtPlotGrid*) 0)
  {
    delete d_grid;
    d_grid = (QwtPlotGrid*) 0;

    replot();
    emit gridOn(false);
  }
}
//----------------------------------------------------------------------------
bool PlotArea::checkGrid() const
{
  qDebug("PlotArea::checkGrid()");
  return d_grid != (QwtPlotGrid*) 0 ? true : false;
}
//----------------------------------------------------------------------------
void PlotArea::enableVLine(bool on)
{
  qDebug("PlotArea::enableVline(bool on=%s)", _b2s(on));

  if (on && d_vLine == (QwtPlotMarker*) 0)
  {
    d_vLine = new QwtPlotMarker();
    d_vLine->setLineStyle(QwtPlotMarker::VLine);
    d_vLine->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
    d_vLine->setLinePen(d_conf.vLinePen);
    d_vLine->attach(this);
    replot();
  }
  else if (!on && d_vLine != (QwtPlotMarker*) 0)
  {
    delete d_vLine;
    d_vLine = (QwtPlotMarker*) 0;
    replot();
  }
}
//----------------------------------------------------------------------------
void PlotArea::setVLine(double x, bool showText)
{
  qDebug("PlotArea::setVLine(double x=%g, bool showText=%s)",
         x, _b2s(showText));

  enableVLine(true);
  d_vLine->setValue(x, 0.);

  if (showText)
  {
    QString label;
    label.sprintf("%g", x);
    QwtText text(label);
    text.setColor(d_conf.hLineTextColor);
    text.setFont(d_conf.vLineFont);
    d_vLine->setLabel(text);
  }

  replot();
}
//----------------------------------------------------------------------------
bool PlotArea::checkVLine() const
{
  qDebug("PlotArea::checkVLine()");
  return d_vLine != (QwtPlotMarker*) 0 ? true : false;
}
//----------------------------------------------------------------------------
void PlotArea::enableHLine(bool on)
{
  qDebug("PlotArea::enableHLine(bool on=%s)", _b2s(on));

  if (on && d_hLine == (QwtPlotMarker*) 0)
  {
    d_hLine = new QwtPlotMarker();
    d_hLine->setLineStyle(QwtPlotMarker::HLine);
    d_hLine->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
    d_hLine->setLinePen(d_conf.hLinePen);
    d_hLine->attach(this);
    replot();
  }
  else if (!on && d_hLine != (QwtPlotMarker*) 0)
  {
    delete d_hLine;
    d_hLine = (QwtPlotMarker*) 0;
    replot();
  }
}
//----------------------------------------------------------------------------
void PlotArea::setHLine(double y, bool showText)
{
  qDebug("PlotArea::setHLine(double y=%g, bool showText=%s)",
         y, _b2s(showText));

  enableHLine(true);
  d_hLine->setValue(0., y);

  if (showText)
  {
    QString label;
    label.sprintf("%g", y);
    QwtText text(label);
    text.setColor(d_conf.hLineTextColor);
    text.setFont(d_conf.hLineFont);
    d_hLine->setLabel(text);
  }

  replot();
}
//----------------------------------------------------------------------------
bool PlotArea::checkHLine() const
{
  qDebug("PlotArea::checkHLine()");
  return d_hLine != (QwtPlotMarker*) 0 ? true : false;
}
//----------------------------------------------------------------------------
void PlotArea::enableMarker(bool on)
{
  qDebug("PlotArea::enableMarker(bool on=%s)", _b2s(on));

  if (on && d_marker == (QwtPlotMarker*) 0)
  {
    d_marker = new QwtPlotMarker();
    d_marker->setLineStyle(QwtPlotMarker::NoLine);
    d_marker->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
    d_marker->setSymbol(new QwtSymbol(d_conf.markerStyle,
                                      d_conf.markerBrush,
                                      d_conf.markerPen,
                                      QSize(d_conf.markerSize,
                                            d_conf.markerSize)));
    d_marker->attach(this);
    replot();
  }
  else if (!on && d_marker != (QwtPlotMarker*) 0)
  {
    delete d_marker;
    d_marker = (QwtPlotMarker*) 0;
    replot();
  }
}
//----------------------------------------------------------------------------
void PlotArea::setMarker(double x, double y, bool showText)
{
  qDebug("PlotArea::setMarker(double x=%g, double y=%g, bool showText=%s)",
         x, y, _b2s(showText));

  enableMarker(true);
  d_marker->setValue(x, y);

  if (showText)
  {
    QString label;
    label.sprintf("(%g, %g)", x, y);
    QwtText text(label);
    text.setColor(d_conf.markerTextColor);
    text.setFont(d_conf.markerFont);
    d_marker->setLabel(text);
  }
}
//----------------------------------------------------------------------------
bool PlotArea::checkMarker() const
{
  qDebug("PlotArea::checkMarker()");
  return d_marker != (QwtPlotMarker*) 0 ? true : false;
}
//----------------------------------------------------------------------------
void PlotArea::enableAntialiased(bool on)
{
  qDebug("PlotArea::enableAntialiased(bool on=%s)", _b2s(on));

  bool old = d_conf.antialiased;
  d_conf.antialiased = on;

  foreach (QwtPlotCurve *curve, d_crv)
    curve->setRenderHint(QwtPlotItem::RenderAntialiased, on);

  if (on != old)
  {
    replot();
    emit antialiasedOn(on);
  }
}
//----------------------------------------------------------------------------
bool PlotArea::checkAntialiased() const
{
  qDebug("PlotArea::checkAntialiased()");
  return d_conf.antialiased;
}
//----------------------------------------------------------------------------
void PlotArea::setXYTitle(int axisId, const QString &title)
{
  qDebug("PlotArea::setXYTitle()");

  enableAxis  (axisId);
  setAxisTitle(axisId, title);
  replot();
}
//----------------------------------------------------------------------------
void PlotArea::enableXBottom(bool on)
{
  qDebug("PlotArea::enableXBottom(bool on=%s)", _b2s(on));
  enableAxis(QwtPlot::xBottom, on);
  replot();
}
//----------------------------------------------------------------------------
void PlotArea::enableXTop(bool on)
{
  qDebug("PlotArea::enableXTop(bool on=%s)", _b2s(on));
  enableAxis(QwtPlot::xTop, on);
  replot();
}
//----------------------------------------------------------------------------
void PlotArea::enableYLeft(bool on)
{
  qDebug("PlotArea::enableYLeft(bool on=%s)", _b2s(on));
  enableAxis(QwtPlot::yLeft, on);
  replot();
}
//----------------------------------------------------------------------------
void PlotArea::enableYRight(bool on)
{
  qDebug("PlotArea::enableYRight(bool on=%s)", _b2s(on));
  enableAxis(QwtPlot::yRight, on);
  replot();
}
//----------------------------------------------------------------------------
void PlotArea::clear()
{
  qDebug("PlotArea::clear()");

  foreach (QwtPlotCurve *curve, d_crv)
    delete curve;

  d_crv.clear();
  replot();
}
//----------------------------------------------------------------------------
QwtPlotCurve* PlotArea::addCurve(
  const double *xData,   // указатель на массив X
  const double *yData,   // указатель на массив Y
  int sizeData,          // число точек (X, Y)
  const CurveConf &conf, // параметры отображения графика
  bool rawData)          // признак исп. Raw Data
{
  qDebug("PlotArea::addCurve(int sizeData=%i)", sizeData);

  // создать новую кривую и добавить ее в список
  QwtPlotCurve *curve = new QwtPlotCurve(conf.legend);

  // сглаживание
  if (d_conf.antialiased)
    curve->setRenderHint(QwtPlotItem::RenderAntialiased);

  // установить цвет/стиль
  if (conf.curve != QwtPlotCurve::NoCurve)
  {
    curve->setStyle(conf.curve);
    curve->setPen(conf.pen);

    if (conf.curve == QwtPlotCurve::Dots)
    { // round dots
      QPen pen = curve->pen();
      pen.setCapStyle(Qt::RoundCap);
      curve->setPen(pen);
    }

    if (conf.curve == QwtPlotCurve::Lines ||
        conf.curve == QwtPlotCurve::Steps)
      curve->setLegendAttribute(QwtPlotCurve::LegendShowLine);
  }
  else // conf.curve == QwtPlotCurve::NoCurve
  {
    // FIXME: fix Qwt bug (draw line in NoCurve mode)
    curve->setStyle(QwtPlotCurve::Dots);
    curve->setPen(QPen(QColor(0, 0, 0, 0), 0)); // transparant 100%
  }

  // установить цвета/стиль/размер символов
  if (conf.symStyle != QwtSymbol::NoSymbol)
  {
    QwtSymbol *sym = new QwtSymbol(conf.symStyle, conf.symBrush, conf.symPen,
                                   QSize(conf.symSize, conf.symSize));
    curve->setSymbol(sym);

    curve->setLegendAttribute(QwtPlotCurve::LegendShowSymbol);
  }

  // привязать к осям
  curve->setXAxis(conf.xAxis);
  curve->setYAxis(conf.yAxis);

  // передать/привязать данные
  if (rawData)
    curve->setRawSamples(xData, yData, sizeData);
  else
    curve->setSamples(xData, yData, sizeData);

  // добавить указатель в список кривых
  d_crv.append(curve);

  // привязать к QwtPlot
  curve->attach(this);

  replot();
  return curve;
}
//----------------------------------------------------------------------------
void PlotArea::removeCurve(QwtPlotCurve *curve)
{
  qDebug("PlotArea::removeCurve(QwtPlotCurve *curve=%p)", curve);

  if (d_crv.contains(curve))
  {
    d_crv.removeAll(curve);
    delete curve;
  }

  replot();
}
//----------------------------------------------------------------------------
void PlotArea::redraw()
{
  qDebug("PlotArea::redraw()");
  d_zoomer[0]->setZoomBase();
  d_zoomer[1]->setZoomBase();
  updateLayout();
  replot();
}
//----------------------------------------------------------------------------
void PlotArea::scrollX(double xStep)
{
  qDebug("PlotArea::scrollX(double xStep=%g)", xStep);

  int xAxisList[] = {QwtPlot::xBottom, QwtPlot::xTop};

  for (int i = 0; i < 2; i++)
  {
    int axisId = xAxisList[i];
    double x1 = axisScaleDiv(axisId).lowerBound();
    double x2 = axisScaleDiv(axisId).upperBound();
    double dx = xStep * (x2 - x1) / 100.;
    setAxisScale(axisId, x1 + dx, x2 + dx);
  }

  replot();
}
//----------------------------------------------------------------------------
void PlotArea::scrollY(double yStep)
{
  qDebug("PlotArea::scrollY(double yStep=%g)", yStep);

  int yAxisList[] = {QwtPlot::yLeft, QwtPlot::yRight};
  
  for (int i = 0; i < 2; i++)
  {
    int axisId = yAxisList[i];
    double y1 = axisScaleDiv(axisId).lowerBound();
    double y2 = axisScaleDiv(axisId).upperBound();
    double dy = yStep * (y2 - y1) / 100.;
    setAxisScale(axisId, y1 + dy, y2 + dy);
  }

  replot();
}
//----------------------------------------------------------------------------
// zoom по X [+/- %]
void PlotArea::zoomX(double zoomStep)
{
  qDebug("PlotArea::zoomX(double zoomStep=%g)", zoomStep);

  int xAxisList[] = {QwtPlot::xBottom, QwtPlot::xTop};

  for (int i = 0; i < 2; i++)
  {
    int axisId = xAxisList[i];
    double x1 = axisScaleDiv(axisId).lowerBound();
    double x2 = axisScaleDiv(axisId).upperBound();
    double dx = zoomStep * (x2 - x1) / 200.;
    setAxisScale(axisId, x1 + dx, x2 - dx);
  }

  replot();
}
//----------------------------------------------------------------------------
// zoom по Y [+/- %]
void PlotArea::zoomY(double zoomStep)
{
  qDebug("PlotArea::zoomY(double zoomStep=%g)", zoomStep);

  int yAxisList[] = {QwtPlot::yLeft, QwtPlot::yRight};
  
  for (int i = 0; i < 2; i++)
  {
    int axisId = yAxisList[i];
    double y1 = axisScaleDiv(axisId).lowerBound();
    double y2 = axisScaleDiv(axisId).upperBound();
    double dy = zoomStep * (y2 - y1) / 200.;
    setAxisScale(axisId, y1 + dy, y2 - dy);
  }

  replot();
}
//----------------------------------------------------------------------------
void PlotArea::center()
{
  qDebug("PlotArea::center()");

  int yAxisList[] = { QwtPlot::xBottom, QwtPlot::xTop,
                      QwtPlot::yLeft,   QwtPlot::yRight };

  double pos[4];
  getXY(pos + 0, pos + 1, pos + 2, pos + 3);
  
  for (int i = 0; i < 4; i++)
  {
    int axisId = yAxisList[i];
    double d1 = axisScaleDiv(axisId).lowerBound();
    double d2 = axisScaleDiv(axisId).upperBound();
    double dd = (d2 - d1) * 0.5;
    setAxisScale(axisId, pos[i] - dd, pos[i] + dd);
  }

  replot();
}
//----------------------------------------------------------------------------
void PlotArea::getXY(double *xBottom, double *xTop,
                     double *yLeft,   double *yRight)
{
  QPoint pos = d_tracker->position();
  qDebug("PlotArea::getXY(): pos.x()=%i, pos.y()=%i", pos.x(), pos.y());

  *xBottom = invTransform(QwtPlot::xBottom, pos.x());
  *xTop    = invTransform(QwtPlot::xTop,    pos.x());
  *yLeft   = invTransform(QwtPlot::yLeft,   pos.y());
  *yRight  = invTransform(QwtPlot::yRight,  pos.y());
}
//----------------------------------------------------------------------------
void PlotArea::exportImg(QString fname, bool dialog)
{ // экспорт в PNG/BMP
  qDebug("PlotArea::exportImg()");

#ifndef QT_NO_FILEDIALOG
  if (dialog)
    fname = QFileDialog::getSaveFileName(
              this,
              tr("Export File Name"),
              fname,
              tr("Image (*.png *.bmp *.tiff *.jpg)"));
#endif

  if (!fname.isEmpty())
  {
    QPixmap pix = QPixmap::grabWidget(this);
    pix.save(fname);
  }
}
//----------------------------------------------------------------------------
void PlotArea::exportSvg(QString fname, bool dialog)
{ // экспорт в SVG
  qDebug("PlotArea::exportSvg()");

#ifdef QT_SVG_LIB
#  ifndef QT_NO_FILEDIALOG
  if (dialog)
    fname = QFileDialog::getSaveFileName(
              this,
              tr("Export File Name"),
              fname,
              tr("SVG Image (*.svg)"));
#  endif

  if (!fname.isEmpty())
  {
    QSize size = this->size();
    int h = size.height() * 2 / 7; // FIXME: magic conctsnts
    int w = size.width()  * 2 / 7; // FIXME: magic constants
    QwtPlotRenderer renderer;
    renderer.renderDocument(this, fname, "svg", QSize(w, h));
  }
#endif // QT_SVG_LIB
}
//----------------------------------------------------------------------------
void PlotArea::exportPdf(QString fname, bool dialog)
{ // экспорт в PDF
  qDebug("PlotArea::exportPdf()");

#ifndef QT_NO_FILEDIALOG
  if (dialog)
    fname = QFileDialog::getSaveFileName(
              this,
              tr("Export File Name"),
              fname,
              tr("SVG Image (*.pdf)"));
#endif

  if (!fname.isEmpty())
  {
    QSize size = this->size();
    int h = size.height() * 2 / 7; // FIXME: magic conctsnts
    int w = size.width()  * 2 / 7; // FIXME: magic constants
    QwtPlotRenderer renderer;
    renderer.renderDocument(this, fname, "pdf", QSize(w, h));
  }
}
//----------------------------------------------------------------------------
void PlotArea::exportPrn(QString docName)
{ // экспорт в PDF/PS или печать
  qDebug("PlotArea::exportPrn()");

#ifndef QT_NO_PRINTER
  QPrinter printer(QPrinter::HighResolution);

  if (!docName.isEmpty())
  {
    docName.replace(QRegExp(QString::fromLatin1("\n")), tr(" -- "));
    printer.setDocName(docName);
  }

  printer.setCreator("QPlot");
  printer.setOrientation(QPrinter::Landscape);

  QPrintDialog dialog(&printer);
  if (dialog.exec())
  {
    QwtPlotRenderer renderer;

    if (printer.colorMode() == QPrinter::GrayScale)
    {
      renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground);
      renderer.setDiscardFlag(QwtPlotRenderer::DiscardCanvasBackground);
      renderer.setDiscardFlag(QwtPlotRenderer::DiscardCanvasFrame);
      renderer.setLayoutFlag (QwtPlotRenderer::FrameWithScales);
    }

    renderer.renderTo(this, printer);
  }
#endif // !QT_NO_PRINTER
}
//----------------------------------------------------------------------------
void PlotArea::zoomed(const QRectF &rect0)
{
  qDebug("PlotArea::zoomed():");

  //QRectF rect0 = d_zoomer[0]->zoomRect();
  double xBottom = rect0.x();
  double wBottom = rect0.width();
  double yLeft   = rect0.y();
  double hLeft   = rect0.height();
  qDebug() << "    "
           << xBottom << ' ' << wBottom << ' '
           << yLeft   << ' ' << hLeft;

  QRectF rect1 = d_zoomer[1]->zoomRect();
  double xTop    = rect1.x();
  double wTop    = rect1.width();
  double yRight  = rect1.y();
  double hRight  = rect1.height();
  qDebug() << "    "
           << xTop   << ' ' << wTop << ' '
           << yRight << ' ' << hRight;

  emit zoomOn(xBottom, wBottom,
              yLeft,   hLeft,
              xTop,    wTop,
              yRight,  hRight);
}
//----------------------------------------------------------------------------
void PlotArea::mousePressEvent(QMouseEvent *event)
{
  Qt::MouseButtons buttons = event->buttons();
  Qt::KeyboardModifiers modifiers = event->modifiers();

  const char *button = (buttons & Qt::LeftButton)  ? "LeftButton"  :
                       (buttons & Qt::RightButton) ? "RightButton" :
                       (buttons & Qt::MidButton)   ? "MidleButton" :
                                                     "UnknownButton";
  qDebug("PlotArea::mousePressEvent(%s)", button);
  
  if (modifiers == Qt::ShiftModifier && (buttons & Qt::RightButton))
  { // Shift + RightButton => reset zoom
    if (!checkZoom())
      resetZoom(); // в режиме "zoom on" сочетание перехватывает Qwt
    event->accept();
    return;
  }

  if (modifiers == Qt::ShiftModifier && (buttons & Qt::LeftButton))
  { // Shift + LeftButton => set center to cursor
    center();
    event->accept();
    return;
  }

  double        xBottom,  xTop,  yLeft,  yRight;
  getXY       (&xBottom, &xTop, &yLeft, &yRight);
  emit clickOn( xBottom,  xTop,  yLeft,  yRight, buttons, modifiers);
}
//----------------------------------------------------------------------------
void PlotArea::mouseDoubleClickEvent(QMouseEvent *event)
{
  qDebug("PlotArea::mouseDoubleClickEvent()");
  resetZoom();
  event->accept();
}
//----------------------------------------------------------------------------
void PlotArea::wheelEvent(QWheelEvent *event)
{
  qDebug("PlotArea::wheelEvent(delta=%i)", event->delta());

  Qt::KeyboardModifiers modifiers = event->modifiers();
  Qt::MouseButtons buttons = event->buttons();

  if (modifiers != Qt::ControlModifier && buttons == Qt::NoButton)
  { // scroll if Ctrl and any buttons unpressed
    float numDegrees = (float) event->delta() / 8.;
    float numSteps = numDegrees / 15.;

    if (event->orientation() == Qt::Horizontal || modifiers == Qt::ShiftModifier)
      scrollX(numSteps * d_conf.scrollXStep);
    else
      scrollY(numSteps * d_conf.scrollYStep);
  }
  event->accept();
}
//----------------------------------------------------------------------------
void PlotArea::keyPressEvent(QKeyEvent *event)
{
  int k = event->key();
  Qt::KeyboardModifiers m = event->modifiers();
  char key[2];
  key[0] = (char) k;
  key[1] = '\0';

  qDebug("PlotArea::keyPressEvent(key='%s%s', code=0x%02X)",
         
         (m == Qt::ControlModifier) ? "Ctrl+"   :
         (m == Qt::ShiftModifier)   ? "Shift+"  :
         (m == Qt::AltModifier)     ? "Alt+"    :
         (m == Qt::MetaModifier)    ? "Meta+"   :
         (m == Qt::KeypadModifier)  ? "KeyPad+" : "",

         (k == Qt::Key_Right)     ? "Right"     :
         (k == Qt::Key_Left)      ? "Left"      :
         (k == Qt::Key_Up)        ? "Up"        :
         (k == Qt::Key_Down)      ? "Down"      :
         (k == Qt::Key_Escape)    ? "Escape"    :
         (k == Qt::Key_Enter)     ? "Enter"     :
         (k == Qt::Key_Return)    ? "Return"    :
         (k == Qt::Key_Tab)       ? "Tab"       :
         (k == Qt::Key_Insert)    ? "Insert"    :
         (k == Qt::Key_Delete)    ? "Delete"    :
         (k == Qt::Key_Backspace) ? "Backspace" :
         (k == Qt::Key_Home)      ? "Home"      :
         (k == Qt::Key_End)       ? "End"       :
         (k == Qt::Key_PageUp)    ? "PageUp"    :
         (k == Qt::Key_PageDown)  ? "PageDown"  :
         (k >= ' ' && k < 128)    ? key         : "???",
         
         k);

  if (k == Qt::Key_Z)
  { // "Z" -> zoom on/off
    enableZoom(!checkZoom());
  }
  else if (k == Qt::Key_R)
  { // "R" -> reset zoom
    resetZoom();
  }
  else if (k == Qt::Key_Escape)
  { // "ESC" -> reset zoom
    if (!checkZoom())
      resetZoom(); // в режиме "zoom on" ESC перехватывает Qwt
  }
  else if (k == Qt::Key_L)
  { // "L" -> legend on/off
    bool on = !checkLegend();
    enableLegend(on);
  }
  else if (k == Qt::Key_G)
  { // "G" -> grid on/off
    bool on = !checkGrid();
    enableGrid(on);
  }
  else if (k == Qt::Key_A)
  { // "A" -> antialiased on/off
    bool on = !checkAntialiased();
    enableAntialiased(on);
  }
  else if (k == Qt::Key_C)
  { // "C" -> set cursor to center
    center();
  }
  else if (k == Qt::Key_M)
  { // "M" -> on/off marker
    if (checkMarker())
    { // off marker
      enableMarker(false);
    }
    else
    { // on marker
      double x, y, x1, y1;
      getXY(&x, &x1, &y, &y1);
      setMarker(x, y);
    }
  }
  else if (k == Qt::Key_V)
  { // "V" -> on/off vLine
    if (checkVLine())
    { // off vLine
      enableVLine(false);
    }
    else
    { // on vLine
      double x, y, x1, y1;
      getXY(&x, &x1, &y, &y1);
      setVLine(x);
    }
  }
  else if (k == Qt::Key_H)
  { // "H" -> on/off hLine
    if (checkHLine())
    { // off hLine
      enableHLine(false);
    }
    else
    { // on hLine
      double x, y, x1, y1;
      getXY(&x, &x1, &y, &y1);
      setHLine(y);
    }
  }
  else if (m == Qt::ShiftModifier)
  { // Shift pressed
    if      (k == Qt::Key_Right) scrollX( d_conf.scrollXStep); // scroll right
    else if (k == Qt::Key_Left)  scrollX(-d_conf.scrollXStep); // scroll left
    else if (k == Qt::Key_Up)    scrollY( d_conf.scrollYStep); // scroll up
    else if (k == Qt::Key_Down)  scrollY(-d_conf.scrollYStep); // scroll down
  }
  else if (m == Qt::ControlModifier)
  { // Ctrl pressed
    if      (k == Qt::Key_Right) zoomX( d_conf.zoomXStep); // zoom X up
    else if (k == Qt::Key_Left)  zoomX(-d_conf.zoomXStep); // zoom X down
    else if (k == Qt::Key_Up)    zoomY( d_conf.zoomYStep); // zoom Y up 
    else if (k == Qt::Key_Down)  zoomY(-d_conf.zoomYStep); // zoom Y down
  }
  else if (k == Qt::Key_End)      zoomX( d_conf.zoomXStep); // zoom X up
  else if (k == Qt::Key_Home)     zoomX(-d_conf.zoomXStep); // zoom X down
  else if (k == Qt::Key_PageUp)   zoomY( d_conf.zoomYStep); // zoom Y up 
  else if (k == Qt::Key_PageDown) zoomY(-d_conf.zoomYStep); // zoom Y down

  emit keyOn(event);
}
//----------------------------------------------------------------------------

/*** end of "plot_area.cpp" file ***/
