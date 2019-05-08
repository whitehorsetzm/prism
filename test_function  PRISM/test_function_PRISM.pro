TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    setupCellNeig.cpp \
    findicellfast.cpp \
    partion.cpp \
    constructfacets.cpp \
    updatetriindex.cpp \
    read_cgns.cpp

HEADERS += \
    struts.h
