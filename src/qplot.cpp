/*
 * QPlot - 2d plotting tool based on Qwt
 * File: "qplot.cpp"
 */
//----------------------------------------------------------------------------
#include "qplot.h"
//----------------------------------------------------------------------------
// std::string -> QString
#define _QS(str) QString::fromLocal8Bit(str.c_str())
//----------------------------------------------------------------------------
// QString -> char*
#define _CS(str) (str.toLocal8Bit().data())
//----------------------------------------------------------------------------
static void qplot_set_color(aclass::aini *ini_file,
                            const char *section, const char *ident,
                            QColor &color)
{
 color = QColor(ini_file->read_str(section, ident,
                                   _CS(color.name())
                                  ).c_str());
}
//----------------------------------------------------------------------------
static void qplot_set_pen_color(aclass::aini *ini_file,
                                 const char *section, const char *ident,
                                 QPen &pen)
{
  pen.setColor(QColor(ini_file->read_str(section, ident,
                                         _CS(pen.color().name())
                                        ).c_str()));
}
//----------------------------------------------------------------------------
static void qplot_set_brush_color(aclass::aini *ini_file,
                                 const char *section, const char *ident,
                                 QBrush &brush)
{
  brush.setColor(QColor(ini_file->read_str(section, ident,
                                           _CS(brush.color().name())
                                          ).c_str()));
}
//----------------------------------------------------------------------------
// заполнить PlotAreaConf из секции INI-файла
bool qplot_read_conf(aclass::aini *f,    // INI-file
                     const char *s,      // section
                     PlotAreaConf *conf) // output data
{
  // проверить наличие секции
  if (!f->has_section(s)) return false;

  qDebug("qplot_read_conf(): read section '%s'' in '%s' file",
         s, f->get_fname());

  // булевы опции
  conf->zoom     = f->read_bool(s, "zoom",     conf->zoom);
  conf->legend   = f->read_bool(s, "legend",   conf->legend);
  conf->grid     = f->read_bool(s, "grid",     conf->grid);
  conf->gridXMin = f->read_bool(s, "gridXmin", conf->gridXMin);
  conf->gridYMin = f->read_bool(s, "gridYmin", conf->gridYMin);
  conf->antialiased    = f->read_bool(s, "antialiased", conf->antialiased);
  conf->pickerAlwaysOn = f->read_bool(s, "pickerAlwaysOn",
                                      conf->pickerAlwaysOn);

  // шаг сдига/прокрутки в процентах
  conf->scrollXStep = f->read_double(s, "scrollXStep", conf->scrollXStep);
  conf->scrollYStep = f->read_double(s, "scrollYStep", conf->scrollYStep);

  // основные цвета
  qplot_set_color      (f, s, "background",     conf->background);
  qplot_set_pen_color  (f, s, "zoomColor",      conf->zoomPen);
  qplot_set_pen_color  (f, s, "gridMajorColor", conf->gridMajorPen);
  qplot_set_pen_color  (f, s, "gridMinorColor", conf->gridMinorPen);
  qplot_set_pen_color  (f, s, "pickerColor",    conf->pickerPen);
  qplot_set_pen_color  (f, s, "trackerColor",   conf->trackerPen);
  qplot_set_pen_color  (f, s, "vLineColor",     conf->vLinePen);
  qplot_set_pen_color  (f, s, "hLineColor",     conf->hLinePen);
  qplot_set_pen_color  (f, s, "markerColor",    conf->markerPen);
  qplot_set_brush_color(f, s, "markerBrush",    conf->markerBrush);

  // основной шрифт
  QString fontFamily = _QS(f->read_str(s, "font",
                                       _CS(conf->markerFont.family())));
  conf->markerFont.setFamily(fontFamily);
  conf->vLineFont.setFamily(fontFamily);
  conf->hLineFont.setFamily(fontFamily);

  int fontSize = (int) f->read_long(s, "fontSize",
                                    conf->markerFont.pointSize());
  conf->markerFont.setPointSize(fontSize);
  conf->vLineFont.setPointSize(fontSize);
  conf->hLineFont.setPointSize(fontSize);

  // размер маркера
  conf->markerSize = (int) f->read_long(s, "markerSize",
                                        (long) conf->markerSize);

  return true;
}
//----------------------------------------------------------------------------
// постоить графики на основе файла задания
// (возвращает false, если ни одного графика не построено)
bool qplot_run(
  const char  *mission_file, // имя INI-файла задания
  PlotArea    *pa,           // указатель на PlotArea : QwtPlot widget
  QMainWindow *mw)           // указатель на PlotWin  : QMainWindow
{
  qDebug("qplot_run(mission_file='%s')", mission_file);
  pa = pa;

  // открыть INI-файл задания
  aclass::aini mf(mission_file);
//!!! FIXME
//  if (!mf.has_section("0") && !mf.has_section("1"))
//    return false; // no curve section

  // прочитать глобальную секцию
  // ---------------------------
  // название для главного окна приложения
  std::string title = mf.read_str("", "title", "");
  if (title.size()) mw->setWindowTitle(_QS(title));

  // заданная ширина и высота главного окна
  QSize wsize = mw->size();
  int width = (int) mf.read_long("", "width", -1);
  if (width > 0) wsize.setWidth(width);
  int height = (int) mf.read_long("", "height", -1);
  if (height > 0) wsize.setHeight(height);
  if (width > 0 || height > 0) mw->resize(wsize);

  // прочитать секцию [area] - заполнить PlotAreaConf
  // ------------------------------------------------
  PlotAreaConf conf = pa->getConf();
  qplot_read_conf(&mf, "area", &conf);
  pa->setConf(conf); // установить конфигурацию PlotArea

  pa->replot();
  return true;
}
//----------------------------------------------------------------------------
// демонстрационное построение графиков
void qplot_demo(PlotArea *pa)
{
  qDebug("qplot_demo()");

  //!!!
  // set picker always on
  PlotAreaConf conf = pa->getConf();
  conf.pickerAlwaysOn = true;
  pa->setConf(conf);

  // create curves data
  int N = 1000; // !!!
  double *t  = new double[N];
  double *x  = new double[N];
  for (int i = 0; i < N; i++)
  {
    t[i]  = ((double) i) * 360. * 2. / ((double) N);
    x[i]  = cos(t[i] * M_PI / 180.);
  }

  int n = 27;
  double *fi = new double[N];
  double *y  = new double[N];
  for (int i = 0; i < n; i++)
  {
    double t  = ((double) i) * 360. * 2. / ((double) n);
    fi[i] = t * M_PI / 180.;
    y[i]  = sin(t * M_PI / 180.) * 10;
  }

  pa->clear();

  // добавить график "X(t)"
  //QwtPlotCurve *X =
  pa->addCurve(
    t,                     // указатель на массив X
    x,                     // указатель на массив Y
    N,                     // число точек (X, Y)
    "X(t)",                // имя графика
    QPen(Qt::red, 5, Qt::DashLine, Qt::RoundCap), // цвет/тип
    QwtPlot::xBottom,      // ось X
    QwtPlot::yLeft,        // ось Y
    QwtPlotCurve::Lines);  // тип кривой

  // добавить график "Y(fi)"
  pa->addCurve(
    fi,                     // указатель на массив X
    y,                      // указатель на массив Y
    n,                      // число точек (X, Y)
    "Y(fi)",                // имя графика
    QPen(Qt::green, 3),     // цвет/тип
    QwtPlot::xTop,          // ось X
    QwtPlot::yRight,        // ось Y
    //QwtPlotCurve::Lines,  // тип кривой
    QwtPlotCurve::Sticks,   // тип кривой
    QwtSymbol::XCross,      // тип символов
    QPen(Qt::black, 2),     // цвет символа
    QBrush(Qt::gray),       // заливка символа
    7,                      // размер символа
    false);                 // признак исп. Raw Data

  //pa->removeCurve(X);

  delete[] y;
  delete[] x;
  delete[] fi;
  delete[] t;

  // axes
  //pa->setXYTitle(QwtPlot::xTop,    "fi");
  //pa->setXYTitle(QwtPlot::xBottom, "t");
  //pa->setXYTitle(QwtPlot::yLeft,   "X");
  //pa->setXYTitle(QwtPlot::yRight,  "Y");
  pa->enableXTop(true);   // ui->pa->enableAxis(QwtPlot::xTop);
  pa->enableYRight(true); // ui->pa->enableAxis(QwtPlot::yRight);

  // ox: 0...720, 0...4*M_PI
  pa->setAxisScale(QwtPlot::xBottom, 0., 720.);
  pa->setAxisScale(QwtPlot::xTop,    0., 4*M_PI);

  // oy: -1...1, -10...10
  pa->setAxisScale(QwtPlot::yLeft,  -1., 1.);
  pa->setAxisScale(QwtPlot::yRight, -10., 10.);

  // markers
  pa->setVLine(100);
  pa->setHLine(-0.5);
  pa->setMarker(200., 0.5);

  //pa->redraw();
  //pa->replot();
}
//----------------------------------------------------------------------------
/*** end of "core.h" file ***/
