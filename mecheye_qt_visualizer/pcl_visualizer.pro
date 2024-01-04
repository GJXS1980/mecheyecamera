#-------------------------------------------------
#
# Project created by QtCreator 2014-05-01T14:24:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pcl_visualizer
TEMPLATE = app

# Specify the VTK module
CONFIG += vtk

# Specify include paths for VTK headers
INCLUDEPATH += /usr/local/vtk-8.2/include/vtk-8.2 \
               /usr/local/pcl-1.14/include/pcl-1.13

# Specify library paths for PCL libraries
LIBS += -L/usr/local/pcl-1.14/lib
        # Add other PCL libraries as needed


# Specify library paths for VTK libraries
LIBS += -L/usr/local/vtk-8.2/lib

SOURCES += main.cpp\
        pclviewer.cpp

HEADERS  += pclviewer.h

FORMS    += pclviewer.ui
