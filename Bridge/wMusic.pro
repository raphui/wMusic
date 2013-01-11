TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.c \
    audio/audio.c \
    audio/alsa-audio.c \
    file/fileManager.c \
    network/serverManager.c \
    network/networkCommand.c \
    spotify/spotifyManager.c \
    spotify/searchManager.c \
    spotify/playlistManager.c \
    spotify/playerManager.c \
    spotify/accountManager.c \
    utils/trace.c \
    utils/threadManager.c \
    vlc/vlcManager.c \
    system/environmentManager.c \
    spotify/playQueueManager.c \
    system/cli.c \
    utils/zmemory.c \
    network/multicastAddrManager.c \
    network/streamManager.c


unix:!macx:!symbian: LIBS += -lspotify -lasound -lvlc

HEADERS += \
    audio/audio.h \
    file/fileManager.h \
    network/serverManager.h \
    network/networkCommand.h \
    spotify/spotifyManager.h \
    spotify/searchManager.h \
    spotify/playlistManager.h \
    spotify/playerManager.h \
    spotify/accountManager.h \
    utils/types.h \
    utils/trace.h \
    utils/threadManager.h \
    vlc/vlcManager.h \
    system/environmentManager.h \
    spotify/playQueue.h \
    spotify/playQueueManager.h \
    system/cli.h \
    utils/zmemory.h \
    network/multicastAddrManager.h \
    network/streamManager.h

target.path = /home/pi/Development
INSTALLS += target

OTHER_FILES +=
