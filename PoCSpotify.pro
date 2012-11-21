TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.c \
    audio.c \
    alsa-audio.c


unix:!macx:!symbian: LIBS += -lspotify -lasound

HEADERS += \
    queue.h \
    audio.h
