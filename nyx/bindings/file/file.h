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

#ifndef BINDING_FILE_H
#define BINDING_FILE_H

#include "../../nyx.h"


#ifdef USE_FILE

#include <stdio.h>

void file_binding_open(node *state);
void file_binding_close(node *state);

node *file_bind(node *base_class,node *class);
node *file_create_class_object(node *base_class);
node *file_writeall(node *state,node *obj,node *block,node *parameters);
node *file_readall(node *state,node *obj,node *block,node *parameters);
node *file_writeallbytes(node *state,node *obj,node *block,node *parameters);
node *file_readallbytes(node *state,node *obj,node *block,node *parameters);
node *file_close(node *state,node *obj,node *block,node *parameters);
node *file_open(node *state,node *obj,node *block,node *parameters);

#endif

#endif
