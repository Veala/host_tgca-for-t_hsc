#-------------------------------------------------
#
# Project created by QtCreator 2018-07-09T19:45:57
#
#-------------------------------------------------

QT       += core gui uitools designer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network printsupport

TARGET = tgca
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    command.cpp \
    configuration.cpp \
    confselect.cpp \
    connection.cpp \
    ctableeditcol.cpp \
    ctestbc.cpp \
    device.cpp \
    gendata.cpp \
    registers.cpp \
    tests/abstracttest.cpp \
    tests/bulbtest.cpp \
    tests/echotest.cpp \
    tests/memtest.cpp \
    tests/regtest.cpp \
    tests/trmsingletest.cpp

HEADERS  += mainwindow.h \
    command.h \
    configuration.h \
    confselect.h \
    connection.h \
    ctableeditcol.h \
    ctestbc.h \
    device.h \
    extern_hsc.h \
    gendata.h \
    registers.h \
    tests/abstracttest.h \
    tests/bulbtest.h \
    tests/echotest.h \
    tests/memtest.h \
    tests/regtest.h \
    tests/trmsingletest.h

FORMS    += mainwindow.ui \
    configuration.ui \
    confselect.ui \
    connection.ui \
    device.ui \
    ../default/settings_bulb_test.ui \
    ../default/settings_echo_test.ui \
    ../default/settings_mem_test.ui \
    ../default/settings_trm_single_test.ui

RESOURCES += \
    tgca.qrc
