QT       += core gui opengl
CONFIG   += uitools qtestlib precompile_header

INCLUDEPATH += .. ../lib ../../ffmpeg-0.8.5 ../../ffmpeg-0.8.5-debug

# this is somehow necessary for ffmpeg libraries
DEFINES += __STDC_CONSTANT_MACROS
QMAKE_CXXFLAGS +=  -Wno-deprecated -Wno-deprecated-declarations -Wno-attributes

QMAKE_LFLAGS = -w

HEADERS += ../stable.h
PRECOMPILED_HEADER = ../stable.h
