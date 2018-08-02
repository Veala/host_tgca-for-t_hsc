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
    tests/abstracttest.cpp \
    configuration.cpp \
    confselect.cpp \
    connection.cpp \
    ctableeditcol.cpp \
    device.cpp \
    vskregview.cpp \
    tests/memtest.cpp \
    tests/regtest.cpp \
    tests/echotest.cpp

HEADERS  += mainwindow.h \
    tests/abstracttest.h \
    configuration.h \
    confselect.h \
    connection.h \
    ctableeditcol.h \
    device.h \
    extern_hsc.h \
    registers.h \
    vskregview.h

FORMS    += mainwindow.ui \
    configuration.ui \
    confselect.ui \
    connection.ui \
    device.ui \
    vskregview.ui \
    ../default/settings_echo_test.ui \
    ../default/settings_mem_test.ui

RESOURCES += \
    tgca.qrc
