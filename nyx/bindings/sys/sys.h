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

#ifndef BINDING_SYS_H
#define BINDING_SYS_H

#include "../../nyx.h"


#ifdef USE_SYS

#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "time.h"

#ifdef WIN32
FILE *popen(const char *command, const char *type);
#endif


void sys_bind(node *state,node *class);
node *sys_create_class_object(node *state);

node *sys(node *state,node *obj,node *block,node *parameters);

node *sys_name(node *state,node *obj,node *block,node *parameters);
node *sys_working_directory(node *state,node *obj,node *block,node *parameters);
node *sys_change_working_directory(node *state,node *obj,node *block,node *parameters);
node *sys_execute(node *state,node *obj,node *block,node *parameters);
node *sys_dump(node *state,node *obj,node *block,node *parameters);
node *sys_time(node *state,node *obj,node *block,node *parameters);
node *sys_sleep(node *state,node *obj,node *block,node *parameters);
node *sys_interpreter_version(node *state,node *obj,node *block,node *parameters);
node *sys_interpreter_filename(node *state,node *obj,node *block,node *parameters);
node *sys_script_filename(node *state,node *obj,node *block,node *parameters);
node *sys_exit(node *state,node *obj,node *block,node *parameters);
#endif

#endif
