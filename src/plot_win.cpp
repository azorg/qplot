/*
 * QPlot - 2d plotting tool based on Qwt
 * File: "plot_win.cpp" (main Qt window)
 */
//----------------------------------------------------------------------------
#include <QtGui>
#include <QMessageBox>
#include <QDebug>
//----------------------------------------------------------------------------
#include "plot_win.h"
#include "qplot.h"
//----------------------------------------------------------------------------
// вспомагательная функция для отладочной печати
static const char *_b2s(bool on)
{
  static const char *str_true  = "true";
  static const char *str_false = "false";
  return on ? str_true : str_false;
}
//----------------------------------------------------------------------------
PlotWin::PlotWin(QWidget *parent) : QMainWindow(parent)
{
  qDebug("PlotWin::PlotWin(QWidget *parent=%p)", parent);

  ui = new Ui::PlotWin;
  ui->setupUi(this);

  // set menu checkboxes:
  bool on = ui->pa->checkZoom();
  if (ui->actZoom->isChecked() != on)
      ui->actZoom->setChecked(on);

  on = ui->pa->checkLegend();
  if (ui->actLegend->isChecked() != on)
      ui->actLegend->setChecked(on);

  on = ui->pa->checkGrid();
  if (ui->actGrid->isChecked() != on)
      ui->actGrid->setChecked(on);

  //showInfo(tr("Select Help->demo in main menu"));
  qplot_demo(ui->pa);
  ui->pa->redraw();
}
//----------------------------------------------------------------------------
PlotWin::~PlotWin()
{
  qDebug("PlotWin::~PlotWin()");
  delete ui;
}
//----------------------------------------------------------------------------
void PlotWin::showInfo(QString text)
{
  qDebug("PlotWin::showInfo(QString text='%s')",
         text.toLocal8Bit().data());

  statusBar()->showMessage(text);
}
//----------------------------------------------------------------------------
void PlotWin::on_actOpenFile_triggered()
{
  qDebug("PlotWin::on_actOpenFile_triggered() <- "
         "File->Open QPlot mission INI-file");
  //...
}
//----------------------------------------------------------------------------
void PlotWin::on_actExportImg_triggered()
{
  qDebug("PlotWin::on_actExportImg_triggered() <- "
         "File->Export to Image");

  ui->pa->exportImg();
}
//----------------------------------------------------------------------------
void PlotWin::on_actExportSvg_triggered()
{
  qDebug("PlotWin::on_actExportSvg_triggered() <- "
         "File->Export to SVG");

  ui->pa->exportSvg();
}
//----------------------------------------------------------------------------
void PlotWin::on_actExportPdf_triggered()
{
  qDebug("PlotWin::on_actExportPdf_triggered() <- "
         "File->Export to PDF");

  ui->pa->exportPdf();
}
//----------------------------------------------------------------------------
void PlotWin::on_actExportPrn_triggered()
{
  qDebug("PlotWin::on_actExportPrn_triggered() <- "
         "File->Print (Export to PDF/PS)");

  ui->pa->exportPrn();
}
//----------------------------------------------------------------------------
void PlotWin::on_actZoom_toggled(bool on)
{
  qDebug("PlotWin::on_actZoom_toggled(bool on=%s) <- "
         "View->Zoom", _b2s(on));

  ui->pa->enableZoom(on);
}
//----------------------------------------------------------------------------
void PlotWin::on_actResetZoom_triggered()
{
  qDebug("PlotWin::on_actResetZoom_triggered() <- "
         "View->Reset Zoom");

  ui->pa->resetZoom();
}
//----------------------------------------------------------------------------
void PlotWin::on_actLegend_toggled(bool on)
{
  qDebug("PlotWin::on_actLegend_toggled(bool on=%s) <- "
         "View->Legend", _b2s(on));

  ui->pa->enableLegend(on);
}
//----------------------------------------------------------------------------
void PlotWin::on_actGrid_toggled(bool on)
{
  qDebug("PlotWin::on_actGrid_toggled(bool on=%s) <- "
         "View->Grid", _b2s(on));

  ui->pa->enableGrid(on);
}
//----------------------------------------------------------------------------
void PlotWin::on_actAntialiased_toggled(bool on)
{
  qDebug("PlotWin::on_actAntialiased_toggled(bool on=%s) <- "
         "View->Antialiased", _b2s(on));

  ui->pa->enableAntialiased(on);
}
//----------------------------------------------------------------------------
void PlotWin::on_actAbout_triggered()
{
  qDebug("PlotWin::on_actAbout_triggered() <- "
         "Help->About");

  QMessageBox::about(
    this,
    tr("About QPlot"),
    tr("Example plotting tool via Qwt\n"
       "Author: Alex Zorg <azorg@mail.ru>\n"
       "Licensed by GPLv2"));
}
//----------------------------------------------------------------------------
void PlotWin::on_actAboutQt_triggered()
{
  qDebug("PlotWin::on_actAboutQt_triggered() <- "
         "Help->About Qt");

  QMessageBox::aboutQt(this, tr("About Qt"));
}
//----------------------------------------------------------------------------
void PlotWin::on_actDemo_triggered()
{ 
  qDebug("PlotWin::on_actDemo_triggered() <- "
         "Help->Demo");

  //!!! FIXME
  qplot_demo(ui->pa);
}
//----------------------------------------------------------------------------
#if 0 // FIXME
void PlotWin::on_pa_zoomed(double xBottom, double wBottom,
                           double yLeft,   double hLeft,
                           double xTop,    double wTop,
                           double yRight,  double hRight)
{
  // FIXME
  qDebug("PlotWin::on_pa_zoomed(...)");

  qDebug("zoomed0 x=%g w=%g y=%g h=%g",
         xBottom, wBottom, yLeft, hLeft);

  qDebug("zoomed1 x=%g w=%g y=%g h=%g",
         xTop, wTop, yRight, hRight);
}
#endif
//----------------------------------------------------------------------------
void PlotWin::on_pa_zoomOn(bool on)
{
  qDebug("PlotWin::on_pa_zoomOn(bool on=%s)", _b2s(on));

  if (ui->actZoom->isChecked() != on)
    ui->actZoom->setChecked(on);

  showInfo(on ? tr("Zoom On") : tr("Zoom Off"));
}
//----------------------------------------------------------------------------
void PlotWin::on_pa_legendOn(bool on)
{
  qDebug("PlotWin::on_pa_legendOn(bool on=%s)", _b2s(on));

  if (ui->actLegend->isChecked() != on)
    ui->actLegend->setChecked(on);

  showInfo(on ? tr("Legend On") : tr("Legend Off"));
}
//----------------------------------------------------------------------------
void PlotWin::on_pa_gridOn(bool on)
{
  qDebug("PlotWin::on_pa_gridOn(bool on=%s)", _b2s(on));

  if (ui->actGrid->isChecked() != on)
    ui->actGrid->setChecked(on);

  showInfo(on ? tr("Grid On") : tr("Grid Off"));
}
//----------------------------------------------------------------------------
void PlotWin::on_pa_antialiasedOn(bool on)
{
  qDebug("PlotWin::on_pa_antialiasedOn(bool on=%s)", _b2s(on));

  if (ui->actAntialiased->isChecked() != on)
    ui->actAntialiased->setChecked(on);

  showInfo(on ? tr("Antialiased On") : tr("Antialiased Off"));
}
//----------------------------------------------------------------------------
void PlotWin::on_pa_clickOn(double xBottom, double xTop,
                            double yLeft,   double yRight,
                            Qt::MouseButtons buttons,
                            Qt::KeyboardModifiers modifiers)
{
  qDebug("PlotWin::on_pa_clickOn(double xBottom=%g, double xTop=%g,\n"
         "                       double yLeft=%g, double yRight=%g\n"
         "                       buttons=0x%08X)",
         xBottom, xTop, yLeft, yRight, (int) buttons);

  modifiers = modifiers; // FIXME

  QString info;
  //!!! FIXME
  info.sprintf("x1=%g, x2=%g, y1=%g, y2=%g",
               xBottom, xTop, yLeft, yRight);
  showInfo(info);
}
//----------------------------------------------------------------------------
void PlotWin::on_pa_keyOn(QKeyEvent *event)
{
  int k = event->key();
  Qt::KeyboardModifiers m = event->modifiers();
  char key[2];
  key[0] = (char) k;
  key[1] = '\0';

  qDebug("PlotWin::keyPressEvent(key='%s%s', code=0x%02X)",

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

  if (k == Qt::Key_F)
    setWindowState(windowState() ^ Qt::WindowFullScreen);

  if (k == Qt::Key_Q)
    QCoreApplication::exit(0);

  event->accept();
}
//----------------------------------------------------------------------------

/*** end of "plot_win.cpp" file ***/
