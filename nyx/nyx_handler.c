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
  node *real_value = node_GetItemByKey(value,"value");
  if(node_GetType(real_value)==NODE_TYPE_SINT32)
    node_SetSint32(real_value,+node_GetSint32(real_value));
  else if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
    node_SetDouble(real_value,+node_GetDouble(real_value));
  return(value);
}

node *nyxh_pre_sub(node *state,node *obj,node *block,node *parameters)
{
  node *value = copy_class(obj);
  add_garbage(state,value);
  node *real_value = get_value(value);
  if(node_GetType(real_value)==NODE_TYPE_SINT32)
    node_SetSint32(real_value,-node_GetSint32(real_value));
  else if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
    node_SetDouble(real_value,-node_GetDouble(real_value));
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
  {
    //printf("negating :[%s] to %f\n",node_GetString(real_value),-atof(node_GetString(real_value)));
    node_SetDouble(real_value,-atof(node_GetString(real_value)));//TODO what about being a sint32
  }
  return(value);
}

node *nyxh_pre_not(node *state,node *obj,node *block,node *parameters)
{
  node *value = copy_class(obj);
  add_garbage(state,value);
  node *real_value = get_value(value);
  if(node_GetType(real_value)==NODE_TYPE_SINT32)
    node_SetSint32(real_value,!node_GetSint32(real_value));
  else if(node_GetType(real_value)==NODE_TYPE_SINT32)
    node_SetDouble(real_value,!node_GetDouble(real_value));
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
    node_SetSint32(real_value,!atol(node_GetString(real_value)));
  return(value);
}

node *nyxh_add(node *state,node *obj,node *block,node *parameters)
{
  node *value = copy_class(obj);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = get_value(value);
  node *real_value2 = get_value(value2);
  if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
  {
    if(node_GetType(real_value2)==NODE_TYPE_STRING)
    {
      char *num=str_FromDouble(node_GetDouble(real_value));
      char *cat_string=str_Cat(num,node_GetString(real_value2));
      node_SetString(real_value,cat_string);
      free(cat_string);
      free(num);
    }
    else
    {
      double d1,d2;
      d1=node_GetDouble(real_value);
      if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
        d2=node_GetDouble(real_value2);
      else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
        d2=(double)node_GetSint32(real_value2);
      node_SetDouble(real_value,d1+d2);
    }
  }
  else if(node_GetType(real_value)==NODE_TYPE_SINT32)
  {
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
    {
      double d1,d2;
      d1=(double)node_GetSint32(real_value);
      d2=node_GetDouble(real_value2);
      node_SetDouble(real_value,d1+d2);
    }
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    {
      node_SetSint32(real_value,node_GetSint32(real_value)+node_GetSint32(real_value2));
    }
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
    {
      char *num=str_FromLong(node_GetSint32(real_value));
      char *cat_string=str_Cat(num,node_GetString(real_value2));
      node_SetString(real_value,cat_string);
      free(cat_string);
      free(num);
    }
  }
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
  {
    if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    {
      char *num=str_FromLong(node_GetSint32(real_value2));
      char *cat_string=str_Cat(node_GetString(real_value),num);
      node_SetString(real_value,cat_string);
      free(cat_string);
      free(num);
    }
    else if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
    {
      char *num=str_FromDouble(node_GetDouble(real_value2));
      char *cat_string=str_Cat(node_GetString(real_value),num);
      node_SetString(real_value,cat_string);
      free(cat_string);
      free(num);
    }
    else
    {
      char *cat_string=str_Cat(node_GetString(real_value),node_GetString(real_value2));
      node_SetString(real_value,cat_string);
      free(cat_string);
    }
  }
  return(value);
}

/*node *nyxh_add(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = node_GetItemByKey(value,"value");
  node *real_value2 = node_GetItemByKey(value2,"value");
  printf("adding values together\n");
  printf("a:\n");
  node_PrintTree(real_value);
  printf("b:\n");
  node_PrintTree(real_value2);

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
    printf("appending int:[%s] + [%s] = [%s]\n",node_GetString(real_value),num,cat_string);
    node_SetString(real_value,cat_string);
    free(cat_string);
    free(num);
  }
  return(value);
}*/

node *nyxh_sub(node *state,node *obj,node *block,node *parameters)
{
  node *value = copy_class(obj);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = get_value(value);
  node *real_value2 = get_value(value2);

  if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
  {
    double d1,d2;
    d1=node_GetDouble(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      d2=node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      d2=(double)node_GetSint32(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      d2=(double)atof(node_GetString(real_value2));
    set_obj_float(value,"value",d1-d2);    
  }
  else if(node_GetType(real_value)==NODE_TYPE_SINT32)
  {
    long l1,l2;
    l1=node_GetSint32(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      l2=(long)node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      l2=node_GetSint32(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      l2=atol(node_GetString(real_value2));
    set_obj_int(value,"value",l1-l2);    
  } 
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
  {
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
    {
      set_obj_float(value,"value",atof(node_GetString(real_value))-node_GetSint32(real_value2));    
    }
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    {
      set_obj_int(value,"value",atol(node_GetString(real_value))-node_GetSint32(real_value2));    
    }
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      set_obj_int(value,"value",0);//TODO check if this is the best return value here
  }
  return(value);
}

node *nyxh_div(node *state,node *obj,node *block,node *parameters)
{
  node *value = copy_class(obj);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = get_value(value);
  node *real_value2 = get_value(value2);

  if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
  {
    double d1,d2;
    d1=node_GetDouble(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      d2=node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      d2=(double)node_GetSint32(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      d2=(double)atof(node_GetString(real_value2));
    set_obj_float(value,"value",d1/d2);    
  }
  else if(node_GetType(real_value)==NODE_TYPE_SINT32)
  {
    long l1,l2;
    l1=node_GetSint32(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      l2=(long)node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      l2=node_GetSint32(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      l2=atol(node_GetString(real_value2));
    set_obj_int(value,"value",l1/l2);    
  } 
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
  {
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
    {
      set_obj_float(value,"value",atof(node_GetString(real_value))/node_GetSint32(real_value2));    
    }
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    {
      set_obj_int(value,"value",atol(node_GetString(real_value))/node_GetSint32(real_value2));    
    }
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      set_obj_int(value,"value",0);//TODO check if this is the best return value here
  }
  return(value);
}

node *nyxh_mul(node *state,node *obj,node *block,node *parameters)
{
  node *value = copy_class(obj);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = get_value(value);
  node *real_value2 = get_value(value2);
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
  node *value = get_false_class(state);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = get_value(obj);
  node *real_value2 = get_value(value2);

  if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
  {
    double d1,d2;
    d1=node_GetDouble(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      d2=node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      d2=(double)node_GetSint32(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      d2=(double)atof(node_GetString(real_value2));

    if(d1<d2)
      value = get_true_class(state);
  }
  else if(node_GetType(real_value)==NODE_TYPE_SINT32)
  {
    long l1,l2;
    l1=node_GetSint32(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      l2=(long)node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      l2=node_GetSint32(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      l2=atol(node_GetString(real_value2));

    if(l1<l2)
      value = get_true_class(state);
  } 
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
  {
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
    {
      if(atof(node_GetString(real_value))<node_GetDouble(real_value2))
        value = get_true_class(state);
    }
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    {
      if(atol(node_GetString(real_value))<node_GetSint32(real_value2))
        value = get_true_class(state);
    }
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
    if(strlen(node_GetString(real_value))<strlen(node_GetString(real_value2)))
      value = get_true_class(state);
  }

  return(value);
}

node *nyxh_gt(node *state,node *obj,node *block,node *parameters)
{
  node *value = get_false_class(state);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = get_value(obj);
  node *real_value2 = get_value(value2);

  if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
  {
    double d1,d2;
    d1=node_GetDouble(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      d2=node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      d2=(double)node_GetSint32(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      d2=(double)atof(node_GetString(real_value2));

    if(d1>d2)
      value = get_true_class(state);
  }
  else if(node_GetType(real_value)==NODE_TYPE_SINT32)
  {
    long l1,l2;
    l1=node_GetSint32(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      l2=(long)node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      l2=node_GetSint32(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      l2=atol(node_GetString(real_value2));

    if(l1>l2)
      value = get_true_class(state);
  } 
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
  {
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
    {
      if(atof(node_GetString(real_value))>node_GetDouble(real_value2))
        value = get_true_class(state);
    }
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    {
      if(atol(node_GetString(real_value))>node_GetSint32(real_value2))
        value = get_true_class(state);
    }
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
    if(strlen(node_GetString(real_value))>strlen(node_GetString(real_value2)))
      value = get_true_class(state);
  }

  return(value);
}

node *nyxh_eq(node *state,node *obj,node *block,node *parameters)
{
  node *value = get_false_class(state);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = get_value(obj);
  node *real_value2 = get_value(value2);

  if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
  {
    double d1,d2;
    d1=node_GetDouble(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      d2=node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      d2=(double)node_GetSint32(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      d2=(double)atof(node_GetString(real_value2));

    if(d1==d2)
      value = get_true_class(state);
  }
  else if(node_GetType(real_value)==NODE_TYPE_SINT32)
  {
    long l1,l2;
    l1=node_GetSint32(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      l2=(long)node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      l2=node_GetSint32(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      l2=atol(node_GetString(real_value2));

    if(l1==l2)
      value = get_true_class(state);
  } 
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
  {
    char *s1=NULL;
    char *s2=NULL;
    int free_s2=0;
    s1=node_GetString(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
    {
      s2=str_FromDouble(node_GetDouble(real_value2));
      free_s2 = 1;
    }
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    {
      s2=str_FromLong(node_GetSint32(real_value2));
      free_s2 = 1;
    }
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      s2=node_GetString(real_value2);
    if(!strcmp(s1,s2))
      value = get_true_class(state);
    if(free_s2)
      free(s2);
  }

  return(value);
}

node *nyxh_lt_eq(node *state,node *obj,node *block,node *parameters)
{
  node *value = get_false_class(state);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = get_value(obj);
  node *real_value2 = get_value(value2);

  if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
  {
    double d1,d2;
    d1=node_GetDouble(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      d2=node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      d2=(double)node_GetSint32(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      d2=(double)atof(node_GetString(real_value2));

    if(d1<=d2)
      value = get_true_class(state);
  }
  else if(node_GetType(real_value)==NODE_TYPE_SINT32)
  {
    long l1,l2;
    l1=node_GetSint32(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      l2=(long)node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      l2=node_GetSint32(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      l2=atol(node_GetString(real_value2));

    if(l1<=l2)
      value = get_true_class(state);
  } 
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
  {
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
    {
      if(atof(node_GetString(real_value))<=node_GetDouble(real_value2))
        value = get_true_class(state);
    }
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    {
      if(atol(node_GetString(real_value))<=node_GetSint32(real_value2))
        value = get_true_class(state);
    }
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
    if(strlen(node_GetString(real_value))<=strlen(node_GetString(real_value2)))
      value = get_true_class(state);
  }

  return(value);
}

node *nyxh_gt_eq(node *state,node *obj,node *block,node *parameters)
{
  node *value = get_false_class(state);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = get_value(obj);
  node *real_value2 = get_value(value2);

  if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
  {
    double d1,d2;
    d1=node_GetDouble(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      d2=node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      d2=(double)node_GetSint32(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      d2=(double)atof(node_GetString(real_value2));

    if(d1>=d2)
      value = get_true_class(state);
  }
  else if(node_GetType(real_value)==NODE_TYPE_SINT32)
  {
    long l1,l2;
    l1=node_GetSint32(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      l2=(long)node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      l2=node_GetSint32(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      l2=atol(node_GetString(real_value2));

    if(l1>=l2)
      value = get_true_class(state);
  } 
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
  {
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
    {
      if(atof(node_GetString(real_value))>=node_GetDouble(real_value2))
        value = get_true_class(state);
    }
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    {
      if(atol(node_GetString(real_value))>=node_GetSint32(real_value2))
        value = get_true_class(state);
    }
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
    if(strlen(node_GetString(real_value))>=strlen(node_GetString(real_value2)))
      value = get_true_class(state);
  }

  return(value);
}

node *nyxh_neq(node *state,node *obj,node *block,node *parameters)
{
  node *value = get_false_class(state);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = get_value(obj);
  node *real_value2 = get_value(value2);

  if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
  {
    double d1,d2;
    d1=node_GetDouble(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      d2=node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      d2=(double)node_GetSint32(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      d2=(double)atof(node_GetString(real_value2));

    if(d1!=d2)
      value = get_true_class(state);
  }
  else if(node_GetType(real_value)==NODE_TYPE_SINT32)
  {
    long l1,l2;
    l1=node_GetSint32(real_value);
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
      l2=(long)node_GetDouble(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
      l2=node_GetSint32(real_value2);
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
      l2=atol(node_GetString(real_value2));

    if(l1!=l2)
      value = get_true_class(state);
  } 
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
  {
    if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
    {
      if(atof(node_GetString(real_value))!=node_GetDouble(real_value2))
        value = get_true_class(state);
    }
    else if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    {
      if(atol(node_GetString(real_value))!=node_GetSint32(real_value2))
        value = get_true_class(state);
    }
    else if(node_GetType(real_value2)==NODE_TYPE_STRING)
    if(strcmp(node_GetString(real_value),node_GetString(real_value2)))
      value = get_true_class(state);
  }

  return(value);
}


node *nyxh_copy(node *state,node *obj,node *block,node *parameters)
{
  node *value = copy_class(obj);
  add_garbage(state,value);
  return(value);
}


/*
// = handler
node *nyxh_assign(node *state,node *obj,node *block,node *parameters)
{
  node *obj_name = node_GetItemByKey(obj,"name");
  //printf("assigning:%s,%x with: %s,%x\n",get_obj_name(obj),obj,get_obj_name(node_GetItem(parameters,0)),node_GetItem(parameters,0));
  //fflush(stdout);
  node *parent = node_GetParent(obj);
  //if(parent)
  //{
  int r = node_RemoveItem(parent,obj);
  node_SetParent(obj,NULL);
  if(r==-1)
  {
    printf("error item not removed\n");
  }
  dec_obj_refcount(obj);
  //}
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
  if(item_index!=NULL) //TODO check if really working like expected
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
  //node *anon = node_GetItemByKey(value,"anonymous_block_parent");
  //if(anon!=NULL)
  //{
  //  node_RemoveItem(value,anon);
  //  node_FreeTree(anon);
  //}
  return(value);
}
*/

node *nyxh_assign(node *state,node *obj,node *block,node *parameters)
{
  node *obj_name = node_GetItemByKey(obj,"name");
  //printf("assigning:%s,%x with: %s,%x\n",get_obj_name(obj),obj,get_obj_name(node_GetItem(parameters,0)),node_GetItem(parameters,0));
  //fflush(stdout);
  node *root = node_GetRoot(obj);
  //printf("obj %x root: %x ,%s ,%d\n",obj,root,node_GetKey(root),node_GetType(root));


  
  if(root!=obj && !strcmp(node_GetKey(root),"nyx_object") && get_obj_refcount(root)==0)
  //if(root!=obj)
  {
    //printf("adding non bound obj %x to block %x\n",root,block);
    //node_PrintTree(root);
    //fflush(stdout);
    add_member(block,root);
    inc_obj_refcount(root);
  }
  

  node *parent = node_GetParent(obj);
  if(parent)
  {
    int r = node_RemoveItem(parent,obj);
    node_SetParent(obj,NULL);
    if(r==-1)
    {
      printf("error item not removed\n");
    }
  }
  dec_obj_refcount(obj);
  //}
  add_garbage(state,obj);

  node *item_index = node_GetItemByKey(obj,"item_index");

  node *value = node_CopyTree(node_GetItem(parameters,0),True,True);
  reset_obj_refcount(value);
  //if(item_index==NULL)
  set_obj_string(value,"name",node_GetString(obj_name));
  //else 
  //  printf("leaving name as it is:[%s]\n",);
  //node_AddItem(parent,value);
  //node *old_member
  //if(!strcmp(node_GetKey(root),"nyx_state"))
  //if(root!=obj)
  //{
    //printf("adding member %x to block %x\n",value,block);
    //add_member(block,value);
  if(parent)
  {
    //printf("adding %x to parent %x(pp:%x)\n",value,parent,node_GetParent(parent));
    //fflush(stdout);
    node_AddItem(parent,value);
    node_SetParent(value,parent);
    inc_obj_refcount(value);
  }
  //}
  else
  {
    //printf("adding %x to parent %x,",value,parent);
    //printf("adding %x to block %x\n",value,block);
    //fflush(stdout);
    //node_AddItem(parent,value);
    //add_member(obj,value);
    add_member(block,value);
    inc_obj_refcount(value);
  }
  //node_SetParent(value,parent);
  //printf("assigning:[%s]\n",node_GetString(obj_name));
  if(item_index!=NULL) //TODO check if really working like expected
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
  //node *anon = node_GetItemByKey(value,"anonymous_block_parent");
  //if(anon!=NULL)
  //{
  //  node_RemoveItem(value,anon);
  //  node_FreeTree(anon);
  //}
  return(value);
}

//println,print handler
node *nyxh_print(node *state,node *obj,node *block,node *parameters)
{
  node *value = get_true_class(state);
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
          node_Print(get_value(item),False,False);
          fflush(stdout);
        } 
      }
      else
      {
        node_Print(get_value(token),False,False);
        fflush(stdout);
      }
    }      
  }
  else
  {
      node_Print(get_value(obj),False,False);
  }
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
  node *value = get_false_class(state);
  long parent_value = node_GetSint32(get_value(obj));
  node *exe_block = node_GetItem(parameters,0);
  if(!parent_value)
  {
    execute_obj(state,exe_block,block,NULL,True,False);
    value = get_true_class(state);
  }
  return(value);
}

//do handler
node *nyxh_do(node *state,node *obj,node *block,node *parameters)
{
  node *value = get_false_class(state);
  long parent_value = node_GetSint32(get_value(obj));
  node *exe_block = node_GetItem(parameters,0);
  if(parent_value)
  {
    execute_obj(state,exe_block,block,NULL,True,False);
    value = get_true_class(state);
  }
  return(value);
}

node *nyxh_switch_name_value(node *state,node *obj,node *block,node *parameters)
{
  node *value = get_value(obj);
  if(node_GetType(value)!=NODE_TYPE_STRING)
    return(obj);
  node *switch_obj = node_GetItem(parameters,0);
  node *switch_obj_copy = copy_class(switch_obj);
  node *name = node_GetItemByKey(switch_obj_copy,"name");
  node_SetString(name,node_GetString(value));
  add_garbage(state,switch_obj_copy);
  return(switch_obj_copy);
}

node *nyxh_immediate_add(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(obj,"value");
  if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
  {
    double d1;
    d1=node_GetDouble(real_value);
    node_SetDouble(real_value,d1+1.0f);
  }
  else if(node_GetType(real_value)==NODE_TYPE_SINT32)
    node_SetSint32(real_value,node_GetSint32(real_value)+1);
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
  {
    node_SetSint32(real_value,atoi(node_GetString(real_value))+1);
  }
  return(value);
}

node *nyxh_immediate_sub(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_CopyTree(obj,True,True);
  node_SetParent(value,NULL);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(obj,"value");
  if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
  {
    double d1;
    d1=node_GetDouble(real_value);
    node_SetDouble(real_value,d1-1.0f);
  }
  else if(node_GetType(real_value)==NODE_TYPE_SINT32)
    node_SetSint32(real_value,node_GetSint32(real_value)-1);
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
  {
    node_SetSint32(real_value,atoi(node_GetString(real_value))+1);
  }
  return(value);
}

node *nyxh_pre_immediate_add(node *state,node *obj,node *block,node *parameters)
{
  node *real_value = node_GetItemByKey(obj,"value");
  if(node_GetType(real_value)==NODE_TYPE_SINT32)
    node_SetSint32(real_value,node_GetSint32(real_value)+1);
  else if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
    node_SetDouble(real_value,node_GetDouble(real_value)+1.0f);
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
  {
    node_SetSint32(real_value,atoi(node_GetString(real_value))+1);
  }
  return(obj);
}

node *nyxh_pre_immediate_sub(node *state,node *obj,node *block,node *parameters)
{
  node *real_value = node_GetItemByKey(obj,"value");
  if(node_GetType(real_value)==NODE_TYPE_SINT32)
    node_SetSint32(real_value,node_GetSint32(real_value)-1);
  else if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
    node_SetDouble(real_value,node_GetDouble(real_value)-1.0f);
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
  {
    node_SetSint32(real_value,atoi(node_GetString(real_value))-1);
  }
  return(obj);
}


//? handler
node *nyxh_cmp(node *state,node *obj,node *block,node *parameters)
{
  //?(expression,true_func,false_func,loop_while_true); 
  //execute based on expression value (0,1,..) ,loops if needed
  //returns(expression)

  node *value = NULL;

  node *expression_block = node_GetItem(parameters,0);
  node *true_block = node_GetItem(parameters,1);
  node *false_block = node_GetItem(parameters,2);
  node *loop_exe = node_GetItem(parameters,3);

  inc_obj_refcount(expression_block);
  inc_obj_refcount(true_block);
  inc_obj_refcount(false_block);

  node *loop = execute_obj(state,loop_exe,block,NULL,True,False);
  node *loop_value = get_value(loop);
  long lv = node_GetSint32(loop_value);
  if(lv)
  {
    node *exp_obj = execute_obj(state,expression_block,block,NULL,True,False);
    while(node_GetSint32(get_value(exp_obj)))
    {
      execute_obj(state,true_block,block,NULL,True,False);
      char *block_flag=check_block_flag(state);
      if(block_flag)
      {
        if(!strcmp(block_flag,"break")||!strcmp(block_flag,"exit")||!strcmp(block_flag,"return"))
        break;
      free(block_flag);
      }
      exp_obj = execute_obj(state,expression_block,block,NULL,True,False);
    }
    execute_obj(state,false_block,block,NULL,True,False);
    value = get_true_class(state);
  }
  else
  {
    node *exp_val=execute_obj(state,expression_block,block,NULL,True,False);
    if(node_GetSint32(get_value(exp_val)))
      value = get_true_class(state);
    else
      value = get_false_class(state);

    if(node_GetSint32(get_value(exp_val)))
    {
      execute_obj(state,true_block,block,NULL,True,False);
    }
    else
      execute_obj(state,false_block,block,NULL,True,False);
  }

  dec_obj_refcount(expression_block);
  dec_obj_refcount(true_block);
  dec_obj_refcount(false_block);
  add_garbage(state,expression_block);
  add_garbage(state,true_block);
  add_garbage(state,false_block);
  return(value);
}

// ?? handler
node *nyxh_init_cmp(node *state,node *obj,node *block,node *parameters)
{
  node *value = NULL;

  node *init_block = node_GetItem(parameters,0);
  node *expression_block = node_GetItem(parameters,1);
  node *true_block = node_GetItem(parameters,2);
  node *false_block = node_GetItem(parameters,3);
  node *loop_exe = node_GetItem(parameters,4);
  node *loop = execute_obj(state,loop_exe,block,NULL,True,False);
  inc_obj_refcount(init_block);
  inc_obj_refcount(expression_block);
  inc_obj_refcount(true_block);
  inc_obj_refcount(false_block);

  execute_obj(state,init_block,block,NULL,True,True);
  node *loop_value = get_value(loop);
  long lv = node_GetSint32(loop_value);
  if(lv)
  {
    node *exp_obj = execute_obj(state,expression_block,block,NULL,True,False);
    while(node_GetSint32(get_value(exp_obj)))
    {
      execute_obj(state,true_block,block,NULL,True,False);
      char *block_flag=check_block_flag(state);
      if(block_flag)
      {
        if(!strcmp(block_flag,"break")||!strcmp(block_flag,"exit")||!strcmp(block_flag,"return"))
          break;
        else if(!strcmp(block_flag,"restart"))
          execute_obj(state,init_block,block,NULL,True,False);
        free(block_flag);
      }
      exp_obj = execute_obj(state,expression_block,block,NULL,True,False);
    }
    execute_obj(state,false_block,block,NULL,True,False);
    value = get_true_class(state);
  }
  else
  {
    node *exp_val=execute_obj(state,expression_block,block,NULL,True,False);
    if(node_GetSint32(get_value(exp_val)))
      value = get_true_class(state);
    else
      value = get_false_class(state);
    if(node_GetSint32(get_value(exp_val)))
      execute_obj(state,true_block,block,NULL,True,False);
    else
      execute_obj(state,false_block,block,NULL,True,False);
  }

  dec_obj_refcount(init_block);
  dec_obj_refcount(expression_block);
  dec_obj_refcount(true_block);
  dec_obj_refcount(false_block);
  add_garbage(state,init_block);
  add_garbage(state,expression_block);
  add_garbage(state,true_block);
  add_garbage(state,false_block);

  return(value);
}

node *nyxh_break(node *state,node *obj,node *block,node *parameters)
{
  node *value = get_true_class(state);
  set_obj_string(state,"block_flag","break");
  if(node_GetItemsNum(parameters)>0)
  {
    node *count = node_GetItem(parameters,0);
    node *real_value = get_value(count);
    set_obj_int(state,"block_break_count",node_GetSint32(real_value));
  }
  else
  {
    set_obj_int(state,"block_break_count",1);
  }
  return(value);
}

node *nyxh_return(node *state,node *obj,node *block,node *parameters)//TODO
{
  node *value = NULL;
  set_obj_string(state,"block_flag","return");
  set_obj_int(state,"block_break_count",0);

  if(node_GetItemsNum(parameters)>0)
  {
    value = node_GetItem(parameters,0);
  }
  else
  {
    node *base_class = node_GetItemByKey(state,"nyx_object");
    node *value = create_class_instance(base_class);
    reset_obj_refcount(value);
    add_garbage(state,value);
  }
  return(value);
}

node *nyxh_item_at(node *state,node *obj,node *block,node *parameters)
{
  node *items = node_GetItemByKey(obj,"items");
  if(node_GetItemsNum(parameters)>0)
  {
    node *nindex = node_GetItem(parameters,0);
    node *real_nindex_value = get_value(nindex);
    long index = node_GetSint32(real_nindex_value);
    node *value = node_GetItem(items,index);
    return(value);
  }
  node *value = get_false_class(state);//TODO add get_null_class for a better differentiation
  return(value);
}

node *nyxh_in(node *state,node *obj,node *block,node *parameters)
{
  node *value = get_false_class(state);
  node *obj_value = get_value(obj);
  if(node_GetItemsNum(parameters)>0)
  {
    node *array = node_GetItem(parameters,0);
    node *items = node_GetItemByKey(array,"items");
    node_ItemIterationReset(items);
    while(node_ItemIterationUnfinished(items))
    {
      node *item = node_ItemIterate(items);
      node *item_value = get_value(item);
      //node *nitem_index = node_GetItemByKey(item,"item_index");
      //long item_index = node_GetSint32(nitem_index);
      if(node_GetType(obj_value)==NODE_TYPE_STRING && node_GetType(item_value)==NODE_TYPE_STRING)
      {
        if(!strcmp(node_GetString(item_value),node_GetString(obj_value)))
        {
          value = get_true_class(state);
          return(value);
        }
      } 
      else if(node_GetType(obj_value)==NODE_TYPE_SINT32 && node_GetType(item_value)==NODE_TYPE_SINT32)
      {
        if(node_GetSint32(item_value)==node_GetSint32(obj_value))
        {
          value = get_true_class(state);
          return(value);
        }
      }  
      else if(node_GetType(obj_value)==NODE_TYPE_DOUBLE && node_GetType(item_value)==NODE_TYPE_DOUBLE)
      {
        if(node_GetDouble(item_value)==node_GetDouble(obj_value))
        {
          value = get_true_class(state);
          return(value);
        }
      }  

    }
  }
  return(value);
}

node *nyxh_has(node *state,node *obj,node *block,node *parameters)
{
  node *value = get_false_class(state);
  if(node_GetItemsNum(parameters)>0)
  {
    node *key = node_GetItem(parameters,0);
    node *key_value = get_value(key);
    node *found = get_member(obj,node_GetString(key_value));
    if(found!=NULL)
    {
      value = get_true_class(state);
    }
  }
  return(value);
}

node *nyxh_in_keys(node *state,node *obj,node *block,node *parameters)
{
  node *value = get_false_class(state);
  if(node_GetItemsNum(parameters)>0)
  {
    node *array = node_GetItem(parameters,0);
    node *found = get_item(state,array,obj,False);
    if(found!=NULL)
    {
      value = get_true_class(state);
    }
  }
  return(value);
}

node *nyxh_name(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  set_obj_string(value,"value",get_obj_name(obj));
  return(value);
}

node *nyxh_restart(node *state,node *obj,node *block,node *parameters)
{
  node *value = get_true_class(state);
  set_obj_string(state,"block_flag","restart");
  if(node_GetItemsNum(parameters)>0)
  {
    node *count = node_GetItem(parameters,0);
    node *real_value = get_value(count);
    set_obj_int(state,"block_break_count",node_GetSint32(real_value));
  }
  else
  {
    set_obj_int(state,"block_break_count",1);
  }
  return(value);
}

node *nyxh_continue(node *state,node *obj,node *block,node *parameters)
{
  node *value=get_true_class(state);
  set_obj_string(state,"block_flag","continue");
  if(node_GetItemsNum(parameters)>0)
  {
    node *count = node_GetItem(parameters,0);
    node *real_value = get_value(count);
    set_obj_int(state,"block_break_count",node_GetSint32(real_value));
  }
  else
  {
    set_obj_int(state,"block_break_count",1);
  }
  return(value);
}

node *nyxh_index_of(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  node *real_value = get_value(value);
  add_garbage(state,value);
  long index = -1;
  if(node_GetItemsNum(parameters))
  {
    node *needle = node_GetItem(parameters,0);
    node *needle_value = get_value(needle);
    node *obj_value = get_value(obj);
    if(node_GetType(needle_value)==NODE_TYPE_STRING && node_GetType(obj_value) == NODE_TYPE_STRING)
    {
      char *cneedle = node_GetString(needle_value);
      char *chay = node_GetString(obj_value);
      char *pos = strstr(chay,cneedle);
      if(pos)
      {
        index = (long)(chay-pos);
      } 

    }
  }
  node_SetSint32(real_value,index);
  return(value);
}

node *nyxh_substr(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  set_obj_string(value,"value","");
  node *real_value = get_value(value);
  add_garbage(state,value);
  if(node_GetItemsNum(parameters))
  {
    node *index = node_GetItem(parameters,0);
    node *index_value = get_value(index);
    node *len = node_GetItem(parameters,1);
    node *len_value = NULL;
    long llen = -1;
    if(len && node_GetType(len_value)==NODE_TYPE_SINT32)
    {
      len_value = get_value(len);
      llen = node_GetSint32(len_value);
    }
    node *obj_value = get_value(obj);

    if(node_GetType(index_value)==NODE_TYPE_SINT32 && node_GetType(obj_value) == NODE_TYPE_STRING)
    {
      long lindex = node_GetSint32(index_value);
      char *chay = node_GetString(obj_value);
      char *sub = str_Sub(chay,lindex,llen);
      node_SetString(real_value,sub);
      free(sub);
    }
  }
  return(value);
}

node *nyxh_len(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = NULL;
  long len = 0;
  if(node_GetItemsNum(parameters))
    value = node_GetItem(parameters,0);
  else if(obj != block)
    value = obj;
  else
  {
    value = create_class_instance(base_class);
    add_garbage(state,value);
  }
  node *items = node_GetItemByKey(value,"items");
  node *real_value = get_value(value);
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
  real_value = get_value(value);
  node_SetSint32(real_value,len);
  add_garbage(state,value);
  return(value);
}

node *nyxh_input(node *state,node *obj,node *block,node *parameters)
{
  // a.input()
  // a=input()
  // input(a)
  node *base_class = get_base_class(state);
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
    add_garbage(state,value);
  }
  node *real_value = get_value(value);
  node_SetString(real_value,line);
  free(line);
  return(value);
}

node *nyxh_int(node *state,node *obj,node *block,node *parameters)
{
  //returns integer of input
  node *base_class = get_base_class(state);
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
    add_garbage(state,value2);
    set_obj_int(value2,"value",0);
  }
  node *real_value2 = get_value(value2);
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    node_SetSint32(real_value,node_GetSint32(real_value2));
  else if(node_GetType(real_value2)==NODE_TYPE_STRING)
    node_SetSint32(real_value,atoi(node_GetString(real_value2)));
  else if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
    node_SetSint32(real_value,(long)node_GetDouble(real_value2));
  return(value);
}

node *nyxh_float(node *state,node *obj,node *block,node *parameters)
{
  //returns float of input
  node *base_class = get_base_class(state);
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
    add_garbage(state,value2);
    set_obj_int(value2,"value",0);
  }
  node *real_value2 = get_value(value2);
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  node *real_value = get_value(value);
  if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    node_SetDouble(real_value,(double)node_GetSint32(real_value2));
  else if(node_GetType(real_value2)==NODE_TYPE_STRING)
    node_SetDouble(real_value,atof(node_GetString(real_value2)));
  else if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
    node_SetDouble(real_value,node_GetDouble(real_value2));
  return(value);
}

node *nyxh_str(node *state,node *obj,node *block,node *parameters)
{
  //returns string of integer or float input
  node *base_class = get_base_class(state);
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
    add_garbage(state,value2);
    set_obj_string(value2,"value","");
  }
  node *real_value2 = get_value(value2);
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  node *real_value = get_value(value);
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
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = get_value(obj);
  node *real_value2 = get_value(value2);
  set_obj_int(value,"value",0);
  long l1,l2;
  if(node_GetType(real_value)==NODE_TYPE_SINT32)
    l1=node_GetSint32(real_value);
  else if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
    l1=(long)node_GetDouble(real_value);
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
    l1=atol(node_GetString(real_value));

  if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    l2=node_GetSint32(real_value2);
  else if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
    l2=(long)node_GetDouble(real_value2);
  else if(node_GetType(real_value2)==NODE_TYPE_STRING)
    l2=atol(node_GetString(real_value2));
  node_SetSint32(get_value(value),l1&l2);
  return(value);
}

node *nyxh_or(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = get_value(obj);
  node *real_value2 = get_value(value2);
  set_obj_int(value,"value",0);
  long l1,l2;
  if(node_GetType(real_value)==NODE_TYPE_SINT32)
    l1=node_GetSint32(real_value);
  else if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
    l1=(long)node_GetDouble(real_value);
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
    l1=atol(node_GetString(real_value));

  if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    l2=node_GetSint32(real_value2);
  else if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
    l2=(long)node_GetDouble(real_value2);
  else if(node_GetType(real_value2)==NODE_TYPE_STRING)
    l2=atol(node_GetString(real_value2));
  node_SetSint32(get_value(value),l1|l2);
  return(value);
}

node *nyxh_binary_and(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = get_value(obj);
  node *real_value2 = get_value(value2);
  set_obj_int(value,"value",0);
  long l1,l2;
  if(node_GetType(real_value)==NODE_TYPE_SINT32)
    l1=node_GetSint32(real_value);
  else if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
    l1=(long)node_GetDouble(real_value);
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
    l1=atol(node_GetString(real_value));

  if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    l2=node_GetSint32(real_value2);
  else if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
    l2=(long)node_GetDouble(real_value2);
  else if(node_GetType(real_value2)==NODE_TYPE_STRING)
    l2=atol(node_GetString(real_value2));
  node_SetSint32(get_value(value),l1&l2);
  return(value);
}

node *nyxh_binary_or(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = get_value(obj);
  node *real_value2 = get_value(value2);
  set_obj_int(value,"value",0);
  long l1,l2;
  if(node_GetType(real_value)==NODE_TYPE_SINT32)
    l1=node_GetSint32(real_value);
  else if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
    l1=(long)node_GetDouble(real_value);
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
    l1=atol(node_GetString(real_value));

  if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    l2=node_GetSint32(real_value2);
  else if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
    l2=(long)node_GetDouble(real_value2);
  else if(node_GetType(real_value2)==NODE_TYPE_STRING)
    l2=atol(node_GetString(real_value2));
  node_SetSint32(get_value(value),l1|l2);
  return(value);
}

node *nyxh_binary_xor(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  node *value2 = node_GetItem(parameters,0);
  node *real_value = get_value(obj);
  node *real_value2 = get_value(value2);
  set_obj_int(value,"value",0);
  long l1,l2;
  if(node_GetType(real_value)==NODE_TYPE_SINT32)
    l1=node_GetSint32(real_value);
  else if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
    l1=(long)node_GetDouble(real_value);
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
    l1=atol(node_GetString(real_value));

  if(node_GetType(real_value2)==NODE_TYPE_SINT32)
    l2=node_GetSint32(real_value2);
  else if(node_GetType(real_value2)==NODE_TYPE_DOUBLE)
    l2=(long)node_GetDouble(real_value2);
  else if(node_GetType(real_value2)==NODE_TYPE_STRING)
    l2=atol(node_GetString(real_value2));
  node_SetSint32(get_value(value),l1^l2);
  return(value);
}

node *nyxh_pre_binary_not(node *state,node *obj,node *block,node *parameters)
{
  node *value = copy_class(obj);
  add_garbage(state,value);
  node *real_value = get_value(value);
  if(node_GetType(real_value)==NODE_TYPE_SINT32)
    node_SetSint32(real_value,~node_GetSint32(real_value));
  else if(node_GetType(real_value)==NODE_TYPE_DOUBLE)
    node_SetSint32(real_value,~(long)node_GetDouble(real_value));
  else if(node_GetType(real_value)==NODE_TYPE_STRING)
    node_SetSint32(real_value,~atol(node_GetString(real_value)));
  return(value);
}

node *nyxh_from_json(node *state,node *obj,node *block,node *parameters)
{
  //converts json string to objects
  node *base_class = get_base_class(state);
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
    add_garbage(state,value2);
    set_obj_string(value2,"value","");
  }
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  node *real_value2 = get_value(value2);
  convert_from_json(state,value,node_GetString(real_value2));
  return(value);
}

node *nyxh_split(node *state,node *obj,node *block,node *parameters)
{
  //splits string using splitting characters into a string array
  node *base_class = get_base_class(state);
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
    add_garbage(state,value2);
    set_obj_string(value2,"value","");
  }
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  node *items = create_obj("items");
  add_obj_kv(value,items);
  if(splitter==NULL)
    return(value);

  node *real_value2 = get_value(value2);
  node *splitter_value = get_value(splitter);
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
  node *base_class = get_base_class(state);
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
    add_garbage(state,value2);
    set_obj_string(value2,"value","");
  }
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  node *real_value = get_value(value);
  char *json = convert_to_json(state,value2);
  node_SetString(real_value,json);
  free(json);
  return(value);
}

node *nyxh_import(node *state,node *obj,node *block,node *parameters)//TODO
{
  node *base_class = get_base_class(state);
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
    add_garbage(state,value2);
    set_obj_string(value2,"value","");
  }
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  node *real_value2 = get_value(value2);
  convert_from_json(state,value,node_GetString(real_value2));
  return(value);
}

node *nyxh_dump_obj(node *state,node *obj,node *block,node *parameters)
{
  //dumps object as json
  node *base_class = get_base_class(state);
  node *value2 = NULL;
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  if(node_GetItemsNum(parameters))
  {
    value2 = node_GetItem(parameters,0);
  }
  else //if(obj != block)
  {
    value2 = obj;
  }
  node *real_value = get_value(value);
  char *json = convert_to_json(state,value2);//TODO add parameter to not filter any sub obj
  node_SetString(real_value,json);
  free(json);
  return(value);
}

node *nyxh_get_block(node *state,node *obj,node *block,node *parameters)
{
  return(block);
}

node *nyxh_parent(node *state,node *obj,node *block,node *parameters)
{
  node *value = get_false_class(state);//TODO change get_null_class
  node *mparent = node_GetParent(obj);
  if(mparent != NULL)
  {
    node *parent = node_GetParent(mparent);
    if(parent != NULL)
      return(parent);
  }
  return(value);
}

node *nyxh_eval(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
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
    value2 = create_class_instance(base_class);//TODO replace something more static without the need for gc
    set_obj_string(value2,"value","");
    add_garbage(state,value2);
  }
  node *real_value2 = get_value(value2);
  node *nyx_stream = nyx_LoadString(node_GetString(real_value2));
  node *nyx_block = node_GetItemByKey(nyx_stream,"nyx_block");
  node_RemoveItem(nyx_stream,nyx_block);
  node_FreeTree(nyx_stream);
  node *value = create_block_class_instance(base_class,nyx_block);
  set_obj_node(value,"anonymous_block_parent",block);
  add_garbage(state,value);
  node_FreeTree(nyx_block);
  return(value);
}

// : handler
node *nyxh_set_value_only(node *state,node *obj,node *block,node *parameters)
{
  node *value = node_GetItemByKey(obj,"value");
  node *value2 = node_GetItem(parameters,0);
  node_SetType(value,node_GetType(value2));
  node_SetValue(value,node_GetValue(value2),True,True);//TODO only works for simple value only, what about classes ?
  return(obj);
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

