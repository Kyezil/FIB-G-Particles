HEADERS	+= *.h
SOURCES	+= *.cpp

TEMPLATE   = lib
CONFIG     += plugin 
CONFIG     -= debug
CONFIG     += release
CONFIG     += build_all
CONFIG     += opengl
#CONFIG     += silent
QT         += opengl
QT         += widgets

INCLUDEPATH += include
INCLUDEPATH += ../../viewer/glwidget/include
INCLUDEPATH += ../../viewer/interfaces
INCLUDEPATH += ../../viewer/core/include
QMAKE_CXXFLAGS += -std=c++11

DESTDIR = ../bin

MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

unix:target.path = ../bin/plugins
win32:target.path = ../bin/plugins

# GLEW
win32:INCLUDEPATH += E:/lib/glew-1.10.0/include/
win32:LIBS += -LE:/lib/glew-1.10.0/lib/Release/Win32
win32:LIBS += -lglew32
INSTALLS   += target

win32:LIBS += -L../../viewer/bin/ -lcore
win32:  LIBS += ../../viewer/bin/pluginglwidget.lib -L../bin/ -lpluginglwidget

macx: LIBS += -L../../viewer/bin/ -lcore
macx: LIBS += -L../bin/ -lglwidget






