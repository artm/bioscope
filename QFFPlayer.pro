QT       += core gui

TARGET = QFFPlayer
TEMPLATE = app

SOURCES += main.cpp\
        PlayerShell.cpp

HEADERS  += PlayerShell.hpp

INCLUDEPATH += ../ffmpeg-0.8.5
LIBS += -L../ffmpeg-0.8.5-build/libavcodec -lavcodec \
        -L../ffmpeg-0.8.5-build/libavformat -lavformat  \
        -L../ffmpeg-0.8.5-build/libavutil -lavutil  \
        -L../ffmpeg-0.8.5-build/libswscale -lswscale

FORMS += \
    player.ui


