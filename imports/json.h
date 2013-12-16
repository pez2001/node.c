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


#ifndef JSON_H
#define JSON_H

#include "../node.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define JSON_STATE_IN_NOTHING 0
#define JSON_STATE_IN_OBJ 1
#define JSON_STATE_IN_STRING 2
#define JSON_STATE_IN_ARRAY 4
#define JSON_STATE_IN_KEY 8
#define JSON_STATE_IN_VALUE 16


//im not shooting for an ast , more of a stream and state based interpreter
typedef struct _json_state 
{
  int state;   
  node *actual_obj;
  node *parent_obj;

}json_state;


node *json_Load(char *json,unsigned long len); 
node *json_LoadFile(char *filename); 

#endif