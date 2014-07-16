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

#ifndef NYX_H
#define NYX_H

#ifdef USE_MEMORY_DEBUGGING
#include "memory.h"
#endif


#include "../node.h"
#include "../imports/json.h"
#include "../imports/nyx.h"

#include "bindings/curl/curl.h"


#include "nyx_handler.h"

#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "getopt.h"
#include "signal.h"
#ifdef WIN32
#define _WIN32_WINNT  0x501 
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

//#include "termios.h"



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


typedef node *(*nyx_function_handler)(node*,node*,node*,node*);

/*basic object handling*/
node *create_obj(char *name);
void add_obj_kv(node *obj,node *kv);
void add_obj_string(node *obj,char *key,char *string);
void add_obj_int(node *obj,char *key,long i);
void add_obj_float(node *obj,char *key,double d);
void set_obj_string(node *obj,char *key,char *string);
void set_obj_int(node *obj,char *key,long i);
void set_obj_node(node *obj,char *key,node *n);
void set_obj_float(node *obj,char *key,double d);
char *get_obj_name(node *obj);
char *get_obj_type(node *obj);
void add_obj_double(node *obj,char *key,double d);
node *get_value(node *obj);
void set_obj_ptr(node *obj,char *key,void *n);

/*class handling internal*/
void add_class_object_function(node *class,char *method_name,node*(*handler)(node*,node*,node*,node*));

node *create_sys_class_object(void);
node *create_file_class_object(void);

char *state_to_json(node *state);
void append_http_query_array(node *state,node *value);

void add_garbage(node *state,node *obj);
void free_garbage(node *state,long min_level,node *skip_this);

/*class handling*/
void add_member(node *obj,node *member);
node *get_member(node *obj,char *key);
node *get_item(node *state,node *obj,node *key);

node *create_class_instance(node *class_obj);
node *create_base_obj_layout(char *obj_name);
node *create_class_object(void);


node *create_socket_class_object(void);
node *create_http_class_object(void);
node *create_file_class_object(void);

node *create_block_class_object(node *base_class,node *block);

node *create_nyx_state(node *base_class);

char *check_block_flag(node *state);

long get_execution_level(node *state);
void inc_execution_level(node *state);
void dec_execution_level(node *state);

node *execute_obj(node *state,node *obj,node *block,node *parameters,BOOL execute_block,BOOL execute_in_block);

node *search_block_path_for_member(node *block,char *key);

node *evaluate_statement(node *state,node *statement,node *block,long iteration_start_index,char *preop);
node *evaluate_block_instance(node *state,node *block_class_instance);
node *evaluate_block_instance_in(node *state,node *block_class_instance,node *block);
node *evaluate_block(node *state,node *block);
node *evaluate_block_in(node *state,node *block,node *master_block);

node *call_function(node *state,char *name,node *parameters);

char *convert_to_json(node *state,node *obj);
void convert_from_json(node *state,node *output,char *json);
void inc_obj_refcount(node *obj);
void reset_obj_refcount(node *obj);	
void dec_obj_refcount(node *obj);
long get_obj_refcount(node *obj);

node *init_nyx(void);
void close_nyx(node *state);

#endif
