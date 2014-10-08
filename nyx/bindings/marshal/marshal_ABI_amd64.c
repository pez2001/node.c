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

  __asm__ __volatile__(
    "movq %%rsp,%0\n\t"
    : "=r" (ret)
    : 
    : 
    );
  doubles_index = doubles-1;
  data_index = non_doubles-1;

  if(pushes%2==1)
    __asm__ __volatile__("subq $8,%%rsp\n\t" :  :  : "rsp","cc");

  //remember to change rdx -> rbx
  // rcx also
  //remove some unneeded opcodes in final assembly 
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
