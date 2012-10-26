! include( ../common.pri ) { error( Can't include common.pri ) }

TARGET = ../demo

SOURCES += \
    main.cpp \
    BioscopeTestSuite.cpp \
    PlayerShell.cpp

HEADERS  += \
    PlayerShell.hpp \
    BioscopeTestSuite.hpp

FORMS += \
    player.ui

RESOURCES += \
    player.qrc

LIBS += \
    ../../ffmpeg-0.8.5-debug/libavformat/libavformat.a \
    ../../ffmpeg-0.8.5-debug/libavcodec/libavcodec.a \
    ../../ffmpeg-0.8.5-debug/libavutil/libavutil.a \
    ../../ffmpeg-0.8.5-debug/libswscale/libswscale.a
LIBS += -lm
win32:LIBS += -lpthreadGC2 -lpsapi
macx:LIBS += -Wl,-framework,Cocoa -lz

LIBS += -L../bioscope -lbioscope
