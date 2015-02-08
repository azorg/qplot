/*
 * Example 2d plotting Qt widget based on Qwt
 * File: "plot_area.cpp"
 * Author: Alex Zorg <azorg@mail.ru>
 */
//----------------------------------------------------------------------------
//!!!#include <qprinter.h>
#include <qpicture.h>
#include <qpainter.h>
#include <qfiledialog.h>
//!!!#include <qprintdialog.h>
#include <qfileinfo.h>
#include <qwt_picker_machine.h>
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

    // Shift+RightButton: zoom out by 1
    setMousePattern(QwtEventPattern::MouseSelect3,
                    Qt::RightButton, Qt::ShiftModifier);

    // Ctrl+RightButton: zoom out to full size
    setMousePattern(QwtEventPattern::MouseSelect2,
                    Qt::RightButton, Qt::ControlModifier);
  }
};
//----------------------------------------------------------------------------
PlotArea::PlotArea(QWidget *parent) : QwtPlot(parent)
{
  qDebug("PlotArea::PlotArea(QWidget *parent=%p)", parent);

  setAutoReplot(false);

  //!!! FIXME
  //const int margin = 5;
  //this->setContentsMargins( margin, margin, margin, 0 );
  //setContextMenuPolicy( Qt::NoContextMenu );

  // zoomer (if zoom on)
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

  // picker (if zoom off)
  d_picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
                               QwtPlotPicker::CrossRubberBand,
                               QwtPicker::ActiveOnly,
                               this->canvas());
  d_picker->setStateMachine(new QwtPickerDragPointMachine());
  //d_picker->setRubberBandPen(QColor(Qt::green)); // set in setConf()
  d_picker->setRubberBand(QwtPicker::CrossRubberBand);
  //d_picker->setTrackerPen(QColor(Qt::white)); // set in setConf()

  // magnifier
  d_magnifier = new QwtPlotMagnifier(this->canvas());

  // установки по-умолчанию
  d_legend = (QwtLegend*)     0;
  d_grid   = (QwtPlotGrid*)   0;
  d_vLine  = (QwtPlotMarker*) 0;
  d_hLine  = (QwtPlotMarker*) 0;
  d_marker = (QwtPlotMarker*) 0;

  setConf(d_conf);


  // выделене с помощью zoom'а
  //!!! FIXME
  //connect(zoomer[0], SIGNAL(zoomed(const QwtDoubleRect &)),
  //                     SLOT(zoomed(const QwtDoubleRect &)));

  // finish
  setAutoReplot(true);
  replot();
}
//----------------------------------------------------------------------------
PlotArea::~PlotArea()
{
  qDebug("PlotArea::~PlotArea()");

  clear();
  
  delete d_magnifier;
  delete d_panner;
  delete d_zoomer[1];
  delete d_zoomer[0];
  delete d_picker;

  if (checkVLine())  delete d_vLine;
  if (checkHLine())  delete d_hLine;
  if (checkMarker()) delete d_marker;
  if (checkGrid())   delete d_grid;
  if (checkLegend()) delete d_legend;
}
//----------------------------------------------------------------------------
void PlotArea::setConf(const PlotAreaConf &newConf)
{
  qDebug("PlotArea::setConf()");

  d_conf = newConf;

  // цвет фона
  setCanvasBackground(d_conf.background);

  // zoom
  d_zoomer[0]->setRubberBandPen(d_conf.zoomPen);
  d_zoomer[0]->setTrackerPen(d_conf.trackerPen);
  d_zoomer[0]->setAxis(d_conf.trackerXAxis, d_conf.trackerYAxis);
  d_zoomer[1]->setAxis(d_conf.trackerXAxis != xBottom ? xBottom : xTop,
                       d_conf.trackerYAxis != yLeft   ? yLeft   : yRight);

  enableZoom(d_conf.zoom);

  // legend
  enableLegend(d_conf.legend);

  // grid
  enableGrid(d_conf.grid);

  // picker
  d_picker->setRubberBandPen(d_conf.pickerPen);
  d_picker->setTrackerPen(d_conf.trackerPen);
  d_picker->setAxis(d_conf.trackerXAxis,
                    d_conf.trackerYAxis);
  if (d_conf.pickerAlwaysOn)
    d_picker->setTrackerMode(QwtPicker::AlwaysOn);
  else
    d_picker->setTrackerMode(QwtPicker::ActiveOnly);
}
//----------------------------------------------------------------------------
void PlotArea::resetZoom()
{
  qDebug("PlotArea::resetZoom()");

  d_zoomer[0]->zoom(0);
  d_zoomer[1]->zoom(0);
}
//----------------------------------------------------------------------------
void PlotArea::enableZoom(bool on)
{
  qDebug("PlotArea::enableZoom(bool on=%s)", on ? "true" : "false");

  d_panner->setEnabled(on);

  d_zoomer[0]->setEnabled(on);
  d_zoomer[0]->zoom(0);
  d_zoomer[1]->setEnabled(on);
  d_zoomer[1]->zoom(0);

  bool old = !d_picker->isEnabled();
  d_picker->setEnabled(!on);

  if (on != old)
    emit zoomOn(on);
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
  qDebug("PlotArea::enableLegend(bool on=%s)", on ? "true" : "false");

  if (on && d_legend == (QwtLegend*) 0)
  {
    d_legend = new QwtLegend(this);
    //d_legend->setItemMode(QwtLegend::CheckableItem);

    if (d_conf.legendBox)
      d_legend->setFrameStyle(QFrame::Box | QFrame::Sunken);

    insertLegend(d_legend, d_conf.legendPosition);

    emit legendOn(true);
  }
  else if (!on && d_legend != (QwtLegend*) 0)
  {
    delete d_legend;
    d_legend = (QwtLegend*) 0;
    updateLayout();

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
  qDebug("PlotArea::enableGrid(bool on=%s)", on ? "true" : "false");

  if (on && d_grid == (QwtPlotGrid*) 0)
  {
    d_grid = new QwtPlotGrid();
    d_grid->enableXMin(d_conf.gridXMin);
    d_grid->enableYMin(d_conf.gridYMin);
    //!!! FIXME d_grid->setMajPen(d_conf.gridMajPen);
    //!!! FIXME d_grid->setMinPen(d_conf.gridMinPen);
    d_grid->attach(this);

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
  qDebug("PlotArea::enableVline(bool on=%s)", on ? "true" : "false");

  if (on && d_vLine == (QwtPlotMarker*) 0)
  {
    d_vLine = new QwtPlotMarker();
    d_vLine->setLineStyle(QwtPlotMarker::VLine);
    d_vLine->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
    d_vLine->setLinePen(d_conf.vLinePen);
    d_vLine->attach(this);
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
         x, showText ? "true" : "false");

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
  qDebug("PlotArea::enableHLine(bool on=%s)", on ? "true" : "false");

  if (on && d_hLine == (QwtPlotMarker*) 0)
  {
    d_hLine = new QwtPlotMarker();
    d_hLine->setLineStyle(QwtPlotMarker::HLine);
    d_hLine->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
    d_hLine->setLinePen(d_conf.hLinePen);
    d_hLine->attach(this);
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
         y, showText ? "true" : "false");

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
  qDebug("PlotArea::enableMarker(bool on=%s)", on ? "true" : "false");

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
         x, y, showText ? "true" : "false");

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

  replot();
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
  qDebug("PlotArea::enableAntialiased(bool on=%s)", on ? "true" : "false");

  bool old = d_conf.antialiased;
  d_conf.antialiased = on;

  foreach (QwtPlotCurve *curve, d_crv)
    curve->setRenderHint(QwtPlotItem::RenderAntialiased, on);

  replot();

  if (on != old)
    emit antialiasedOn(on);
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
}
//----------------------------------------------------------------------------
void PlotArea::disableXBottom(bool off)
{
  qDebug("PlotArea::disableXBottom(bool off=%s)", off ? "true" : "false");
  enableAxis(QwtPlot::xBottom, !off);
}
//----------------------------------------------------------------------------
void PlotArea::disableXTop(bool off)
{
  qDebug("PlotArea::disableXTop(bool off=%s)", off ? "true" : "false");
  enableAxis(QwtPlot::xTop, !off);
}
//----------------------------------------------------------------------------
void PlotArea::disableYLeft(bool off)
{
  qDebug("PlotArea::disableYLeft(bool off=%s)", off ? "true" : "false");
  enableAxis(QwtPlot::yLeft, !off);
}
//----------------------------------------------------------------------------
void PlotArea::disableYRight(bool off)
{
  qDebug("PlotArea::disableYRight(bool off=%s)", off ? "true" : "false");
  enableAxis(QwtPlot::yRight, !off);
}
//----------------------------------------------------------------------------
void PlotArea::clear()
{
  qDebug("PlotArea::clear()");

  foreach (QwtPlotCurve *curve, d_crv)
    delete curve;

  d_crv.clear();
}
//----------------------------------------------------------------------------
QwtPlotCurve* PlotArea::addCurve(
  const double *xData,            // указатель на массив X
  const double *yData,            // указатель на массив Y
  int sizeData,                   // число точек (X, Y)
  const QString &title,           // имя графика
  const QPen &pen,                // цвет кривой
  int xAxis,                      // ось X
  int yAxis,                      // ось Y
  QwtPlotCurve::CurveStyle style, // тип кривой
  QwtSymbol::Style symStyle,      // тип символов
  const QPen &symPen,             // цвет символа
  const QBrush &symBrush,         // заливка символа
  int symSize,                    // размер символа
  bool rawData)                   // признак исп. Raw Data
{
  qDebug("PlotArea::addCurve(int sizeData=%i)", sizeData);

  // создать новую кривую и добавить ее в список
  QwtPlotCurve *curve = new QwtPlotCurve(title);

  // сглаживание
  if (d_conf.antialiased)
    curve->setRenderHint(QwtPlotItem::RenderAntialiased);

  // установить цвет/стиль
  if (style != QwtPlotCurve::NoCurve)
  {
    curve->setPen(pen);
    curve->setStyle(style);
  }

  //!!! FIXME легенда???
  curve->setLegendAttribute(QwtPlotCurve::LegendShowLine);

  // установить цвета/стиль/размер символов
  if (symStyle != QwtSymbol::NoSymbol)
  {
    QwtSymbol *sym = new QwtSymbol(symStyle, symBrush, symPen,
                                   QSize(symSize, symSize));
    curve->setSymbol(sym);
  }

  // привязать к осям
  curve->setXAxis(xAxis);
  curve->setYAxis(yAxis);

  // передать/привязать данные
  if (rawData)
    curve->setRawSamples(xData, yData, sizeData);
  else
    curve->setSamples(xData, yData, sizeData);

  // обавить указатель в список кривых
  d_crv.append(curve);

  // привязать к QwtPlot
  curve->attach(this);

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

  bool doAutoReplot = autoReplot();
  int xAxisList[] = {QwtPlot::xBottom, QwtPlot::xTop};

  for (int i = 0; i < 2; i++)
  {
    int axisId = xAxisList[i];
    double x1 = axisScaleDiv(axisId).lowerBound();
    double x2 = axisScaleDiv(axisId).upperBound();
    double dx = xStep * (x2 - x1) / 100.;
    setAxisScale(axisId, x1 + dx, x2 + dx);
  }

  setAutoReplot(doAutoReplot);
  replot();
}
//----------------------------------------------------------------------------
void PlotArea::scrollY(double yStep)
{
  qDebug("PlotArea::scrollY(double yStep=%g)", yStep);

  bool doAutoReplot = autoReplot();
  int yAxisList[] = {QwtPlot::yLeft, QwtPlot::yRight};
  
  for (int i = 0; i < 2; i++)
  {
    int axisId = yAxisList[i];
    double y1 = axisScaleDiv(axisId).lowerBound();
    double y2 = axisScaleDiv(axisId).upperBound();
    double dy = yStep * (y2 - y1) / 100.;
    setAxisScale(axisId, y1 + dy, y2 + dy);
  }
  
  setAutoReplot(doAutoReplot);
  replot();
}
//----------------------------------------------------------------------------
void PlotArea::center()
{
  qDebug("PlotArea::center()");

  bool doAutoReplot = autoReplot();
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

  setAutoReplot(doAutoReplot);
  replot();
}
//----------------------------------------------------------------------------
void PlotArea::getXY(double *xBottom, double *xTop,
                     double *yLeft,   double *yRight)
{
  QPoint pos;
  if (d_picker->isEnabled())
    pos = d_picker->trackerPosition();
  else
    pos = d_zoomer[0]->trackerPosition();
  
  qDebug("PlotArea::getXY(): pos.x()=%i, pos.y()=%i", pos.x(), pos.y());

  *xBottom = invTransform(QwtPlot::xBottom, pos.x());
  *xTop    = invTransform(QwtPlot::xTop,    pos.x());
  *yLeft   = invTransform(QwtPlot::yLeft,   pos.y());
  *yRight  = invTransform(QwtPlot::yRight,  pos.y());
}
//----------------------------------------------------------------------------
void PlotArea::exportImg(QString fname)
{ // экспорт в PNG/BMP
  qDebug("PlotArea::exportImg()");

#ifndef QT_NO_FILEDIALOG
  fname = QFileDialog::getSaveFileName(
            this,
            tr("Export File Name"),
            fname,
            tr("Image (*.png;*.bmp;*.tiff;*.jpg)"));
#endif

  if (!fname.isEmpty())
  {
    //fname = QFileInfo(fname).absolutePath();
    QPixmap pix = QPixmap::grabWidget(this);
    pix.save(fname);
  }
}
//----------------------------------------------------------------------------
void PlotArea::exportSvg(QString fname)
{ // экспорт в SVG
  qDebug("PlotArea::exportSvg()");

#ifdef QT_SVG_LIB
#  ifndef QT_NO_FILEDIALOG
  fname = QFileDialog::getSaveFileName(
      this,
      tr("Export File Name"),
      fname,
      tr("SVG Image (*.svg)"));
#  endif

  if (!fname.isEmpty())
  {
    QSvgGenerator generator;
    generator.setFileName(fname);
    generator.setSize(QSize(800, 600)); // FIXME
    //!!! FIXME
    ///print(generator);
  }
#endif // QT_SVG_LIB
}
//----------------------------------------------------------------------------
void PlotArea::exportPrn(QString name)
{ // экспорт в PDF/PS или печать
  qDebug("PlotArea::exportPrn()");

  //!!! FIXME
  name = name;

//#ifndef QT_NO_PRINTER
#if 0 //!!! FIXME
  QPrinter printer;

  if (!name.isEmpty())
  {
    name.replace(QRegExp (QString::fromLatin1("\n")), tr(" -- "));
    printer.setDocName(name);
  }

  printer.setCreator("QPlot");
  printer.setOrientation(QPrinter::Landscape);

  QPrintDialog dialog(&printer);
  if (dialog.exec())
  {
    QwtPlotPrintFilter filter;
    if (printer.colorMode() == QPrinter::GrayScale)
    {
      int options =  QwtPlotPrintFilter::PrintAll;
      options    &= ~QwtPlotPrintFilter::PrintBackground;
      options    |=  QwtPlotPrintFilter::PrintFrameWithScales;
      filter.setOptions(options);
    }
    print(printer, filter);
  }
#endif
}
//----------------------------------------------------------------------------
#if 0 //!!! FIXME
void PlotArea::zoomed(const QwtDoubleRect &)
{
  qDebug("PlotArea::zoomed()");

  QwtDoubleRect rect = zoomer[0]->zoomRect();
  double xBottom = rect.x();
  double wBottom = rect.width();
  double yLeft   = rect.y();
  double hLeft   = rect.height();

  rect = zoomer[1]->zoomRect();
  double xTop    = rect.x();
  double wTop    = rect.width();
  double yRight  = rect.y();
  double hRight  = rect.height();

  emit scaleOn(xBottom, wBottom,
               yLeft,   hLeft,
               xTop,    wTop,
               yRight,  hRight);
}
#endif
//----------------------------------------------------------------------------
void PlotArea::mousePressEvent(QMouseEvent *evt)
{
  Qt::MouseButtons buttons = evt->buttons();

  const char *button = (buttons & Qt::LeftButton)  ? "LeftButton"  :
                       (buttons & Qt::RightButton) ? "RightButton" :
                       (buttons & Qt::MidButton)   ? "MidleButton" :
                                                     "UnknownButton";
  qDebug("PlotArea::mousePressEvent(%s)", button);

  double        xBottom,  xTop,  yLeft,  yRight;
  getXY       (&xBottom, &xTop, &yLeft, &yRight);

  emit clickOn( xBottom,  xTop,  yLeft,  yRight, buttons);
}
//----------------------------------------------------------------------------
void PlotArea::mouseDoubleClickEvent(QMouseEvent *)
{
  qDebug("PlotArea::mouseDoubleClickEvent()");

  if (checkZoom())
    resetZoom();
}
//----------------------------------------------------------------------------
void PlotArea::keyPressEvent(QKeyEvent *event)
{
  int k = event->key();
  int m = event->modifiers();
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
         (k >= ' ' && k < 128)    ? key         : "???",
         k);

  if (k == Qt::Key_Z)
  { // "Z" -> zoom On/Off
    bool on = !checkZoom();
    enableZoom(on);
  }
  else if (k == Qt::Key_R)
  { // "R" -> reset zoom
    resetZoom();
  }
  else if (k == Qt::Key_L)
  { // "L" -> legend On/Off
    bool on = !checkLegend();
    enableLegend(on);
    emit legendOn(on);
  }
  else if (k == Qt::Key_G)
  { // "G" -> grid On/Off
    bool on = !checkGrid();
    enableGrid(on);
    emit gridOn(on);
  }
  else if (k == Qt::Key_A)
  { // "A" -> antialiased On/Off
    bool on = !checkAntialiased();
    enableAntialiased(on);
    emit antialiasedOn(on);
  }
  else if (k == Qt::Key_C)
  { // "C" -> scroll to center
    center();
  }
  else if (k == Qt::Key_M)
  { // "M" -> on/off marker
    if (m == Qt::ControlModifier)
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
    if (m == Qt::ControlModifier)
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
    if (m == Qt::ControlModifier)
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
}
//----------------------------------------------------------------------------

/*** end of "plot_area.cpp" file ***/
