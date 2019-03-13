#-------------------------------------------------
#
# Project created by QtCreator 2019-03-12T08:58:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtopengl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    camera.cpp \
    slices.cpp \
    meshmodel.cpp \
    mesh_info_widget.cpp \
    render.cpp

HEADERS  += mainwindow.h \
    camera.h \
    slices.h \
    objloader.h \
    meshmodel.h \
    mesh_info_widget.h \
    render.h
