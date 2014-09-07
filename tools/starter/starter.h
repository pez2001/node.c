#ifdef WIN32
  #include <windows.h>
  #include <process.h> 
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sched.h>
#include <stddef.h>
//#include <spawn.h>



#include "../../strings.h"


#ifdef WIN32
FILE *popen(const char *command, const char *type);
#endif


#define STARTER_FLAGS_SHOW_CONSOLE 1
//#define STARTER_FLAGS_USE 1


typedef struct _entry
{
  unsigned long command_len;
  unsigned long payload_len;
  unsigned long second_payload_len;
  //unsigned long flags;
} entry;


typedef struct _tag
{
  int magic;
  int num_entries;
  int get_options;
} tag;




