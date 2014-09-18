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

#include "nyx.h"

void add_json_tree(node *state,node *output,node *tree,BOOL add_to_array,long item_index)
{
  node *base_class = get_base_class(state);
  node *child = create_class_instance(base_class);
  if(node_GetKey(tree)!=NULL)
    set_obj_string(child,"name",node_GetKey(tree));
  if(node_GetType(tree)==NODE_TYPE_STRING)
    set_obj_string(child,"value",node_GetString(tree));
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
    long i = 0;
    while(node_array_IterationUnfinished(tree))
    {
      node *array_token = node_array_Iterate(tree);
      add_json_tree(state,items,array_token,1,i);
      i++;
    }
  }
  if(add_to_array)
  {
    node_AddItem(output,child);
    node_SetParent(child,output);
    set_obj_int(child,"item_index",item_index);
  }
  else
    add_member(output,child);
  inc_obj_refcount(child);
  node_ItemIterationReset(tree);
  while(node_ItemIterationUnfinished(tree))
  {
    node *sub = node_ItemIterate(tree);
    add_json_tree(state,child,sub,0,-1);
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
    add_json_tree(state,output,sub,0,-1);
  }
  node_FreeTree(json_tree);
}

char *add_tabs(char *s,long tabs_num)
{
  if(!tabs_num)
    return(s);
  int i=0;
  char *tmp = s;
  for(i=0;i<tabs_num;i++)
    tmp = str_CatFree(tmp,"\t");
  return(tmp);
}

char *add_to_json(node *state,node *tree,long level,BOOL print_key)
{
  node *name = node_GetItemByKey(tree,"name");
  node *members = node_GetItemByKey(tree,"members");
  node *items = node_GetItemByKey(tree,"items");
  node *value = node_GetItemByKey(tree,"value");
  char *json = str_CreateEmpty();
  long items_num=0;

  json = add_tabs(json,level);
  if(print_key)
  {
    json = str_CatFree(json,"\"");
    json = str_CatFree(json,node_GetString(name));
    json = str_CatFree(json,"\": ");
  }
  if(items!=NULL)
  {
    json = str_CatFree(json,"[");
    node_ItemIterationReset(items);
    while(node_ItemIterationUnfinished(items))
    {
      node *sub = node_ItemIterate(items);
      char *json_sub_tmp = add_to_json(state,sub,level+1,0);
      json = str_CatFree(json,json_sub_tmp);
      json = str_CatFree(json,",");
      free(json_sub_tmp);
    }
    json = str_CatFree(json,"\n]");
  }
  else
  {
    if(node_GetItemsNum(members))
    {
      char *json_sub = str_CreateEmpty();
      node_ItemIterationReset(members);
      while(node_ItemIterationUnfinished(members))
      {
        node *sub = node_ItemIterate(members);
        node *type = node_GetItemByKey(sub,"type");
        if(!strcmp(node_GetString(type),"function"))
          continue;
        char *json_sub_tmp = add_to_json(state,sub,level+1,1);
        json_sub = str_CatFree(json_sub,json_sub_tmp);
        json_sub = str_CatFree(json_sub,",\n");
        free(json_sub_tmp);
        items_num++;
      }
      if(items_num)
      {
        json = str_CatFree(json,"\n");
        json = add_tabs(json,level);
        json = str_CatFree(json,"{\n");
        json = str_CatFree(json,json_sub);
        json = add_tabs(json,level);
        json = str_CatFree(json,"}");
      }
      free(json_sub);
    }
    if(!items_num)
    {
      if(node_GetType(value)==NODE_TYPE_STRING)
      {
        json = str_CatFree(json,"\"");
        json = str_CatFree(json,node_GetString(value));
        json = str_CatFree(json,"\"");
      }
      else if(node_GetType(value)==NODE_TYPE_SINT32)
      {
        char *num=str_FromLong(node_GetSint32(value));
        json = str_CatFree(json,num);
        free(num);
      }
      else if(node_GetType(value)==NODE_TYPE_DOUBLE)
      {
        char *num=str_FromDouble(node_GetDouble(value));
        json = str_CatFree(json,num);
        free(num);
      }
      //json = str_CatFree(json,",");
    }
  }
  return(json);
}

char *convert_to_json(node *state,node *obj)
{
  //printf("<pre>\n");
  //node_PrintTree(obj);
  //printf("</pre>\n");
  //node *members = node_GetItemByKey(obj,"members");
  char *json = str_CreateEmpty();
  json = str_CatFree(json,"{\n");
/*  if(node_GetItemsNum(members))
  {
    node_ItemIterationReset(members);
    while(node_ItemIterationUnfinished(members))
    {
      node *sub = node_ItemIterate(members);
      node *type = node_GetItemByKey(sub,"type");
      if(!strcmp(node_GetString(type),"function"))
        continue;
      char *json_sub=add_to_json(state,sub,1,1);
      json = str_CatFree(json,json_sub);
      json = str_CatFree(json,",\n");
      free(json_sub);
    }
  }*/
  char *json_sub=add_to_json(state,obj,0,0);
  json = str_CatFree(json,json_sub);
  free(json_sub);
  //printf("json:[%s]\n",json);

  json = str_CatFree(json,"}\n");
  return(json);
}

char *state_add_to_json(node *state,node *tree,long level)
{
  char *json = str_CreateEmpty();
  long items_num=0;

  json=add_tabs(json,level);
  json = str_CatFree(json,"\"");
  json = str_CatFree(json,node_GetKey(tree));
  json = str_CatFree(json,"\" : ");
  if(node_GetItemsNum(tree))
  {
    char *json_sub = str_CreateEmpty();
    node_ItemIterationReset(tree);
    while(node_ItemIterationUnfinished(tree))
    {
      node *sub = node_ItemIterate(tree);
      char *json_sub_tmp = state_add_to_json(state,sub,level+1);
      json_sub = str_CatFree(json_sub,json_sub_tmp);
      free(json_sub_tmp);
      items_num++;
    }
    if(items_num)
    {
      json = str_CatFree(json,"\n");
      json=add_tabs(json,level);
      json = str_CatFree(json,"{\n");
      json = str_CatFree(json,json_sub);
      json=add_tabs(json,level);
      json = str_CatFree(json,"}\n");
    }
    free(json_sub);
  }
  if(!items_num)
  {
    if(node_GetType(tree)==NODE_TYPE_STRING)
    {
      json = str_CatFree(json,"\"");
      json = str_CatFree(json,node_GetString(tree));
      json = str_CatFree(json,"\"");
    }
    else if(node_GetType(tree)==NODE_TYPE_SINT32)
    {
      char *num=str_FromLong(node_GetSint32(tree));
      json = str_CatFree(json,num);
      free(num);
    }
    else if(node_GetType(tree)==NODE_TYPE_DOUBLE)
    {
      char *num=str_FromDouble(node_GetDouble(tree));
      json = str_CatFree(json,num);
      free(num);
    }
    json = str_CatFree(json,",\n");
  }
  return(json);
}

char *state_to_json(node *state)
{
  char *json = str_CreateEmpty();
  json = str_CatFree(json,"{\n");
  if(node_GetItemsNum(state))
  {
    node_ItemIterationReset(state);
    while(node_ItemIterationUnfinished(state))
    {
      node *sub = node_ItemIterate(state);
      char *json_sub=state_add_to_json(state,sub,1);
      json = str_CatFree(json,json_sub);
      free(json_sub);
    }
  }
  json = str_CatFree(json,"}\n");
  return(json);
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

int remove_obj_kv(node *obj,node *kv)
{
  long r = node_RemoveItem(obj,kv);
  node_SetParent(kv,NULL);
  if(r==-1)
    return(0);
  return(1);
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
    node_SetNode(kv,n);
  }
}

void add_obj_ptr(node *obj,char *key,void *n)
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
    node_SetUser(kv,n);
  }
}

void set_obj_value(node *obj,node *value)
{
  node *kv = node_GetItemByKey(obj,"value");
  if(kv==NULL)
    add_obj_kv(obj,value);
  else
  {
    remove_obj_kv(obj,kv);
    node_FreeTree(kv);
    add_obj_kv(obj,value);
  }
}

char *get_obj_str(node *obj,char *key)
{
  node *kv = node_GetItemByKey(obj,key);
  return(node_GetString(kv));
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

void add_private(node *obj,node *private)
{
  node *privates = node_GetItemByKey(obj,"privates");
  add_obj_kv(privates,private);
}

void remove_private(node *obj,node *private)
{
  node *privates = node_GetItemByKey(obj,"privates");
  remove_obj_kv(privates,private);
}

void add_member(node *obj,node *member)
{
  node *members = node_GetItemByKey(obj,"members");
  add_obj_kv(members,member);
}

int remove_member(node *obj,node *member)
{
  node *members = node_GetItemByKey(obj,"members");
  return(remove_obj_kv(members,member));
}

void remove_item(node *obj,node *item)
{
  node *items = node_GetItemByKey(obj,"items");
  remove_obj_kv(items,item);
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

node *get_object(node *obj,char *key)
{
  if(obj!=NULL)
  {
    node_ItemIterationReset(obj);
    while(node_ItemIterationUnfinished(obj))
    {
      node *item = node_ItemIterate(obj);
      node *item_name = node_GetItemByKey(item,"name");
      if(!strcmp(node_GetString(item_name),key))
        return(item);
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
      long mmlen = str_MatchCount(node_GetString(member_name),key);
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
      if(p==obj)
      {
        printf("circular reference detected: %x (%s)\n",obj,get_obj_name(obj));
        fflush(stdout);
        //int x=1/0;
      }
      node *sub_match=get_member_part(p,key);
      if(sub_match!=NULL)
      {
        long sub_mmlen = str_MatchCount(get_obj_name(sub_match),key);
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

long get_items_new_index(node *items)
{
  long index = 0;
  node_ItemIterationReset(items);
  while(node_ItemIterationUnfinished(items))
  {
    node *item = node_ItemIterate(items);
    node *nitem_index = node_GetItemByKey(item,"item_index");
    if(nitem_index!=NULL)
    {
      long item_index = node_GetSint32(nitem_index);
      if(item_index>index)
        index=item_index;
    }
  }
  if(node_GetItemsNum(items))
    index = index + 1;
  return(index);
}

long get_last_index(node *items) //TODO use an index value instead which will be updated if an item is added ,etc
{
  long item_index = -1; 
  node_ItemIterationReset(items);
  while(node_ItemIterationUnfinished(items))
  {
    node *item = node_ItemIterate(items);
    node *nindex = node_GetItemByKey(item,"item_index");
    long cindex = node_GetSint32(nindex);
    if(item_index < cindex)
      item_index = cindex;
  }
  return(item_index);
}

long get_last_index_obj(node *obj)
{
  node *items = node_GetItemByKey(obj,"items");
  return(get_last_index(items));
}

node *get_item(node *state,node *obj,node *key,BOOL append_new_item)//TODO remove BOOl and seperate into 2 functions
{
  node *items = node_GetItemByKey(obj,"items");
  if(!items)
  {
    printf("tried to search for an item in a non array obj :%s\n",get_obj_name(obj));
    return(NULL);
  }
  node *key_value = get_value(key);
  char *key_name = NULL;
  long item_index = -1; 
  if(node_GetType(key_value) == NODE_TYPE_STRING)
  {
    key_name = node_GetString(key_value);
  }
  else if(node_GetType(key_value) == NODE_TYPE_SINT32)
  {
    item_index = node_GetSint32(key_value);
  }
  node_ItemIterationReset(items);
  while(node_ItemIterationUnfinished(items))
  {
    node *item = node_ItemIterate(items);
    if(node_GetType(key_value) == NODE_TYPE_STRING)
    {
      node *item_name = node_GetItemByKey(item,"name");
      //printf("array cmp:[%s] == [%s]\n",node_GetString(item_name),key_name);
      //fflush(stdout);
      if(!strcmp(node_GetString(item_name),key_name))
        return(item);
    }
    else if(node_GetType(key_value) == NODE_TYPE_SINT32)
    {
      node *nindex = node_GetItemByKey(item,"item_index");
      long cindex = node_GetSint32(nindex);
      if(item_index == cindex)
        return(item);
    }
  }
  if(append_new_item)
  {
    node *base_class = get_base_class(state);
    node *item = create_class_instance(base_class);
    node_SetParent(item,items);
    node_AddItem(items,item);
    inc_obj_refcount(item);
    if(node_GetType(key_value) == NODE_TYPE_STRING)
    {
      //item_index = node_GetItemsNum(items)-1;
      item_index = get_items_new_index(items);
      //item_index = get_last_index(items)+1;
      //if(item_index<0)
      //  item_index = 0;
      set_obj_int(item,"item_index",item_index);
      set_obj_string(item,"name",key_name);
      //printf("iindex:%d,name:[%s]\n",item_index,key_name);
    }
    else if(node_GetType(key_value) == NODE_TYPE_SINT32)
    {
      set_obj_int(item,"item_index",item_index);
    }
    /*reset_obj_refcount(item);
    add_garbage(state,item);*/
    return(item);
  }
  else
    return(NULL);
}

node *create_class_instance(node *class_obj)
{
  node *child = node_CopyTree(class_obj,True,True);
  reset_obj_refcount(child);
  node_SetParent(child,NULL);
  return(child);
}

//void free_obj(node *state,node *obj)
//{
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
//  node_FreeTree(obj);
//}

node *create_base_obj_layout(char *obj_name)
{
  node *base = create_obj("nyx_object");
  add_obj_string(base,"type","class");
  add_obj_string(base,"name",obj_name);
  add_obj_kv(base,create_obj("members"));//TODO dont add this automatically
  add_obj_kv(base,create_obj("privates"));//TODO dont add this automatically
  set_obj_int(base,"value",0);
  add_obj_int(base,"refcount",0);
  return(base);
}

//state,execution_obj,block
void add_class_object_function(node *class,char *method_name,node*(*handler)(node*,node*,node*,node*))
{
  node *method = create_base_obj_layout(method_name);
  set_obj_string(method,"type","function");
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpedantic"
    set_obj_ptr(method,"handler",(void*)handler);
  #pragma GCC diagnostic pop
    
  inc_obj_refcount(method);
  add_member(class,method);
}

node *create_proxy_object(node *state,node *target,char *name)
{
  node *base_class = get_base_class(state);
  node *proxy = create_class_instance(base_class);
  set_obj_string(proxy,"type","proxy");
  set_obj_string(proxy,"name",name);
  //printf("creating proxy:%x for %x,%s\n",proxy,target,name);
  set_obj_ptr(proxy,"target",(void*)target);
  return(proxy);
}

node *get_proxy_target(node *proxy)
{
  node *target = node_GetItemByKey(proxy,"target");
  node *value = (node*)(unsigned long)node_GetValue(target);
  return(value);
}

node *resolve_object(node *obj)
{
  if(!strcmp(get_obj_type(obj),"proxy"))
    return(get_proxy_target(obj));
  else
    return(obj);
}

node *create_class_object(void)
{
  node *base = create_base_obj_layout("object");
  add_class_object_function(base,"=",nyxh_assign);
  //add_class_object_function(base,":",nyxh_set_value_only);
  add_class_object_function(base,":",nyxh_switch_name_value);
  add_class_object_function(base,":=",nyxh_assign_copy);


  add_class_object_function(base,"+",nyxh_add);
  add_class_object_function(base,"-",nyxh_sub);
  add_class_object_function(base,"/",nyxh_div);
  add_class_object_function(base,"*",nyxh_mul);
  add_class_object_function(base,"<",nyxh_lt);
  add_class_object_function(base,">",nyxh_gt);
  add_class_object_function(base,"==",nyxh_eq);
  add_class_object_function(base,"<=",nyxh_lt_eq);
  add_class_object_function(base,">=",nyxh_gt_eq);
  add_class_object_function(base,"!=",nyxh_neq);
  add_class_object_function(base,"^",nyxh_binary_xor);
  add_class_object_function(base,"&&",nyxh_and);
  add_class_object_function(base,"||",nyxh_or);
  add_class_object_function(base,"&",nyxh_binary_and);
  add_class_object_function(base,"|",nyxh_binary_or);
  add_class_object_function(base,"PRE~",nyxh_pre_binary_not);
  add_class_object_function(base,"PRE!",nyxh_pre_not);
  add_class_object_function(base,"PRE-",nyxh_pre_sub);
  add_class_object_function(base,"PRE+",nyxh_pre_add);

  add_class_object_function(base,"++",nyxh_immediate_add);
  add_class_object_function(base,"--",nyxh_immediate_sub);
  add_class_object_function(base,"PRE--",nyxh_pre_immediate_sub);
  add_class_object_function(base,"PRE++",nyxh_pre_immediate_add);

  
  add_class_object_function(base,"println",nyxh_println);
  add_class_object_function(base,"print",nyxh_print);
  add_class_object_function(base,"copy",nyxh_copy);
  //add_class_object_function(base,"http_query",nyxh_http_query);
  add_class_object_function(base,"str",nyxh_str);
  add_class_object_function(base,"float",nyxh_float);
  add_class_object_function(base,"len",nyxh_len);
  add_class_object_function(base,"input",nyxh_input); //TODO check if search member part gets troubled by this -> add additional var/check if only full match possible
  add_class_object_function(base,"int",nyxh_int);
  add_class_object_function(base,"index_of",nyxh_index_of);
  add_class_object_function(base,"in_keys",nyxh_in_keys);
  add_class_object_function(base,"in",nyxh_in);
  add_class_object_function(base,"each",nyxh_each);
  add_class_object_function(base,"remove",nyxh_remove);
  //add_class_object_function(base,"remove_by_key",nyxh_remove_by_key);
  add_class_object_function(base,"sub",nyxh_substr);
  add_class_object_function(base,"replace",nyxh_replace);
  add_class_object_function(base,"else",nyxh_else);
  add_class_object_function(base,"do",nyxh_do);
  add_class_object_function(base,"has",nyxh_has);
  add_class_object_function(base,"_name_",nyxh_name);
  add_class_object_function(base,"from_json",nyxh_from_json);
  add_class_object_function(base,"to_json",nyxh_to_json);
  add_class_object_function(base,"split",nyxh_split);
  add_class_object_function(base,"parent",nyxh_parent);
  add_class_object_function(base,"get_block",nyxh_get_block);
  return(base);
}

node *create_block_class_object(node *base_class)
{
  node *base = create_class_instance(base_class);
  set_obj_string(base,"name","block");
  set_obj_string(base,"type","nyx_il_block");

  //add_class_object_function(base,"open",nyxh_open);
  //add_class_object_function(base,"close",nyxh_close);
 
  add_class_object_function(base,"?",nyxh_cmp);
  add_class_object_function(base,"??",nyxh_init_cmp);

  add_class_object_function(base,"break",nyxh_break);
  add_class_object_function(base,"continue",nyxh_continue);
  add_class_object_function(base,"restart",nyxh_restart);
  add_class_object_function(base,"return",nyxh_return);
  add_class_object_function(base,"import",nyxh_import);
  add_class_object_function(base,"eval",nyxh_eval);
  add_class_object_function(base,"test",nyxh_test);
  //add_class_object_function(base,"args",nyxh_args);
  return(base);
}

node *create_block_class_instance(node *state,node *block)
{
  node *block_class = get_block_class(state);
  node *base = create_class_instance(block_class);
  //printf("creating block class instance: %x for %x\n",base,block);
  node *il_block = node_CopyTree(block,True,True);
  add_obj_kv(base,il_block); 
  reset_obj_refcount(base);
  node_SetParent(base,NULL); 
  return(base);
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
  node *garbage = get_garbage(state);
  long execution_level = get_execution_level(state);
  //printf("adding to gc with exe level:%d\n",execution_level); 
  if(node_HasItem(garbage,obj))
    return;
  if(node_GetParent(obj)!=NULL)
  {
    //printf("tried add gc obj with a parent:%x:%s\n",obj,get_obj_name(obj));
    //int x=1/0;
    return;
  }
  if(node_GetItemByKey(obj,"no_gc")!=NULL)
  {
    //printf("skipping \"no_gc\" obj: %x\n",obj);
    //int x=1/0;
    return;
  }
  ///printf("adding to gc with exe level:%x (%s) (el:%d) (ref:%d)\n",obj,get_obj_name(obj),execution_level,get_obj_refcount(obj)); 
  ///fflush(stdout);
  node_AddItem(garbage,obj);
  node_SetTag(obj,(void*)execution_level);
}

void free_garbage(node *state,long min_level,node *skip_this)
{
  //printf("freeing garbage items with min: %d\n",min_level);
  //fflush(stdout);
  node *garbage = get_garbage(state);
  node_ItemIterationReset(garbage);
  while(node_ItemIterationUnfinished(garbage))
  {
    node *gc = node_ItemIterate(garbage);
    long gc_level = (long)node_GetTag(gc);
    /*if(skip_this == gc)
      printf("not freed: skipping: %x\n",gc);
    if(get_obj_refcount(gc)>0)
      printf("not freed: refcount>0 : %x (%d)\n",gc,get_obj_refcount(gc));
    if(gc_level < min_level)
      printf("not freed: gc_level < min_level : %x (%d<%d) (refs:%d)\n",gc,gc_level,min_level,get_obj_refcount(gc));
    */

    if(gc==skip_this)
    {
      //printf("removing non freed from gc list(skip): %x (%s)\n",gc,get_obj_name(gc));
      //fflush(stdout);
      node_RemoveItem(garbage,gc);
      node_SetItemIterationIndex(garbage,node_GetItemIterationIndex(garbage)-1);
      continue;
    }
    if(get_obj_refcount(gc)>=1)
    {
      //printf("removing non freed from gc list(refs): %x (%s)\n",gc,get_obj_name(gc));
      //fflush(stdout);
      node_RemoveItem(garbage,gc);
      node_SetItemIterationIndex(garbage,node_GetItemIterationIndex(garbage)-1);
      continue;
    }
    if(gc_level >= min_level)
    {
      ///printf("removing:%x,%d (%s)\n",gc,gc_level,get_obj_name(gc));
      ///fflush(stdout);
      if(!strcmp(get_obj_type(gc),"proxy"))
      {
        ///printf("removing proxy:%x (%s)\n",gc,get_obj_name(gc));
        ///fflush(stdout);
        node *ptarget = get_proxy_target(gc);
        ///printf("proxy target: %x (%s)\n",ptarget,get_obj_name(ptarget));
        ///fflush(stdout);
        dec_obj_refcount(ptarget);
        add_garbage(state,ptarget);
      }
      node *gc_members = node_GetItemByKey(gc,"members");
      node_ItemIterationReset(gc_members);
      while(node_ItemIterationUnfinished(gc_members))
      {
        node *gc_member = node_ItemIterate(gc_members);
        //printf("removing sub member: %x (%s)\n",gc_member,get_obj_name(gc_member));
        dec_obj_refcount(gc_member);
        node_SetParent(gc_member,NULL);
        add_garbage(state,gc_member);
      }
      node_ClearItems(gc_members);
      node *gc_items = node_GetItemByKey(gc,"items");
      if(gc_items)
      {
        node_ItemIterationReset(gc_items);
        while(node_ItemIterationUnfinished(gc_items))
        {
          node *gc_item = node_ItemIterate(gc_items);
          //printf("removing sub item: %x (%s)\n",gc_item,get_obj_name(gc_item));
          dec_obj_refcount(gc_item);
          node_SetParent(gc_item,NULL);
          add_garbage(state,gc_item);
        }
        node_ClearItems(gc_items);
      }
      node_FreeTree(gc);
      node_RemoveItem(garbage,gc);
      node_SetItemIterationIndex(garbage,node_GetItemIterationIndex(garbage)-1);
      //printf("removed:%x,%d\n",gc,gc_level);
      //fflush(stdout);
      //continue;
    }
    /*else
    {
      printf("removing non freed from gc list(gc_level): %x (%s)\n",gc,get_obj_name(gc));
      node_RemoveItem(garbage,gc);
      node_SetItemIterationIndex(garbage,node_GetItemIterationIndex(garbage)-1);
    }*/
  }
  //printf("garbage collected\n");
  //fflush(stdout);
  //node_ClearItems(garbage);
}

node *get_base_class(node *state)
{
  return(node_GetItem(state,0));
}

node *get_block_class(node *state)
{
  return(node_GetItem(state,1));
}

node *get_true_class(node *state)
{
  return(node_GetItem(state,2));
}

node *get_false_class(node *state)
{
  return(node_GetItem(state,3));
}

node *get_null_class(node *state)
{
  return(node_GetItem(state,4));
}

node *get_garbage(node *state)
{
  return(node_GetItem(state,5));
}

node *get_modules(node *state)
{
  return(node_GetItem(state,6));
}

node *copy_class(node *class_obj)
{
  //removes no gc tag
  //and other internal objects
  node *value = node_CopyTree(class_obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  node *no_gc = node_GetItemByKey(value,"no_gc");
  if(no_gc)
    remove_obj_kv(value,no_gc);
  return(value);
}

//move only data keep the ptr - important if proxies are used
void clean_move(node *state,node *dst,node *src)//TODO rename to clean_copy
{
  //printf("clean_move:%x to %x\n",src,dst);
  //update base values
  set_obj_string(dst,"type",get_obj_str(src,"type"));
  //set_obj_string(base,"name",obj_name);
  
  node *src_value = node_CopyTree(get_value(src),True,True);
  set_obj_value(dst,src_value);

  node *dst_il_block = node_GetItemByKey(src,"nyx_block");
  if(dst_il_block)
    remove_obj_kv(dst,dst_il_block);

  node *src_il_block = node_GetItemByKey(src,"nyx_block");
  if(src_il_block)
  {
    //printf("found il_block in move\n");
    //fflush(stdout);
    node *il_block = node_CopyTree(src_il_block,True,True);
    add_obj_kv(dst,il_block);
  }

  /*special case should not be moved
  node *dst_obj_parameters = node_GetItemByKey(dst,"nyx_parameters");
  if(dst_obj_parameters)
  {
    remove_obj_kv(dst,dst_obj_parameters);
    node_FreeTree(dst_obj_parameters);
  }
  node *src_obj_parameters = node_GetItemByKey(src,"nyx_parameters");
  if(src_obj_parameters!=NULL)
  {
    remove_obj_kv(src,src_obj_parameters);
    //node *pars = node_CopyTree(src_obj_parameters,True,True);
    //add_obj_kv(dst,pars);
    add_obj_kv(dst,src_obj_parameters);
  }*/



  node *dst_abp = node_GetItemByKey(src,"anonymous_block_parent");
  if(dst_abp)
    remove_obj_kv(dst,dst_abp);

  node *src_abp = node_GetItemByKey(src,"anonymous_block_parent");
  if(src_abp)
  {
    //printf("found abp in move\n");
    //fflush(stdout);
    node *il_block = node_CopyTree(src_abp,True,True);
    add_obj_kv(dst,il_block);
  }
  //TODO better object freeing , accounting complex sub objects
  node *dst_members = node_GetItemByKey(dst,"members");
  node *src_members = node_GetItemByKey(src,"members");
  node_ItemIterationReset(dst_members);
  while(node_ItemIterationUnfinished(dst_members))
  {
    node *dst_member = node_ItemIterate(dst_members);
    dec_obj_refcount(dst_member);
    node_SetParent(dst_member,NULL);
    add_garbage(state,dst_member);
  }
  node_RemoveItem(dst,dst_members);
  node_ClearItems(dst_members);
  node_FreeTree(dst_members);

  node *members = node_CopyTree(src_members,True,True);
  add_obj_kv(dst,members);


  node *dst_items = node_GetItemByKey(dst,"items");
  node *src_items = node_GetItemByKey(src,"items");
  if(dst_items)
  {
    node_ItemIterationReset(dst_items);
    while(node_ItemIterationUnfinished(dst_items))
    {
      node *dst_item = node_ItemIterate(dst_items);
      dec_obj_refcount(dst_item);
      node_SetParent(dst_item,NULL);
      add_garbage(state,dst_item);
    }
    node_RemoveItem(dst,dst_items);
    node_ClearItems(dst_items);
    node_FreeTree(dst_items);
  }
  if(src_items)
  {
    node *items = node_CopyTree(src_items,True,True);
    //printf("copied items tree:\n");
    //node_PrintTree(items);
    add_obj_kv(dst,items);
  }


  node *dst_privates = node_GetItemByKey(dst,"privates");
  node *src_privates = node_GetItemByKey(src,"privates");

  node_RemoveItem(dst,dst_privates);
  node_FreeTree(dst_privates);

  node *privates = node_CopyTree(src_privates,True,True);
  add_obj_kv(dst,privates);//TODO ref_counts not correct
  //add_obj_int(base,"refcount",0);

}

void open_modules(node *state)
{
  #ifdef USE_SYS
    sys_binding_open(state);
  #endif
  #ifdef USE_FILE
    file_binding_open(state);
  #endif
  #ifdef USE_SOCKETS
    sockets_binding_open(state);
  #endif
  #ifdef USE_HTTP
    http_binding_open(state);
  #endif
  #ifdef USE_CURL
    curl_binding_open(state);
  #endif
  #ifdef USE_MICROHTTPD
    microhttpd_binding_open(state);
  #endif
  #ifdef USE_WEBSOCKETS
    websockets_binding_open(state);
  #endif
}

void close_modules(node *state)
{
  #ifdef USE_SYS
    sys_binding_close(state);
  #endif
  #ifdef USE_FILE
    file_binding_close(state);
  #endif
  #ifdef USE_SOCKETS
    sockets_binding_close(state);
  #endif
  #ifdef USE_HTTP
    http_binding_close(state);
  #endif
  #ifdef USE_CURL
    curl_binding_close(state);
  #endif
  #ifdef USE_MICROHTTPD
    microhttpd_binding_close(state);
  #endif
  #ifdef USE_WEBSOCKETS
    websockets_binding_close(state);
  #endif
}

node *create_nyx_state(node *base_class,node *block_class)
{
  node *state = create_obj("nyx_state");
  node *garbage = create_obj("garbage");
  //node *class_types = create_obj("class_types");
  node *blocks = create_obj("blocks");
  add_obj_kv(state,base_class);
  add_obj_kv(state,block_class);

  node *_true = create_class_instance(base_class);
  set_obj_string(_true,"name","True");
  set_obj_int(_true,"value",1);
  set_obj_int(_true,"no_gc",1);
  add_obj_kv(state,_true);

  node *_false = create_class_instance(base_class);
  set_obj_string(_false,"name","False");
  set_obj_int(_false,"value",0);
  set_obj_int(_false,"no_gc",1);
  add_obj_kv(state,_false);

  node *_null = create_class_instance(base_class);
  set_obj_string(_null,"name","Null");
  set_obj_int(_null,"value",0);//TODO change to real null value
  set_obj_int(_null,"no_gc",1);
  add_obj_kv(state,_null);

  add_obj_kv(state,garbage);

  node *modules = create_class_instance(base_class);
  set_obj_string(modules,"name","modules");
  set_obj_int(modules,"value",1);
  set_obj_int(modules,"no_gc",1);
  add_obj_kv(state,modules);
  add_obj_int(state,"execution_level",0);

  //add_obj_kv(state,class_types);
  add_obj_kv(state,blocks);
  return(state);
}

node *execute_obj(node *state,node *obj,node *block,node *parameters,BOOL execute_block,BOOL execute_in_block,BOOL resolve_obj)
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
      node *subs = NULL;
      node *block_parameters = node_GetItemByKey(obj,"nyx_parameters");
      if(block_parameters)
      {
        subs = create_obj("subs");
        long pars_num = node_GetItemsNum(pars);
        long block_pars_num = 0;
        block_pars_num = node_GetItemsNum(block_parameters);
        long p_index = 0;
        long max_num = pars_num;
        if(block_pars_num>pars_num)
          max_num = block_pars_num;
        node *tmp_parent = node_GetParent(obj);
        node_SetParent(obj,NULL);
        node *abp = node_GetItemByKey(obj,"anonymous_block_parent");
        node *tmp_abp_value = NULL;
        if(abp!=NULL)
        {
          tmp_abp_value = (node*)(unsigned long)node_GetValue(abp);
          node_SetNode(abp,NULL);
        }
        while(p_index<max_num)
        {
          node *par = NULL;
          node *block_par = NULL;
          node *sub = NULL;
          if(p_index<pars_num)
            par = node_GetItem(pars,p_index);
          if(p_index<block_pars_num) 
            block_par = node_GetItem(block_parameters,p_index);
          if(block_par && !strcmp(node_GetKey(block_par),"nyx_statement"))
          {
            sub = evaluate_statement(state,block_par,obj,0,NULL,False,NULL);
          } 
          if(par)
          {
            if(sub)
            {
              node *oldm = get_member_non_recursive(obj,get_obj_name(sub));
              if(oldm)
              {
                remove_member(obj,oldm);
                dec_obj_refcount(oldm);
                add_garbage(state,oldm);
              }
              node *proxy = create_proxy_object(state,par,get_obj_name(sub));
              add_member(obj,proxy);
              inc_obj_refcount(proxy);
              inc_obj_refcount(par);
              //printf("added par proxy for removal:%s (%x)\n",get_obj_name(proxy),proxy);
              //fflush(stdout);
              node_AddItem(subs,proxy);
            }
            else
            {
              //printf("added par for removal:%s (%x)\n",get_obj_name(par),par);//removed because of double frees with eval code
              //fflush(stdout);
              //node_AddItem(subs,par);
            }
          } 
          p_index++;
        }
        if(abp!=NULL)
          node_SetNode(abp,tmp_abp_value);
        node_SetParent(obj,tmp_parent);
      }
      else
      {
        if(node_GetItemsNum(pars))
        {
          //printf("preparing arguments\n");
          node *base_class = get_base_class(state);
          node *arguments = create_class_instance(base_class);
          set_obj_string(arguments,"name","arguments");
          node *items = create_obj("items");
          add_obj_kv(arguments,items);
          node_ItemIterationReset(pars);
          long item_index=0;
          while(node_ItemIterationUnfinished(pars))
          {
            node *par = node_ItemIterate(pars);
            //node *item = node_CopyTree(par,True,True);
            node *item = create_proxy_object(state,par,get_obj_name(par));
            //node *item = par;
            //reset_obj_refcount(item);
            node_AddItem(items,item);
            node_SetParent(item,items);
            set_obj_int(item,"item_index",item_index);
            inc_obj_refcount(item);
            inc_obj_refcount(par);
            item_index++;
          }
          //node_PrintTree(arguments);    
          add_member(obj,arguments);
          inc_obj_refcount(arguments);
        }
      }
      if(execute_in_block)
        value = evaluate_block_instance_in(state,obj,block);
      else
        value = evaluate_block_instance(state,obj);
      node *arguments = get_member_non_recursive(obj,"arguments");
      if(arguments!=NULL)
      {
        remove_member(obj,arguments);
        node *old_items = node_GetItemByKey(arguments,"items");
        node_ItemIterationReset(old_items);
        while(node_ItemIterationUnfinished(old_items))
        {
          node *arg = node_ItemIterate(old_items);
          node_SetParent(arg,NULL);
          dec_obj_refcount(arg);
          add_garbage(state,arg);
        }   
        node_ClearItems(old_items); 
        dec_obj_refcount(arguments);
        add_garbage(state,arguments);
      }
      if(subs && node_GetItemsNum(subs)) //remove all parameters added - TODO doesnt account for in function block assigns
      {
        node_ItemIterationReset(subs);
        while(node_ItemIterationUnfinished(subs))
        {
          node *sub = node_ItemIterate(subs);
          if(remove_member(obj,sub))
          {
            dec_obj_refcount(sub);
            add_garbage(state,sub);
          }
        }
      }
      if(subs)
      {
        node_ClearItems(subs);
        node_FreeTree(subs);
      }

    }
    else
    {
      value = obj;
    }
  }
  else if(!strcmp(get_obj_type(obj),"function"))
  {
    //if(execute_block)
    //{
      node *handler = node_GetItemByKey(obj,"handler");
      if(handler!=NULL)
      {
        //node *(*real_handler)(node*,node*,node*,node*) = (nyx_function_handler)node_GetValue(handler);
        //TODO to make this thread safe
        //add a locking mechanism here - should be sufficient
        //to make the whole interpreter threadsafe without the need for locks in scripts
        //in eval check val find+add (lock there too i guess)
        nyx_function_handler real_handler = (nyx_function_handler)(unsigned long)node_GetValue(handler);
        node *parent = node_GetParent(node_GetParent(obj));
        value = real_handler(state,parent,block,pars);
      }
    //}
    //else
    //{
    //  value = obj;
    //}
  }
  else if(!strcmp(get_obj_type(obj),"proxy"))
  {
    if(resolve_obj)
    {
      //printf("resolving proxy obj: %x,%s\n",obj,get_obj_name(obj));
      //fflush(stdout);
      value=get_proxy_target(obj);
    }
    else 
      value = obj;
    //printf("resolved to: %x\n",value);
    //node_PrintTree(value);
    //fflush(stdout);
    //printf("resolved to: %x,%s\n",value,get_obj_name(value));
    //fflush(stdout);
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
    if(node_GetValue(abp))
    {
      found_obj = get_member((node*)(unsigned long)node_GetValue(abp),key);
      if(found_obj==NULL)
      {
        found_obj = search_block_path_for_member((node*)(unsigned long)node_GetValue(abp),key);
      }
    }
  }
  return(found_obj);
}

node *evaluate_statement(node *state,node *statement,node *block,long iteration_start_index,char *preop,BOOL resolve_obj,node *extra_search_block)//,long auto_add_new)
{
  node_SetItemIterationIndex(statement,iteration_start_index);
  long index = iteration_start_index;
  node *parameters = create_obj("parameters");
  node *actual_obj = block;
  node *prev_token = NULL;
  node *base_class = get_base_class(state);
  char *use_preop = preop;
  //inc_execution_level(state);
  long is_sub_id = 0;


  while(node_ItemIterationUnfinished(statement))
  {
    node *token = node_ItemIterate(statement);
    
    /*printf("------\nevaluating next token in %x\n",block);
    node_Print(token,True,False);
    //node_PrintTree(token);
    printf("------\n");
    printf("actual_obj: %x ,block: %x ,extra_search_block: %x exe_level: %d\n",actual_obj,block,extra_search_block,get_execution_level(state));
    printf("------\n");
    fflush(stdout);
    */

    if(!strcmp(node_GetKey(token),"nyx_parameters"))
    {
      node_ItemIterationReset(token);
      while(node_ItemIterationUnfinished(token))
      {
        node *parameter_token = node_ItemIterate(token);
        actual_obj = evaluate_statement(state,parameter_token,block,0,NULL,True,NULL);
      }

      if(use_preop!=NULL)
      {
        char *preop_prefixed=str_Cat("PRE",use_preop);
        node *found_obj = get_member_part(actual_obj,preop_prefixed);
        if(found_obj!=NULL)
        {
          actual_obj = execute_obj(state,found_obj,block,NULL,False,False,True);
        }
        free(preop_prefixed);
        free(use_preop);
        use_preop=NULL;
      }
    }
    else if(!strcmp(node_GetKey(token),"nyx_array") && actual_obj!=block) //TODO better 'with' array operation check
    //else if(prev_token!=NULL && !strcmp(node_GetKey(prev_token),"val") && !strcmp(node_GetKey(token),"nyx_array"))
    {
      if(!node_GetItemsNum(token))
      {
        //printf("no search key defined\n");
        node *items = node_GetItemByKey(actual_obj,"items");
        if(items==NULL)
        {
          node *items = create_obj("items");
          add_obj_kv(actual_obj,items);
          add_class_object_function(actual_obj,"item_at",nyxh_item_at);
          add_class_object_function(actual_obj,"append_item",nyxh_append_item);
        }
        node *base_class = node_GetItemByKey(state,"nyx_object");
        node *item = create_class_instance(base_class);
        node_SetParent(item,items);
        node_AddItem(items,item);
        inc_obj_refcount(item);
        long item_index = get_items_new_index(items);
        set_obj_int(item,"item_index",item_index);
        actual_obj = item;
        if(resolve_obj)
          actual_obj = resolve_object(actual_obj);
      }
      else
      {
        node *key_obj = evaluate_statement(state,node_GetItem(token,0),block,0,NULL,True,NULL);
        //printf("searching for key in :%x, %s\n",actual_obj,get_obj_name(actual_obj));
        //node_PrintTree(actual_obj);
        if(node_GetItemByKey(actual_obj,"items")!=NULL)
        {
          node *found_obj = get_item(state,actual_obj,key_obj,True);
          if(resolve_obj)
            found_obj = resolve_object(found_obj);
          //printf("found:%x (%s)\n",found_obj,get_obj_name(found_obj));
          //fflush(stdout);
          //node_PrintTree(found_obj);
          actual_obj = found_obj;
        }
        else //check if string is used as array with a number as key - return a single char string
        {
          node *real_value = node_GetItemByKey(actual_obj,"value");
          node *real_key_value = node_GetItemByKey(key_obj,"value");
          if(node_GetType(real_value)==NODE_TYPE_STRING && node_GetType(real_key_value)==NODE_TYPE_SINT32)
          {
            char *tok = str_Sub(node_GetString(real_value),node_GetSint32(real_key_value),1);
            actual_obj = create_class_instance(base_class);
            reset_obj_refcount(actual_obj);
            set_obj_string(actual_obj,"name","character");
            set_obj_string(actual_obj,"value",tok);
            free(tok);
            reset_obj_refcount(actual_obj);
            add_garbage(state,actual_obj);
          }
        }
      }
    }
    else if(!strcmp(node_GetKey(token),"nyx_array"))
    {
      node *child = create_class_instance(base_class);
      //printf("creating array: %x\n",child);
      node *items = create_obj("items");
      add_obj_kv(child,items);
      add_class_object_function(child,"item_at",nyxh_item_at);
      add_class_object_function(child,"append_item",nyxh_append_item);
      add_garbage(state,child);
      node_ItemIterationReset(token);
      long item_index = 0;
      while(node_ItemIterationUnfinished(token))
      {
        node *parameter_token = node_ItemIterate(token);
        node *array_obj = evaluate_statement(state,parameter_token,block,0,NULL,True,NULL);
        node *item = node_CopyTree(array_obj,True,True);
        reset_obj_refcount(item);
        //printf("creating array item: %x\n",item);
        node_SetParent(item,items);
        inc_obj_refcount(item);
        node_AddItem(items,item);
        set_obj_int(item,"item_index",item_index);
        item_index++;
      }
      actual_obj=child;
    }
    else if(!strcmp(node_GetKey(token),"nyx_block"))
    {
      node *block_class_instance = create_block_class_instance(state,token);
      node *peek = node_ItemPeek(statement);
      if(peek!=NULL && !strcmp(node_GetKey(peek),"nyx_parameters"))
      {
        node *exe_parameters = create_obj("parameters");
        node *sub_parameters = node_ItemIterate(statement);
        node_ItemIterationReset(sub_parameters);
        while(node_ItemIterationUnfinished(sub_parameters))
        {
          node *parameter_token = node_ItemIterate(sub_parameters);
          node *sub_obj = evaluate_statement(state,parameter_token,block,0,NULL,True,NULL);
          node_AddItem(exe_parameters,sub_obj);
        }
        index++;
        actual_obj = execute_obj(state,block_class_instance,block,exe_parameters,True,False,True);
        add_garbage(state,block_class_instance);
      }
      else
      {
        set_obj_node(block_class_instance,"anonymous_block_parent",block);
        add_garbage(state,block_class_instance);
        actual_obj = block_class_instance;
      }
    }
    else if(!strcmp(node_GetKey(token),"str"))
    {
      node *child = create_class_instance(base_class);
      set_obj_string(child,"value",(char*)(unsigned long)node_GetValue(token));
      add_garbage(state,child);
      actual_obj = child;
      //printf("created string: %x (%s)\n",child,node_GetValue(token));
      if(use_preop!=NULL)// && (node_ItemPeek(statement)==NULL || (node_ItemPeek(statement)!=NULL && !strcmp(node_GetKey(node_ItemPeek(statement)),"ops") && !strcmp(node_GetString(node_ItemPeek(statement)),"."))) )
      {
        if( !(node_ItemPeek(statement)!=NULL && !strcmp(node_GetKey(node_ItemPeek(statement)),"ops") && !strcmp(node_GetString(node_ItemPeek(statement)),".")) )
        {
          char *preop_prefixed=str_Cat("PRE",use_preop);
          node *found_obj = get_member_part(actual_obj,preop_prefixed);
          if(found_obj!=NULL)
          { 
            actual_obj = execute_obj(state,found_obj,block,NULL,False,False,True);
          }
          free(preop_prefixed);
          free(use_preop);
          use_preop=NULL;
        }
      }
    }
    else if(!strcmp(node_GetKey(token),"val"))
    {
      if(node_GetType(token) == NODE_TYPE_STRING)
      {
        if(strcmp((char*)(unsigned long)node_GetValue(token),"@"))
        {
          node *found_obj = get_member(actual_obj,(char*)(unsigned long)node_GetValue(token));
          if(found_obj==NULL && !is_sub_id)
          {
            found_obj = search_block_path_for_member(actual_obj,(char*)(unsigned long)node_GetValue(token));
            //if(found_obj)
            //  printf("found in block path\n");
          }
          if(found_obj==NULL && !is_sub_id && extra_search_block)
          {
            found_obj = get_member(extra_search_block,(char*)(unsigned long)node_GetValue(token));
            //if(found_obj)
            //  printf("found in extra members\n");
            if(found_obj==NULL)
            {
              found_obj = search_block_path_for_member(extra_search_block,(char*)(unsigned long)node_GetValue(token));
              //if(found_obj)
              //  printf("found in extra search block path\n");
            }
          }
          if(found_obj==NULL)
          {
            node *child = create_class_instance(base_class);
            set_obj_string(child,"name",node_GetString(token));
            
            if(is_sub_id)
            {
              node *oldm=get_member_non_recursive(actual_obj,node_GetString(token));
              if(oldm)
              {
                //printf("replacing old sub\n");
                remove_member(actual_obj,oldm);
                dec_obj_refcount(oldm);
                add_garbage(state,oldm);
              }
              //printf("adding sub id: %x (%s) to %x (%s)\n",child,get_obj_name(child),actual_obj,get_obj_name(actual_obj));
              //fflush(stdout);
              add_member(actual_obj,child); 
              inc_obj_refcount(child);
              is_sub_id = 0;
            }
            //add_member(actual_obj,child); //Dont add members automatically only in assign do a namespace add
            //inc_obj_refcount(child);
            actual_obj = child;
            add_garbage(state,actual_obj);
            //printf("created obj %x in %x: %x,%s\n",child,block,actual_obj,get_obj_name(actual_obj));
            //fflush(stdout);
            //node_PrintTree(actual_obj);
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
            //printf("found obj %x,%s\n",found_obj,get_obj_name(found_obj));
            //fflush(stdout);
            if(resolve_obj)
            {
              node *peek = node_ItemPeek(statement);
              if(peek!=NULL && !strcmp(node_GetKey(peek),"ops") && !strcmp((char*)(unsigned long)node_GetValue(peek),"="))
              {
              }
              else
              {
                found_obj = resolve_object(found_obj);
              }
            }
            //printf(" in %x,%s\n",node_GetParent(node_GetParent(found_obj)),get_obj_name(node_GetParent(node_GetParent(found_obj))));
            //fflush(stdout);
            //node_ItemPeek(statement)!=NULL && 
            if(((node_ItemPeek(statement)==NULL)||(strcmp(node_GetKey(node_ItemPeek(statement)),"nyx_parameters"))) && ( !strcmp(get_obj_type(found_obj),"function") || !strcmp(get_obj_type(found_obj),"nyx_il_block") ) )
            {
                //printf("found function without brackets:%s\n",node_GetValue(token));
                node *exe_parameters = create_obj("parameters");
                actual_obj = execute_obj(state,found_obj,block,exe_parameters,False,False,True);
                //node_PrintTree(actual_obj);
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
                      node *sub_obj = evaluate_statement(state,parameter_token,block,0,NULL,True,NULL);
                      //node *sub_obj = evaluate_statement(state,parameter_token,found_obj,0,NULL,True,block);
                      node_AddItem(exe_parameters,sub_obj);
                    }
                    index++;
                  }
                  actual_obj = execute_obj(state,found_obj,block,exe_parameters,False,False,True);
                }
                else
                {
                  node *peek = node_ItemPeek(statement);
                  if(peek!=NULL && !strcmp(node_GetKey(peek),"nyx_parameters"))
                  {
                    node *sub_parameters = node_ItemIterate(statement);
                    node *peek = node_ItemPeek(statement);
                    if(peek!=NULL && !strcmp(node_GetKey(peek),"ops") && !strcmp((char*)(unsigned long)node_GetValue(peek),"="))
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
                    {//TODO move argument and parameter definitions here instead of in execute_obj (less irradic behaviour)
                      node *exe_parameters = create_obj("parameters");
                      node *subs = create_obj("subs");
                      node_ItemIterationReset(sub_parameters);
                      node *fmembers=node_GetItemByKey(found_obj,"members");
                      while(node_ItemIterationUnfinished(sub_parameters))
                      {
                        node *parameter_token = node_ItemIterate(sub_parameters);
                        //node *sub_obj = evaluate_statement(state,parameter_token,block,0,NULL,True);
                        node *sub_obj = evaluate_statement(state,parameter_token,found_obj,0,NULL,True,block);
                        if(node_GetParent(sub_obj)==fmembers)
                        {
                          //printf("sub was added to function mark for deletion:%s (%x)\n",get_obj_name(sub_obj),sub_obj);
                          node_AddItem(subs,sub_obj);
                        }
                        node_AddItem(exe_parameters,sub_obj);
                      }
                      actual_obj = execute_obj(state,found_obj,block,exe_parameters,True,False,True);

                      node_ItemIterationReset(subs);
                      while(node_ItemIterationUnfinished(subs))
                      {
                        node *sub = node_ItemIterate(subs);
                        remove_member(found_obj,sub);
                        //printf("eval:removing sub:%s (%x)\n",get_obj_name(sub),sub);
                        dec_obj_refcount(sub);
                        add_garbage(state,sub);
                      }
                      node_ClearItems(subs);
                      node_FreeTree(subs);
                      //if(extra_search_block)
                      //  actual_obj = execute_obj(state,found_obj,extra_search_block,exe_parameters,True,True,True);
                      //else
                      //  actual_obj = execute_obj(state,found_obj,block,exe_parameters,True,False,True);
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
      }
      else if(node_GetType(token) == NODE_TYPE_SINT32)
      {
        node *child = create_class_instance(base_class);
        node *peek = node_ItemPeek(statement);
        node *peek_further = node_ItemPeekFurther(statement,2);
        if( peek!=NULL && !strcmp(node_GetKey(peek),"ops") && !strcmp(node_GetString(peek),".") && (peek_further!=NULL && node_GetType(peek_further) == NODE_TYPE_SINT32) )
        {
          node_ItemIterate(statement);
          node *tenth_val = node_ItemIterate(statement);
          index+=2;
          long tenth = node_GetSint32(tenth_val);
          char *t=str_FromLong(tenth);
          long tlen=strlen(t);
          free(t);
          double d=((double)node_GetSint32(token))+(((double)tenth)/pow(10,tlen));
          //select/poll events
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
        //printf("created int: %x (%d)\n",child,node_GetSint32(token));
        actual_obj = child;
        add_garbage(state,child);
      }
      if(use_preop!=NULL)// && (node_ItemPeek(statement)==NULL || (node_ItemPeek(statement)!=NULL && !strcmp(node_GetKey(node_ItemPeek(statement)),"ops") && !strcmp(node_GetString(node_ItemPeek(statement)),"."))) )
      {
        if( !(node_ItemPeek(statement)!=NULL && !strcmp(node_GetKey(node_ItemPeek(statement)),"ops") && !strcmp(node_GetString(node_ItemPeek(statement)),".")) )
        {
          char *preop_prefixed=str_Cat("PRE",use_preop);
          node *found_obj = get_member_part(actual_obj,preop_prefixed);
          if(found_obj!=NULL)
          { 
            actual_obj = execute_obj(state,found_obj,block,NULL,False,False,True);
          }
          free(preop_prefixed);
          free(use_preop);
          use_preop=NULL;
        }
      }
    }
    else if(!strcmp(node_GetKey(token),"ops"))
    {
        if(!strcmp(node_GetString(token),"."))
        {
          
          //node *sub_token = node_ItemIterate(statement);
          //printf("found . : %s\n",node_GetString(sub_token));
          //node *found_obj = get_member(actual_obj,node_GetString(sub_token));
          //index+=1;
          //block = actual_obj;
          //if(found_obj)
          //  actual_obj = found_obj;
          is_sub_id = 1;
        }
        else
        {
          if(prev_token!=NULL)
          {
            if(!strcmp(node_GetKey(prev_token),"ops"))
            {
              use_preop = str_Copy(node_GetString(token));
            }
          }
          else
          {
            use_preop = str_Copy(node_GetString(token));
          }
          //printf("searching member part:[%s][%s][%s]\n",get_obj_name(actual_obj),node_GetValue(token),use_preop);
          //fflush(stdout);
          //node_PrintTree(actual_obj);
          node *found_obj = get_member_part(actual_obj,(char*)(unsigned long)node_GetValue(token));
          if(found_obj!=NULL && (use_preop == NULL || !strlen(use_preop)))
          {
            //printf("found this:[%s] of [%s]\n",get_obj_name(found_obj),get_obj_name(node_GetParent(node_GetParent(found_obj))));
            long token_len = strlen(node_GetString(token));
            char *found_name = get_obj_name(found_obj);
            if((long)strlen(found_name)<token_len)
            {
              long found_name_len=strlen(found_name);
              use_preop=str_Sub(node_GetString(token),found_name_len,token_len-found_name_len);
            }

            if(index+1<node_GetItemsNum(statement))
            {
              node *sub_obj = evaluate_statement(state,statement,block,index+1,use_preop,True,NULL);
              node_AddItem(parameters,sub_obj);
            }
            actual_obj = found_obj;
            actual_obj = execute_obj(state,actual_obj,block,parameters,False,False,True);
            //printf("evaluate obj in ret op out:[%s]\n",get_obj_name(actual_obj));
            //dec_execution_level(state);
            return(actual_obj);
          }
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
  actual_obj = execute_obj(state,actual_obj,block,parameters,False,False,resolve_obj);
  return(actual_obj);
}

char *check_block_flag(node *state)
{
  char *ret = NULL;
  node *block_flag=node_GetItemByKey(state,"block_flag");
  if(block_flag!=NULL) 
  {
    char *bf = node_GetString(block_flag);
    if(!strcmp(bf,"exit"))
    {
      //node *nexit_code=node_GetItemByKey(state,"exit_code");
      //int exit_code = (int)node_GetSint32(nexit_code);
      //printf("exiting with code: %d\n",exit_code);
      set_obj_int(state,"block_break_count",0);
      //set_obj_string(state,"block_flag","");
      ret = str_Copy("exit");
      return(ret);
    }
    node *block_break_count = node_GetItemByKey(state,"block_break_count");
    long count = node_GetSint32(block_break_count);
    if(count==0 && strlen(bf))
    {
      set_obj_int(state,"block_break_count",0);
      ret = str_Copy(bf);
      set_obj_string(state,"block_flag","");
    }
    else if(strlen(bf))
    {
      set_obj_int(state,"block_break_count",count-1);
      ret = str_Copy("break");
    }
  }
  return(ret);
}

node *evaluate_block(node *state,node *block)
{
  node *block_class_instance = create_block_class_instance(state,block);
  node *blocks = node_GetItemByKey(state,"blocks");
  add_obj_kv(blocks,block_class_instance);
  return(evaluate_block_instance_in(state,block_class_instance,block_class_instance));
}

node *evaluate_block_in(node *state,node *block,node *master_block)
{
  node *block_class_instance = create_block_class_instance(state,block);
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
  int gc_threshold=0;/*2*/ //TODO remove execution level based gc - just save the return obj and everything should be fine
  inc_execution_level(state);
  node *ret = NULL;
  node *il_block = node_GetItemByKey(block_class_instance,"nyx_block");
  if(!node_GetItemsNum(il_block))
  {
      node *base_class = get_base_class(state);
      ret = create_class_instance(base_class);
      reset_obj_refcount(ret);
      add_garbage(state,ret);
      dec_execution_level(state);
      return(ret);
  }
  else
  {
    node_ItemIterationReset(il_block);
    while(node_ItemIterationUnfinished(il_block))
    {
      node *nyx_statement = node_ItemIterate(il_block);
      ret = evaluate_statement(state,nyx_statement,block,0,NULL,True,NULL);
      char *block_flag=check_block_flag(state);
      if(block_flag)
      {
        if(!strcmp(block_flag,"break"))
        {
          free(block_flag);
          free_garbage(state,get_execution_level(state)+gc_threshold,ret);
          dec_execution_level(state);
          return(ret);
        }
        else if(!strcmp(block_flag,"exit"))
        {
          free(block_flag);
          free_garbage(state,get_execution_level(state)+gc_threshold,ret);
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
  }
  if(ret==NULL)
  {
    printf("eval return NULL\n");
  }
  //printf("block %x (%d) (exe block: %x) finished: ret_obj: %x\n",block_class_instance,get_execution_level(state),block,ret);
  free_garbage(state,get_execution_level(state)+gc_threshold,ret);
  dec_execution_level(state);
  add_garbage(state,ret);
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
    ret = execute_obj(state,found_obj,block,parameters,False,False,True);
  }
  else
  {
    ret = execute_obj(state,found_obj,block,NULL,True,False,True);
  }
  return(ret);
}


void nyx_add_parameter(node *state,char *parameter)
{
  #ifdef USE_SYS
  node *nyx_stream = nyx_LoadString(parameter);
  if(nyx_stream!=NULL)
  {
    node *nyx_block = node_GetItemByKey(nyx_stream,"nyx_block");
    if(nyx_block!=NULL)
    {
      add_obj_kv(state,nyx_block);
      node_RemoveItem(nyx_stream,nyx_block);
      node_FreeTree(nyx_stream);
      node *ret_obj = evaluate_block(state,nyx_block);  
      node_RemoveItem(state,nyx_block);
      sys_add_parameter(state,ret_obj);
      node_FreeTree(nyx_block);
    }
  }
  #endif
}

node *init_nyx(void)
{
  node *base_class = create_class_object();
  node *block_class = create_block_class_object(base_class);
  node *nyx_state = create_nyx_state(base_class,block_class);
  open_modules(nyx_state);
  return(nyx_state);
}

void close_nyx(node *state)
{
  close_modules(state);
  free_garbage(state,0,NULL);
  node *garbage = node_GetItemByKey(state,"garbage");
  node_ClearItems(garbage);
  node_FreeTree(state);
}

