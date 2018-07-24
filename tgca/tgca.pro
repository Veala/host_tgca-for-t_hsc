#-------------------------------------------------
#
# Project created by QtCreator 2018-07-09T19:45:57
#
#-------------------------------------------------

QT       += core gui uitools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = tgca
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    configuration.cpp \
    vskregview.cpp \
    device.cpp \
    connection.cpp \
    abstracttest.cpp

HEADERS  += mainwindow.h \
    abstracttest.h \
    configuration.h \
    connection.h \
    ctableeditcol.h \
    vskregview.h \
    extern_hsc.h \
    device.h \
    registers.h

FORMS    += mainwindow.ui \
    configuration.ui \
    vskregview.ui \
    device.ui \
    connection.ui \
    settingsregtest.ui

RESOURCES += \
    tgca.qrc
