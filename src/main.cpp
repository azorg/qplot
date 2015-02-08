/*
 * QPlot - QWT example 2d plotting tool via Qwt
 * File: "main.cpp"
 * Author: Alex Zorg <azorg@mail.ru>
 */
//----------------------------------------------------------------------------
//#include <qapplication.h> // QApplication
#include <QApplication>
#include "plot_win.h"     // PlotWin
//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  qDebug("QT_VERSION=0x%08x", QT_VERSION);

  QApplication app(argc, argv);

  PlotWin pw;
  //pw.resize(640, 480); // FIXME
  pw.show();

  int retv = app.exec();

  return retv;
}
//----------------------------------------------------------------------------

/*** end of "main.cpp" ***/

