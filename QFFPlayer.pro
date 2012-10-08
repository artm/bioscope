QT       += core gui
CONFIG   += uitools qtestlib precompile_header

INCLUDEPATH += ../ffmpeg-0.8.5
LIBS += ../ffmpeg-0.8.5-debug/libavcodec/libavcodec.a \
        ../ffmpeg-0.8.5-debug/libavformat/libavformat.a \
        ../ffmpeg-0.8.5-debug/libavutil/libavutil.a \
        ../ffmpeg-0.8.5-debug/libswscale/libswscale.a
LIBS += -Wl,-framework,Cocoa -lm -lz

# this is somehow necessary for ffmpeg libraries
DEFINES += __STDC_CONSTANT_MACROS
QMAKE_CXXFLAGS +=  -Wno-deprecated -Wno-deprecated-declarations -Wno-attributes

TARGET = QFFPlayer
TEMPLATE = app
PRECOMPILED_HEADER = stable.h

SOURCES += main.cpp\
    PlayerShell.cpp \
    Bioscope.cpp \
    BioscopeTestSuite.cpp \
    BioscopeDriver.cpp

HEADERS  += PlayerShell.hpp \
    stable.h \
    Bioscope.hpp \
    BioscopeTestSuite.hpp \
    BioscopeDriver.hpp

QMAKE_LFLAGS = -w

FORMS += \
    player.ui

RESOURCES += \
    player.qrc


