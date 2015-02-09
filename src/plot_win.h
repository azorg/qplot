/*
 * QPlot - 2d plotting tool based on Qwt
 * File: "plot_win.h" (main Qt window)
 */
//----------------------------------------------------------------------------
#ifndef PLOT_WIN_H
#define PLOT_WIN_H
//----------------------------------------------------------------------------
#include "ui_plot_win.h" // Ui::plot_win, QwtPlot
//----------------------------------------------------------------------------
class PlotWin : public QMainWindow
{
  Q_OBJECT

public:
  PlotWin(QWidget *parent = 0);
  ~PlotWin();

  // вывести сообщение в строку состояния
  void showInfo(QString text = QString::null);

  // демонстрационный вывод графика
  void demoPlot();

private Q_SLOTS:
  // главное меню
  void on_actExportImg_triggered();        // File->Export to Image
  void on_actExportSvg_triggered();        // File->Export to SVG
  void on_actExportPdf_triggered();        // File->Export to PDF
  void on_actExportPrn_triggered();        // File->Print (Export to PDF/PS)

  void on_actZoom_toggled(bool on);        // View->Zoom
  void on_actResetZoom_triggered();        // View->Reset Zooom
  void on_actLegend_toggled(bool on);      // View->Legend
  void on_actGrid_toggled(bool on);        // View->Grid
  void on_actAntialiased_toggled(bool on); // View->Antialiased

  void on_actAbout_triggered();            // Help->About
  void on_actAboutQt_triggered();          // Help->About Qt
  void on_actDemo_triggered();             // Help->Demo

  // обработчики сигналов от ui->pa (PlotArea)
  void on_pa_zoomOn(bool on);        // zoom on/off
  void on_pa_legendOn(bool on);      // legend on/off
  void on_pa_gridOn(bool on);        // grid on/off
  void on_pa_antialiasedOn(bool on); // antialiased on/off

  // on mouse click
  void on_pa_clickOn(double xBottom, double xTop, // point by click
                     double yLeft,   double yRight,
                     Qt::MouseButtons buttons,
                     Qt::KeyboardModifiers modifiers);

  // on key pressed
  void on_pa_keyOn(QKeyEvent *event);

  //!!! FIXME
  //void on_pa_scaled(double xBottom, double wBottom, // zoomed or scralled
  //                  double yLeft,   double hLeft,
  //                  double xTop,    double wTop,
  //                  double yRight,  double hRight);

private:
  bool demo; // true in demo mode
  Ui::PlotWin *ui;
};
//----------------------------------------------------------------------------

#endif // PLOT_WIN_H

/*** end of "plot_win.h" file ***/

