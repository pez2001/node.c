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

#ifndef MODULE_LIBJOY_H
#define MODULE_LIBJOY_H

#include "../../nyx.h"

#include "../../bindings/sys/sys.h"

#include <libjoy.h>

#include <errno.h>

node *module_open(node *state);
void module_close(node *state);

node *nyxjoy_bind(node *state,node *class);
node *nyxjoy_create_class_object(node *state);
node *nyxjoy_open(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxjoy_close(node *state,node *self,node *obj,node *block,node *parameters);

node *nyxjoy_get_event(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxjoy_get_filtered_event(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxjoy_get_filtered_type_event(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxjoy_get_ordered_mode(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxjoy_set_ordered_mode(node *state,node *self,node *obj,node *block,node *parameters);

node *nyxjoy_sm_open(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxjoy_sm_close(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxjoy_sm_add(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxjoy_sm_remove(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxjoy_sm_poll(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxjoy_sm_get_state(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxjoy_sm_wait(node *state,node *self,node *obj,node *block,node *parameters);
node *nyxjoy_sm_wait_extra_precision(node *state,node *self,node *obj,node *block,node *parameters);

#endif
