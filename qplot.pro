TARGET = qplot
TEMPLATE = app

CONFIG += silent
#CONFIG += debug

#DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += __STDC_FORMAT_MACROS
#DEFINES += QPLOT_WIN32

QT += svg

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += gui
  QT += widgets
  QT += printsupport
  QT += concurrent
  DEFINES += HAVE_QT5
}

QWT = /usr/local/qwt-6.1.2
QWT_LIB = $${QWT}/lib
QWT_INC = $${QWT}/include

#QWT_LIB = /usr/lib
#QWT_INC = /usr/include/qwt

LIBS += -L$${QWT_LIB} -lqwt

INCLUDEPATH += \
  $${QWT_INC} \
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


