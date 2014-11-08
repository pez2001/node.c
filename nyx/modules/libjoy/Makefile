LIBJOY = $(shell echo \\\#include \<libjoy.h\>\\n int main\(int c,char**v\){} | gcc -ljoy -xc -o a.out - 2>/dev/null ;echo $$? ;rm a.out 2>/dev/null ;rm a.exe 2>/dev/null)

ifneq ($(LIBJOY), 0)
	echo "Compilation dependancy not met -> install libjoy"
endif


USER := $(shell whoami)
UNAME := $(shell uname)
SYS := $(shell gcc -dumpmachine)

ifneq (, $(findstring linux, $(SYS)))
	PLATFORM_EXT = 
	PLATFORM_LIB_PREFIX = lib
	PLATFORM_LIB_EXT = .so
	PLATFORM_NAME = Linux
	PLATFORM_LIBS = -L../.. -lnyx -lm -ljoy
	#PLATFORM_LIBS = -L../../.. -L../.. -lnode -lnyx -lm -ljoy
	#PLATFORM_LIBS = ../../libnyx.a -lm -ljoy
	PLATFORM_CFLAGS = -fPIC -DPIC -DLINUX -D_XOPEN_SOURCE=700
	PLATFORM_DEBUG_CFLAGS = -fPIC -DPIC -DLINUX -D_XOPEN_SOURCE=700
	PLATFORM_LDFLAGS = 
else ifneq (, $(findstring mingw, $(SYS)))
	PLATFORM_NAME = Win32
	PLATFORM_EXT = .exe
	PLATFORM_LIB_PREFIX = 
	PLATFORM_LIB_EXT = .dll
	PLATFORM_LIBS = -lm -ljoy.dll
	PLATFORM_CFLAGS = -DWIN32 -DWINVER=0x501 -D_WIN32_WINNT=0x0501
	PLATFORM_DEBUG_CFLAGS = -DWIN32 -DWINVER=0x501 -D_WIN32_WINNT=0x0501
	PLATFORM_LDFLAGS = -static -static-libgcc

else ifneq (, $(findstring cygwin, $(SYS)))
 # Do cygwin things
else
 # Do things for others
endif


MAJOR_VERSION = 0
MINOR_VERSION = 2
BUILD = 722
DEBUG_BUILD = 478

CSTD = -std=c99
CFLAGS= -Wall -Wextra -W -w -Os $(CSTD) -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) $(PLATFORM_CFLAGS)
DEBUG_CFLAGS = -Wall -Wextra -Wformat=0 -pedantic -fbounds-check -Wuninitialized -O0 -g3 -Wall $(CSTD) -DUSE_DEBUGGING -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) -DDEBUG_BUILD=$(DEBUG_BUILD) $(PLATFORM_DEBUG_CFLAGS)
LDFLAGS= $(PLATFORM_LIBS) $(PLATFORM_LDFLAGS)
DEBUG_LDFLAGS = $(LDFLAGS)

CC=gcc
AR=ar -c
LD=ld

MOD_LJOY_FILES = nyxmod_libjoy.c
MOD_LJOY_OBJ = nyxmod_libjoy.o
MOD_LJOY_BINS = nyxmod_libjoy$(PLATFORM_LIB_EXT)
MOD_LJOY_DEBUG_OBJ = nyxmod_libjoy.do
MOD_LJOY_INCLUDE_FILES = nyxmod_libjoy.h

TOOLS_BUILD_INC_FILES = tools/build_inc/build_inc.c
TOOLS_BUILD_INC_BINS = build_inc$(PLATFORM_EXT)
TOOLS_BUILD_INC_INCLUDE_FILES = tools/build_inc/build_inc.h
TOOLS_BUILD_INC_OBJ = tools/build_inc/build_inc.o

all: library library_debug

debug: library_debug

module: library
	
library_debug: build_inc mod_ljoy_shared_debug
	@./build_inc$(PLATFORM_EXT) Makefile DEBUG_BUILD

library: build_inc mod_ljoy_shared
	@./build_inc$(PLATFORM_EXT) Makefile BUILD

build_inc: $(TOOLS_BUILD_INC_OBJ) 
	$(CC) $(TOOLS_BUILD_INC_OBJ) -o build_inc 

mod_ljoy_shared_debug: $(MOD_LJOY_DEBUG_OBJ)
	$(CC) -rdynamic -shared -nostdlib -DCREATELIB $(MOD_LJOY_DEBUG_OBJ) $(PLATFORM_LIBS) -o nyxmod_libjoy_debug$(PLATFORM_LIB_EXT)

mod_ljoy_shared: $(MOD_LJOY_OBJ)
	$(CC) -rdynamic -shared -nostdlib -DCREATELIB $(PLATFORM_LIBS) $(MOD_LJOY_OBJ) -o nyxmod_libjoy$(PLATFORM_LIB_EXT)
	@strip nyxmod_libjoy$(PLATFORM_LIB_EXT)	

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $< $(FAILED_NOTIFY)

%.do: %.c 
	$(CC) $(DEBUG_CFLAGS) -c -o $@ $< $(FAILED_NOTIFY)

clean: clean_binaries clean_debug
	rm -f nyxmod_libjoy.$(PLATFORM_LIB_EXT) *.elf *.do *.da *.o *.a *.so tools/build_inc/*.o 

clean_binaries:
	rm -f nyxmod_libjoy.$(PLATFORM_LIB_EXT) build_inc$(PLATFORM_EXT) 

clean_debug:
	rm -f nyxmod_libjoy_debug.$(PLATFORM_LIB_EXT) build_inc$(PLATFORM_EXT) *.do *.da 

	
indent:	
	indent -bap -bli0 -i4 -l79 -ncs -npcs -npsl -fca -lc79 -fc1 -ts4 *.c *.h
