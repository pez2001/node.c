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

#include "marshal.h"

#ifdef USE_MARSHAL


node *marshal_create_class_object(node *state)
{
  node *base = create_base_obj_layout("marshal");
  add_class_object_function(base,"=",nyxh_assign);
  add_class_object_function(base,"open_module",marshal_open_module);
  add_class_object_function(base,"close_module",marshal_close_module);
  add_class_object_function(base,"open_lib",marshal_open_library);
  add_class_object_function(base,"close_lib",marshal_close_library);
  add_class_object_function(base,"load_function",marshal_load_function);

  node *base_class = get_base_class(state);
  node *return_type_double = create_class_instance(base_class);
  inc_obj_refcount(return_type_double);
  set_obj_string(return_type_double,"name","DOUBLE");
  set_obj_int(return_type_double,"value",1);
  add_member(base,return_type_double);

  node *return_type_string = create_class_instance(base_class);
  inc_obj_refcount(return_type_string);
  set_obj_string(return_type_string,"name","STRING");
  set_obj_int(return_type_string,"value",2);
  add_member(base,return_type_string);

  node *return_type_sint32 = create_class_instance(base_class);
  inc_obj_refcount(return_type_sint32);
  set_obj_string(return_type_sint32,"name","SINT32");
  set_obj_int(return_type_sint32,"value",0);
  add_member(base,return_type_sint32);
  return(base);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
 
void marshal_binding_open(node *state)
{
  node *modules = get_modules(state);
  node *block_class = get_block_class(state);
  node *marshal = marshal_bind(state,modules);
  node *proxy = create_proxy_object(state,marshal,"marshal");
  inc_obj_refcount(marshal);
  add_member(block_class,proxy);
  inc_obj_refcount(proxy);
  sys_add_module(state,"marshal");
}

void marshal_binding_close(node *state)
{

}

node *marshal_bind(node *state,node *class)
{
  node *marshal = marshal_create_class_object(state);
  add_member(class,marshal);
  inc_obj_refcount(marshal);
  return(marshal);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"



typedef node *(*func)(node *state);

node *marshal_open_module(node *state,node *self,node *obj,node *block,node *parameters)
{
  node *olibrary_name = node_GetItem(parameters,0);
  node *library_name = get_value(olibrary_name);

  node *value = get_false_class(state);
  void *lib = dlopen(node_GetString(library_name),RTLD_LAZY);
  if(lib==NULL)
  {
    printf("module not loaded:%s (%s)\n",node_GetString(library_name),dlerror());
  } 
  else
  {
    //now get module_open symbol and call it
     void *sym = dlsym(lib,"module_open");
    if(sym==NULL)
    {
      printf("no module_open function found\n");
      node *base_class = get_base_class(state);
      value = create_class_instance(base_class);
      set_obj_string(value,"name","marshal.lib_handle");
      add_garbage(state,value);
      set_obj_ptr(value,"value",lib);
    }
    else
    {
      func init = (func)sym;
      value=init(state);
      set_obj_ptr(value,"value",lib);
      add_garbage(state,value);
    }
  }
  return(value);    
}

node *marshal_close_module(node *state,node *self,node *obj,node *block,node *parameters)
{
  node *library = node_GetItem(parameters,0);
  void *lib = (void*)node_GetValue(get_value(library));
  if(lib)
  {
     void *sym = dlsym(lib,"module_close");
    if(sym==NULL)
    {
      printf("no module_close function found\n");
    }
    else
    {
      func _close = (func)sym;
      _close(state);
    }
    dlclose(lib);
    return(get_true_class(state));
  }
  else 
    return(get_false_class(state));
}

#pragma GCC diagnostic pop

node *marshal_open_library(node *state,node *self,node *obj,node *block,node *parameters)
{
  node *olibrary_name = node_GetItem(parameters,0);
  node *library_name = get_value(olibrary_name);

  node *value = NULL;
  void *lib = dlopen(node_GetString(library_name),RTLD_LAZY);
  if(lib==NULL)
  {
    value = get_false_class(state);
  } 
  else
  {
    node *base_class = get_base_class(state);
    value = create_class_instance(base_class);
    set_obj_string(value,"name","marshal.lib_handle");
    add_garbage(state,value);
    set_obj_ptr(value,"value",lib);
  }
  return(value);    
}

node *marshal_close_library(node *state,node *self,node *obj,node *block,node *parameters)
{
  node *library = node_GetItem(parameters,0);
  void *lib = (void*)node_GetValue(get_value(library));
  dlclose(lib);
  return(get_true_class(state));
}

node *marshal_load_function(node *state,node *self,node *obj,node *block,node *parameters)
{
  node *library = node_GetItem(parameters,0);
  node *ofunc_name = node_GetItem(parameters,1);
  node *func_name = get_value(ofunc_name);
  void *lib = (void*)node_GetValue(get_value(library));
  void *sym = dlsym(lib,node_GetString(func_name));
  if(sym==NULL)
  {
    printf("function %s not found\n",node_GetString(func_name));
    return(get_false_class(state));
  }
  else
  {
    node *base_class = get_base_class(state);
    node *method = create_class_instance(base_class);
    set_obj_string(method,"name",node_GetString(func_name));
    set_obj_string(method,"type","function");
    node *privates = node_GetItemByKey(method,"privates");
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
      set_obj_ptr(privates,"handler",(void*)marshal_call_function);//TODO move to privates
    #pragma GCC diagnostic pop
    set_obj_ptr(privates,"marshal_sym",sym);//TODO move to privates
    add_garbage(state,method);
    return(method);
  }
}

#pragma GCC diagnostic pop

#endif

