TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.c \
    audio.c \
    alsa-audio.c \
    server.c


unix:!macx:!symbian: LIBS += -lspotify -lasound

HEADERS += \
    queue.h \
    audio.h \
    server.h \
    types.h

target.path = /home/pi/Development
INSTALLS += target
