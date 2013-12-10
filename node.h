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

#ifndef NODE_H
#define NODE_H

#include "lists.h"

#ifdef __cplusplus
extern "C"  {
#endif

#define NODE_TYPE_NULL 0
#define NODE_TYPE_INT 1
#define NODE_TYPE_FLOAT 2
#define NODE_TYPE_DOUBLE 3
#define NODE_TYPE_UINT8 4
#define NODE_TYPE_UINT16 5
#define NODE_TYPE_UINT32 6
#define NODE_TYPE_UINT64 7
#define NODE_TYPE_SINT8 8
#define NODE_TYPE_SINT16 9
#define NODE_TYPE_SINT32 10
#define NODE_TYPE_SINT64 11
#define NODE_TYPE_BOOL 12
#define NODE_TYPE_STRING 13
#define NODE_TYPE_ARRAY 14
#define NODE_TYPE_NODE 15
#define NODE_TYPE_STUB 16



typedef struct
{
  char *key;
  void *value;
  int type;
  item_list *items;
} node;


char *node_CopyString(char *string);

node *node_Create(void);
node *node_CreateFilled(char *key,void *value,int type,item_list *items);
void *node_CreateValue(int type,void *value);
void node_Free(node *n,BOOL free_value);
void node_FreeValue(int type,void *value);
void node_SetKey(node *n,char *key);
void node_SetValue(node *n,void *value,BOOL copy_value,BOOL free_old_value);
void node_SetValueType(node *n,int type,void *value,BOOL copy_value,BOOL free_old_value);
void node_SetType(node *n,int type);
void node_SetItems(node *n, item_list *items);
node *node_Copy(node *n,BOOL copy_value);

void *node_GetValue(node *n);
int node_GetType(node *n);



#ifdef __cplusplus
} 
#endif


#endif