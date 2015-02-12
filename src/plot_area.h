/*
 * PlotArea - 2d plotting widget based on Qwt
 * File: "plot_area.h"
 */
//----------------------------------------------------------------------------
#ifndef PLOT_AREA_H
#define PLOT_AREA_H
//----------------------------------------------------------------------------
#include <qwt_plot.h>           // QwtPlot
#include <qwt_legend.h>         // QwtLegend
#include <qwt_plot_grid.h>      // QwtPlotGrid
#include <qwt_plot_picker.h>    // QwtPlotPicker
#include <qwt_plot_zoomer.h>    // QwtPlotZoomer
#include <qwt_plot_panner.h>    // QwtPlotPanner
#include <qwt_plot_magnifier.h> // QwtPlotMagnifier
#include <qwt_plot_marker.h>    // QwtPlotMarker
#include <qwt_plot_curve.h>     // QwtPlotCurve
#include <qwt_symbol.h>         // QwtSymbol
//----------------------------------------------------------------------------
// параметры конфигурации для для PlotArea
class PlotAreaConf
{
public:
  PlotAreaConf()
  { // параметры по-умолчанию:
    background      = Qt::white;            // белый фон
    zoom            = false;                // zoom вкл.
    zoomPen         = QPen(Qt::darkMagenta, 0, Qt::SolidLine);
    legend          = false;                // легенда выкл.
    legendBox       = false;                // легенда без рамки
    legendPosition  = QwtPlot::RightLegend; // легенда справа
    grid            = true;                 // сетка выкл.
    gridXMin        = true;                 // мелкая сетка по X
    gridYMin        = true;                 // мелкая сетка по Y
    gridMajorPen    = QPen(Qt::darkGray, 0, Qt::DashLine);
    gridMinorPen    = QPen(Qt::gray,     0, Qt::DotLine);
    antialiased     = false;                // сглаживание откл.
    pickerPen       = QPen(Qt::darkMagenta, 0, Qt::SolidLine);
    pickerAlwaysOn  = false;                // координаты курсора всегда
    trackerPen      = QColor(Qt::darkBlue);
    trackerXAxis    = QwtPlot::xBottom;     // нижняя ось X
    trackerYAxis    = QwtPlot::yLeft;       // левая ось Y
    trackerFont     = QFont("Helvetica", 10, QFont::Bold);
    scrollXStep     = 2.;                   // шаг сдвига/прокрутки по X 2%
    scrollYStep     = 2.;                   // шаг сдвига/прокрутки по Y 2%
    vLinePen        = QPen(Qt::blue, 0, Qt::DashDotLine);
    vLineTextColor  = Qt::blue;
    vLineFont       = QFont("Helvetica", 10, QFont::Bold);
    hLinePen        = QPen(Qt::blue, 0, Qt::DashDotLine);
    hLineTextColor  = Qt::blue;
    hLineFont       = QFont("Helvetica", 10, QFont::Bold);
    markerStyle     = QwtSymbol::Diamond;
    markerBrush     = QColor(Qt::yellow);
    markerPen       = QColor(Qt::blue);
    markerSize      = 9;
    markerTextColor = Qt::blue;
    markerFont      = QFont("Helvetica", 10, QFont::Bold);
  }

  QColor background;                      // цвет фона
  bool zoom;                              // режим zoom
  QPen zoomPen;                           // тип линии выделения
  bool legend;                            // отображать легенду
  bool legendBox;                         // легенда в рамке
  QwtPlot::LegendPosition legendPosition; // позиция легенды
  bool grid;                              // отображать сетку
  bool gridXMin;                          // мелкая сетка по X
  bool gridYMin;                          // мелкая сетка по Y
  QPen gridMajorPen;                      // тип сетки
  QPen gridMinorPen;                      // тип мелкой сетки
  bool antialiased;                       // сглаживание (тормозит!)
  QPen pickerPen;                         // тип линий указания
  bool pickerAlwaysOn;                    // показывать координаты курсора
  QPen trackerPen;                        // цвет трекера
  int trackerXAxis;                       // ось X трекера
  int trackerYAxis;                       // ось Y трекера
  QFont trackerFont;                      // шрифт трекера
  double scrollXStep;                     // шаг сдвига по X [%]
  double scrollYStep;                     // шаг сдвига по Y [%]
  QPen   vLinePen;                        // тип вертикальной линии маркера
  QColor vLineTextColor;                  // цвет надписи верт. линии
  QFont  vLineFont;                       // шрифт на верт. линии маркера
  QPen   hLinePen;                        // тип горизонтальной линии маркера
  QColor hLineTextColor;                  // цвет надписи гориз. линии маркера
  QFont  hLineFont;                       // шрифт надписи гориз. линии маркера
  QwtSymbol::Style markerStyle;           // стиль маркера
  QBrush           markerBrush;           // заливка маркера
  QPen             markerPen;             // контур маркера
  int              markerSize;            // размер маркера
  QColor           markerTextColor;       // цвет надписи маркера
  QFont            markerFont;            // шрифт маркера
};
//----------------------------------------------------------------------------
class PlotAreaZoomer;
class PlotAreaTracker;
//----------------------------------------------------------------------------
// наследик QwtPlot с рядом типичных и дополнительных свойств и методов
class PlotArea : public QwtPlot
{
  Q_OBJECT

public:
  PlotArea(QWidget *parent = 0); // конструктор
  virtual ~PlotArea();           // деструктор

  // полезные методы унаследованные от QwtPlot/QWidget без перегрузки:
  //void setTitle(const QString &title);
  //void setCanvasBackground(const QColor &c);wt
  //void canvas()->setLineWidth(int w);
  //void QWidget::setContentsMargins(int left, int top, int right, int bottom);
  //enum Axis {yLeft, yRight, xBottom, xTop, axisCnt}
  //void enableAxis(int axisId, bool tf=true);
  //void setAxisTitle(int axisId, const QString &);
  //void setAxisFont(int axisId, const QFont &f);
  //void setAxisAutoScale(int axisId, bool on=true);
  //void setAxisScale(int axisId, double min, double max, double step=0);
  //void setAxisScaleEngine(int axisId, QwtScaleEngine *se);
  //void setAxis...

  // вернуть текущую конфигурацию (цвета, типы линий, опции отображения)
  PlotAreaConf getConf() const;

  // установить конфигурацию (цвета, типы линий, опции отображения)
  void setConf(const PlotAreaConf &conf);

  void resetZoom();                // сбросить zoom
  void enableZoom(bool on);        // включить zoom
  bool checkZoom() const;          // true если вкл. zoom

  void enableLegend(bool on);      // включить legend
  bool checkLegend() const;        // true если вкл. legend

  void enableGrid(bool on);        // включить grid
  bool checkGrid() const;          // true если вкл. grid

  void enableVLine(bool on);       // включить вертикальную линию маркера
  void setVLine(double y, bool showText = true); // устан. верт. линию
  bool checkVLine() const;         // true если верт. линия маркера вкл.
  
  void enableHLine(bool on);       // включить горизонтальную линию маркера
  void setHLine(double x, bool showText = true); // устан. горизонт. линию
  bool checkHLine() const;         // true если гориз. линия маркера вкл.

  void enableMarker(bool on);       // включить маркер
  void setMarker(double x, double y, bool showText = true); // устан. марк.
  bool checkMarker() const;         // true если маркер вкл.

  void enableAntialiased(bool on); // включить Antialiased
  bool checkAntialiased() const;   // true если вкл. Antialiased

  // метки осей (при задаче метки ось включается)
  // axisId: QwtPLot::yLeft, QwtPLot::yRight, QwtPLot::xBottom, QwtPLot::xTop}
  void setXYTitle(int axisId, const QString &title);

  // отключение осей (по умолчанию QwtPlot включает yLeft и xBottom)
  void enableXBottom(bool on = true);  // вкл. нижнюю ось X
  void enableXTop   (bool on = false); // вкл. верхноюю ось X
  void enableYLeft  (bool on = true);  // вкл. левую ось Y
  void enableYRight (bool on = false); // вкл. правую ось Y

  void clear(); // удалить все графики

  // добавить график
  QwtPlotCurve *addCurve(
    const double *xData,                  // указатель на массив X
    const double *yData,                  // указатель на массив Y
    int sizeData,                         // число точек (X, Y)
    const QString &title = QString(),     // имя графика
    const QPen &pen = QPen(Qt::black),    // цвет кривой
    int xAxis = QwtPlot::xBottom,         // ось X
    int yAxis = QwtPlot::yLeft,           // ось Y
    QwtPlotCurve::CurveStyle
          style = QwtPlotCurve::Lines,    // тип кривой
    QwtSymbol::Style
          symStyle = QwtSymbol::NoSymbol, // тип символов
    const QPen &symPen = QPen(Qt::black), // цвет символа
    const QBrush
          &symBrush = QBrush(Qt::gray),   // заливка символа
    int symSize = 5,                      // размер символа
    bool rawData = false);                // признак исп. Raw Data

  // удалить график
  void removeCurve(QwtPlotCurve *curve);

  void redraw(); // обновить (перерисовать c базовым zoom)

  void scrollX(double xStep); // прокрутка по X [+/- %]
  void scrollY(double yStep); // прокрутка по Y [+/- %]

  void center(); // текущее положение курсора в центр

  void getXY(double *xBottom, double *xTop,    // сообщить координаты курсора
             double *yLeft,   double *yRight); // по 4-м шкалам

  // экспорт в PNG/BMP
  void exportImg(QString fname = "qplot.png", bool dialog = true);

  // экспорт в SVG
  void exportSvg(QString fname = "qplot.svg", bool dialog = true);

  // экспорт в PDF
  void exportPdf(QString fname = "qplot.pdf", bool dialog = true);

  // печать (или экспорт в PDF/PS)
  void exportPrn(QString docName = "qplot");

signals:
  // координаты курсора по шкалам (по левой кнопке мыши)
  void clickOn(double xBottom, double xTop,
               double yLeft,   double yRight,
               Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);

  void keyOn(QKeyEvent *event); // on key pressed

  void zoomOn(bool on);         // on/off zoom
  void legendOn(bool on);       // on/off legend
  void gridOn(bool on);         // on/off grid
  void antialiasedOn(bool on);  // on/off antialiased


  // пользователь выполнил zoom
  void zoomOn(double xBottom, double wBottom,
               double yLeft,   double hLeft,
               double xTop,    double wTop,
               double yRight,  double hRight);

private Q_SLOTS:
  void zoomed(const QRectF &rect); // совершен zoom

protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  void keyPressEvent(QKeyEvent *event);

private:
  PlotAreaConf d_conf; // конфигурация

  PlotAreaTracker  *d_tracker;   // указатель координат курсора мыши
  PlotAreaZoomer   *d_zoomer[2]; // zoom'ы (0 - по левой шкале, 1 - по правой)
  QwtPlotPicker    *d_picker;    // сборщик (отображение координат курсора)
  QwtPlotPanner    *d_panner;    // "таскатель" в zoom'е
  QwtPlotMagnifier *d_magnifier; // лупа

  QwtLegend     *d_legend; // легенда
  QwtPlotGrid   *d_grid;   // масштабная сетка
  QwtPlotMarker *d_vLine;  // вертикальная линия
  QwtPlotMarker *d_hLine;  // горизонтальная линия
  QwtPlotMarker *d_marker; // маркер (жирная точка)

  QList<QwtPlotCurve*> d_crv; // список отображаемых кривых
};
//----------------------------------------------------------------------------
#endif // PLOT_AREA_H

/*** end of "plot_area.h" file ***/
