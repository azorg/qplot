/*
 * QPlot - 2d plotting tool based on Qwt
 * File: "qplot.h"
 */
//----------------------------------------------------------------------------
#ifndef QPLOT_H
#define QPLOT_H
//----------------------------------------------------------------------------
#include "aini.h"      // aclass::aini
#include "plot_area.h" // PlotArea, PlotAreaConf
#include "plot_win.h"  // PlotWin
//----------------------------------------------------------------------------
// заполнить PlotAreaConf из секции INI-файла
bool qplot_read_conf(aclass::aini *f,     // INI-file
                     const char *s,       // section
                     PlotAreaConf *conf); // output data
//----------------------------------------------------------------------------
bool qplot_run(
  const char  *mission_file, // имя INI-файла задания
  PlotArea    *pa,           // указатель на PlotArea : QwtPlot widget
  QMainWindow *mw);          // указатель на PlotWin  : QMainWindow
//----------------------------------------------------------------------------
// демонстрационное построение графиков
void qplot_demo(PlotArea *pa);
//----------------------------------------------------------------------------
#endif // QPLOT_H

/*** end of "qplot.h" file ***/
