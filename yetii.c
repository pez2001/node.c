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

#include "yetii.h"


node *convert_json(node *state,node *output,char *json)
{
  node *base_class = node_GetItemByKey(state,"yeti_object");
  //printf("converting string to json:%s\n",json);
  node *json_tree = json_LoadString(json);
  //node_PrintTree(json_tree);
  node_ItemIterationReset(json_tree);
  while(node_ItemIterationUnfinished(json_tree))
  {
    node *sub = node_ItemIterate(json_tree);
    //printf("%s:%d\n",node_GetKey(sub),node_GetType(sub));
    node *child = create_class_instance(base_class);
    set_obj_string(child,"name",node_GetKey(sub));
    if(node_GetType(sub)==NODE_TYPE_STRING)
      set_obj_string(child,"value",node_GetValue(sub));
    else if(node_GetType(sub)==NODE_TYPE_UINT32)
      set_obj_int(child,"value",(long)node_GetUint32(sub));
    else if(node_GetType(sub)==NODE_TYPE_INT)
      set_obj_int(child,"value",node_GetInt(sub));
    else if(node_GetType(sub)==NODE_TYPE_SINT32)
      set_obj_int(child,"value",node_GetSint32(sub));
    add_member(output,child);
  }
  node_FreeTree(json_tree);
  return(NULL);
}

void append_http_query_array_item(node *state,node *array,char *value)
{
  long value_len = strlen(value);
  long offset =0;
  char *val=CreateEmptyString();
  char *key=CreateEmptyString();
  int in_key=1;
  node *base_class = node_GetItemByKey(state,"yeti_object");

  //printf("adding value :[%s]%d<br>\n",value,value_len);
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
  //printf("parsed [%s]:[%s]<br>\n",key,val);
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
  //char *query="ds=34&t1=34";
  if(query==NULL || !strlen(query))
    return;
  node *items = create_obj("items");
  add_obj_kv(value,items);
  
  long query_len = strlen(query);
  long offset =0;
  char *val=CreateEmptyString();
  
  //printf("query:[%s]<br>\n",query);
  while(offset<query_len)
  {
    char qc=query[offset];
    if(qc=='&')
    {
      //printf("value seperator:[%s]<br>\n",val);
      append_http_query_array_item(state,items,val);
      free(val);
      val = CreateEmptyString();
    }
    else
      val=AddCharToString(val,qc);
    offset++;
  }
  //printf("last value :[%s]<br>\n",val);
  if(strlen(val))
    append_http_query_array_item(state,items,val);
  free(val);
}

char *convert_to_string(long i)
{
  char *ret=NULL;
  long len = snprintf(NULL,0,"%d",i);
  if(len)
  {
    ret = (char*)malloc(len+1);
    snprintf(ret,len+1,"%d",i);
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
  //memset(tmp, 0, strlen(a) + strlen(b) + 1);
  memcpy(tmp, a, strlen(a));
  memcpy(tmp + strlen(a), b, strlen(b)+1);
  return(tmp);
}

char *StringMult(char *a,long count)
{
  if(a == NULL || !count)
      return(NULL);
  char *tmp = (char*)malloc((strlen(a)*count) + 1);
  long i=0;
  //printf("multi string:%s,%d\n",a,count);
  for(i=0;i<count;i++)
  {
    memcpy(tmp+(i*strlen(a)), a, strlen(a));
  }
  memset(tmp+(strlen(a)*count) + 0, 0, 1);
  return(tmp);
}

char *Substring(char *a,long start,long len)
{
  long e = start + len;
  if(e> strlen(a))
    e = strlen(a) - start;
  else
    e = len;
  char *tmp = (char*)malloc(e + 1);
  memset(tmp+e+1, 0, 1);
  memcpy(tmp, a+start, e);
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
  //printf("created obj :%s @:%x\n",name,obj);
  return(obj);
}

void add_obj_kv(node *obj,node *kv)
{
  node_AddItem(obj,kv);
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

void set_obj_node(node *obj,char *key,node *n)
{
  node *kv = node_GetItemByKey(obj,key);
  if(kv==NULL)
    add_obj_node(obj,key,n);
  else
  {
    node_SetType(kv,NODE_TYPE_NODE);
    //printf("setting node value:%x\n",n);
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
  //node_SetSint32(refcount,1);
}

void reset_obj_refcount(node *obj)
{
  node *refcount = node_GetItemByKey(obj,"refcount");
  node_SetSint32(refcount,0);
}

void dec_obj_refcount(node *obj)
{
  node *refcount = node_GetItemByKey(obj,"refcount");
  //node_SetSint32(refcount,0);

  if((node_GetSint32(refcount)-1)<0)
  {
    //printf("decrementing beneath 0 refs!\n");
    //int i=1/0;
  }
  node_SetSint32(refcount,node_GetSint32(refcount)-1);
}

long get_obj_refcount(node *obj)
{
  node *refcount = node_GetItemByKey(obj,"refcount");
  return(node_GetSint32(refcount));
}

void add_obj_double(node *obj,char *key,double d)
{
  node *kv = node_Create();
  node_SetKey(kv,key);
  node_SetDouble(kv,d);
  add_obj_kv(obj,kv); 
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
      //node_PrintTree(member);
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
  node *base = create_obj("yeti_object");
  add_obj_string(base,"type","class");
  add_obj_string(base,"name",obj_name);
  //add_obj_kv(base,create_obj("base_class_instance"));
  add_obj_kv(base,create_obj("base_class_type"));
  add_obj_kv(base,create_obj("members"));
  add_obj_kv(base,create_obj("value"));
  set_obj_int(base,"value",0);
  add_obj_int(base,"refcount",0);
  return(base);
}

void add_class_object_internal_function(node *class,node *base_class,char *method_name)//void *addr
{
  node *method = create_base_obj_layout(method_name);
  set_obj_string(method,"type","function");
  //set_obj_node(base,"base_class_instance",class);
  set_obj_node(method,"base_class_type",base_class);
  inc_obj_refcount(method);
  add_member(class,method);
}

node *create_file_class_object(void)
{
  node *base = create_base_obj_layout("file");
  add_class_object_internal_function(base,base,"read");
  add_class_object_internal_function(base,base,"readall");
  add_class_object_internal_function(base,base,"write");
  add_class_object_internal_function(base,base,"open");
  add_class_object_internal_function(base,base,"close");
  add_class_object_internal_function(base,base,"seek");
  add_class_object_internal_function(base,base,"set_mode");
  add_class_object_internal_function(base,base,"flush");
  return(base);
}

/*implement class construction via classic constructor*/

node *create_class_object(void)
{
  node *base = create_base_obj_layout("object");
  add_class_object_internal_function(base,base,"=");
  add_class_object_internal_function(base,base,":");
  add_class_object_internal_function(base,base,"+");
  add_class_object_internal_function(base,base,"print");
  add_class_object_internal_function(base,base,"println");
  add_class_object_internal_function(base,base,"input");
  add_class_object_internal_function(base,base,"http_query");
  add_class_object_internal_function(base,base,"str");
  add_class_object_internal_function(base,base,"int");
  add_class_object_internal_function(base,base,"len");
  add_class_object_internal_function(base,base,"-");
  add_class_object_internal_function(base,base,"/");
  add_class_object_internal_function(base,base,"*");

  add_class_object_internal_function(base,base,"<");
  add_class_object_internal_function(base,base,">");
  add_class_object_internal_function(base,base,"==");
  add_class_object_internal_function(base,base,"<=");
  add_class_object_internal_function(base,base,">=");
  add_class_object_internal_function(base,base,"!=");
  add_class_object_internal_function(base,base,"!");
  add_class_object_internal_function(base,base,"?");
  add_class_object_internal_function(base,base,"??");
  add_class_object_internal_function(base,base,"break");
  add_class_object_internal_function(base,base,"continue");
  add_class_object_internal_function(base,base,"restart");
  add_class_object_internal_function(base,base,"import");
  add_class_object_internal_function(base,base,"eval");
  add_class_object_internal_function(base,base,"open");
  add_class_object_internal_function(base,base,"close");
  add_class_object_internal_function(base,base,"from_json");
  add_class_object_internal_function(base,base,"to_json");
  
  //add_class_object_internal_function(base,"get");
  add_class_object_internal_function(base,base,"test");
  //add_member(base,create_file_class_object());

  return(base);
}

node *create_block_obj(node *base_class,node *block)
{
  node *base = create_class_instance(base_class);//,"block");
  set_obj_string(base,"name","block");
  set_obj_string(base,"type","yeti_il_block");
  node *il_block = node_CopyTree(block,True,True);
  add_obj_kv(base,il_block);  
  return(base);
}

void free_execution_obj(node *exe_obj)
{
  node *parameters = node_GetItemByKey(exe_obj,"parameters");
  if(parameters!=NULL)
  {
    node_ItemIterationReset(parameters);
    while(node_ItemIterationUnfinished(parameters))
    {
      node *parameter = node_ItemIterate(parameters);
      if(!strcmp(node_GetKey(parameter),"exe_object"))
      {
        //printf("freeing exe_object in parameters\n");
        free_execution_obj(parameter);
      }
    }
  }
  node_ClearItems(parameters);
  node *sub_exe_obj = node_GetItemByKey(exe_obj,"sub_exe_object");
  if(sub_exe_obj!=NULL)
    free_execution_obj(sub_exe_obj);
  //node *obj = node_GetItem(exe_obj,0);
  node_Free(parameters,False);
  node_ClearItems(exe_obj);
  node_Free(exe_obj,False);
  //node_Free(parameters);
}

void add_garbage(node *state,node *obj)
{
  node *garbage = node_GetItemByKey(state,"garbage");
  if(node_HasItem(garbage,obj))
  {
    //printf("dupe in garbage\n");
    return;
  }  
  if(node_GetParent(obj)!=NULL)
  {
    printf("tried add gc obj with a parent:%x\n",obj);
    //int x=1/0;
  }

  node_AddItem(garbage,obj);
  //printf("adding to garbage:%x (%d)\n",obj,get_obj_refcount(obj));
}

void free_garbage(node *state)
{
  //node_PrintTree(state->garbage);
  node *garbage = node_GetItemByKey(state,"garbage");
  //printf("freeing garbage:%d\n",node_GetItemsNum(garbage));
  node_ItemIterationReset(garbage);
  while(node_ItemIterationUnfinished(garbage))
  {
    node *gc = node_ItemIterate(garbage);
    //node_PrintTree(gc);
    if(get_obj_refcount(gc)<=0)
    {
      if(node_GetParent(gc)!=NULL)
      {
        //printf("tried to free obj with a parent:%x\n",gc);
        node_FreeTree(gc);
      }
      else 
      {
        //printf("freeing:%x\n",gc);
        node_FreeTree(gc);
      }
    }
    else
    {
      //printf("object not collected ->still has refs:%d\n",get_obj_refcount(gc));

    }
  }
  node_ClearItems(garbage);
}

node *create_execution_obj(node *method,node *parameters,node *sub_execution_obj)
{
  node *base = create_obj("exe_object");
  node_AddItem(base,method);
  node_AddItem(base,parameters);
  if(sub_execution_obj!=NULL)
  {
    node_AddItem(base,sub_execution_obj);
    node_SetKey(sub_execution_obj,"sub_exe_object");
  }
  return(base);
}

node *create_yeti_state(node *yeti_block,node *base_class)
{
  node *state = create_obj("yeti_state");
  node *garbage = create_obj("garbage");
  node *class_types = create_obj("class_types");
  add_obj_kv(state,class_types);
  add_obj_kv(state,garbage);
  add_obj_kv(state,yeti_block);
  add_obj_kv(state,base_class);
  return(state);
}

void prepare_execution_parameters(node *state,node *parameters,node *block,node *output)
{
  node_ItemIterationReset(parameters);
  while(node_ItemIterationUnfinished(parameters))
  {
    node *token = node_ItemIterate(parameters);
    if(!strcmp(node_GetKey(token),"exe_object"))
    {
      //printf("getting par:\n");
      //node *sub = execute_obj(state,token,block,execute_block);//,tmp_dont_execute_block);
      node *sub = execute_obj(state,token,block,False);//,tmp_dont_execute_block);
      //node_PrintTree(sub);
      node_AddItem(output,sub);
      //printf("adding parameter sub:%x:(%d)\n",sub,get_obj_refcount(sub));
      //add_garbage(state,sub);
    }
  }
}

node *execute_obj(node *state,node *execution_obj,node *block,BOOL execute_block)//,BOOL dont_execute_block)
{
  //deep first recursive
  //BOOL tmp_dont_execute_block = False;
  node *value = NULL;
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  //node_PrintTree(execution_obj);
  //printf("executing:%x\n",exe_obj);

  //node *garbage = node_GetItemByKey(state,"garbage");

  /*
  if(strcmp(node_GetKey(exe_obj),"array") && !strcmp(get_obj_type(exe_obj),"function"))
  {
    node *parent = node_GetParent(node_GetParent(exe_obj));
    char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
    if(!strcmp(name,"="))
    {
      //printf("found function\n");
      //node_PrintTree(parent);
      node *empty_shell = node_GetItemByKey(parent,"empty_function_shell");   
      if(empty_shell!=NULL)
        tmp_dont_execute_block = True;
    }
  } 
  */
  /*
  node_ItemIterationReset(parameters);
  while(node_ItemIterationUnfinished(parameters))
  {
    node *token = node_ItemIterate(parameters);
    if(!strcmp(node_GetKey(token),"exe_object"))
    {
      //printf("getting par:\n");
      //node *sub = execute_obj(state,token,block,execute_block);//,tmp_dont_execute_block);
      node *sub = execute_obj(state,token,block,False);//,tmp_dont_execute_block);
      //node_PrintTree(sub);
      node_AddItem(real_parameters,sub);
      //printf("adding parameter sub:%x:(%d)\n",sub,get_obj_refcount(sub));
      //add_garbage(state,sub);
    }
  }
  */

  //prepare_execution_parameters(state,parameters,block,real_parameters);


  if(!strcmp(node_GetKey(exe_obj),"array")) //TODO MOVE to evaluate
  {
    node *base_class = node_GetItemByKey(state,"yeti_object");
    value = create_class_instance(base_class);
    //node_SetParent(value,NULL)
    node *items = create_obj("items");
    add_obj_kv(value,items);
    add_garbage(state,value);
    node_ItemIterationReset(exe_obj);
    while(node_ItemIterationUnfinished(exe_obj))
    {
      node *token = node_ItemIterate(exe_obj);
      if(!strcmp(node_GetKey(token),"exe_object"))
      {
        node *sub = execute_obj(state,token,block,execute_block);//,False);
        add_garbage(state,sub);
        node *sub_c = node_CopyTree(sub,True,True);
        node_AddItem(items,sub_c);
        node_SetParent(sub_c,items);
        inc_obj_refcount(sub_c);
        free_execution_obj(token);
      }
    }
    node_ClearItems(exe_obj);
    node_Free(exe_obj,True);
  }/*
  else if(!strcmp(get_obj_type(exe_obj),"yeti_il_block"))
  {
        value = exe_obj;
  }*/
  else if(!strcmp(get_obj_type(exe_obj),"yeti_il_block"))
  {
    //node *exe_block = node_GetItemByKey(exe_obj,"execute_block");
    //printf("yeti_block\n");
    //node_PrintTree(exe_obj);
    //if( exe_block!= NULL && !strcmp(node_GetString(exe_block),"True") && dont_execute_block==False )
    if(execute_block)
    {
      prepare_execution_parameters(state,parameters,exe_obj,real_parameters);
      //prepare_execution_parameters(state,parameters,block,real_parameters);
      //node *il_block = node_GetItemByKey(exe_obj,"yeti_block");
      //printf("executing block:%x\n",exe_obj);
      //node_PrintTree(exe_obj);
      
      //node *bmembers = node_GetItemByKey(block,"members");
      //node *old_block_parent = node_GetParent(exe_obj);
      //node_SetParent(exe_obj,bmembers);

      node *block_parameters = node_GetItemByKey(exe_obj,"yeti_parameters");
      if(block_parameters!=NULL)
      {
        node_ItemIterationReset(block_parameters);
        long p_index = 0;
        while(node_ItemIterationUnfinished(block_parameters))
        {
          node *token = node_ItemIterate(block_parameters);
          if(!strcmp(node_GetKey(token),"yeti_statement"))
          {
            node *tmp_parent = node_GetParent(exe_obj);
            node_SetParent(exe_obj,NULL);

            node *obj = evaluate_statement(state,token,exe_obj,0);
            //node *sub = execute_obj(state,obj,exe_obj,execute_block);
            node *sub = execute_obj(state,obj,exe_obj,False);
            node_SetParent(exe_obj,tmp_parent);

            node *obj_name = node_GetItemByKey(sub,"name");
            
            node *real_parent = node_GetParent(sub);

            node_RemoveItem(real_parent,sub);
            node_SetParent(sub,NULL);
            dec_obj_refcount(sub);
            //printf("adding exec sub:%x(rm:%x)\n",sub,node_GetParent(real_parent));
            add_garbage(state,sub);

            value = node_CopyTree(node_GetItem(real_parameters,p_index),True,True);
            reset_obj_refcount(value);
            set_obj_string(value,"name",node_GetString(obj_name));
            node_AddItem(real_parent,value);
            node_SetParent(value,real_parent);
            //node_SetParent(value,node_GetParent(node_GetItem(real_parameters,p_index)));
            inc_obj_refcount(value);
            node *obj_parameters = node_GetItemByKey(sub,"yeti_parameters");
            if(obj_parameters!=NULL)
            {
              node_RemoveItem(sub,obj_parameters);
              add_obj_kv(value,node_CopyTree(obj_parameters,True,True));
            }
            //printf("added exe value(item:%s):%x\n",node_GetString(obj_name),value);
            //node_PrintTree(value);
            free_execution_obj(obj);
          }
          p_index++;
        }
      }
      else
      {
        //printf("block has no parameters\n");
      }
      //set_obj_string(exe_obj,"execute_block","False");
      value = evaluate_block_instance(state,exe_obj);
      //value = evaluate_block_instance_in(state,exe_obj,block);
      //node_SetParent(exe_obj,old_block_parent);
      //value = exe_obj;
      //printf("exec block finished\n");
    }
    else
    {
      value = exe_obj;
    }
  }
  else if(!strcmp(get_obj_type(exe_obj),"function"))
  {
    char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
    node *parent = node_GetParent(node_GetParent(exe_obj));
    if(!strcmp(name,"+"))
    {
      prepare_execution_parameters(state,parameters,block,real_parameters);
      value = node_CopyTree(parent,True,True);
      node_SetParent(value,NULL);
      reset_obj_refcount(value);
      //add_garbage(state,parent);
      add_garbage(state,value);
      node *value2 = node_GetItem(real_parameters,0);
      node *real_value = node_GetItemByKey(value,"value");
      node *real_value2 = node_GetItemByKey(value2,"value");
      if(node_GetType(real_value)==NODE_TYPE_SINT32 && node_GetType(real_value2)==NODE_TYPE_SINT32)
        node_SetSint32(real_value,node_GetSint32(real_value)+node_GetSint32(real_value2));
      else if(node_GetType(real_value)==NODE_TYPE_STRING && node_GetType(real_value2)==NODE_TYPE_STRING)
      {
        char *cat_string=StringCat(node_GetString(real_value),node_GetString(real_value2));
        node_SetString(real_value,cat_string);
        free(cat_string);
      }
      else if(node_GetType(real_value)==NODE_TYPE_STRING && node_GetType(real_value2)==NODE_TYPE_SINT32)
      {
        char *num=convert_to_string(node_GetSint32(real_value2));
        char *cat_string=StringCat(node_GetString(real_value),num);
        node_SetString(real_value,cat_string);
        free(cat_string);
        free(num);
      }
      //printf("++\n");
      //node_PrintTree(real_value);
    }
    else if(!strcmp(name,"-"))
    {
      prepare_execution_parameters(state,parameters,block,real_parameters);
      value = node_CopyTree(parent,True,True);
      node_SetParent(value,NULL);
      reset_obj_refcount(value);
      //add_garbage(state,parent);
      add_garbage(state,value);
      node *value2 = node_GetItem(real_parameters,0);
      node *real_value = node_GetItemByKey(value,"value");
      node *real_value2 = node_GetItemByKey(value2,"value");
      node_SetSint32(real_value,node_GetSint32(real_value)-node_GetSint32(real_value2));
    }
    else if(!strcmp(name,"/"))
    {
      prepare_execution_parameters(state,parameters,block,real_parameters);
      value = node_CopyTree(parent,True,True);
      node_SetParent(value,NULL);
      reset_obj_refcount(value);
      //add_garbage(state,parent);
      add_garbage(state,value);
      node *value2 = node_GetItem(real_parameters,0);
      node *real_value = node_GetItemByKey(value,"value");
      node *real_value2 = node_GetItemByKey(value2,"value");
      node_SetSint32(real_value,node_GetSint32(real_value)/node_GetSint32(real_value2));
    }
    else if(!strcmp(name,"*"))
    {
      prepare_execution_parameters(state,parameters,block,real_parameters);
      value = node_CopyTree(parent,True,True);
      node_SetParent(value,NULL);
      reset_obj_refcount(value);
      //add_garbage(state,parent);
      add_garbage(state,value);
      node *value2 = node_GetItem(real_parameters,0);
      node *real_value = node_GetItemByKey(value,"value");
      node *real_value2 = node_GetItemByKey(value2,"value");
      if(node_GetType(real_value)==NODE_TYPE_SINT32 && node_GetType(real_value2)==NODE_TYPE_SINT32)
        node_SetSint32(real_value,node_GetSint32(real_value)*node_GetSint32(real_value2));
      else if(node_GetType(real_value)==NODE_TYPE_STRING && node_GetType(real_value2)==NODE_TYPE_SINT32)
      {
        long mult=node_GetSint32(real_value2);
        long i=0;
        char *dst = NULL;
        if(mult>0)
        {
          //for(i=0;i<mult;i++)
          //{
          dst = StringMult(node_GetString(real_value),mult);
          //}
          node_SetString(real_value,dst);
          free(dst);
        }
      }
    }
    else if(!strcmp(name,"<"))
    {
      prepare_execution_parameters(state,parameters,block,real_parameters);
      value = node_CopyTree(parent,True,True);
      node_SetParent(value,NULL);
      reset_obj_refcount(value);
      //add_garbage(state,parent);
      add_garbage(state,value);
      node *value2 = node_GetItem(real_parameters,0);
      node *real_value = node_GetItemByKey(value,"value");
      node *real_value2 = node_GetItemByKey(value2,"value");
      //node_PrintTree(value);

      //printf("%x << %x\n",value,value2);
      //node_PrintTree(value2);

      //printf("%d<%d = %d\n",node_GetSint32(real_value),node_GetSint32(real_value2),node_GetSint32(real_value)<node_GetSint32(real_value2));
      node_SetSint32(real_value,node_GetSint32(real_value)<node_GetSint32(real_value2));
      //printf("==\n");
      //node_PrintTree(real_value);
    }
    else if(!strcmp(name,">"))
    {
      prepare_execution_parameters(state,parameters,block,real_parameters);
      value = node_CopyTree(parent,True,True);
      node_SetParent(value,NULL);
      reset_obj_refcount(value);
      //add_garbage(state,parent);
      add_garbage(state,value);
      node *value2 = node_GetItem(real_parameters,0);
      node *real_value = node_GetItemByKey(value,"value");
      node *real_value2 = node_GetItemByKey(value2,"value");
      //printf("%d>%d = %d\n",node_GetSint32(real_value),node_GetSint32(real_value2),node_GetSint32(real_value)>node_GetSint32(real_value2));
      node_SetSint32(real_value,node_GetSint32(real_value)>node_GetSint32(real_value2));
    }
    else if(!strcmp(name,"=="))
    {
      prepare_execution_parameters(state,parameters,block,real_parameters);
      value = node_CopyTree(parent,True,True);
      node_SetParent(value,NULL);
      reset_obj_refcount(value);
      //add_garbage(state,parent);
      add_garbage(state,value);
      node *value2 = node_GetItem(real_parameters,0);
      node *real_value = node_GetItemByKey(value,"value");
      node *real_value2 = node_GetItemByKey(value2,"value");
      //printf("%d==%d = %d\n",node_GetSint32(real_value),node_GetSint32(real_value2),node_GetSint32(real_value)==node_GetSint32(real_value2));
      //node_PrintTree(parent);
      node_SetSint32(real_value,node_GetSint32(real_value)==node_GetSint32(real_value2));
    }
    else if(!strcmp(name,"<="))
    {
      prepare_execution_parameters(state,parameters,block,real_parameters);
      value = node_CopyTree(parent,True,True);
      node_SetParent(value,NULL);
      reset_obj_refcount(value);
      //add_garbage(state,parent);
      add_garbage(state,value);
      node *value2 = node_GetItem(real_parameters,0);
      node *real_value = node_GetItemByKey(value,"value");
      node *real_value2 = node_GetItemByKey(value2,"value");
      node_SetSint32(real_value,node_GetSint32(real_value)<=node_GetSint32(real_value2));
    }
    else if(!strcmp(name,">="))
    {
      prepare_execution_parameters(state,parameters,block,real_parameters);
      value = node_CopyTree(parent,True,True);
      node_SetParent(value,NULL);
      reset_obj_refcount(value);
      //add_garbage(state,parent);
      add_garbage(state,value);
      node *value2 = node_GetItem(real_parameters,0);
      node *real_value = node_GetItemByKey(value,"value");
      node *real_value2 = node_GetItemByKey(value2,"value");
      node_SetSint32(real_value,node_GetSint32(real_value)>=node_GetSint32(real_value2));
    }
    else if(!strcmp(name,"!="))
    {
      prepare_execution_parameters(state,parameters,block,real_parameters);
      //node_PrintTree(parent);
      value = node_CopyTree(parent,True,True);
      //printf("(copy)\n");
      node_SetParent(value,NULL);
      reset_obj_refcount(value);
      //add_garbage(state,parent);
      add_garbage(state,value);
      //node_PrintTree(value);
      //fflush(stdout);
      /*int x=1/0;*/
      node *value2 = node_GetItem(real_parameters,0);
      node *real_value = node_GetItemByKey(value,"value");
      node *real_value2 = node_GetItemByKey(value2,"value");
      //printf("(%x)%d!=(%x)%d = %d\n",value,node_GetSint32(real_value),value2,node_GetSint32(real_value2),node_GetSint32(real_value)!=node_GetSint32(real_value2));
      node_SetSint32(real_value,node_GetSint32(real_value)!=node_GetSint32(real_value2));
    }
    else if(!strcmp(name,":"))
    {
      prepare_execution_parameters(state,parameters,block,real_parameters);
      value = node_GetItemByKey(parent,"value");
      node *value2_parent = node_GetItem(real_parameters,0);
      node *value2 = node_GetItemByKey(value2_parent,"value");
      node_SetType(value,node_GetType(value2));
      node_SetValue(value,node_GetValue(value2),True,True);
      value = parent;
    }
    else if(!strcmp(name,"="))
    {
      prepare_execution_parameters(state,parameters,block,real_parameters);
      node *obj_name = node_GetItemByKey(parent,"name");
      //node_PrintTree(parent);
      node *real_parent = node_GetParent(parent);
      //printf("removing :(%x) %s from PP:%x\n",parent,node_GetString(obj_name),node_GetParent(real_parent));
      //node_Print(real_parent,True,True);
      //node_PrintTree(parent);
      //fflush(stdout);
      int r = node_RemoveItem(real_parent,parent);
      node_SetParent(parent,NULL);
      if(r==-1)
      {
        printf("error item not removed\n");
      }
      dec_obj_refcount(parent);
      //printf("adding = parent:%x\n",parent);
      add_garbage(state,parent);
      //node_PrintTree(real_parameters);
      value = node_CopyTree(node_GetItem(real_parameters,0),True,True);
      reset_obj_refcount(value);
      set_obj_string(value,"name",node_GetString(obj_name));
      node_AddItem(real_parent,value);
      inc_obj_refcount(value);
      node_SetParent(value,real_parent);
      node *obj_parameters = node_GetItemByKey(parent,"yeti_parameters");
      if(obj_parameters!=NULL)
      {
        node *pars = node_CopyTree(obj_parameters,True,True);
        //add_obj_kv(value,obj_parameters);
        add_obj_kv(value,pars);
      }
      //printf("new value(%s) after = %x\n",get_obj_name(value),value);
      //set_obj_string(value,"execute_block","False");
      //node_PrintTree(get_value(value));

    }
    else if(!strcmp(name,"print") || !strcmp(name,"println"))
    {
      prepare_execution_parameters(state,parameters,block,real_parameters);
      if(node_GetItemsNum(real_parameters))
      {
        node_ItemIterationReset(real_parameters);
        while(node_ItemIterationUnfinished(real_parameters))
        {
          node *token = node_ItemIterate(real_parameters);
          node *items = node_GetItemByKey(token,"items");
          if(items!=NULL)
          {
            node_ItemIterationReset(items);
            while(node_ItemIterationUnfinished(items))
            {
              node *item = node_ItemIterate(items);
              node_Print(node_GetItemByKey(item,"value"),False,False);
            } 
          }
          else
          {
            //printf("[v:%x]\n",node_GetItemByKey(token,"value"));
            node_Print(node_GetItemByKey(token,"value"),False,False);
          }
        }      
      }else
      {
        //printf("print has no parameters\n");
        if(parent!=NULL && parent != block)
          node_Print(node_GetItemByKey(parent,"value"),False,False);
      }
      if(!strcmp(name,"println"))
        printf("\n");
      node *base_class = node_GetItemByKey(state,"yeti_object");
      value = create_class_instance(base_class);
      reset_obj_refcount(value);
      add_garbage(state,value);
    }
    else if(!strcmp(name,"?"))
    {
      //?(expression,true_func,false_func,loop_while_true); 
      //execute based on expression value (0,1,..) ,loops if needed
      //returns(expression)
      //prepare_execution_parameters(state,parameters,block,real_parameters);
      node *expression_block = node_GetItem(parameters,0);
      node *true_block = node_GetItem(parameters,1);
      node *false_block = node_GetItem(parameters,2);
      //node *loop = node_GetItem(real_parameters,3);
      node *loop_exe = node_GetItem(parameters,3);

      node *loop = execute_obj(state,loop_exe,block,False);//,tmp_dont_execute_block);

      node *loop_value = node_GetItemByKey(loop,"value");
      long lv = node_GetSint32(loop_value);

      node *expression_block_obj = node_GetItem(expression_block,0);
      node *true_block_obj = node_GetItem(true_block,0);
      node *false_block_obj = node_GetItem(false_block,0);
      node *bmembers = node_GetItemByKey(block,"members");
      node *old_expression_block_parent = node_GetParent(expression_block_obj);
      node *old_true_block_parent = node_GetParent(true_block_obj);
      node *old_false_block_parent = node_GetParent(false_block_obj);
      /*if(!strcmp(get_obj_type(expression_block_obj),"yeti_il_block"))
        node_SetParent(expression_block_obj,bmembers);
      if(!strcmp(get_obj_type(true_block_obj),"yeti_il_block"))
        node_SetParent(true_block_obj,bmembers);
      if(!strcmp(get_obj_type(false_block_obj),"yeti_il_block"))
        node_SetParent(false_block_obj,bmembers);
      */
      if(lv)
      {
        node *exp_obj = execute_obj(state,expression_block,block,True);
        while(node_GetSint32(node_GetItemByKey(exp_obj,"value")))
        {
          node *eval=execute_obj(state,true_block,block,True);
          node *block_flag=node_GetItemByKey(state,"block_flag");
          if(block_flag!=NULL) //MOVE TO sep func
          {
            char *bf = node_GetString(block_flag);
            if(!strcmp(bf,"restart"))
            {
              set_obj_string(state,"block_flag","");
            } 
            else if(!strcmp(bf,"continue"))
            {
              set_obj_string(state,"block_flag","");
            } 
            else if(!strcmp(bf,"break"))
            {
              node *block_break_count = node_GetItemByKey(state,"block_break_count");
              long count = node_GetSint32(block_break_count);
              if(!(count-1))
              {
                set_obj_int(state,"block_break_count",0);
                set_obj_string(state,"block_flag","");
                break;
              }
              else
              {
                set_obj_int(state,"block_break_count",count-1);
                break;
              }
            } 
          }
          exp_obj = execute_obj(state,expression_block,block,True);
        }
        node *blk_val=execute_obj(state,false_block,block,True);
      }
      else
      {
        node *exp_val=execute_obj(state,expression_block,block,True);
        node *blk_val=NULL;
        if(node_GetSint32(node_GetItemByKey(exp_val,"value")))
        {
            blk_val=execute_obj(state,true_block,block,True);
        }
        else
          blk_val=execute_obj(state,false_block,block,True);
      }
      /*
      if(!strcmp(get_obj_type(expression_block_obj),"yeti_il_block"))
        node_SetParent(expression_block_obj,old_expression_block_parent);
      if(!strcmp(get_obj_type(true_block_obj),"yeti_il_block"))
        node_SetParent(true_block_obj,old_true_block_parent);
      if(!strcmp(get_obj_type(false_block_obj),"yeti_il_block"))
        node_SetParent(false_block_obj,old_false_block_parent);
      if(old_expression_block_parent==NULL)
        add_garbage(state,expression_block_obj);
      if(old_true_block_parent==NULL)
        add_garbage(state,true_block_obj);
      if(old_false_block_parent==NULL)
        add_garbage(state,false_block_obj);
      */

      node *base_class = node_GetItemByKey(state,"yeti_object");
      value = create_class_instance(base_class);
      reset_obj_refcount(value);
      add_garbage(state,value);
    }
    else if(!strcmp(name,"??"))
    {
      //?(expression,true_func,false_func,loop_while_true); 
      //execute based on expression value (0,1,..) ,loops if needed
      //returns(expression)
      node *init_block = node_GetItem(parameters,0);
      node *expression_block = node_GetItem(parameters,1);
      node *true_block = node_GetItem(parameters,2);
      node *false_block = node_GetItem(parameters,3);
      node *loop_exe = node_GetItem(parameters,4);
      node *loop = execute_obj(state,loop_exe,block,False);//,tmp_dont_execute_block);



      node *init_block_obj = node_GetItem(init_block,0);
      node *expression_block_obj = node_GetItem(expression_block,0);
      node *true_block_obj = node_GetItem(true_block,0);
      node *false_block_obj = node_GetItem(false_block,0);
      node *bmembers = node_GetItemByKey(block,"members");
      node *old_init_block_parent = node_GetParent(init_block_obj);
      node *old_expression_block_parent = node_GetParent(expression_block_obj);
      node *old_true_block_parent = node_GetParent(true_block_obj);
      node *old_false_block_parent = node_GetParent(false_block_obj);
      if(!strcmp(get_obj_type(init_block_obj),"yeti_il_block"))
        node_SetParent(init_block_obj,bmembers);
      if(!strcmp(get_obj_type(expression_block_obj),"yeti_il_block"))
        node_SetParent(expression_block_obj,bmembers);
      if(!strcmp(get_obj_type(true_block_obj),"yeti_il_block"))
        node_SetParent(true_block_obj,bmembers);
      if(!strcmp(get_obj_type(false_block_obj),"yeti_il_block"))
        node_SetParent(false_block_obj,bmembers);



      //node *init_ret_obj = execute_obj(state,init_block,block,True);
      node *init_eval = execute_obj(state,init_block,block,True);
      //node *init_eval=evaluate_block_instance_in(state,init_block_obj,block);

      node *loop_value = node_GetItemByKey(loop,"value");
      long lv = node_GetSint32(loop_value);

      if(lv)
      {
        node *exp_obj = execute_obj(state,expression_block,block,True);
        while(node_GetSint32(node_GetItemByKey(exp_obj,"value")))
        {
          node *eval=execute_obj(state,true_block,block,True);
          node *block_flag=node_GetItemByKey(state,"block_flag");
          if(block_flag!=NULL)
          {
            char *bf = node_GetString(block_flag);
            if(!strcmp(bf,"restart"))
            {
              set_obj_string(state,"block_flag","");
            } 
            else if(!strcmp(bf,"continue"))
            {
              set_obj_string(state,"block_flag","");
            } 
            else if(!strcmp(bf,"break"))
            {
              node *block_break_count = node_GetItemByKey(state,"block_break_count");
              long count = node_GetSint32(block_break_count);
              if(!(count-1))
              {
                set_obj_int(state,"block_break_count",0);
                set_obj_string(state,"block_flag","");
                break;
              }
              else
              {
                set_obj_int(state,"block_break_count",count-1);
                break;
              }
            } 
          }
          exp_obj = execute_obj(state,expression_block,block,True);
        }
      }
      else
      {
        node *exp_val=execute_obj(state,expression_block,block,True);
        node *blk_val=NULL;
        if(node_GetSint32(node_GetItemByKey(exp_val,"value")))
          blk_val=execute_obj(state,true_block,block,True);
        else
          blk_val=execute_obj(state,false_block,block,True);
      }

      if(!strcmp(get_obj_type(init_block_obj),"yeti_il_block"))
        node_SetParent(init_block_obj,old_init_block_parent);
      if(!strcmp(get_obj_type(expression_block_obj),"yeti_il_block"))
        node_SetParent(expression_block_obj,old_expression_block_parent);
      if(!strcmp(get_obj_type(true_block_obj),"yeti_il_block"))
        node_SetParent(true_block_obj,old_true_block_parent);
      if(!strcmp(get_obj_type(false_block_obj),"yeti_il_block"))
        node_SetParent(false_block_obj,old_false_block_parent);

      node *base_class = node_GetItemByKey(state,"yeti_object");
      value = create_class_instance(base_class);
      reset_obj_refcount(value);
      add_garbage(state,value);
    }
    /*else if(!strcmp(name,"??"))
    {
      node *init_block = node_GetItem(real_parameters,0);
      node *expression_block = node_GetItem(parameters,1);
      node *true_block = node_GetItem(parameters,2);
      node *false_block = node_GetItem(parameters,3);
      node *loop = node_GetItem(real_parameters,4);
      node *loop_value = node_GetItemByKey(loop,"value");
      long lv = node_GetSint32(loop_value);
      node *init_eval=evaluate_block_instance_in(state,init_block,block);
      //add_garbage(state,init_eval);
      if(lv)
      {
        //while(node_GetSint32(node_GetItemByKey(evaluate_block_instance(state,expression_block),"value")))
        while(node_GetSint32(node_GetItemByKey(execute_obj(state,expression_block,block,True),"value")))
        {
          //node *eval=evaluate_block_instance(state,true_block);
          node *eval=execute_obj(state,true_block,block,True);
          add_garbage(state,eval);
        }
      }
      else
      {
        //node *exp_val=evaluate_block_instance(state,expression_block);
        node *exp_val=execute_obj(state,expression_block,block,True);
        node *blk_val=NULL;
        if(node_GetSint32(node_GetItemByKey(exp_val,"value")))
        {
          blk_val=execute_obj(state,true_block,block,True);
          //blk_val=evaluate_block_instance(state,true_block);
        }
        else
        {
          blk_val=execute_obj(state,false_block,block,True);
          //blk_val=execute_obj(state,false_block,block,True);
        }
        add_garbage(state,exp_val);
        add_garbage(state,blk_val);
      }
      dec_obj_refcount(init_block);
      node *base_class = node_GetItemByKey(state,"yeti_object");
      value = create_class_instance(base_class);
      reset_obj_refcount(value);
      add_garbage(state,value);
    }*/
    else if(!strcmp(name,"break"))
    {
      prepare_execution_parameters(state,parameters,block,real_parameters);
      set_obj_string(state,"block_flag","break");
      if(node_GetItemsNum(real_parameters)>0)
      {
        node *count = node_GetItem(real_parameters,0);
        node *real_value = node_GetItemByKey(count,"value");
        set_obj_int(state,"block_break_count",node_GetSint32(real_value));
        //value = node_CopyTree(parent,True,True);
        //add_obj_kv(state,);
        //printf("called break with count:%d\n",node_GetSint32(real_value));

      }
      else
      {
        set_obj_int(state,"block_break_count",1);
        //printf("called break\n");
      }
      node *base_class = node_GetItemByKey(state,"yeti_object");
      value = create_class_instance(base_class);
      //node_SetParent(value,NULL);
      reset_obj_refcount(value);
      add_garbage(state,value);
    }
    else if(!strcmp(name,"restart"))
    {
      set_obj_string(state,"block_flag","restart");
      node *base_class = node_GetItemByKey(state,"yeti_object");
      value = create_class_instance(base_class);
      //node_SetParent(value,NULL);
      reset_obj_refcount(value);
      add_garbage(state,value);
    }
    else if(!strcmp(name,"continue"))
    {
      set_obj_string(state,"block_flag","continue");
      node *base_class = node_GetItemByKey(state,"yeti_object");
      value = create_class_instance(base_class);
      //node_SetParent(value,NULL);
      reset_obj_refcount(value);
      add_garbage(state,value);
    }
    else if(!strcmp(name,"len"))
    {
      prepare_execution_parameters(state,parameters,block,real_parameters);
      node *base_class = node_GetItemByKey(state,"yeti_object");
      long len = 0;
      if(node_GetItemsNum(real_parameters))
        value = node_GetItem(real_parameters,0);
      else if(parent!=NULL && parent != block)
        value = parent;
      else
      {
        value = create_class_instance(base_class);
        node_SetParent(value,NULL);
        reset_obj_refcount(value);
        add_garbage(state,value);
      }
      node *items=node_GetItemByKey(value,"items");
      if(items!=NULL)
        len = node_GetItemsNum(items);
      node *real_value = node_GetItemByKey(value,"value");
      node_SetSint32(real_value,len);
    }
    else if(!strcmp(name,"input"))
    {
      //x a.input()
      //x a=input()
      //x input(a)
      prepare_execution_parameters(state,parameters,block,real_parameters);
      node *base_class = node_GetItemByKey(state,"yeti_object");
      char *line = CreateEmptyString();
      char c='\n';
      //while((c=getc(stdin))!='\n')
      while((c=fgetc(stdin))!='\n' && c!=EOF && c!=0)
      {
        line = AddCharToString(line,c);
      }

      if(node_GetItemsNum(real_parameters))
      {
        //printf("setting par\n");
        value = node_GetItem(real_parameters,0);
        //node_PrintTree(value);
      }
      else if(parent!=NULL && parent != block)
      { 
        //printf("setting parent:%s\n",node_GetString(get_obj_name(parent)));
        //node_Print(node_GetItemByKey(parent,"value"),False,False);
        value = parent;
      }
      else
      {
        value = create_class_instance(base_class);
        node_SetParent(value,NULL);
        reset_obj_refcount(value);
        add_garbage(state,value);
      }
      node *real_value = node_GetItemByKey(value,"value");
      node_SetString(real_value,line);
      free(line);
    }
    else if(!strcmp(name,"http_query"))
    {
      //returns http query vars as array
      prepare_execution_parameters(state,parameters,block,real_parameters);
      node *base_class = node_GetItemByKey(state,"yeti_object");
      if(node_GetItemsNum(real_parameters))
      {
        value = node_GetItem(real_parameters,0);
      }
      else if(parent!=NULL && parent != block)
      { 
        value = parent;
      }
      else
      {
        value = create_class_instance(base_class);
        node_SetParent(value,NULL);
        reset_obj_refcount(value);
        add_garbage(state,value);
      }
      node *real_value = node_GetItemByKey(value,"value");
      append_http_query_array(state,value);
    }
    else if(!strcmp(name,"int"))
    {
      //returns integer of string input
      prepare_execution_parameters(state,parameters,block,real_parameters);
      node *base_class = node_GetItemByKey(state,"yeti_object");
      node *value2 = NULL;
      if(node_GetItemsNum(real_parameters))
      {
        value2 = node_GetItem(real_parameters,0);
      }
      else if(parent!=NULL && parent != block)
      { 
        value2 = parent;
      }
      else
      {
        value2 = create_class_instance(base_class);
        node_SetParent(value2,NULL);
        reset_obj_refcount(value2);
        add_garbage(state,value2);
        set_obj_int(value2,"value",0);
      }
      node *real_value2 = node_GetItemByKey(value2,"value");
      value = create_class_instance(base_class);
      node_SetParent(value,NULL);
      reset_obj_refcount(value);
      add_garbage(state,value);
      node *real_value = node_GetItemByKey(value,"value");
      if(node_GetType(real_value2)==NODE_TYPE_SINT32)
        node_SetSint32(real_value,node_GetSint32(real_value2));
      else if(node_GetType(real_value2)==NODE_TYPE_STRING)
        node_SetSint32(real_value,atoi(node_GetString(real_value2)));
    }
    else if(!strcmp(name,"str"))
    {
      //returns string of integer input
      prepare_execution_parameters(state,parameters,block,real_parameters);
      node *base_class = node_GetItemByKey(state,"yeti_object");
      node *value2 = NULL;
      if(node_GetItemsNum(real_parameters))
      {
        value2 = node_GetItem(real_parameters,0);
      }
      else if(parent!=NULL && parent != block)
      { 
        value2 = parent;
      }
      else
      {
        value2 = create_class_instance(base_class);
        node_SetParent(value2,NULL);
        reset_obj_refcount(value2);
        add_garbage(state,value2);
        set_obj_string(value2,"value","");
      }
      node *real_value2 = node_GetItemByKey(value2,"value");
      value = create_class_instance(base_class);
      node_SetParent(value,NULL);
      reset_obj_refcount(value);
      add_garbage(state,value);
      node *real_value = node_GetItemByKey(value,"value");
      if(node_GetType(real_value2)==NODE_TYPE_SINT32)
        node_SetString(real_value,convert_to_string(node_GetSint32(real_value2)));
      else if(node_GetType(real_value2)==NODE_TYPE_STRING)
        node_SetString(real_value,node_GetString(real_value2));
    }
    else if(!strcmp(name,"test"))
    {
      prepare_execution_parameters(state,parameters,block,real_parameters);
      //node *parent = exe_obj;
      //value = node_CopyTree(parent,True,True);
      node *base_class = node_GetItemByKey(state,"yeti_object");
      value = create_class_instance(base_class);
      //set_obj_string(value,"type","object");
      //node_SetParent(value,NULL);

      reset_obj_refcount(value);
      add_garbage(state,value);
      //value = node_GetItemByKey(parent,"value");
      node *value2 = node_GetItem(real_parameters,0);
      node *real_value = node_GetItemByKey(value,"value");
      node *real_value2 = node_GetItemByKey(value2,"value");
      node_SetSint32(real_value,20+node_GetSint32(real_value2));
      //printf("test object\n");
      //node_PrintTree(value);
      //printf("test object end\n");
      //value = get_member(value,"test");
      //value = parent;
    }
    else if(!strcmp(name,"open"))
    {
      //returns io stream object
      prepare_execution_parameters(state,parameters,block,real_parameters);
      node *base_class = node_GetItemByKey(state,"yeti_object");
      node *filename = NULL;
      if(node_GetItemsNum(real_parameters))
      {
        filename = node_GetItem(real_parameters,0);
      }

      node *real_filename = node_GetItemByKey(filename,"value");
      //value = create_class_instance(base_class);
      value = create_file_class_object();
      node_SetParent(value,NULL);
      reset_obj_refcount(value);
      add_garbage(state,value);
      //node *real_value = node_GetItemByKey(value,"value");
      if(node_GetType(real_filename)==NODE_TYPE_STRING)
      {
        char *fname = node_GetString(real_filename);
        //node_SetString(real_value,);
        //printf("opening file:%s\n",fname);
        FILE *f = fopen(fname,"rb+");
        set_obj_node(value,"file_handle",f);
      }
    }
    else if(!strcmp(name,"close"))
    {
      //returns string of integer input
      prepare_execution_parameters(state,parameters,block,real_parameters);
      node *base_class = node_GetItemByKey(state,"yeti_object");
      /*node *value2 = NULL;
      if(node_GetItemsNum(real_parameters))
      {
        value2 = node_GetItem(real_parameters,0);
      }
      else if(parent!=NULL && parent != block)
      { 
        value2 = parent;
      }
      else
      {
        value2 = create_class_instance(base_class);
        node_SetParent(value2,NULL);
        reset_obj_refcount(value2);
        add_garbage(state,value2);
        set_obj_string(value2,"value","");
      }*/
      long ret = -1;
      if(parent!=NULL && parent != block)
      { 
        node *fvalue = parent;
        node *handle = node_GetItemByKey(fvalue,"file_handle");
        if(handle!=NULL)
        {
          FILE *fhandle = node_GetValue(handle);
          ret = fclose(fhandle);
          //printf("closed file\n");
        }
      }

      value = create_class_instance(base_class);
      //node_SetParent(value,NULL);
      reset_obj_refcount(value);
      add_garbage(state,value);
      node *real_value = node_GetItemByKey(value,"value");
      node_SetSint32(real_value,ret);
    }
    else if(!strcmp(name,"readall"))
    {
      //returns string with all file content
      prepare_execution_parameters(state,parameters,block,real_parameters);
      node *base_class = node_GetItemByKey(state,"yeti_object");
      /*node *value2 = NULL;
      if(node_GetItemsNum(real_parameters))
      {
        value2 = node_GetItem(real_parameters,0);
      }
      else if(parent!=NULL && parent != block)
      { 
        value2 = parent;
      }
      else
      {
        value2 = create_class_instance(base_class);
        node_SetParent(value2,NULL);
        reset_obj_refcount(value2);
        add_garbage(state,value2);
        set_obj_string(value2,"value","");
      }*/
      char *ret = NULL;
      if(parent!=NULL && parent != block)
      { 
        node *fvalue = parent;
        node *handle = node_GetItemByKey(fvalue,"file_handle");
        if(handle!=NULL)
        {
          FILE *fhandle = node_GetValue(handle);
          fseek(fhandle,0,SEEK_END);
          long len = ftell(fhandle);
          fseek(fhandle,0,SEEK_SET);
          //char *content =
          ret = (char*)malloc(len+1);
          memset(ret+len + 0, 0, 1);
          fread(ret,len,1,fhandle);
          //printf("reading file content:%s\n",ret);
        }
      }

      value = create_class_instance(base_class);
      //node_SetParent(value,NULL);
      reset_obj_refcount(value);
      add_garbage(state,value);
      node *real_value = node_GetItemByKey(value,"value");
      node_SetString(real_value,ret);
      free(ret);
    }
    else if(!strcmp(name,"from_json"))
    {
      //converts json string to objects
      prepare_execution_parameters(state,parameters,block,real_parameters);
      node *base_class = node_GetItemByKey(state,"yeti_object");
      node *value2 = NULL;
      if(node_GetItemsNum(real_parameters))
      {
        value2 = node_GetItem(real_parameters,0);
      }
      else if(parent!=NULL && parent != block)
      { 
        value2 = parent;
      }
      else
      {
        value2 = create_class_instance(base_class);
        node_SetParent(value2,NULL);
        reset_obj_refcount(value2);
        add_garbage(state,value2);
        set_obj_string(value2,"value","");
      }


      value = create_class_instance(base_class);
      //node_SetParent(value,NULL);
      reset_obj_refcount(value);
      add_garbage(state,value);
      node *real_value = node_GetItemByKey(value,"value");
      node *real_value2 = node_GetItemByKey(value2,"value");

      //if(parent!=NULL && parent != block)
      //{ 
      //}
      //printf("converting json:%s\n",node_GetString(real_value2));
      convert_json(state,value,node_GetString(real_value2));
      //node_SetString(real_value,ret);
    }
    else if(!strcmp(name,"import"))
    {
      //
      prepare_execution_parameters(state,parameters,block,real_parameters);
      node *base_class = node_GetItemByKey(state,"yeti_object");
      node *value2 = NULL;
      if(node_GetItemsNum(real_parameters))
      {
        value2 = node_GetItem(real_parameters,0);
      }
      else if(parent!=NULL && parent != block)
      { 
        value2 = parent;
      }
      else
      {
        value2 = create_class_instance(base_class);
        node_SetParent(value2,NULL);
        reset_obj_refcount(value2);
        add_garbage(state,value2);
        set_obj_string(value2,"value","");
      }


      value = create_class_instance(base_class);
      //node_SetParent(value,NULL);
      reset_obj_refcount(value);
      add_garbage(state,value);
      node *real_value = node_GetItemByKey(value,"value");
      node *real_value2 = node_GetItemByKey(value2,"value");

      //if(parent!=NULL && parent != block)
      //{ 
      //}
      //printf("converting json:%s\n",node_GetString(real_value2));
      convert_json(state,value,node_GetString(real_value2));
      //node_SetString(real_value,ret);
    }
    else if(!strcmp(name,"eval"))
    {
      //
      prepare_execution_parameters(state,parameters,block,real_parameters);
      node *base_class = node_GetItemByKey(state,"yeti_object");
      node *value2 = NULL;
      if(node_GetItemsNum(real_parameters))
      {
        //printf("eval parameter\n");
        value2 = node_GetItem(real_parameters,0);
      }
      else if(parent!=NULL && parent != block)
      { 
        value2 = parent;
        //printf("eval parent\n");
      }
      else
      {
        value2 = create_class_instance(base_class);
        node_SetParent(value2,NULL);
        reset_obj_refcount(value2);
        add_garbage(state,value2);
        set_obj_string(value2,"value","");
      }


      node *real_value2 = node_GetItemByKey(value2,"value");
      //printf("evaluating:%s\n",node_GetString(real_value2));
      node *yeti_stream = yeti_LoadString(node_GetString(real_value2));
      //node_PrintTree(yeti_stream);
      node *yeti_block = node_GetItemByKey(yeti_stream,"yeti_block");
      node_RemoveItem(yeti_stream,yeti_block);
      node_FreeTree(yeti_stream);
      value = create_block_obj(base_class,yeti_block);
      node_SetParent(value,NULL);
      reset_obj_refcount(value);
      add_garbage(state,value);
      node_FreeTree(yeti_block);
    }
  }
  else if(!strcmp(node_GetKey(exe_obj),"yeti_object"))
  {
    //char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
    value = exe_obj;
    //add_garbage(state,value);
  }

  node *sub_exe_obj = node_GetItemByKey(execution_obj,"sub_exe_object");
  if(sub_exe_obj!=NULL)
  {
    //value = execute_obj(state,sub_exe_obj,block,execute_block);//,False);
    value = execute_obj(state,sub_exe_obj,block,False);
  }
  node_ClearItems(real_parameters);
  node_Free(real_parameters,False);
  if(value==NULL)
  {
    printf("exec return NULL\n");
    //int x = 1/0;
  }
  return(value);
}


node *search_block_path_for_member(node *block,char *key)
{
  node *found_obj=NULL;
  if(node_GetItemByKey(block,"anonymous_block_parent")!=NULL)
  {
    //printf("have to check anonymous block parent too:%x\n",block);
    node *abp=node_GetItemByKey(block,"anonymous_block_parent");
    found_obj = get_member(node_GetValue(abp),key);
    if(found_obj==NULL)
    {
      found_obj = search_block_path_for_member(node_GetValue(abp),key);
    }
  }
  return(found_obj);
}

node *evaluate_statement(node *state,node *statement,node *block,long iteration_start_index)
{
  //long old_iteration_index = node_GetItemIterationIndex(statement);
  node_SetItemIterationIndex(statement,iteration_start_index);
  long index = iteration_start_index;
  node *parameters = create_obj("parameters");
  node *actual_obj = block;
  node *sub_exe_obj = NULL;
  node *prev_token = NULL;
  node *base_class = node_GetItemByKey(state,"yeti_object");

  while(node_ItemIterationUnfinished(statement))
  {
    node *token = node_ItemIterate(statement);
    //printf("op: %s\n",node_GetString(token));
    //node_Print(token,False,False);
    //printf("\n");
    if(!strcmp(node_GetKey(token),"yeti_parameters"))
    {
      node_ItemIterationReset(token);
      while(node_ItemIterationUnfinished(token))
      {
        node *parameter_token = node_ItemIterate(token);
        node *sub_obj = evaluate_statement(state,parameter_token,block,0);
        node_AddItem(parameters,sub_obj);
      }
      if(index+1<node_GetItemsNum(statement))
      {
        node *sub_obj = evaluate_statement(state,statement,block,index+1);
        sub_exe_obj = sub_obj;
      }
    }
    else if(prev_token!=NULL && !strcmp(node_GetKey(prev_token),"val") && !strcmp(node_GetKey(token),"yeti_array"))
    {
      node *key_exe_obj = evaluate_statement(state,node_GetItem(token,0),block,0);
      node *key_obj = execute_obj(state,key_exe_obj,block,True);//,False);/*TODO move to execute somehow*/
      node *found_obj = get_item(actual_obj,key_obj);
      if(found_obj==NULL)
      {
        printf("array entry not found\n");
      }
      else
      {
        actual_obj = found_obj;
      }
      free_execution_obj(key_exe_obj);
    }
    else if(!strcmp(node_GetKey(token),"yeti_array"))
    {
      node *array = create_obj("array");
      node_ItemIterationReset(token);
      while(node_ItemIterationUnfinished(token))
      {
        node *parameter_token = node_ItemIterate(token);
        node *sub_obj = evaluate_statement(state,parameter_token,block,0);
        node_AddItem(array,sub_obj);
      }
      actual_obj=array;
      if(index+1<node_GetItemsNum(statement))
      {
        node *sub_obj = evaluate_statement(state,statement,block,index+1);
        sub_exe_obj = sub_obj;
      }
      //add_garbage(state,array);
    }
    else if(!strcmp(node_GetKey(token),"yeti_block"))
    {
      node *block_class_instance = create_block_obj(base_class,token);
      //add_obj_string(block_class_instance,"execute_block","True");
      //node_SetParent(block_class_instance,block);
      node *bmembers = node_GetItemByKey(block,"members");
      //node *bmembers = node_GetItemByKey(actual_obj,"members");
      //node_SetParent(block_class_instance,bmembers);

      node *exe_parameters = create_obj("parameters");
      node *peek = node_ItemPeek(statement);
      if(peek!=NULL && !strcmp(node_GetKey(peek),"yeti_parameters"))
      {
        //printf("found yeti block to be executed\n");
        node *sub_parameters = node_ItemIterate(statement);
        node_ItemIterationReset(sub_parameters);
        while(node_ItemIterationUnfinished(sub_parameters))
        {
          node *parameter_token = node_ItemIterate(sub_parameters);
          node *sub_obj = evaluate_statement(state,parameter_token,block,0);
          node_AddItem(exe_parameters,sub_obj);
        }
        index++;
        node *func_exe_obj = create_execution_obj(block_class_instance,exe_parameters,NULL);
        actual_obj = execute_obj(state,func_exe_obj,block,True);//,False);/*TODO move to execute somehow*/
        //actual_obj = execute_obj(state,func_exe_obj,block,False);/*TODO move to execute somehow*/
        free_execution_obj(func_exe_obj);
        //printf("adding blkinst:%x\n",block_class_instance);
        //node_SetParent(block_class_instance,NULL);
        add_garbage(state,block_class_instance);
        //node_Free(exe_parameters,True);
      }
      else
      {
        set_obj_node(block_class_instance,"anonymous_block_parent",block);
        node_Free(exe_parameters,True);
        //printf("not exec add:%x\n",block_class_instance);
        //node_SetParent(block_class_instance,NULL);
        add_garbage(state,block_class_instance);
        actual_obj = block_class_instance;
      }
    }
    else if(!strcmp(node_GetKey(token),"str"))
    {
      node *child = create_class_instance(base_class);
      set_obj_string(child,"value",node_GetValue(token));
      add_garbage(state,child);
      //node_SetParent(child,NULL);
      //printf("created string:%x : parent:%x\n",child,node_GetParent(child));
      actual_obj = child;
    }
    else if(!strcmp(node_GetKey(token),"val"))
    {
      if(node_GetType(token) == NODE_TYPE_STRING)
      {
        if(!strcmp(node_GetValue(token),"value"))
        {
          printf("setting actual obj to:\n");
          node *old_actual_obj = actual_obj;
          actual_obj = node_GetItemByKey(actual_obj,"value");
          if(node_GetValue(actual_obj)==NULL)
          {
            printf("value undefined\n");
            node *child = create_class_instance(base_class);
            set_obj_node(old_actual_obj,"value",child);
            node_SetParent(child,old_actual_obj);
            actual_obj = child;
          }
        }
        else if(!strcmp(node_GetValue(token),"@"))
        { }
        else
        {
          node *found_obj = get_member(actual_obj,node_GetValue(token));
          if(found_obj==NULL)
            found_obj = search_block_path_for_member(actual_obj,node_GetValue(token));
          if(found_obj==NULL)
          {
            node *child = create_class_instance(base_class);
            set_obj_string(child,"name",node_GetValue(token));
            //printf("creating:%s (%x) in %x\n",node_GetString(token),child,actual_obj);
            //if(actual_obj != NULL)
            add_member(actual_obj,child);
            inc_obj_refcount(child);
            actual_obj = child;
            node *peek = node_ItemPeek(statement);
            if(peek!=NULL && !strcmp(node_GetKey(peek),"yeti_parameters"))
            {
              //printf("found function def parameters\n");
              node *parameters_definition = node_ItemIterate(statement);
              node *pars = node_CopyTree(parameters_definition,True,True);
              //node_AddItem(child,parameters_definition);
              node_AddItem(child,pars);
              //add_obj_string(child,"empty_function_shell","True");
              //set_obj_string(child,"execute_block","False");
              //node_RemoveItem(statement,parameters_definition);
              index++;
            }
          }
          else
          {
            node *print_node = node_GetParent(found_obj);
            node *ppp=NULL;
            if(print_node!=NULL)
              ppp=node_GetParent(print_node);
            //printf("found:%s,%x in %x\n",node_GetValue(token),found_obj,ppp);
            //node_PrintTree(get_value(found_obj));
            //actual_obj = found_obj;
            if(!strcmp(get_obj_type(found_obj),"function"))
            {
              //printf("found a function during evaluation\n");
              //node_PrintTree(actual_obj);
              node *exe_parameters = create_obj("parameters");
              node *peek = node_ItemPeek(statement);
              if(peek!=NULL && !strcmp(node_GetKey(peek),"yeti_parameters"))
              {
                node *sub_parameters = node_ItemIterate(statement);
                node_ItemIterationReset(sub_parameters);
                while(node_ItemIterationUnfinished(sub_parameters))
                {
                  node *parameter_token = node_ItemIterate(sub_parameters);
                  //node *sub_obj = evaluate_statement(state,parameter_token,block,0);
                  //node *sub_obj = evaluate_statement(state,parameter_token,found_obj,0);
                  node *sub_obj = evaluate_statement(state,parameter_token,block,0);
                  //node *sub_obj = evaluate_statement(state,parameter_token,actual_obj,0);
                  node_AddItem(exe_parameters,sub_obj);
                }
                /*
                if(index+1<node_GetItemsNum(statement))
                {
                  node *sub_obj = evaluate_statement(state,statement,block,index+1);
                  sub_exe_obj = sub_obj;
                }*/
                index++;
              }
              /*node *func_exe_obj = evaluate_statement(state,actual_obj,block,0);*/
              //node *func_exe_obj = create_execution_obj(actual_obj,exe_parameters,sub_exe_obj);
              //actual_obj = found_obj;


              //node *bmembers = node_GetItemByKey(block,"members");
              //node *old_func_parent = node_GetParent(found_obj);
              //node_SetParent(found_obj,bmembers);
              node *func_exe_obj = create_execution_obj(found_obj,exe_parameters,NULL);
              actual_obj = execute_obj(state,func_exe_obj,block,False);//,False);/*TODO move to execute somehow*/
              //node_SetParent(found_obj,old_func_parent);

              //actual_obj = execute_obj(state,actual_obj,block,False);
              free_execution_obj(func_exe_obj);
            }
            else// if(!strcmp(get_obj_type(actual_obj),"yeti_il_block"))
            {
              node *exe_parameters = create_obj("parameters");
              node *peek = node_ItemPeek(statement);
              if(peek!=NULL && !strcmp(node_GetKey(peek),"yeti_parameters"))
              {
                //printf("found parameters after id\n");
                node *sub_parameters = node_ItemIterate(statement);
                node *peek = node_ItemPeek(statement);
                if(peek!=NULL && !strcmp(node_GetKey(peek),"ops") && !strcmp(node_GetValue(peek),"="))
                {
                  //printf("redefinition of function\n");
                  //node *parameters_definition = node_ItemIterate(statement);
                  node *pars = node_CopyTree(sub_parameters,True,True);
                  node *old_pars = node_GetItemByKey(found_obj,"yeti_parameters");
                  if(old_pars!=NULL)
                  {
                    node_RemoveItem(found_obj,old_pars);
                    node_FreeTree(old_pars);
                  }
                  node_AddItem(found_obj,pars);
                  actual_obj = found_obj;
                  node_Free(exe_parameters,True);
                }
                else
                {
                  node_ItemIterationReset(sub_parameters);
                  while(node_ItemIterationUnfinished(sub_parameters))
                  {
                    node *parameter_token = node_ItemIterate(sub_parameters);
                    node *sub_obj = evaluate_statement(state,parameter_token,block,0);
                    //node *sub_obj = evaluate_statement(state,parameter_token,actual_obj,0);
                    node_AddItem(exe_parameters,sub_obj);
                  }
                  node *func_exe_obj = create_execution_obj(found_obj,exe_parameters,NULL);
                  //node *bmembers = node_GetItemByKey(block,"members");
                  //node *old_func_parent = node_GetParent(found_obj);
                  //node_SetParent(found_obj,bmembers);
                  actual_obj = execute_obj(state,func_exe_obj,block,True);//,False);/*TODO move to execute somehow*/
                  //actual_obj = execute_obj(state,func_exe_obj,actual_obj,True);//,False);/*TODO move to execute somehow*/
                  //node_SetParent(found_obj,old_func_parent);
                  free_execution_obj(func_exe_obj);
                }
                index++;
              }
              else
              {
                actual_obj = found_obj;
                node_Free(exe_parameters,True);
              }
            }
            /*else
            {

            }*/
          }
        }
      }
      else if(node_GetType(token) == NODE_TYPE_SINT32)
      {
        node *child = create_class_instance(base_class);
        //node_SetParent(child,NULL);
        //printf("created int:%x\n",child);
        set_obj_int(child,"value",node_GetSint32(token));
        actual_obj = child;//get_member(child,"get");
        add_garbage(state,child);
      }
    }
    else if(!strcmp(node_GetKey(token),"ops"))
    {
      /*if(!strcmp(node_GetString(token),"."))
      {
        //printf("sub id following\n");
        //
      }
      else*/
      //{
        node *found_obj = get_member(actual_obj,node_GetValue(token));
        if(found_obj!=NULL)
        {
          node *peek = node_ItemPeek(statement);
          if(peek!=NULL && !strcmp(node_GetKey(peek),"yeti_parameters"))
          {
            printf("found dupe brackets(%s)\n",node_GetString(token));
          }
          /*node *peek = node_ItemPeek(statement);
          if(peek!=NULL && !strcmp(node_GetKey(peek),"yeti_parameters"))
          //if(!strcmp(node_GetKey(token),"yeti_parameters"))
          {
            printf("found dupe brackets()\n");
            node *token = node_ItemIterate(statement);
            node_ItemIterationReset(token);
            while(node_ItemIterationUnfinished(token))
            {
              node *parameter_token = node_ItemIterate(token);
              node *sub_obj = evaluate_statement(state,parameter_token,block,0);
              node_AddItem(parameters,sub_obj);
            }
            node_PrintTree(parameters);
            if(index+1<node_GetItemsNum(statement))
            {
              node *sub_obj = evaluate_statement(state,statement,block,index+1);
              sub_exe_obj = sub_obj;
            }
            index++;
          }
          else*/ if(index+1<node_GetItemsNum(statement))
          {
            node *sub_obj = evaluate_statement(state,statement,block,index+1);
            node_AddItem(parameters,sub_obj);
          }
         
          actual_obj = found_obj;
        }
      //}
    }
    prev_token = token;
    index++;    
  }
  node *exe_obj = create_execution_obj(actual_obj,parameters,sub_exe_obj);
  return(exe_obj);
}

node *evaluate_block(node *state,node *block)
{
  node *ret = NULL;
  node *base_class = node_GetItemByKey(state,"yeti_object");
  node *block_class_instance = create_block_obj(base_class,block);
  node *il_block = node_GetItemByKey(block_class_instance,"yeti_block");
  if(!node_GetItemsNum(il_block))
  {
      node *base_class = node_GetItemByKey(state,"yeti_object");
      ret = create_class_instance(base_class);
      //node_SetParent(ret,NULL);
      reset_obj_refcount(ret);
      //printf("adding block instance empty ret:%x\n",ret);
      add_garbage(state,ret);
  }
  else
  {
    node_ItemIterationReset(il_block);
    while(node_ItemIterationUnfinished(il_block))
    {
      node *yeti_statement = node_ItemIterate(il_block);
      //printf("evaluating statement\n");
      node *obj = evaluate_statement(state,yeti_statement,block_class_instance,0);
      //printf("executing statement\n");
      //node_PrintTree(obj);
      ret=execute_obj(state,obj,block_class_instance,False);//,False);
      //ret=execute_obj(state,obj,block_class_instance,True);//,False);
      if(!node_ItemIterationUnfinished(il_block))
      {
        ret=node_CopyTree(ret,True,True);
        free_execution_obj(obj);
        free_garbage(state);
        //printf("block class instance:\n");
        //node_PrintTree(block_class_instance);
        //fflush(stdout);
        node_FreeTree(block_class_instance);
        return(ret);
      }
      //node_PrintTree(ret);
      //node_FreeTree(obj);
      //printf("finished execution object\n");
      //printf("freeing execution object\n");
      free_execution_obj(obj);
      node *block_flag=node_GetItemByKey(state,"block_flag");
      if(block_flag!=NULL)
      {
        char *bf = node_GetString(block_flag);
        if(!strcmp(bf,"restart"))
        { 
          node_ItemIterationReset(il_block);
          set_obj_string(state,"block_flag","");
        } 
        else if(!strcmp(bf,"continue"))
        {
          set_obj_string(state,"block_flag","");
          long itindex = node_GetItemIterationIndex(il_block);
          if(itindex+1<node_GetItemsNum(il_block))
            node_SetItemIterationIndex(il_block,itindex+1);
        } 
        else if(!strcmp(bf,"break"))
        {
          //printf("breaking block\n");
          node *block_break_count = node_GetItemByKey(state,"block_break_count");
          long count = node_GetSint32(block_break_count);
          //printf("break count:%d\n",count);
          if(!(count-1))
          {
            set_obj_int(state,"block_break_count",0);
            set_obj_string(state,"block_flag","");
            //printf("last break\n");
            node_FreeTree(block_class_instance);
            return;
          }
          else
          {
            set_obj_int(state,"block_break_count",count-1);
            //printf("next break\n");
            node_FreeTree(block_class_instance);
            return;
          }
          //set_obj_string(state,"block_flag","");
          //free_garbage(state);
        } 
      }
  //printf("block class instance:\n");
  //node_PrintTree(block_class_instance);


      free_garbage(state);
    }
  }
  printf("block class instance:\n");
  node_PrintTree(block_class_instance);
  //free_obj(state,block_class_instance);
  //printf("freeing block_class_instance\n");
  //free_garbage(state);
  node_FreeTree(block_class_instance);
  return(ret);
}

node *evaluate_block_instance(node *state,node *block_class_instance)
{
  node *ret = NULL;
  node *il_block = node_GetItemByKey(block_class_instance,"yeti_block");
  if(!node_GetItemsNum(il_block))
  {
      node *base_class = node_GetItemByKey(state,"yeti_object");
      ret = create_class_instance(base_class);
      //node_SetParent(ret,NULL);
      reset_obj_refcount(ret);
      //printf("adding block instance empty ret:%x\n",ret);
      add_garbage(state,ret);
  }
  else
  {
    node_ItemIterationReset(il_block);
    while(node_ItemIterationUnfinished(il_block))
    {
      node *yeti_statement = node_ItemIterate(il_block);
      node *obj = evaluate_statement(state,yeti_statement,block_class_instance,0);
      //node *exe_obj = execute_obj(state,obj,block_class_instance,False);
      ret = execute_obj(state,obj,block_class_instance,False);//,False);
      //ret = execute_obj(state,obj,block_class_instance,True);//,False);
      //free_garbage(state);//FAST GARBAGE GC
      //node_FreeTree(obj);
      free_execution_obj(obj);
      node *block_flag=node_GetItemByKey(state,"block_flag");
      if(block_flag!=NULL)
      {
        char *bf = node_GetString(block_flag);
        if(!strcmp(bf,"restart"))
        {
          node_ItemIterationReset(il_block);
          set_obj_string(state,"block_flag","");
        } 
        else if(!strcmp(bf,"continue"))
        {
          set_obj_string(state,"block_flag","");
          long itindex = node_GetItemIterationIndex(il_block);
          if(itindex+1<node_GetItemsNum(il_block))
            node_SetItemIterationIndex(il_block,itindex+1);
        } 
        else if(!strcmp(bf,"break"))
        {
          //printf("breaking block\n");
          node *block_break_count = node_GetItemByKey(state,"block_break_count");
          long count = node_GetSint32(block_break_count);
          //printf("break count:%d\n",count);
          if(!(count-1))
          {
            set_obj_int(state,"block_break_count",0);
            set_obj_string(state,"block_flag","");
            //printf("last break\n");
            return(ret);
          }
          else
          {
            set_obj_int(state,"block_break_count",count-1);
            //printf("next break\n");
            return(ret);
          }
          //set_obj_string(state,"block_flag","");
          //free_garbage(state);
        } 
      }
    }
  }
  if(ret==NULL)
  {
    printf("eval return NULL\n");
    //int x = 1/0;
  }
  //free_garbage(state);
  return(ret);
}

node *evaluate_block_instance_in(node *state,node *block_class_instance,node *block)
{
  node *ret = NULL;
  node *il_block = node_GetItemByKey(block_class_instance,"yeti_block");
  if(!node_GetItemsNum(il_block))
  {
      node *base_class = node_GetItemByKey(state,"yeti_object");
      ret = create_class_instance(base_class);
      //node_SetParent(ret,NULL);
      reset_obj_refcount(ret);
      //printf("adding block instance empty ret:%x\n",ret);
      add_garbage(state,ret);
  }
  else
  {
    node_ItemIterationReset(il_block);
    while(node_ItemIterationUnfinished(il_block))
    {
      node *yeti_statement = node_ItemIterate(il_block);
      node *obj = evaluate_statement(state,yeti_statement,block,0);
      //node *exe_obj = execute_obj(state,obj,block_class_instance,False);
      ret = execute_obj(state,obj,block,False);//,False);
      //free_garbage(state);
      //node_FreeTree(obj);
      free_execution_obj(obj);
      node *block_flag=node_GetItemByKey(state,"block_flag");
      if(block_flag!=NULL)
      {
        char *bf = node_GetString(block_flag);
        if(!strcmp(bf,"restart"))
        {
          node_ItemIterationReset(il_block);
          set_obj_string(state,"block_flag","");
        } 
        else if(!strcmp(bf,"continue"))
        {
          set_obj_string(state,"block_flag","");
          long itindex = node_GetItemIterationIndex(il_block);
          if(itindex+1<node_GetItemsNum(il_block))
            node_SetItemIterationIndex(il_block,itindex+1);
        } 
        else if(!strcmp(bf,"break"))
        {
          //printf("breaking block\n");
          node *block_break_count = node_GetItemByKey(state,"block_break_count");
          long count = node_GetSint32(block_break_count);
          //printf("break count:%d\n",count);
          if(!(count-1))
          {
            set_obj_int(state,"block_break_count",0);
            set_obj_string(state,"block_flag","");
            //printf("last break\n");
            return(ret);
          }
          else
          {
            set_obj_int(state,"block_break_count",count-1);
            //printf("next break\n");
            return(ret);
          }
          //set_obj_string(state,"block_flag","");
          //free_garbage(state);
        } 
      }
    }
  }
  if(ret==NULL)
  {
    printf("eval return NULL\n");
    //int x = 1/0;
  }
  //free_garbage(state);
  return(ret);
}

int main(int argc, char** argv)
{
  #ifdef USE_MEMORY_DEBUGGING
  mem_Init();
  #endif

  int ret = 0;
  node *base_class = create_class_object();
  node *yeti_stream = NULL;
  if(argc<2)
  {
    char *line = CreateEmptyString();
    char c=EOF;
    while((c=getc(stdin))!=EOF)
    {
      line = AddCharToString(line,c);
    }
    yeti_stream = yeti_LoadString(line);
    free(line);
  }
  else
  {
    if(!strcmp(argv[1],"-v"))
    {
      printf("yeti interpreter %d.%d (build %d)\n",MAJOR_VERSION,MINOR_VERSION,BUILD+1);
      node_FreeTree(base_class);
      return(0);
    }

    if(!strcmp(argv[1],"-ast"))
    {
      yeti_stream = yeti_LoadFile(argv[2]);
      node_PrintTree(yeti_stream);
      node_FreeTree(yeti_stream);
      node_FreeTree(base_class);
      return(0);
    }
    yeti_stream = yeti_LoadFile(argv[1]);
  }
    
  
  node *yeti_block = node_GetItemByKey(yeti_stream,"yeti_block");
  node_RemoveItem(yeti_stream,yeti_block);
  node_FreeTree(yeti_stream);
  node *yeti_state = create_yeti_state(yeti_block,base_class);
  if(yeti_stream!=NULL)
  {
    node *ret_obj=evaluate_block(yeti_state,yeti_block);
    node *real_value = node_GetItemByKey(ret_obj,"value");
    if(node_GetType(real_value)==NODE_TYPE_SINT32)
      ret = node_GetSint32(real_value);
    else if(node_GetType(real_value)==NODE_TYPE_STRING)
      ret = atoi(node_GetString(real_value));
    node_FreeTree(ret_obj);
  }
  node *garbage = node_GetItemByKey(yeti_state,"garbage");
  node_ClearItems(garbage);
  node_FreeTree(yeti_state);
  #ifdef USE_MEMORY_DEBUGGING
  mem_Close();
  #endif
  return(ret);
}