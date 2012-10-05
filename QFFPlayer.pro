QT       += core gui
CONFIG   += link_pkgconfig uitools qtestlib precompile_header

PKGCONFIG += libavformat libavcodec libavutil libswscale
# this is somehow necessary for ffmpeg libraries
DEFINES += __STDC_CONSTANT_MACROS
QMAKE_CXXFLAGS +=  -Wno-deprecated -Wno-deprecated-declarations -Wno-attributes

TARGET = QFFPlayer
TEMPLATE = app
PRECOMPILED_HEADER = stable.h

SOURCES += main.cpp\
    PlayerShell.cpp \
    Bioscope.cpp \
    BioscopeTestSuite.cpp

HEADERS  += PlayerShell.hpp \
    stable.h \
    Bioscope.hpp \
    BioscopeTestSuite.hpp

QMAKE_LFLAGS = -w

FORMS += \
    player.ui

RESOURCES += \
    player.qrc
