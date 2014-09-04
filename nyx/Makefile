
USE_SOCKETS = True
USE_CURL = True
USE_WEBSOCKETS = True
USE_HTTP = True
USE_MICROHTTPD = True
USE_CURSES = True
USE_SYS = True
USE_FILE = True

#USE_MEMORY_DEBUG = True



USER := $(shell whoami)
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	PLATFORM_EXT = 
	PLATFORM_NAME = Linux
	PLATFORM_LIBS = -lm
	PLATFORM_CFLAGS =
	PLATFORM_DEBUG_CFLAGS =
	ifeq ($(USE_CURL),True)
		PLATFORM_LIBS += -lcurl
		PLATFORM_CFLAGS += -DUSE_CURL
		PLATFORM_DEBUG_CFLAGS += -DUSE_CURL 
	endif
endif

ifeq ($(UNAME), MINGW32_NT-6.2)
	PLATFORM_NAME = Win32
	PLATFORM_EXT = .exe
	PLATFORM_LIBS = -lm
	PLATFORM_CFLAGS = -DWIN32 -DWINVER=0x501 -D_WIN32_WINNT=0x0501
	PLATFORM_DEBUG_CFLAGS = -DWIN32
	ifeq ($(USE_CURL),True)
		PLATFORM_LIBS += -lcurl.dll
		PLATFORM_CFLAGS += -DUSE_CURL
		PLATFORM_DEBUG_CFLAGS += -DUSE_CURL 
	endif
	ifeq ($(USE_MICROHTTPD),True)
		PLATFORM_LIBS += -lmicrohttpd.dll
		PLATFORM_CFLAGS += -DUSE_MICROHTTPD
		PLATFORM_DEBUG_CFLAGS += -DUSE_MICROHTTPD 
	endif
	ifeq ($(USE_SOCKETS),True)
		PLATFORM_LIBS += -lwsock32 -lws2_32
		PLATFORM_CFLAGS += -DUSE_SOCKETS
		PLATFORM_DEBUG_CFLAGS += -DUSE_SOCKETS 
	endif
	ifeq ($(USE_WEBSOCKETS),True)
		PLATFORM_LIBS += -lwebsockets.dll
		PLATFORM_CFLAGS += -DUSE_WEBSOCKETS 
		PLATFORM_DEBUG_CFLAGS += -DUSE_WEBSOCKETS 
	endif
	ifeq ($(USE_CURSES),True)
		PLATFORM_LIBS += -lcurses
		PLATFORM_CFLAGS += -DUSE_CURSES
		PLATFORM_DEBUG_CFLAGS += -DUSE_CURSES 
	endif
	ifeq ($(USE_HTTP),True)
		PLATFORM_CFLAGS += -DUSE_HTTP
		PLATFORM_DEBUG_CFLAGS += -DUSE_HTTP
	endif
	ifeq ($(USE_SYS),True)
		PLATFORM_CFLAGS += -DUSE_SYS
		PLATFORM_DEBUG_CFLAGS += -DUSE_SYS
	endif
	ifeq ($(USE_FILE),True)
		PLATFORM_CFLAGS += -DUSE_FILE
		PLATFORM_DEBUG_CFLAGS += -DUSE_FILE
	endif
	ifeq ($(USE_MEMORY_DEBUG),True)
		#PLATFORM_CFLAGS += -DUSE_MEMORY_DEBUGGING
		PLATFORM_DEBUG_CFLAGS += -DUSE_MEMORY_DEBUGGING
	endif

else
	PLATFORM_NAME = Win32
	PLATFORM_EXT = 
	PLATFORM_LIBS = -lwsock32 -lws2_32 -lm -lcurl.dll
	PLATFORM_CFLAGS = -DWIN32 -DWINVER=0x501 -D_WIN32_WINNT=0x0501
	PLATFORM_DEBUG_CFLAGS = -DWIN32
endif


#BEGIN_NOTIFY =	@playsound.exe /c/Users/pez2001/Downloads/217656__reitanna__knuckles-cracking.wav
#FAILED_NOTIFY = || @playsound.exe /c/Users/pez2001/Downloads/123921__silencer1337__machinefail.wav
#SUCCESS_NOTIFY = @playsound.exe /c/Users/pez2001/Downloads/187404__mazk1985__robot-ready.wav

BEGIN_NOTIFY = 
FAILED_NOTIFY = 
SUCCESS_NOTIFY = 


MAJOR_VERSION = 0
MINOR_VERSION = 5
BUILD = 1046
DEBUG_BUILD = 2115

CSTD = c99

#-DUSE_MEMORY_DEBUGGING
#CFLAGS= -W -w -Os -std=$(CSTD) -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) $(PLATFORM_CFLAGS) -lm
#DEBUG_CFLAGS = -DUSE_MEMORY_DEBUGGING -m32 -g3 -O0 -Wall -pedantic -Wstrict-prototypes -std=$(CSTD) -fbounds-check -Wuninitialized -DUSE_DEBUGGING -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) -DDEBUG_BUILD=$(DEBUG_BUILD) -lm
CFLAGS= -W -w -Os -std=$(CSTD) -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) $(PLATFORM_CFLAGS)
DEBUG_CFLAGS = -m32 -g3 -O0 -Wall -pedantic -Wstrict-prototypes -std=$(CSTD) -fbounds-check -Wuninitialized -DUSE_DEBUGGING -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) -DDEBUG_BUILD=$(DEBUG_BUILD) $(PLATFORM_DEBUG_CFLAGS)
#DEBUG_CFLAGS = -m32 -g3 -O0 -Wall -pedantic -Wstrict-prototypes -std=$(CSTD) -fbounds-check -Wuninitialized -DUSE_DEBUGGING -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) -DDEBUG_BUILD=$(DEBUG_BUILD) $(PLATFORM_DEBUG_CFLAGS)
LDFLAGS= $(PLATFORM_LIBS) -static -static-libgcc
# -static-libstdc++
DEBUG_LDFLAGS = $(LDFLAGS)

CC=gcc
AR=ar
LD=ld

NYX_FILES = nyx.c nyx_handler.c bindings/curl/curl.c bindings/microhttpd/microhttpd.c bindings/websockets/websockets.c bindings/sockets/sockets.c bindings/http/http.c bindings/sys/sys.c bindings/file/file.c
NYX_OBJ = nyx.o nyx_handler.o bindings/curl/curl.o bindings/microhttpd/microhttpd.o bindings/websockets/websockets.o bindings/sockets/sockets.o bindings/http/http.o bindings/sys/sys.o bindings/file/file.o
NYX_BINS = nyx$(PLATFORM_EXT)
NYX_DEBUG_OBJ = nyx.do nyx_handler.do bindings/curl/curl.do bindings/microhttpd/microhttpd.do bindings/websockets/websockets.do bindings/sockets/sockets.do bindings/http/http.do bindings/sys/sys.do bindings/file/file.do
NYX_INCLUDE_FILES = nyx.h nyx_handler.h bindings/curl/curl.h bindings/microhttpd/microhttpd.h bindings/websockets/websockets.h  bindings/sockets/sockets.h bindings/http/http.h bindings/sys/sys.h bindings/file/file.h

UT_FILES = unit_tests.c
UT_OBJ = unit_tests.o
UT_BINS = unit_tests$(PLATFORM_EXT)
UT_DEBUG_OBJ = unit_tests.do
UT_INCLUDE_FILES = unit_tests.h

TOOLS_BUILD_INC_FILES = tools/build_inc/build_inc.c
TOOLS_BUILD_INC_BINS = build_inc$(PLATFORM_EXT)
TOOLS_BUILD_INC_INCLUDE_FILES = tools/build_inc/build_inc.h
TOOLS_BUILD_INC_OBJ = tools/build_inc/build_inc.o


print_version:
	@echo -n "$(BUILD)"

test: all
	./unit_tests$(PLATFORM_EXT)

test_debug: debug
	./unit_tests_debug$(PLATFORM_EXT)
	
debug: build_inc nyx_static_debug unit_tests_debug
	./build_inc$(PLATFORM_EXT) Makefile DEBUG_BUILD

clean_all: clean clean_debug clean_binaries all 
	@echo "Compiling for "$(PLATFORM_NAME)
	./build_inc$(PLATFORM_EXT) Makefile BUILD
	$(SUCCESS_NOTIFY)

all: build_inc nyx_static nyx_dynamic unit_tests debug 
	@echo "Compiling for "$(PLATFORM_NAME)
	./build_inc$(PLATFORM_EXT) Makefile BUILD
	$(SUCCESS_NOTIFY)

unit_tests: nyx_static $(UT_OBJ) 
	$(CC) $(UT_OBJ) libnyx.a ../libnode.da $(LDFLAGS) -o unit_tests 
	strip ./unit_tests$(PLATFORM_EXT)

unit_tests_debug: nyx_static_debug $(UT_DEBUG_OBJ) 
	$(CC) $(UT_DEBUG_OBJ) libnyx.da ../libnode.da $(DEBUG_LDFLAGS) -o unit_tests_debug 

build_inc: $(TOOLS_BUILD_INC_OBJ) 
	$(CC) $(TOOLS_BUILD_INC_OBJ) -o build_inc 

nyx_static: $(NYX_OBJ)
	$(AR) -rs libnyx.a ../libnode.a $(NYX_OBJ) 

nyx_static_debug: $(NYX_DEBUG_OBJ)
	$(AR) -rs libnyx.da ../libnode.da $(NYX_DEBUG_OBJ) 

nyx_dynamic: $(NYX_OBJ)
	$(CC) -DCREATELIB -shared ../node.dll $(NYX_OBJ) $(PLATFORM_LIBS) -o nyx.dll
	strip nyx.dll	

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $< $(FAILED_NOTIFY)


%.do: %.c 
	$(CC) $(DEBUG_CFLAGS) -c -o $@ $< $(FAILED_NOTIFY)

clean:
	$(BEGIN_NOTIFY)
	rm -f nyx.dll *.elf *.do *.da *.o *.a *.so tools/build_inc/*.o bindings/curl/*.o bindings/curl/*.do bindings/micohttpd/*.o bindings/microhttpd/*.do bindings/websockets/*.do bindings/websockets/*.o bindings/sockets/*.do bindings/sockets/*.o bindings/http/*.do bindings/http/*.o bindings/sys/*.do bindings/sys/*.o bindings/file/*.do bindings/file/*.o

clean_binaries:
	rm -f unit_tests$(PLATFORM_EXT) unit_tests_debug$(PLATFORM_EXT) build_inc$(PLATFORM_EXT) 

clean_debug:
	rm -f build_inc$(PLATFORM_EXT) unit_tests_debug$(PLATFORM_EXT) *.do *.da 
	
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
