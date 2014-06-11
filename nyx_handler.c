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

#include "nyx_handler.h"

void *nyxh_handler_test(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(real_parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  printf("handler test called:%d\n",node_GetSint32(real_value2));
  node_SetSint32(real_value,100+node_GetSint32(real_value2));
  return(value);
}

void *nyxh_add(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = node_CopyTree(parent,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
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
  return(value);
}

void *nyxh_sub(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = node_CopyTree(parent,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(real_parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetSint32(real_value,node_GetSint32(real_value)-node_GetSint32(real_value2));
  return(value);
}

void *nyxh_div(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = node_CopyTree(parent,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(real_parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetSint32(real_value,node_GetSint32(real_value)/node_GetSint32(real_value2));
  return(value);
}

void *nyxh_mul(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = node_CopyTree(parent,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
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
      dst = StringMult(node_GetString(real_value),mult);
      node_SetString(real_value,dst);
      free(dst);
    }
  }
  return(value);
}

void *nyxh_lt(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = node_CopyTree(parent,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(real_parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetSint32(real_value,node_GetSint32(real_value)<node_GetSint32(real_value2));
  return(value);
}

void *nyxh_gt(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = node_CopyTree(parent,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(real_parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetSint32(real_value,node_GetSint32(real_value)>node_GetSint32(real_value2));
  return(value);
}

void *nyxh_eq(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = node_CopyTree(parent,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(real_parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetSint32(real_value,node_GetSint32(real_value)==node_GetSint32(real_value2));
  return(value);
}

void *nyxh_lt_eq(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = node_CopyTree(parent,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(real_parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetSint32(real_value,node_GetSint32(real_value)<=node_GetSint32(real_value2));
  return(value);
}

void *nyxh_gt_eq(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = node_CopyTree(parent,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(real_parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetSint32(real_value,node_GetSint32(real_value)>=node_GetSint32(real_value2));
  return(value);
}

void *nyxh_neq(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = node_CopyTree(parent,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(real_parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetSint32(real_value,node_GetSint32(real_value)!=node_GetSint32(real_value2));
  return(value);
}

// : handler
void *nyxh_set_value_only(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = node_GetItemByKey(parent,"value");
  node *value2_parent = node_GetItem(real_parameters,0);
  node *value2 = node_GetItemByKey(value2_parent,"value");
  node_SetType(value,node_GetType(value2));
  node_SetValue(value,node_GetValue(value2),True,True);
  value = parent;
  return(value);
}

// = handler
void *nyxh_assign(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *obj_name = node_GetItemByKey(parent,"name");
  node *real_parent = node_GetParent(parent);
  int r = node_RemoveItem(real_parent,parent);
  node_SetParent(parent,NULL);
  if(r==-1)
  {
    printf("error item not removed\n");
  }
  dec_obj_refcount(parent);
  add_garbage(state,parent);
  node *value = node_CopyTree(node_GetItem(real_parameters,0),True,True);
  reset_obj_refcount(value);
  set_obj_string(value,"name",node_GetString(obj_name));
  node_AddItem(real_parent,value);
  inc_obj_refcount(value);
  node_SetParent(value,real_parent);
  node *obj_parameters = node_GetItemByKey(parent,"nyx_parameters");
  if(obj_parameters!=NULL)
  {
    node *pars = node_CopyTree(obj_parameters,True,True);
    add_obj_kv(value,pars);
  }
  return(value);
}

//println,print handler
void *nyxh_print(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

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
        node_Print(node_GetItemByKey(token,"value"),False,False);
      }
    }      
  }
  else
  {
    if(parent!=NULL && parent != block)
      node_Print(node_GetItemByKey(parent,"value"),False,False);
  }
  if(!strcmp(name,"println"))
    printf("\n");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

//else handler
void *nyxh_else(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);

  long parent_value = 0;
  if(parent!=NULL)
    parent_value = node_GetSint32(node_GetItemByKey(parent,"value"));

  node *exe_block = node_GetItem(parameters,0);
  //node *exe_block_obj = node_GetItem(exe_block,0);
  //node *bmembers = node_GetItemByKey(block,"members");
  if(!parent_value)
  {
    node *blk_val=execute_obj(state,exe_block,block,True);
  }
  set_obj_int(value,"value",parent_value);
  return(value);
}

//? handler
void *nyxh_cmp(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");
  //?(expression,true_func,false_func,loop_while_true); 
  //execute based on expression value (0,1,..) ,loops if needed
  //returns(expression)

  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);

  node *expression_block = node_GetItem(parameters,0);
  node *true_block = node_GetItem(parameters,1);
  node *false_block = node_GetItem(parameters,2);
  node *loop_exe = node_GetItem(parameters,3);
  node *loop = execute_obj(state,loop_exe,block,False);//,tmp_dont_execute_block);
  node *loop_value = node_GetItemByKey(loop,"value");
  long lv = node_GetSint32(loop_value);

  node *expression_block_obj = node_GetItem(expression_block,0);
  node *true_block_obj = node_GetItem(true_block,0);
  node *false_block_obj = node_GetItem(false_block,0);
  node *bmembers = node_GetItemByKey(block,"members");
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
    set_obj_int(value,"value",node_GetSint32(node_GetItemByKey(exp_val,"value")));
    if(node_GetSint32(node_GetItemByKey(exp_val,"value")))
    {
      blk_val=execute_obj(state,true_block,block,True);
    }
    else
      blk_val=execute_obj(state,false_block,block,True);
  }
  return(value);
}

// ?? handler
void *nyxh_init_cmp(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

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
  node *init_eval = execute_obj(state,init_block,block,True);
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
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

void *nyxh_break(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  set_obj_string(state,"block_flag","break");
  if(node_GetItemsNum(real_parameters)>0)
  {
    node *count = node_GetItem(real_parameters,0);
    node *real_value = node_GetItemByKey(count,"value");
    set_obj_int(state,"block_break_count",node_GetSint32(real_value));
  }
  else
  {
    set_obj_int(state,"block_break_count",1);
  }
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

void *nyxh_restart(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  set_obj_string(state,"block_flag","restart");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

void *nyxh_continue(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  set_obj_string(state,"block_flag","continue");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

void *nyxh_len(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = NULL;
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
  return(value);
}

void *nyxh_input(node *state,node *execution_obj,node *block)
{
  // a.input()
  // a=input()
  // input(a)
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = NULL;
  char *line = CreateEmptyString();
  char c='\n';
  while((c=fgetc(stdin))!='\n' && c!=EOF && c!=0)
  {
    line = AddCharToString(line,c);
  }
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
  node_SetString(real_value,line);
  free(line);
  return(value);
}

void *nyxh_http_query(node *state,node *execution_obj,node *block)
{
  //returns http query vars as array
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");
  
  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = NULL;
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
  return(value);
}

void *nyxh_int(node *state,node *execution_obj,node *block)
{
  //returns integer of string input
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
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
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    node_SetSint32(real_value,node_GetSint32(real_value2));
  else if(node_GetType(real_value2)==NODE_TYPE_STRING)
    node_SetSint32(real_value,atoi(node_GetString(real_value2)));
  return(value);
}

void *nyxh_str(node *state,node *execution_obj,node *block)
{
  //returns string of integer input
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
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
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    node_SetString(real_value,convert_to_string(node_GetSint32(real_value2)));
  else if(node_GetType(real_value2)==NODE_TYPE_STRING)
    node_SetString(real_value,node_GetString(real_value2));
  return(value);
}

void *nyxh_test(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(real_parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetSint32(real_value,20+node_GetSint32(real_value2));
  return(value);
}

void *nyxh_open(node *state,node *execution_obj,node *block)
{
  //returns io stream object
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *filename = NULL;
  node *mode = NULL;
  node *value = create_file_class_object();
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  if(node_GetItemsNum(real_parameters))
  {
    filename = node_GetItem(real_parameters,0);
    mode = node_GetItem(real_parameters,1);
    node *real_filename = node_GetItemByKey(filename,"value");
    node *real_mode = node_GetItemByKey(mode,"value");
    if(node_GetType(real_filename)==NODE_TYPE_STRING)
    {
      char *fname = node_GetString(real_filename);
      char *m = node_GetString(real_mode);
      FILE *f = fopen(fname,m);
      set_obj_node(value,"file_handle",f);
    }
  }
  return(value);
}

void *nyxh_close(node *state,node *execution_obj,node *block)
{
  //close a file
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  long ret = -1;
  if(parent!=NULL && parent != block)
  { 
    /*node *top=parent;
    printf("close p:%s\n",get_obj_name(top));
    if(node_GetParent(top)!=NULL)
    {
      top=node_GetParent(node_GetParent(top));
      printf("close n:%s\n",get_obj_name(top));
    }*/
    node *fvalue = parent;
    node *handle = node_GetItemByKey(fvalue,"file_handle");
    if(handle!=NULL)
    {
      //printf("closing file\n");
      FILE *fhandle = node_GetValue(handle);
      ret = fclose(fhandle);
    }
    if(strcmp(get_obj_name(parent),"file"))
    {
      return(parent);
    }
  }
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  node_SetSint32(real_value,ret);
  return(value);
}

void *nyxh_readall(node *state,node *execution_obj,node *block)
{
  //returns string with all file content
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
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
      ret = (char*)malloc(len+1);
      memset(ret+len + 0, 0, 1);
      fread(ret,len,1,fhandle);
      node_SetString(real_value,ret);
      set_obj_node(value,"file_handle",fhandle);
      free(ret);
    }
  }
  return(value);
}

void *nyxh_writeall(node *state,node *execution_obj,node *block)
{
  //writes string to file 
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value2 = NULL;
  if(node_GetItemsNum(real_parameters))
  {
    value2 = node_GetItem(real_parameters,0);
  }
  if(parent!=NULL && parent != block && value2 != NULL)
  { 
    node *real_value = node_GetItemByKey(value2,"value");
    node *fvalue = parent;
    node *handle = node_GetItemByKey(fvalue,"file_handle");
    if(handle!=NULL)
    {
      FILE *fhandle = node_GetValue(handle);
      char *content = node_GetString(real_value);
      long len = strlen(content);
      fwrite(content,len,1,fhandle);
    }
  }
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

void *nyxh_from_json(node *state,node *execution_obj,node *block)
{
  //converts json string to objects
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
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
    reset_obj_refcount(value2);
    add_garbage(state,value2);
    set_obj_string(value2,"value","");
  }
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  convert_from_json(state,value,node_GetString(real_value2));
  return(value);
}

void *nyxh_to_json(node *state,node *execution_obj,node *block)
{
  //converts objects to json string
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
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
    reset_obj_refcount(value2);
    add_garbage(state,value2);
    set_obj_string(value2,"value","");
  }
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  char *json = convert_to_json(state,value2);
  node_SetString(real_value,json);
  free(json);
  return(value);
}

void *nyxh_import(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
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
    reset_obj_refcount(value2);
    add_garbage(state,value2);
    set_obj_string(value2,"value","");
  }
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  convert_from_json(state,value,node_GetString(real_value2));
  return(value);
}

void *nyxh_dump(node *state,node *execution_obj,node *block)
{
  //converts state to to json string
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
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
    reset_obj_refcount(value2);
    add_garbage(state,value2);
    set_obj_string(value2,"value","");
  }
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  char *json = state_to_json(state);
  node_SetString(real_value,json);
  free(json);
  return(value);
}

void *nyxh_execute(node *state,node *execution_obj,node *block)
{
  //converts json string to objects
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  if(node_GetItemsNum(real_parameters))
  {
    node *command = node_GetItem(real_parameters,0);
    node *real_command = node_GetItemByKey(command,"value");
    if(node_GetType(real_command)==NODE_TYPE_STRING)
    {
      char *cc = node_GetString(real_command);
      FILE *f = popen(cc,"rb");
      if(f!=NULL)
      {
        char *buf = (char*)malloc(100);
        memset(buf,0,100);
        char *ret= CreateEmptyString();
        int fr=0;
        while((fr=fread(buf,1,99,f))!=0)
        {
          ret=StringAdd(ret,buf);
          memset(buf,0,100);
        }
        node_SetString(real_value,ret);
        fclose(f);
        free(ret);
        free(buf);
      }
      else 
        printf("%s\n",strerror(errno));
    }
  }
  return(value);
}

void *nyxh_change_working_directory(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  if(node_GetItemsNum(real_parameters))
  {
    node *dir = node_GetItem(real_parameters,0);
    node *real_dir = node_GetItemByKey(dir,"value");
    char *c=node_GetString(real_dir);
    chdir(c);
  }
  return(value);
}

void *nyxh_working_directory(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  node *items = create_obj("items");
  add_obj_kv(value,items);
  node *path = create_class_instance(base_class);
  reset_obj_refcount(path);
  inc_obj_refcount(path);
  set_obj_string(path,"name","path");
  char *cwd =(char*)malloc(512);
  cwd = getcwd(cwd,512);
  set_obj_string(path,"value",cwd);
  node_AddItem(items,path);
  free(cwd);
  return(value);
}

void *nyxh_name(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  //struct utsname uname_data;
  //uname(&uname_data);
  //node_SetString(real_value,uname_data.sysname);
  node_SetString(real_value,"not supported");
  return(value);
}

void *nyxh_sys(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  node *value = create_sys_class_object();
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

void *nyxh_eval(node *state,node *execution_obj,node *block)
{
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");
  node *exe_obj = node_GetItem(execution_obj,0);
  char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
  node *parent = node_GetParent(node_GetParent(exe_obj));
  node *base_class = node_GetItemByKey(state,"nyx_object");

  prepare_execution_parameters(state,parameters,block,real_parameters);
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
    reset_obj_refcount(value2);
    add_garbage(state,value2);
    set_obj_string(value2,"value","");
  }
  node *real_value2 = node_GetItemByKey(value2,"value");
  node *nyx_stream = nyx_LoadString(node_GetString(real_value2));
  node *nyx_block = node_GetItemByKey(nyx_stream,"nyx_block");
  node_RemoveItem(nyx_stream,nyx_block);
  node_FreeTree(nyx_stream);
  node *value = create_block_obj(base_class,nyx_block);
  set_obj_node(value,"anonymous_block_parent",block);
  node_SetParent(value,NULL); //needed ?
  reset_obj_refcount(value);
  add_garbage(state,value);
  node_FreeTree(nyx_block);
  return(value);
}

