/*
 * QPlot - 2d plotting tool based on Qwt
 * File: "qplot_tbl.h"
 */
//----------------------------------------------------------------------------
#ifndef QPLOT_TBL_H
#define QPLOT_TBL_H
//----------------------------------------------------------------------------
#include <qwt_plot_curve.h> // QwtPlotCurve
#include <qwt_symbol.h>     // QwtSymbol
//----------------------------------------------------------------------------
// ignore case string compare (std::string std, const char *cstr)
#define _STRCMP(std, cstr) str_icmp_cc(std.c_str(), cstr)
//----------------------------------------------------------------------------
typedef struct {
  const int id;     // 1...13
  const char *name; // "char", "int", "float", "double", ...
} qplot_type_name_t;
//----------------------------------------------------------------------------
typedef struct {
  const int id;   // 1...13
  const int size; // sizeof()
} qplot_type_size_t;
//----------------------------------------------------------------------------
typedef struct {
  const char *name; // none, lines, stick, steps, dots, ...
  QwtPlotCurve::CurveStyle style;
} qplot_qwt_curve_t;
//----------------------------------------------------------------------------
typedef struct {
  const char *name;   // Solid, Dash, Dot, DashDot, DashDotDot, ...
  Qt::PenStyle style; // Qt::SolidLine, Qt::DashLine, Qt::DotLine, ...
} qplot_pen_style_t;
//----------------------------------------------------------------------------
typedef struct {
  const char *name;       // no, cross, XCross...
  QwtSymbol::Style style; // QwtSymbol::NoSymbol, QwtSymbol::Cross, ...
} qplot_qwt_symbol_t;
//----------------------------------------------------------------------------
extern qplot_type_name_t  qplot_types_name[];  // types id by name table
extern qplot_type_size_t  qplot_types_size[];  // sizeof() by id table
extern qplot_qwt_curve_t  qplot_qwt_curves[];  // qwt curve style by name tab.
extern qplot_pen_style_t  qplot_pen_styles[];  // qt pen style by name table
extern qplot_qwt_symbol_t qplot_qwt_symbols[]; // qwt symbol by name table
//----------------------------------------------------------------------------
int qplot_id_by_type(std::string type); // get id by type name
std::string gplot_type_by_id(int id);   // get type name by id
int qplot_sizeof_by_id(int id);         // get sizeof() by type id
//----------------------------------------------------------------------------
// get qwt curve style by name
QwtPlotCurve::CurveStyle qplot_qwt_curve_by_name(std::string curve);
//----------------------------------------------------------------------------
// get qt pen style by name
Qt::PenStyle qplot_pen_by_name(std::string pen);
//----------------------------------------------------------------------------
// get qwt symbol style by name
QwtSymbol::Style qplot_qwt_symbol_by_name(std::string symbol);
//----------------------------------------------------------------------------
#endif // QPLOT_TBL_H

/*** end of "qplot_tbl.h" file ***/

