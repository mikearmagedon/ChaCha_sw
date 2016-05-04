#QT += core
#QT -= gui

TARGET = Case_Study
CONFIG += console
#CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    pthrd.cpp \
    producer.cpp \
    consumer.cpp \
    chacha20.cpp \
    chacha20block.cpp \
    processing.cpp

HEADERS += \
    pthrd.h \
    producer.h \
    consumer.h \
    chacha20.h \
    chacha20block.h \
    processing.h \
    defines.h



LIBS += -pthread

LIBS += -lrt



