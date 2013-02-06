TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.c \
    utils/trace.c \
    vlc/vlcManager.c \
    utils/zmemory.c \
    utils/threadManager.c \
    system/cli.c \
    network/streamManager.c \
    network/serverManager.c \
    network/networkCommand.c

unix:!macx:!symbian: LIBS += -lvlc -lpthread

target.path = /home/pi/Development
INSTALLS += target

HEADERS += \
    utils/types.h \
    utils/trace.h \
    vlc/vlcManager.h \
    utils/zmemory.h \
    utils/threadManager.h \
    system/cli.h \
    network/streamManager.h \
    network/serverManager.h \
    network/networkCommand.h
