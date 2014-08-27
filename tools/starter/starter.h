#ifdef WIN32
  #include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <process.h> 

#include "strings.h"


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


/*unsigned short header[5];
fread(&header,10,1,in);
printf("MAGIC:%c%c\n",header[0]&255,(header[0]>>8)&255);
unsigned short bytes_in_last_page = header[1];
if(bytes_in_last_page==0)
  bytes_in_last_page=512;
unsigned short num_pages = header[2]-1;
unsigned short num_relocs = header[3];
unsigned short header_size = header[4];
printf("bytes in last page:%u\n",bytes_in_last_page);
printf("pages:%u\n",num_pages);
printf("header_size:%u\n",(header_size*16)+(num_relocs*2));
printf("bytes in mz:%u\n",(num_pages*512)+bytes_in_last_page);
unsigned long next_header_offset = (num_pages*512)+bytes_in_last_page;

unsigned long pe_offset; 
fseek(in,0x3c,SEEK_SET);
fread(&pe_offset,4,1,in);
printf("PE offset:%u\n",pe_offset);

unsigned short pe_magic; 
fseek(in,pe_offset,SEEK_SET);
fread(&pe_magic,2,1,in);
printf("PE MAGIC:%c%c\n",pe_magic&255,(pe_magic>>8)&255);
*/


