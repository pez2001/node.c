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

#define _XOPEN_SOURCE

//#ifndef _GNU_SOURCE
//#define _GNU_SOURCE
//#endif
//#define _XOPEN_SOURCE 500
//#define _BSD_SOURCE
//#define _POSIX_C_SOURCE 3

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


node *nyxh_eval(node *state,node *obj,node *block,node *parameters);
node *nyxh_sys(node *state,node *obj,node *block,node *parameters);
node *nyxh_sys_name(node *state,node *obj,node *block,node *parameters);
node *nyxh_sys_working_directory(node *state,node *obj,node *block,node *parameters);
node *nyxh_sys_change_working_directory(node *state,node *obj,node *block,node *parameters);
node *nyxh_sys_execute(node *state,node *obj,node *block,node *parameters);
node *nyxh_sys_dump(node *state,node *obj,node *block,node *parameters);
node *nyxh_import(node *state,node *obj,node *block,node *parameters);
node *nyxh_to_json(node *state,node *obj,node *block,node *parameters);
node *nyxh_from_json(node *state,node *obj,node *block,node *parameters);
node *nyxh_writeall(node *state,node *obj,node *block,node *parameters);
node *nyxh_readall(node *state,node *obj,node *block,node *parameters);
node *nyxh_close(node *state,node *obj,node *block,node *parameters);
node *nyxh_open(node *state,node *obj,node *block,node *parameters);
node *nyxh_test(node *state,node *obj,node *block,node *parameters);
node *nyxh_str(node *state,node *obj,node *block,node *parameters);
node *nyxh_int(node *state,node *obj,node *block,node *parameters);
node *nyxh_float(node *state,node *obj,node *block,node *parameters);
node *nyxh_http_query(node *state,node *obj,node *block,node *parameters);
node *nyxh_input(node *state,node *obj,node *block,node *parameters);
node *nyxh_len(node *state,node *obj,node *block,node *parameters);
node *nyxh_continue(node *state,node *obj,node *block,node *parameters);
node *nyxh_restart(node *state,node *obj,node *block,node *parameters);
node *nyxh_break(node *state,node *obj,node *block,node *parameters);
node *nyxh_init_cmp(node *state,node *obj,node *block,node *parameters);
node *nyxh_cmp(node *state,node *obj,node *block,node *parameters);
node *nyxh_print(node *state,node *obj,node *block,node *parameters);
node *nyxh_println(node *state,node *obj,node *block,node *parameters);
node *nyxh_assign(node *state,node *obj,node *block,node *parameters);
node *nyxh_set_value_only(node *state,node *obj,node *block,node *parameters);
node *nyxh_neq(node *state,node *obj,node *block,node *parameters);
node *nyxh_gt_eq(node *state,node *obj,node *block,node *parameters);
node *nyxh_lt_eq(node *state,node *obj,node *block,node *parameters);
node *nyxh_eq(node *state,node *obj,node *block,node *parameters);
node *nyxh_gt(node *state,node *obj,node *block,node *parameters);
node *nyxh_lt(node *state,node *obj,node *block,node *parameters);
node *nyxh_mul(node *state,node *obj,node *block,node *parameters);
node *nyxh_div(node *state,node *obj,node *block,node *parameters);
node *nyxh_sub(node *state,node *obj,node *block,node *parameters);
node *nyxh_add(node *state,node *obj,node *block,node *parameters);
node *nyxh_split(node *state,node *obj,node *block,node *parameters);
node *nyxh_handler_test(node *state,node *obj,node *block,node *parameters);

node *nyxh_else(node *state,node *obj,node *block,node *parameters);
node *nyxh_do(node *state,node *obj,node *block,node *parameters);
node *nyxh_args(node *state,node *obj,node *block,node *parameters);


node *nyxh_and(node *state,node *obj,node *block,node *parameters);
node *nyxh_or(node *state,node *obj,node *block,node *parameters);
node *nyxh_pre_not(node *state,node *obj,node *block,node *parameters);

node *nyxh_sys_time(node *state,node *obj,node *block,node *parameters);

node *nyxh_sys_interpreter_version(node *state,node *obj,node *block,node *parameters);
node *nyxh_sys_interpreter_filename(node *state,node *obj,node *block,node *parameters);
node *nyxh_sys_script_filename(node *state,node *obj,node *block,node *parameters);
node *nyxh_sys_exit(node *state,node *obj,node *block,node *parameters);

node *nyxh_parent(node *state,node *obj,node *block,node *parameters);
node *nyxh_get_block(node *state,node *obj,node *block,node *parameters);

node *nyxh_return(node *state,node *obj,node *block,node *parameters);

node *nyxh_name(node *state,node *obj,node *block,node *parameters);

node *nyxh_item_at(node *state,node *obj,node *block,node *parameters);


node *nyxh_pre_sub(node *state,node *obj,node *block,node *parameters);
node *nyxh_pre_add(node *state,node *obj,node *block,node *parameters);


node *nyxh_socket_open(node *state,node *obj,node *block,node *parameters);
node *nyxh_socket_bind(node *state,node *obj,node *block,node *parameters);
node *nyxh_socket_connect(node *state,node *obj,node *block,node *parameters);
node *nyxh_socket_close(node *state,node *obj,node *block,node *parameters);
node *nyxh_socket_set_address(node *state,node *obj,node *block,node *parameters);
node *nyxh_socket_read(node *state,node *obj,node *block,node *parameters);
node *nyxh_socket_write(node *state,node *obj,node *block,node *parameters);

node *nyxh_http_create_request(node *state,node *obj,node *block,node *parameters);
node *nyxh_http_parse_answer(node *state,node *obj,node *block,node *parameters);



#endif
