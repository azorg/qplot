/*
 * QPlot - 2d plotting tool based on Qwt
 * File: "qplot.cpp"
 */
//----------------------------------------------------------------------------
#include "qplot.h"
//----------------------------------------------------------------------------
std::string qplot_mission_file = ""; // имя INI-файла задания
//----------------------------------------------------------------------------
// демонстрационное построение графиков (если нет задания)
void qplot_demo(PlotArea *pa)
{
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
