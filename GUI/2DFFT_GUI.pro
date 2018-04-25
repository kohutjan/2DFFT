#-------------------------------------------------
#
# Project created by QtCreator 2018-04-24T15:18:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2DFFT_GUI
TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc

SOURCES += main.cpp \
           mainwindow.cpp \
           ../src/filter_loader.cpp \
           ../src/frequency_convolution.cpp \
           ../src/run.cpp \
           ../src/spatial_convolution.cpp

HEADERS  += mainwindow.hpp \
            ../include/convolution.hpp \
            ../include/filter.hpp \
            ../include/filter_loader.hpp \
            ../include/filter_statistic.hpp \
            ../include/frequency_convolution.hpp \
            ../include/run.hpp \
            ../include/spatial_convolution.hpp

FORMS    += mainwindow.ui

CONFIG += c++11


