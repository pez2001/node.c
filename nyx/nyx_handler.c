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


node *nyxh_pre_add(node *state,node *obj,node *block,node *parameters)//TODO
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  if(node_GetType(real_value)==NODE_TYPE_SINT32 && node_GetType(real_value2)==NODE_TYPE_SINT32)
    node_SetSint32(real_value,node_GetSint32(real_value)+node_GetSint32(real_value2));
  else if(node_GetType(real_value)==NODE_TYPE_STRING && node_GetType(real_value2)==NODE_TYPE_STRING)
  {
    char *cat_string=str_Cat(node_GetString(real_value),node_GetString(real_value2));
    node_SetString(real_value,cat_string);
    free(cat_string);
  }
  else if(node_GetType(real_value)==NODE_TYPE_STRING && node_GetType(real_value2)==NODE_TYPE_SINT32)
  {
    char *num=str_FromLong(node_GetSint32(real_value2));
    char *cat_string=str_Cat(node_GetString(real_value),num);
    node_SetString(real_value,cat_string);
    free(cat_string);
    free(num);
  }
  return(value);
}

node *nyxh_pre_sub(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  if(node_GetType(real_value)==NODE_TYPE_SINT32)
    node_SetSint32(real_value,-node_GetSint32(real_value));
  else if(node_GetType(real_value)==NODE_TYPE_SINT32)
    node_SetDouble(real_value,-node_GetDouble(real_value));
  return(value);
}

node *nyxh_pre_not(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  if(node_GetType(real_value)==NODE_TYPE_SINT32)
    node_SetSint32(real_value,!node_GetSint32(real_value));
  else if(node_GetType(real_value)==NODE_TYPE_SINT32)
    node_SetDouble(real_value,!node_GetDouble(real_value));
  return(value);
}

node *nyxh_handler_test(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  printf("handler test called:%d\n",node_GetSint32(real_value2));
  node_SetSint32(real_value,100+node_GetSint32(real_value2));
  return(value);
}

node *nyxh_args(node *state,node *obj,node *block,node *parameters)//TODO
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  if(node_GetType(real_value)==NODE_TYPE_SINT32 && node_GetType(real_value2)==NODE_TYPE_SINT32)
    node_SetSint32(real_value,node_GetSint32(real_value)+node_GetSint32(real_value2));
  else if(node_GetType(real_value)==NODE_TYPE_STRING && node_GetType(real_value2)==NODE_TYPE_STRING)
  {
    char *cat_string=str_Cat(node_GetString(real_value),node_GetString(real_value2));
    node_SetString(real_value,cat_string);
    free(cat_string);
  }
  else if(node_GetType(real_value)==NODE_TYPE_STRING && node_GetType(real_value2)==NODE_TYPE_SINT32)
  {
    char *num=str_FromLong(node_GetSint32(real_value2));
    char *cat_string=str_Cat(node_GetString(real_value),num);
    node_SetString(real_value,cat_string);
    free(cat_string);
    free(num);
  }
  return(value);
}

node *nyxh_add(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");


  if(node_GetType(real_value)==NODE_TYPE_DOUBLE || node_GetType(real_value2)==NODE_TYPE_DOUBLE)
  {
    double d1,d2;
    if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
      d1=node_GetDouble(real_value);
    else if(node_GetType(real_value)==NODE_TYPE_SINT32)
      d1=(double)node_GetSint32(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      d2=node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      d2=(double)node_GetSint32(real_value2);
    node_SetDouble(real_value,d1+d2);
  }
  else if(node_GetType(real_value)==NODE_TYPE_SINT32 && node_GetType(real_value2)==NODE_TYPE_SINT32)
    node_SetSint32(real_value,node_GetSint32(real_value)+node_GetSint32(real_value2));
  else if(node_GetType(real_value)==NODE_TYPE_STRING && node_GetType(real_value2)==NODE_TYPE_STRING)
  {
    char *cat_string=str_Cat(node_GetString(real_value),node_GetString(real_value2));
    node_SetString(real_value,cat_string);
    free(cat_string);
  }
  else if(node_GetType(real_value)==NODE_TYPE_STRING && node_GetType(real_value2)==NODE_TYPE_SINT32)
  {
    char *num=str_FromLong(node_GetSint32(real_value2));
    char *cat_string=str_Cat(node_GetString(real_value),num);
    node_SetString(real_value,cat_string);
    free(cat_string);
    free(num);
  }
  return(value);
}

node *nyxh_sub(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  if(node_GetType(real_value)==NODE_TYPE_DOUBLE || node_GetType(real_value2)==NODE_TYPE_DOUBLE)
  {
    double d1,d2;
    if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
      d1=node_GetDouble(real_value);
    else if(node_GetType(real_value)==NODE_TYPE_SINT32)
      d1=(double)node_GetSint32(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      d2=node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      d2=(double)node_GetSint32(real_value2);
    node_SetDouble(real_value,d1-d2);
  }
  else
    node_SetSint32(real_value,node_GetSint32(real_value)-node_GetSint32(real_value2));
  return(value);
}

node *nyxh_div(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  if(node_GetType(real_value)==NODE_TYPE_DOUBLE || node_GetType(real_value2)==NODE_TYPE_DOUBLE)
  {
    double d1,d2;
    if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
      d1=node_GetDouble(real_value);
    else if(node_GetType(real_value)==NODE_TYPE_SINT32)
      d1=(double)node_GetSint32(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      d2=node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      d2=(double)node_GetSint32(real_value2);
    node_SetDouble(real_value,d1/d2);
  }
  else
    node_SetSint32(real_value,node_GetSint32(real_value)/node_GetSint32(real_value2));
  return(value);
}

node *nyxh_mul(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  if(node_GetType(real_value)==NODE_TYPE_DOUBLE || node_GetType(real_value2)==NODE_TYPE_DOUBLE)
  {
    double d1,d2;
    if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
      d1=node_GetDouble(real_value);
    else if(node_GetType(real_value)==NODE_TYPE_SINT32)
      d1=(double)node_GetSint32(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      d2=node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      d2=(double)node_GetSint32(real_value2);
    node_SetDouble(real_value,d1*d2);
  }
  else if(node_GetType(real_value)==NODE_TYPE_SINT32 && node_GetType(real_value2)==NODE_TYPE_SINT32)
    node_SetSint32(real_value,node_GetSint32(real_value)*node_GetSint32(real_value2));
  else if(node_GetType(real_value)==NODE_TYPE_STRING && node_GetType(real_value2)==NODE_TYPE_SINT32)
  {
    long mult=node_GetSint32(real_value2);
    char *dst = NULL;
    if(mult>0)
    {
      dst = str_Mult(node_GetString(real_value),mult);
      node_SetString(real_value,dst);
      free(dst);
    }
  }
  return(value);
}

node *nyxh_lt(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  if(node_GetType(real_value)==NODE_TYPE_DOUBLE || node_GetType(real_value2)==NODE_TYPE_DOUBLE)
  {
    double d1,d2;
    if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
      d1=node_GetDouble(real_value);
    else if(node_GetType(real_value)==NODE_TYPE_SINT32)
      d1=(double)node_GetSint32(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      d2=node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      d2=(double)node_GetSint32(real_value2);
    node_SetSint32(real_value,d1<d2);
  }
  else
    node_SetSint32(real_value,node_GetSint32(real_value)<node_GetSint32(real_value2));
  return(value);
}

node *nyxh_gt(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  if(node_GetType(real_value)==NODE_TYPE_DOUBLE || node_GetType(real_value2)==NODE_TYPE_DOUBLE)
  {
    double d1,d2;
    if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
      d1=node_GetDouble(real_value);
    else if(node_GetType(real_value)==NODE_TYPE_SINT32)
      d1=(double)node_GetSint32(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      d2=node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      d2=(double)node_GetSint32(real_value2);
    node_SetSint32(real_value,d1>d2);
  }
  else
    node_SetSint32(real_value,node_GetSint32(real_value)>node_GetSint32(real_value2));
  return(value);
}

node *nyxh_eq(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  if(node_GetType(real_value)==NODE_TYPE_DOUBLE || node_GetType(real_value2)==NODE_TYPE_DOUBLE)
  {
    double d1,d2;
    if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
      d1=node_GetDouble(real_value);
    else if(node_GetType(real_value)==NODE_TYPE_SINT32)
      d1=(double)node_GetSint32(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      d2=node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      d2=(double)node_GetSint32(real_value2);
    node_SetSint32(real_value,d1==d2);
  }
  else
    node_SetSint32(real_value,node_GetSint32(real_value)==node_GetSint32(real_value2));
  return(value);
}

node *nyxh_lt_eq(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  if(node_GetType(real_value)==NODE_TYPE_DOUBLE || node_GetType(real_value2)==NODE_TYPE_DOUBLE)
  {
    double d1,d2;
    if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
      d1=node_GetDouble(real_value);
    else if(node_GetType(real_value)==NODE_TYPE_SINT32)
      d1=(double)node_GetSint32(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      d2=node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      d2=(double)node_GetSint32(real_value2);
    node_SetSint32(real_value,d1<=d2);
  }
  else
    node_SetSint32(real_value,node_GetSint32(real_value)<=node_GetSint32(real_value2));
  return(value);
}

node *nyxh_gt_eq(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  if(node_GetType(real_value)==NODE_TYPE_DOUBLE || node_GetType(real_value2)==NODE_TYPE_DOUBLE)
  {
    double d1,d2;
    if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
      d1=node_GetDouble(real_value);
    else if(node_GetType(real_value)==NODE_TYPE_SINT32)
      d1=(double)node_GetSint32(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      d2=node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      d2=(double)node_GetSint32(real_value2);
    node_SetSint32(real_value,d1>=d2);
  }
  else
    node_SetSint32(real_value,node_GetSint32(real_value)>=node_GetSint32(real_value2));
  return(value);
}

node *nyxh_neq(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  if(node_GetType(real_value)==NODE_TYPE_DOUBLE || node_GetType(real_value2)==NODE_TYPE_DOUBLE)
  {
    double d1,d2;
    if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
      d1=node_GetDouble(real_value);
    else if(node_GetType(real_value)==NODE_TYPE_SINT32)
      d1=(double)node_GetSint32(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      d2=node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      d2=(double)node_GetSint32(real_value2);
    node_SetSint32(real_value,d1!=d2);
  }
  else
    node_SetSint32(real_value,node_GetSint32(real_value)!=node_GetSint32(real_value2));
  return(value);
}

// : handler
node *nyxh_set_value_only(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_GetItemByKey(obj,"value");
  node *value2 = node_GetItem(parameters,0);
  node_SetType(value,node_GetType(value2));
  node_SetValue(value,node_GetValue(value2),True,True);
  return(obj);
}

// = handler
node *nyxh_assign(node *state,node *obj,node *block,node *parameters)
{
  node *obj_name = node_GetItemByKey(obj,"name");
  node *parent = node_GetParent(obj);
  int r = node_RemoveItem(parent,obj);
  node_SetParent(obj,NULL);
  if(r==-1)
  {
    printf("error item not removed\n");
  }
  dec_obj_refcount(obj);
  add_garbage(state,obj);

  node *item_index = node_GetItemByKey(obj,"item_index");

  node *value = node_CopyTree(node_GetItem(parameters,0),True,True);
  reset_obj_refcount(value);
  //if(item_index==NULL)
  set_obj_string(value,"name",node_GetString(obj_name));
  //else 
  //  printf("leaving name as it is:[%s]\n",);
  node_AddItem(parent,value);
  inc_obj_refcount(value);
  node_SetParent(value,parent);
  //printf("assigning:[%s]\n",node_GetString(obj_name));
  if(item_index!=NULL)
  {
    node_RemoveItem(obj,item_index);
    node_SetParent(item_index,value);
    node_AddItem(value,item_index);
  }
  node *obj_parameters = node_GetItemByKey(obj,"nyx_parameters");
  if(obj_parameters!=NULL)
  {
    node *pars = node_CopyTree(obj_parameters,True,True);
    add_obj_kv(value,pars);
  }
  node *anon = node_GetItemByKey(value,"anonymous_block_parent");
  if(anon!=NULL)
  {
    node_RemoveItem(value,anon);
    node_FreeTree(anon);
  }
  return(value);
}

//println,print handler
node *nyxh_print(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  if(node_GetItemsNum(parameters))
  {
    node_ItemIterationReset(parameters);
    while(node_ItemIterationUnfinished(parameters))
    {
      node *token = node_ItemIterate(parameters);
      node *items = node_GetItemByKey(token,"items");
      if(items!=NULL)
      {
        node_ItemIterationReset(items);
        while(node_ItemIterationUnfinished(items))
        {
          node *item = node_ItemIterate(items);
          node_Print(node_GetItemByKey(item,"value"),False,False);
          fflush(stdout);
        } 
      }
      else
      {
        node_Print(node_GetItemByKey(token,"value"),False,False);
        fflush(stdout);
      }
    }      
  }
  else
  {
      node_Print(node_GetItemByKey(obj,"value"),False,False);
  }
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

node *nyxh_println(node *state,node *obj,node *block,node *parameters)
{
  node *ret = nyxh_print(state,obj,block,parameters);
  printf("\n");
  fflush(stdout);
  return(ret);
}

//else handler
node *nyxh_else(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  long parent_value = 0;
  reset_obj_refcount(value);
  add_garbage(state,value);
  parent_value = node_GetSint32(node_GetItemByKey(obj,"value"));
  node *exe_block = node_GetItem(parameters,0);
  if(!parent_value)
  {
    execute_obj(state,exe_block,block,NULL,True);
  }
  set_obj_int(value,"value",parent_value);
  return(value);
}

//do handler
node *nyxh_do(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  long parent_value = 0;
  reset_obj_refcount(value);
  add_garbage(state,value);
  parent_value = node_GetSint32(node_GetItemByKey(obj,"value"));
  node *exe_block = node_GetItem(parameters,0);
  if(parent_value)
  {
    execute_obj(state,exe_block,block,NULL,True);
  }
  set_obj_int(value,"value",parent_value);
  return(value);
}

//? handler
node *nyxh_cmp(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  //?(expression,true_func,false_func,loop_while_true); 
  //execute based on expression value (0,1,..) ,loops if needed
  //returns(expression)

  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);

  node *expression_block = node_GetItem(parameters,0);
  node *true_block = node_GetItem(parameters,1);
  node *false_block = node_GetItem(parameters,2);
  node *loop_exe = node_GetItem(parameters,3);

  inc_obj_refcount(expression_block);
  inc_obj_refcount(true_block);
  inc_obj_refcount(false_block);

  node *loop = execute_obj(state,loop_exe,block,NULL,True);
  node *loop_value = node_GetItemByKey(loop,"value");
  long lv = node_GetSint32(loop_value);
  if(lv)
  {
    node *exp_obj = execute_obj(state,expression_block,block,NULL,True);
    while(node_GetSint32(node_GetItemByKey(exp_obj,"value")))
    {
      execute_obj(state,true_block,block,NULL,True);
      char *block_flag=check_block_flag(state);
      if(!strcmp(block_flag,"break")||!strcmp(block_flag,"exit"))
        break;
      free(block_flag);
      exp_obj = execute_obj(state,expression_block,block,NULL,True);
    }
    execute_obj(state,false_block,block,NULL,True);
  }
  else
  {
    node *exp_val=execute_obj(state,expression_block,block,NULL,True);
    set_obj_int(value,"value",node_GetSint32(node_GetItemByKey(exp_val,"value")));
    if(node_GetSint32(node_GetItemByKey(exp_val,"value")))
    {
      execute_obj(state,true_block,block,NULL,True);
    }
    else
      execute_obj(state,false_block,block,NULL,True);
  }

  dec_obj_refcount(expression_block);
  dec_obj_refcount(true_block);
  dec_obj_refcount(false_block);
  add_garbage(state,expression_block);
  add_garbage(state,true_block);
  add_garbage(state,false_block);

  add_garbage(state,value);
  return(value);
}

// ?? handler
node *nyxh_init_cmp(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);

  node *init_block = node_GetItem(parameters,0);
  node *expression_block = node_GetItem(parameters,1);
  node *true_block = node_GetItem(parameters,2);
  node *false_block = node_GetItem(parameters,3);
  node *loop_exe = node_GetItem(parameters,4);
  node *loop = execute_obj(state,loop_exe,block,NULL,True);
  inc_obj_refcount(init_block);
  inc_obj_refcount(expression_block);
  inc_obj_refcount(true_block);
  inc_obj_refcount(false_block);

  execute_obj(state,init_block,block,NULL,True);
  node *loop_value = node_GetItemByKey(loop,"value");
  long lv = node_GetSint32(loop_value);
  if(lv)
  {
    node *exp_obj = execute_obj(state,expression_block,block,NULL,True);
    while(node_GetSint32(node_GetItemByKey(exp_obj,"value")))
    {
      execute_obj(state,true_block,block,NULL,True);
      char *block_flag=check_block_flag(state);
      if(!strcmp(block_flag,"break")||!strcmp(block_flag,"exit"))
        break;
      else if(!strcmp(block_flag,"restart"))
        execute_obj(state,init_block,block,NULL,True);
      free(block_flag);
      exp_obj = execute_obj(state,expression_block,block,NULL,True);
    }
    execute_obj(state,false_block,block,NULL,True);
  }
  else
  {
    node *exp_val=execute_obj(state,expression_block,block,NULL,True);
    set_obj_int(value,"value",node_GetSint32(node_GetItemByKey(exp_val,"value")));
    if(node_GetSint32(node_GetItemByKey(exp_val,"value")))
      execute_obj(state,true_block,block,NULL,True);
    else
      execute_obj(state,false_block,block,NULL,True);
  }

  dec_obj_refcount(init_block);
  dec_obj_refcount(expression_block);
  dec_obj_refcount(true_block);
  dec_obj_refcount(false_block);
  add_garbage(state,init_block);
  add_garbage(state,expression_block);
  add_garbage(state,true_block);
  add_garbage(state,false_block);

  add_garbage(state,value);
  //printf("returning :%x\n",value);
  //node_PrintTree(value);
  //fflush(stdout);
  return(value);
}

node *nyxh_break(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  set_obj_string(state,"block_flag","break");
  if(node_GetItemsNum(parameters)>0)
  {
    node *count = node_GetItem(parameters,0);
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

node *nyxh_return(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

node *nyxh_item_at(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *items = node_GetItemByKey(obj,"items");
  if(node_GetItemsNum(parameters)>0)
  {
    node *nindex = node_GetItem(parameters,0);
    node *real_nindex_value = node_GetItemByKey(nindex,"value");
    long index = node_GetSint32(real_nindex_value);
    node *value = node_GetItem(items,index);
    //if(value==NULL)
    //  printf("no item found at:%d\n",index);
    return(value);
  }
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

node *nyxh_name(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  set_obj_string(value,"value",get_obj_name(obj));
  return(value);
}

node *nyxh_restart(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  set_obj_string(state,"block_flag","restart");
  if(node_GetItemsNum(parameters)>0)
  {
    node *count = node_GetItem(parameters,0);
    node *real_value = node_GetItemByKey(count,"value");
    set_obj_int(state,"block_break_count",node_GetSint32(real_value));
    //printf("called restart:%d\n",node_GetSint32(real_value));
    //printf("block_flag:[%s]\n",node_GetString(node_GetItemByKey(state,"block_flag")));
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

node *nyxh_continue(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  set_obj_string(state,"block_flag","continue");
  if(node_GetItemsNum(parameters)>0)
  {
    node *count = node_GetItem(parameters,0);
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

node *nyxh_len(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = NULL;
  long len = 0;
  if(node_GetItemsNum(parameters))
    value = node_GetItem(parameters,0);
  else if(obj != block)
    value = obj;
  else
  {
    value = create_class_instance(base_class);
    node_SetParent(value,NULL);
    reset_obj_refcount(value);
    add_garbage(state,value);
  }
  node *items=node_GetItemByKey(value,"items");
  node *real_value = node_GetItemByKey(value,"value");
  if(items!=NULL)
  {
    len = node_GetItemsNum(items);
  }
  else
  {
    if(node_GetType(real_value)==NODE_TYPE_STRING)
      len = strlen(node_GetString(real_value));
  }

  value = create_class_instance(base_class);
  real_value = node_GetItemByKey(value,"value");
  node_SetSint32(real_value,len);
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

node *nyxh_input(node *state,node *obj,node *block,node *parameters)
{
  // a.input()
  // a=input()
  // input(a)
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = NULL;
  char *line = str_CreateEmpty();
  char c='\n';
  while((c=fgetc(stdin))!='\n' && c!=EOF && c!=0)
  {
    line = str_AddChar(line,c);
  }
  if(node_GetItemsNum(parameters))
  {
    value = node_GetItem(parameters,0);
  }
  else if(obj != block)
  { 
    value = obj;
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

node *nyxh_http_query(node *state,node *obj,node *block,node *parameters)
{
  //returns http query vars as array
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = NULL;
  if(node_GetItemsNum(parameters))
  {
    value = node_GetItem(parameters,0);
  }
  else if(obj != block)
  { 
    value = obj;
  }
  else
  {
    value = create_class_instance(base_class);
    node_SetParent(value,NULL);
    reset_obj_refcount(value);
    add_garbage(state,value);
  }
  append_http_query_array(state,value);
  return(value);
}

node *nyxh_int(node *state,node *obj,node *block,node *parameters)
{
  //returns integer of string input
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value2 = NULL;
  if(node_GetItemsNum(parameters))
  {
    value2 = node_GetItem(parameters,0);
  }
  else if(obj != block)
  { 
    value2 = obj;
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

node *nyxh_float(node *state,node *obj,node *block,node *parameters)
{
  //returns float of string input
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value2 = NULL;
  if(node_GetItemsNum(parameters))
  {
    value2 = node_GetItem(parameters,0);
  }
  else if(obj != block)
  { 
    value2 = obj;
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
    node_SetDouble(real_value,atof(node_GetString(real_value2)));
  return(value);
}

node *nyxh_str(node *state,node *obj,node *block,node *parameters)
{
  //returns string of integer or float input
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value2 = NULL;
  if(node_GetItemsNum(parameters))
  {
    value2 = node_GetItem(parameters,0);
  }
  else if(obj != block)
  { 
    value2 = obj;
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
  {
    char *str = str_FromLong(node_GetSint32(real_value2));
    node_SetString(real_value,str);
    free(str);
  }
  else if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
  {
    char *str = str_FromDouble(node_GetDouble(real_value2));
    node_SetString(real_value,str);
    free(str);
  }
  else if(node_GetType(real_value2)==NODE_TYPE_STRING)
    node_SetString(real_value,node_GetString(real_value2));
  return(value);
}

node *nyxh_and(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *input_value = node_GetItemByKey(obj,"value");
  node *input2_value = node_GetItemByKey(value2,"value");
  if(node_GetType(input_value)==NODE_TYPE_DOUBLE || node_GetType(input2_value)==NODE_TYPE_DOUBLE)
  {
    long d1,d2;
    if(node_GetType(input_value)==NODE_TYPE_DOUBLE)
      d1=(long)node_GetDouble(input_value);
    else if(node_GetType(input_value)==NODE_TYPE_SINT32)
      d1=node_GetSint32(input_value);
    if(node_GetType(input2_value)==NODE_TYPE_DOUBLE)
      d2=(long)node_GetDouble(input2_value);
    else if(node_GetType(input2_value)==NODE_TYPE_SINT32)
      d2=node_GetSint32(input2_value);
    node_SetSint32(real_value,d1&d2);
  }
  else
  {
    node_SetSint32(real_value,node_GetSint32(input_value)&node_GetSint32(input2_value));
  }
  return(value);
}

node *nyxh_or(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *input_value = node_GetItemByKey(obj,"value");
  node *input2_value = node_GetItemByKey(value2,"value");
  if(node_GetType(input_value)==NODE_TYPE_DOUBLE || node_GetType(input2_value)==NODE_TYPE_DOUBLE)
  {
    long d1,d2;
    if(node_GetType(input_value)==NODE_TYPE_DOUBLE)
      d1=(long)node_GetDouble(input_value);
    else if(node_GetType(input_value)==NODE_TYPE_SINT32)
      d1=node_GetSint32(input_value);
    if(node_GetType(input2_value)==NODE_TYPE_DOUBLE)
      d2=(long)node_GetDouble(input2_value);
    else if(node_GetType(input2_value)==NODE_TYPE_SINT32)
      d2=node_GetSint32(input2_value);
    node_SetSint32(real_value,d1|d2);
  }
  else
    node_SetSint32(real_value,node_GetSint32(input_value)|node_GetSint32(input2_value));
  return(value);
}

node *nyxh_test(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetSint32(real_value,20+node_GetSint32(real_value2));
  return(value);
}

node *nyxh_open(node *state,node *obj,node *block,node *parameters)
{
  //returns io stream object
  node *filename = NULL;
  node *mode = NULL;
  node *value = create_file_class_object();
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  if(node_GetItemsNum(parameters))
  {
    filename = node_GetItem(parameters,0);
    mode = node_GetItem(parameters,1);
    node *real_filename = node_GetItemByKey(filename,"value");
    node *real_mode = node_GetItemByKey(mode,"value");
    if(node_GetType(real_filename)==NODE_TYPE_STRING)
    {
      char *fname = node_GetString(real_filename);
      char *m = node_GetString(real_mode);
      FILE *f = fopen(fname,m);
      set_obj_ptr(value,"file_handle",f);
    }
  }
  return(value);
}

node *nyxh_close(node *state,node *obj,node *block,node *parameters)
{
  //close a file
  node *base_class = node_GetItemByKey(state,"nyx_object");
  long ret = -1;
  //printf("closing file\n");
  //node_PrintTree(obj);
  node *handle = node_GetItemByKey(obj,"file_handle");
  if(handle!=NULL)
  {
    FILE *fhandle = node_GetValue(handle);
    ret = fclose(fhandle);
    //node_SetNull(handle);
  }
  if(strcmp(get_obj_name(obj),"file"))
  {
    //printf("chained return\n");
    return(obj);
  }
  //printf("unchained return\n");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  node_SetSint32(real_value,ret);
  return(value);
}

node *nyxh_readall(node *state,node *obj,node *block,node *parameters)
{
  //returns string with all file content
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  node *handle = node_GetItemByKey(obj,"file_handle");
  if(handle!=NULL)
  {
    char *ret = NULL;
    FILE *fhandle = node_GetValue(handle);
    fseek(fhandle,0,SEEK_END);
    long len = ftell(fhandle);
    fseek(fhandle,0,SEEK_SET);
    ret = (char*)malloc(len+1);
    memset(ret+len + 0, 0, 1);
    fread(ret,len,1,fhandle);
    node_SetString(real_value,ret);
    set_obj_ptr(value,"file_handle",fhandle);
    add_class_object_function(value,"close",nyxh_close);
    free(ret);
  }
  return(value);
}

node *nyxh_writeall(node *state,node *obj,node *block,node *parameters)
{
  //writes string to file 
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = NULL;
  if(node_GetItemsNum(parameters))
  {
    value2 = node_GetItem(parameters,0);
    //if(obj != block)
    //{ 
    node *real_value = node_GetItemByKey(value2,"value");
    node *handle = node_GetItemByKey(obj,"file_handle");
    if(handle!=NULL)
    {
      FILE *fhandle = node_GetValue(handle);
      char *content = node_GetString(real_value);
      long len = strlen(content);
      fwrite(content,len,1,fhandle);
      set_obj_ptr(value,"file_handle",fhandle);
    }
    //}
  }
  return(value);
}

node *nyxh_from_json(node *state,node *obj,node *block,node *parameters)
{
  //converts json string to objects
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value2 = NULL;
  if(node_GetItemsNum(parameters))
  {
    value2 = node_GetItem(parameters,0);
  }
  else if(obj != block)
  { 
    value2 = obj;
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
  node *real_value2 = node_GetItemByKey(value2,"value");
  convert_from_json(state,value,node_GetString(real_value2));
  return(value);
}

node *nyxh_split(node *state,node *obj,node *block,node *parameters)
{
  //splits string using splitting characters into a string array
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value2 = NULL;
  node *splitter = NULL;
  if(node_GetItemsNum(parameters))
  {
    if(node_GetItemsNum(parameters)>1)
    {
      value2 = node_GetItem(parameters,0);
      splitter = node_GetItem(parameters,1);
    }
    else
      splitter = node_GetItem(parameters,0);
  }
  
  if(obj != block)
  { 
    value2 = obj;
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
  node *items = create_obj("items");
  add_obj_kv(value,items);
  if(splitter==NULL)
    return(value);


  node *real_value2 = node_GetItemByKey(value2,"value");
  node *splitter_value = node_GetItemByKey(splitter,"value");
  char *str = node_GetString(real_value2);
  char *splitter_str = node_GetString(splitter_value);
  //now split the string
  long str_len = strlen(str);
  long splitter_len = strlen(splitter_str);
  long i=0;
  long last_split=0;
  long item_index = 0;
  while(i<str_len)
  {
    char c = str[i];
    long spi= 0;
    while(spi<splitter_len)
    {
      char s = splitter_str[spi];
      if(c==s)
      {
        //split here
        long tok_len = i - last_split;
        if(tok_len)
        {
          char *tok = str_Sub(str,last_split,tok_len);
          node *ntok = create_class_instance(base_class);
          reset_obj_refcount(ntok);
          inc_obj_refcount(ntok);
          set_obj_string(ntok,"name","token");
          set_obj_string(ntok,"value",tok);
          node_AddItem(items,ntok);
          set_obj_int(ntok,"item_index",item_index);
          item_index++;
          free(tok);
        }
        last_split = i+1;
      }
      spi++;
    }
    i++;
  }
  //account for a possible last token
  long tok_len = str_len - last_split;
  if(tok_len)
  {
    char *tok = str_Sub(str,last_split,tok_len);
    node *ntok = create_class_instance(base_class);
    reset_obj_refcount(ntok);
    inc_obj_refcount(ntok);
    set_obj_string(ntok,"name","token");
    set_obj_string(ntok,"value",tok);
    node_AddItem(items,ntok);
    set_obj_int(ntok,"item_index",item_index);
    free(tok);
  }
  return(value);
}

node *nyxh_to_json(node *state,node *obj,node *block,node *parameters)
{
  //converts objects to json string
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value2 = NULL;
  if(node_GetItemsNum(parameters))
  {
    value2 = node_GetItem(parameters,0);
  }
  else if(obj != block)
  { 
    value2 = obj;
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
  char *json = convert_to_json(state,value2);
  node_SetString(real_value,json);
  free(json);
  return(value);
}

node *nyxh_import(node *state,node *obj,node *block,node *parameters)//TODO
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value2 = NULL;
  if(node_GetItemsNum(parameters))
  {
    value2 = node_GetItem(parameters,0);
  }
  else if(obj != block)
  { 
    value2 = obj;
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
  node *real_value2 = node_GetItemByKey(value2,"value");
  convert_from_json(state,value,node_GetString(real_value2));
  return(value);
}

node *nyxh_sys_dump(node *state,node *obj,node *block,node *parameters)
{
  //dumps the whole interpreter state as json
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = NULL;
  if(node_GetItemsNum(parameters))
  {
    value = node_GetItem(parameters,0);
  }
  else if(obj != block)
  {
    value = obj;
  }
  else
  { 
    value = create_class_instance(base_class);
    reset_obj_refcount(value);
    add_garbage(state,value);
  }
  node *real_value = node_GetItemByKey(value,"value");
  char *json = state_to_json(state);
  node_SetString(real_value,json);
  free(json);
  return(value);
}

node *nyxh_dump_obj(node *state,node *obj,node *block,node *parameters)
{
  //dumps object as json
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value2 = NULL;
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  if(node_GetItemsNum(parameters))
  {
    value2 = node_GetItem(parameters,0);
  }
  else //if(obj != block)
  {
    value2 = obj;
  }/*
  else
  { 
    value2 = create_class_instance(base_class);
    reset_obj_refcount(value2);
    add_garbage(state,value2);
  }*/
  node *real_value = node_GetItemByKey(value,"value");
  char *json = convert_to_json(state,value2);//TODO add parameter to not filter any sub obj
  node_SetString(real_value,json);
  free(json);
  return(value);
}

node *nyxh_sys_execute(node *state,node *obj,node *block,node *parameters)
{
  //execute an external program and returns the output
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  if(node_GetItemsNum(parameters))
  {
    node *command = node_GetItem(parameters,0);
    node *real_command = node_GetItemByKey(command,"value");
    if(node_GetType(real_command)==NODE_TYPE_STRING)
    {
      char *cc = node_GetString(real_command);
      FILE *f = popen(cc,"rb");
      if(f!=NULL)
      {
        char *buf = (char*)malloc(100);
        memset(buf,0,100);
        char *ret= str_CreateEmpty();
        int fr=0;
        while((fr=fread(buf,1,99,f))!=0)
        {
          ret=str_CatFree(ret,buf);
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

node *nyxh_sys_change_working_directory(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  if(node_GetItemsNum(parameters))
  {
    node *dir = node_GetItem(parameters,0);
    node *real_dir = node_GetItemByKey(dir,"value");
    char *c=node_GetString(real_dir);
    chdir(c);
  }
  return(value);
}

node *nyxh_sys_working_directory(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *items = create_obj("items");
  add_obj_kv(value,items);
  node *path = create_class_instance(base_class);
  reset_obj_refcount(path);
  inc_obj_refcount(path);
  set_obj_string(path,"name","path");
  set_obj_int(path,"item_index",0);
  char *cwd =(char*)malloc(512);
  cwd = getcwd(cwd,512);
  set_obj_string(path,"value",cwd);
  node_AddItem(items,path);
  free(cwd);
  return(value);
}

node *nyxh_sys_interpreter_version(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  set_obj_string(value,"name","nyx_version");
  node *items = create_obj("items");
  add_obj_kv(value,items);
  node *major = create_class_instance(base_class);
  reset_obj_refcount(major);
  inc_obj_refcount(major);
  set_obj_string(major,"name","major");
  set_obj_int(major,"value",(long) MAJOR_VERSION);
  set_obj_int(major,"item_index",0);
  node_AddItem(items,major);
  node *minor = create_class_instance(base_class);
  reset_obj_refcount(minor);
  inc_obj_refcount(minor);
  set_obj_string(minor,"name","minor");
  set_obj_int(minor,"value",(long) MINOR_VERSION);
  set_obj_int(minor,"item_index",1);
  node_AddItem(items,minor);
  node *build = create_class_instance(base_class);
  reset_obj_refcount(build);
  inc_obj_refcount(build);
  set_obj_string(build,"name","build");
  set_obj_int(build,"value",(long) BUILD+1);
  set_obj_int(build,"item_index",2);
  node_AddItem(items,build);
  return(value);
}

node *nyxh_sys_interpreter_filename(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *interpreter_filename = node_GetItemByKey(state,"interpreter_filename");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  if(interpreter_filename!=NULL)
  {
    node_SetString(real_value,node_GetString(interpreter_filename));
  }
  else
    node_SetString(real_value,"");
  return(value);
}

node *nyxh_sys_script_filename(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *script_filename = node_GetItemByKey(state,"script_filename");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  if(script_filename!=NULL)
  {
    node_SetString(real_value,node_GetString(script_filename));
  }
  else
    node_SetString(real_value,"");
  return(value);
}

node *nyxh_sys_exit(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  set_obj_string(state,"block_flag","exit");
  if(node_GetItemsNum(parameters)>0)
  {
    node *ecode = node_GetItem(parameters,0);
    node *real_value = node_GetItemByKey(ecode,"value");
    set_obj_int(state,"exit_code",node_GetSint32(real_value));
  }
  else
  {
    set_obj_int(state,"exit_code",0);
  }
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

node *nyxh_get_block(node *state,node *obj,node *block,node *parameters)
{
  return(block);
}

node *nyxh_parent(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *mparent = node_GetParent(obj);
  if(mparent != NULL)
  {
    node *parent = node_GetParent(mparent);
    if(parent != NULL)
      return(parent);
  }
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

node *nyxh_sys_time(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  node_SetSint32(real_value,(long)time(NULL));
  return(value);
}

node *nyxh_sys_name(node *state,node *obj,node *block,node *parameters)
{
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

node *nyxh_sys(node *state,node *obj,node *block,node *parameters)
{
  node *value = create_sys_class_object();
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

node *nyxh_eval(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value2 = NULL;
  if(node_GetItemsNum(parameters))
  {
    value2 = node_GetItem(parameters,0);
  }
  else if(obj != block)
  { 
    value2 = obj;
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
  node *value = create_block_class_object(base_class,nyx_block);
  set_obj_node(value,"anonymous_block_parent",block);
  node_SetParent(value,NULL); //needed ?
  reset_obj_refcount(value);
  add_garbage(state,value);
  node_FreeTree(nyx_block);
  return(value);
}

node *nyxh_socket_open(node *state,node *obj,node *block,node *parameters)
{
  //returns socket stream object
  //node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_socket_class_object();
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  set_obj_int(value,"value",0);
  if(node_GetItemsNum(parameters))
  {
    //node *nprotocol = NULL;
    node *ntype = NULL;
    //node *ndomain = NULL;
    //node *real_ndomain = NULL;
    //node *real_nprotocol = NULL;
    ntype = node_GetItem(parameters,0);
    //if(node_GetItemsNum(parameters)>=1)
    //  ndomain = node_GetItem(parameters,1);
    //if(node_GetItemsNum(parameters)>=2)
    //  nprotocol = node_GetItem(parameters,2);
    node *real_ntype = node_GetItemByKey(ntype,"name");
    //if(ndomain!=NULL)
    //  real_ndomain = node_GetItemByKey(ndomain,"name");
    //if(nprotocol!=NULL)
    //  real_nprotocol = node_GetItemByKey(nprotocol,"name");
    if(node_GetType(real_ntype)==NODE_TYPE_STRING)
    {
      int type = SOCK_STREAM;
      int domain = PF_INET;
      int protocol = 0;
      char *stype = node_GetString(real_ntype);
      if(!strcmp(stype,"SOCK_DGRAM"))
        type = SOCK_DGRAM;
      int sock = socket(domain,type,protocol);
      set_obj_int(value,"socket_handle",(long)sock);
      set_obj_int(value,"socket_type",(long)type);
      //printf("socket_handle opened:%d\n",sock);
    }
  }
  return(value);
}

node *nyxh_socket_close(node *state,node *obj,node *block,node *parameters)
{
  //close a socket
  node *base_class = node_GetItemByKey(state,"nyx_object");
  long ret = -1;
  node *handle = node_GetItemByKey(obj,"socket_handle");
  if(handle!=NULL)
  {
    int socket_handle = (int)node_GetSint32(handle);
    ret = close(socket_handle);
    //set_obj_int(obj,"value",0);
  }
  if(strcmp(get_obj_name(obj),"socket"))
  {
    return(obj);
  }
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  node_SetSint32(real_value,ret);
  return(value);
}

node *nyxh_socket_set_address(node *state,node *obj,node *block,node *parameters)
{
  //set address of stream object
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  //node *real_value = node_GetItemByKey(value,"value");
  node *socket_handle = node_GetItemByKey(obj,"socket_handle");
  if(socket_handle!=NULL)
  {
    //int sock = node_GetValue(socket_handle);
    if(node_GetItemsNum(parameters))
    {
      node *nhost = node_GetItem(parameters,0);
      node *nport = node_GetItem(parameters,1);
      node *real_nhost = node_GetItemByKey(nhost,"value");
      node *real_nport = node_GetItemByKey(nport,"value");
      if(node_GetType(real_nhost)==NODE_TYPE_STRING && node_GetType(real_nport)==NODE_TYPE_SINT32)
      { 
        struct sockaddr_in addr;
        char *host = node_GetString(real_nhost);
        int port = node_GetSint32(real_nport);
        addr.sin_family = AF_INET;
        addr.sin_port = port;
        addr.sin_addr.s_addr = inet_addr(host);
        //addr.sin_zero = 0;
        set_obj_ptr(obj,"socket_addr",&addr);
      }
    }
  }
  return(value);
}

node *nyxh_socket_connect(node *state,node *obj,node *block,node *parameters)
{
  //connect stream object to some host and port
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  //node *real_value = node_GetItemByKey(value,"value");
  node *socket_handle = node_GetItemByKey(obj,"socket_handle");
  node *socket_type = node_GetItemByKey(obj,"socket_type");
  set_obj_int(obj,"value",0);
  set_obj_int(value,"value",0);
  if(socket_handle!=NULL && socket_type != NULL)
  {
    int sock = (int)node_GetSint32(socket_handle);
    int type = (int)node_GetSint32(socket_type);
    //int orgsock=sock;
    //node_SetString(real_value,ret);
    if(node_GetItemsNum(parameters))
    {
      node *nhost = node_GetItem(parameters,0);
      node *nport = node_GetItem(parameters,1);
      node *real_nhost = node_GetItemByKey(nhost,"value");
      node *real_nport = node_GetItemByKey(nport,"value");
      if(node_GetType(real_nhost)==NODE_TYPE_STRING && node_GetType(real_nport)==NODE_TYPE_SINT32)
      { 
        struct addrinfo hints;
        struct addrinfo *result;
        int r;
        char *host = node_GetString(real_nhost);
        int port = node_GetSint32(real_nport);
        char *sport = str_FromLong(port);
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = type;
        hints.ai_flags = AI_PASSIVE;
        hints.ai_protocol = 0;
        hints.ai_canonname = NULL;
        hints.ai_addr = NULL;
        hints.ai_next = NULL;


        r = getaddrinfo(host,sport,&hints,&result);
        if(r==0)
        {
          //printf("host:%s found!:%d\n",host,sock);
          struct addrinfo *test=result;
          while(test!=NULL)
          {
            /*struct timeval tv;
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            if(setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char*)&tv,sizeof(tv)))
            { 
              perror("setsockopt");
              return(value); 
            }*/ 
            /*int timeout=10;
            if(setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout)))
            { 
              perror("setsockopt");
              return(value); 
            }*/             
            if(connect(sock,test->ai_addr,test->ai_addrlen)!=-1)
            {
              //printf("connected to host\n");
              unsigned long one = 1;
              if(ioctlsocket(sock,FIONBIO,&one)==SOCKET_ERROR)
              {
                perror("ioctlsocket");
                return(value); 
              }
              /*int timeout=0;
              if(setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout)))
              { 
                perror("setsockopt");
                return(value); 
              }*/              
              set_obj_int(value,"value",1);
              set_obj_int(obj,"value",1);
              break;
            }
            printf("reopening socket\n");
            close(sock);
            sock = socket(test->ai_family,type,test->ai_protocol);
            node_SetSint32(socket_handle,sock);
            //printf("updated socket handle\n");
            test=test->ai_next;
          }

          if(test!=NULL && result!=NULL)
            freeaddrinfo(result);
        }
        else
        {
          printf("host:%s not found:%d\n",host,r);
        }
        free(sport);
      }
    }
  }
  return(value);
}

node *nyxh_socket_read(node *state,node *obj,node *block,node *parameters)
{
  //returns string with data received
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  node *nsocket_handle = node_GetItemByKey(obj,"socket_handle");
  //node *ntype = node_GetItemByKey(fvalue,"socket_type");
  if(nsocket_handle!=NULL)
  {
    char *ret = NULL;
    int socket_handle = (int)node_GetSint32(nsocket_handle);
    //printf("receiving data:%d\n",socket_handle);
    //fflush(stdout);
    int len = 1023;
    int data_received = 0;
    char buf[1024];
    ret = str_CreateEmpty();
    while(len==1023)
    {
      memset(buf,0,1024);
      len = recv(socket_handle,(char*)&buf,1023,0);
      data_received+=len;
      //printf("recv len:%d\n",len);
      //fflush(stdout);
      if(len<0)
      {
        //printf("recv error:%d\n",errno);
        //printf("error receiving data:%d\n",len);
        
        //if(errno!=10035 && errno != 0)
          break;
        //else
        //  len=1023;
      }
      if(len==0)
      {
        //printf("received data:%d\n",data_received);
        break;
      }
      if(len)
        ret=str_CatFree(ret,buf);
    }
    node_SetString(real_value,ret);
    set_obj_int(value,"socket_handle",(long)socket_handle);
    free(ret);
  }
  return(value);
}

node *nyxh_socket_write(node *state,node *obj,node *block,node *parameters)
{
  //writes string to socket
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = NULL;
  if(node_GetItemsNum(parameters))
  {
    value2 = node_GetItem(parameters,0);
  }
  if(obj != block && value2 != NULL)
  { 
    node *real_value = node_GetItemByKey(value2,"value");
    node *nsocket_handle = node_GetItemByKey(obj,"socket_handle");
    if(nsocket_handle!=NULL)
    {
      int socket_handle = (int)node_GetSint32(nsocket_handle);
      char *content = node_GetString(real_value);
      //printf("sending:[%s]\n",content);
      long len = strlen(content);
      int data_send = 0;
      int num = 0;
      while(data_send<len)
      {
        num = send(socket_handle,content+data_send,len-data_send,0);
        if(num==-1)
        {
          set_obj_int(value,"value",-1);        
          break;
        }
        data_send+=num;
      }
    }
  }
  return(value);
}

node *nyxh_http_create_request(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  set_obj_string(value,"name","http.request");
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  node_SetString(real_value,"not supported");
  return(value);
}

node *nyxh_http_parse_answer(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value2 = NULL;
  node *value = create_class_instance(base_class);
  set_obj_string(value,"name","http.answer");
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *items = create_obj("items");
  add_obj_kv(value,items);

  if(node_GetItemsNum(parameters))
  {
    value2 = node_GetItem(parameters,0);
  }
  else if(obj != block)
  { 
    value2 = obj;
  }
  else
  {
    value2 = create_class_instance(base_class);
    reset_obj_refcount(value2);
    add_garbage(state,value2);
    set_obj_string(value2,"value","");
  }
  node *real_value2 = node_GetItemByKey(value2,"value");
  char *answer = node_GetString(real_value2);
  long answer_len = strlen(answer);
  if(!answer_len)
    return(value);
  //long header_length = 0;
  long i = 0;
  long last_line_start = 0;
  long last_line_len = 0;
  while(i<answer_len)
  {
    char c = answer[i];
    if(c=='\r' || c == '\n')
    {
      if(i-last_line_start)
      {
        char *line = str_Sub(answer,last_line_start,i-last_line_start);
        //printf("line:[%s]\n",line);
        free(line);
      }
      if(last_line_len==0 && (i-last_line_start)==0)
      {
        //printf("header end at:%d\n",i);
        i++;
        break;
      }
      last_line_len = i-last_line_start;
      last_line_start=i+1;
    }
    i++;
  }
  char *body = str_Sub(answer,i+1,answer_len-i-1);
  //printf("body:[%s]\n",body);

  /*
  HTTP/1.1 200 OK
  Date: Thu, 15 Jul 2004 19:20:21 GMT
  Server: Apache/1.3.5 (Unix)
  Accept-Ranges: bytes
  Content-length: 46
  Connection: close
  Content-type: text/html

  <h1>Antwort</h1>
  <p>Ich bin eine Antwort</p>
  */
  char *header = str_Sub(answer,0,i);
  node *nheader = create_class_instance(base_class);
  reset_obj_refcount(nheader);
  inc_obj_refcount(nheader);
  set_obj_string(nheader,"name","header");
  set_obj_string(nheader,"value",header);
  node_AddItem(items,nheader);
  set_obj_int(nheader,"item_index",0);
  free(header);


  node *nbody = create_class_instance(base_class);
  reset_obj_refcount(nbody);
  inc_obj_refcount(nbody);
  set_obj_string(nbody,"name","body");
  set_obj_string(nbody,"value",body);
  node_AddItem(items,nbody);
  set_obj_int(nbody,"item_index",1);
  free(body);
  return(value);
}


