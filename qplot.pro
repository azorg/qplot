TARGET = qplot
TEMPLATE = app

CONFIG += silent
CONFIG += debug

QT += svg

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
  QT += printsupport
  DEFINES += HAVE_QT5
}

DEFINES += __STDC_FORMAT_MACROS

#QWT = /usr/local/qwt-svn
QWT = /usr/local/qwt

LIBS += -L$${QWT}/lib -lqwt

INCLUDEPATH += \
  $${QWT}/include \
  src \
  libs \
  libs/clib

SOURCES += \
  libs/clib/str.c \
  libs/clib/ini.c \
  src/plot_area.cpp \
  src/plot_win.cpp \
  src/core.cpp \
  src/main.cpp

HEADERS += \
  src/plot_area.h \
  src/plot_win.h \
  src/core.h

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

