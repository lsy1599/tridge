#-------------------------------------------------
#
# Project created by QtCreator 2017-01-19T04:29:38
#
#-------------------------------------------------

QT       += core gui
QT += sql
LIBS     += libwinspool
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tridge
TEMPLATE = app


SOURCES += main.cpp\
        tridge.cpp \
    View/mainview.cpp \
    View/localview.cpp \
    View/trackingsettingsview.cpp \
    printerscontroller.cpp \
    Module/printermonitor.cpp \
    Module/printers.cpp \
    Module/dbmonitor.cpp \
    settingsform.cpp \
    Module/printerclient.cpp

HEADERS  += tridge.h \
    View/mainview.h \
    View/localview.h \
    View/trackingsettingsview.h \
    printerscontroller.h \
    Module/printermonitor.h \
    Module/printers.h \
    Module/dbmonitor.h \
    settingsform.h \
    Module/printerclient.h

FORMS    += tridge.ui \
    View/mainview.ui \
    View/localview.ui \
    View/trackingsettingsview.ui \
    settingsform.ui
