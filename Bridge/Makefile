TARGET_NAME := wMusic-Bridge
PROJECT_HOME := /home/raphio/Development/SpotifyEverywhere/Bridge
INCLUDES_SRC := -I$(PROJECT_HOME)
INCLUDES_LIBS_HEADERS := -I/usr/local/include/
INCLUDES_HEADERS := -I/opt/cross/x-tools/arm-unknown-linux-gnueabi/arm-unknown-linux-gnueabi/sysroot/usr/include
CC := /opt/cross/x-tools/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-g++
GCC := /opt/cross/x-tools/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-gcc
DEBUGFLAGS_C-Compiler := -g -O0 -fno-omit-frame-pointer -pipe -Wall
LIBS := -L/home/raphio/Development/SpotifyEverywhere/ARMLibs -lvlc -lspotify -lasound -lpthread-2.13 -lvlccore -ldbus-1
OBJFILES := $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.c,%.o,$(wildcard src/*.c))
SRC := 	audio/audio.c \
	audio/alsa-audio.c \
	file/fileManager.c \
	network/networkCommand.c \
	network/serverManager.c \
	network/streamManager.c \
	spotify/accountManager.c \
	spotify/playlistManager.c \
	spotify/playerManager.c \
	spotify/searchManager.c \
	spotify/playQueueManager.c \
	spotify/spotifyManager.c \
	system/cli.c \
	system/environmentManager.c \
	utils/threadManager.c \
	utils/trace.c \
	utils/zmemory.c \
	vlc/vlcManager.c

%.o: %.c
	$(GCC) -o $(SRC) $(DEBUGFLAGS_C-Compiler) $(INCLUDES_SRC) $(INCLUDES_LIBS_HEADERS) $(INCLUDES_HEADERS) $(LIBS)

all: $(OBJFILES)
	$(GCC) -o $(TARGET) $(OBJFILES) $(LIBS)

clean:
	rm -f $(OBJFILES) rm -f $(TARGET)

