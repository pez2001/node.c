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
  n->key = NULL;
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
    case NODE_TYPE_BINARY:
         r = (void*)node_CreateBinary(((node_binary*)value)->value,((node_binary*)value)->len);
         break;
    default:
         break;
  }
  return(r);
}

void node_Free(node *n,BOOL free_value)
{
  if(free_value)
  {
    node_FreeValue(n->type,n->value);
  }
  if(n->key != NULL)
    free(n->key);
  node_FreeItems(n);
  list_Close(n->items);
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
         node_FreeArray(value,True);
         break;
    case NODE_TYPE_NODE:
         break;
    case NODE_TYPE_STUB:
         break;
    case NODE_TYPE_BINARY:
         node_FreeBinary((node_binary*)value,False); //TODO recheck freeing style
         break;
    default:
         break;
  }
}


void node_ParseNumber(node *n,char *number_string)
{
  double d = atof(number_string);
  long l = (long)d;
  if(d!=0.0f && (d==(double)l))
  {
    node_SetSint32(n,l);
  }else
  if(d==0.0f)
  {
    long l = atol(number_string);
    node_SetSint32(n,l);
  }
  else 
  {
    node_SetDouble(n,d);
  }
}


void node_print_tabs(int num)
{
  for(int i=0;i<num;i++)
   printf("\t");
}


void node_PrintWithTabs(node *n,int with_key,int tabs_num)
{
  if(with_key && node_HasKey(n) != NULL)
  {
    node_print_tabs(tabs_num);
    printf("%s",n->key);
    printf(" = ");
  }

  switch(n->type)
  {
    case NODE_TYPE_NULL:
         if(with_key)
         {
           printf("null");
         } 
         break;
    case NODE_TYPE_INT:
         printf("%d",*(int*)n->value);
         break;
    case NODE_TYPE_FLOAT:
         printf("%-7.7g",*(float*)n->value);
         break;
    case NODE_TYPE_DOUBLE:
         printf("%-7.7g",*(double*)n->value);
         break;
    case NODE_TYPE_UINT8:
         printf("%d",*(unsigned char*)n->value);
         break;
    case NODE_TYPE_SINT8:
         printf("%d",*(char*)n->value);
         break;
    case NODE_TYPE_BOOL:
         if(*(unsigned char*)n->value)
           printf("True");
         else
          printf("False");
         break;
    case NODE_TYPE_UINT16:
         printf("%d",*(unsigned short*)n->value);
         break;
    case NODE_TYPE_SINT16:
         printf("%d",*(short*)n->value);
         break;
    case NODE_TYPE_UINT32:
         printf("%d",*(unsigned long*)n->value);
         break;
    case NODE_TYPE_SINT32:
         printf("%d",*(long*)n->value);
         break;
    case NODE_TYPE_SINT64:
         printf("%d",*(long long*)n->value);
         break;
    case NODE_TYPE_UINT64:
         printf("%d",*(unsigned long long*)n->value);
         break;
    case NODE_TYPE_STRING:
         printf((char*)n->value);
         break;
    case NODE_TYPE_ARRAY:
         {
         if(!node_HasKey(n))
           node_print_tabs(tabs_num+1);
         printf("[");
         long old_index = node_array_GetIterationIndex(n);
         node_array_IterationReset(n);
         while(node_array_IterationUnfinished(n))
         { 
            node *i = node_array_Iterate(n);
            node_PrintWithTabs(i,False,tabs_num+1);
            if(node_array_IterationUnfinished(n))
            {
              //if(!node_HasKey(n))
              //  node_print_tabs(tabs_num+1);
              printf(",");
            }
         }
         //node_print_tabs(tabs_num+1);
         printf("]");
         if(!node_HasKey(n))
           printf("\n");
         node_array_SetIterationIndex(n,old_index);
         break;
         }
    case NODE_TYPE_NODE:
         if(node_HasItems(n))
         { 
           printf("{\n");
           long old_index = node_GetItemIterationIndex(n);
           node_ItemIterationReset(n);
           while(node_ItemIterationUnfinished(n))
           {
              node *i = node_ItemIterate(n);
              node_PrintWithTabs(i,True,tabs_num+1);
           }
           node_SetItemIterationIndex(n,old_index);
           node_print_tabs(tabs_num+1);
           printf("}");
         if(!node_HasKey(n))
           printf("\n");
          if(!node_HasKey(n))
            node_print_tabs(tabs_num+1);
         }
         else
          printf("{}");
         break;
    case NODE_TYPE_STUB:
         break;
    case NODE_TYPE_BINARY:
         break;
    default:
         break;
  }
  if(with_key && node_HasKey(n) != NULL)
    printf("\n");
}

void node_Print(node *n,int with_key)
{
  if(with_key)
  {
    if(n->key != NULL)
      printf("%s",n->key);
    else
      printf("[undefined]");
    printf(" = ");
  }

  switch(n->type)
  {
    case NODE_TYPE_NULL:
         break;
    case NODE_TYPE_INT:
         printf("%d",*(int*)n->value);
         break;
    case NODE_TYPE_FLOAT:
         printf("%13g",*(float*)n->value);
         break;
    case NODE_TYPE_DOUBLE:
         printf("%13g",*(double*)n->value);
         break;
    case NODE_TYPE_UINT8:
         printf("%d",*(unsigned char*)n->value);
         break;
    case NODE_TYPE_SINT8:
         printf("%d",*(char*)n->value);
         break;
    case NODE_TYPE_BOOL:
         if(*(unsigned char*)n->value)
           printf("True");
         else
          printf("False");
         break;
    case NODE_TYPE_UINT16:
         printf("%d",*(unsigned short*)n->value);
         break;
    case NODE_TYPE_SINT16:
         printf("%d",*(short*)n->value);
         break;
    case NODE_TYPE_UINT32:
         printf("%d",*(unsigned long*)n->value);
         break;
    case NODE_TYPE_SINT32:
         printf("%d",*(long*)n->value);
         break;
    case NODE_TYPE_SINT64:
         printf("%d",*(long long*)n->value);
         break;
    case NODE_TYPE_UINT64:
         printf("%d",*(unsigned long long*)n->value);
         break;
    case NODE_TYPE_STRING:
         printf((char*)n->value);
         break;
    case NODE_TYPE_ARRAY:
         {
         printf("[");
         long old_index = node_array_GetIterationIndex(n);
         node_array_IterationReset(n);
         while(node_array_IterationUnfinished(n))
         { 
            node *i = node_array_Iterate(n);
            node_Print(i,True);
            if(node_array_IterationUnfinished(n))
              printf(" , ");
         }
         printf("]\n");
         node_array_SetIterationIndex(n,old_index);
         break;
         }
    case NODE_TYPE_NODE:
         if(node_HasItems(n))
         { 
           node_print_tabs(1);
           printf("{\n");
           long old_index = node_GetItemIterationIndex(n);
           node_ItemIterationReset(n);
           while(node_ItemIterationUnfinished(n))
           {
              node *i = node_ItemIterate(n);
              node_Print(i,True);
           }
           node_SetItemIterationIndex(n,old_index);
           node_print_tabs(1);
           printf("}\n");
         }
         break;
    case NODE_TYPE_STUB:
         break;
    case NODE_TYPE_BINARY:
         break;
    default:
         break;
  }
  if(with_key)
    printf("\n");
}

void node_PrintTreeLevel(node *n,int level)
{
  node_PrintWithTabs(n,True,level);
}

void node_PrintTree(node *n)
{
  node_PrintTreeLevel(n,0);
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

int node_HasKey(node *n)
{
  return(n->key!=NULL);
}

int node_HasValue(node *n)
{
  return(n->value!=NULL);
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

void node_SetNull(node *n)
{
  //printf("setting null\n");
  if(!node_IsType(n,NODE_TYPE_NULL))
  {
     node_FreeValue(n->type,n->value);
     n->value = node_CreateValue(NODE_TYPE_NULL,NULL);
     node_SetType(n,NODE_TYPE_NULL);
  }
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
  node_FreeValue(n->type,n->value);
  n->value = node_CreateValue(NODE_TYPE_STRING,s);
  node_SetType(n,NODE_TYPE_STRING);
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

node *node_RemoveItemByIndex(node *n,long index)
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

int node_HasItems(node *n)
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
    //printf("checking:%s,%d\n",i->key,n->items->num);
    if(i->key!=NULL && !strcmp(i->key,key))
    {
       item = i;
       break;
    }
  }
  node_SetItemIterationIndex(n,old_index);
  return(item);
}

void node_FreeItems(node *n)
{
  node_ItemIterationReset(n);
  while(node_ItemIterationUnfinished(n))
  {
    node *i = node_ItemIterate(n);
    node_FreeTree(i);
  }
}

void node_FreeTree(node *n)
{
  //node_FreeItems(n);
  node_Free(n,True);
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




node_array *node_CreateArray(long num)
{
  node_array *r = (node_array*)malloc(sizeof(node_array));
  r->nodes = list_Create(num,0);
  return(r);
}

void node_FreeArray(node_array *array,BOOL free_nodes)
{
  list_IterationReset(array->nodes);
  while(list_IterationUnfinished(array->nodes))
  { 
    node *i = (node*)list_Iterate(array->nodes);
    //printf("freeing node:%s\n",i->key);
    node_Free(i,free_nodes);
  }
  list_Close(array->nodes);
  free(array);
}

void node_SetArray(node *n,long num)
{
  /*if(node_IsType(n,NODE_TYPE_ARRAY))
  {
     list_Close(((node_array*)n->value)->nodes);
     ((node_array*)n->value)->nodes = binary;
  }
  else
  {*/
  node_FreeValue(n->type,n->value);
  n->value = node_CreateArray(num);
  node_SetType(n,NODE_TYPE_ARRAY);
  //}
}

long node_array_Add(node *n,node *s)
{
  return(list_Push(((node_array*)n->value)->nodes,s));
}

node *node_array_Remove(node *n,long index)
{
  return((node*)list_Remove(((node_array*)n->value)->nodes,index));
}

node *node_array_Get(node *n,long index)
{
  return((node*)list_Get(((node_array*)n->value)->nodes,index));
}

long node_array_GetNum(node *n)
{
  return(list_GetLen(((node_array*)n->value)->nodes));
}

void node_array_Clear(node *n)
{
   list_Clear(((node_array*)n->value)->nodes);
}

node *node_array_Iterate(node *n)
{
  return((node*)list_Iterate(((node_array*)n->value)->nodes)); 
}

int node_array_IterationUnfinished(node *n)
{
  return(list_IterationUnfinished(((node_array*)n->value)->nodes));
}

void node_array_IterationReset(node *n)
{
  list_IterationReset(((node_array*)n->value)->nodes);
}

long node_array_GetIterationIndex(node *n)
{
  return(list_GetIterationIndex(((node_array*)n->value)->nodes));
}

void node_array_SetIterationIndex(node *n,long iteration_index)
{
  list_SetIterationIndex(((node_array*)n->value)->nodes,iteration_index);
}



node_binary *node_CreateBinary(char *binary,unsigned long len)
{
  node_binary *r = (node_binary*)malloc(sizeof(node_binary));
  r->value = binary;
  r->len = len;
  return(r);
}

void node_FreeBinary(node_binary *binary,BOOL free_value)
{
  if(free_value)
    free(binary->value);
  free(binary);  
}

void node_SetBinary(node *n,char *binary,unsigned long len)
{
  if(node_IsType(n,NODE_TYPE_BINARY))
  {
     ((node_binary*)n->value)->value = binary;
     ((node_binary*)n->value)->len = len;
  }
  else
  {
     node_FreeValue(n->type,n->value);
     n->value = node_CreateBinary(binary,len);
     node_SetType(n,NODE_TYPE_BINARY);
  }
}

char *node_GetBinary(node *n)
{
    return(((node_binary*)n->value)->value);
}

unsigned long node_GetBinaryLength(node *n)
{
    return(((node_binary*)n->value)->len);
}









