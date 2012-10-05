QT       += core gui
CONFIG   += precompile_header link_pkgconfig uitools qtestlib

PKGCONFIG += libavformat libavcodec libavutil libswscale

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


FORMS += \
    player.ui

RESOURCES += \
    player.qrc
