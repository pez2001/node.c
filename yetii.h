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


#ifndef YETII_H
#define YETII_H


#include "list.h"
#include "node.h"
#include "imports/json.h"
#include "imports/fbx.h"
#include "imports/yeti.h"


#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/*
typedef struct _yeti_state 
{
	node *top_block_obj;
	//node *base_class;
	node *classes;
	node *garbage;
}yeti_state;
*/

/*internal use functions*/
char *AddCharToString(char *string,char letter);
char *CreateEmptyString(void);

/*basic object handling*/
node *create_obj(char *name);
void add_obj_kv(node *obj,node *kv);
void add_obj_string(node *obj,char *key,char *string);
void set_obj_string(node *obj,char *key,char *string);
void set_obj_int(node *obj,char *key,long i);
void set_obj_node(node *obj,char *key,node *n);
char *get_obj_name(node *obj);
char *get_obj_type(node *obj);
void add_obj_int(node *obj,char *key,long i);
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

/*yeti_state *create_yeti_state(node *base_class);*/
node *create_yeti_state(node *yeti_block,node *base_class);

node *execute_obj(node *state,node *execution_obj,node *block,BOOL execute_block);//,BOOL dont_execute_block);

node *evaluate_statement(node *state,node *statement,node *block,long iteration_start_index);
node *evaluate_block_instance(node *state,node *block_class_instance);
node *evaluate_block_instance_in(node *state,node *block_class_instance,node *block);
void evaluate_block(node *state,node *block);



#endif
