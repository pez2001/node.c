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
#define NODE_TYPE_BINARY 17


typedef struct _node
{
  char *key;
  void *value;
  struct _node *parent;
  int type;
  item_list *items;
  //int is_dirty;//TODO add handling of value updates
} node;


typedef struct _node_array 
{
  int type;
  void **value;
  unsigned long len;
} node_array;

typedef struct _node_stub 
{
  void *tag;
} node_stub;

typedef struct _node_binary 
{
  char *value;
  unsigned long len;
} node_binary;






/*utilities*/
char *node_CopyString(char *string);


/*basic node management*/
node *node_Create(void);
node *node_CreateFilled(node *parent,char *key,void *value,int type,item_list *items);
void *node_CreateValue(int type,void *value);
void node_Free(node *n,BOOL free_value);
void node_FreeValue(int type,void *value);
node *node_Copy(node *n,BOOL copy_value);

/*basic node access*/
void node_SetKey(node *n,char *key);
void node_SetValue(node *n,void *value,BOOL copy_value,BOOL free_old_value);
void node_SetValueType(node *n,int type,void *value,BOOL copy_value,BOOL free_old_value);
void node_SetType(node *n,int type);
void node_SetParent(node *n,node *p);
void node_SetItems(node *n, item_list *items);
char *node_GetKey(node *n);
void *node_GetValue(node *n);
int node_GetType(node *n);
int node_IsType(node *n, int type);
node *node_GetParent(node *n);
item_list *node_GetItems(node *n);


/*tree access*/
long node_AddItem(node *n,node *s);
int node_RemoveItem(node *n,node *s);
int node_RemoveItemByIndex(node *n,long index);
int node_RemoveItemByKey(node *n,char *key);
long node_GetItemIndex(node *n,node *s);
void *node_GetItem(node *n,long index);
long node_GetItemsNum(node *n);
void *node_GetItemByKey(node *n,char *key);
void node_ClearItems(node *n);
void *node_ItemIterate(node *n);
int node_ItemIterationUnfinished(node *n);
void node_ItemIterationReset(node *n);
long node_GetItemIterationIndex(node *n);
void node_SetItemIterationIndex(node *n,long iteration_index);

void node_FreeItems(node *n);


/*standard value types quick access*/
int node_GetInt(node *n);
float node_GetFloat(node *n);
double node_GetDouble(node *n);
unsigned char node_GetUint8(node *n);
unsigned short node_GetUint16(node *n);
unsigned long node_GetUint32(node *n);
unsigned long long node_GetUint64(node *n);
char node_GetSint8(node *n);
short node_GetSint16(node *n);
long node_GetSint32(node *n);
long long node_GetSint64(node *n);
char *node_GetString(node *n);
int node_GetBool(node *n);

void node_SetBool(node *n, int b);
void node_SetInt(node *n, int i);
void node_SetFloat(node *n,float f);
void node_SetDouble(node *n,double d);
void node_SetString(node *n,char *s);
void node_SetUint8(node *n,unsigned char c);
void node_SetUint16(node *n,unsigned short s);
void node_SetUint32(node *n,unsigned long l);
void node_SetUint64(node *n,unsigned long long ll);
void node_SetSint8(node *n,char c);
void node_SetSint16(node *n,short s);
void node_SetSint32(node *n,long l);
void node_SetSint64(node *n,long long ll);


/*special types*/

node_array *node_CreateArray(long num);
node_stub *node_CreateStub();
node_binary *node_CreateBinary(unsigned long,char *binary);

void node_FreeArray();
void node_FreeStub();
void node_FreeBinary();

void node_FillStub(node *n);






#ifdef __cplusplus
} 
#endif


#endif