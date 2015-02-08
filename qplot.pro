TARGET = qplot
TEMPLATE = app
QT += svg
CONFIG += silent

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
  src/main.cpp

HEADERS += \
  src/plot_area.h \
  src/plot_win.h

FORMS += \
  ui/plot_win.ui

RESOURCES += 

OBJECTS_DIR = .build/obj
MOC_DIR     = .build/moc
RCC_DIR     = .build/rcc
UI_DIR      = .build/ui

