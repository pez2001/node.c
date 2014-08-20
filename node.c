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


#ifdef USE_FNV_HASHES

unsigned long node_ComputeHash(char *string)
{
  if(!strlen(string))
    return(0);
  int i = 0;
  unsigned long hash = NODE_FNV_OFFSET;
  while(string[i]!=0)
  {
    hash ^= string[i];
    hash *= NODE_FNV_PRIME;
    i++;
  }
  return(hash);
}

unsigned long node_GetKeyHash(node *n)
{
  return(n->key_hash);
}

void node_SetKeyHash(node *n,unsigned long key_hash)
{
  n->key_hash = key_hash;
}

void *node_GetItemByKeyHash(node *n,unsigned long key_hash)
{
  void *item = NULL;
  long old_index = node_GetItemIterationIndex(n);
  node_ItemIterationReset(n);
  while(node_ItemIterationUnfinished(n))
  {
    node *i = node_ItemIterate(n);
    if(i->key_hash!=0 && i->key_hash == key_hash)
    {
       item = i;
       break;
    }
  }
  node_SetItemIterationIndex(n,old_index);
  return(item);
}

#endif

node *node_Create(void)
{
  node *n = (node*)malloc(sizeof(node));
  n->type = 0;
  n->value = 0;
  n->parent = NULL;
  n->key = NULL;
  #ifdef USE_FNV_HASHES
  n->key_hash = 0;
  #endif
  n->items = list_Create(0,0);
  return(n);
}

node *node_CreateFilled(node *parent,char *key,unsigned long long value,unsigned char type,list *items)
{
  node *n = (node*)malloc(sizeof(node));
  n->key = str_Copy(key);
  n->value = value;
  n->type = type;
  n->items = items;
  n->parent = parent;
  #ifdef USE_FNV_HASHES
  n->key_hash = node_ComputeHash(key);
  #endif
  return(n);
}

unsigned long node_CopyValue(node *n)
{
  unsigned long long r = 0;
  switch(n->type)
  {
    case NODE_TYPE_NULL:
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
    case NODE_TYPE_NODE:
    case NODE_TYPE_USER:
         r = n->value;
         break;
    case NODE_TYPE_STRING:
         {
            char *tmp = str_Copy((char*)(unsigned long)n->value);
            r = (unsigned long long)(unsigned long)tmp;
            //printf("setting value string:[%s]\n",tmp);
            //fflush(stdout);
            //printf("conv value string:[%s]\n",(char*)r);
            //fflush(stdout);
         }
         break;
    case NODE_TYPE_ARRAY:
         {
            node_array *tmp = node_CopyArray((node_array*)(unsigned long)n->value,True);
            r = (unsigned long long)(unsigned long)tmp;
         }
         break;
    case NODE_TYPE_BINARY:
         {
           node_binary *tmp = node_CreateBinary(((node_binary*)(unsigned long)n->value)->value,((node_binary*)(unsigned long)n->value)->len);
            r = (unsigned long long)(unsigned long)tmp;
         }
         break;
    default:
         break;
  }
  return(r);
}

/*
unsigned long long node_CreateValue(unsigned char type,void *value)
{
  unsigned long long r = 0;
  switch(type)
  {
    case NODE_TYPE_NULL:
         break;
    case NODE_TYPE_INT:
         memcpy(&r, value, sizeof(int));
         break;
    case NODE_TYPE_FLOAT:
         memcpy(&r, value, sizeof(float));
    case NODE_TYPE_DOUBLE:
         memcpy(&r, value, sizeof(double));
         break;
    case NODE_TYPE_UINT8:
    case NODE_TYPE_SINT8:
    case NODE_TYPE_BOOL:
         memcpy(&r, value, sizeof(unsigned char));
         break;
    case NODE_TYPE_UINT16:
    case NODE_TYPE_SINT16:
         memcpy(&r, value, sizeof(unsigned short));
         break;
    case NODE_TYPE_UINT32:
    case NODE_TYPE_SINT32:
         memcpy(&r, value, sizeof(unsigned long));
         break;
    case NODE_TYPE_SINT64:
    case NODE_TYPE_UINT64:
         memcpy(&r, value, sizeof(unsigned long long));
         //r = (unsigned long long)*value;
         break;
    case NODE_TYPE_STRING:
         {
            char *tmp = str_Copy((char*)value);
            memcpy(&r, &tmp, sizeof(char*));
            //printf("setting value string:[%s]\n",tmp);
            //fflush(stdout);
            //printf("conv value string:[%s]\n",(char*)r);
            //fflush(stdout);
            //r = (unsigned long long)tmp;
         }
         break;
    case NODE_TYPE_ARRAY:
         {
            //r = (unsigned long long)node_CopyArray(value,True);
            node_array *tmp = node_CopyArray((node_array*)value,True);
            memcpy(&r, &tmp, sizeof(node_array*));
         }
         break;
    case NODE_TYPE_NODE:
         //r = (unsigned long long)value;
         memcpy(&r, &value, sizeof(node*));
         break;
    case NODE_TYPE_STUB:
         break;
    case NODE_TYPE_USER:
         //r = (unsigned long long)value;
         memcpy(&r, value, sizeof(void*));
         break;
    case NODE_TYPE_BINARY:
         {
           node_binary *tmp = node_CreateBinary(((node_binary*)value)->value,((node_binary*)value)->len);
           memcpy(&r, &tmp, sizeof(unsigned long long));
         }
         break;
    default:
         break;
  }
  return(r);
}
*/

void node_Free(node *n,BOOL free_value)
{
  //printf("freeing node:%x\n",n);
  //node_PrintTree(n);
  //fflush(stdout);
  //node_Print(n,True,True);
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

void node_FreeValue(unsigned char type,unsigned long long value)
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
         //if(value!=NULL)
         //  free(value);
         break;
    case NODE_TYPE_STRING:
         if(((char*)(unsigned long)value)!=NULL)
           free((char*)(unsigned long)value);
         break;
    case NODE_TYPE_ARRAY:
         if(((node_array*)(unsigned long)value)!=NULL)
           node_FreeArray((node_array*)(unsigned long)value,True);
         break;
    case NODE_TYPE_NODE:

         /*if(value!=NULL)
         {
           node_PrintTree(value);
           node_FreeTree(value);
         }*/
         break;
    case NODE_TYPE_STUB:
         break;
    case NODE_TYPE_USER:
         break;
    case NODE_TYPE_BINARY:
         if(((node_binary*)(unsigned long)value)!=NULL)
           node_FreeBinary((node_binary*)(unsigned long)value,False); //TODO recheck freeing style
         break;
    default:
         break;
  }
}

int node_count_digits(char *number_string)
{
  unsigned long i = 0;
  unsigned long len = strlen(number_string);
  unsigned long digits = 0;
  unsigned long both = 0;
  unsigned long other = 0;
  unsigned long num_e = 0;
  unsigned long white_spaces = 0;
  unsigned long leading_white_spaces = 0;
  int leading_sign=0;
  unsigned long middle_signs=0;
  while(i++<len)
   if(number_string[i]==' ')
     leading_white_spaces++;
  else
     break; 

  if(number_string[leading_white_spaces]=='+' || number_string[leading_white_spaces]=='-')
    leading_sign=1;

  i=leading_white_spaces;
  while(i<len)
  {
    switch(number_string[i])
    {
       case '0':
       case '1':
       case '2':
       case '3':
       case '4':
       case '5':
       case '6':
       case '7':
       case '8':
       case '9':
         digits++;
         break;
       case 'e':
       case 'E':
         both++;
         num_e++;
         break;
       case '+':
       case '-':
         if(i!=leading_white_spaces)
           middle_signs++;
         break;      
       case ' ':
         white_spaces++;
       case '.':
       case ',':
       case '\t':
         both++;
         break;
       default:
         other++;
         break;
    }

    i++;
  }
  if(num_e>1 || middle_signs>1 || (middle_signs>0 && num_e == 0))
    return(0);
  int lead_worth = 1;
  if(len==1)
    lead_worth = 0;
  return((leading_sign*lead_worth)+digits+both>other+both);
}

int node_NumberIsFloat(char *number_string)
{
  unsigned long i = 0;
  unsigned long len = strlen(number_string);
  while(i++<len)
  {
     if(number_string[i]==',' || number_string[i]=='e' || number_string[i]=='E' || number_string[i]=='.')
       return(True);
  }
  return(False);
}

void node_ParseNumber(node *n,char *number_string)
{
  if(!node_count_digits(number_string))
  {
    node_SetString(n,number_string);
    return;
  }
  if(node_NumberIsFloat(number_string))
  {
    double d = atof(number_string);
    node_SetDouble(n,d);
  }
  else
  {
    long long ll = atoll(number_string);
    long l = atol(number_string);
    if(l!=ll)
      node_SetSint64(n,ll);
    else
      node_SetSint32(n,l);
  }
}

void node_print_tabs(int num)
{
  for(int i=0;i<num;i++)
   printf("\t");
}

void node_PrintWithTabs(node *n,int with_key,int tabs_num)
{
  if(with_key && node_HasKey(n))
  {
    node_print_tabs(tabs_num);
    printf("(%x)%s",n,n->key);
    printf(" = ");
  }
  else
  {
    //node_print_tabs(tabs_num);
    //printf("undefined = ");
  }

  switch(n->type)
  {
    case NODE_TYPE_NULL:
         //if(with_key)
         //{
           //node_print_tabs(tabs_num);
           printf("null");
           //printf(" = ");
         //} 
         break;
    case NODE_TYPE_INT:
         printf("%d",(int)n->value);
         break;
    case NODE_TYPE_FLOAT:
         {
            float *fp = (float*)&(n->value);
            printf("%f",*fp);
         //printf("%-7.7g",*(float*)n->value)
         }
         break;
    case NODE_TYPE_DOUBLE:
         {
            double *dp = (double*)&(n->value);  
            printf("%f",*dp);
         }
         //printf("%-7.7g",*(double*)n->value);
         break;
    case NODE_TYPE_UINT8:
         printf("%u",(unsigned char)n->value);
         break;
    case NODE_TYPE_SINT8:
         printf("%d",(char)n->value);
         break;
    case NODE_TYPE_BOOL:
         if((unsigned char)n->value)
           printf("True");
         else
          printf("False");
         break;
    case NODE_TYPE_UINT16:
         printf("%u",(unsigned short)n->value);
         break;
    case NODE_TYPE_SINT16:
         printf("%d",(short)n->value);
         break;
    case NODE_TYPE_UINT32:
         printf("%u",(unsigned long)n->value);
         break;
    case NODE_TYPE_SINT32:
         printf("%d",(long)n->value);
         break;
    case NODE_TYPE_SINT64:
         printf("%I64d",(long long)n->value);
         break;
    case NODE_TYPE_UINT64:
         printf("%I64u",(unsigned long long)n->value);
         break;
    case NODE_TYPE_STRING:
         printf("\"%s\"",(char*)(unsigned long)n->value);
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
            node_PrintWithTabs(i,True,tabs_num+1);
            if(node_array_IterationUnfinished(n))
            {
              printf(",");
            }
         }
         printf("]");
         if(!node_HasKey(n))
           printf("\n");
         node_array_SetIterationIndex(n,old_index);
         break;
         }
    case NODE_TYPE_NODE:
         if(n->value != 0)
         {
           printf("(@%x)",n->value);
           //node_PrintWithTabs(n->value,True,tabs_num+1);
         }
         else 
          printf("{NULL}");
         break;
    case NODE_TYPE_STUB:
         break;
    case NODE_TYPE_USER:
         printf("user(%x)",n->value);
         break;
    case NODE_TYPE_BINARY:
         printf("binary(%d)",node_GetBinaryLength(n));
         break;
    default:
         break;
  }

  if(node_HasItems(n))
  { 
    if(!node_HasKey(n) && !with_key)
      node_print_tabs(tabs_num+1);
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
  }
  if(with_key && node_HasKey(n))
    printf("\n");
}

void node_Print(node *n,int with_key,int include_items)
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
         printf("%d",(int)n->value);
         break;
    case NODE_TYPE_FLOAT:
         {
            float *fp = (float*)&(n->value);
            printf("%f",*fp);
            //printf("%13g",*(float*)n->value);
         }
         break;
    case NODE_TYPE_DOUBLE:
         {
            double *dp = (double*)&(n->value);
            printf("%f",*dp);
            //printf("%13g",*(double*)n->value);
         }
         break;
    case NODE_TYPE_UINT8:
         printf("%u",(unsigned char)n->value);
         break;
    case NODE_TYPE_SINT8:
         printf("%d",(char)n->value);
         break;
    case NODE_TYPE_BOOL:
         if((unsigned char)n->value)
           printf("True");
         else
          printf("False");
         break;
    case NODE_TYPE_UINT16:
         printf("%u",(unsigned short)n->value);
         break;
    case NODE_TYPE_SINT16:
         printf("%u",(short)n->value);
         break;
    case NODE_TYPE_UINT32:
         printf("%u",(unsigned long)n->value);
         break;
    case NODE_TYPE_SINT32:
         printf("%d",(long)n->value);
         break;
    case NODE_TYPE_SINT64:
         printf("%I64d",(long long)n->value);
         break;
    case NODE_TYPE_UINT64:
         printf("%I64u",(long long)n->value);
         break;
    case NODE_TYPE_STRING:
         //SetConsoleOutputCP(CP_UTF8);
         printf("%s",(char*)(unsigned long)n->value);
         break;
    case NODE_TYPE_ARRAY:
         {
         printf("[");
         long old_index = node_array_GetIterationIndex(n);
         node_array_IterationReset(n);
         while(node_array_IterationUnfinished(n))
         { 
            node *i = node_array_Iterate(n);
            node_Print(i,True,include_items);
            if(node_array_IterationUnfinished(n))
              printf(" , ");
         }
         printf("]\n");
         node_array_SetIterationIndex(n,old_index);
         break;
         }
    case NODE_TYPE_NODE:
         if(include_items && node_HasItems(n))
         { 
           node_print_tabs(1);
           printf("{\n");
           long old_index = node_GetItemIterationIndex(n);
           node_ItemIterationReset(n);
           while(node_ItemIterationUnfinished(n))
           {
              node *i = node_ItemIterate(n);
              node_Print(i,True,True);
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
  n->key = str_Copy(key);
  #ifdef USE_FNV_HASHES
  n->key_hash = node_ComputeHash(key);
  #endif
}

void node_SetValue(node *n,unsigned long long value,BOOL copy_value,BOOL free_old_value)
{
  if(free_old_value)
    node_FreeValue(n->type,n->value);
  if(copy_value)
  {
    unsigned long long v = node_CopyValue(n); 
    n->value = v;
  }
  else
    n->value = value;
}

void node_SetType(node *n,unsigned char type)
{
  n->type = type;
}

void node_SetValueType(node *n,unsigned char type,unsigned long long value,BOOL copy_value,BOOL free_old_value)
{
  node_SetType(n,type);
  node_SetValue(n,value,copy_value,free_old_value);
}

void node_SetTag(node *n,void *tag)
{
  n->tag = tag;
}

void *node_GetTag(node *n)
{
  return(n->tag);
}


void node_SetItems(node *n, list *items)
{
  n->items = items;
}

void node_SetParent(node *n,node *p)
{
  n->parent = p;
}

node *node_Copy(node *n,BOOL copy_value)
{
  unsigned long long v = node_CopyValue(n); 
  list *l = list_Copy(n->items);
  node *r = node_CreateFilled(n->parent,n->key,v,n->type,l);
  return(r);
}

node *node_CopySub(node *n,BOOL copy_value)
{
  unsigned long long v = node_CopyValue(n); 
  list *l = list_Create(0,0);
  node *r = node_CreateFilled(n->parent,n->key,v,n->type,l);
  return(r);
}


void node_CopySubTree(node *n,node *parent,BOOL copy_values,BOOL update_parents)
{
  node *new = node_CopySub(n,copy_values);
  if(update_parents)
  {
    node_SetParent(new,parent);
  }
  node_AddItem(parent,new);
  node_ItemIterationReset(n);
  while(node_ItemIterationUnfinished(n))
  {
    node *sub = node_ItemIterate(n);
    node_CopySubTree(sub,new,copy_values,update_parents);
  }
}

node *node_CopyTree(node *n,BOOL copy_values,BOOL update_parents)
{
  node *new = node_CopySub(n,copy_values);
  node_ItemIterationReset(n);
  while(node_ItemIterationUnfinished(n))
  {
    node *sub = node_ItemIterate(n);
    node_CopySubTree(sub,new,copy_values,update_parents);
  }
  return(new);
}

int node_IsType(node *n, unsigned char type)
{
  return(n->type == type);
}

node *node_GetParent(node *n)
{
  return(n->parent);
}

node *node_GetRoot(node *n)
{
  node *s = n;
  node *ret = n;
  while((s=node_GetParent(s)))
  {
    ret=s;
  }
  return(ret);
}

unsigned long long node_GetValue(node *n)
{
  return(n->value);
}

unsigned char node_GetType(node *n)
{
  return(n->type);
}

char *node_GetKey(node *n)
{
  return(n->key);
}

list *node_GetItems(node *n)
{
  return(n->items);
}

int node_HasKey(node *n)
{
  return(n->key!=NULL);
}

/*int node_HasValue(node *n)
{
  return(n->value!=0);
}*/

int node_GetInt(node *n)
{
  return((int)n->value);
}

float node_GetFloat(node *n)
{
    //return((float)n->value);
    float *fp = (float*)&(n->value);
    return(*fp);
}

double node_GetDouble(node *n)
{
    double *dp = (double*)&(n->value);
    return(*dp);
}

unsigned char node_GetUint8(node *n)
{
    return((unsigned char)n->value);
}

unsigned short node_GetUint16(node *n)
{
    return((unsigned short)n->value);
}

unsigned long node_GetUint32(node *n)
{
    return((unsigned long)n->value);
}

unsigned long long node_GetUint64(node *n)
{
    return((unsigned long long)n->value);
}

node *node_GetNode(node *n)
{
    return((node*)(unsigned long)n->value);
}

char node_GetSint8(node *n)
{
    return((char)n->value);
}

short node_GetSint16(node *n)
{
    return((short)n->value);
}

long node_GetSint32(node *n)
{
    return((long)n->value);
}

long long node_GetSint64(node *n)
{
    return((long long)n->value);
}

char *node_GetString(node *n)
{
    return((char*)(unsigned long)n->value);
}

int node_GetBool(node *n)
{
    return((unsigned char)n->value);
}

void *node_GetUser(node *n)
{
    return((void*)(unsigned long)n->value);
}

void node_SetUser(node *n,void *user)
{
  if(node_IsType(n,NODE_TYPE_USER))
     n->value = (unsigned long long)(unsigned long)user;
  else
  {
     node_FreeValue(n->type,n->value);
     //n->value = node_CreateValue(NODE_TYPE_USER,&user);
     n->value = (unsigned long long)(unsigned long)user;
     node_SetType(n,NODE_TYPE_USER);
  }
}

void node_SetNull(node *n)
{
  if(!node_IsType(n,NODE_TYPE_NULL))
  {
     node_FreeValue(n->type,n->value);
     //n->value = node_CreateValue(NODE_TYPE_NULL,NULL);
     n->value = 0;
     node_SetType(n,NODE_TYPE_NULL);
  }
}

void node_SetBool(node *n, int b)
{
  if(node_IsType(n,NODE_TYPE_BOOL))
     n->value = (unsigned long long)b;
  else
  {
     node_FreeValue(n->type,n->value);
     //n->value = node_CreateValue(NODE_TYPE_BOOL,&b);
     n->value = (unsigned long long)b;
     node_SetType(n,NODE_TYPE_BOOL);
  }
}

void node_SetInt(node *n, int i)
{
  if(node_IsType(n,NODE_TYPE_INT))
     n->value = (unsigned long long)i;
  else
  {
     node_FreeValue(n->type,n->value);
     //n->value = node_CreateValue(NODE_TYPE_INT,&i);
     n->value = (unsigned long long)i;
     node_SetType(n,NODE_TYPE_INT);
  }
}

void node_SetFloat(node *n,float f)
{
  if(node_IsType(n,NODE_TYPE_FLOAT))
  {
    //n->value = (unsigned long long)f;
    //memcpy(&n->value,&f,sizeof(float));
    //n->value = f;
    float *fp = (float*)&(n->value);
    *fp = f;
  }
  else
  {
    node_FreeValue(n->type,n->value);
    //n->value = node_CreateValue(NODE_TYPE_FLOAT,&f);
    //n->value = (unsigned long long)f;
    //memcpy(&n->value,&f,sizeof(float));
    //n->value = f;
    float *fp = (float*)&(n->value);
    *fp = f;
    node_SetType(n,NODE_TYPE_FLOAT);
  }
}

void node_SetDouble(node *n,double d)
{
  if(node_IsType(n,NODE_TYPE_DOUBLE))
  {
    //n->value = (unsigned long long)d;
    //memcpy(&n->value,&d,sizeof(double));
    //n->value = d;
    double *dp = (double*)&(n->value);
    *dp = d;
  }
  else
  {
    node_FreeValue(n->type,n->value);
    //n->value = node_CreateValue(NODE_TYPE_DOUBLE,&d);
    //n->value = (unsigned long long)d;
    //memcpy(&n->value,&d,sizeof(double));
    //n->value = d;
    double *dp = (double*)&(n->value);
    *dp = d;
    node_SetType(n,NODE_TYPE_DOUBLE);
  }
}

void node_SetString(node *n,char *s)
{
  node_FreeValue(n->type,n->value);
  //n->value = node_CreateValue(NODE_TYPE_STRING,s);
  n->value = (unsigned long long)(unsigned long)str_Copy(s);
  node_SetType(n,NODE_TYPE_STRING);
}

void node_SetNode(node *n,node *dst)
{
  if(node_IsType(n,NODE_TYPE_NODE))
    n->value = (unsigned long long)(unsigned long)dst;
  else
  {
     node_FreeValue(n->type,n->value);
     n->value = (unsigned long long)(unsigned long)dst;
     node_SetType(n,NODE_TYPE_NODE);
  }
}

void node_SetUint8(node *n,unsigned char c)
{
  if(node_IsType(n,NODE_TYPE_UINT8))
     n->value = (unsigned long long)c;
  else
  {
     node_FreeValue(n->type,n->value);
     //n->value = node_CreateValue(NODE_TYPE_UINT8,&c);
     n->value = (unsigned long long)c;
     node_SetType(n,NODE_TYPE_UINT8);
  }
}

void node_SetUint16(node *n,unsigned short s)
{
  if(node_IsType(n,NODE_TYPE_UINT16))
     n->value = (unsigned long long)s;
  else
  {
     node_FreeValue(n->type,n->value);
     //n->value = node_CreateValue(NODE_TYPE_UINT16,&s);
     n->value = (unsigned long long)s;
     node_SetType(n,NODE_TYPE_UINT16);
  }
}

void node_SetUint32(node *n,unsigned long l)
{
  if(node_IsType(n,NODE_TYPE_UINT32))
     n->value = (unsigned long long)l;
  else
  {
     node_FreeValue(n->type,n->value);
     //n->value = node_CreateValue(NODE_TYPE_UINT32,&l);
     n->value = (unsigned long long)l;
     node_SetType(n,NODE_TYPE_UINT32);
  }
}

void node_SetUint64(node *n,unsigned long long ll)
{
  if(node_IsType(n,NODE_TYPE_UINT64))
     n->value = ll;
  else
  {
     node_FreeValue(n->type,n->value);
     //n->value = node_CreateValue(NODE_TYPE_UINT64,&ll);
     n->value = ll;
     node_SetType(n,NODE_TYPE_UINT64);
  }
}

void node_SetSint8(node *n,char c)
{
  if(node_IsType(n,NODE_TYPE_SINT8))
     n->value = (unsigned long long)c;
  else
  {
     node_FreeValue(n->type,n->value);
     //n->value = node_CreateValue(NODE_TYPE_SINT8,&c);
     n->value = (unsigned long long)c;
     node_SetType(n,NODE_TYPE_SINT8);
  }
}

void node_SetSint16(node *n,short s)
{
  if(node_IsType(n,NODE_TYPE_SINT16))
     n->value = (unsigned long long)s;
  else
  {
     node_FreeValue(n->type,n->value);
     //n->value = node_CreateValue(NODE_TYPE_SINT16,&s);
     n->value = (unsigned long long)s;
     node_SetType(n,NODE_TYPE_SINT16);
  }
}

void node_SetSint32(node *n,long l)
{
  if(node_IsType(n,NODE_TYPE_SINT32))
     n->value = (unsigned long long)l;
  else
  {
     node_FreeValue(n->type,n->value);
     //n->value = node_CreateValue(NODE_TYPE_SINT32,&l);
     n->value = (unsigned long long)l;
     node_SetType(n,NODE_TYPE_SINT32);
  }
}

void node_SetSint64(node *n,long long ll)
{
  if(node_IsType(n,NODE_TYPE_SINT64))
     n->value = (unsigned long long)ll;
  else
  {
     node_FreeValue(n->type,n->value);
     //n->value = node_CreateValue(NODE_TYPE_SINT64,&ll);
     n->value = (unsigned long long)ll;
     node_SetType(n,NODE_TYPE_SINT64);
  }
}

void node_InsertItem(node *n,node *s,long index)
{
  list_Insert(n->items,0,s);
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

int node_HasItem(node *n,node *s)
{
  if(node_GetItemIndex(n,s)!=-1)
    return(True);
  return(False);
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

node *node_GetItemByKey(node *n,char *key)
{
  void *item = NULL;
  long old_index = node_GetItemIterationIndex(n);
  node_ItemIterationReset(n);
  while(node_ItemIterationUnfinished(n))
  {
    node *i = node_ItemIterate(n);
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

node *node_ItemIterate(node *n)
{
  return(list_Iterate(n->items));
}

node *node_ItemPeek(node *n)
{
  if(!list_IterationUnfinished(n->items))
    return(NULL);
  long old = list_GetIterationIndex(n->items);
  node *r = list_Iterate(n->items);
  list_SetIterationIndex(n->items,old);
  return(r);
}

node *node_ItemPeekFurther(node *n,long offset)
{
  if(!list_IterationUnfinished(n->items))
    return(NULL);

  long old = list_GetIterationIndex(n->items);
  long c=0;
  node *r=NULL;
  while(c<offset)
  {
    if(!list_IterationUnfinished(n->items))
    {
      list_SetIterationIndex(n->items,old);
      return(NULL);
    }
    r = list_Iterate(n->items);
    c++;
  }
  list_SetIterationIndex(n->items,old);
  return(r);
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
  //printf("created array: %x\n",r);
  //fflush(stdout);
  return(r);
}

void node_FreeArray(node_array *array,BOOL free_nodes)
{
  //printf("freeing array: %x\n",array);
  //fflush(stdout);
  list_IterationReset(array->nodes);
  while(list_IterationUnfinished(array->nodes))
  { 
    node *i = (node*)list_Iterate(array->nodes);
    node_Free(i,free_nodes);
  }
  list_Close(array->nodes);
  free(array);
}

node_array *node_CopyArray(node_array *array,BOOL copy_values)
{
  node_array *new_array = node_CreateArray(0);
  list_IterationReset(array->nodes);
  while(list_IterationUnfinished(array->nodes))
  { 
    node *i = (node*)list_Iterate(array->nodes);
    node *copy = node_CopyTree(i,copy_values,True);
    list_Push(new_array->nodes,copy);
  }
  return(new_array);
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
  //node_array *array = node_CreateArray(num);
  //memcpy(&(n->value), &array, sizeof(node_array*));
  n->value = (unsigned long long)(unsigned long)node_CreateArray(num);
  node_SetType(n,NODE_TYPE_ARRAY);
  //}
}

long node_array_Add(node *n,node *s)
{
  return(list_Push(((node_array*)(unsigned long)n->value)->nodes,s));
}

node *node_array_Remove(node *n,long index)
{
  return((node*)list_Remove(((node_array*)(unsigned long)n->value)->nodes,index));
}

node *node_array_Get(node *n,long index)
{
  return((node*)list_Get(((node_array*)(unsigned long)n->value)->nodes,index));
}

long node_array_GetNum(node *n)
{
  return(list_GetLen(((node_array*)(unsigned long)n->value)->nodes));
}

void node_array_Clear(node *n)
{
   list_Clear(((node_array*)(unsigned long)n->value)->nodes);
}

node *node_array_Iterate(node *n)
{
  return((node*)list_Iterate(((node_array*)(unsigned long)n->value)->nodes)); 
}

int node_array_IterationUnfinished(node *n)
{
  return(list_IterationUnfinished(((node_array*)(unsigned long)n->value)->nodes));
}

void node_array_IterationReset(node *n)
{
  list_IterationReset(((node_array*)(unsigned long)n->value)->nodes);
}

long node_array_GetIterationIndex(node *n)
{
  return(list_GetIterationIndex(((node_array*)(unsigned long)n->value)->nodes));
}

void node_array_SetIterationIndex(node *n,long iteration_index)
{
  list_SetIterationIndex(((node_array*)(unsigned long)n->value)->nodes,iteration_index);
}

node_binary *node_CreateBinary(void *binary,unsigned long len)
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

void node_SetBinary(node *n,void *binary,unsigned long len)
{
  if(node_IsType(n,NODE_TYPE_BINARY))
  {
     ((node_binary*)(unsigned long)n->value)->value = binary;
     ((node_binary*)(unsigned long)n->value)->len = len;
  }
  else
  {
     node_FreeValue(n->type,n->value);
     n->value = (unsigned long long)(unsigned long)node_CreateBinary(binary,len);
     node_SetType(n,NODE_TYPE_BINARY);
  }
}

void *node_GetBinary(node *n)
{
    return(((node_binary*)(unsigned long)n->value)->value);
}

unsigned long node_GetBinaryLength(node *n)
{
    return(((node_binary*)(unsigned long)n->value)->len);
}









