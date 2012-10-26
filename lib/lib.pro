! include( ../common.pri ) { error( Can't include common.pri ) }

TEMPLATE = lib
CONFIG += staticlib
TARGET += bioscope

SOURCES += \
    Bioscope.cpp \
    BioscopeDriver.cpp \
    BioscopeThread.cpp

HEADERS  += \
    Bioscope.hpp \
    BioscopeDriver.hpp \
    BioscopeThread.hpp
