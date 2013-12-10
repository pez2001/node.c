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



typedef struct
{
char *key;
void *value;
int type;
items_list *items;
} node;



#ifdef __cplusplus
} 
#endif


#endif