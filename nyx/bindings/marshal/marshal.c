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

//node *marshal_call_function(node *state,node *self,node *obj,node *block,node *parameters); 



node *marshal_create_class_object(node *state)
{
  node *base = create_base_obj_layout("marshal");
  add_class_object_function(base,"=",nyxh_assign);
  add_class_object_function(base,"test",marshal_test);
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

typedef void (*func)();

#pragma GCC diagnostic pop


node *marshal_open_module(node *state,node *self,node *obj,node *block,node *parameters)
{

}

node *marshal_close_module(node *state,node *self,node *obj,node *block,node *parameters)
{

}

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

/*
node *marshal_call_function_old_float_overflow(node *state,node *self,node *obj,node *block,node *parameters)
{
  //printf("calling object: %x : %s\n",self,get_obj_name(self));
  //fflush(stdout);

  node *privates = node_GetItemByKey(self,"privates");
  node *symbol = node_GetItemByKey(privates,"marshal_sym");
 
  node *return_type_value = get_member(self,"return_type");
  long return_type = 0;
  if(return_type_value)
  {
    return_type = node_GetSint32(get_value(resolve_object(return_type_value)));
    printf("found return as double marker:%d\n",return_type);
  }

  void *sym = (void*)node_GetValue(symbol);
  //printf("sym handle:%x\n",sym);

  unsigned long long ret = 0;
  long doubles_index = 0;
  long pop_index = 0;


  unsigned long long pars_num = (unsigned long long)node_GetItemsNum(parameters);  
  long pindex = 0;
  char *is_double=(char*)malloc(pars_num);
  //is_double[0] = 0;
  //is_double[1] = 1;
  
  node_ItemIterationReset(parameters);
  while(node_ItemIterationUnfinished(parameters))
  {
    node *par = node_ItemIterate(parameters);
    node *par_value = get_value(par);
    if(node_GetType(par_value)==NODE_TYPE_DOUBLE)
      is_double[pindex] = 1;
    else
      is_double[pindex] = 0;
    pindex++;
  }
  
  __asm__ __volatile__(
    //"movq %%rbp,%%rsp\n\t"
    "movq %%rsp,%0\n\t"
    : "=r" (ret)
    : 
    : 
    );
  printf("args num:%d\n",pars_num);
  pindex = pars_num -1;
  node_ItemReverseIterationReset(parameters);
  while(node_ItemReverseIterationUnfinished(parameters))
  {
    node *par = node_ItemReverseIterate(parameters);
    //void *data = (void*)node_GetUser(get_value(par));
    node *par_value = get_value(par);
    //printf("pindex:%d\n",pindex);
    //int t = node_GetType(par_value);
    /*if(node_GetType(par_value)==NODE_TYPE_DOUBLE)
      is_double[pindex] = 1;
    else
      is_double[pindex] = 0;
    */
    /*if(node_GetType(par_value)==NODE_TYPE_DOUBLE)
    {
      //double *data = (double*)&(par_value->value);
      double data = node_GetDouble(par_value);
      //printf("adding:%f\n",data);
      __asm__ __volatile__(
        //"movsd %[data],%%xmm0\n\t"
        "movq %[data],%%rdi\n\t"
        "push %%rdi\n\t"
        : 
        : [ data ] "m" ( data )
        : "%rdi"
        );
    }
    else*/
/*    {
      unsigned long long data = node_GetUint64(par_value);
      __asm__ __volatile__(
        "movq %0,%%rdi\n\t"
        "push %%rdi\n\t"
        : 
        : "r" (data)
        : "%rdi"
        );
    }

    pindex--;
  }
  while(pop_index < pars_num)
  {

    //printf("pop_index:%d\n",pop_index);
    if(is_double[pop_index])
    {
      if(doubles_index==0)     
        __asm__ __volatile__("movsd (%%rsp), %%xmm0\n\t" :  :  : "xmm0","cc");
      if(doubles_index==1)     
        __asm__ __volatile__("movsd (%%rsp), %%xmm1\n\t" :  :  : "xmm1","cc");
      if(doubles_index==2)     
        __asm__ __volatile__("movsd (%%rsp), %%xmm2\n\t" :  :  : "xmm2","cc");
      if(doubles_index==3)     
        __asm__ __volatile__("movsd (%%rsp), %%xmm3\n\t" :  :  : "xmm3","cc");
      if(doubles_index==4)     
        __asm__ __volatile__("movsd (%%rsp), %%xmm4\n\t" :  :  : "xmm4","cc");
      if(doubles_index==5)     
        __asm__ __volatile__("movsd (%%rsp), %%xmm5\n\t" :  :  : "xmm5","cc");
      if(doubles_index==6)     
        __asm__ __volatile__("movsd (%%rsp), %%xmm6\n\t" :  :  : "xmm6","cc");
      if(doubles_index==7)     
        __asm__ __volatile__("movsd (%%rsp), %%xmm7\n\t" :  :  : "xmm7","cc");
      __asm__ __volatile__("popq %%rax\n\t" :  :  : "rax","cc");
      doubles_index++;
    }
    else
    {
    if(pop_index==0)
      __asm__ __volatile__("popq %%rdi\n\t" :  :  : "rdi","cc");
    if(pop_index==1)
    {
        //__asm__ __volatile__("movsd (%%rsp), %%xmm0\n\t" :  :  : "xmm0","cc");
      __asm__ __volatile__("popq %%rsi\n\t" :  :  : "rsi","cc");
    }
    if(pop_index==2)
      __asm__ __volatile__("popq %%rdx\n\t" :  :  : "rdx","cc");
    if(pop_index==3)
      __asm__ __volatile__("popq %%rcx\n\t" :  :  : "rcx","cc");
    if(pop_index==4)
      __asm__ __volatile__("popq %%r8\n\t" :  :  : "r8","cc");
    if(pop_index==5)
      __asm__ __volatile__("popq %%r9\n\t" :  :  : "r9","cc");
    }
    pop_index++;
  }
  /*
  if(pars_num>0)
  {
    __asm__ __volatile__("popq %%rdi\n\t" :  :  : );
  }
  if(pars_num>1)
  {
    __asm__ __volatile__("movsd (%%rsp), %%xmm0\n\t" :  :  : );
    __asm__ __volatile__("popq %%rsi\n\t" :  :  : );
  }
  if(pars_num>2)
    __asm__ __volatile__("popq %%rdx\n\t" :  :  : );
  if(pars_num>3)
    __asm__ __volatile__("popq %%rcx\n\t" :  :  : );
  if(pars_num>4)
    __asm__ __volatile__("popq %%r8\n\t" :  :  : );
  if(pars_num>5)
    __asm__ __volatile__("popq %%r9\n\t" :  :  : );
  */
  //double fl=14.0f;
/*  unsigned long long call_ret=0;
  __asm__ __volatile__(
    //"xorq %%rax,%%rax\n\r"
    //"movl $0,%%eax\n\t"
    //"movsd %[fl],%%xmm0\n\t"
    "movq %1,%%rax\n\t"
    "call %2\n\t"
    "movq %%rax,%0\n\t"
    : "=r" (call_ret)
    : "r" (pars_num), "r" (sym)//, [ fl ] "m" ( fl )
    :
    );
    //printf("call_ret:%u\n",call_ret);
    
    node *base_class = get_base_class(state);
    node *value = create_class_instance(base_class);
    set_obj_string(value,"name","marshal.return_value");
    add_garbage(state,value);
    node *real_value = get_value(value);
    node_SetUint64(real_value,call_ret);
    node_SetType(real_value,NODE_TYPE_SINT32);
    if(return_type == 1)
    {
      node_SetType(real_value,NODE_TYPE_DOUBLE);
      printf("returning double value:%f\n",node_GetDouble(real_value));
    }

  __asm__ __volatile__(
    "movq %0,%%rsp\n\t"
    : 
    : "r" (ret)
    : 
    );
  free(is_double);
  return(value);
}
*/

/*
node *marshal_call_function_pre_best(node *state,node *self,node *obj,node *block,node *parameters)
{
  //printf("calling object: %x : %s\n",self,get_obj_name(self));
  //fflush(stdout);
  unsigned long long pars_num = (unsigned long long)node_GetItemsNum(parameters);  
  node *privates = node_GetItemByKey(self,"privates");
  node *symbol = node_GetItemByKey(privates,"marshal_sym");
  node *return_type_value = get_member(self,"return_type");
  long return_type = 0;
  if(return_type_value)
  {
    return_type = node_GetSint32(get_value(resolve_object(return_type_value)));
    printf("found return as double marker:%d\n",return_type);
  }
  void *sym = (void*)node_GetValue(symbol);

  unsigned long long call_ret=0;
  unsigned long long data;

  unsigned long long ret = 0;
  long doubles_index = 0;
  long data_index = 0;
  long pop_index = 0;
  long pindex = 0;
  //long fpushes = 0;
  long doubles = 0;
  long doubles_pushes = 0;
  long non_doubles_pushes = 0;
  long non_doubles = 0;
  long pushes = 0;
  char *is_double=(char*)malloc(pars_num);
  unsigned long long *par_data=(unsigned long long*)malloc(sizeof(unsigned long long)*pars_num);
  
  node_ItemReverseIterationReset(parameters);
  while(node_ItemReverseIterationUnfinished(parameters))
  {
    node *par = node_ItemReverseIterate(parameters);
    node *par_value = get_value(par);
    unsigned long long data = node_GetUint64(par_value);
    if(node_GetType(par_value)==NODE_TYPE_DOUBLE)
    {
      is_double[pindex] = 1;
      doubles++;
    }
    else
      is_double[pindex] = 0;
    par_data[pindex] = data;
    pindex++;
  }

  non_doubles = pars_num - doubles;
  doubles_pushes = doubles - 8;
  non_doubles_pushes = non_doubles - 6;
  if(doubles_pushes<0)
    doubles_pushes = 0 ;  
  if(non_doubles_pushes<0)
    non_doubles_pushes = 0 ;  
  pushes = non_doubles_pushes + doubles_pushes;
  printf("args num:%d\n",pars_num);
  printf("doubles num:%d\n",doubles);
  printf("non doubles num:%d\n",non_doubles);
  printf("double_pushes:%d\n",doubles_pushes);
  printf("non double_pushes:%d\n",non_doubles_pushes);
  printf("pushes:%d\n",pushes);

  __asm__ __volatile__(
    "movq %%rsp,%0\n\t"
    : "=r" (ret)
    : 
    : 
    );
  printf("begin esp:%d\n",ret);
  pop_index = pars_num -1;
  while(pop_index>=0)
  //while(pop_index<pars_num)
  {
    data = par_data[pop_index];
    if(is_double[pop_index])
    {
      if(doubles_index==0)     
        __asm__ __volatile__("movsd %[data], %%xmm0\n\t" :  : [ data ] "m" ( data ) : "xmm0","cc");
      else if(doubles_index==1)     
        __asm__ __volatile__("movsd %[data], %%xmm1\n\t" :  : [ data ] "m" ( data ) : "xmm1","cc");
      else if(doubles_index==2)     
        __asm__ __volatile__("movsd %[data], %%xmm2\n\t" :  : [ data ] "m" ( data ) : "xmm2","cc");
      else if(doubles_index==3)     
        __asm__ __volatile__("movsd %[data], %%xmm3\n\t" :  : [ data ] "m" ( data ) : "xmm3","cc");
      else if(doubles_index==4)     
        __asm__ __volatile__("movsd %[data], %%xmm4\n\t" :  : [ data ] "m" ( data ) : "xmm4","cc");
      else if(doubles_index==5)     
        __asm__ __volatile__("movsd %[data], %%xmm5\n\t" :  : [ data ] "m" ( data ) : "xmm5","cc");
      else if(doubles_index==6)     
        __asm__ __volatile__("movsd %[data], %%xmm6\n\t" :  : [ data ] "m" ( data ) : "xmm6","cc");
      else if(doubles_index==7)     
        __asm__ __volatile__("movsd %[data], %%xmm7\n\t" :  : [ data ] "m" ( data ) : "xmm7","cc");
      else
      {
        //fpushes++;
        //__asm__ __volatile__("movq %[data],%%rax\n\t" 
        //  "pushq %%rax\n\t" :  : [ data ] "m" ( data ) : "%rsp","cc");
        __asm__ __volatile__("pushq %0\n\t" :  : "a" ( data ) : "rsp","cc");
      }
      doubles_index++;
    }
    else
    {
      if(data_index==0)
        __asm__ __volatile__("movq %[data],%%rdi\n\t" :  : [ data ] "m" ( data ) : "rdi","cc");
      else if(data_index==1)
        __asm__ __volatile__("movq %[data],%%rsi\n\t" :  : [ data ] "m" ( data ) : "rsi","cc");
      else if(data_index==2)
        __asm__ __volatile__("movq %[data],%%rdx\n\t" :  : [ data ] "m" ( data ) : "rdx","cc");
      else if(data_index==3)
        __asm__ __volatile__("movq %[data],%%rcx\n\t" :  : [ data ] "m" ( data ) : "rcx","cc");
      else if(data_index==4)
        __asm__ __volatile__("movq %[data],%%r8\n\t" :  : [ data ] "m" ( data ) : "r8","cc");
      else if(data_index==5)
        __asm__ __volatile__("movq %[data],%%r9\n\t" :  : [ data ] "m" ( data ) : "r9","cc");
      else
      {
        __asm__ __volatile__(
          //"movq %[data],%%rax\n\t" 
          "pushq %0\n\t" :  : "a" ( data ) : "rsp","cc");
      }
      data_index++;
    }
    pop_index--;
    //pop_index++;
  }
  unsigned long rets=0;
  __asm__ __volatile__(
    //"movq %1,%%rax\n\t"
    "movl $0, %%eax\n\t"
    "call %1\n\t"
    "movq %%rax,%0\n\t"
    : "=r" (call_ret)
    //: "r" (pars_num), "r" (sym)
    : "b" (sym)
    : "rdx","cc"
    );

  __asm__ __volatile__(
    "movq %0,%%rsp\n\t"
    : 
    : "r" (ret)
    : "rsp","cc"
    );
    printf("end esp:%d\n",ret);
    printf("call_ret:%u\n",call_ret);
    fflush(stdout);
    node *base_class = get_base_class(state);
    node *value = create_class_instance(base_class);
    set_obj_string(value,"name","marshal.return_value");
    add_garbage(state,value);
    node *real_value = get_value(value);
    node_SetUint64(real_value,call_ret);
    node_SetType(real_value,NODE_TYPE_SINT32);
    if(return_type == 1)
    {
      node_SetType(real_value,NODE_TYPE_DOUBLE);
      printf("returning double value:%f\n",node_GetDouble(real_value));
    }

  free(is_double);
  free(par_data);
  return(value);
}
*/

node *marshal_call_function(node *state,node *self,node *obj,node *block,node *parameters)
{
  unsigned long long pars_num = (unsigned long long)node_GetItemsNum(parameters);  
  node *privates = node_GetItemByKey(self,"privates");
  node *symbol = node_GetItemByKey(privates,"marshal_sym");
  node *return_type_value = get_member(self,"return_type");
  long return_type = 0;
  if(return_type_value)
    return_type = node_GetSint32(get_value(resolve_object(return_type_value)));
  void *sym = (void*)node_GetValue(symbol);

  unsigned long long call_ret=0;
  unsigned long long data;

  unsigned long long ret = 0;
  long doubles_index = 0;
  long data_index = 0;
  long pop_index = 0;
  long pindex = 0;
  long doubles = 0;
  long doubles_pushes = 0;
  long non_doubles_pushes = 0;
  long non_doubles = 0;
  long pushes = 0;
  char *is_double=(char*)malloc(pars_num);
  unsigned long long *par_data=(unsigned long long*)malloc(sizeof(unsigned long long)*pars_num);
  
  node_ItemReverseIterationReset(parameters);
  while(node_ItemReverseIterationUnfinished(parameters))
  {
    node *par = node_ItemReverseIterate(parameters);
    node *par_value = get_value(par);
    unsigned long long data = node_GetUint64(par_value);
    if(node_GetType(par_value)==NODE_TYPE_DOUBLE)
    {
      is_double[pindex] = 1;
      doubles++;
    }
    else
      is_double[pindex] = 0;
    par_data[pindex] = data;
    pindex++;
  }

  non_doubles = pars_num - doubles;
  doubles_pushes = doubles - 8;
  non_doubles_pushes = non_doubles - 6;//7;//6
  if(doubles_pushes<0)
    doubles_pushes = 0 ;  
  if(non_doubles_pushes<0)
    non_doubles_pushes = 0 ;  
  pushes = non_doubles_pushes + doubles_pushes;
  //printf("args num:%d\n",pars_num);
  //printf("doubles num:%d\n",doubles);
  //printf("non doubles num:%d\n",non_doubles);
  //printf("double_pushes:%d\n",doubles_pushes);
  //printf("non double_pushes:%d\n",non_doubles_pushes);
  //printf("pushes:%d\n",pushes);

  __asm__ __volatile__(
    "movq %%rsp,%0\n\t"
    : "=r" (ret)
    : 
    : 
    );
  //printf("begin esp:%d\n",ret);
  //pop_index = pars_num -1;
  //while(pop_index>=0)
  doubles_index = doubles-1;
  data_index = non_doubles-1;

  if(pushes%2==1)
    __asm__ __volatile__("subq $8,%%rsp\n\t" :  :  : "rsp","cc");

  while(pop_index<pars_num)
  {
    data = par_data[pop_index];
    if(is_double[pop_index])
    {
      if(doubles_index==0)     
        __asm__ __volatile__("movq %0, %%xmm0\n\t" :  : "a" ( data ) : "xmm0","cc");
      else if(doubles_index==1)     
        __asm__ __volatile__("movq %0, %%xmm1\n\t" :  : "a" ( data ) : "xmm1","cc");
      else if(doubles_index==2)     
        __asm__ __volatile__("movq %0, %%xmm2\n\t" :  : "a" ( data ) : "xmm2","cc");
      else if(doubles_index==3)     
        __asm__ __volatile__("movq %0, %%xmm3\n\t" :  : "a" ( data ) : "xmm3","cc");
      else if(doubles_index==4)     
        __asm__ __volatile__("movq %0, %%xmm4\n\t" :  : "a" ( data ) : "xmm4","cc");
      else if(doubles_index==5)     
        __asm__ __volatile__("movq %0, %%xmm5\n\t" :  : "a" ( data ) : "xmm5","cc");
      else if(doubles_index==6)     
        __asm__ __volatile__("movq %0, %%xmm6\n\t" :  : "a" ( data ) : "xmm6","cc");
      else if(doubles_index==7)     
        __asm__ __volatile__("movq %0, %%xmm7\n\t" :  : "a" ( data ) : "xmm7","cc");
      else
        __asm__ __volatile__("pushq %0\n\t" :  : "a" ( data ) : "rsp","cc");
      doubles_index--;
    }
    else
    {
      if(data_index==0)
        __asm__ __volatile__("movq %0,%%rdi\n\t" :  : "a" ( data ) : "rdi","cc");
      else if(data_index==1)
        __asm__ __volatile__("movq %0,%%rsi\n\t" :  : "a" ( data ) : "rsi","cc");
      else if(data_index==2)
        __asm__ __volatile__("movq %0,%%rdx\n\t" :  : "a" ( data ) : "rdx","cc");
      else if(data_index==3)
        __asm__ __volatile__("movq %0,%%rcx\n\t" :  : "a" ( data ) : "rcx","cc");
      else if(data_index==4)
        __asm__ __volatile__("movq %0,%%r8\n\t" :  : "a" ( data ) : "r8","cc");
      else if(data_index==5)
        __asm__ __volatile__("movq %0,%%r9\n\t" :  : "a" ( data ) : "r9","cc");
      //else if(data_index==6)
      //  __asm__ __volatile__("movq %0,%%r10\n\t" :  : "a" ( data ) : "r10","cc");
      else
        __asm__ __volatile__("pushq %0\n\t" :  : "a" ( data ) : "rsp","cc");
      data_index--;
    }
    pop_index++;
  }

  if(doubles>8)
    doubles = 8;


  __asm__ __volatile__(
    "movq %1, %%rax\n\t"
    "call %2\n\t"
    "movq %%rax,%0\n\t"
    : "=r" (call_ret)
    : "a" (doubles), "b" (sym)
    : "rdx","cc"
    );

  __asm__ __volatile__(
    "movq %0,%%rsp\n\t"
    : 
    : "r" (ret)
    : "rsp","cc"
    );
    node *base_class = get_base_class(state);
    node *value = create_class_instance(base_class);
    set_obj_string(value,"name","marshal.return_value");
    add_garbage(state,value);
    node *real_value = get_value(value);
    node_SetUint64(real_value,call_ret);
    node_SetType(real_value,NODE_TYPE_SINT32);
    if(return_type == 1)
      node_SetType(real_value,NODE_TYPE_DOUBLE);

  free(is_double);
  free(par_data);
  return(value);
}

/*
node *marshal_call_function2(node *state,node *self,node *obj,node *block,node *parameters)
{
  //printf("calling object: %x : %s\n",self,get_obj_name(self));
  //fflush(stdout);

  node *privates = node_GetItemByKey(self,"privates");
  node *symbol = node_GetItemByKey(privates,"marshal_sym");
 
  node *return_type_value = get_member(self,"return_type");
  long return_type = 0;
  if(return_type_value)
  {
    return_type = node_GetSint32(get_value(resolve_object(return_type_value)));
    printf("found return as double marker:%d\n",return_type);
  }

  void *sym = (void*)node_GetValue(symbol);
  //printf("sym handle:%x\n",sym);

  unsigned long long ret = 0;
  long doubles_index = 0;
  long pop_index = 0;

  unsigned long long call_ret=0;

  unsigned long long pars_num = (unsigned long long)node_GetItemsNum(parameters);  
  long pindex = 0;
  char *is_double=(char*)malloc(pars_num);
  //is_double[0] = 0;
  //is_double[1] = 1;
  
  node_ItemIterationReset(parameters);
  while(node_ItemIterationUnfinished(parameters))
  {
    node *par = node_ItemIterate(parameters);
    node *par_value = get_value(par);
    if(node_GetType(par_value)==NODE_TYPE_DOUBLE)
      is_double[pindex] = 1;
    else
      is_double[pindex] = 0;
    pindex++;
  }
  
  __asm__ __volatile__(
    //"movq %%rbp,%%rsp\n\t"
    "movq %%rsp,%0\n\t"
    : "=r" (ret)
    : 
    : "rsp","cc"
    );
  printf("args num:%d\n",pars_num);
  pindex = pars_num -1;
  node_ItemReverseIterationReset(parameters);
  while(node_ItemReverseIterationUnfinished(parameters))
  {
    node *par = node_ItemReverseIterate(parameters);
    //void *data = (void*)node_GetUser(get_value(par));
    node *par_value = get_value(par);
    //printf("pindex:%d\n",pindex);
    //int t = node_GetType(par_value);
    /*if(node_GetType(par_value)==NODE_TYPE_DOUBLE)
      is_double[pindex] = 1;
    else
      is_double[pindex] = 0;
    */
    /*if(node_GetType(par_value)==NODE_TYPE_DOUBLE)
    {
      //double *data = (double*)&(par_value->value);
      double data = node_GetDouble(par_value);
      //printf("adding:%f\n",data);
      __asm__ __volatile__(
        //"movsd %[data],%%xmm0\n\t"
        "movq %[data],%%rdi\n\t"
        "push %%rdi\n\t"
        : 
        : [ data ] "m" ( data )
        : "%rdi"
        );
    }
    else*/
    //{
/*      unsigned long long data = node_GetUint64(par_value);
      __asm__ __volatile__(
        "movq %0,%%rdi\n\t"
        "push %%rdi\n\t"
        : 
        : "r" (data)
        : "%rdi"
        );
    //}

    pindex--;
  }
  while(pop_index < pars_num)
  {

    //printf("pop_index:%d\n",pop_index);
    if(is_double[pop_index])
    {
      if(doubles_index==0)     
        __asm__ __volatile__("movsd (%%rsp), %%xmm0\n\t" :  :  : "xmm0","cc");
      if(doubles_index==1)     
        __asm__ __volatile__("movsd (%%rsp), %%xmm1\n\t" :  :  : "xmm1","cc");
      if(doubles_index==2)     
        __asm__ __volatile__("movsd (%%rsp), %%xmm2\n\t" :  :  : "xmm2","cc");
      if(doubles_index==3)     
        __asm__ __volatile__("movsd (%%rsp), %%xmm3\n\t" :  :  : "xmm3","cc");
      if(doubles_index==4)     
        __asm__ __volatile__("movsd (%%rsp), %%xmm4\n\t" :  :  : "xmm4","cc");
      if(doubles_index==5)     
        __asm__ __volatile__("movsd (%%rsp), %%xmm5\n\t" :  :  : "xmm5","cc");
      if(doubles_index==6)     
        __asm__ __volatile__("movsd (%%rsp), %%xmm6\n\t" :  :  : "xmm6","cc");
      if(doubles_index==7)     
        __asm__ __volatile__("movsd (%%rsp), %%xmm7\n\t" :  :  : "xmm7","cc");
      __asm__ __volatile__("popq %%rax\n\t" :  :  : "rax","cc");
      doubles_index++;
    }
    else
    {
    if(pop_index==0)
      __asm__ __volatile__("popq %%rdi\n\t" :  :  : "rdi","cc");
    if(pop_index==1)
      __asm__ __volatile__("popq %%rsi\n\t" :  :  : "rsi","cc");
    if(pop_index==2)
      __asm__ __volatile__("popq %%rdx\n\t" :  :  : "rdx","cc");
    if(pop_index==3)
      __asm__ __volatile__("popq %%rcx\n\t" :  :  : "rcx","cc");
    if(pop_index==4)
      __asm__ __volatile__("popq %%r8\n\t" :  :  : "r8","cc");
    if(pop_index==5)
      __asm__ __volatile__("popq %%r9\n\t" :  :  : "r9","cc");
    }
    pop_index++;
  }
  /*
  if(pars_num>0)
  {
    __asm__ __volatile__("popq %%rdi\n\t" :  :  : );
  }
  if(pars_num>1)
  {
    __asm__ __volatile__("movsd (%%rsp), %%xmm0\n\t" :  :  : );
    __asm__ __volatile__("popq %%rsi\n\t" :  :  : );
  }
  if(pars_num>2)
    __asm__ __volatile__("popq %%rdx\n\t" :  :  : );
  if(pars_num>3)
    __asm__ __volatile__("popq %%rcx\n\t" :  :  : );
  if(pars_num>4)
    __asm__ __volatile__("popq %%r8\n\t" :  :  : );
  if(pars_num>5)
    __asm__ __volatile__("popq %%r9\n\t" :  :  : );
  */
  //double fl=14.0f;
/*  __asm__ __volatile__(
    //"xorq %%rax,%%rax\n\r"
    //"movl $0,%%eax\n\t"
    //"movsd %[fl],%%xmm0\n\t"
    "movq %1,%%rax\n\t"
    "call %2\n\t"
    "movq %%rax,%0\n\t"
    : "=r" (call_ret)
    : "r" (pars_num), "r" (sym)//, [ fl ] "m" ( fl )
    :
    );
    //printf("call_ret:%u\n",call_ret);
    
    node *base_class = get_base_class(state);
    node *value = create_class_instance(base_class);
    set_obj_string(value,"name","marshal.return_value");
    add_garbage(state,value);
    node *real_value = get_value(value);
    node_SetUint64(real_value,call_ret);
    node_SetType(real_value,NODE_TYPE_SINT32);
    if(return_type == 1)
    {
      node_SetType(real_value,NODE_TYPE_DOUBLE);
      printf("returning double value:%f\n",node_GetDouble(real_value));
    }

  __asm__ __volatile__(
    "movq %0,%%rsp\n\t"
    : 
    : "r" (ret)
    : "rsp","cc"
    );
  free(is_double);
  return(value);
}
*/
/*
node *marshal_call_function_old34(node *state,node *self,node *obj,node *block,node *parameters)
{
  //printf("calling object: %x : %s\n",self,get_obj_name(self));
  //fflush(stdout);
  unsigned long long pars_num = (unsigned long long)node_GetItemsNum(parameters);  
  node *privates = node_GetItemByKey(self,"privates");
  node *symbol = node_GetItemByKey(privates,"marshal_sym");
  node *return_type_value = get_member(self,"return_type");
  long return_type = 0;
  if(return_type_value)
  {
    return_type = node_GetSint32(get_value(resolve_object(return_type_value)));
    //printf("found return as double marker:%d\n",return_type);
  }
  void *sym = (void*)node_GetValue(symbol);


  unsigned long long data;
  unsigned long long call_ret=0;
  unsigned long long stack_pointer = 0;

  long doubles_num = 0;
  long registers_num = 0;
  long stacks_num = 0;
  long index = 0;

  unsigned long long *par_stack_data=(unsigned long long*)malloc(sizeof(unsigned long long)*pars_num);
  unsigned long long *par_register_data=(unsigned long long*)malloc(sizeof(unsigned long long)*pars_num);
  unsigned long long *par_double_data=(unsigned long long*)malloc(sizeof(unsigned long long)*pars_num);
  
  node_ItemIterationReset(parameters);
  while(node_ItemIterationUnfinished(parameters))
  {
    node *par = node_ItemIterate(parameters);
    node *par_value = get_value(par);
    unsigned long long data = node_GetUint64(par_value);
    if(node_GetType(par_value)==NODE_TYPE_DOUBLE)
    {
      if(doubles_num<=8)
      {
        par_double_data[doubles_num] = data;
        doubles_num++;
      }
      else
      {
        par_stack_data[stacks_num] = data;
        stacks_num++;
      }
    }
    else
    {
      if(registers_num<=6)
      {
        //printf("reg:%d[%x : %d]\n",registers_num,data,data);
        par_register_data[registers_num] = data;
        registers_num++;
      }
      else
      {
        par_stack_data[stacks_num] = data;
        stacks_num++;
      }
    }
  }

  //printf("args num:%d\n",pars_num);
  //printf("doubles num:%d\n",doubles_num);
  //printf("registers num:%d\n",registers_num);
  //printf("stack pushes:%d\n",stacks_num);

  __asm__ __volatile__(
    "movq %%rsp,%0\n\t"
    : "=r" (stack_pointer)
    : 
    : 
    );

  if(stacks_num)
  {
    index = stacks_num -1;
    while(index>=0)
      __asm__ __volatile__("pushq %0\n\t" :  : "r" ( par_stack_data[index--] ) : "rsp","cc");
  }

  if(doubles_num>0)
    __asm__ __volatile__("movsd %[data], %%xmm0\n\t" :  : [ data ] "m" ( par_double_data[0] ) : "xmm0","cc");
  else if(doubles_num>1)
    __asm__ __volatile__("movsd %[data], %%xmm1\n\t" :  : [ data ] "m" ( par_double_data[1] ) : "xmm1","cc");
  else if(doubles_num>2)
    __asm__ __volatile__("movsd %[data], %%xmm2\n\t" :  : [ data ] "m" ( par_double_data[2] ) : "xmm2","cc");
  else if(doubles_num>3)
    __asm__ __volatile__("movsd %[data], %%xmm3\n\t" :  : [ data ] "m" ( par_double_data[3] ) : "xmm3","cc");
  else if(doubles_num>4)
    __asm__ __volatile__("movsd %[data], %%xmm4\n\t" :  : [ data ] "m" ( par_double_data[4] ) : "xmm4","cc");
  else if(doubles_num>5)
    __asm__ __volatile__("movsd %[data], %%xmm5\n\t" :  : [ data ] "m" ( par_double_data[5] ) : "xmm5","cc");
  else if(doubles_num>6)
    __asm__ __volatile__("movsd %[data], %%xmm6\n\t" :  : [ data ] "m" ( par_double_data[6] ) : "xmm6","cc");
  else if(doubles_num>7)
    __asm__ __volatile__("movsd %[data], %%xmm7\n\t" :  : [ data ] "m" ( par_double_data[7] ) : "xmm7","cc");


  if(registers_num>0)
    __asm__ __volatile__("movq %[data],%%rdi\n\t" :  : [ data ] "m" ( par_register_data[0] ) : "rdi","cc");
  else if(registers_num>1)
    __asm__ __volatile__("movq %[data],%%rsi\n\t" :  : [ data ] "m" ( par_register_data[1] ) : "rsi","cc");
  else if(registers_num>2)
    __asm__ __volatile__("movq %[data],%%rdx\n\t" :  : [ data ] "m" ( par_register_data[2] ) : "rdx","cc");
  else if(registers_num>3)
    __asm__ __volatile__("movq %[data],%%rcx\n\t" :  : [ data ] "m" ( par_register_data[3] ) : "rcx","cc");
  else if(registers_num>4)
    __asm__ __volatile__("movq %[data],%%r8\n\t" :  : [ data ] "m" ( par_register_data[4] ) : "r8","cc");
  else if(registers_num>5)
    __asm__ __volatile__("movq %[data],%%r9\n\t" :  : [ data ] "m" ( par_register_data[5] ) : "r9","cc");


  __asm__ __volatile__(
    "movq %1,%%rax\n\t"
    "call %2\n\t"
    "movq %%rax,%0\n\t"
    : "=r" (call_ret)
    : "r" (pars_num), "r" (sym)
    : "rax","cc"
    );
  __asm__ __volatile__(
    "movq %0,%%rsp\n\t"
    : 
    : "r" (stack_pointer)
    : "rsp","cc"
    );
    //printf("end esp:%d\n",stack_pointer);
    //printf("call_ret:%u\n",call_ret);
    //fflush(stdout);
    node *base_class = get_base_class(state);
    node *value = create_class_instance(base_class);
    set_obj_string(value,"name","marshal.return_value");
    add_garbage(state,value);
    node *real_value = get_value(value);
    node_SetUint64(real_value,call_ret);
    node_SetType(real_value,NODE_TYPE_SINT32);
    if(return_type == 1)
    {
      node_SetType(real_value,NODE_TYPE_DOUBLE);
      //printf("returning double value:%f\n",node_GetDouble(real_value));
    }
  free(par_stack_data);
  free(par_register_data);
  free(par_double_data);
  return(value);
}
*/
/*
node *marshal_call_function(node *state,node *self,node *obj,node *block,node *parameters)
{
  //printf("calling object: %x : %s\n",self,get_obj_name(self));
  //fflush(stdout);
  unsigned long long pars_num = (unsigned long long)node_GetItemsNum(parameters);  
  node *privates = node_GetItemByKey(self,"privates");
  node *symbol = node_GetItemByKey(privates,"marshal_sym");
  node *return_type_value = get_member(self,"return_type");
  long return_type = 0;
  if(return_type_value)
  {
    return_type = node_GetSint32(get_value(resolve_object(return_type_value)));
    printf("found return as double marker:%d\n",return_type);
  }
  void *sym = (void*)node_GetValue(symbol);


  unsigned long long ret = 0;
  long doubles_index = 0;
  long data_index = 0;
  long pop_index = 0;
  long pindex = 0;
  //long fpushes = 0;
  long doubles = 0;
  long doubles_pushes = 0;
  long non_doubles_pushes = 0;
  long non_doubles = 0;
  long pushes = 0;
  char *is_double=(char*)malloc(pars_num);
  unsigned long long *par_data=(unsigned long long*)malloc(sizeof(unsigned long long)*pars_num);
  
  node_ItemReverseIterationReset(parameters);
  while(node_ItemReverseIterationUnfinished(parameters))
  {
    node *par = node_ItemReverseIterate(parameters);
    node *par_value = get_value(par);
    unsigned long long data = node_GetUint64(par_value);
    if(node_GetType(par_value)==NODE_TYPE_DOUBLE)
    {
      is_double[pindex] = 1;
      doubles++;
    }
    else
      is_double[pindex] = 0;
    par_data[pindex] = data;
    pindex++;
  }

  non_doubles = pars_num - doubles;
  doubles_pushes = doubles - 8;
  non_doubles_pushes = non_doubles - 6;
  if(doubles_pushes<0)
    doubles_pushes = 0 ;  
  if(non_doubles_pushes<0)
    non_doubles_pushes = 0 ;  
  pushes = non_doubles_pushes + doubles_pushes;
  printf("args num:%d\n",pars_num);
  printf("doubles num:%d\n",doubles);
  printf("non doubles num:%d\n",non_doubles);
  printf("double_pushes:%d\n",doubles_pushes);
  printf("non double_pushes:%d\n",non_doubles_pushes);
  printf("pushes:%d\n",pushes);

  __asm__ __volatile__(
    "movq %%rsp,%0\n\t"
    : "=r" (ret)
    : 
    : 
    );
  printf("begin esp:%d\n",ret);
  pop_index = pars_num -1 -(pars_num-pushes);
  while(pop_index>=0)
  {
    unsigned long long data = par_data[pop_index];
    __asm__ __volatile__("pushq %0\n\t" :  : "r" ( data ) : "%rsp","cc");
    pop_index--;
  }

  doubles_index++;
    if(is_double[pop_index])
    {
      if(doubles_index==0)     
        __asm__ __volatile__("movsd %[data], %%xmm0\n\t" :  : [ data ] "m" ( data ) : "xmm0","cc");
      else if(doubles_index==1)     
        __asm__ __volatile__("movsd %[data], %%xmm1\n\t" :  : [ data ] "m" ( data ) : "xmm1","cc");
      else if(doubles_index==2)     
        __asm__ __volatile__("movsd %[data], %%xmm2\n\t" :  : [ data ] "m" ( data ) : "xmm2","cc");
      else if(doubles_index==3)     
        __asm__ __volatile__("movsd %[data], %%xmm3\n\t" :  : [ data ] "m" ( data ) : "xmm3","cc");
      else if(doubles_index==4)     
        __asm__ __volatile__("movsd %[data], %%xmm4\n\t" :  : [ data ] "m" ( data ) : "xmm4","cc");
      else if(doubles_index==5)     
        __asm__ __volatile__("movsd %[data], %%xmm5\n\t" :  : [ data ] "m" ( data ) : "xmm5","cc");
      else if(doubles_index==6)     
        __asm__ __volatile__("movsd %[data], %%xmm6\n\t" :  : [ data ] "m" ( data ) : "xmm6","cc");
      else if(doubles_index==7)     
        __asm__ __volatile__("movsd %[data], %%xmm7\n\t" :  : [ data ] "m" ( data ) : "xmm7","cc");
      else
      {
    else
    {
      if(data_index==0)
        __asm__ __volatile__("movq %[data],%%rdi\n\t" :  : [ data ] "m" ( data ) : "rdi","cc");
      else if(data_index==1)
        __asm__ __volatile__("movq %[data],%%rsi\n\t" :  : [ data ] "m" ( data ) : "rsi","cc");
      else if(data_index==2)
        __asm__ __volatile__("movq %[data],%%rdx\n\t" :  : [ data ] "m" ( data ) : "rdx","cc");
      else if(data_index==3)
        __asm__ __volatile__("movq %[data],%%rcx\n\t" :  : [ data ] "m" ( data ) : "rcx","cc");
      else if(data_index==4)
        __asm__ __volatile__("movq %[data],%%r8\n\t" :  : [ data ] "m" ( data ) : "r8","cc");
      else if(data_index==5)
        __asm__ __volatile__("movq %[data],%%r9\n\t" :  : [ data ] "m" ( data ) : "r9","cc");
      else
      {
        __asm__ __volatile__("movq %[data],%%rax\n\t" 
          "pushq %%rax\n\t" :  : [ data ] "m" ( data ) : "rsp","cc");
      }
      data_index++;
    }
    //pop_index++;
  }
  unsigned long long call_ret=0;
  
  __asm__ __volatile__(
    "movq %1,%%rax\n\t"
    "call %2\n\t"
    "movq %%rax,%0\n\t"
    : "=r" (call_ret)
    : "r" (pars_num), "r" (sym)
    : "rax","cc"
    );
  
  __asm__ __volatile__(
    "movq %0,%%rsp\n\t"
    : 
    : "r" (ret)
    : "rsp","cc"
    );
    printf("end esp:%d\n",ret);
    printf("call_ret:%u\n",call_ret);
    fflush(stdout);
    node *base_class = get_base_class(state);
    node *value = create_class_instance(base_class);
    set_obj_string(value,"name","marshal.return_value");
    add_garbage(state,value);
    node *real_value = get_value(value);
    node_SetUint64(real_value,call_ret);
    node_SetType(real_value,NODE_TYPE_SINT32);
    if(return_type == 1)
    {
      node_SetType(real_value,NODE_TYPE_DOUBLE);
      printf("returning double value:%f\n",node_GetDouble(real_value));
    }

  free(is_double);
  free(par_data);
  return(value);
}
*/


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


node *marshal_test(node *state,node *self,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  set_obj_string(value,"name","marshal.test");
  add_garbage(state,value);
  node *real_value = get_value(value);
  printf("marshalling test\n");

  void *lib = dlopen("libc.so.6",RTLD_LAZY);
  if(lib==NULL)
  {
    printf("library not found\n");
  } 
  else 
  {
    printf("checking for printf call\n");
    void *sym = dlsym(lib,"printf");
    if(sym==NULL)
    {
      printf("no printf function found\n");
    }
    else
    {
      printf("trying printf call\n");
      func init = (func)sym;
      //init("printf[%s]\n","hello world");
      char *format = "printf[%s]\n";
      char *hello = "hello world";
      /*init(format,hello);*/
      /*__asm__("pushq %%rax\n\t" :"=r"(hello)); 
      __asm__("pushq %%rax\n\t" :"=r"(format)); 
      __asm__("movq $2, %rax\n\t");
          __asm__("call printf\n\t"); 
      __asm__("popq %rbx\n\t"); 
      __asm__("popq %rbx\n\t"); */
      long ret = 0;
        //"call printf\n\t"
      __asm__ __volatile__(
        "movq %2,%%rdi\n\t"
        "movq %1,%%rsi\n\t"
        "movl $0,%%eax\n\t"
        "call %3\n\t"
        "movq %0,%%rax\n\t"
        : "=r" (ret)
        : "r" (hello),"r" (format), "r" (sym)
        :
      );
      printf("call finished\n");
      fflush(stdout);
      /*
      __asm__("movq %[hello],%rax\n\t"
          "pushq %rax\n\t"
          "movq  %[format],%rax\n\t"
          "pushq %rax\n\t"
          "call printf\n\t"
          "popq  %rbx\n\t"
          "popq  %rbx");

      */
    }
    dlclose(lib);
  }

  return(value);
}



#pragma GCC diagnostic pop


#endif

