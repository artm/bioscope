QT       += core gui opengl
CONFIG   += uitools qtestlib precompile_header

INCLUDEPATH += ../ffmpeg-0.8.5 ../ffmpeg-0.8.5-debug
LIBS += \
    ../ffmpeg-0.8.5-debug/libavformat/libavformat.a \
    ../ffmpeg-0.8.5-debug/libavcodec/libavcodec.a \
    ../ffmpeg-0.8.5-debug/libavutil/libavutil.a \
    ../ffmpeg-0.8.5-debug/libswscale/libswscale.a
LIBS += -lm
win32:LIBS += -lpthreadGC2 -lpsapi
macx:LIBS += -Wl,-framework,Cocoa -lz

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
    BioscopeDriver.cpp \
    BioscopeThread.cpp

HEADERS  += PlayerShell.hpp \
    stable.h \
    Bioscope.hpp \
    BioscopeTestSuite.hpp \
    BioscopeDriver.hpp \
    BioscopeThread.hpp

QMAKE_LFLAGS = -w

FORMS += \
    player.ui

RESOURCES += \
    player.qrc

OTHER_FILES += \
    README.md

