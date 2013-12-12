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
  n->parent = NULL;
  n->items = list_Create(0,0);
  return(n);
}

node *node_CreateFilled(node *parent,char *key,void *value,int type,item_list *items)
{
  node *n = (node*)malloc(sizeof(node));
  n->key = node_CopyString(key);
  n->value = value;
  n->type = type;
  n->items = items;
  n->parent = parent;
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
         memcpy(r, value, sizeof(int));
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
         if(value!=NULL)
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
    n->value = node_CreateValue(n->type,value);
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

void node_SetParent(node *n,node *p)
{
  n->parent = p;
}

node *node_Copy(node *n,BOOL copy_value)
{
  void *v = node_CreateValue(n->type,n->value); 
  item_list *l = list_Copy(n->items);
  node *r = node_CreateFilled(n->parent,n->key,v,n->type,l);
  return(r);
}

int node_IsType(node *n, int type)
{
  return(n->type == type);
}

node *node_GetParent(node *n)
{
  return(n->parent);
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


int node_GetInt(node *n)
{
  return(*(int*)n->value);
}

float node_GetFloat(node *n)
{
    return(*(float*)n->value);
}

double node_GetDouble(node *n)
{
    return(*(double*)n->value);
}

unsigned char node_GetUint8(node *n)
{
    return(*(unsigned char*)n->value);
}

unsigned short node_GetUint16(node *n)
{
    return(*(unsigned short*)n->value);
}

unsigned long node_GetUint32(node *n)
{
    return(*(unsigned long*)n->value);
}

unsigned long long node_GetUint64(node *n)
{
    return(*(unsigned long long*)n->value);
}

char node_GetSint8(node *n)
{
    return(*(char*)n->value);
}

short node_GetSint16(node *n)
{
    return(*(short*)n->value);
}

long node_GetSint32(node *n)
{
    return(*(long*)n->value);
}

long long node_GetSint64(node *n)
{
    return(*(long long*)n->value);
}

char *node_GetString(node *n)
{
    return((char*)n->value);
}

int node_GetBool(node *n)
{
    return(*(unsigned char*)n->value);
}

void node_SetBool(node *n, int b)
{
  if(node_IsType(n,NODE_TYPE_BOOL))
     *(int*)n->value = b;
  else
  {
     node_FreeValue(n->type,n->value);
     n->value = node_CreateValue(NODE_TYPE_BOOL,&b);
     node_SetType(n,NODE_TYPE_BOOL);
  }
}

void node_SetInt(node *n, int i)
{
  if(node_IsType(n,NODE_TYPE_INT))
     *(int*)n->value = i;
  else
  {
     node_FreeValue(n->type,n->value);
     n->value = node_CreateValue(NODE_TYPE_INT,&i);
     node_SetType(n,NODE_TYPE_INT);
  }
}

void node_SetFloat(node *n,float f)
{
  if(node_IsType(n,NODE_TYPE_FLOAT))
     *(float*)n->value = f;
  else
  {
     node_FreeValue(n->type,n->value);
     n->value = node_CreateValue(NODE_TYPE_FLOAT,&f);
     node_SetType(n,NODE_TYPE_FLOAT);
  }
}

void node_SetDouble(node *n,double d)
{
  if(node_IsType(n,NODE_TYPE_DOUBLE))
     *(double*)n->value = d;
  else
  {
     node_FreeValue(n->type,n->value);
     n->value = node_CreateValue(NODE_TYPE_DOUBLE,&d);
     node_SetType(n,NODE_TYPE_DOUBLE);
  }
}

void node_SetString(node *n,char *s)
{
  if(node_IsType(n,NODE_TYPE_STRING))
     n->value = s;
  else
  {
     node_FreeValue(n->type,n->value);
     n->value = node_CreateValue(NODE_TYPE_STRING,s);
     node_SetType(n,NODE_TYPE_STRING);
  }
}

void node_SetUint8(node *n,unsigned char c)
{
  if(node_IsType(n,NODE_TYPE_UINT8))
     *(unsigned char*)n->value = c;
  else
  {
     node_FreeValue(n->type,n->value);
     n->value = node_CreateValue(NODE_TYPE_UINT8,&c);
     node_SetType(n,NODE_TYPE_UINT8);
  }
}

void node_SetUint16(node *n,unsigned short s)
{
  if(node_IsType(n,NODE_TYPE_UINT16))
     *(unsigned short*)n->value = s;
  else
  {
     node_FreeValue(n->type,n->value);
     n->value = node_CreateValue(NODE_TYPE_UINT16,&s);
     node_SetType(n,NODE_TYPE_UINT16);
  }
}

void node_SetUint32(node *n,unsigned long l)
{
  if(node_IsType(n,NODE_TYPE_UINT32))
     *(unsigned long*)n->value = l;
  else
  {
     node_FreeValue(n->type,n->value);
     n->value = node_CreateValue(NODE_TYPE_UINT32,&l);
     node_SetType(n,NODE_TYPE_UINT32);
  }
}

void node_SetUint64(node *n,unsigned long long ll)
{
  if(node_IsType(n,NODE_TYPE_UINT64))
     *(unsigned long long*)n->value = ll;
  else
  {
     node_FreeValue(n->type,n->value);
     n->value = node_CreateValue(NODE_TYPE_UINT64,&ll);
     node_SetType(n,NODE_TYPE_UINT64);
  }
}

void node_SetSint8(node *n,char c)
{
  if(node_IsType(n,NODE_TYPE_SINT8))
     *(char*)n->value = c;
  else
  {
     node_FreeValue(n->type,n->value);
     n->value = node_CreateValue(NODE_TYPE_SINT8,&c);
     node_SetType(n,NODE_TYPE_SINT8);
  }
}

void node_SetSint16(node *n,short s)
{
  if(node_IsType(n,NODE_TYPE_SINT16))
     *(short*)n->value = s;
  else
  {
     node_FreeValue(n->type,n->value);
     n->value = node_CreateValue(NODE_TYPE_SINT16,&s);
     node_SetType(n,NODE_TYPE_SINT16);
  }
}

void node_SetSint32(node *n,long l)
{
  if(node_IsType(n,NODE_TYPE_SINT32))
     *(long*)n->value = l;
  else
  {
     node_FreeValue(n->type,n->value);
     n->value = node_CreateValue(NODE_TYPE_SINT32,&l);
     node_SetType(n,NODE_TYPE_SINT32);
  }
}

void node_SetSint64(node *n,long long ll)
{
  if(node_IsType(n,NODE_TYPE_SINT64))
     *(long long*)n->value = ll;
  else
  {
     node_FreeValue(n->type,n->value);
     n->value = node_CreateValue(NODE_TYPE_SINT64,&ll);
     node_SetType(n,NODE_TYPE_SINT64);
  }
}


long node_AddItem(node *n,node *s)
{
  return(list_Push(n->items,s));
}

int node_RemoveItem(node *n,node *s)
{
  return(list_RemoveItem(n->items,s));
}

int node_RemoveItemByIndex(node *n,long index)
{
  return(list_Remove(n->items,index));
}

int node_RemoveItemByKey(node *n,char *key)
{
  node *r = node_GetItemByKey(n,key);
  return(node_RemoveItem(n,r));
}

long node_GetItemIndex(node *n,node *s)
{
  return(list_GetIndex(n->items,s));
}

void *node_GetItem(node *n,long index)
{
  return(list_Get(n->items,index));
}

long node_GetItemsNum(node *n)
{
  return(list_GetLen(n->items));
}

void *node_GetItemByKey(node *n,char *key)
{
  void *item = NULL;
  long old_index = node_GetItemIterationIndex(n);
  node_ItemIterationReset(n);
  while(node_ItemIterationUnfinished(n))
  {
    node *i = node_ItemIterate(n);
    //printf("iteration:%d:%s\n",n->items->iteration_index,i->key);
    if(!strcmp(i->key,key))
    {
       //printf("key found\n");
       item = i;
       break;
    }
  }
  node_SetItemIterationIndex(n,old_index);
  return(item);
}

void node_FreeItems(node *n)
{


}

void node_ClearItems(node *n)
{
  list_Clear(n->items);
}

void *node_ItemIterate(node *n)
{
  return(list_Iterate(n->items));
}

int node_ItemIterationUnfinished(node *n)
{
  return(list_IterationUnfinished(n->items)); 
}

void node_ItemIterationReset(node *n)
{
  list_IterationReset(n->items);
}

long node_GetItemIterationIndex(node *n)
{
  return(list_GetIterationIndex(n->items));
}

void node_SetItemIterationIndex(node *n,long iteration_index)
{
  list_SetIterationIndex(n->items,iteration_index);
}


