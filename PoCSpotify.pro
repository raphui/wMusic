TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.c


unix:!macx:!symbian: LIBS += -lspotify
