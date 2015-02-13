/*
 * QPlot - 2d plotting tool based on Qwt
 * File: "qplot.cpp"
 */
//----------------------------------------------------------------------------
#include <stdio.h>
#include "qplot.h"
//----------------------------------------------------------------------------
#define _SET_COLOR(ini_file, section, ident, color)   \
  color = QColor(ini_file->read_str(section, ident,   \
                                    _CS(color.name()) \
                                   ).c_str());
//----------------------------------------------------------------------------
#define _SET_PEN_COLOR(ini_file, section, ident, pen)            \
  pen.setColor(QColor(ini_file->read_str(section, ident,         \
                                         _CS(pen.color().name()) \
                                        ).c_str()));
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
  _SET_COLOR    (f, s, "background",     conf->background);
  _SET_PEN_COLOR(f, s, "zoomColor",      conf->zoomPen);
  _SET_PEN_COLOR(f, s, "gridMajorColor", conf->gridMajorPen);
  _SET_PEN_COLOR(f, s, "gridMinorColor", conf->gridMinorPen);
  _SET_PEN_COLOR(f, s, "pickerColor",    conf->pickerPen);
  _SET_PEN_COLOR(f, s, "trackerColor",   conf->trackerPen);
  _SET_PEN_COLOR(f, s, "vLineColor",     conf->vLinePen);
  _SET_PEN_COLOR(f, s, "hLineColor",     conf->hLinePen);
  _SET_PEN_COLOR(f, s, "markerColor",    conf->markerPen);
  _SET_PEN_COLOR(f, s, "markerBrush",    conf->markerBrush);
  conf->vLineTextColor  = conf->vLinePen.color();
  conf->hLineTextColor  = conf->hLinePen.color();
  conf->markerTextColor = conf->markerPen.color();

  // основной шрифт
  QString fontFamily = _QS(f->read_str(s, "fontFamily",
                                       _CS(conf->markerFont.family())));
  conf->trackerFont.setFamily(fontFamily);
  conf->markerFont.setFamily(fontFamily);
  conf->vLineFont.setFamily(fontFamily);
  conf->hLineFont.setFamily(fontFamily);

  int fontSize = (int) f->read_long(s, "fontSize",
                                    conf->markerFont.pointSize());
  conf->trackerFont.setPointSize(fontSize);
  conf->markerFont.setPointSize(fontSize);
  conf->vLineFont.setPointSize(fontSize);
  conf->hLineFont.setPointSize(fontSize);
  
  int weight = conf->markerFont.weight();
  std::string str = f->read_str(s, "fontWeight",
                                weight == QFont::Light    ? "Light"    :
                                weight == QFont::Normal   ? "Normal"   :
                                weight == QFont::DemiBold ? "DemiBold" :
                                weight == QFont::Bold     ? "Bold"     :
                                weight == QFont::Black    ? "Black"    :
                                "Normal");
  transform(str.begin(), str.end(), str.begin(), tolower);
  weight = str == "light"    ?  QFont::Light    :
           str == "normal"   ?  QFont::Normal   :
           str == "demibold" ?  QFont::DemiBold :
           str == "bold"     ?  QFont::Bold     :
           str == "black"    ?  QFont::Black    : QFont::Normal;
  conf->trackerFont.setWeight(weight);
  conf->markerFont.setWeight(weight);
  conf->vLineFont.setWeight(weight);
  conf->hLineFont.setWeight(weight);
  bool italic = f->read_bool(s, "fontItalic", conf->markerFont.italic());
  conf->trackerFont.setItalic(italic);
  conf->markerFont.setItalic(italic);
  conf->vLineFont.setItalic(italic);
  conf->hLineFont.setItalic(italic);

  // размер маркера
  conf->markerSize = (int) f->read_long(s, "markerSize",
                                        (long) conf->markerSize);
  return true;
}
//----------------------------------------------------------------------------
#define _SET_AXIS_TITLE(f, s, i, axisId, pa)         \
  if (f->has_ident(s, i))                            \
  {                                                  \
    std::string title = f->read_str(s, i, "");       \
    if (title.size())                                \
    {                                                \
      pa->setAxisTitle(QwtPlot::axisId, _QS(title)); \
      pa->enableAxis(QwtPlot::axisId, true);         \
    }                                                \
    else                                             \
      pa->enableAxis(QwtPlot::axisId, false);        \
  }
//----------------------------------------------------------------------------
#define _SET_AXIS_SCALE(f, s, i_min, i_max, axisId, pa) \
  if (f->has_ident(s, i_min) && f->has_ident(s, i_max)) \
  {                                                     \
    double a_min = f->read_double(s, i_min);            \
    double a_max = f->read_double(s, i_max);            \
    pa->setAxisScale(QwtPlot::axisId, a_min, a_max);    \
  }
//----------------------------------------------------------------------------
// установить параметры осей PlotArea из секции INI-файла
bool qplot_read_axis(aclass::aini *f, // INI-file
                     const char *s,   // section
                     PlotArea *pa)    // output data
{
  // проверить наличие секции
  if (!f->has_section(s)) return false;

  qDebug("qplot_read_axis(): read section '%s'' in '%s' file",
           s, f->get_fname());

  // название построения
  std::string title = f->read_str(s, "title", "");
  pa->setTitle(_QS(title));

  // подписи к осям
  _SET_AXIS_TITLE(f, s, "xBottomTitle", xBottom, pa);
  _SET_AXIS_TITLE(f, s, "xTopTitle",    xTop,    pa);
  _SET_AXIS_TITLE(f, s, "yLeftTitle",   yLeft,   pa);
  _SET_AXIS_TITLE(f, s, "yRightTitle",  yRight,  pa);

  // пределы по осям
  _SET_AXIS_SCALE(f, s, "xBottomMin", "xBottomMax", xBottom, pa);
  _SET_AXIS_SCALE(f, s, "xTopMin",    "xTopMax",    xTop,    pa);
  _SET_AXIS_SCALE(f, s, "yLeftMin",   "yLeftMax",   yLeft,   pa);
  _SET_AXIS_SCALE(f, s, "yRightMin",  "yRightMax",  yRight,  pa);

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

  // открыть INI-файл задания
  aclass::aini f(mission_file);

  // прочитать глобальную секцию
  // ---------------------------
  // название главного окна приложения
  std::string title = f.read_str("", "title", "");
  if (title.size()) mw->setWindowTitle(_QS(title));

  // заданная ширина и высота главного окна
  QSize wsize = mw->size();
  int width = (int) f.read_long("", "width", -1);
  if (width > 0) wsize.setWidth(width);
  int height = (int) f.read_long("", "height", -1);
  if (height > 0) wsize.setHeight(height);
  if (width > 0 || height > 0) mw->resize(wsize);

  // прочитать секцию [area] - заполнить PlotAreaConf
  // ------------------------------------------------
  PlotAreaConf conf = pa->getConf();
  qplot_read_conf(&f, "area", &conf);
  pa->setConf(conf);

  // отключить маркер и линии
  pa->enableMarker(false);
  pa->enableVLine(false);
  pa->enableHLine(false);

  // прочитать секцию [axis] - настроить оси
  // ---------------------------------------
  qplot_read_axis(&f, "axis", pa);

  if (!f.has_section("0") && !f.has_section("1"))
    return false; // no curve section

  // удалить все построенные кривые
  pa->clear();

  // прочитать секции c описаниями кривых построений
  int num = (int) f.read_long("", "num", 1000); //!!! FIXME
  for (int i = 0; i < num; i++)
  {
    char s[32]; //!!! FIXME
    snprintf(s, 30, "%i", i);
    if (!f.has_section(s))
      continue;

    std::string file = f.read_str(s, "file", "");
    if (file == "")
      continue;

    long start = f.read_long(s, "start", 0);
    long size  = f.read_long(s, "size", -1);
    long step  = f.read_long(s, "step",  1);
    step = step > 0 ? step : 1;

    std::string fmt = f.read_str(s, "format", "txt");
    int format = 0; // text
    if (fmt == "bin" || fmt == "binary" || fmt == "raw")
      format = 1; // binary



  }


  pa->redraw();
  return true;
}
//----------------------------------------------------------------------------
// демонстрационное построение графиков
void qplot_demo(PlotArea *pa)
{
  qDebug("qplot_demo()");

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
    QPen(Qt::red, 2, Qt::DashLine, Qt::RoundCap), // цвет/тип
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
    QPen(Qt::magenta, 2),   // цвет символа
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
  //pa->enableXTop(true);   // ui->pa->enableAxis(QwtPlot::xTop);
  //pa->enableYRight(true); // ui->pa->enableAxis(QwtPlot::yRight);

  // ox: 0...720, 0...4*M_PI
  pa->setAxisScale(QwtPlot::xBottom, 0., 720.);
  pa->setAxisScale(QwtPlot::xTop,    0., 4*M_PI);

  // oy: -1...1, -10...10
  //pa->setAxisScale(QwtPlot::yLeft,  -1., 1.);
  //pa->setAxisScale(QwtPlot::yRight, -10., 10.);

  // markers
  pa->setVLine(100);
  pa->setHLine(-0.5);
  pa->setMarker(200., 0.5);

  pa->redraw();
}
//----------------------------------------------------------------------------
/*** end of "qplot.cpp" file ***/

