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


typedef struct _yeti_state 
{
	node *il_stream;
	node *top_scope;
	list *statement_stack;
	list *obj_stack;
	node *root_obj;
	list *id_stack;
	list *parameter_stack;
	node *base_class;

}yeti_state;


node *evaluate_statement(yeti_state *state,node *statement,node *block,long iteration_start_index);

/*
object system layout

using class instances
originating from a class tree







*/






#endif
