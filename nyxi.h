/* 
 * node.c - c based data tree with various imports & exports
 * (c) 2013 by Tim Theede aka Pez2001 <pez2001@voyagerproject.de> / vp
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * THIS SOFTWARE IS SUPPLIED AS IT IS WITHOUT ANY WARRANTY!
 *
 */

#ifndef NYXI_H
#define NYXI_H

#ifdef USE_MEMORY_DEBUGGING
#include "memory.h"
#endif


#include "list.h"
#include "node.h"
#include "imports/json.h"
#include "imports/fbx.h"
#include "imports/nyx.h"


#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "getopt.h"
#include "signal.h"
//#include "termios.h"


const char *nyxi_helpmsg = "Usage: nyxi [OPTION]... [INPUT FILE]\n\
Execute nyx script Files.\n\
\n\
  -i    interactive mode\n\
  -a    print script ast\n\
  -s    interpret INPUT FILE as script string\n\
  -h    display this help and exit\n\
  -v    output version information and exit\n\
  -p    print the return value\n\
\n\
  Use - as input file or leave empty to use STDIN\n\
  CTRL-D to exit\n\
\n\
  Return value of script will be used as the exit code.\n\
\n\
      Report bugs to <pez2001@voyagerproject.de>.\n";


/*#include "sys/utsname.h"*/

/*
typedef struct _nyx_state 
{
	node *top_block_obj;
	//node *base_class;
	node *classes;
	node *garbage;
}nyx_state;
*/

/*internal use functions*/
char *AddCharToString(char *string,char letter);
char *CreateEmptyString(void);
char *StringCat(char *a,char *b);
char *StringCatFree(char *a,char *b);
char *StringMult(char *a,long count);
char *StringCopy(char *src);
char *StringAdd(char *a,char *b);


char *convert_to_string(long i);

/*basic object handling*/
node *create_obj(char *name);
void add_obj_kv(node *obj,node *kv);
void add_obj_string(node *obj,char *key,char *string);
void add_obj_int(node *obj,char *key,long i);
void set_obj_string(node *obj,char *key,char *string);
void set_obj_int(node *obj,char *key,long i);
void set_obj_node(node *obj,char *key,node *n);
char *get_obj_name(node *obj);
char *get_obj_type(node *obj);
void add_obj_double(node *obj,char *key,double d);
node *get_value(node *obj);

/*class handling internal*/
void add_class_object_internal_function(node *class,node *base_class,char *method_name);

/*class handling*/
void add_member(node *obj,node *member);
node *get_member(node *obj,char *key);
node *get_item(node *obj,node *key);

node *create_class_instance(node *class_obj);
node *create_base_obj_layout(char *obj_name);
node *create_class_object(void);

node *create_block_obj(node *base_class,node *block);
node *create_execution_obj(node *method,node *parameters,node *sub_execution_obj);

/*nyx_state *create_nyx_state(node *base_class);*/
//node *create_nyx_state(node *nyx_block,node *base_class);
node *create_nyx_state(node *base_class);

node *execute_obj(node *state,node *execution_obj,node *block,BOOL execute_block);//,BOOL dont_execute_block);

node *search_block_path_for_member(node *block,char *key);

node *evaluate_statement(node *state,node *statement,node *block,long iteration_start_index);
node *evaluate_block_instance(node *state,node *block_class_instance);
node *evaluate_block_instance_in(node *state,node *block_class_instance,node *block);
node *evaluate_block(node *state,node *block);

node *call_function(node *state,char *name,node *parameters);//,node *block);

char *convert_to_json(node *state,node *obj);
void convert_from_json(node *state,node *output,char *json);
#endif