TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.c \
    audio.c \
    alsa-audio.c \
    trace.c \
    threadManager.c \
    spotifyManager.c \
    accountManager.c \
    playerManager.c \
    searchManager.c \
    playlistManager.c \
    networkCommand.c \
    serverManager.c \
    vlcManager.c


unix:!macx:!symbian: LIBS += -lspotify -lasound -lvlc

HEADERS += \
    queue.h \
    audio.h \
    types.h \
    trace.h \
    threadManager.h \
    spotifyManager.h \
    accountManager.h \
    playerManager.h \
    searchManager.h \
    playlistManager.h \
    networkCommand.h \
    serverManager.h \
    vlcManager.h

target.path = /home/pi/Development
INSTALLS += target
