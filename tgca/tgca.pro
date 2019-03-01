#-------------------------------------------------
#
# Project created by QtCreator 2018-07-09T19:45:57
#
#-------------------------------------------------

QT       += core gui uitools designer
CONFIG   += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network printsupport

TARGET = tgca
TEMPLATE = app

RC_FILE = tgca.rc

SOURCES += main.cpp\
        mainwindow.cpp \
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
    tests/trmsingletest.cpp \
    writereadreg.cpp \
    tests/monitortest.cpp \
    monitor.cpp \
    #crutches.cpp \
    tests/spipart.cpp \
    funclib.cpp \
    #data_structs.cpp
    tests/varcommtest.cpp \
    testutil.cpp \
    command.cpp \
    animatedspinnericon.cpp \
    tests/commontest.cpp \
    tests/nulltest.cpp \
    tests/ramtest.cpp \
    tests/trashtest.cpp \
    tests/noisetest.cpp \
    tests/varbroadtest.cpp

HEADERS  += mainwindow.h \
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
    tests/trmsingletest.h \
    tests/monitortest.h \
    monitor.h \
    #crutches.h \
    tests/spipart.h \
    funclib.h \
    data_structs.h \
    tests/varcommtest.h \
    testutil.h \
    command.h \
    picts.h \
    animatedspinnericon.h \
    tests/commontest.h \
    tests/nulltest.h \
    tests/ramtest.h \
    tests/trashtest.h \
    tests/noisetest.h \
    tests/varbroadtest.h

FORMS    += mainwindow.ui \
    configuration.ui \
    confselect.ui \
    connection.ui \
    device.ui \
    ../default/settings_bulb_test.ui \
    ../default/settings_echo_test.ui \
    ../default/settings_mem_test.ui \
    ../default/settings_reg_test.ui \
    ../default/settings_trm_single_test.ui \
    ../default/settings_monitor_test.ui \
    monitor.ui \
    ../default/settings_spi_part.ui \
    ../default/stats_mem_test.ui \
    ../default/stats_reg_test.ui \
    ../default/stats_echo_test.ui \
    ../default/stats_bulb_test.ui \
    ../default/stats_monitor_test.ui \
    ../default/stats_trm_single_test.ui \
    ../default/stats_spi_part.ui \
    ../default/settings_var_comm_test.ui \
    ../default/stats_var_comm_test.ui \
    ../default/settings_null_test.ui \
    ../default/stats_null_test.ui \
    ../default/settings_noise_test.ui \
    ../default/stats_noise_test.ui \
    ../default/settings_ram_test.ui \
    ../default/stats_ram_test.ui \
    ../default/settings_trash_test.ui \
    ../default/stats_trash_test.ui \
    ../default/settings_var_broad_test.ui \
    ../default/stats_var_broad_test.ui \
    ../default/codeselect.ui

RESOURCES += \
    tgca.qrc
