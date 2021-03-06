#-------------------------------------------------
#
# Project created by QtCreator 2016-05-03T12:37:29
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT       += sql

CONFIG += c++11

TARGET = SportDiary
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    userselectiondialogwindow.cpp \
    userinfowidget.cpp \
    user.cpp \
    treemodel.cpp \
    treeitem.cpp \
    states.cpp \
    tablerecordeditwidget.cpp \
    addnewuserwidget.cpp \
    imagemodel.cpp \
    mysqlexecutor.cpp \
    filemanager.cpp \
    pathsoffiles.cpp \
    exercisecatalogwindow.cpp \
    exerciseinfowindow.cpp \
    exerciseinfo.cpp

HEADERS  += mainwindow.h \
    userselectiondialogwindow.h \
    userinfowidget.h \
    user.h \
    treemodel.h \
    treeitem.h \
    states.h \
    tablerecordeditwidget.h \
    addnewuserwidget.h \
    imagemodel.h \
    mysqlexecutor.h \
    filemanager.h \
    pathsoffiles.h \
    exercisecatalogwindow.h \
    exerciseinfowindow.h \
    exerciseinfo.h

FORMS    += mainwindow.ui \
    userselectiondialogwindow.ui \
    addnewuserwidget.ui \
    exercisecatalogwindow.ui \
    exerciseinfowindow.ui

RESOURCES += \
    src.qrc
