TARGET := wMusic-Bridge
TARGET_X86 := wMusic-Bridge-x86

PROJECT_HOME := /home/raphio/Development/wMusic/Bridge

INCLUDES_SRC := -I$(PROJECT_HOME)

INCLUDES_LIBS_HEADERS := -I/usr/local/include/
INCLUDES_LIBS_HEADERS_X86 := -I/usr/local/include/ -I/usr/include/
INCLUDES_HEADERS := -I/opt/cross/x-tools/arm-unknown-linux-gnueabi/arm-unknown-linux-gnueabi/sysroot/usr/include

CC := /opt/cross/x-tools/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-gcc
CC_X86 := gcc

DEBUGFLAGS_C-Compiler := -g -O0 -fno-omit-frame-pointer -pipe -Wall

LIBS := -L/home/raphio/Development/wMusic/ARMLibs -lvlc -lspotify -lasound -lpthread-2.13 -lvlccore -ldbus-1 -lrt-2.13
LIBS_X86 := -lvlc -lspotify -lasound -lpthread

SRC := $(wildcard */*.c)
SRC := $(SRC) $(wildcard *.c)
EXCLUDES := $(shell ls shairport/*.c)
SRC := $(filter-out $(EXCLUDES) , $(SRC) )
OBJ := $(SRC:.c=.o)


#ifeq( $@ , arm )
%.o: %.c
	$(CC) $(DEBUGFLAGS_C-Compiler) $(INCLUDES_SRC) $(INCLUDES_LIBS_HEADERS) -c -fmessage-length=0 -o $@ $<
#else
#%.o: %.c
#	$(CC_X86) $(DEBUGFLAGS_C-Compiler) $(INCLUDES_SRC) $(INCLUDES_LIBS_HEADERS) -c -fmessage-length=0 -o $@ $<
#endif

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

arm: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

x86: $(OBJ)
	$(CC_X86) $(OBJ) -o $(TARGET) $(LIBS)

clean:
	rm -f $(OBJFILES) rm -f $(TARGET)

