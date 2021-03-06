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



#include "nyx.h"



#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "time.h"


//bunch of default functions


node *nyxh_eval(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_import(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_to_json(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_from_json(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_str(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_int(node *state,node *self,node *obj,node *block,node *parameters);

node *nyxh_char(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_from_char(node *state,node *self,node *obj,node *block,node *parameters);

node *nyxh_float(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_input(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_len(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_continue(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_restart(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_break(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_init_cmp(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_cmp(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_print(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_println(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_assign(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_assign_copy(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_set_value_only(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_neq(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_gt_eq(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_lt_eq(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_eq(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_gt(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_lt(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_mul(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_div(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_sub(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_add(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_split(node *state,node *self,node *obj,node *block,node *parameters);

node *nyxh_modulo(node *state,node *self,node *obj,node *block,node *parameters);


node *nyxh_index_of(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_substr(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_replace(node *state,node *self,node *obj,node *block,node *parameters);



node *nyxh_else(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_do(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_each(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_remove(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_slice(node *state,node *self,node *obj,node *block,node *parameters);
//node *nyxh_remove_by_key(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_args(node *state,node *self,node *obj,node *block,node *parameters);


node *nyxh_switch_name_value(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_immediate_add(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_immediate_sub(node *state,node *self,node *obj,node *block,node *parameters);

node *nyxh_pre_immediate_add(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_pre_immediate_sub(node *state,node *self,node *obj,node *block,node *parameters);


node *nyxh_and(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_or(node *state,node *self,node *obj,node *block,node *parameters);

node *nyxh_binary_and(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_binary_or(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_binary_xor(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_pre_binary_not(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_pre_not(node *state,node *self,node *obj,node *block,node *parameters);

node *nyxh_pre_at(node *state,node *self,node *obj,node *block,node *parameters);


node *nyxh_copy(node *state,node *self,node *obj,node *block,node *parameters);

node *nyxh_parent(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_get_block(node *state,node *self,node *obj,node *block,node *parameters);

node *nyxh_return(node *state,node *self,node *obj,node *block,node *parameters);

node *nyxh_name(node *state,node *self,node *obj,node *block,node *parameters);

node *nyxh_append_item(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_item_at(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_item_index_of(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_in(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_in_keys(node *state,node *self,node *obj,node *block,node *parameters);

node *nyxh_has(node *state,node *self,node *obj,node *block,node *parameters);

node *nyxh_remove_member(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_add_member(node *state,node *self,node *obj,node *block,node *parameters);


node *nyxh_pre_sub(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_pre_add(node *state,node *self,node *obj,node *block,node *parameters);



node *nyxh_handler_test(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxh_test(node *state,node *self,node *obj,node *block,node *parameters);



#endif
