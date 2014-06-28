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

#include "nyxi.h"

const char *nyxi_helpmsg = "Usage: nyxi [OPTION]... [INPUT FILE]\n\
Execute nyx script Files.\n\
\n\
  -i    interactive mode\n\
  -a    print script ast\n\
  -s    interpret INPUT FILE as script string\n\
  -h    display this help and exit\n\
  -v    output version information and exit\n\
  -p    print the return value\n\
\n\
  Use - as input file or leave empty to use STDIN\n\
  CTRL-D to exit\n\
\n\
  Return value of script will be used as the exit code.\n\
\n\
      Report bugs to <pez2001@voyagerproject.de>.\n";



/*TODO

sparse arrays
x return empty array values
test array handling
support floats
//support byte chunks as value 
add binary typing of strings (intepreted as byte stream)(to read c structs etc)

test class inheritance
and reduce amount of members in classes instanced from the default base class

dump/load state via control object
x writeall
x open files in different modes
seeking
read write (num bytes)


x add evaluate_string function

catch ctrl-D
x interpret each received line via stdin seperately

add create pre sized arrays function


x function call ptr mechanism and seperation from execute_object
x add_handler etc

*/


void add_json_tree(node *state,node *output,node *tree,BOOL add_to_array)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *child = create_class_instance(base_class);
  if(node_GetKey(tree)!=NULL)
    set_obj_string(child,"name",node_GetKey(tree));
  if(node_GetType(tree)==NODE_TYPE_STRING)
    set_obj_string(child,"value",node_GetValue(tree));
  else if(node_GetType(tree)==NODE_TYPE_UINT32)
    set_obj_int(child,"value",(long)node_GetUint32(tree));
  else if(node_GetType(tree)==NODE_TYPE_INT)
    set_obj_int(child,"value",node_GetInt(tree));
  else if(node_GetType(tree)==NODE_TYPE_SINT32)
    set_obj_int(child,"value",node_GetSint32(tree));
  else if(node_GetType(tree)==NODE_TYPE_FLOAT)
    set_obj_float(child,"value",(double)node_GetFloat(tree));
  else if(node_GetType(tree)==NODE_TYPE_DOUBLE)
    set_obj_float(child,"value",node_GetDouble(tree));
  else if(node_GetType(tree)==NODE_TYPE_ARRAY)
  {
    node *items = create_obj("items");
    add_obj_kv(child,items);
    node_array_IterationReset(tree);
    while(node_array_IterationUnfinished(tree))
    {
      node *array_token = node_array_Iterate(tree);
      add_json_tree(state,items,array_token,1);
    }
  }
  if(add_to_array)
    node_AddItem(output,child);
  else
    add_member(output,child);
  node_ItemIterationReset(tree);
  while(node_ItemIterationUnfinished(tree))
  {
    node *sub = node_ItemIterate(tree);
    add_json_tree(state,child,sub,0);
  }
}

void convert_from_json(node *state,node *output,char *json)
{
  node *json_tree = json_LoadString(json);
  if(json_tree==NULL)
    return;
  node_ItemIterationReset(json_tree);
  while(node_ItemIterationUnfinished(json_tree))
  {
    node *sub = node_ItemIterate(json_tree);
    add_json_tree(state,output,sub,0);
  }
  node_FreeTree(json_tree);
}

char *add_tabs(char *s,long tabs_num)
{
  int i=0;
  char *tmp = s;
  for(i=0;i<tabs_num;i++)
    tmp = StringAdd(tmp,"\t");
  return(tmp);
}

char *add_to_json(node *state,node *tree,long level,BOOL print_key)
{
  node *name = node_GetItemByKey(tree,"name");
  node *members = node_GetItemByKey(tree,"members");
  node *items = node_GetItemByKey(tree,"items");
  node *value = node_GetItemByKey(tree,"value");
  char *json = CreateEmptyString();
  long items_num=0;

  json=add_tabs(json,level);
  if(print_key)
  {
    json = StringAdd(json,"\"");
    json = StringAdd(json,node_GetString(name));
    json = StringAdd(json,"\": ");
  }
  if(items!=NULL)
  {
    json = StringAdd(json,"[");
    node_ItemIterationReset(items);
    while(node_ItemIterationUnfinished(items))
    {
      node *sub = node_ItemIterate(items);
      char *json_sub_tmp = add_to_json(state,sub,level+1,0);
      json = StringAdd(json,json_sub_tmp);
      json = StringAdd(json,",");
      free(json_sub_tmp);
    }
    json = StringAdd(json,"\n]");
  }
  else
  {
    if(node_GetItemsNum(members))
    {
      char *json_sub = CreateEmptyString();
      node_ItemIterationReset(members);
      while(node_ItemIterationUnfinished(members))
      {
        node *sub = node_ItemIterate(members);
        node *type = node_GetItemByKey(sub,"type");
        if(!strcmp(node_GetString(type),"function"))
          continue;
        char *json_sub_tmp = add_to_json(state,sub,level+1,1);
        json_sub = StringAdd(json_sub,json_sub_tmp);
        json_sub = StringAdd(json_sub,",\n");
        free(json_sub_tmp);
        items_num++;
      }
      if(items_num)
      {
        json = StringAdd(json,"\n");
        json=add_tabs(json,level);
        json = StringAdd(json,"{\n");
        json = StringAdd(json,json_sub);
        json=add_tabs(json,level);
        json = StringAdd(json,"}");
      }
      free(json_sub);
    }
    if(!items_num)
    {
      if(node_GetType(value)==NODE_TYPE_STRING)
      {
        json = StringAdd(json,"\"");
        json = StringAdd(json,node_GetString(value));
        json = StringAdd(json,"\"");
      }
      else if(node_GetType(value)==NODE_TYPE_SINT32)
      {
        char *num=convert_long_to_string(node_GetSint32(value));
        json = StringAdd(json,num);
        free(num);
      }
      else if(node_GetType(value)==NODE_TYPE_DOUBLE)
      {
        char *num=convert_double_to_string(node_GetDouble(value));
        json = StringAdd(json,num);
        free(num);
      }
      //json = StringAdd(json,",");
    }
  }
  return(json);
}

char *convert_to_json(node *state,node *obj)
{
  node *members = node_GetItemByKey(obj,"members");
  char *json = CreateEmptyString();
  json = StringAdd(json,"{\n");
  if(node_GetItemsNum(members))
  {
    node_ItemIterationReset(members);
    while(node_ItemIterationUnfinished(members))
    {
      node *sub = node_ItemIterate(members);
      node *type = node_GetItemByKey(sub,"type");
      if(!strcmp(node_GetString(type),"function"))
        continue;
      char *json_sub=add_to_json(state,sub,1,1);
      json = StringAdd(json,json_sub);
      json = StringAdd(json,",\n");
      free(json_sub);
    }
  }
  json = StringAdd(json,"}\n");
  return(json);
}

char *state_add_to_json(node *state,node *tree,long level)
{
  char *json = CreateEmptyString();
  long items_num=0;

  json=add_tabs(json,level);
  json = StringAdd(json,"\"");
  json = StringAdd(json,node_GetKey(tree));
  json = StringAdd(json,"\" : ");
  if(node_GetItemsNum(tree))
  {
    char *json_sub = CreateEmptyString();
    node_ItemIterationReset(tree);
    while(node_ItemIterationUnfinished(tree))
    {
      node *sub = node_ItemIterate(tree);
      char *json_sub_tmp = state_add_to_json(state,sub,level+1);
      json_sub = StringAdd(json_sub,json_sub_tmp);
      free(json_sub_tmp);
      items_num++;
    }
    if(items_num)
    {
      json = StringAdd(json,"\n");
      json=add_tabs(json,level);
      json = StringAdd(json,"{\n");
      json = StringAdd(json,json_sub);
      json=add_tabs(json,level);
      json = StringAdd(json,"}\n");
    }
    free(json_sub);
  }
  if(!items_num)
  {
    if(node_GetType(tree)==NODE_TYPE_STRING)
    {
      json = StringAdd(json,"\"");
      json = StringAdd(json,node_GetString(tree));
      json = StringAdd(json,"\"");
    }
    else if(node_GetType(tree)==NODE_TYPE_SINT32)
    {
      char *num=convert_long_to_string(node_GetSint32(tree));
      json = StringAdd(json,num);
      free(num);
    }
    else if(node_GetType(tree)==NODE_TYPE_DOUBLE)
    {
      char *num=convert_double_to_string(node_GetDouble(tree));
      json = StringAdd(json,num);
      free(num);
    }
    json = StringAdd(json,",\n");
  }
  return(json);
}

char *state_to_json(node *state)
{
  char *json = CreateEmptyString();
  json = StringAdd(json,"{\n");
  if(node_GetItemsNum(state))
  {
    node_ItemIterationReset(state);
    while(node_ItemIterationUnfinished(state))
    {
      node *sub = node_ItemIterate(state);
      char *json_sub=state_add_to_json(state,sub,1);
      json = StringAdd(json,json_sub);
      free(json_sub);
    }
  }
  json = StringAdd(json,"}\n");
  return(json);
}

void append_http_query_array_item(node *state,node *array,char *value)
{
  long value_len = strlen(value);
  long offset =0;
  char *val=CreateEmptyString();
  char *key=CreateEmptyString();
  int in_key=1;
  node *base_class = node_GetItemByKey(state,"nyx_object");
  while(offset<value_len)
  {
    char vc=value[offset];
    if(vc=='=')
    {
      in_key=0;
      offset++;
      if(offset>=value_len)
        break;
      vc=value[offset];
    }
    if(in_key)
      key=AddCharToString(key,vc);
    else
      val=AddCharToString(val,vc);
    offset++;
  }
  node *item = create_class_instance(base_class);
  reset_obj_refcount(item);
  node_AddItem(array,item);
  node_SetParent(item,array);
  inc_obj_refcount(item);
  set_obj_string(item,"name",key);
  set_obj_string(item,"value",val);
  free(key);
  free(val);
}

void append_http_query_array(node *state,node *value)
{
  char *query = getenv("QUERY_STRING");
  if(query==NULL || !strlen(query))
    return;
  node *items = create_obj("items");
  add_obj_kv(value,items);
  long query_len = strlen(query);
  long offset =0;
  char *val=CreateEmptyString();
  while(offset<query_len)
  {
    char qc=query[offset];
    if(qc=='&')
    {
      append_http_query_array_item(state,items,val);
      free(val);
      val = CreateEmptyString();
    }
    else
      val=AddCharToString(val,qc);
    offset++;
  }
  if(strlen(val))
    append_http_query_array_item(state,items,val);
  free(val);
}

char *convert_long_to_string(long i)
{
  char *ret=NULL;
  long len = snprintf(NULL,0,"%ld",i);
  if(len)
  {
    ret = (char*)malloc(len+1);
    snprintf(ret,len+1,"%ld",i);
  }
  else
    ret=CreateEmptyString();
  return(ret);
} 

char *convert_double_to_string(double d)
{
  char *ret=NULL;
  long len = snprintf(NULL,0,"%13g",d);
  if(len)
  {
    ret = (char*)malloc(len+1);
    snprintf(ret,len+1,"%13g",d);
  }
  else
    ret=CreateEmptyString();
  return(ret);
} 

char *StringCopy(char *src)
{
  char *a = (char*)malloc(strlen(src)+1);
  memcpy(a, src, strlen(src)+1);
  return(a);
}

char *StringCat(char *a,char *b)
{
  if(a == NULL && b != NULL)
    return(StringCopy(b));
  else
    if(a != NULL && b == NULL)
      return(StringCopy(a));
  else
    if(a == NULL && b == NULL)
      return(NULL);
  char *tmp = (char*)malloc(strlen(a) + strlen(b) + 1);
  memcpy(tmp, a, strlen(a));
  memcpy(tmp + strlen(a), b, strlen(b)+1);
  return(tmp);
}

char *StringCatFree(char *a,char *b)
{
  if(a == NULL && b != NULL)
    return(StringCopy(b));
  else
    if(a != NULL && b == NULL)
      return(StringCopy(a));
  else
    if(a == NULL && b == NULL)
      return(NULL);
  char *tmp = (char*)malloc(strlen(a) + strlen(b) + 1);
  memcpy(tmp, a, strlen(a));
  memcpy(tmp + strlen(a), b, strlen(b)+1);
  free(a);
  return(tmp);
}

char *StringAdd(char *a,char *b)
{
  if(a == NULL && b != NULL)
    return(StringCopy(b));
  else
    if(a != NULL && b == NULL)
      return(a);
  else
    if(a == NULL && b == NULL)
      return(CreateEmptyString());
  long lena = strlen(a);
  long lenb = strlen(b);
  char *tmp = realloc(a, lena + lenb + 1);
  memcpy(tmp + lena, b, lenb+1);
  return(tmp);
}

char *StringMult(char *a,long count)
{
  if(a == NULL || !count)
      return(NULL);
  char *tmp = (char*)malloc((strlen(a)*count) + 1);
  long i=0;
  for(i=0;i<count;i++)
  {
    memcpy(tmp+(i*strlen(a)), a, strlen(a));
  }
  memset(tmp+(strlen(a)*count) + 0, 0, 1);
  return(tmp);
}

long StringMatchCount(char *a,char *b)
{
  if(a==NULL || b==NULL)
    return(0);
  long i=0;
  long alen=strlen(a);
  long blen=strlen(b);
  long match_max_len = alen;
  if(blen<match_max_len)
    match_max_len=blen;
  for(i=0;i<match_max_len;i++)
  {
    if(a[i]!=b[i])
      return(i);
  }
  return(match_max_len);
}

char *SubString(char *a,long start,long len)
{
  long e = start + len;
  long l=len;
  if(e>strlen(a))
    l = strlen(a) - start;
  char *tmp = (char*)malloc(l + 1);
  memset(tmp+l, 0, 1);
  memcpy(tmp, a+start, l);
  return(tmp);
}

char *AddCharToString(char *string,char letter)
{ 
  int len=strlen(string);
  string = (char*)realloc(string,len+2);
  string[len+1] = 0;
  string[len] = letter;
  return(string);
}

char *CreateEmptyString(void)
{
    char *string = (char*)malloc(1);
    string[0] = 0;
    return(string);
}

node *create_obj(char *name)
{
  node *obj = node_Create();
  node_SetType(obj,NODE_TYPE_NODE);
  node_SetKey(obj,name);
  return(obj);
}

void add_obj_kv(node *obj,node *kv)
{
  node_AddItem(obj,kv);
  node_SetParent(kv,obj);
}

void insert_obj_kv(node *obj,node *kv)
{
  node_InsertItem(obj,kv,0);
  node_SetParent(kv,obj);
}

void add_obj_string(node *obj,char *key,char *string)
{
  node *kv = node_Create();
  node_SetKey(kv,key);
  node_SetString(kv,string);
  add_obj_kv(obj,kv); 
}

void add_obj_int(node *obj,char *key,long i)
{
  node *kv = node_Create();
  node_SetKey(kv,key);
  node_SetSint32(kv,i);
  add_obj_kv(obj,kv); 
}

void add_obj_float(node *obj,char *key,double d)
{
  node *kv = node_Create();
  node_SetKey(kv,key);
  node_SetDouble(kv,d);
  add_obj_kv(obj,kv); 
}

void add_obj_node(node *obj,char *key,node *n)
{
  node *kv = node_Create();
  node_SetKey(kv,key);
  node_SetNode(kv,n);
  add_obj_kv(obj,kv); 
}

void set_obj_string(node *obj,char *key,char *string)
{
  node *kv = node_GetItemByKey(obj,key);
  if(kv==NULL)
    add_obj_string(obj,key,string);
  else
    node_SetString(kv,string);
}

void set_obj_int(node *obj,char *key,long i)
{
  node *kv = node_GetItemByKey(obj,key);
  if(kv==NULL)
    add_obj_int(obj,key,i);
  else
    node_SetSint32(kv,i);
}

void set_obj_float(node *obj,char *key,double d)
{
  node *kv = node_GetItemByKey(obj,key);
  if(kv==NULL)
    add_obj_float(obj,key,d);
  else
    node_SetDouble(kv,d);
}

void set_obj_node(node *obj,char *key,node *n)
{
  node *kv = node_GetItemByKey(obj,key);
  if(kv==NULL)
    add_obj_node(obj,key,n);
  else
  {
    node_SetType(kv,NODE_TYPE_NODE);
    kv->value = n;
  }
}

void add_obj_ptr(node *obj,char *key,node *n)
{
  node *kv = node_Create();
  node_SetKey(kv,key);
  node_SetUser(kv,n);
  add_obj_kv(obj,kv); 
}

void set_obj_ptr(node *obj,char *key,void *n)
{
  node *kv = node_GetItemByKey(obj,key);
  if(kv==NULL)
    add_obj_ptr(obj,key,n);
  else
  {
    node_SetType(kv,NODE_TYPE_USER);
    kv->value = n;
  }
}

char *get_obj_name(node *obj)
{
  node *kv = node_GetItemByKey(obj,"name");
  return(node_GetString(kv));
}

char *get_obj_type(node *obj)
{
  node *kv = node_GetItemByKey(obj,"type");
  return(node_GetString(kv));
}

void set_obj_refcount(node *obj,long refcount)
{
  node *_refcount = node_GetItemByKey(obj,"refcount");
  node_SetSint32(_refcount,refcount);
}

void inc_obj_refcount(node *obj)
{
  node *refcount = node_GetItemByKey(obj,"refcount");
  node_SetSint32(refcount,node_GetSint32(refcount)+1);
}

void reset_obj_refcount(node *obj)
{
  node *refcount = node_GetItemByKey(obj,"refcount");
  node_SetSint32(refcount,0);
}

void dec_obj_refcount(node *obj)
{
  node *refcount = node_GetItemByKey(obj,"refcount");
  node_SetSint32(refcount,node_GetSint32(refcount)-1);
}

long get_obj_refcount(node *obj)
{
  node *refcount = node_GetItemByKey(obj,"refcount");
  return(node_GetSint32(refcount));
}

node *get_value(node *obj)
{
  /*returns the object's value*/
  return(node_GetItemByKey(obj,"value"));
}

void obj_print(node *obj)
{
  node *v = get_value(obj);
  node_Print(v,False,False);
}

void add_member(node *obj,node *member)
{
  node *members = node_GetItemByKey(obj,"members");
  //insert_obj_kv(members,member);
  add_obj_kv(members,member);
}

node *get_member_non_recursive(node *obj,char *key)
{
  if(obj==NULL)
    return(NULL);
  node *members = node_GetItemByKey(obj,"members");
  if(members!=NULL)
  {
    node_ItemIterationReset(members);
    while(node_ItemIterationUnfinished(members))
    {
      node *member = node_ItemIterate(members);
      node *member_name = node_GetItemByKey(member,"name");
      if(!strcmp(node_GetString(member_name),key))
        return(member);
    }
  }
  return(NULL);
}

node *get_member(node *obj,char *key)
{
  if(obj==NULL)
    return(NULL);
  node *members = node_GetItemByKey(obj,"members");
  if(members!=NULL)
  {
    node_ItemIterationReset(members);
    while(node_ItemIterationUnfinished(members))
    {
      node *member = node_ItemIterate(members);
      node *member_name = node_GetItemByKey(member,"name");
      if(!strcmp(node_GetString(member_name),key))
        return(member);
    }
  }
  
  node *mem = node_GetParent(obj);
  if(mem!=NULL)
  {
    node *p = node_GetParent(mem);
    if(p!=NULL)
      //return(get_member_non_recursive(p,key));
      return(get_member(p,key));
  }
  return(NULL);
}

node *get_member_part(node *obj,char *key)
{
  if(obj==NULL)
    return(NULL);
  long match_len = 0;
  node *match = NULL;
  node *members = node_GetItemByKey(obj,"members");
  if(members!=NULL)
  {
    node_ItemIterationReset(members);
    while(node_ItemIterationUnfinished(members))
    {
      node *member = node_ItemIterate(members);
      node *member_name = node_GetItemByKey(member,"name");
      //if(!strcmp(node_GetString(member_name),key))
      //  return(member);
      long mmlen = StringMatchCount(node_GetString(member_name),key);
      if(mmlen>match_len)
      {
        match = member; 
        match_len = mmlen;
        //printf("best match so far with:%d [%s] of [%s]\n",match_len,get_obj_name(match),get_obj_name(node_GetParent(node_GetParent(match))));
      }
    }
  }

  node *mem = node_GetParent(obj);
  if(mem!=NULL)
  {
    node *p = node_GetParent(mem);
    if(p!=NULL)
    {
      node *sub_match=get_member_part(p,key);
      if(sub_match!=NULL)
      {
        long sub_mmlen = StringMatchCount(get_obj_name(sub_match),key);
        if(sub_mmlen>match_len)
        {
          match = sub_match;
          //printf("better match with:%d as %d [%s] of [%s]\n",sub_mmlen,match_len,get_obj_name(match),get_obj_name(node_GetParent(node_GetParent(match))));
          match_len = sub_mmlen;
        }
      }
      //return(get_member(p,key));
    }
  }
  if(match_len)
  {
    //printf("ret match:%d [%s] of [%s]\n",match_len,get_obj_name(match),get_obj_name(node_GetParent(node_GetParent(match))));
    return(match);
  }
  return(NULL);
}

node *get_item(node *obj,node *key)
{
  node *items = node_GetItemByKey(obj,"items");
  node *key_value = node_GetItemByKey(key,"value");
  if(node_GetType(key_value) == NODE_TYPE_STRING)
  {
    char *key_name = node_GetString(key_value);
    node_ItemIterationReset(items);
    while(node_ItemIterationUnfinished(items))
    {
      node *item = node_ItemIterate(items);
      node *item_name = node_GetItemByKey(item,"name");
      if(!strcmp(node_GetString(item_name),key_name))
        return(item);
    }
  }
  else if(node_GetType(key_value) == NODE_TYPE_SINT32)
  {
    node *item = node_GetItem(items,node_GetSint32(key_value));
    return(item);
  }
  return(NULL);
}

node *create_class_instance(node *class_obj)
{
  node *child = node_CopyTree(class_obj,True,True);
  /*node *base_class_type = (node*)node_GetValue(node_GetItemByKey(class_obj,"base_class_type"));
  if(base_class_type!=NULL)
  {
    node *base_class_instance = create_class_instance(base_class_type);
    set_obj_node(child,"base_class_instance",base_class_instance);
  }*/
  //printf("created class instance:%x :%s\n",child,get_obj_name(child));
  node_SetParent(child,NULL);
  return(child);
}

void free_obj(node *state,node *obj)
{
  /*
  node *base_class_instance = node_GetItemByKey(obj,"base_class_instance");
  if(base_class_instance!=NULL && base_class_instance != state->base_class)
    free_obj(state,base_class_instance);
  node *members = node_GetItemByKey(obj,"members");
  if(members==NULL)
    return;
  node_ItemIterationReset(members);
  while(node_ItemIterationUnfinished(members))
  {
    node *member = node_ItemIterate(members);
    free_obj(state,member);
  }
  */
  node_FreeTree(obj);
}

node *create_base_obj_layout(char *obj_name)
{
  node *base = create_obj("nyx_object");
  add_obj_string(base,"type","class");
  add_obj_string(base,"name",obj_name);
  //add_obj_kv(base,create_obj("base_class_instance"));
  add_obj_kv(base,create_obj("base_class_type"));
  add_obj_kv(base,create_obj("members"));
  set_obj_int(base,"value",0);
  add_obj_int(base,"refcount",0);


  return(base);
}


void add_class_object_internal_function(node *class,node *base_class,char *method_name)
{
  node *method = create_base_obj_layout(method_name);
  set_obj_string(method,"type","function");
  //set_obj_node(base,"base_class_instance",class);
  set_obj_node(method,"base_class_type",base_class);
  inc_obj_refcount(method);
  add_member(class,method);
}

//state,execution_obj,block
void add_class_object_function(node *class,node *base_class,char *method_name,node*(*handler)(node*,node*,node*,node*))
{
  node *method = create_base_obj_layout(method_name);
  set_obj_string(method,"type","function");
  //set_obj_node(base,"base_class_instance",class);
  set_obj_node(method,"base_class_type",base_class);
  set_obj_ptr(method,"handler",(void*)handler);
  inc_obj_refcount(method);
  add_member(class,method);
}

node *create_file_class_object(void)
{
  node *base = create_base_obj_layout("file");

  add_class_object_function(base,base,"=",nyxh_assign);

  add_class_object_function(base,base,"readall",nyxh_readall);
  add_class_object_function(base,base,"writeall",nyxh_writeall);
  add_class_object_function(base,base,"open",nyxh_open);
  add_class_object_function(base,base,"close",nyxh_close);
  //add_class_object_function(base,base,"write",nyxh_write);
  //add_class_object_function(base,base,"read",nyxh_read);
  //add_class_object_function(base,base,"seek",nyxh_seek);
  //add_class_object_function(base,base,"set_mode",nyxh_set_mode);
  //add_class_object_function(base,base,"flush",nyxh_flush);
  //add_class_object_function(base,base,"ioctl",nyxh_ioctl);

  return(base);
}


node *create_http_class_object(void)
{

  node *base = create_base_obj_layout("http");
  add_class_object_function(base,base,"=",nyxh_assign);

  add_class_object_function(base,base,"create_request",nyxh_http_create_request);
  add_class_object_function(base,base,"parse_answer",nyxh_http_parse_answer);
  return(base);
}


node *create_socket_class_object(void)
{

  node *base = create_base_obj_layout("socket");
  add_class_object_function(base,base,"=",nyxh_assign);

  add_class_object_function(base,base,"open",nyxh_socket_open);
  add_class_object_function(base,base,"close",nyxh_socket_close);
  add_class_object_function(base,base,"write",nyxh_socket_write);
  add_class_object_function(base,base,"read",nyxh_socket_read);
  add_class_object_function(base,base,"set_address",nyxh_socket_set_address);
  add_class_object_function(base,base,"connect",nyxh_socket_connect);
  node *sock_dgram_const = create_base_obj_layout("SOCK_DGRAM");
  node *sock_stream_const = create_base_obj_layout("SOCK_STREAM");
  add_member(base,sock_dgram_const);
  add_member(base,sock_stream_const);

  //add_class_object_function(base,base,"write",nyxh_socket_write);
  //add_class_object_function(base,base,"read",nyxh_socket_read);
  return(base);
}




node *create_sys_class_object(void)
{
  node *base = create_base_obj_layout("sys");
  add_class_object_function(base,base,"=",nyxh_assign);

  add_class_object_function(base,base,"name",nyxh_name);
  add_class_object_function(base,base,"working_directory",nyxh_working_directory); //contains name/path and files as sub items
  add_class_object_function(base,base,"change_working_directory",nyxh_change_working_directory);
  add_class_object_function(base,base,"dump",nyxh_dump);
  //add_class_object_function(base,base,"load",nyxh_load);
  //add_class_object_function(base,base,"reset",nyxh_reset);
  //add_class_object_function(base,base,"exit",nyxh_exit);
  add_class_object_function(base,base,"execute",nyxh_execute);
  //add_class_object_function(base,base,"remove",nyxh_remove);//remove file/directory
  //add_class_object_function(base,base,"gc_collect",nyxh_gc_collect);
  //add_class_object_function(base,base,"error",nyxh_error);//print to stderr
  //add_class_object_function(base,base,"script_filename",nyxh_script_filename);//if applicable
  //add_class_object_function(base,base,"interpreter_filename",nyxh_interpreter_filename);
  //add_class_object_function(base,base,"interpreter_version",nyxh_interpreter_version);//returns array with major/minor/build
  //add_class_object_function(base,base,"obj_kv",nyxh_obj_kv);//returns an kv pair in an object as valued new base class instance

  return(base);
}


/*implement class construction via classic constructor*/

node *create_class_object(void)
{
  node *base = create_base_obj_layout("object");
  add_class_object_function(base,base,"=",nyxh_assign);
  add_class_object_function(base,base,":",nyxh_set_value_only);
  add_class_object_function(base,base,"+",nyxh_add);
  add_class_object_function(base,base,"-",nyxh_sub);
  add_class_object_function(base,base,"/",nyxh_div);
  add_class_object_function(base,base,"*",nyxh_mul);
  add_class_object_function(base,base,"<",nyxh_lt);
  add_class_object_function(base,base,">",nyxh_gt);
  add_class_object_function(base,base,"==",nyxh_eq);
  add_class_object_function(base,base,"<=",nyxh_lt_eq);
  add_class_object_function(base,base,">=",nyxh_gt_eq);
  add_class_object_function(base,base,"!=",nyxh_neq);
  add_class_object_function(base,base,"&&",nyxh_and);
  add_class_object_function(base,base,"||",nyxh_or);
  add_class_object_function(base,base,"PRE!",nyxh_pre_not);
  add_class_object_function(base,base,"PRE-",nyxh_pre_sub);
  add_class_object_function(base,base,"PRE+",nyxh_pre_add);
  

  add_class_object_function(base,base,"print",nyxh_print);
  add_class_object_function(base,base,"println",nyxh_println);
  add_class_object_function(base,base,"input",nyxh_input);
  add_class_object_function(base,base,"http_query",nyxh_http_query);
  add_class_object_function(base,base,"str",nyxh_str);
  add_class_object_function(base,base,"int",nyxh_int);
  add_class_object_function(base,base,"float",nyxh_float);
  add_class_object_function(base,base,"len",nyxh_len);
  add_class_object_function(base,base,"else",nyxh_else);
  add_class_object_function(base,base,"from_json",nyxh_from_json);
  add_class_object_function(base,base,"to_json",nyxh_to_json);
  add_class_object_function(base,base,"split",nyxh_split);
  
  add_class_object_function(base,base,"open",nyxh_open);
  add_class_object_function(base,base,"close",nyxh_close);
  

  add_class_object_function(base,base,"?",nyxh_cmp);
  add_class_object_function(base,base,"??",nyxh_init_cmp);
  


  //add_member(base,create_sys_class_object());

  return(base);
}

node *create_block_class_object(node *base_class,node *block)
{
  node *base = create_class_instance(base_class);//,"block");
  set_obj_string(base,"name","block");
  set_obj_string(base,"type","nyx_il_block");
  node *il_block = node_CopyTree(block,True,True);
  add_obj_kv(base,il_block);  

  add_class_object_function(base,base,"break",nyxh_break);
  add_class_object_function(base,base,"continue",nyxh_continue);
  add_class_object_function(base,base,"restart",nyxh_restart);
  add_class_object_function(base,base,"import",nyxh_import);
  add_class_object_function(base,base,"eval",nyxh_eval);
  add_class_object_function(base,base,"sys",nyxh_sys);
  add_class_object_function(base,base,"test",nyxh_test);
  //add_class_object_function(base,base,"args",nyxh_args);
  add_class_object_function(base,base,"handler_test",nyxh_handler_test);
  add_member(base,create_file_class_object());
  add_member(base,create_socket_class_object());
  add_member(base,create_http_class_object());



  return(base);
}

void advance_garbage(node *state)
{
  node *garbage = node_GetItemByKey(state,"garbage");
  node *gc_cache = node_GetItemByKey(state,"garbage_cache");
  printf("adding gc_cache to gc\n");
  node_ItemIterationReset(gc_cache);
  while(node_ItemIterationUnfinished(gc_cache))
  {
    node *obj = node_ItemIterate(gc_cache);
    if(node_HasItem(garbage,obj))
      continue;
    if(node_GetParent(obj)!=NULL)
    {
      printf("tried add gc obj with a parent:%x\n",obj);
    }
    node_AddItem(garbage,obj);
  }
  node_ClearItems(gc_cache);
}

long get_execution_level(node *state)
{
  node *nexecution_level = node_GetItemByKey(state,"execution_level");
  long execution_level = node_GetSint32(nexecution_level);
  return(execution_level);
}

void inc_execution_level(node *state)
{
  node *nexecution_level = node_GetItemByKey(state,"execution_level");
  node_SetSint32(nexecution_level,node_GetSint32(nexecution_level)+1);
}

void dec_execution_level(node *state)
{
  node *nexecution_level = node_GetItemByKey(state,"execution_level");
  node_SetSint32(nexecution_level,node_GetSint32(nexecution_level)-1);
}

void add_garbage(node *state,node *obj)
{
  node *garbage = node_GetItemByKey(state,"garbage");
  long execution_level = get_execution_level(state);
  //printf("adding to gc with exe level:%d\n",execution_level); 
  if(node_HasItem(garbage,obj))
    return;
  if(node_GetParent(obj)!=NULL)
  {
    printf("tried add gc obj with a parent:%x\n",obj);
  }
  node_AddItem(garbage,obj);
  node_SetTag(obj,(void*)execution_level);
}

void free_garbage(node *state,long min_level)
{
  //printf("freeing garbage items with min: %d\n",min_level);
  //fflush(stdout);
  node *garbage = node_GetItemByKey(state,"garbage");
  node_ItemIterationReset(garbage);
  while(node_ItemIterationUnfinished(garbage))
  {
    node *gc = node_ItemIterate(garbage);
    long gc_level = (long)node_GetTag(gc);
    if((get_obj_refcount(gc)<=0) && (gc_level >= min_level))
    {
      //printf("removing:%x,%d\n",gc,gc_level);
      //fflush(stdout);
      node_FreeTree(gc);
      node_RemoveItem(garbage,gc);
      //node_ItemIterationReset(garbage);
      node_SetItemIterationIndex(garbage,node_GetItemIterationIndex(garbage)-1);
      //printf("removed:%x,%d\n",gc,gc_level);
      //fflush(stdout);
    }
  }
  //printf("garbage collected\n");
  //fflush(stdout);
  //node_ClearItems(garbage);
}

node *create_nyx_state(node *base_class)
{
  node *state = create_obj("nyx_state");
  node *garbage = create_obj("garbage");
  node *garbage2 = create_obj("garbage_cache");
  node *class_types = create_obj("class_types");
  node *blocks = create_obj("blocks");
  add_obj_int(state,"execution_level",1);
  add_obj_kv(state,class_types);
  add_obj_kv(state,garbage);
  add_obj_kv(state,garbage2);
  add_obj_kv(state,blocks);
  add_obj_kv(state,base_class);
  return(state);
}

node *execute_obj(node *state,node *obj,node *block,node *parameters,BOOL execute_block)
{
  node *value = NULL;
  node *pars = NULL;
  if(parameters==NULL)
    pars = create_obj("parameters");
  else
    pars = parameters;
  if(!strcmp(get_obj_type(obj),"nyx_il_block"))
  {
    if(execute_block)
    {
      node *block_parameters = node_GetItemByKey(obj,"nyx_parameters");
      if(block_parameters!=NULL)
      {
        node_ItemIterationReset(block_parameters);
        long p_index = 0;
        while(node_ItemIterationUnfinished(block_parameters))
        {
          node *token = node_ItemIterate(block_parameters);
          if(!strcmp(node_GetKey(token),"nyx_statement"))
          {
            node *tmp_parent = node_GetParent(obj);
            node_SetParent(obj,NULL);
            node *sub = evaluate_statement(state,token,obj,0,NULL);
            node_SetParent(obj,tmp_parent);
            node *obj_name = node_GetItemByKey(sub,"name");
            node *parent = node_GetParent(sub);
            node_RemoveItem(parent,sub);
            node_SetParent(sub,NULL);
            dec_obj_refcount(sub);
            add_garbage(state,sub);
            value = node_CopyTree(node_GetItem(pars,p_index),True,True);
            reset_obj_refcount(value);
            set_obj_string(value,"name",node_GetString(obj_name));
            node_AddItem(parent,value);
            node_SetParent(value,parent);
            inc_obj_refcount(value);
            node *obj_parameters = node_GetItemByKey(sub,"nyx_parameters");
            if(obj_parameters!=NULL)
            {
              node_RemoveItem(sub,obj_parameters);
              add_obj_kv(value,node_CopyTree(obj_parameters,True,True));
            }
            //TODO anonymous block ?
          }
          p_index++;
        }
      }
      else
      {
        if(node_GetItemsNum(pars))
        {
          node *base_class = node_GetItemByKey(state,"nyx_object");
          node *arguments = create_class_instance(base_class);
          set_obj_string(arguments,"name","arguments");
          node *items = create_obj("items");
          add_obj_kv(arguments,items);
          node_ItemIterationReset(pars);
          while(node_ItemIterationUnfinished(pars))
          {
            node *par = node_ItemIterate(pars);
            node *item = node_CopyTree(par,True,True);
            node_AddItem(items,item);
            node_SetParent(item,items);
            inc_obj_refcount(item);
          }          
          add_member(obj,arguments);//TODO what if arguments already exists ?
        }
      }
      value = evaluate_block_instance(state,obj);
      node *obj_members = node_GetItemByKey(obj,"members");
      node *arguments = node_GetItemByKey(obj_members,"arguments");
      if(arguments!=NULL)
      {
        node_RemoveItem(obj_members,arguments);
        node_FreeTree(arguments);
      }
    }
    else
    {
      value = obj;
    }
  }
  else if(!strcmp(get_obj_type(obj),"function"))
  {
    node *handler = node_GetItemByKey(obj,"handler");
    if(handler!=NULL)
    {
      //node *(*real_handler)(node*,node*,node*,node*) = (nyx_function_handler)node_GetValue(handler);
      nyx_function_handler real_handler = (nyx_function_handler)node_GetValue(handler);
      node *parent = node_GetParent(node_GetParent(obj));
      value = real_handler(state,parent,block,pars);
    }
  }
  else if(!strcmp(node_GetKey(obj),"nyx_object"))
  {
    //nothing to be done ,just return obj
    value = obj;
  }
  if(value==NULL)
  {
    //return an interpreter error here 
    printf("exec return NULL\n");
  }
  node_ClearItems(pars);
  node_FreeTree(pars);
  return(value);
}

node *search_block_path_for_member(node *block,char *key)
{
  node *found_obj=NULL;
  if(node_GetItemByKey(block,"anonymous_block_parent")!=NULL)
  {
    node *abp=node_GetItemByKey(block,"anonymous_block_parent");
    //printf("searching in anon parent:%x\n",node_GetValue(abp));
    found_obj = get_member(node_GetValue(abp),key);
    if(found_obj==NULL)
    {
      found_obj = search_block_path_for_member(node_GetValue(abp),key);
    }
  }
  return(found_obj);
}

node *evaluate_statement(node *state,node *statement,node *block,long iteration_start_index,char *preop)
{
  node_SetItemIterationIndex(statement,iteration_start_index);
  long index = iteration_start_index;
  node *parameters = create_obj("parameters");
  node *actual_obj = block;
  node *prev_token = NULL;
  node *base_class = node_GetItemByKey(state,"nyx_object");
  char *use_preop = preop;
  inc_execution_level(state);
  //node *gc_cache = create_obj("garbage_cache");


  while(node_ItemIterationUnfinished(statement))
  {
    node *token = node_ItemIterate(statement);
    //printf("------\nevaluating next token\n");
    //node_PrintTree(token);
    //printf("------\n");
    if(!strcmp(node_GetKey(token),"nyx_parameters"))
    {
      node_ItemIterationReset(token);
      while(node_ItemIterationUnfinished(token))
      {
        node *parameter_token = node_ItemIterate(token);
        actual_obj = evaluate_statement(state,parameter_token,block,0,NULL);
      }

      if(use_preop!=NULL)
      {
        char *preop_prefixed=StringCat("PRE",use_preop);
        node *found_obj = get_member_part(actual_obj,preop_prefixed);
        if(found_obj!=NULL)
        {
          actual_obj = execute_obj(state,found_obj,block,NULL,False);
        }
        free(preop_prefixed);
        free(use_preop);
        use_preop=NULL;
      }
    }
    else if(prev_token!=NULL && !strcmp(node_GetKey(prev_token),"val") && !strcmp(node_GetKey(token),"nyx_array"))
    {
      node *key_obj = evaluate_statement(state,node_GetItem(token,0),block,0,NULL);
      if(node_GetItemByKey(actual_obj,"items")!=NULL)
      {
        node *found_obj = get_item(actual_obj,key_obj);
        if(found_obj==NULL)
        {
          actual_obj = create_class_instance(base_class);
          reset_obj_refcount(actual_obj);
          add_garbage(state,actual_obj);
          //node_AddItem(gc_cache,actual_obj);
        }
        else
        {
          actual_obj = found_obj;
        }
      }
      else //check if string is used as array with a number as key - return a single char string
      {
        node *real_value = node_GetItemByKey(actual_obj,"value");
        node *real_key_value = node_GetItemByKey(key_obj,"value");
        if(node_GetType(real_value)==NODE_TYPE_STRING && node_GetType(real_key_value)==NODE_TYPE_SINT32)
        {
          char *tok = SubString(node_GetString(real_value),node_GetSint32(real_key_value),1);
          actual_obj = create_class_instance(base_class);
          reset_obj_refcount(actual_obj);
          set_obj_string(actual_obj,"name","character");
          set_obj_string(actual_obj,"value",tok);
          free(tok);
          reset_obj_refcount(actual_obj);
          add_garbage(state,actual_obj);
          //node_AddItem(gc_cache,actual_obj);
        }
      }
    }
    else if(!strcmp(node_GetKey(token),"nyx_array"))
    {
      node *child = create_class_instance(base_class);
      node *items = create_obj("items");
      add_obj_kv(child,items);
      add_garbage(state,child);
      //node_AddItem(gc_cache,child);
      node_ItemIterationReset(token);
      while(node_ItemIterationUnfinished(token))
      {
        node *parameter_token = node_ItemIterate(token);
        node *array_obj = evaluate_statement(state,parameter_token,block,0,NULL);
        node *item = node_CopyTree(array_obj,True,True);
        node_SetParent(item,items);
        inc_obj_refcount(item);
        node_AddItem(items,item);
      }
      actual_obj=child;
    }
    else if(!strcmp(node_GetKey(token),"nyx_block"))
    {
      node *block_class_instance = create_block_class_object(base_class,token);
      node *exe_parameters = create_obj("parameters");
      node *peek = node_ItemPeek(statement);
      if(peek!=NULL && !strcmp(node_GetKey(peek),"nyx_parameters"))
      {
        node *sub_parameters = node_ItemIterate(statement);
        node_ItemIterationReset(sub_parameters);
        while(node_ItemIterationUnfinished(sub_parameters))
        {
          node *parameter_token = node_ItemIterate(sub_parameters);
          node *sub_obj = evaluate_statement(state,parameter_token,block,0,NULL);
          node_AddItem(exe_parameters,sub_obj);
        }
        index++;
        actual_obj = execute_obj(state,block_class_instance,block,exe_parameters,True);
        add_garbage(state,block_class_instance);
        //node_AddItem(gc_cache,block_class_instance);
      }
      else
      {
        set_obj_node(block_class_instance,"anonymous_block_parent",block);
        node_ClearItems(exe_parameters);
        node_Free(exe_parameters,True);
        add_garbage(state,block_class_instance);
        //node_AddItem(gc_cache,block_class_instance);
        actual_obj = block_class_instance;
      }
    }
    else if(!strcmp(node_GetKey(token),"str"))
    {
      node *child = create_class_instance(base_class);
      set_obj_string(child,"value",node_GetValue(token));
      add_garbage(state,child);
      //node_AddItem(gc_cache,child);
      actual_obj = child;
    }
    else if(!strcmp(node_GetKey(token),"val"))
    {
      if(node_GetType(token) == NODE_TYPE_STRING)
      {
        if(strcmp(node_GetValue(token),"@"))
        {
          node *found_obj = get_member(actual_obj,node_GetValue(token));
          if(found_obj==NULL)
            found_obj = search_block_path_for_member(actual_obj,node_GetValue(token));
          if(found_obj==NULL)
          {
            node *child = create_class_instance(base_class);
            set_obj_string(child,"name",node_GetValue(token));
            add_member(actual_obj,child);
            inc_obj_refcount(child);
            actual_obj = child;
            node *peek = node_ItemPeek(statement);
            if(peek!=NULL && !strcmp(node_GetKey(peek),"nyx_parameters"))
            {
              node *parameters_definition = node_ItemIterate(statement);
              node *pars = node_CopyTree(parameters_definition,True,True);
              node_AddItem(child,pars);
              index++;
            }
          }
          else
          {
            //repeat until no parameter blocks left
            while((node_ItemPeek(statement)!=NULL && !strcmp(node_GetKey(node_ItemPeek(statement)),"nyx_parameters")) )
            {

              if(!strcmp(get_obj_type(found_obj),"function"))
              {
                node *exe_parameters = create_obj("parameters");
                node *peek = node_ItemPeek(statement);
                if(peek!=NULL && !strcmp(node_GetKey(peek),"nyx_parameters"))
                {
                  node *sub_parameters = node_ItemIterate(statement);
                  node_ItemIterationReset(sub_parameters);
                  while(node_ItemIterationUnfinished(sub_parameters))
                  {
                    node *parameter_token = node_ItemIterate(sub_parameters);
                    node *sub_obj = evaluate_statement(state,parameter_token,block,0,NULL);
                    node_AddItem(exe_parameters,sub_obj);
                  }
                  index++;
                }
                actual_obj = execute_obj(state,found_obj,block,exe_parameters,False);
              }
              else
              {
                node *peek = node_ItemPeek(statement);
                if(peek!=NULL && !strcmp(node_GetKey(peek),"nyx_parameters"))
                {
                  node *sub_parameters = node_ItemIterate(statement);
                  node *peek = node_ItemPeek(statement);
                  if(peek!=NULL && !strcmp(node_GetKey(peek),"ops") && !strcmp(node_GetValue(peek),"="))
                  {
                    node *pars = node_CopyTree(sub_parameters,True,True);
                    node *old_pars = node_GetItemByKey(found_obj,"nyx_parameters");
                    if(old_pars!=NULL)
                    {
                      node_RemoveItem(found_obj,old_pars);
                      node_FreeTree(old_pars);
                    }
                    node_AddItem(found_obj,pars);
                    actual_obj = found_obj;
                  }
                  else
                  {
                    node *exe_parameters = create_obj("parameters");
                    node_ItemIterationReset(sub_parameters);
                    while(node_ItemIterationUnfinished(sub_parameters))
                    {
                      node *parameter_token = node_ItemIterate(sub_parameters);
                      node *sub_obj = evaluate_statement(state,parameter_token,block,0,NULL);
                      node_AddItem(exe_parameters,sub_obj);
                    }
                    actual_obj = execute_obj(state,found_obj,block,exe_parameters,True);
                  }
                  index++;
                }
                else
                {
                  actual_obj = found_obj;
                }
              }
              found_obj=actual_obj; /*wuerg*/
            }            
            actual_obj = found_obj; /*wuerg*/
          }
        }
      }
      else if(node_GetType(token) == NODE_TYPE_SINT32)
      {
        node *child = create_class_instance(base_class);
        node *peek = node_ItemPeek(statement);
        if(peek!=NULL && !strcmp(node_GetKey(peek),"ops") && !strcmp(node_GetString(peek),"."))
        {
          //TODO check if val is really a number
          node *sub_ops = node_ItemIterate(statement);
          node *tenth_val = node_ItemIterate(statement);
          index+=2;
          long tenth = node_GetSint32(tenth_val);
          char *t=convert_long_to_string(tenth);
          long tlen=strlen(t);
          free(t);
          double d=((double)node_GetSint32(token))+(((double)tenth)/pow(10,tlen));
          //array increase if key is an index > array_len
          //global functions (copy and parenting on the fly)
          //select/poll events
          //api clean ups
          //parse error reporting
          //error/exception handling
          //shell mode
          //node.c better hashing support /easy use with getitembykey function ?per defines?
          //add some examples
          //build nyx homepage using nyx and jquery ajax json css
          //sdl api subset support 
          //secure http input values
          //node.c sockets support
          //insert members at pos 0 instead of adding
          set_obj_float(child,"value",d);
        }
        else
          set_obj_int(child,"value",node_GetSint32(token));
        actual_obj = child;
        add_garbage(state,child);
        //node_AddItem(gc_cache,child);
      }
      if(use_preop!=NULL)
      {
        char *preop_prefixed=StringCat("PRE",use_preop);
        node *found_obj = get_member_part(actual_obj,preop_prefixed);
        if(found_obj!=NULL)
        {
          actual_obj = execute_obj(state,found_obj,block,NULL,False);
        }
        free(preop_prefixed);
        free(use_preop);
        use_preop=NULL;
      }
    }
    else if(!strcmp(node_GetKey(token),"ops"))
    {
        if(prev_token!=NULL)
        {
          if(!strcmp(node_GetKey(prev_token),"ops"))
          {
            use_preop = StringCopy(node_GetString(token));
          }
        }
        else
        {
          use_preop = StringCopy(node_GetString(token));
        }
        //printf("searching member part:[%s][%s]\n",get_obj_name(actual_obj),node_GetValue(token));
        //node_PrintTree(actual_obj);
        node *found_obj = get_member_part(actual_obj,node_GetValue(token));
        //printf("found this:[%s] of [%s]\n",get_obj_name(found_obj),get_obj_name(node_GetParent(node_GetParent(found_obj))));
        if(found_obj!=NULL && use_preop == NULL)
        {
          long token_len = strlen(node_GetString(token));
          char *found_name = get_obj_name(found_obj);
          if(strlen(found_name)<token_len)
          {
            long found_name_len=strlen(found_name);
            use_preop=SubString(node_GetValue(token),found_name_len,token_len-found_name_len);
          }

          if(index+1<node_GetItemsNum(statement))
          {
            node *sub_obj = evaluate_statement(state,statement,block,index+1,use_preop);
            node_AddItem(parameters,sub_obj);
          }
          actual_obj = found_obj;
          actual_obj = execute_obj(state,actual_obj,block,parameters,False);
          //printf("evaluate obj in ret op out:[%s]\n",get_obj_name(actual_obj));
          //advance_garbage(state);
          dec_execution_level(state);
          return(actual_obj);
       }
        /*else if(index+1<node_GetItemsNum(statement))
        {
          node *sub_obj = evaluate_statement(state,statement,block,index+1,use_preop);
          node_AddItem(parameters,sub_obj);
        }*/
    }
    prev_token = token;
    index++;    
  }
  actual_obj = execute_obj(state,actual_obj,block,parameters,False);
  //printf("evaluate obj in ret:[%s]\n",get_obj_name(actual_obj));
  //advance_garbage(state);
  dec_execution_level(state);
  return(actual_obj);
}

char *check_block_flag(node *state)
{
  char *ret = NULL;
  node *block_flag=node_GetItemByKey(state,"block_flag");
  //printf("checking block flags\n");
  //fflush(stdout);
  if(block_flag!=NULL) 
  {
    //node_PrintTree(block_flag);
    char *bf = node_GetString(block_flag);
    //printf("[%s]\n",bf);
    node *block_break_count = node_GetItemByKey(state,"block_break_count");
    long count = node_GetSint32(block_break_count);
    if(count==0 && strlen(bf))
    {
      set_obj_int(state,"block_break_count",0);
      ret = StringCopy(bf);
      set_obj_string(state,"block_flag","");
      //printf("block break count -> 0 [%s]\n",ret);
    }
    else if(strlen(bf))
    {
      set_obj_int(state,"block_break_count",count-1);
      //printf("break count dec:%d\n",count-1);
      ret = StringCopy("break");
    }
    else
    {
      ret=CreateEmptyString();
      //printf("created empty string2\n");
    }
  }
  else
  {
    ret=CreateEmptyString();
    //printf("created empty string\n");
  }
  //printf("ret:[%s]:%x\n",ret,ret);
  //fflush(stdout);
  
  //if(!strcmp(ret,""))
  //  printf("returning empty string\n");
  //fflush(stdout);
  return(ret);
}

node *evaluate_block(node *state,node *block)
{
  node *ret = NULL;
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *block_class_instance = create_block_class_object(base_class,block);
  node *blocks = node_GetItemByKey(state,"blocks");
  add_obj_kv(blocks,block_class_instance);
  return(evaluate_block_instance_in(state,block_class_instance,block_class_instance));
}

node *evaluate_block_in(node *state,node *block,node *master_block)
{
  node *ret = NULL;
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *block_class_instance = create_block_class_object(base_class,block);
  node *blocks = node_GetItemByKey(state,"blocks");
  add_obj_kv(blocks,block_class_instance);
  return(evaluate_block_instance_in(state,block_class_instance,master_block));
}

node *evaluate_block_instance(node *state,node *block_class_instance)
{
  return(evaluate_block_instance_in(state,block_class_instance,block_class_instance));
}

node *evaluate_block_instance_in(node *state,node *block_class_instance,node *block)
{
  inc_execution_level(state);
  node *ret = NULL;
  node *il_block = node_GetItemByKey(block_class_instance,"nyx_block");
  if(!node_GetItemsNum(il_block))
  {
      node *base_class = node_GetItemByKey(state,"nyx_object");
      ret = create_class_instance(base_class);
      reset_obj_refcount(ret);
      add_garbage(state,ret);
  }
  else
  {
    node_ItemIterationReset(il_block);
    while(node_ItemIterationUnfinished(il_block))
    {
      node *nyx_statement = node_ItemIterate(il_block);
      ret = evaluate_statement(state,nyx_statement,block,0,NULL);
      char *block_flag=check_block_flag(state);
      if(!strcmp(block_flag,"break"))
      {
        //node_FreeTree(block_class_instance);
        free(block_flag);
        free_garbage(state,get_execution_level(state)+2);
        //advance_garbage(state);
        dec_execution_level(state);
        return(ret);
      }
      else if(!strcmp(block_flag,"restart"))
        node_ItemIterationReset(il_block);
      else if(!strcmp(block_flag,"continue"))
      {
        long itindex = node_GetItemIterationIndex(il_block);
        if(itindex+1<node_GetItemsNum(il_block))
          node_SetItemIterationIndex(il_block,itindex+1);
      }
      free(block_flag);
    }
  }
  if(ret==NULL)
  {
    printf("eval return NULL\n");
  }
  free_garbage(state,get_execution_level(state)+2);
  dec_execution_level(state);
  //free_garbage(state);
  //advance_garbage(state);
  return(ret);
}

node *call_function(node *state,char *name,node *parameters)//TODO only searches first master block
{
  node *ret=NULL;
  node *blocks = node_GetItemByKey(state,"blocks");
  node *block = node_GetItem(blocks,0);
  node *found_obj = get_member(block,name);
  if(found_obj==NULL)
    found_obj = search_block_path_for_member(block,name);
  if(found_obj==NULL)
  {
    return(NULL);
  }
  if(!strcmp(get_obj_type(found_obj),"function"))
  {
    ret = execute_obj(state,found_obj,block,parameters,False);
  }
  else
  {
    ret = execute_obj(state,found_obj,block,NULL,True);
  }
  return(ret);
}

node *init_nyx(void)
{
  node *base_class = create_class_object();
  //node *nyx_state = create_nyx_state(nyx_block,base_class);
  node *nyx_state = create_nyx_state(base_class);
  return(nyx_state);
}

volatile int done=0;

void sig_handler(int sig)//,siginfo_t *siginfo,void *context)
{
  //printf("signo:%d\n",sig);
  //printf("signo:%d\n",siginfo->si_signo);
  //printf("sigcode:%d\n",siginfo->si_code);
  done=1;
}

//struct termios *setup_terminal()
//#define _O_U16TEXT 0x20000
void setup_terminal(void)
{
    //struct termios old;
    //struct termios new;
    //sigaction(sig_handler,0);
    //signal(SIGABRT,sig_handler);
    //signal(SIGTERM,sig_handler);
    signal(SIGINT,sig_handler);
    //_setmode(_fileno(stdout), _O_U16TEXT);
    //signal(1,sig_handler);
    //signal(3,sig_handler);
    //signal(20,sig_handler);
    //signal(sig_handler,0);
    //setvbuf(stdout,NULL,_IONBUF,0);
    //tcgetattr(0,&old);
    //new = old;
    //new.c_cc[VEOF] = 3;
    //new.c_cc[VINTR] = 4;
    //tcsetattr(0,TCSANOW,&new);
    //return(old);
}

//void close_terminal(struct termios *old)
void close_terminal(void)
{
  //tcsettr(0,TCSANOW,old);
}


void close_nyx(node *state)
{
  node *garbage = node_GetItemByKey(state,"garbage");
  node_ClearItems(garbage);
  node_FreeTree(state);
}

int main(int argc, char** argv)
{
  #ifdef USE_MEMORY_DEBUGGING
  mem_Init();
  #endif
  #ifdef WIN32
  WSADATA wsaData;
  WSAStartup(MAKEWORD(2, 2), &wsaData);
  #endif


  int c = 0;
  int print_return_value=0;
  //int interactive_mode=0;
  int print_ast=0;
  int use_input_as_script_string=0;

  int ret = 0;
  //printf("checking pars\n");
  
  //while ((c = getopt (argc, argv, "if:ohv")) != -1)
  while ((c = getopt (argc, argv, "iashvp")) != -1)
  {
    switch (c)
    {
      /*case 'f':
        if(strcasecmp(optarg,"RPYC")==0)
        {
          format_pyc = 0;
          format_rpyc = 1;
          format_rpyc_plus = 0;
        } else
        if(strcasecmp(optarg,"RPYC+")==0)
        {
          format_pyc = 0;
          format_rpyc = 0;
          format_rpyc_plus = 1;
        }
        //fprintf (stderr,"\n");
        //id=atoi(optarg);
        break;
        */
      case 's':
        use_input_as_script_string = 1;
        break;
      case 'p':
        print_return_value = 1;
        break;
      /*case 'i':
        interactive_mode = 1;
        break;*/
      case 'a':
        print_ast = 1;
        break;
      case 'h':
        printf(nyxi_helpmsg);
        return(0);
      case 'v':
        //printf("Version : %d.%d-%d\n",MAJOR_VERSION,MINOR_VERSION,BUILD+1);
        printf("nyx interpreter %d.%d (build %d)\n",MAJOR_VERSION,MINOR_VERSION,BUILD+1);
        return(0);
      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint(optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
        return(1);
      default:
        //printf("aborting.\n");
        abort();
    }
  }
  char *filename = argv[optind];
  node *nyx_stream = NULL;
  //printf("filename:[%s]\n",filename);
  if(filename != NULL && strlen(filename)>0 && !use_input_as_script_string && strcmp(filename,"-")) //use filename to load a script file
  {
    //printf("reading file\n");
    nyx_stream = nyx_LoadFile(filename);
  }
  else if(filename != NULL && strcmp(filename,"-") && use_input_as_script_string)//) //string input //!(strlen(filename)==1 &&
  {
    //printf("reading string\n");
    nyx_stream = nyx_LoadString(filename);
  }
  /*else if(interactive_mode) //interactive 
  { 
    printf("interactive\n");
  }*/
  else //read from stdin
  {
    setup_terminal();
    node *state = init_nyx();
    node *blocks=node_GetItemByKey(state,"blocks");
    char *buf =(char*)malloc(100);
    char *tmp_get=NULL;
    node *ret_obj=NULL;
    //printf("reading standard input\n");
    while((tmp_get=fgets(buf,100,stdin))!=NULL && !done && ((long)(tmp_get)!=EOF))
    {
      //printf("interpreting:[%s]\n",buf);
      nyx_stream = nyx_LoadString(buf);
      if(nyx_stream!=NULL)
      {
        //node_PrintTree(nyx_stream);
        node *nyx_block = node_GetItemByKey(nyx_stream,"nyx_block");
        if(nyx_block!=NULL)
        {
          //node *master_block = node_GetItemByKey(,"nyx_block");
          node *master_block = node_GetItem(blocks,0);
          node_RemoveItem(nyx_stream,nyx_block);
          node_FreeTree(nyx_stream);
          if(master_block!=NULL)
          {
            ret_obj = evaluate_block_in(state,nyx_block,master_block);  
          }
          else
          {
            //add_obj_kv(state,nyx_block);
            ret_obj = evaluate_block(state,nyx_block);  
          }

          //free_garbage(state);
          node *real_value = node_GetItemByKey(ret_obj,"value");
          if(print_return_value)
          {
            node_Print(real_value,False,False);
            printf("\n");
          }
          if(node_GetType(real_value)==NODE_TYPE_SINT32)
            ret = node_GetSint32(real_value);
          else if(node_GetType(real_value)==NODE_TYPE_STRING)
            ret = atoi(node_GetString(real_value));
          //advance_garbage(state);
          free_garbage(state,0);

          //node_FreeTree(ret_obj);
          //fflush(stdout);
        }
        else
          node_FreeTree(nyx_stream);
      }
    }
    close_terminal();//(old_term);
    close_nyx(state);
    return(0);
  }
  //printf("before interpret stream\n");
  if(print_ast)
  {
    if(nyx_stream!=NULL)
    {
      node_PrintTree(nyx_stream);
      node_FreeTree(nyx_stream);
    }
    return(0);
  }
  else // interpret stream
  { 
    //printf("interpret stream\n");
    node *state = init_nyx();
    if(nyx_stream!=NULL)
    {
      node *nyx_block = node_GetItemByKey(nyx_stream,"nyx_block");
      if(nyx_block!=NULL)
      {
        add_obj_kv(state,nyx_block);
        node_RemoveItem(nyx_stream,nyx_block);
        node_FreeTree(nyx_stream);
        node *ret_obj = evaluate_block(state,nyx_block);  
        //test call into script 
        //node *parameters = create_obj("parameters");
        //printf("calling defined script function\n");
        //node *ret_call_obj = call_function(nyx_state,"external_call",parameters);//,nyx_block);
        //free_garbage(state);
        node *real_value = node_GetItemByKey(ret_obj,"value");

        if(print_return_value)
        {
          node_Print(real_value,False,False);
          printf("\n");
        }

        if(node_GetType(real_value)==NODE_TYPE_SINT32)
          ret = node_GetSint32(real_value);
        else if(node_GetType(real_value)==NODE_TYPE_STRING)
          ret = atoi(node_GetString(real_value));
        //advance_garbage(state);
        free_garbage(state,0);
        //node_FreeTree(ret_obj);
        //fflush(stdout);
      }
      else
        node_FreeTree(nyx_stream);
    }
    close_nyx(state);
  }
  #ifdef WIN32
  WSACleanup();
  #endif
  #ifdef USE_MEMORY_DEBUGGING
  mem_Close();
  #endif
  return(ret%256);
}