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
  unsigned long long value;
  struct _node *parent;
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
 * node_CreateFilled() - Creates a Node and sets the most important members
 * @n: Node to use
 * @parent: Pointer to Parent Node
 * @key: String used as key
 * @value: Value for the 8 bytes value data storage
 * @type: Type of the Node
 * @items: list of items to use as sub member of the node
 *
 * Creates a value filled node.
 *
 *
 * Return: pointer to the new value filled node
 */
node *node_CreateFilled(node *parent,char *key,unsigned long long value,unsigned char type,list *items);

/**
 * node_CopyValue() - Copies a Value and returns it
 * @n: Node to use
 *
 * Copies a value. (used internally during node_CopyTree)
 *
 * Return: the copied value as unsigned long
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
 *  Frees up the memory used by the string,binary and array types
 *  Note: user types won't be freed
 *
 */
void node_FreeValue(unsigned char type,unsigned long long value);

/**
 * node_Copy() - Copy a node
 * @n: Node to use
 * @copy_value: set to True if value should be copied too
 * 
 * a single node will be copied with items references included
 * the value will only be copied if copy_value was set to True
 *
 * Return: copy of node n
 */
node *node_Copy(node *n,BOOL copy_value); //TODO add copy_items list parameter 

/**
 * node_CopyTree() - Copy a Node-Tree
 * @n: Node to use
 * @copy_calues: set to True if all values should be copied too
 * @update_parents: set to True if you want to create a completely 
 *  seperate tree with all parents correctly set
 *
 * a whole new tree copy will be created with all parents updated
 * if update_parents was set to True and all values copied
 * if copy_values was set to True
 *
 * Return: node tree copy of node n
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
 * @value: 8 byte data store value
 * @copy_value: set to True to make a true copy of the value
 * @free_old_value: set to True to automatically free any memory used by currently set value
 *
 * Sets the node value to a new value.
 * if copy_value was set to True a true copy of the value will be created and set as node value
 * if free_old_value was set to True the old value of the node will be freed up
 */
void node_SetValue(node *n,unsigned long long value,BOOL copy_value,BOOL free_old_value);

/**
 * node_SetValueType() - Manually sets the 8 byte data storage to a new value and sets the type 
 * @n: Node to use
 * @type: type used as new node type
 * @value: 8 byte data store value
 * @copy_value: set to True to make a true copy of the value
 * @free_old_value: set to True to automatically free any memory used by currently set value
 *
 * Sets the node value and type.
 * if copy_value was set to True a true copy of the value will be created and set as node value
 * if free_old_value was set to True the old value of the node will be freed up
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
 * update the type of the node
 * Note: doesnt change anything else in the node
 *
 */
void node_SetType(node *n,unsigned char type);

/**
 * node_SetParent() - Sets the node parent node
 * @n: Node to use
 * @p: new parent node
 *
 * update the node parent pointer
 */
void node_SetParent(node *n,node *p);

/**
 * node_SetItems() - Sets the node items list
 * @n: Node to use
 * @items: new items list
 * 
 * updates the node items list
 * Note: you dont need to manually free the list
 * if the node is freed normally
 *
 */
void node_SetItems(node *n, list *items);

/**
 * node_GetKey() - Gets the node key
 * @n: Node to use
 *
 * Returns the key of the node 
 *
 * Return: key string
 */
char *node_GetKey(node *n);

/**
 * node_GetValue() - Gets the node value
 * @n: Node to use
 *
 * Returns the data storage value of the node
 *
 * Return: 8 byte data store value
 */
unsigned long long node_GetValue(node *n);

/**
 * node_GetType() - Gets the node type
 * @n: Node to use
 *
 * Returns the node type (see top for type defines)
 *
 * Return: type as unsigned char
 */
unsigned char node_GetType(node *n);

/**
 * node_IsType() - Check node type against specified type
 * @n: Node to use
 * @type: type the compare with
 * 
 * Compares node type with the specified type 
 * and returns True if types match
 *
 * Return: int set to 1 if the types are the same
 */
int node_IsType(node *n, unsigned char type);

/**
 * node_GetParent() - Gets the node parent
 * @n: Node to use
 *
 * Returns the node parent pointer ,
 * can be NULL
 *
 * Return: parent node
 */
node *node_GetParent(node *n);

/**
 * node_GetItems() - Gets the node item list
 * @n: Node to use
 *
 * Returns the node item list pointer
 *
 * Return: item list
 */
list *node_GetItems(node *n);

/**
 * node_HasKey() - Checks if the node has a key string set
 * @n: Node to use
 *
 * Returns True if the node has a key string specified
 *
 * Return: int set to True if node has a key 
 */
int node_HasKey(node *n);


/*int node_HasValue(node *n);*/ //TODO

/**
 * node_GetRoot() - Gets the node trees root node
 * @n: Node to use
 *
 * the recursive function returns the root node pointer 
 * of the tree (parents chain)
 *
 * Return: top node 
 */
node *node_GetRoot(node *n);

/**
 * node_StringPrint() - Prints node contents to a string
 * @n: Node to use
 * @with_key: print the node key (if any) if True
 * @include_items: print sub items too if True
 * 
 * Prints the node contents to a newly created string
 * adds meta data if specified
 *
 * Return: new string 
 */
char *node_StringPrint(node *n,int with_key,int include_items);

/**
 * node_Print() - Prints the node to STDOUT
 * @n: Node to use
 * @with_key: print the key string if True
 * @include_items: print sub items if True
 *
 * Prints the node contents to STDOUT
 * outputs meta data if specified
 */
void node_Print(node *n,int with_key,int include_items);

/**
 * node_PrintWithTabs() - Prints the tabs formatted node to STDOUT
 * @n: Node to use
 * @with_key: print the key string if True
 * @tabs_num: number of tabs in front of each line
 *
 * Prints the node contents to STDOUT
 * using tabs_num tabs in front of each line
 * outputs meta data if specified
 */
void node_PrintWithTabs(node *n,int with_key,int tabs_num);

/**
 * node_PrintTree() - Prints the node tree to STDOUT
 * @n: Node to use
 *
 * Prints the node tree contents to STDOUT
 * outputs all node meta data 
 */
void node_PrintTree(node *n);

/**
 * node_ParseNumber() - Parses a string
 * @n: Node to use
 * @number_string: string to parse for numbers
 * 
 * Sets the value of node n to the parsed number
 * floats and integers formats are supported
 * if no number was found the node will be set
 * to the string
 */
void node_ParseNumber(node *n,char *number_string);


/*tree access*/

/**
 * node_AddItem() - Adds node to items list
 * @n: Node to use
 * @s: node to add
 * 
 * Appends a node to the node sub items list
 * and returns the index position
 *
 * Return: index of added item
 */
long node_AddItem(node *n,node *s);

/**
 * node_InsertItem() - Insert node into items list
 * @n: Node to use
 * @s: node to insert
 * @index: index at which the the node will be put
 *
 * Inserts a node at a specified index into the node 
 * sub items list
 */
void node_InsertItem(node *n,node *s,long index);

/**
 * node_RemoveItem() - Removes an item
 * @n: Node to use
 * @s: Node to remove
 * 
 * Removes the specified node from the nodes
 * sub items list and returns the index position
 * of the removed node
 *
 * Return: index of removed node
 */
int node_RemoveItem(node *n,node *s);

/**
 * node_RemoveItemByIndex() - Removes an item specified by the index
 * @n: Node to use
 * @index: index position of the node to be removed
 * 
 * Removes the node at index from the node
 * sub items list and returns a pointer to
 * the removed node
 *
 * Return: removed node pointer
 */
node *node_RemoveItemByIndex(node *n,long index);

/**
 * node_RemoveItemByKey() - Removes an item specified by the key
 * @n: Node to use
 * @key: key string of the node to be removed
 * 
 * Removes the first node with the specified
 * key string from the node sub items list and 
 * returns the index at which the node was found
 * 
 * Return: index of removed node
 */
int node_RemoveItemByKey(node *n,char *key);

/**
 * node_GetItemIndex() - Gets the item index
 * @n: Node to use
 * @s: node to get the index for
 * 
 * Gets the items index and returns it
 * if s was not found in the sub items list
 * -1 will be returned
 *
 * Return: index of node
 */
long node_GetItemIndex(node *n,node *s);

/**
 * node_GetItem() - Get node at index
 * @n: Node to use
 * @index: position of the node to be returned
 * 
 * Gets the node at index in the node 
 * sub items list and returns it
 * if the index is out of range NULL is returned
 * 
 * Return: node pointer or NULL
 */
void *node_GetItem(node *n,long index); //TODO return node*

/**
 * node_GetItemsNum() - Gets the number of items
 * @n: Node to use
 *
 * Returns the number of items in the
 * node sub items list
 *
 * Return: number of items
 */
long node_GetItemsNum(node *n);

/**
 * node_HasItems() - Check if node has items
 * @n: Node to use
 *
 * Check if the node has any items in its
 * sub items list and returns True if it has
 *
 * Return: True if node has sub items
 */
int node_HasItems(node *n);

/**
 * node_GetItemByKey() - Gets the node the specified key
 * @n: Node to use
 * @key: key string to search for
 * 
 * Searches for the first node with the same key
 * if none was found it returns NULL
 * 
 * Return: node pointer or NULL
 */
node *node_GetItemByKey(node *n,char *key);

node *node_FlatGetItemByKey(node *n,char *key);

node *node_RecursiveGetItemByKey(node *n,char *key);

node *node_IndexedGetItemByKey(node *n,char *key,long start_index);

void *node_GetItemByPath(node *n,char *key_path);

/**
 * node_HasItem() - Check s if the node has the item in its list
 * @n: Node to use
 * @s: node to search for
 *  
 * checks if the node is found in the node 
 * sub items list and returns True if it is.
 * 
 * Return: True if the node was found
 */
int node_HasItem(node *n,node *s);

/**
 * node_ClearItems() - Clears the items list
 * @n: Node to use
 *
 * Clears the items list
 * Note: items will not be freed
 * nor their parents updated
 */
void node_ClearItems(node *n);

/**
 * node_ItemIterate() - Iterate through all Items
 * @n: Node to use
 *
 * Iterates through all sub items contained in the items list of n
 * returns the next node as pointer
 * increments the nodes iteration index member
 *
 * Return: Node Pointer
 */
node *node_ItemIterate(node *n);

/**
 * node_ItemReverseIterate() - Iterate through all Items in reverse
 * @n: Node to use
 *
 * Iterates through all sub items contained in the items list of n
 * returns the previous node as pointer
 * decrements the nodes iteration index member
 *
 * Return: node pointer of iteration item
 */
node *node_ItemReverseIterate(node *n);

/**
 * node_ItemPeek() - Peek the next item
 * @n: Node to use
 *
 * Temporarily iterates further and returns
 * the item
 *
 * Return: node pointer of next iteration item
 */
node *node_ItemPeek(node *n);

/**
 * node_ItemPeekFurther() - Peeks the next item further
 * @n: Node to use
 *
 * Temporarily iterates even further and returns
 * the item
 *
 * Return: node pointer of peeked iteration item
 */
node *node_ItemPeekFurther(node *n,long offset);


/**
 * node_ItemIterationUnfinished() - Checks if the iteration has finished
 * @n: Node to use
 *
 * Checks if the item iteration has completed
 * and returns True if it has
 *
 * Return: True if the iteration has finished
 */
int node_ItemIterationUnfinished(node *n);

/**
 * node_ItemReverseIterationUnfinished() - Checks if the reverse iteration has finished
 * @n: Node to use
 *
 * Checks if the item reverse iteration has completed
 * and returns True if it has
 *
 * Return: True if the reverse iteration has finished
 */
int node_ItemReverseIterationUnfinished(node *n);

/**
 * node_ItemIterationReset() - Resets the item iteration
 * @n: Node to use
 *
 * Sets the iteration index to 0
 */
void node_ItemIterationReset(node *n);

/**
 * node_ItemReverseIterationReset() - Resets the reverse item iteration
 * @n: Node to use
 *
 * Sets the iteration index to items num - 1
 */
void node_ItemReverseIterationReset(node *n);

/**
 * node_GetItemIterationIndex() - Gets the node item iteration index
 * @n: Node to use
 *
 * Returns the actual item iteration index
 *
 * Return: index of iteration
 */
long node_GetItemIterationIndex(node *n);

/**
 * node_SetItemIterationIndex() - Sets the item iteration index
 * @n: Node to use
 * @iteration_index: new item iteration index value
 *
 * Sets the item iteration index to a new index value
 */
void node_SetItemIterationIndex(node *n,long iteration_index);

/**
 * node_FreeItems() - Frees all sub items including values
 * @n: Node to use
 *
 * Frees all sub items including their values
 * if the sub item is a tree the tree will
 * also be freed
 * Note: the containing node will not be freed
 */
void node_FreeItems(node *n);

/**
 * node_FreeTree() - Frees a node tree
 * @n: Node to use
 *
 * Frees the node tree including all values
 *
 */
void node_FreeTree(node *n);


/*standard value types quick access*/

/**
 * node_GetInt() - Gets the node value as int
 * @n: Node to use
 *
 * returns the node value as int
 *
 * Return: int
 */
int node_GetInt(node *n);

/**
 * node_GetFloat() - Get the node value as float
 * @n: Node to use
 *
 * returns the value as float value
 *
 * Return: float value
 */
float node_GetFloat(node *n);

/**
 * node_GetDouble() - Gets the node value as double
 * @n: Node to use
 *
 * returns the value as double value
 *
 * Return: double value
 */
double node_GetDouble(node *n);

/**
 * node_GetUint8() - Gets the node value as uint8
 * @n: Node to use
 *
 * returns the value as unsigned char 
 *
 * Return: uint8 value
 */
unsigned char node_GetUint8(node *n);

/**
 * node_GetUint16() - Gets the node value uint16
 * @n: Node to use
 *
 * returns the value as unsigned short
 *
 * Return: uint16
 */
unsigned short node_GetUint16(node *n);

/**
 * node_GetUint32() - Gets the node value as uint32
 * @n: Node to use
 *
 * returns the value as unsigned long
 *
 * Return: uint32
 */
unsigned long node_GetUint32(node *n);

/**
 * node_GetUint64() - Gets the node value as uint64
 * @n: Node to use
 *
 * returns the value as unsigned long long
 *
 * Return: uint64
 */
unsigned long long node_GetUint64(node *n);

/**
 * node_GetSint8() - Gets the node value as sint8
 * @n: Node to use
 *
 * returns the value as char
 *
 * Return: sint8
 */
char node_GetSint8(node *n);

/**
 * node_GetSint16() - Gets the node value as sint16
 * @n: Node to use
 *
 * returns the value as short
 *
 * Return: sint16
 */
short node_GetSint16(node *n);

/**
 * node_GetSint32() - Gets the node value as sint32
 * @n: Node to use
 *
 * returns the value as long
 *
 * Return: sint32
 */
long node_GetSint32(node *n);

/**
 * node_GetSint64() - Gets the node value as sint64
 * @n: Node to use
 *
 * returns the node value as long long
 *
 * Return: sint64
 */
long long node_GetSint64(node *n);

/**
 * node_GetString() - Gets the node value as string
 * @n: Node to use
 *
 * returns the node value as char*
 *
 * Return: string
 */
char *node_GetString(node *n);

/**
 * node_GetBool() - Gets the node value as bool
 * @n: Node to use
 *
 * returns the node value as int
 *
 * Return: bool
 */
int node_GetBool(node *n);

/**
 * node_GetUser() - Gets the node value as user pointer
 * @n: Node to use
 *
 * returns the node value as void*
 *
 * Return: user pointer
 */
void *node_GetUser(node *n);

/**
 * node_GetNode() - Gets the node value as node
 * @n: Node to use
 *
 * returns the node value as node pointer
 *
 * Return: node pointer
 */
node *node_GetNode(node *n);

/**
 * node_SetNull() - Sets the node value to NULL
 * @n: Node to use
 *
 * Sets the node value to NULL
 * updates the type and frees the previous
 * value if needed
 */
void node_SetNull(node *n);

/**
 * node_SetBool() - Sets the node value to a bool
 * @n: Node to use
 * @b: new bool value
 *
 * Sets the node value to the bool specified
 * updates the type and frees the previous
 * value if needed
 */
void node_SetBool(node *n, int b);

/**
 * node_SetInt() - Sets the node value to an int
 * @n: Node to use
 * @i: new int value
 *
 * Sets the node value to the int specified
 * updates the type and frees the previous
 * value if needed
 */
void node_SetInt(node *n, int i);

/**
 * node_SetFloat() - Sets the node value to a float
 * @n: Node to use
 * @f: new float value
 *
 * Sets the node value to the float specified
 * updates the type and frees the previous
 * value if needed
 */
void node_SetFloat(node *n,float f);

/**
 * node_SetDouble() - Sets the node value to a double
 * @n: Node to use
 * @f: new double value
 *
 * Sets the node value to the double specified
 * updates the type and frees the previous
 * value if needed
 */
void node_SetDouble(node *n,double d);

/**
 * node_SetString() - Sets the node value to a string
 * @n: Node to use
 * @f: new string
 *
 * Sets the node value to the string specified
 * updates the type and frees the previous
 * value if needed
 * Note: the string will be copied
*/
void node_SetString(node *n,char *s);

/**
 * node_SetUint8() - Sets the node value to an uint8
 * @n: Node to use
 * @f: new uint8 value
 *
 * Sets the node value to the uint8 specified
 * updates the type and frees the previous
 * value if needed
 */
void node_SetUint8(node *n,unsigned char c);

/**
 * node_SetUint16() - Sets the node value to an uint16
 * @n: Node to use
 * @f: new uint16 value
 *
 * Sets the node value to the uint16 specified
 * updates the type and frees the previous
 * value if needed
 */
void node_SetUint16(node *n,unsigned short s);

/**
 * node_SetUint32() - Sets the node value to an uint32
 * @n: Node to use
 * @f: new uint32 value
 *
 * Sets the node value to the uint32 specified
 * updates the type and frees the previous
 * value if needed
 */
void node_SetUint32(node *n,unsigned long l);

/**
 * node_SetUint64() - Sets the node value to an uint64
 * @n: Node to use
 * @f: new uint64 value
 *
 * Sets the node value to the uint64 specified
 * updates the type and frees the previous
 * value if needed
 */
void node_SetUint64(node *n,unsigned long long ll);

/**
 * node_SetSint8() - Sets the node value to a sint8
 * @n: Node to use
 * @f: new sint8 value
 *
 * Sets the node value to the sint8 specified
 * updates the type and frees the previous
 * value if needed
 */
void node_SetSint8(node *n,char c);

/**
 * node_SetSint16() - Sets the node value to a sint16
 * @n: Node to use
 * @f: new sint16 value
 *
 * Sets the node value to the sint16 specified
 * updates the type and frees the previous
 * value if needed
 */
void node_SetSint16(node *n,short s);

/**
 * node_SetSint32() - Sets the node value to a sint32
 * @n: Node to use
 * @f: new sint32 value
 *
 * Sets the node value to the sint32 specified
 * updates the type and frees the previous
 * value if needed
 */
void node_SetSint32(node *n,long l);

/**
 * node_SetSint64() - Sets the node value to a sint64
 * @n: Node to use
 * @f: new sint64 value
 *
 * Sets the node value to the sint64 specified
 * updates the type and frees the previous
 * value if needed
 */
void node_SetSint64(node *n,long long ll);

/**
 * node_SetNode() - Sets the node value to a node
 * @n: Node to use
 * @f: new node pointer
 *
 * Sets the node value to the node specified
 * updates the type and frees the previous
 * value if needed
 */
void node_SetNode(node *n,node *dst);

/**
 * node_SetUser() - Sets the node value to a user pointer
 * @n: Node to use
 * @f: new user pointer
 *
 * Sets the node value to the user pointer specified
 * updates the type and frees the previous
 * value if needed
 */
void node_SetUser(node *n,void *user);


/*special types*/

/**
 * node_CreateArray() - Creates an array
 * @n: Node to use
 *
 * Creates an array and returns it
 *
 * Return: node_array special type
 */
node_array *node_CreateArray(long num);

/**
 * node_FreeArray() - Frees an array
 * @n: Node to use
 * @free_nodes: True to free items
 * 
 * Frees the array and the items
 * if specified
 */
void node_FreeArray(node_array *array,BOOL free_nodes);

/**
 * node_CopyArray() - Copies an array
 * @n: Node to use
 * @copy_values: True to copy values
 * 
 * Copies an array and its values
 * if specified.The new array will
 * be returned
 *
 * Return: node_array
 */
node_array *node_CopyArray(node_array *array,BOOL copy_values);



/**
 * node_CreateBinary() - Creates a binary
 * @n: Node to use
 * @binary: pointer to memory
 * @len: length of memory chunk
 *
 * Creates a binary for the memory chunk specified
 * doesnt copy the chunk
 *
 * Return: node_binary special type
 */
node_binary *node_CreateBinary(void *binary,unsigned long len);

/**
 * node_CopyBinary() - Creates a binary with a memory chunk copy
 * @n: Node to use
 * @binary: pointer to memory
 * @len: length of memory chunk
 *
 * Creates a binary for the memory chunk specified
 * the memory chunk will be copied
 *
 * Return: node_binary special type
 */
node_binary *node_CopyBinary(void *binary,unsigned long len);

/**
 * node_FreeBinary() - Frees a binary
 * @n: Node to use
 * @free_value: True if memory chunk should be freed
 * 
 * Frees the binary and if specified the memory chunk it
 * points to
 */
void node_FreeBinary(node_binary *binary,BOOL free_value);

/**
 * node_SetBinary() - Sets the node value to a binary 
 * @n: Node to use
 * @binary: memory pointer
 * @len: length of the memory chunk
 * 
 * Sets the node value to the memory chunk specified
 * updates the type and frees the previous
 * value if needed
 * Note: doesnt copy the memory chunk
 */
void node_SetBinary(node *n,void *binary,unsigned long len);

/**
 * node_GetBinary() - Gets the binaries the memory pointer
 * @n: Node to use
 *
 * Returns the memory pointer of the binary
 *
 * Return: pointer to memory chunk
 */
void *node_GetBinary(node *n);

/**
 * node_GetBinaryLength() - Gets the binaries memory length
 * @n: Node to use
 *
 * Returns the length of the memory chunk
 *
 * Return: memory length
 */
unsigned long node_GetBinaryLength(node *n);


//node_stub *node_CreateStub();//TODO
//void node_FillStub(node *n);
//void node_FreeStub();


/*node array access*/

/**
 * node_SetArray() - Sets the node value to an array 
 * @n: Node to use
 * @num: length of array
 * 
 * Create an array with the specified number of items
 * and sets the node value to the array created
 * updates the type and frees the previous
 * value if needed
 * Note: creates a new array
 */
void node_SetArray(node *n,long num);

/**
 * node_array_Add() - add an item to the array
 * @n: Node to use
 * @s: item to be added
 * 
 * Appends the item to array
 *
 * Return: index of array position where the item was added to
 */
long node_array_Add(node *n,node *s);

/**
 * node_array_Remove() - removes an item from the array
 * @n: Node to use
 * @index: position of the array item to be removed
 * 
 * Removes an array item at the index position
 * and returns the pointer to it
 * if the index was out of range it returns NULL
 * Return: 
 */
node *node_array_Remove(node *n,long index);

/**
 * node_array_Get() - Gets the array item at the index position
 * @n: Node to use
 * @index: position of the array item to be returned
 * 
 * Returns the array item at the index position
 * if the index was out of range NULL will be returned
 * Return: node pointer or NULL
 */
node *node_array_Get(node *n,long index);

/**
 * node_array_GetNum() - Gets the number of array items 
 * @n: Node to use
 *
 * Returns the number of array items
 *
 * Return: items num
 */
long node_array_GetNum(node *n);

/**
 * node_array_Clear() - Clears the array
 * @n: Node to use
 *
 * Clears the array.
 * Note: doesnt free array items
 */
void node_array_Clear(node *n);

/**
 * node_array_Iterate() - Iterate through all array Items
 * @n: Node to use
 *
 * Iterates through all array items 
 * returns the next node as pointer
 * increments the nodes array iteration index member
 *
 * Return: Node Pointer
 */
node *node_array_Iterate(node *n);

/**
 * node_array_IterationUnfinished() - Checks if the array iteration has finished
 * @n: Node to use
 *
 * Returns True if the array iteration has completed
 *
 * Return: True if the array iteration has completed
 */
int node_array_IterationUnfinished(node *n);

/**
 * node_array_IterationReset() - Resets the array iteration
 * @n: Node to use
 *
 * Sets the array iteration index to 0
 */
void node_array_IterationReset(node *n);

/**
 * node_array_ReverseIterate() - Reverse Iterate through all array Items
 * @n: Node to use
 *
 * Reverse iterates through all sub items contained in the items list of n
 * returns the next node as pointer
 * decrements the nodes array iteration index member
 *
 * Return: Node Pointer
 */
node *node_array_ReverseIterate(node *n);

/**
 * node_array_ReverseIterationUnfinished() - Checks if the reverse array iteration has finished
 * @n: Node to use
 *
 * Returns True if the reverse array iteration has finished
 *
 * Return: True if the reverse array iteration has finished
 */
int node_array_ReverseIterationUnfinished(node *n);

/**
 * node_array_ReverseIterationReset() - Resets the array reverse iteration
 * @n: Node to use
 *
 * Sets the array iteration index to number of array items - 1
 */
void node_array_ReverseIterationReset(node *n);

/**
 * node_array_GetIterationIndex() - Gets the array iteration index
 * @n: Node to use
 *
 * Returns the actual array iteration index
 *
 * Return: array iteration index
 */
long node_array_GetIterationIndex(node *n);

/**
 * node_array_SetIterationIndex() - Sets the array iteration index
 * @n: Node to use
 * @iteration_index: the new array iteration index value
 * 
 * Sets the array iteration index value
 */
void node_array_SetIterationIndex(node *n,long iteration_index);



#ifdef __cplusplus
} 
#endif

#endif
