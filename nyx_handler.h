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

#ifndef NYX_HANDLER_H
#define NYX_HANDLER_H

#ifdef USE_MEMORY_DEBUGGING
#include "memory.h"
#endif


#include "list.h"
#include "node.h"
#include "imports/json.h"
#include "imports/fbx.h"
#include "imports/nyx.h"

#include "nyxi.h"


#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "getopt.h"


//bunch of default functions


void *nyxh_eval(node *state,node *execution_obj,node *block);
void *nyxh_sys(node *state,node *execution_obj,node *block);
void *nyxh_name(node *state,node *execution_obj,node *block);
void *nyxh_working_directory(node *state,node *execution_obj,node *block);
void *nyxh_change_working_directory(node *state,node *execution_obj,node *block);
void *nyxh_execute(node *state,node *execution_obj,node *block);
void *nyxh_dump(node *state,node *execution_obj,node *block);
void *nyxh_import(node *state,node *execution_obj,node *block);
void *nyxh_to_json(node *state,node *execution_obj,node *block);
void *nyxh_from_json(node *state,node *execution_obj,node *block);
void *nyxh_writeall(node *state,node *execution_obj,node *block);
void *nyxh_readall(node *state,node *execution_obj,node *block);
void *nyxh_close(node *state,node *execution_obj,node *block);
void *nyxh_open(node *state,node *execution_obj,node *block);
void *nyxh_test(node *state,node *execution_obj,node *block);
void *nyxh_str(node *state,node *execution_obj,node *block);
void *nyxh_int(node *state,node *execution_obj,node *block);
void *nyxh_http_query(node *state,node *execution_obj,node *block);
void *nyxh_input(node *state,node *execution_obj,node *block);
void *nyxh_len(node *state,node *execution_obj,node *block);
void *nyxh_continue(node *state,node *execution_obj,node *block);
void *nyxh_restart(node *state,node *execution_obj,node *block);
void *nyxh_break(node *state,node *execution_obj,node *block);
void *nyxh_init_cmp(node *state,node *execution_obj,node *block);
void *nyxh_cmp(node *state,node *execution_obj,node *block);
void *nyxh_print(node *state,node *execution_obj,node *block);
void *nyxh_assign(node *state,node *execution_obj,node *block);
void *nyxh_set_value_only(node *state,node *execution_obj,node *block);
void *nyxh_neq(node *state,node *execution_obj,node *block);
void *nyxh_gt_eq(node *state,node *execution_obj,node *block);
void *nyxh_lt_eq(node *state,node *execution_obj,node *block);
void *nyxh_eq(node *state,node *execution_obj,node *block);
void *nyxh_gt(node *state,node *execution_obj,node *block);
void *nyxh_lt(node *state,node *execution_obj,node *block);
void *nyxh_mul(node *state,node *execution_obj,node *block);
void *nyxh_div(node *state,node *execution_obj,node *block);
void *nyxh_sub(node *state,node *execution_obj,node *block);
void *nyxh_add(node *state,node *execution_obj,node *block);
void *nyxh_handler_test(node *state,node *execution_obj,node *block);



#endif
