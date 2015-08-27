/*
 * QPlot - 2d plotting tool based on Qwt
 * File: "plot_win.cpp" (main Qt window)
 */
//----------------------------------------------------------------------------
#include <QtGui>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
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

  QString mission_file = QFileDialog::getOpenFileName(
          this,
          tr("Open QPlot mission INI-File"),
          "",
          tr("QPlot mission INI-file:  *.qplot, *ini(*.qplot *.ini);;"
             "Any files:  *.*(*.*)"));

  if (mission_file != "")
  {
    // проверить существует ли файл
    FILE *f = fopen(mission_file.toLocal8Bit().data(), "r");
    if (f == (FILE*) NULL)
    { // can't open mission file
      //...
      showInfo(tr("Can't open mission INI-file"));
      return;
    }
    fclose(f);

    if (qplot_run(mission_file.toLocal8Bit().data(), ui->pa, this))
      showInfo(tr("Success"));
    else
      showInfo(tr("Failure"));
  }
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
void PlotWin::on_actExit_triggered()
{
  qDebug("PlotWin::on_actExit_triggered() <- "
         "File->Exit");

  QCoreApplication::exit(0);
}
//----------------------------------------------------------------------------
void PlotWin::on_actZoom_toggled(bool on)
{
  qDebug("PlotWin::on_actZoom_toggled(bool on=%s) <- "
         "View->Zoom", _b2s(on));

  ui->pa->enableZoom(on);
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
void PlotWin::on_actAutoscale_triggered()
{
  qDebug("PlotWin::on_actAutoscale_triggered() <- "
         "View->Auto scale axis");

  ui->pa->setAxisAutoScale(QwtPlot::xBottom);
  ui->pa->setAxisAutoScale(QwtPlot::yLeft);
  ui->pa->setAxisAutoScale(QwtPlot::xTop);
  ui->pa->setAxisAutoScale(QwtPlot::yRight);

  ui->pa->replot();
}
//----------------------------------------------------------------------------
void PlotWin::on_actHelp_triggered()
{
  qDebug("PlotWin::on_actHelp_triggered() <- "
         "Help->Help");

  QMessageBox::about(
    this,
    tr("QPlot keys binding"),
    tr(
      " * Mouse wheel + Ctrl - zoom (magnifier)\n"
      " * Right mouse button pressed + Ctrl + mouse up/down - zoom (magnifier)\n"
      " * Midlle mouse button pressed + mouse - shift (panner)\n"
      " * Shift + right mouse button, Esc, R or double click -\n"
      "     return to base scale\n"
      " * Shift + arrow keys - X/Y scrolling\n"
      " * Win/Meta + arrow keys or Home/End/PgUp/PgDn -X/Y zoom\n"
      " * Mouse wheel - vertical scroll\n"
      " * Mouse wheel + Shift - horisintall scrolling\n"
      " * Shift + left mouse button or C - cursor position to center\n"
      " * Z - on/off zoom mode\n"
      " * L - on/off legend\n"
      " * G - on/off grid\n"
      " * A - on/off antialiased\n"
      " * M - set/reset meker\n"
      " * V - set/reset vertical line\n"
      " * H - set/reset horisontal line\n"
      " * F - on/off full screen mode\n"
      " * Q - Quit from application\n"
      " * O - open qplot mission INI-file\n"
      " * S - Auto scale axis\n"
      " * D - Demo mode\n"
      " * F1 - this help\n"
      "\n"
      "In zoom on mode:\n"
      " * Left mouse button - select area zoom\n"
      " * Right mouse button - back 1 zoom step\n"
      "\n"
      "In zoom off mode:\n"
      " * Left mouse button pressed - view horisontal/vertical line\n"
      " * Right mouse button - back 1 zoom step\n"
      ));
}
//----------------------------------------------------------------------------
void PlotWin::on_actAbout_triggered()
{
  qDebug("PlotWin::on_actAbout_triggered() <- "
         "Help->About");

  QMessageBox::about(
    this,
    tr("About QPlot"),
    tr("QPLot - 2-d plotting tool based on Qwt project v"
       QWT_VERSION_STR " (http://qwt.sf.net)\n"
       "Author: Alex Zorg <azorg@mail.ru>\n"
       "Licensed by GPLv2/QWTv1"));
}
//----------------------------------------------------------------------------
void PlotWin::on_actAboutQt_triggered()
{
  qDebug("PlotWin::on_actAboutQt_triggered() <- "
         "Help->About Qt");

  QMessageBox::aboutQt(this, tr("About Qt"));
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
  info.sprintf("x1=%g, y1=%g, x2=%g, y2=%g",
               xBottom, yLeft, xTop, yRight);
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
         (k == Qt::Key_Home)      ? "Home"      :
         (k == Qt::Key_End)       ? "End"       :
         (k == Qt::Key_PageUp)    ? "PageUp"    :
         (k == Qt::Key_PageDown)  ? "PageDown"  :
         (k >= ' ' && k < 128)    ? key         : "???",

         k);

  if (k == Qt::Key_F) // full screen mode
    setWindowState(windowState() ^ Qt::WindowFullScreen);

  if (k == Qt::Key_Q) // quit/exit
    on_actExit_triggered();

  if (k == Qt::Key_O) // open mission file
    on_actOpenFile_triggered();

  if (k == Qt::Key_S) // auto scale axis
    on_actAutoscale_triggered();

  if (k == Qt::Key_D) // demo
    qplot_demo(ui->pa);

  if (k == Qt::Key_F1) // help
    on_actHelp_triggered();

  // !!! FIXME easter egg for debug
  if (k == Qt::Key_I)
  {
    qplot_run("binary.qplot.ini", ui->pa, this);
  }

  event->accept();
}
//----------------------------------------------------------------------------

/*** end of "plot_win.cpp" file ***/
