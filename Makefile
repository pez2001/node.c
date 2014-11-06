
LIBDL = $(shell echo \\\#include \<dlfcn.h\>\\n int main\(int c,char**v\){} | gcc -ldl -xc -o a.out - 2>/dev/null ;echo $$? ;rm a.out 2>/dev/null ;rm a.exe 2>/dev/null)
LIBCURL = $(shell echo \\\#include \<curl/curl.h\>\\n int main\(int c,char**v\){} | gcc -lcurl -xc -o a.out - 2>/dev/null ;echo $$? ;rm a.out 2>/dev/null ;rm a.exe 2>/dev/null)
LIBWEBSOCKETS = $(shell echo "int main(int c,char**v){}" | gcc -lwebsockets -xc -o a.out - 2>/dev/null ;echo $$? ;rm a.out 2>/dev/null ;rm a.exe 2>/dev/null)
#LIBMICROHTTPD = $(shell echo "int main(int c,char**v){}" | gcc -lmicrohttpd -xc -o a.out - 2>/dev/null ;echo $$? ;rm a.out 2>/dev/null ;rm a.exe 2>/dev/null)

#USE_MEMORY_DEBUG=True

USER := $(shell whoami)
UNAME := $(shell uname)
SYS := $(shell gcc -dumpmachine)


ifneq (, $(findstring linux, $(SYS)))
PLATFORM_EXT = 
PLATFORM_LIB_PREFIX = lib
PLATFORM_LIB_EXT = .so
PLATFORM_NAME = Linux
PLATFORM_LIBS = -lm
PLATFORM_LDFLAGS = 
#-static
#-fPIC -DPIC
PLATFORM_CFLAGS = -fPIC -DPIC -DLINUX -D_XOPEN_SOURCE=700
PLATFORM_DEBUG_CFLAGS = -fPIC -DPIC -DLINUX -D_XOPEN_SOURCE=700
#-DUSE_MEMORY_DEBUGGING

ifeq ($(LIBCURL), 0)
	PLATFORM_LIBS += -lcurl
endif
ifeq ($(LIBWEBSOCKETS), 0)
	PLATFORM_LIBS += -lwebsockets
endif
ifeq ($(LIBMICROHTTPD), 0)
	PLATFORM_LIBS += -lmicrohttpd
endif
ifeq ($(USE_MEMORY_DEBUG),True)
	#PLATFORM_CFLAGS += -DUSE_MEMORY_DEBUGGING
	PLATFORM_DEBUG_CFLAGS += -DUSE_MEMORY_DEBUGGING
endif
ifeq ($(LIBDL), 0)
	PLATFORM_LIBS += -ldl
endif


else ifneq (, $(findstring mingw, $(SYS)))
PLATFORM_NAME = Win32
PLATFORM_EXT = .exe
PLATFORM_LIB_PREFIX = 
PLATFORM_LIB_EXT = .dll
PLATFORM_LIBS = -lwsock32 -lws2_32 -lm -lcurl.dll -lmicrohttpd.dll -lwebsockets.dll
PLATFORM_LDFLAGS = -static -static-libgcc -static-libstdc++
PLATFORM_CFLAGS = -DWIN32 -DWINVER=0x501 -D_WIN32_WINNT=0x0501
PLATFORM_DEBUG_CFLAGS = -DWIN32 -DWINVER=0x501 -D_WIN32_WINNT=0x0501
#-DUSE_MEMORY_DEBUGGING

ifeq ($(LIBCURL), 0)
	PLATFORM_LIBS += -lcurl.dll
endif
ifeq ($(LIBWEBSOCKETS), 0)
	PLATFORM_LIBS += -lwebsockets.dll
endif
ifeq ($(LIBMICROHTTPD), 0)
	PLATFORM_LIBS += -lmicrohttpd.dll
endif
ifeq ($(USE_MEMORY_DEBUG),True)
	#PLATFORM_CFLAGS += -DUSE_MEMORY_DEBUGGING
	PLATFORM_DEBUG_CFLAGS += -DUSE_MEMORY_DEBUGGING
endif


else ifneq (, $(findstring cygwin, $(SYS)))
 # Do cygwin things
else
 # Do things for others
endif



#BEGIN_NOTIFY =	@playsound.exe /c/Users/pez2001/Downloads/217656__reitanna__knuckles-cracking.wav
#FAILED_NOTIFY = || @playsound.exe /c/Users/pez2001/Downloads/123921__silencer1337__machinefail.wav
#SUCCESS_NOTIFY = @playsound.exe /c/Users/pez2001/Downloads/187404__mazk1985__robot-ready.wav

BEGIN_NOTIFY = notify-send -t 100 -u low -i face-plain "starting compilation of node.c for $(PLATFORM_NAME)"
FAILED_NOTIFY = notify-send -t 1000 -u critical -i face-devilish "compilation of node.c has failed"
SUCCESS_NOTIFY = notify-send -t 1000 -u low -i face-laugh "compilation of node.c successful (build:$(BUILD))"

BEGIN_NOTIFY = 
FAILED_NOTIFY = 
SUCCESS_NOTIFY = 


MAJOR_VERSION = 0
MINOR_VERSION = 6
BUILD = 6303
DEBUG_BUILD = 5241

CSTD = -std=c99
#-std c99

#CFLAGS= -W -w -Os -std=$(CSTD) -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) $(PLATFORM_CFLAGS) -lm
#DEBUG_CFLAGS = -DUSE_MEMORY_DEBUGGING -m32 -g3 -O0 -Wall -pedantic -Wstrict-prototypes -std=$(CSTD) -fbounds-check -Wuninitialized -DUSE_DEBUGGING -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) -DDEBUG_BUILD=$(DEBUG_BUILD) -lm
CFLAGS= -Wall -Wextra -W -w -Os $(CSTD) -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) $(PLATFORM_CFLAGS)
DEBUG_CFLAGS = -Wall -Wextra -Wformat=0 -pedantic -fbounds-check -Wuninitialized -O0 -g3 -Wall $(CSTD) -DUSE_DEBUGGING -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) -DDEBUG_BUILD=$(DEBUG_BUILD) $(PLATFORM_DEBUG_CFLAGS)
#-pedantic -fbounds-check -Wuninitialized -O0 -g3
#-Wstrict-prototypes
#DEBUG_CFLAGS = -m32 -g3 -O0 -Wall -pedantic -Wstrict-prototypes -std=$(CSTD) -fbounds-check -Wuninitialized -DUSE_DEBUGGING -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) -DDEBUG_BUILD=$(DEBUG_BUILD) $(PLATFORM_DEBUG_CFLAGS)
LDFLAGS= $(PLATFORM_LIBS) $(PLATFORM_LDFLAGS) 
#DEBUG_LDFLAGS = -m32 $(LDFLAGS)
DEBUG_LDFLAGS = $(LDFLAGS)

CC=gcc
AR=ar -c
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

TOOLS_STARTER_FILES = tools/starter/starter.c 
TOOLS_STARTER_BINS = starter$(PLATFORM_EXT)
TOOLS_STARTER_INCLUDE_FILES = tools/starter/starter.h 
TOOLS_STARTER_OBJ = tools/starter/starter.o
TOOLS_STARTER_DEBUG_OBJ = tools/starter/starter.do




def: clean all debug

.PHONY: all

all:
	$(MAKE) library && $(MAKE) compile_end || $(MAKE) compile_failed

.PHONY: debug

debug:
	$(MAKE) library_debug && $(MAKE) compile_debug_end || $(MAKE) compile_debug_failed


.PHONY: compile_start

compile_start: 
	@echo "====================================================="
	@echo "::::::::::::\033[34m Compiling node.c for $(PLATFORM_NAME)\033[0m :::::::::::::"
	@$(BEGIN_NOTIFY)

.PHONY: compile_debug_start

compile_debug_start: 
	@echo "====================================================="
	@echo "::::::::\033[36m Compiling node.c (DEBUG) for $(PLATFORM_NAME)\033[0m :::::::::"
	@$(BEGIN_NOTIFY)


.PHONY: compile_end

compile_end:
	@echo "====================================================="
	@echo "::::::::::\033[32m Compilation of node.c SUCCESSFUL\033[0m :::::::::"
	@echo "====================================================="
	@./build_inc$(PLATFORM_EXT) Makefile BUILD
	@./build_inc$(PLATFORM_EXT) README.md BUILD
	@$(SUCCESS_NOTIFY)

.PHONY: compile_debug_end

compile_debug_end:
	@echo "====================================================="
	@echo "::::::\033[32m Compilation of node.c (DEBUG) SUCCESSFUL\033[0m :::::"
	@echo "====================================================="
	@./build_inc$(PLATFORM_EXT) Makefile BUILD
	@./build_inc$(PLATFORM_EXT) README.md BUILD
	@$(SUCCESS_NOTIFY)


.PHONY: compile_failed

compile_failed:
	@echo "====================================================="
	@echo ":::::::::::\033[31m Compilation of node.c FAILED\033[0m ::::::::::::"
	@echo "====================================================="
	@$(FAILED_NOTIFY)

.PHONY: compile_debug_failed

compile_debug_failed:
	@echo "====================================================="
	@echo ":::::::\033[31m Compilation of node.c (DEBUG) FAILED\033[0m ::::::::"
	@echo "====================================================="
	@$(FAILED_NOTIFY)


test: all
	@./unit_tests$(PLATFORM_EXT)


test_debug: debug
	@./unit_tests_debug$(PLATFORM_EXT)
	
library_debug: compile_debug_start build_inc node_static_debug node_shared_debug starter_debug libnyx_debug nyxi_debug unit_tests_debug
	@./build_inc$(PLATFORM_EXT) Makefile DEBUG_BUILD

#	@echo "Compiling for "$(PLATFORM_NAME)
#	./build_inc$(PLATFORM_EXT) Makefile BUILD
#	$(SUCCESS_NOTIFY)

library: compile_start node_static node_shared build_inc starter libnyx nyxi unit_tests 

print_version:
	@echo -n "$(BUILD)"

clean_all: clean libnyx_clean clean_debug clean_binaries all debug 
	@echo "Cleaned all"


.PHONY: libnyx
.PHONY: libnyx_clean
.PHONY: libnyx_debug
	
libnyx: node_static
	$(MAKE) -C nyx all

libnyx_clean: 
	$(MAKE) -C nyx clean 

libnyx_clean_binaries: 
	$(MAKE) -C nyx clean_binaries

libnyx_debug: node_static_debug 
	$(MAKE) -C nyx debug

unit_tests: node_static $(UT_OBJ) 
	$(CC) $(UT_OBJ) libnode.a  $(LDFLAGS) -o unit_tests 
	@strip ./unit_tests$(PLATFORM_EXT)

unit_tests_debug: node_static_debug $(UT_DEBUG_OBJ) 
	$(CC) $(UT_DEBUG_OBJ) libnode.da $(DEBUG_LDFLAGS) -o unit_tests_debug 

nyxi: node_static libnyx $(NYXI_OBJ) 
	$(CC) $(NYXI_OBJ) nyx/libnyx.a libnode.a $(LDFLAGS) -o nyxi 
	@strip ./nyxi$(PLATFORM_EXT)

nyxi_debug: node_static_debug libnyx_debug $(NYXI_DEBUG_OBJ) 
	$(CC) $(NYXI_DEBUG_OBJ) nyx/libnyx.da libnode.da $(DEBUG_LDFLAGS) -o nyxi_debug 

build_inc: $(TOOLS_BUILD_INC_OBJ) 
	$(CC) $(TOOLS_BUILD_INC_OBJ) -o build_inc 

starter: $(TOOLS_STARTER_OBJ) 
	$(CC) $(TOOLS_STARTER_OBJ) libnode.a -o starter

starter_debug: $(TOOLS_STARTER_DEBUG_OBJ) 
	$(CC) $(TOOLS_STARTER_DEBUG_OBJ) libnode.da -o starter_debug

node_static: $(NODE_OBJ)
	$(AR) -rs libnode.a $(NODE_OBJ)

node_static_debug: $(NODE_DEBUG_OBJ)
	$(AR) -rs libnode.da $(NODE_DEBUG_OBJ)

node_shared: $(NODE_OBJ)
	$(CC) -rdynamic -nostdlib -DCREATELIB -shared $(NODE_OBJ) -o $(PLATFORM_LIB_PREFIX)node$(PLATFORM_LIB_EXT)
	@strip $(PLATFORM_LIB_PREFIX)node$(PLATFORM_LIB_EXT)	

node_shared_debug: $(NODE_DEBUG_OBJ)
	$(CC) -rdynamic -nostdlib -DCREATELIB -shared $(NODE_DEBUG_OBJ) -o $(PLATFORM_LIB_PREFIX)node_debug$(PLATFORM_LIB_EXT)

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $<


%.do: %.c 
	$(CC) $(DEBUG_CFLAGS) -c -o $@ $<

install:
	@echo "====================================================="
	@echo "::::::::::::::::\033[32m INSTALLING Node.c/Nyx\033[0m ::::::::::::::"
	@echo "====================================================="
	@cp libnode.so /usr/lib
	@cp nyx/libnyx.so /usr/lib
	@cp nyxi /usr/bin

uninstall:
	@echo "====================================================="
	@echo ":::::::::::::::::\033[32m REMOVING Node.c/Nyx\033[0m :::::::::::::::"
	@echo "====================================================="
	@rm /usr/lib/libnode.so /usr/lib/libnyx.so /usr/bin/nyxi



clean: libnyx_clean
	rm -f $(PLATFORM_LIB_PREFIX)node$(PLATFORM_LIB_EXT) *.do *.da *.o *.a *.so tools/build_inc/*.o imports/*.o imports/*.do tools/starter/*.o tools/starter/*.do

clean_binaries: libnyx_clean_binaries
	rm -f unit_tests$(PLATFORM_EXT) unit_tests_debug$(PLATFORM_EXT) nyxi$(PLATFORM_EXT) nyxi_debug$(PLATFORM_EXT) build_inc$(PLATFORM_EXT) starter$(PLATFORM_EXT) starter_debug$(PLATFORM_EXT)

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
	@git add .
	@git commit -m "$(msg)"
	@git push 
	
indent:	
	indent -bap -bli0 -i4 -l79 -ncs -npcs -npsl -fca -lc79 -fc1 -ts4 *.c *.h

todo:
	find . -type f -printf '%p\n' | xargs -d'\n' grep -I -n "TODO"

