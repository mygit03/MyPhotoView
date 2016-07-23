#-------------------------------------------------
#
# Project created by QtCreator 2016-07-08T15:22:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyPhotoView
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    about.h

FORMS    += mainwindow.ui \
    about.ui

RESOURCES += \
    images.qrc

DISTFILES += \
    myApp.rc

RC_FILE += \
    myApp.rc
