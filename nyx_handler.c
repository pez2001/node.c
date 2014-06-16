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


void *nyxh_pre_add(node *state,node *obj,node *block,node *parameters)
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

void *nyxh_pre_sub(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  node_SetSint32(real_value,-node_GetSint32(real_value));
  return(value);
}

void *nyxh_handler_test(node *state,node *obj,node *block,node *parameters)
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

void *nyxh_args(node *state,node *obj,node *block,node *parameters)
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

void *nyxh_add(node *state,node *obj,node *block,node *parameters)
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

void *nyxh_sub(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetSint32(real_value,node_GetSint32(real_value)-node_GetSint32(real_value2));
  return(value);
}

void *nyxh_div(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetSint32(real_value,node_GetSint32(real_value)/node_GetSint32(real_value2));
  return(value);
}

void *nyxh_mul(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
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

void *nyxh_lt(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetSint32(real_value,node_GetSint32(real_value)<node_GetSint32(real_value2));
  return(value);
}

void *nyxh_gt(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetSint32(real_value,node_GetSint32(real_value)>node_GetSint32(real_value2));
  return(value);
}

void *nyxh_eq(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetSint32(real_value,node_GetSint32(real_value)==node_GetSint32(real_value2));
  return(value);
}

void *nyxh_lt_eq(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetSint32(real_value,node_GetSint32(real_value)<=node_GetSint32(real_value2));
  return(value);
}

void *nyxh_gt_eq(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetSint32(real_value,node_GetSint32(real_value)>=node_GetSint32(real_value2));
  return(value);
}

void *nyxh_neq(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetSint32(real_value,node_GetSint32(real_value)!=node_GetSint32(real_value2));
  return(value);
}

// : handler
void *nyxh_set_value_only(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_GetItemByKey(obj,"value");
  node *value2 = node_GetItem(parameters,0);
  node *real_value2 = node_GetItemByKey(value2,"value");
  node_SetType(value,node_GetType(value2));
  node_SetValue(value,node_GetValue(value2),True,True);
  return(obj);
}

// = handler
void *nyxh_assign(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
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
  node *value = node_CopyTree(node_GetItem(parameters,0),True,True);
  reset_obj_refcount(value);
  set_obj_string(value,"name",node_GetString(obj_name));
  node_AddItem(parent,value);
  printf("assigned:%x old:%x (%s)\n",value,parent,get_obj_name(value));
  fflush(stdout);
  inc_obj_refcount(value);
  node_SetParent(value,parent);
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
void *nyxh_print(node *state,node *obj,node *block,node *parameters)
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
      node_Print(node_GetItemByKey(obj,"value"),False,False);
  }
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

void *nyxh_println(node *state,node *obj,node *block,node *parameters)
{
  node *ret = nyxh_print(state,obj,block,parameters);
  printf("\n");
  return(ret);
}

//else handler
void *nyxh_else(node *state,node *obj,node *block,node *parameters)
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
    node *blk_val=execute_obj(state,exe_block,block,NULL,True);
  }
  set_obj_int(value,"value",parent_value);
  return(value);
}

//? handler
void *nyxh_cmp(node *state,node *obj,node *block,node *parameters)
{
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
  node *loop = execute_obj(state,loop_exe,block,NULL,True);
  node *loop_value = node_GetItemByKey(loop,"value");
  long lv = node_GetSint32(loop_value);
  if(lv)
  {
    node *exp_obj = execute_obj(state,expression_block,block,NULL,True);
    while(node_GetSint32(node_GetItemByKey(exp_obj,"value")))
    {
      node *eval=execute_obj(state,true_block,block,NULL,True);
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
      exp_obj = execute_obj(state,expression_block,block,NULL,True);
    }
    node *blk_val=execute_obj(state,false_block,block,NULL,True);
  }
  else
  {
    node *exp_val=execute_obj(state,expression_block,block,NULL,True);
    node *blk_val=NULL;
    set_obj_int(value,"value",node_GetSint32(node_GetItemByKey(exp_val,"value")));
    if(node_GetSint32(node_GetItemByKey(exp_val,"value")))
    {
      blk_val=execute_obj(state,true_block,block,NULL,True);
    }
    else
      blk_val=execute_obj(state,false_block,block,NULL,True);
  }
  return(value);
}

// ?? handler
void *nyxh_init_cmp(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *init_block = node_GetItem(parameters,0);
  node *expression_block = node_GetItem(parameters,1);
  node *true_block = node_GetItem(parameters,2);
  node *false_block = node_GetItem(parameters,3);
  node *loop_exe = node_GetItem(parameters,4);
  node *loop = execute_obj(state,loop_exe,block,NULL,False);//,tmp_dont_execute_block);
  node *init_eval = execute_obj(state,init_block,block,NULL,True);
  node *loop_value = node_GetItemByKey(loop,"value");
  long lv = node_GetSint32(loop_value);
  if(lv)
  {
    node *exp_obj = execute_obj(state,expression_block,block,NULL,True);
    while(node_GetSint32(node_GetItemByKey(exp_obj,"value")))
    {
      node *eval=execute_obj(state,true_block,block,NULL,True);
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
      exp_obj = execute_obj(state,expression_block,block,NULL,True);
    }
  }
  else
  {
    node *exp_val=execute_obj(state,expression_block,block,NULL,True);
    node *blk_val=NULL;
    if(node_GetSint32(node_GetItemByKey(exp_val,"value")))
      blk_val=execute_obj(state,true_block,block,NULL,True);
    else
      blk_val=execute_obj(state,false_block,block,NULL,True);
  }
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

void *nyxh_break(node *state,node *obj,node *block,node *parameters)
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

void *nyxh_restart(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  set_obj_string(state,"block_flag","restart");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

void *nyxh_continue(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  set_obj_string(state,"block_flag","continue");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

void *nyxh_len(node *state,node *obj,node *block,node *parameters)
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
  if(items!=NULL)
    len = node_GetItemsNum(items);
  node *real_value = node_GetItemByKey(value,"value");
  node_SetSint32(real_value,len);
  return(value);
}

void *nyxh_input(node *state,node *obj,node *block,node *parameters)
{
  // a.input()
  // a=input()
  // input(a)
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = NULL;
  char *line = CreateEmptyString();
  char c='\n';
  while((c=fgetc(stdin))!='\n' && c!=EOF && c!=0)
  {
    line = AddCharToString(line,c);
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

void *nyxh_http_query(node *state,node *obj,node *block,node *parameters)
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
  node *real_value = node_GetItemByKey(value,"value");
  append_http_query_array(state,value);
  return(value);
}

void *nyxh_int(node *state,node *obj,node *block,node *parameters)
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

void *nyxh_str(node *state,node *obj,node *block,node *parameters)
{
  //returns string of integer input
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
    node_SetString(real_value,convert_to_string(node_GetSint32(real_value2)));
  else if(node_GetType(real_value2)==NODE_TYPE_STRING)
    node_SetString(real_value,node_GetString(real_value2));
  return(value);
}

void *nyxh_test(node *state,node *obj,node *block,node *parameters)
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

void *nyxh_open(node *state,node *obj,node *block,node *parameters)
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
      set_obj_node(value,"file_handle",f);
    }
  }
  return(value);
}

void *nyxh_close(node *state,node *obj,node *block,node *parameters)
{
  //close a file
  node *base_class = node_GetItemByKey(state,"nyx_object");
  long ret = -1;
  if(obj != block)
  { 
    node *fvalue = obj;
    node *handle = node_GetItemByKey(fvalue,"file_handle");
    if(handle!=NULL)
    {
      FILE *fhandle = node_GetValue(handle);
      ret = fclose(fhandle);
    }
    if(strcmp(get_obj_name(obj),"file"))
    {
      return(obj);
    }
  }
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  node_SetSint32(real_value,ret);
  return(value);
}

void *nyxh_readall(node *state,node *obj,node *block,node *parameters)
{
  //returns string with all file content
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  char *ret = NULL;
  if(obj != block)
  { 
    node *fvalue = obj;
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

void *nyxh_writeall(node *state,node *obj,node *block,node *parameters)
{
  //writes string to file 
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value2 = NULL;
  if(node_GetItemsNum(parameters))
  {
    value2 = node_GetItem(parameters,0);
  }
  if(obj != block && value2 != NULL)
  { 
    node *real_value = node_GetItemByKey(value2,"value");
    node *fvalue = obj;
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

void *nyxh_from_json(node *state,node *obj,node *block,node *parameters)
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
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  convert_from_json(state,value,node_GetString(real_value2));
  return(value);
}

void *nyxh_to_json(node *state,node *obj,node *block,node *parameters)
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
  node *real_value2 = node_GetItemByKey(value2,"value");
  char *json = convert_to_json(state,value2);
  node_SetString(real_value,json);
  free(json);
  return(value);
}

void *nyxh_import(node *state,node *obj,node *block,node *parameters)
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
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  convert_from_json(state,value,node_GetString(real_value2));
  return(value);
}

void *nyxh_dump(node *state,node *obj,node *block,node *parameters)
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

void *nyxh_execute(node *state,node *obj,node *block,node *parameters)
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

void *nyxh_change_working_directory(node *state,node *obj,node *block,node *parameters)
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

void *nyxh_working_directory(node *state,node *obj,node *block,node *parameters)
{
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

void *nyxh_name(node *state,node *obj,node *block,node *parameters)
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

void *nyxh_sys(node *state,node *obj,node *block,node *parameters)
{
  node *value = create_sys_class_object();
  reset_obj_refcount(value);
  add_garbage(state,value);
  return(value);
}

void *nyxh_eval(node *state,node *obj,node *block,node *parameters)
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
  node *value = create_block_obj(base_class,nyx_block);
  set_obj_node(value,"anonymous_block_parent",block);
  node_SetParent(value,NULL); //needed ?
  reset_obj_refcount(value);
  add_garbage(state,value);
  node_FreeTree(nyx_block);
  return(value);
}

