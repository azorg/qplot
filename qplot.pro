TARGET = qplot
TEMPLATE = app

CONFIG += silent
CONFIG += debug

#DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += __STDC_FORMAT_MACROS

QT += svg

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
  QT += printsupport
  DEFINES += HAVE_QT5
}

#QWT = /usr/local/qwt-svn
QWT = /usr/local/qwt

LIBS += -L$${QWT}/lib -lqwt

INCLUDEPATH += \
  $${QWT}/include \
  src \
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

