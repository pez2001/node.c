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

#include "node.h"


char *node_CopyString(char *string)
{
  char *r = (char*)malloc(strlen(string) + 1);
  memset(r, 0, strlen(string) + 1);
  memcpy(r, string, strlen(string));
  return(r);
}

node *node_Create(void)
{
  node *n = (node*)malloc(sizeof(node));
  n->type = 0;
  n->value = NULL;
  n->items = list_Create(0,0);
  return(n);
}

node *node_CreateFilled(char *key,void *value,int type,item_list *items)
{
  node *n = (node*)malloc(sizeof(node));
  n->key = node_CopyString(key);
  n->value = value;
  n->type = type;
  n->items = items;
  return(n);
}

void *node_CreateValue(int type,void *value)
{
  void *r = NULL;
  switch(type)
  {
    case NODE_TYPE_NULL:
         break;
    case NODE_TYPE_INT:
         r = malloc(sizeof(int));
         break;
    case NODE_TYPE_FLOAT:
         r = malloc(sizeof(float));
         memcpy(r, value, sizeof(float));
    case NODE_TYPE_DOUBLE:
         r = malloc(sizeof(double));
         memcpy(r, value, sizeof(double));
         break;
    case NODE_TYPE_UINT8:
    case NODE_TYPE_SINT8:
    case NODE_TYPE_BOOL:
         r = malloc(sizeof(unsigned char));
         memcpy(r, value, sizeof(unsigned char));
         break;
    case NODE_TYPE_UINT16:
    case NODE_TYPE_SINT16:
         r = malloc(sizeof(unsigned short));
         memcpy(r, value, sizeof(unsigned short));
         break;
    case NODE_TYPE_UINT32:
    case NODE_TYPE_SINT32:
         r = malloc(sizeof(unsigned long));
         memcpy(r, value, sizeof(unsigned long));
         break;
    case NODE_TYPE_SINT64:
    case NODE_TYPE_UINT64:
         r = malloc(sizeof(unsigned long long));
         memcpy(r, value, sizeof(unsigned long long));
         break;
    case NODE_TYPE_STRING:
         r = (void*)node_CopyString(value);
         break;
    case NODE_TYPE_ARRAY:
         break;
    case NODE_TYPE_NODE:
         break;
    case NODE_TYPE_STUB:
         break;
    default:
         break;
  }
  return(r);
}

void node_Free(node *n,BOOL free_value)
{
  if(free_value)
    node_FreeValue(n->type,n->value);
  free(n);
}

void node_FreeValue(int type,void *value)
{
  switch(type)
  {
    case NODE_TYPE_NULL:
         break;
    case NODE_TYPE_INT:
    case NODE_TYPE_FLOAT:
    case NODE_TYPE_DOUBLE:
    case NODE_TYPE_UINT8:
    case NODE_TYPE_SINT8:
    case NODE_TYPE_BOOL:
    case NODE_TYPE_UINT16:
    case NODE_TYPE_SINT16:
    case NODE_TYPE_UINT32:
    case NODE_TYPE_SINT32:
    case NODE_TYPE_SINT64:
    case NODE_TYPE_UINT64:
    case NODE_TYPE_STRING:
         free(value);
         break;
    case NODE_TYPE_ARRAY:
         break;
    case NODE_TYPE_NODE:
         break;
    case NODE_TYPE_STUB:
         break;
    default:
         break;
  }
}

void node_SetKey(node *n,char *key)
{
  if(n->key != NULL)
    free(n->key);
  n->key = node_CopyString(key);
}

void node_SetValue(node *n,void *value,BOOL copy_value,BOOL free_old_value)
{
  if(free_old_value)
    node_FreeValue(n->type,n->value);
  if(copy_value)
  {
    
  }
  else
    n->value = value;
}

void node_SetType(node *n,int type)
{
  n->type = type;
}

void node_SetValueType(node *n,int type,void *value,BOOL copy_value,BOOL free_old_value)
{
  node_SetType(n,type);
  node_SetValue(n,value,copy_value,free_old_value);
}


void node_SetItems(node *n, item_list *items)
{
  n->items = items;
}

node *node_Copy(node *n,BOOL copy_value)
{
  void *v = node_CreateValue(n->type,n->value); 
  item_list *l = list_Copy(n->items);
  node *r = node_CreateFilled(n->key,v,n->type,l);
  return(r);
}

void *node_GetValue(node *n)
{
  return(n->value);
}

int node_GetType(node *n)
{
  return(n->type);
}

char *node_GetKey(node *n)
{
  return(n->key);
}

item_list *node_GetItems(node *n)
{
  return(n->items);
}

