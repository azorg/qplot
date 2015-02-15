TARGET = mkdata
TEMPLATE = app

CONFIG += silent
#CONFIG += debug

#DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += __STDC_FORMAT_MACROS

QT +=
LIBS +=

INCLUDEPATH += \
  ../../clib

SOURCES += \
  ../../clib/str.c \
  ../../clib/ini.c \
  mkdata.cpp \

HEADERS += \
  ../../clib/str.h \
  ../../clib/ini.h \
  ../../clib/aini.h

FORMS +=
RESOURCES += 

#CONFIG(debug, debug|release) { DESTDIR = _bin_debug   }
#else                         { DESTDIR = _bin_release }
DESTDIR = .

OBJECTS_DIR = $${DESTDIR}/.build/obj
MOC_DIR     = $${DESTDIR}/.build/moc
RCC_DIR     = $${DESTDIR}/.build/rcc
UI_DIR      = $${DESTDIR}/.build/ui

