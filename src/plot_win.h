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
  PlotWin(const std::string mission_file = "", QWidget *parent = 0);
  ~PlotWin();

  // вывести сообщение в строку состояния
  void showInfo(QString text = QString::null);

  // доступ к PlotArea
  PlotArea *pa() { return ui->pa; }

private Q_SLOTS:
  // обработчики пунктов главного меню:
  void on_actOpenFile_triggered();  // File->Open QPlot mission INI-file
  void on_actExportImg_triggered(); // File->Export to Image
  void on_actExportSvg_triggered(); // File->Export to SVG
  void on_actExportPdf_triggered(); // File->Export to PDF
  void on_actExportPrn_triggered(); // File->Print (Export to PDF/PS)
  void on_actExit_triggered();      // File->Exit

  void on_actZoom_toggled(bool on);        // View->Zoom
  void on_actLegend_toggled(bool on);      // View->Legend
  void on_actGrid_toggled(bool on);        // View->Grid
  void on_actAntialiased_toggled(bool on); // View->Antialiased

  void on_actHelp_triggered();    // Help->Help
  void on_actAbout_triggered();   // Help->About
  void on_actAboutQt_triggered(); // Help->About Qt

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
  //void on_pa_zoomed(double xBottom, double wBottom, // zoomed
  //                  double yLeft,   double hLeft,
  //                  double xTop,    double wTop,
  //                  double yRight,  double hRight);

private:
  Ui::PlotWin *ui;
};
//----------------------------------------------------------------------------
#endif // PLOT_WIN_H

/*** end of "plot_win.h" file ***/

