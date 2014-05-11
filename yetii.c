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
  printf("created obj :%s @:%x\n",name,obj);
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
  node_SetSint32(kv,i);
}

void set_obj_node(node *obj,char *key,node *n)
{
  node *kv = node_GetItemByKey(obj,key);
  node_SetType(kv,NODE_TYPE_NODE);
  //printf("setting node value:%x\n",n);
  kv->value = n;
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
  if((node_GetSint32(refcount)-1)<0)
  {
    printf("decrementing beneath 0 refs!\n");
    //int i=1/0;
  }
  node_SetSint32(refcount,node_GetSint32(refcount)-1);

}

long get_obj_refcount(node *obj)
{
  node *refcount = node_GetItemByKey(obj,"refcount");
  return(node_GetSint32(refcount));
}

void add_obj_int(node *obj,char *key,long i)
{
  node *kv = node_Create();
  node_SetKey(kv,key);
  node_SetSint32(kv,i);
  add_obj_kv(obj,kv); 
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

void add_member(node *obj,node *member)
{
  node *members = node_GetItemByKey(obj,"members");
  add_obj_kv(members,member);
}

node *get_member(node *obj,char *key)
{
  node *members = node_GetItemByKey(obj,"members");
  if(members==NULL)
    return(NULL);
  node_ItemIterationReset(members);
  while(node_ItemIterationUnfinished(members))
  {
    node *member = node_ItemIterate(members);
    node *member_name = node_GetItemByKey(member,"name");
    if(!strcmp(node_GetString(member_name),key))
      return(member);
  }
  //node *instance = (node*)node_GetItemByKey(obj,"base_class_instance");
  node *base_class_instance = (node*)node_GetValue(node_GetItemByKey(obj,"base_class_instance"));
  if(base_class_instance!=NULL)
    return(get_member(base_class_instance,key));
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
  node *base_class_type = (node*)node_GetValue(node_GetItemByKey(class_obj,"base_class_type"));
  if(base_class_type!=NULL)
  {
    node *base_class_instance = create_class_instance(base_class_type);
    set_obj_node(child,"base_class_instance",base_class_instance);
  }
  printf("created class instance:%x :%s\n",child,get_obj_name(child));
  return(child);
}

void free_obj(yeti_state *state,node *obj)
{
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
  node_FreeTree(obj);
}

node *create_base_obj_layout(char *obj_name)
{
  node *base = create_obj("yeti_object");
  add_obj_string(base,"type","class");
  add_obj_string(base,"name",obj_name);
  add_obj_kv(base,create_obj("base_class_instance"));
  add_obj_kv(base,create_obj("base_class_type"));
  add_obj_kv(base,create_obj("members"));
  add_obj_kv(base,create_obj("value"));
  add_obj_int(base,"refcount",0);
  return(base);
}


void add_class_object_internal_function(node *class,char *method_name)//void *addr
{
  node *base = create_base_obj_layout(method_name);
  set_obj_string(base,"type","function");
  set_obj_node(base,"base_class_instance",class);
  set_obj_node(base,"base_class_type",class);
  inc_obj_refcount(base);
  add_member(class,base);
}

node *create_class_object(void)
{
  node *base = create_base_obj_layout("object");
  add_class_object_internal_function(base,"=");
  add_class_object_internal_function(base,":");
  add_class_object_internal_function(base,"+");
  add_class_object_internal_function(base,"print");

  //add_internal_function(base,"-");
  //add_internal_function(base,"/");
  //add_internal_function(base,"*");
  //add_class_object_internal_function(base,"get");
  //add_class_object_internal_function(base,"test");
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

void free_execution_obj(yeti_state * state,node *exe_obj)
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
        printf("freeing exe_object in parameters\n");
        free_execution_obj(state,parameter);
      }
    }
  }
  node_ClearItems(parameters);
  node *sub_exe_obj = node_GetItemByKey(exe_obj,"sub_execution_obj");
  if(sub_exe_obj!=NULL)
    free_execution_obj(state,sub_exe_obj);
  node *obj = node_GetItem(exe_obj,0);
  //free_obj(state,obj);
  node_Free(parameters,False);
  node_ClearItems(exe_obj);
  node_Free(exe_obj,False);
  //node_Free(parameters);
}

void add_garbage(yeti_state *state,node *obj)
{
  if(node_HasItem(state->garbage,obj))
  {
    printf("dupe in garbage\n");
    return;
  }  
  node_AddItem(state->garbage,obj);
}

void free_garbage(yeti_state * state)
{
  printf("freeing garbage:%d\n",node_GetItemsNum(state->garbage));
  //node_PrintTree(state->garbage);
  node_ItemIterationReset(state->garbage);
  while(node_ItemIterationUnfinished(state->garbage))
  {
    node *gc = node_ItemIterate(state->garbage);
    //node_PrintTree(gc);
    if(get_obj_refcount(gc)<=0)
      node_FreeTree(gc);
    else
    {
      printf("object not collected ->still has refs:%d\n",get_obj_refcount(gc));

    }
  }
  node_ClearItems(state->garbage);
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

yeti_state *create_yeti_state(node *base_class)
{
  yeti_state *state = (yeti_state*)malloc(sizeof(yeti_state));
  state->base_class = base_class;
  state->garbage = create_obj("garbage");
  return(state);
}


node *execute_obj(yeti_state *state,node *execution_obj,node *block,BOOL dont_execute_block)
{
  //deep first recursive
  BOOL tmp_dont_execute_block = False;
  node *value = NULL;
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  //node_PrintTree(execution_obj);

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

  node_ItemIterationReset(parameters);
  while(node_ItemIterationUnfinished(parameters))
  {
    node *token = node_ItemIterate(parameters);
    if(!strcmp(node_GetKey(token),"exe_object"))
    {
      //printf("getting par:\n");
      node *sub = execute_obj(state,token,block,tmp_dont_execute_block);
      //node_PrintTree(sub);
      node_AddItem(real_parameters,sub);
      //add_garbage(state,sub);
    }
  }
  tmp_dont_execute_block = False;

  if(!strcmp(node_GetKey(exe_obj),"array"))
  {
    value = create_class_instance(state->base_class);
    node *items = create_obj("items");
    add_obj_kv(value,items);
    add_garbage(state,value);
    node_ItemIterationReset(exe_obj);
    while(node_ItemIterationUnfinished(exe_obj))
    {
      node *token = node_ItemIterate(exe_obj);
      if(!strcmp(node_GetKey(token),"exe_object"))
      {
        node *sub = execute_obj(state,token,block,False);
        add_garbage(state,sub);
        node *sub_c = node_CopyTree(sub,True,True);
        node_AddItem(items,sub_c);
        inc_obj_refcount(sub_c);
        free_execution_obj(state,token);
      }
    }
    node_ClearItems(exe_obj);
    node_Free(exe_obj,True);
  }
  else if(!strcmp(get_obj_type(exe_obj),"yeti_il_block"))
  {
    node *exe_block = node_GetItemByKey(exe_obj,"execute_block");
    //printf("yeti_block\n");
    //node_PrintTree(exe_obj);
    if( exe_block!= NULL && !strcmp(node_GetString(exe_block),"True") && dont_execute_block==False )
    {
      //node *il_block = node_GetItemByKey(exe_obj,"yeti_block");
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
            node *obj = evaluate_statement(state,token,exe_obj,0);
            node *sub = execute_obj(state,obj,exe_obj,False);
            //free_garbage(state);


            node *obj_name = node_GetItemByKey(sub,"name");
            node *real_parent = node_GetParent(sub);
            node_RemoveItem(real_parent,sub);
            dec_obj_refcount(sub);
            add_garbage(state,sub);
            value = node_CopyTree(node_GetItem(real_parameters,p_index),True,True);
            set_obj_string(value,"name",node_GetString(obj_name));
            node_AddItem(real_parent,value);
            node_SetParent(value,real_parent);
            inc_obj_refcount(value);
            node *obj_parameters = node_GetItemByKey(sub,"yeti_parameters");
            node_RemoveItem(sub,obj_parameters);
            if(obj_parameters!=NULL)
            {
              add_obj_kv(value,node_CopyTree(obj_parameters,True,True));
            }
            free_execution_obj(state,obj);
          }
          p_index++;
        }
      }
      else
      {
        //printf("block has no parameters\n");
      }
      set_obj_string(exe_obj,"execute_block","False");
      evaluate_block_instance(state,exe_obj);
      value = exe_obj;
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
      value = node_CopyTree(parent,True,True);
      reset_obj_refcount(value);
      //inc_obj_refcount(value);
      add_garbage(state,parent);
      add_garbage(state,value);
      node *value2 = node_GetItem(real_parameters,0);
      node *real_value = node_GetItemByKey(value,"value");
      node *real_value2 = node_GetItemByKey(value2,"value");
      node_SetSint32(real_value,node_GetSint32(real_value)+node_GetSint32(real_value2));
    }
    else if(!strcmp(name,":"))
    {
      value = node_GetItemByKey(parent,"value");
      node *value2_parent = node_GetItem(real_parameters,0);
      node *value2 = node_GetItemByKey(value2_parent,"value");
      node_SetType(value,node_GetType(value2));
      node_SetValue(value,node_GetValue(value2),True,True);
      value = parent;
    }
    else if(!strcmp(name,"="))
    {
      node *obj_name = node_GetItemByKey(parent,"name");
      node *real_parent = node_GetParent(parent);
      //printf("removing :(%x) %s from %x\n",parent,node_GetString(obj_name),real_parent);
      //node_Print(real_parent,True,True);
      //node_PrintTree(real_parent);
      int r = node_RemoveItem(real_parent,parent);
      //node_Print(parent,True,True);
      if(r==-1)
      {
        printf("error item not removed\n");
        int x=1/0;
      }
      dec_obj_refcount(parent);
      add_garbage(state,parent);
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
      set_obj_string(value,"execute_block","False");
    }
    else if(!strcmp(name,"print"))
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
          node_Print(node_GetItemByKey(token,"value"),False,False);
      }      
      printf("\n");
    }
    else if(!strcmp(name,"test"))
    {
      value = node_GetItemByKey(parent,"value");
      node *value_a = node_GetItem(real_parameters,0);
      node_SetSint32(value,20+node_GetSint32(value_a));
      value = parent;
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
    value = execute_obj(state,sub_exe_obj,block,False);
  }
  node_ClearItems(real_parameters);
  node_Free(real_parameters,False);
  return(value);
}

node *evaluate_statement(yeti_state *state,node *statement,node *block,long iteration_start_index)
{
  //long old_iteration_index = node_GetItemIterationIndex(statement);
  node_SetItemIterationIndex(statement,iteration_start_index);
  long index = iteration_start_index;
  node *parameters = create_obj("parameters");
  node *actual_obj = block;
  node *sub_exe_obj = NULL;
  node *prev_token = NULL;
  while(node_ItemIterationUnfinished(statement))
  {
    node *token = node_ItemIterate(statement);
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
      node *key_obj = execute_obj(state,key_exe_obj,block,False);/*TODO move to execute somehow*/
      node *found_obj = get_item(actual_obj,key_obj);
      if(found_obj==NULL)
      {
        printf("array entry not found\n");
      }
      else
      {
        actual_obj = found_obj;
      }
      free_execution_obj(state,key_obj);
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
      node *block_class_instance = create_block_obj(state->base_class,token);
      add_obj_string(block_class_instance,"execute_block","True");
      add_garbage(state,block_class_instance);
      actual_obj = block_class_instance;
    }
    else if(!strcmp(node_GetKey(token),"str"))
    {
      node *child = create_class_instance(state->base_class);
      set_obj_string(child,"value",node_GetValue(token));
      add_garbage(state,child);
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
            node *child = create_class_instance(state->base_class);
            set_obj_node(old_actual_obj,"value",child);
            node_SetParent(child,old_actual_obj);
            actual_obj = child;
          }
        }
        else if(!strcmp(node_GetValue(token),"@"))
        {
        }
        else
        {
          node *found_obj = get_member(actual_obj,node_GetValue(token));
          if(found_obj==NULL)
          {
            node *child = create_class_instance(state->base_class);
            set_obj_string(child,"name",node_GetValue(token));
            add_member(actual_obj,child);
            actual_obj = child;
            node *peek = node_ItemPeek(statement);
            if(peek!=NULL && !strcmp(node_GetKey(peek),"yeti_parameters"))
            {
              //printf("found function def parameters\n");
              node *parameters_definition = node_ItemIterate(statement);
              node *pars = node_CopyTree(parameters_definition,True,True);
              //node_AddItem(child,parameters_definition);
              node_AddItem(child,pars);
              add_obj_string(child,"empty_function_shell","True");
              set_obj_string(child,"execute_block","False");
              //node_RemoveItem(statement,parameters_definition);
              index++;
            }
          }
          else
          {
            actual_obj = found_obj;
            if(!strcmp(get_obj_type(actual_obj),"yeti_il_block"))
            {
              node *peek = node_ItemPeek(statement);
              if(peek!=NULL && !strcmp(node_GetKey(peek),"yeti_parameters"))
              {
                set_obj_string(actual_obj,"execute_block","True");
              }
            }
          }
        }
      }
      else if(node_GetType(token) == NODE_TYPE_SINT32)
      {
        node *child = create_class_instance(state->base_class);
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
          if(index+1<node_GetItemsNum(statement))
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

void evaluate_block(yeti_state *state,node *block)
{
    node *block_class_instance = create_block_obj(state->base_class,block);
    node *il_block = node_GetItemByKey(block_class_instance,"yeti_block");
    node_ItemIterationReset(il_block);
    while(node_ItemIterationUnfinished(il_block))
    {
        node *yeti_statement = node_ItemIterate(il_block);
        printf("evaluating statement\n");
        node *obj = evaluate_statement(state,yeti_statement,block_class_instance,0);
        printf("executing statement\n");
        execute_obj(state,obj,block_class_instance,False);
        //node_PrintTree(obj);
        //node_FreeTree(obj);
        printf("finished execution object\n");

        free_garbage(state);
        printf("freeing execution object\n");
        free_execution_obj(state,obj);
    }
    //node_PrintTree(block_class_instance);
    //free_obj(state,block_class_instance);
    printf("freeing block_class_instance\n");
    node_FreeTree(block_class_instance);
}

void evaluate_block_instance(yeti_state *state,node *block_class_instance)
{
    node *il_block = node_GetItemByKey(block_class_instance,"yeti_block");
    node_ItemIterationReset(il_block);
    while(node_ItemIterationUnfinished(il_block))
    {
        node *yeti_statement = node_ItemIterate(il_block);
        node *obj = evaluate_statement(state,yeti_statement,block_class_instance,0);
        node *exe_obj = execute_obj(state,obj,block_class_instance,False);
        //node_FreeTree(obj);
        free_execution_obj(state,obj);
    }
}


int main(int argc, char** argv)
{
  #ifdef USE_MEMORY_DEBUGGING
  mem_Init();
  #endif

  node *base_class = create_class_object();
  yeti_state *state = create_yeti_state(base_class);

  printf("yeti interpreter 0.1\n");
  if(argc<2)
  {
    printf("error: no input file given as parameter\n");
    return(1);
  }
  node *yeti_stream = yeti_LoadFile(argv[1]);
  //node_PrintTree(yeti_stream);
  
  state->top_scope = node_GetItemByKey(yeti_stream,"yeti_block");
  if(yeti_stream!=NULL)
  {
    evaluate_block(state,state->top_scope);
  }
  //node *c = create_class_instance(base_class);
  //node_FreeTree(state->top_scope);
  //printf("bc:%x,c:%x\n",base_class,c);
  //node_FreeTree(c);
  node_FreeTree(base_class);
  
  node_FreeTree(yeti_stream);
  node_ClearItems(state->garbage);
  node_Free(state->garbage,False);

  free(state);
  #ifdef USE_MEMORY_DEBUGGING
  mem_Close();
  #endif

}