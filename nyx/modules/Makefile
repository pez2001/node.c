LIBJOY = $(shell echo \\\#include \<libjoy.h\>\\n int main\(int c,char**v\){} | gcc -ljoy -xc -o a.out - 2>/dev/null ;echo $$? ;rm a.out 2>/dev/null ;rm a.exe 2>/dev/null)

all: debug
ifneq ($(LIBJOY), 0)
	echo "Compilation dependancy not met -> install libjoy"
else
	$(MAKE) -C libjoy all
endif

debug:
ifneq ($(LIBJOY), 0)
	echo "Compilation dependancy not met -> install libjoy"
else
	$(MAKE) -C libjoy debug
endif


modules:
ifneq ($(LIBJOY), 0)
	echo "Compilation dependancy not met -> install libjoy"
else
	$(MAKE) -C libjoy module
endif


clean:
	$(MAKE) -C libjoy clean
