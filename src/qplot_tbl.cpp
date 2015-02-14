/*
 * QPlot - 2d plotting tool based on Qwt
 * File: "qplot_tbl.cpp"
 */
//----------------------------------------------------------------------------
#include "qplot_tbl.h"
#include "str.h"
//----------------------------------------------------------------------------
// type id by name table
qplot_type_name_t qplot_types_name[] = {
//{ int id, const char *name     }
  {  1, "char"                   },
  {  2, "unsigned char"          },
  {  3, "short"                  },
  {  3, "short int"              },
  {  4, "unsigned short"         },
  {  4, "unsigned short int"     },
  {  5, "int"                    },
  {  6, "unsigned"               },
  {  6, "unsigned int"           },
  {  7, "long"                   },
  {  7, "long int"               },
  {  8, "unsigned long"          },
  {  9, "unsigned long int"      },
  {  9, "long long"              },
  {  9, "long long int"          },
  { 10, "unsigned long long"     },
  { 10, "unsigned long long int" },
  { 11, "float"                  },
  { 12, "double"                 },
  { 13, "long double"            },
  {  0, ""                       }
};
//----------------------------------------------------------------------------
// get id by type name
int qplot_id_by_type(std::string type)
{
  qplot_type_name_t *p = qplot_types_name;
  while (p->id)
  {
    if (_STRCMP(type, p->name) == 0)
      return p->id;
    p++;
  }
  return 11; // default type is float
}
//----------------------------------------------------------------------------
// get type name by id
std::string gplot_type_by_id(int id)
{
  qplot_type_name_t *p = qplot_types_name;
  while (p->id)
  {
    if (id == p->id)
      return std::string(p->name);
    p++;
  }
  return std::string("float"); // default type is float
}
//----------------------------------------------------------------------------
// sizeof() by id table
qplot_type_size_t qplot_types_size[] = {
//{ int id, int size                   }
  {  1, sizeof(char)                   },
  {  2, sizeof(unsigned char)          },
  {  3, sizeof(short)                  },
  {  4, sizeof(unsigned short)         },
  {  5, sizeof(int)                    },
  {  6, sizeof(unsigned)               },
  {  7, sizeof(long)                   },
  {  8, sizeof(unsigned long)          },
  {  9, sizeof(unsigned long int)      },
  { 10, sizeof(unsigned long long)     },
  { 11, sizeof(float)                  },
  { 12, sizeof(double)                 },
  { 13, sizeof(long double)            },
  {  0, 0                              },
};
//----------------------------------------------------------------------------
// get sizeof() by type id
int qplot_sizeof_by_id(int id)
{
  qplot_type_size_t *p = qplot_types_size;
  while (p->id)
  {
    if (id == p->id)
      return p->size;
    p++;
  }
  return sizeof(float); // default type is float
}
//----------------------------------------------------------------------------
// qwt curve style by name table
qplot_qwt_curve_t  qplot_qwt_curves[] = {
//{ const char *name, QwtPLotCurve::CurveStyle style }
  { "No",      QwtPlotCurve::NoCurve },
  { "None",    QwtPlotCurve::NoCurve },
  { "Off",     QwtPlotCurve::NoCurve },
  { "NoCurve", QwtPlotCurve::NoCurve },
  { "Lines",   QwtPlotCurve::Lines   },
  { "On",      QwtPlotCurve::Lines   }, // default curve if "On"
  { "Sticks",  QwtPlotCurve::Sticks  },
  { "Steps",   QwtPlotCurve::Steps   },
  { "Dots",    QwtPlotCurve::Dots    },
  { "",        QwtPlotCurve::Lines   }
};
//----------------------------------------------------------------------------
// get qwt curve style by name
QwtPlotCurve::CurveStyle qplot_qwt_curve_by_name(std::string curve)
{
  qplot_qwt_curve_t *p = qplot_qwt_curves;
  while (strlen(p->name))
  {
    if (_STRCMP(curve, p->name) == 0)
      return p->style;
    p++;
  }
  return p->style; // QwtPLotCurve::Lines by default
}
//----------------------------------------------------------------------------
// qt pen style by name table
qplot_pen_style_t qplot_pen_styles[] = {
//{ const char *name, Qt::PenStyle style }
  { "Solid",      Qt::SolidLine          },
  { "Dash",       Qt::DashLine           },
  { "Dot",        Qt::DotLine            },
  { "DashDot",    Qt::DashDotLine        },
  { "DashDotDot", Qt::DashDotDotLine     },
  { "",           Qt::SolidLine          }
};
//----------------------------------------------------------------------------
// get qt pen style by name
Qt::PenStyle qplot_pen_by_name(std::string pen)
{
  qplot_pen_style_t *p = qplot_pen_styles;
  while (strlen(p->name))
  {
    if (_STRCMP(pen, p->name) == 0)
      return p->style;
    p++;
  }
  return p->style; // Qt::SolidLine by default
}
//----------------------------------------------------------------------------
// qwt symbol by name table
qplot_qwt_symbol_t qplot_qwt_symbols[] = {
//{ const char *name, QwtSymbol::Style style }
  { "No",        QwtSymbol::NoSymbol  },
  { "None",      QwtSymbol::NoSymbol  },
  { "Off",       QwtSymbol::NoSymbol  },
  { "Ellipse",   QwtSymbol::Ellipse   },
  { "Rect",      QwtSymbol::Rect      },
  { "Diamond",   QwtSymbol::Diamond   },
  { "Triangle",  QwtSymbol::Triangle  },
  { "DTriangle", QwtSymbol::DTriangle },
  { "UTriangle", QwtSymbol::UTriangle },
  { "LTriangle", QwtSymbol::LTriangle },
  { "RTriangle", QwtSymbol::RTriangle },
  { "Cross",     QwtSymbol::Cross     },
  { "XCross",    QwtSymbol::XCross    },
  { "On",        QwtSymbol::XCross    }, // default symbol if 'on'
  { "HLine",     QwtSymbol::HLine     },
  { "VLine",     QwtSymbol::VLine     },
  { "Star1",     QwtSymbol::Star1     },
  { "Star2",     QwtSymbol::Star2     },
  { "Hexagon",   QwtSymbol::Hexagon   },
  { "Path",      QwtSymbol::Path      },
  { "",          QwtSymbol::NoSymbol  }
};
//----------------------------------------------------------------------------
// get qwt symbol style by name
QwtSymbol::Style qplot_qwt_symbol_by_name(std::string symbol)
{
  qplot_qwt_symbol_t *p = qplot_qwt_symbols;
  while (strlen(p->name))
  {
    if (_STRCMP(symbol, p->name) == 0)
      return p->style;
    p++;
  }
  return p->style; // QwtSymbol::NoSymbol by default
}
//----------------------------------------------------------------------------
/*** end of "qplot_tbl.cpp" file ***/

