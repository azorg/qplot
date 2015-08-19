/*
 * QPlot - 2d plotting tool based on Qwt
 * File: "qplot.cpp"
 */
//----------------------------------------------------------------------------
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "qplot.h"
#include "qplot_tbl.h"
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
// ignore case string compare (std::string std, const char *cstr)
#define _STRCMP(std, cstr) str_icmp_cc(std.c_str(), cstr)
//----------------------------------------------------------------------------
// fill PlotAreaConf from INI-file section
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
    pa->enableAxis(QwtPlot::axisId, true);              \
  }
//----------------------------------------------------------------------------
// set axes parametrs from INI-file section
bool qplot_read_axis(aclass::aini *f, // INI-file
                     const char *s,   // section
                     PlotArea *pa)    // output data
{
  // enable axis by default
  pa->enableAxis(QwtPlot::xBottom, true);
  pa->enableAxis(QwtPlot::yLeft,   true);
  pa->enableAxis(QwtPlot::xTop,    false);
  pa->enableAxis(QwtPlot::yRight,  false);

  // reset titles
  pa->setAxisTitle(QwtPlot::xBottom, "");
  pa->setAxisTitle(QwtPlot::yLeft,   "");
  pa->setAxisTitle(QwtPlot::xTop,    "");
  pa->setAxisTitle(QwtPlot::yRight,  "");
  pa->setTitle("");

  // set autoscale by default
  pa->setAxisAutoScale(QwtPlot::xBottom);
  pa->setAxisAutoScale(QwtPlot::yLeft);
  pa->setAxisAutoScale(QwtPlot::xTop);
  pa->setAxisAutoScale(QwtPlot::yRight);
      
  // check section exist
  if (!f->has_section(s))
    return false;

  qDebug("qplot_read_axis(): read section '%s'' in '%s' file",
           s, f->get_fname());

  // common tittle
  std::string title = f->read_str(s, "title", "");
  pa->setTitle(_QS(title));

  // axis descritons
  _SET_AXIS_TITLE(f, s, "xBottomTitle", xBottom, pa);
  _SET_AXIS_TITLE(f, s, "xTopTitle",    xTop,    pa);
  _SET_AXIS_TITLE(f, s, "yLeftTitle",   yLeft,   pa);
  _SET_AXIS_TITLE(f, s, "yRightTitle",  yRight,  pa);

  // axis limits
  _SET_AXIS_SCALE(f, s, "xBottomMin", "xBottomMax", xBottom, pa);
  _SET_AXIS_SCALE(f, s, "xTopMin",    "xTopMax",    xTop,    pa);
  _SET_AXIS_SCALE(f, s, "yLeftMin",   "yLeftMax",   yLeft,   pa);
  _SET_AXIS_SCALE(f, s, "yRightMin",  "yRightMax",  yRight,  pa);

  return true;
}
//----------------------------------------------------------------------------
qplot_xy_t qplot_read_binary(
  const std::string file, long start, long size, long step,
  int recordSize, int xType, int yType, int xOff, int yOff)
{
  qplot_xy_t data;
  data.size = 0;

  qDebug("qplot_read_binary(file='%s')", file.c_str());
  
  if (xOff < 0 && yOff < 0)
    return data; // stupid mission

  if ((xOff >= 0 && (xOff + qplot_sizeof_by_id(xType)) > recordSize) ||
      (yOff >= 0 && (yOff + qplot_sizeof_by_id(yType)) > recordSize))
    return data; // wrong offset

  FILE *f = fopen(file.c_str(), "rb");
  if (f == (FILE*) NULL)
    return data; // can't open data file

  char *buf = new char[recordSize];
  if (!buf)
    return data; // Oooops!

  long cnt = 0;
  long step_cnt = 0;
  while (fread(buf, recordSize, 1, f) == 1)
  {
    if (start != 0)
    {
      start--;
      continue;
    }

    if (step_cnt-- != 0)
      continue;
    step_cnt = step - 1;

    if (size != -1 && cnt >= size)
      break;

    if (xOff >= 0)
      data.x.push_back(qplot_peek((const void*) buf, xOff, xType));
    else
      data.x.push_back((double) cnt);

    if (yOff >= 0)
      data.y.push_back(qplot_peek((const void*) buf, yOff, yType));
    else
      data.y.push_back((double) cnt);

    cnt++;
  } // while (fread()...)

  delete[] buf;
  fclose(f);
  data.size = cnt;
  return data;
}
//----------------------------------------------------------------------------
qplot_xy_t qplot_read_text(
  const std::string file, long start, long size, long step,
  char separator, int xCol, int yCol)
{
  qplot_xy_t data;
  data.size = 0;

  qDebug("qplot_read_text(file='%s')", file.c_str());
  
  long cnt = 0;
  long step_cnt = 0;

  if (xCol < 0 && yCol < 0)
    return data; // stupid mission

  std::ifstream fs(file.c_str());
  std::string line;
  while (std::getline(fs, line))
  {
    if (start != 0)
    {
      start--;
      continue;
    }

    if (step_cnt-- != 0)
      continue;
    step_cnt = step - 1;

    if (size != -1 && cnt >= size)
      break;

    std::stringstream str_stream(line);
    std::string cell;
    std::vector<std::string> cells;
    cells.clear();

    while (std::getline(str_stream, cell, separator))
    { //!!! FIXME (use standart trim algoritm next time)
      str_t str = str_cstr(cell.c_str());
      str_t str_trimmed = str_trim(&str);
      
      if (separator == ' ')
      {
        if (str_size(&str_trimmed))
          cells.push_back(str_c(&str_trimmed));
      }
      else
        cells.push_back(str_c(&str_trimmed));
      
      str_free(&str_trimmed);
      str_free(&str);
    }

    if (yCol < 0)
    { // xCol >= 0 && yCol < 0
      if ((int) cells.size() > xCol)
        if (cells[yCol].size())
        {
          data.x.push_back(atof(cells[xCol].c_str()));
          data.y.push_back((double) cnt);
					cnt++;
        }
    }
    else if (xCol < 0)
    { // xCol < 0 && yCol >= 0
      if ((int) cells.size() > yCol)
        if (cells[yCol].size())
        {
          data.x.push_back((double) cnt);
          data.y.push_back(atof(cells[yCol].c_str()));
					cnt++;
        }
    }
    else
    { // xCol >= 0 && yCol >= 0
      if ((int) cells.size() > xCol && (int) cells.size() > yCol)
        if (cells[xCol].size() && cells[yCol].size())
        {
          data.x.push_back(atof(cells[xCol].c_str()));
          data.y.push_back(atof(cells[yCol].c_str()));
					cnt++;
        }
    }
  } // while (std::getline(fs, line))

  data.size = cnt;
  return data;
}
//----------------------------------------------------------------------------
// run by mission INI-file
bool qplot_run(
  const char  *mission_file, // имя INI-файла задания
  PlotArea    *pa,           // указатель на PlotArea : QwtPlot widget
  QMainWindow *mw)           // указатель на PlotWin  : QMainWindow
{
  qDebug("qplot_run(mission_file='%s')", mission_file);

  // open mission INI-file
  aclass::aini f(mission_file);

  std::string title = f.read_str("", "title", "");
  if (!title.size())
    title = mission_file;
  mw->setWindowTitle("QPlot - " + _QS(title));

  QSize wsize = mw->size();
  int width = (int) f.read_long("", "width", -1);
  if (width > 0) wsize.setWidth(width);
  int height = (int) f.read_long("", "height", -1);
  if (height > 0) wsize.setHeight(height);
  if (width > 0 || height > 0) mw->resize(wsize);

  // read [area] section
  PlotAreaConf conf = pa->getConf();
  qplot_read_conf(&f, "area", &conf);
  pa->setConf(conf);

  // off all markers
  pa->enableMarker(false);
  pa->enableVLine(false);
  pa->enableHLine(false);

  // read [axis] section
  qplot_read_axis(&f, "axis", pa);

  if (!f.has_section("0") && !f.has_section("1"))
    return false; // no curve section

  // remove all old curves
  pa->clear();

  // read curve description sections [0], [1], [2], ...
  int begin = (int) f.read_long("", "begin", 0);
  int end   = (int) f.read_long("", "end", 100);
  for (int i = begin; i <= end; i++)
  {
    qplot_xy_t data;

    str_t tmp = str_int(i);
    std::string s = str_c(&tmp);
    str_free(&tmp);
    if (!f.has_section(s)) continue;

    std::string file = f.read_str(s, "file", "");
    if (file.size() == 0) continue;

    long start = f.read_long(s, "start", 0);
    long size  = f.read_long(s, "size", -1);
    long step  = f.read_long(s, "step",  1);
    if (step <= 0) step = 1;

    std::string fmt = f.read_str(s, "format", "txt");
    if (_STRCMP(fmt, "bin"   ) == 0 ||
        _STRCMP(fmt, "binary") == 0 ||
        _STRCMP(fmt, "raw"   ) == 0)
    { // binary data file format
      long recordSize = f.read_long(s, "recordSize");
      if (recordSize <= 0)
      {
        qDebug("recordSize = %li, skip [%i] section",
               recordSize, i);
        continue;
      }

      int xOff = (int) f.read_long(s, "xOff", -1);
      int yOff = (int) f.read_long(s, "yOff", -1);

      std::string type = f.read_str(s, "xType", "float");
      int xType = qplot_id_by_type(type);

      type = f.read_str(s, "yType", "float");
      int yType = qplot_id_by_type(type);

      data = qplot_read_binary(file, start, size, step,
                               recordSize, xType, yType, xOff, yOff);
    }
    else
    { // text data file format
      std::string sep = f.read_str(s, "separator", " ");
      char separator = (sep.size() > 0) ? sep[0] : ' ';

      int xCol = (int) f.read_long(s, "xCol", -1);
      int yCol = (int) f.read_long(s, "yCol", -1);

      data = qplot_read_text(file, start, size, step,
                             separator, xCol, yCol);
    }

    if (data.size <= 0)
    {
      qDebug("data.size = %i, skip [%i] section", data.size, i);
      continue;
    }

    CurveConf conf;
    conf.legend = _QS(f.read_str(s, "legend", ""));

    conf.curve = qplot_qwt_curve_by_name(f.read_str(s, "curve", "lines"));

    conf.pen = QPen(
      QColor(f.read_str(s, "color", "black").c_str()),
      f.read_double(s, "width", 1.0),
      qplot_pen_by_name(f.read_str(s, "style", "solid")));

    conf.symStyle = qplot_qwt_symbol_by_name(
      f.read_str(s, "symbol", "no"));

    conf.symPen = QPen(
      QColor(f.read_str(s, "symColor", "blue").c_str()),
      f.read_double(s, "symWidth", 1.0),
      Qt::SolidLine);

    conf.symBrush = QBrush(
      QColor(f.read_str(s, "symBrush", "gray").c_str()));

    conf.symSize = (int) f.read_long(s, "symSize", 7);

    std::string axis = f.read_str(s, "axisX", "Bottom");
    conf.xAxis = !_STRCMP(axis, "Top") ? QwtPlot::xTop : QwtPlot::xBottom;

    axis = f.read_str(s, "axisY", "Left");
    conf.yAxis = !_STRCMP(axis, "Right") ? QwtPlot::yRight : QwtPlot::yLeft;

		if (data.size)
      //pa->addCurve(data.x.data(), data.y.data(), data.size, conf /*, false*/);
      pa->addCurve(&data.x[0], &data.y[0], data.size, conf /*, false*/);
  } // for (int i = 0; i < num; i++)

  pa->redraw();
  return true;
}
//----------------------------------------------------------------------------
// demo mode
void qplot_demo(PlotArea *pa)
{
  qDebug("qplot_demo()");

  // create curves data
  int N = 1000; // !!!
  std::vector<double> t(N);
  std::vector<double> x(N);

  for (int i = 0; i < N; i++)
  {
    t[i]  = ((double) i) * 360. * 2. / ((double) N);
    x[i]  = cos(t[i] * M_PI / 180.);
  }

  int n = 27;
  std::vector<double> fi(n);
  std::vector<double> y(n);

  for (int i = 0; i < n; i++)
  {
    double t  = ((double) i) * 360. * 2. / ((double) n);
    fi[i] = t * M_PI / 180.;
    y[i]  = sin(t * M_PI / 180.) * 10;
  }

  pa->clear();
  CurveConf conf;

  // добавить график "X(t)"
  conf.legend   = "X(t)";
  conf.curve    = QwtPlotCurve::Lines;
  conf.pen      = QPen(Qt::red, 2, Qt::DotLine, Qt::RoundCap);
  conf.symStyle = QwtSymbol::NoSymbol;
  conf.xAxis    = QwtPlot::xBottom;
  conf.yAxis    = QwtPlot::yLeft;

	if (N)
    //QwtPlotCurve *X =
    pa->addCurve(
      //t.data(), // указатель на массив X
      //x.data(), // указатель на массив Y
      &t[0],    // указатель на массив X
      &x[0],    // указатель на массив Y
      N,        // число точек (X, Y)
      conf);    // параметры отображения графика

  // добавить график "Y(fi)"
  conf.legend   = "Y(fi)";
  //conf.curve    = QwtPlotCurve::NoCurve;
  //conf.curve    = QwtPlotCurve::Steps;
  conf.curve    = QwtPlotCurve::Sticks;
  conf.pen      = QPen(Qt::green, 1);
  conf.symStyle = QwtSymbol::XCross;
  //conf.symStyle = QwtSymbol::NoSymbol;
  conf.symPen   = QPen(Qt::magenta, 3);
  conf.symBrush = QBrush(Qt::gray);
  conf.symSize  = 7;
  conf.xAxis    = QwtPlot::xTop;
  conf.yAxis    = QwtPlot::yRight;
  pa->addCurve(
    //fi.data(), // указатель на массив X
    //y.data(),  // указатель на массив Y
    &fi[0],    // указатель на массив X
    &y[0],     // указатель на массив Y
    n,         // число точек (X, Y)
    conf,      // параметры отображения графика
    false);    // признак исп. Raw Data

  //pa->removeCurve(X);

  // axes
  //pa->setXYTitle(QwtPlot::xTop,    "fi");
  //pa->setXYTitle(QwtPlot::xBottom, "t");
  //pa->setXYTitle(QwtPlot::yLeft,   "X");
  //pa->setXYTitle(QwtPlot::yRight,  "Y");
  pa->enableXBottom();    // ui->pa->enableAxis(QwtPlot::xBottom);
  pa->enableYLeft();      // ui->pa->enableAxis(QwtPlot::yLeft);
  pa->enableXTop(true);   // ui->pa->enableAxis(QwtPlot::xTop);
  pa->enableYRight(true); // ui->pa->enableAxis(QwtPlot::yRight);

  // ox: 0...720, 0...4*M_PI
  pa->setAxisScale(QwtPlot::xBottom, 0., 720.);
  pa->setAxisScale(QwtPlot::xTop,    0., 4*M_PI);

  // oy: -1...1, -10...10
  pa->setAxisScale(QwtPlot::yLeft,  -1., 1.);
  pa->setAxisScale(QwtPlot::yRight, -10., 10.);

  // markers
  //pa->setVLine(100);
  //pa->setHLine(-0.5);
  //pa->setMarker(200., 0.5);

  pa->redraw();
}
//----------------------------------------------------------------------------

/*** end of "qplot.cpp" file ***/
