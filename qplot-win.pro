TARGET = qplot
TEMPLATE = app

CONFIG += silent
CONFIG += static
#CONFIG += debug

DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += __STDC_FORMAT_MACROS
DEFINES += QPLOT_WIN32

QT += svg

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += gui
  QT += widgets
  QT += printsupport
  QT += concurrent
  DEFINES += HAVE_QT5
}

#QWT=C:/Qwt-6.1.2
#QWT_LIB = $${QWT}/lib
#QWT_INC = $${QWT}/include
#LIBS += -L$${QWT_LIB} -lqwt

INCLUDEPATH += \
  $${QWT_INC} \
  src \
  qwt-src \
  clib

SOURCES += \
  clib/str.c \
  clib/ini.c \
  clib/fsize.c \
  src/plot_area.cpp \
  src/plot_win.cpp \
  src/qplot_tbl.cpp \
  src/qplot.cpp \
  src/main.cpp

HEADERS += \
  clib/str.h \
  clib/ini.h \
  clib/aini.h \
  clib/fsize.h \
  src/plot_area.h \
  src/plot_win.h \
  src/qplot_tbl.h \
  src/qplot.h

# QWT sources:
SOURCES += \
  qwt-src/qwt_abstract_legend.cpp \
  qwt-src/qwt_abstract_scale_draw.cpp \
  qwt-src/qwt_curve_fitter.cpp \
  qwt-src/qwt_clipper.cpp \
  qwt-src/qwt_color_map.cpp \
  qwt-src/qwt_dyngrid_layout.cpp \
  qwt-src/qwt_event_pattern.cpp \
  qwt-src/qwt_graphic.cpp \
  qwt-src/qwt_interval.cpp \
  qwt-src/qwt_legend.cpp \
  qwt-src/qwt_legend_data.cpp \
  qwt-src/qwt_legend_label.cpp \
  qwt-src/qwt_magnifier.cpp \
  qwt-src/qwt_math.cpp \
  qwt-src/qwt_null_paintdevice.cpp \
  qwt-src/qwt_painter_command.cpp \
  qwt-src/qwt_painter.cpp \
  qwt-src/qwt_panner.cpp \
  qwt-src/qwt_picker.cpp \
  qwt-src/qwt_picker_machine.cpp \
  qwt-src/qwt_plot_canvas.cpp \
  qwt-src/qwt_plot.cpp \
  qwt-src/qwt_plot_axis.cpp \
  qwt-src/qwt_plot_curve.cpp \
  qwt-src/qwt_plot_dict.cpp \
  qwt-src/qwt_plot_grid.cpp \
  qwt-src/qwt_plot_item.cpp \
  qwt-src/qwt_plot_layout.cpp \
  qwt-src/qwt_plot_legenditem.cpp \
  qwt-src/qwt_plot_magnifier.cpp \
  qwt-src/qwt_plot_marker.cpp \
  qwt-src/qwt_plot_panner.cpp \
  qwt-src/qwt_plot_picker.cpp \
  qwt-src/qwt_plot_seriesitem.cpp \
  qwt-src/qwt_plot_renderer.cpp \
  qwt-src/qwt_plot_zoomer.cpp \
  qwt-src/qwt_point_data.cpp \
  qwt-src/qwt_point_mapper.cpp \
  qwt-src/qwt_point_polar.cpp \
  qwt-src/qwt_point_3d.cpp \
  qwt-src/qwt_pixel_matrix.cpp \
  qwt-src/qwt_spline.cpp \
  qwt-src/qwt_scale_div.cpp \
  qwt-src/qwt_scale_draw.cpp \
  qwt-src/qwt_scale_engine.cpp \
  qwt-src/qwt_scale_map.cpp \
  qwt-src/qwt_scale_widget.cpp \
  qwt-src/qwt_series_data.cpp \
  qwt-src/qwt_symbol.cpp \
  qwt-src/qwt_text.cpp \
  qwt-src/qwt_text_engine.cpp \
  qwt-src/qwt_text_label.cpp \
  qwt-src/qwt_transform.cpp \
  qwt-src/qwt_widget_overlay.cpp

# QWT headers:
HEADERS += \
  qwt-src/qwt_abstract_legend.h \
  qwt-src/qwt_abstract_scale_draw.h \
  qwt-src/qwt_curve_fitter.h \
  qwt-src/qwt_clipper.h \
  qwt-src/qwt_color_map.h \
  qwt-src/qwt_dyngrid_layout.h \
  qwt-src/qwt_event_pattern.h \
  qwt-src/qwt_global.h \
  qwt-src/qwt_graphic.h \
  qwt-src/qwt_interval.h \
  qwt-src/qwt_legend_data.h \
  qwt-src/qwt_legend.h \
  qwt-src/qwt_legend_label.h \
  qwt-src/qwt_magnifier.h \
  qwt-src/qwt_math.h \
  qwt-src/qwt_null_paintdevice.h \
  qwt-src/qwt_painter_command.h \
  qwt-src/qwt_painter.h \
  qwt-src/qwt_panner.h \
  qwt-src/qwt_picker.h \
  qwt-src/qwt_plot_seriesitem.h \
  qwt-src/qwt_picker_machine.h \
  qwt-src/qwt_plot_canvas.h \
  qwt-src/qwt_plot_curve.h \
  qwt-src/qwt_plot_dict.h \
  qwt-src/qwt_plot_grid.h \
  qwt-src/qwt_plot.h \
  qwt-src/qwt_plot_item.h \
  qwt-src/qwt_plot_layout.h \
  qwt-src/qwt_plot_legenditem.h \
  qwt-src/qwt_plot_magnifier.h \
  qwt-src/qwt_plot_marker.h \
  qwt-src/qwt_plot_panner.h \
  qwt-src/qwt_plot_picker.h \
  qwt-src/qwt_plot_renderer.h \
  qwt-src/qwt_plot_zoomer.h \
  qwt-src/qwt_point_data.h \
  qwt-src/qwt_point_mapper.h \
  qwt-src/qwt_point_polar.h \
  qwt-src/qwt_point_3d.h \
  qwt-src/qwt_pixel_matrix.h \
  qwt-src/qwt_spline.h \
  qwt-src/qwt_scale_div.h \
  qwt-src/qwt_samples.h \
  qwt-src/qwt_scale_draw.h \
  qwt-src/qwt_scale_engine.h \
  qwt-src/qwt_scale_map.h \
  qwt-src/qwt_scale_widget.h \
  qwt-src/qwt_series_data.h \
  qwt-src/qwt_series_store.h \
  qwt-src/qwt_symbol.h \
  qwt-src/qwt_text_engine.h \
  qwt-src/qwt_text.h \
  qwt-src/qwt_text_label.h \
  qwt-src/qwt_transform.h \
  qwt-src/qwt_widget_overlay.h

FORMS += \
  ui/plot_win.ui

RESOURCES += 

#CONFIG(debug, debug|release) { DESTDIR = _bin_debug   }
#else                         { DESTDIR = _bin_release }
DESTDIR = .

OBJECTS_DIR = $${DESTDIR}/.build/obj
MOC_DIR     = $${DESTDIR}/.build/moc
RCC_DIR     = $${DESTDIR}/.build/rcc
UI_DIR      = $${DESTDIR}/.build/ui


