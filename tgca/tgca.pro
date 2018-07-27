#-------------------------------------------------
#
# Project created by QtCreator 2018-07-09T19:45:57
#
#-------------------------------------------------

QT       += core gui uitools designer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = tgca
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    configuration.cpp \
    vskregview.cpp \
    device.cpp \
    connection.cpp \
    abstracttest.cpp \
    confselect.cpp

HEADERS  += mainwindow.h \
    abstracttest.h \
    configuration.h \
    connection.h \
    ctableeditcol.h \
    vskregview.h \
    extern_hsc.h \
    device.h \
    registers.h \
    confselect.h

FORMS    += mainwindow.ui \
    configuration.ui \
    vskregview.ui \
    device.ui \
    connection.ui \
    ../default/settings_mem_test.ui \
    ../default/settings_echo_test.ui \
    confselect.ui


RESOURCES += \
    tgca.qrc
