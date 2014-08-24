
USER := $(shell whoami)
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
PLATFORM_EXT = 
PLATFORM_NAME = Linux
PLATFORM_LIBS = -lm -lcurl.dll
PLATFORM_CFLAGS =
PLATFORM_DEBUG_CFLAGS = -DUSE_MEMORY_DEBUGGING
endif
ifeq ($(UNAME), MINGW32_NT-6.2)
PLATFORM_NAME = Win32
PLATFORM_EXT = .exe
PLATFORM_LIBS = -lwsock32 -lws2_32 -lm -lcurl.dll -lmicrohttpd.dll -lwebsockets.dll
PLATFORM_CFLAGS = -DWIN32 -DWINVER=0x501 -D_WIN32_WINNT=0x0501
PLATFORM_DEBUG_CFLAGS = -DWIN32 
#-DUSE_MEMORY_DEBUGGING
else
PLATFORM_NAME = Win32
PLATFORM_EXT = 
PLATFORM_LIBS = -lwsock32 -lws2_32 -lm -lcurl.dll
PLATFORM_CFLAGS = -DWIN32 -DWINVER=0x501 -D_WIN32_WINNT=0x0501
PLATFORM_DEBUG_CFLAGS = -DWIN32 -DUSE_MEMORY_DEBUGGING
endif

#.PHONY: print_version

#BEGIN_NOTIFY =	@playsound.exe /c/Users/pez2001/Downloads/217656__reitanna__knuckles-cracking.wav
#FAILED_NOTIFY = || @playsound.exe /c/Users/pez2001/Downloads/123921__silencer1337__machinefail.wav
#SUCCESS_NOTIFY = @playsound.exe /c/Users/pez2001/Downloads/187404__mazk1985__robot-ready.wav

BEGIN_NOTIFY = 
FAILED_NOTIFY = 
SUCCESS_NOTIFY = 


MAJOR_VERSION = 0
MINOR_VERSION = 4
BUILD = 3395
DEBUG_BUILD = 3735

CSTD = c99

#-DUSE_MEMORY_DEBUGGING
#CFLAGS= -W -w -Os -std=$(CSTD) -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) $(PLATFORM_CFLAGS) -lm
#DEBUG_CFLAGS = -DUSE_MEMORY_DEBUGGING -m32 -g3 -O0 -Wall -pedantic -Wstrict-prototypes -std=$(CSTD) -fbounds-check -Wuninitialized -DUSE_DEBUGGING -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) -DDEBUG_BUILD=$(DEBUG_BUILD) -lm
CFLAGS= -W -w -Os -std=$(CSTD) -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) $(PLATFORM_CFLAGS)
DEBUG_CFLAGS = -m32 -g3 -O0 -Wall -pedantic -Wstrict-prototypes -std=$(CSTD) -fbounds-check -Wuninitialized -DUSE_DEBUGGING -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) -DDEBUG_BUILD=$(DEBUG_BUILD) $(PLATFORM_DEBUG_CFLAGS)
#DEBUG_CFLAGS = -m32 -g3 -O0 -Wall -pedantic -Wstrict-prototypes -std=$(CSTD) -fbounds-check -Wuninitialized -DUSE_DEBUGGING -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) -DDEBUG_BUILD=$(DEBUG_BUILD) $(PLATFORM_DEBUG_CFLAGS)
LDFLAGS= $(PLATFORM_LIBS) -static -static-libgcc -static-libstdc++
DEBUG_LDFLAGS = $(LDFLAGS)

CC=gcc
AR=ar
LD=ld

NODE_FILES = list.c node.c memory.c strings.c imports/json.c imports/fbx.c imports/nyx.c
NODE_BINS = node_c.a
NODE_INCLUDE_FILES = node.h list.h memory.h strings.h imports/json.h imports/fbx.h imports/nyx.h
NODE_OBJ = node.o list.o memory.o strings.o imports/json.o imports/fbx.o imports/nyx.o
NODE_DEBUG_OBJ = node.do list.do memory.do strings.do imports/json.do imports/fbx.do imports/nyx.do

UT_FILES = unit_tests.c
UT_OBJ = unit_tests.o
UT_BINS = unit_tests$(PLATFORM_EXT)
UT_DEBUG_OBJ = unit_tests.do
UT_INCLUDE_FILES = unit_tests.h

NYXI_FILES = nyxi.c
NYXI_OBJ = nyxi.o
NYXI_BINS = nyxi$(PLATFORM_EXT)
NYXI_DEBUG_OBJ = nyxi.do
NYXI_INCLUDE_FILES = nyxi.h

TOOLS_BUILD_INC_FILES = tools/build_inc/build_inc.c
TOOLS_BUILD_INC_BINS = build_inc$(PLATFORM_EXT)
TOOLS_BUILD_INC_INCLUDE_FILES = tools/build_inc/build_inc.h
TOOLS_BUILD_INC_OBJ = tools/build_inc/build_inc.o

TOOLS_STARTER_FILES = tools/starter/starter.c strings.c
TOOLS_STARTER_BINS = starter$(PLATFORM_EXT)
TOOLS_STARTER_INCLUDE_FILES = tools/starter/starter.h strings.h
TOOLS_STARTER_OBJ = tools/starter/starter.o strings.o


print_version:
	@echo -n "$(BUILD)"

test: all
	./unit_tests$(PLATFORM_EXT)

test_debug: debug
	./unit_tests_debug$(PLATFORM_EXT)
	
debug: build_inc node_static_debug libnyx_debug unit_tests_debug nyxi_debug
	./build_inc$(PLATFORM_EXT) Makefile DEBUG_BUILD

clean_all: clean libnyx_clean clean_debug clean_binaries all debug 
	@echo "Compiling for "$(PLATFORM_NAME)
	./build_inc$(PLATFORM_EXT) Makefile BUILD
	$(SUCCESS_NOTIFY)

all: node_static node_dynamic build_inc starter unit_tests debug libnyx nyxi
	@echo "Compiling for "$(PLATFORM_NAME)
	./build_inc$(PLATFORM_EXT) Makefile BUILD
	$(SUCCESS_NOTIFY)

.PHONY: libnyx
.PHONY: libnyx_clean
.PHONY: libnyx_debug
	
libnyx: node_static
	$(MAKE) -C nyx all

libnyx_clean: 
	$(MAKE) -C nyx clean

libnyx_debug: node_static_debug
	$(MAKE) -C nyx debug

unit_tests: node_static $(UT_OBJ) 
	$(CC) $(UT_OBJ) libnode.a  $(LDFLAGS) -o unit_tests 
	strip ./unit_tests$(PLATFORM_EXT)

unit_tests_debug: node_static_debug $(UT_DEBUG_OBJ) 
	$(CC) $(UT_DEBUG_OBJ) libnode.da $(DEBUG_LDFLAGS) -o unit_tests_debug 

nyxi: node_static libnyx $(NYXI_OBJ) 
	$(CC) $(NYXI_OBJ) nyx/libnyx.a libnode.a $(LDFLAGS) -o nyxi 
	strip ./nyxi$(PLATFORM_EXT)

nyxi_debug: node_static_debug libnyx $(NYXI_DEBUG_OBJ) 
	$(CC) $(NYXI_DEBUG_OBJ) nyx/libnyx.da libnode.da $(DEBUG_LDFLAGS) -o nyxi_debug 

build_inc: $(TOOLS_BUILD_INC_OBJ) 
	$(CC) $(TOOLS_BUILD_INC_OBJ) -o build_inc 

starter: $(TOOLS_STARTER_OBJ) 
	$(CC) $(TOOLS_STARTER_OBJ) -o starter


node_static: $(NODE_OBJ)
	$(AR) -rs libnode.a $(NODE_OBJ)

node_static_debug: $(NODE_DEBUG_OBJ)
	$(AR) -rs libnode.da $(NODE_DEBUG_OBJ)

node_dynamic: $(NODE_OBJ)
	$(CC) -DCREATELIB -shared $(NODE_OBJ) -o node.dll
	strip node.dll	

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $< $(FAILED_NOTIFY)


%.do: %.c 
	$(CC) $(DEBUG_CFLAGS) -c -o $@ $< $(FAILED_NOTIFY)

clean: libnyx_clean
	$(BEGIN_NOTIFY)
	rm -f node.dll *.do *.da *.o *.a *.so tools/build_inc/*.o imports/*.o imports/*.do

clean_binaries:
	rm -f unit_tests$(PLATFORM_EXT) unit_tests_debug$(PLATFORM_EXT) nyxi$(PLATFORM_EXT) nyxi_debug$(PLATFORM_EXT) build_inc$(PLATFORM_EXT) 

clean_debug:
	rm -f build_inc$(PLATFORM_EXT) nyxi_debug$(PLATFORM_EXT) unit_tests_debug$(PLATFORM_EXT) *.do *.da 
	
srcdist:	clean clean_binaries
	rm -f -r node.c-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	mkdir node.c-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(NODE_FILES) node.c-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(NODE_INCLUDE_FILES) node.c-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(UT_FILES) node.c-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(UT_INCLUDE_FILES) node.c-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(NODE_PRINT_FILES) node.c-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(NODE_PRINT_INCLUDE_FILES) node.c-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(TOOLS_BUILD_INC_FILES) node.c-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(TOOLS_BUILD_INC_INCLUDE_FILES) node.c-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp -r tools node.c-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp readme node.c-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp Makefile node.c-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	tar -cf node.c.src-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD).tar node.c-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	rm -f node.c.src-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD).tar.gz
	gzip -9 node.c.src-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD).tar
	rm -r node.c-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)

binariesdist: clean_binaries clean all clean
	rm -f -r node.c-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	mkdir node.c-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(NODE_BINS) node.c-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(UT_BINS) node.c-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(NODE_PRINT_BINS) node.c-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(TOOLS_BUILD_INC_BINS) node.c-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp readme node.c-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	tar -cf node.c.bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)-$(PLATFORM_NAME).tar node.c-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	rm -f svimpy.bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)-$(PLATFORM_NAME).tar.gz
	gzip -9 node.c.bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)-$(PLATFORM_NAME).tar
	rm -r node.c-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)

	
dist:	srcdist binariesdist 
	make clean clean_binaries
git:	
	git add .
	git commit -m "$(msg)"
	git push 
	
indent:	
	indent -bap -bli0 -i4 -l79 -ncs -npcs -npsl -fca -lc79 -fc1 -ts4 *.c *.h
