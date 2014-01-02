USER := $(shell whoami)
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
PLATFORM_EXT = 
PLATFORM_NAME = Linux
endif
ifeq ($(UNAME), MINGW32_NT-6.1)
PLATFORM_NAME = Win32
PLATFORM_EXT = .exe
else
PLATFORM_NAME = Win32
PLATFORM_EXT = .exe
endif

MAJOR_VERSION = 0
MINOR_VERSION = 1
BUILD = 580
DEBUG_BUILD = 580

CFLAGS= -W -w -Os -std=c99 -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) -lm
DEBUG_CFLAGS = -m32 -g3 -O0 -Wall -pedantic -Wstrict-prototypes -std=c99 -fbounds-check -Wuninitialized -DUSE_DEBUGGING -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) -DDEBUG_BUILD=$(DEBUG_BUILD) -lm
CC=gcc
AR=ar
LD=ld

NODE_FILES = list.c node.c memory.c imports/json.c imports/fbx.c
NODE_BINS = node_c.a
NODE_INCLUDE_FILES = node.h list.h memory.h imports/json.h imports/fbx.h
NODE_OBJ = node.o list.o memory.o imports/json.o imports/fbx.o
NODE_DEBUG_OBJ = node.do list.do memory.do imports/json.do imports/fbx.do

UT_FILES = unit_tests.c
UT_OBJ = unit_tests.o
UT_BINS = unit_tests$(PLATFORM_EXT)
UT_DEBUG_OBJ = unit_tests.do
UT_INCLUDE_FILES = unit_tests.h

NODE_PRINT_FILES = node_print.c 
NODE_PRINT_BINS = node_print$(PLATFORM_EXT)
NODE_PRINT_INCLUDE_FILES = node_print.h 
NODE_PRINT_OBJ = node_print.o 
NODE_PRINT_DEBUG_OBJ = node_print.do 



TOOLS_BUILD_INC_FILES = tools/build_inc/build_inc.c
TOOLS_BUILD_INC_BINS = build_inc$(PLATFORM_EXT)
TOOLS_BUILD_INC_INCLUDE_FILES = tools/build_inc/build_inc.h
TOOLS_BUILD_INC_OBJ = tools/build_inc/build_inc.o


test: all
	./unit_tests$(PLATFORM_EXT)

test_debug: debug
	./unit_tests_debug$(PLATFORM_EXT)
	
debug: build_inc node_print_debug unit_tests_debug
	./build_inc$(PLATFORM_EXT) Makefile DEBUG_BUILD

all: clean clean_debug clean_binaries node_static node_dynamic build_inc node_print unit_tests debug
	@echo "Compiling for "$(PLATFORM_NAME)
	./build_inc$(PLATFORM_EXT) Makefile BUILD

unit_tests: node_static $(UT_OBJ) 
	$(CC) $(UT_OBJ) node.a -lm -o unit_tests 
	strip ./unit_tests$(PLATFORM_EXT)

unit_tests_debug: node_static_debug $(UT_DEBUG_OBJ) 
	$(CC) $(UT_DEBUG_OBJ) node.da -lm -o unit_tests_debug 

unit_tests_o: $(NODE_OBJ) $(UT_OBJ)
	$(CC) $(NODE_OBJ) $(UT_OBJ)  -lm -o unit_tests 

build_inc: $(TOOLS_BUILD_INC_OBJ) 
	$(CC) $(TOOLS_BUILD_INC_OBJ) -o build_inc

build_inc_o: $(TOOLS_BUILD_INC_OBJ)
	$(CC) $(TOOLS_BUILD_INC_OBJ) -o build_inc 

node_print: node_static  $(NODE_PRINT_OBJ)
	$(CC) $(NODE_PRINT_OBJ) node.a -lm -o node_print 

node_print_debug: node_static_debug $(NODE_PRINT_DEBUG_OBJ)
	$(CC) $(NODE_PRINT_DEBUG_OBJ) node.da -lm -o node_print_debug 

node_print_o: $(NODE_OBJ) $(NODE_PRINT_OBJ)
	$(CC) $(NODE_OBJ) $(NODE_PRINT_OBJ)  -lm -o node_print 
	
node_static: $(NODE_OBJ)
	$(AR) -rs node.a $(NODE_OBJ)
	#strip node.a

node_static_debug: $(NODE_DEBUG_OBJ)
	$(AR) -rs node.da $(NODE_DEBUG_OBJ)

node_dynamic: $(NODE_OBJ)
	$(CC) -DCREATELIB -shared $(NODE_OBJ) $(LDFLAGS) -o node.dll
	strip node.dll	

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $<

%.do: %.c 
	$(CC) $(DEBUG_CFLAGS) -c -o $@ $<


	
clean:
	rm -f *.elf *.do *.da *.o *.a *.so tools/build_inc/*.o 

clean_binaries:
	rm -f node_print$(PLATFORM_EXT) unit_tests$(PLATFORM_EXT) node_print_debug$(PLATFORM_EXT) unit_tests_debug$(PLATFORM_EXT) build_inc$(PLATFORM_EXT) 

clean_debug:
	rm -f build_inc$(PLATFORM_EXT) node_print_debug$(PLATFORM_EXT) unit_tests_debug$(PLATFORM_EXT) *.do *.da 
	
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
	git add *
	git commit
	git push github master
	
indent:	
	indent -bap -bli0 -i4 -l79 -ncs -npcs -npsl -fca -lc79 -fc1 -ts4 *.c *.h
