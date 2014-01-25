#-------------------------------------------------
#
# Project created by QtCreator 2014-01-17T23:09:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

TARGET = TatsumakiDesktop
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serialsetup.cpp \
    protocol.cpp

HEADERS  += mainwindow.h \
    metadata.h \
    serialsetup.h \
    protocol.h

FORMS    += mainwindow.ui \
    serialsetup.ui
