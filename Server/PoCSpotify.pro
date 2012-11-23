TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.c \
    audio.c \
    alsa-audio.c \
    server.c \
    trace.c \
    threadManager.c \
    spotifyManager.c


unix:!macx:!symbian: LIBS += -lspotify -lasound

HEADERS += \
    queue.h \
    audio.h \
    server.h \
    types.h \
    trace.h \
    threadManager.h \
    spotifyManager.h

target.path = /home/pi/Development
INSTALLS += target
