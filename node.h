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


#ifdef USE_MEMORY_DEBUGGING
#include "memory.h"
#endif


#include "list.h"
#include "strings.h"


//#include <wchar.h>

//#define USE_WHOLE_NODE_FUNCTION_SET ;TODO Later filter some functions to get even smaller code




#ifdef __cplusplus
extern "C"  {
#endif


//#define USE_FNV_HASHES


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
#define NODE_TYPE_USER 18


/**
 * struct _node - the basic node structure
 * @key: key string 
 * @value: 8 bytes data storage,
 *    used for most value types
 * @parent: pointer to the node parent
 * @type: node value type
 * @items: list of sub items of the node
 * @tag: tag pointer (user defined meaning)
 * @key_hash: if used holds the key hash
 *    for quicker access. 
 *
 * This structure represents the basic node of our tree structure,
 * it can contain sub nodes and a parent link to form the tree.
 */
typedef struct _node
{
  char *key;
  //void *value; /*TODO make sure its 4byte wide on every platform*/
  //long value;
  unsigned long long value;
  struct _node *parent;
  //int type;
  unsigned char type;
  list *items;
  void *tag;
  #ifdef USE_FNV_HASHES
  unsigned long key_hash;
  list *sorted_items;
  #endif
  //int is_dirty;//TODO add handling of value updates
} node;

/**
 * struct _node_array - additional value structure
 * @nodes: list of nodes in this array
 *
 * Special value type used to store array information in the tree
 */
typedef struct _node_array 
{
  list *nodes;
} node_array;

/*typedef struct _node_stub 
{
  void *tag;
} node_stub;
*/

/**
 * struct _node_binary - additional value structure
 * @value: pointer to data blob
 * @len: length of data blob
 *
 * Special value type used to store binary data in the tree
 */
typedef struct _node_binary 
{
  char *value;
  unsigned long len;
} node_binary;



/*key hashing*/
#ifdef USE_FNV_HASHES
#define NODE_FNV_OFFSET 2166136261
#define NODE_FNV_PRIME 16777619

/**
 * node_ComputeHash() - Computes a FNV hash for the input string
 * @string: string to compute the Hash for
 *
 * Internal function to compute hashes for node keys
 *
 * Return: 32bit hash word
 */
unsigned long node_ComputeHash(char *string);

/**
 * node_GetKeyHash() - Returns the FNV hash of the node
 * @n: Node to use
 *
 * function for easier access of the node key_hash member
 *
 * Return: 32bit hash word of node key
 */
unsigned long node_GetKeyHash(node *n);

/**
 * node_SetKeyHash() - Set the FNV hash of the node
 * @n: Node to use
 * @key_hash: 32 bit hash word
 * 
 * Sets the key_hash member of the node
 */
void node_SetKeyHash(node *n,unsigned long key_hash);

/**
 * node_GetItemByKeyHash() - Gets the Member with matching key_hash
 * @n: Node to use
 * @key_hash: 32 bit hash word
 *
 * Searches the node sub items for a member with the
 * same key_hash (only the first matching member will be returned)
 *
 * Return: Node with corresponding key_hash or NULL if none was found
 */
void *node_GetItemByKeyHash(node *n,unsigned long key_hash);

//void node_SetGlobalHashMode(int mode);


#endif



/*basic node management*/

/**
 * node_Create() - Creates an empty Node
 *
 * Creates a Node and initializes it with
 * Null values (empty key / value  / items / parent and tag)
 * and sets the type to NODE_TYPE_NULL
 *
 * Return: pointer to the newly created node
 */
node *node_Create(void);

/**
 * node_CreateFilled() - Creates a Node and setting the most important members
 * @n: Node to use
 * @parent: Pointer to Parent Node
 * @key: String used as key
 * @value: Value for the 8 bytes value data storage
 * @type: Type of the Node
 * @items: list of items to use as sub member of the node
 *
 *
 *
 *
 * Return: pointer to the new value filled node
 */
node *node_CreateFilled(node *parent,char *key,unsigned long long value,unsigned char type,list *items);

/**
 * node_() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
unsigned long node_CopyValue(node *n);

/**
 * node_Free() - free memory used by the node
 * @n: Node to use
 * @free_value: boolean value should be set if the value should also be freed
 *
 * Frees up the memory used by the node
 * if free_value is set to True the value will also
 * be freed if using any additional managed memory
 */
void node_Free(node *n,BOOL free_value);

/**
 * node_FreeValue() - Frees the memory used the by the node value if applicable
 * @n: Node to use
 * @type: node type
 * @value: 8 bytes data storage value
 *
 *  
 *
 */
void node_FreeValue(unsigned char type,unsigned long long value);

/**
 * node_() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
node *node_Copy(node *n,BOOL copy_value);

/**
 * node_() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
node *node_CopyTree(node *n,BOOL copy_values,BOOL update_parents);


/*basic node access*/

/**
 * node_SetKey() - Sets the node key
 * @n: Node to use
 * @key: String that will be used as new key
 * 
 * Sets the node key (frees the old key if set )
 * Note: a copy of the String will be used as key
 *
 */
void node_SetKey(node *n,char *key);

/**
 * node_SetValue() - Sets the 8 byte data storage to a new value
 * @n: Node to use
 *
 * 
 *
 */
void node_SetValue(node *n,unsigned long long value,BOOL copy_value,BOOL free_old_value);

/**
 * node_SetValueType() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetValueType(node *n,unsigned char type,unsigned long long value,BOOL copy_value,BOOL free_old_value);

/**
 * node_SetTag() - Sets the node tag
 * @n: Node to use
 * @tag: Pointer to void used as tag
 * 
 * Sets the node tag pointer to the tag provided
 *
 */
void node_SetTag(node *n,void *tag);

/**
 * node_GetTag() - Gets the node tag
 * @n: Node to use
 *
 * Returns the node tag pointer
 * Note: the tag pointer will not be freed during node_Free
 *
 * Return: tag pointer
 */
void *node_GetTag(node *n);

/**
 * node_SetType() - Sets the node type
 * @n: Node to use
 * @type: the new node type
 * 
 * 
 *
 */
void node_SetType(node *n,unsigned char type);

/**
 * node_SetParent() - Sets the node parent node
 * @n: Node to use
 *
 * 
 *
 */
void node_SetParent(node *n,node *p);

/**
 * node_SetItems() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetItems(node *n, list *items);

/**
 * node_GetKey() - Gets the node key
 * @n: Node to use
 *
 * 
 *
 */
char *node_GetKey(node *n);

/**
 * node_GetValue() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
unsigned long long node_GetValue(node *n);

/**
 * node_GetType() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
unsigned char node_GetType(node *n);

/**
 * node_IsType() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
int node_IsType(node *n, unsigned char type);

/**
 * node_GetParent() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
node *node_GetParent(node *n);

/**
 * node_GetItems() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
list *node_GetItems(node *n);

/**
 * node_HasKey() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
int node_HasKey(node *n);


/*int node_HasValue(node *n);*/

/**
 * node_GetRoot() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
node *node_GetRoot(node *n);


/**
 * node_Print() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_Print(node *n,int with_key,int include_items);

/**
 * node_PrintWithTabs() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_PrintWithTabs(node *n,int with_key,int tabs_num);

/**
 * node_PrintTree() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_PrintTree(node *n);

/**
 * node_ParseNumber() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_ParseNumber(node *n,char *number_string);


/*tree access*/

/**
 * node_AddItem() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
long node_AddItem(node *n,node *s);

/**
 * node_InsertItem() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_InsertItem(node *n,node *s,long index);

/**
 * node_RemoveItem() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
int node_RemoveItem(node *n,node *s);

/**
 * node_RemoveItemByIndex() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
node *node_RemoveItemByIndex(node *n,long index);

/**
 * node_RemoveItemByKey() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
int node_RemoveItemByKey(node *n,char *key);

/**
 * node_GetItemIndex() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
long node_GetItemIndex(node *n,node *s);

/**
 * node_GetItem() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
void *node_GetItem(node *n,long index);

/**
 * node_GetItemsNum() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
long node_GetItemsNum(node *n);

/**
 * node_HasItems() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
int node_HasItems(node *n);

/**
 * node_GetItemByKey() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
node *node_GetItemByKey(node *n,char *key);

//int node_SetItemByKey(node *n,char *key,)

/**
 * node_HasItem() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
int node_HasItem(node *n,node *s);

//void *node_GetItemByPath(node *n,...);


/**
 * node_ClearItems() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_ClearItems(node *n);

/**
 * node_ItemIterate() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
node *node_ItemIterate(node *n);

/**
 * node_ItemPeek() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
node *node_ItemPeek(node *n);

/**
 * node_ItemPeekFurther() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
node *node_ItemPeekFurther(node *n,long offset);


/**
 * node_ItemIterationUnfinished() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
int node_ItemIterationUnfinished(node *n);

/**
 * node_ItemIterationReset() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_ItemIterationReset(node *n);

/**
 * node_GetItemIterationIndex() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
long node_GetItemIterationIndex(node *n);

/**
 * node_SetItemIterationIndex() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
void node_SetItemIterationIndex(node *n,long iteration_index);

/**
 * node_FreeItems() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_FreeItems(node *n);

/**
 * node_FreeTree() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_FreeTree(node *n);


/*standard value types quick access*/

/**
 * node_GetInt() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
int node_GetInt(node *n);

/**
 * node_GetFloat() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
float node_GetFloat(node *n);

/**
 * node_GetDouble() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
double node_GetDouble(node *n);

/**
 * node_GetUint8() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
unsigned char node_GetUint8(node *n);

/**
 * node_GetUint16() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
unsigned short node_GetUint16(node *n);

/**
 * node_GetUint32() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
unsigned long node_GetUint32(node *n);

/**
 * node_GetUint64() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
unsigned long long node_GetUint64(node *n);

/**
 * node_GetSint8() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
char node_GetSint8(node *n);

/**
 * node_GetSint16() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
short node_GetSint16(node *n);

/**
 * node_GetSint32() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
long node_GetSint32(node *n);

/**
 * node_GetSint64() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
long long node_GetSint64(node *n);

/**
 * node_GetString() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
char *node_GetString(node *n);

/**
 * node_GetBool() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
int node_GetBool(node *n);

/**
 * node_GetUser() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
void *node_GetUser(node *n);

/**
 * node_GetNode() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
node *node_GetNode(node *n);

/**
 * node_SetNull() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetNull(node *n);

/**
 * node_SetBool() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetBool(node *n, int b);

/**
 * node_SetInt() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetInt(node *n, int i);

/**
 * node_SetFloat() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetFloat(node *n,float f);

/**
 * node_SetDouble() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetDouble(node *n,double d);

/**
 * node_SetString() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetString(node *n,char *s);

/**
 * node_SetUint8() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetUint8(node *n,unsigned char c);

/**
 * node_SetUint16() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetUint16(node *n,unsigned short s);

/**
 * node_SetUint32() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetUint32(node *n,unsigned long l);

/**
 * node_SetUint64() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetUint64(node *n,unsigned long long ll);

/**
 * node_SetSint8() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetSint8(node *n,char c);

/**
 * node_SetSint16() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetSint16(node *n,short s);

/**
 * node_SetSint32() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetSint32(node *n,long l);

/**
 * node_SetSint64() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetSint64(node *n,long long ll);

/**
 * node_SetNode() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetNode(node *n,node *dst);

/**
 * node_SetUser() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetUser(node *n,void *user);


/*special types*/

/**
 * node_CreateArray() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
node_array *node_CreateArray(long num);

/**
 * node_FreeArray() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_FreeArray(node_array *array,BOOL free_nodes);

/**
 * node_CopyArray() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
node_array *node_CopyArray(node_array *array,BOOL copy_values);


/*binary type -> binary chunks managed by it need to be freed on its own at the moment*/

/**
 * node_CreateBinary() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
node_binary *node_CreateBinary(void *binary,unsigned long len);

/**
 * node_CopyBinary() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
node_binary *node_CopyBinary(void *binary,unsigned long len);

/**
 * node_FreeBinary() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_FreeBinary(node_binary *binary,BOOL free_value);

/**
 * node_SetBinary() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetBinary(node *n,void *binary,unsigned long len);

/**
 * node_GetBinary() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
void *node_GetBinary(node *n);

/**
 * node_GetBinaryLength() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
unsigned long node_GetBinaryLength(node *n);


//node_stub *node_CreateStub();
//void node_FillStub(node *n);
//void node_FreeStub();


/*node array access*/

/**
 * node_SetArray() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_SetArray(node *n,long num);

/**
 * node_array_Add() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
long node_array_Add(node *n,node *s);

/**
 * node_array_Remove() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
node *node_array_Remove(node *n,long index);

/**
 * node_array_Get() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
node *node_array_Get(node *n,long index);

/**
 * node_array_GetNum() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
long node_array_GetNum(node *n);

/**
 * node_array_Clear() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_array_Clear(node *n);

/**
 * node_array_Iterate() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
node *node_array_Iterate(node *n);

/**
 * node_array_IterationUnfinished() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
int node_array_IterationUnfinished(node *n);

/**
 * node_array_IterationReset() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_array_IterationReset(node *n);

/**
 * node_array_GetIterationIndex() - 
 * @n: Node to use
 *
 * 
 *
 * Return: 
 */
long node_array_GetIterationIndex(node *n);

/**
 * node_array_SetIterationIndex() - 
 * @n: Node to use
 *
 * 
 *
 */
void node_array_SetIterationIndex(node *n,long iteration_index);



#ifdef __cplusplus
} 
#endif

#endif
