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

node *marshal_call_function2(node *state,node *self,node *obj,node *block,node *parameters)
{
  unsigned long long pars_num = (unsigned long long)node_GetItemsNum(parameters);  
  node *privates = node_GetItemByKey(self,"privates");
  node *symbol = node_GetItemByKey(privates,"marshal_sym");
  node *return_type_value = get_member(self,"return_type");
  long return_type = 0;
  if(return_type_value)
    return_type = node_GetSint32(get_value(resolve_object(return_type_value)));
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
      if(doubles_num<8)
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
      if(registers_num<6)
      {
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
  if(doubles_num>1)
    __asm__ __volatile__("movsd %[data], %%xmm1\n\t" :  : [ data ] "m" ( par_double_data[1] ) : "xmm1","cc");
  if(doubles_num>2)
    __asm__ __volatile__("movsd %[data], %%xmm2\n\t" :  : [ data ] "m" ( par_double_data[2] ) : "xmm2","cc");
  if(doubles_num>3)
    __asm__ __volatile__("movsd %[data], %%xmm3\n\t" :  : [ data ] "m" ( par_double_data[3] ) : "xmm3","cc");
  if(doubles_num>4)
    __asm__ __volatile__("movsd %[data], %%xmm4\n\t" :  : [ data ] "m" ( par_double_data[4] ) : "xmm4","cc");
  if(doubles_num>5)
    __asm__ __volatile__("movsd %[data], %%xmm5\n\t" :  : [ data ] "m" ( par_double_data[5] ) : "xmm5","cc");
  if(doubles_num>6)
    __asm__ __volatile__("movsd %[data], %%xmm6\n\t" :  : [ data ] "m" ( par_double_data[6] ) : "xmm6","cc");
  if(doubles_num>7)
    __asm__ __volatile__("movsd %[data], %%xmm7\n\t" :  : [ data ] "m" ( par_double_data[7] ) : "xmm7","cc");


  if(registers_num>0)
    __asm__ __volatile__("movq %0,%%rdi\n\t" :  : "a" ( par_register_data[0] ) : "rdi","cc");
  if(registers_num>1)
    __asm__ __volatile__("movq %0,%%rsi\n\t" :  : "a" ( par_register_data[1] ) : "rsi","cc");
  if(registers_num>2)
    __asm__ __volatile__("movq %0,%%rdx\n\t" :  : "a" ( par_register_data[2] ) : "rdx","cc");
  if(registers_num>3)
    __asm__ __volatile__("movq %0,%%rcx\n\t" :  : "a" ( par_register_data[3] ) : "rcx","cc");
  if(registers_num>4)
    __asm__ __volatile__("movq %0,%%r8\n\t" :  : "a" ( par_register_data[4] ) : "r8","cc");
  if(registers_num>5)
    __asm__ __volatile__("movq %0,%%r9\n\t" :  : "a" ( par_register_data[5] ) : "r9","cc");
  /*
  if(registers_num>0)
    __asm__ __volatile__("movq %[data],%%rdi\n\t" :  : [ data ] "m" ( par_register_data[0] ) : "rdi","cc");
  if(registers_num>1)
    __asm__ __volatile__("movq %[data],%%rsi\n\t" :  : [ data ] "m" ( par_register_data[1] ) : "rsi","cc");
  if(registers_num>2)
    __asm__ __volatile__("movq %[data],%%rdx\n\t" :  : [ data ] "m" ( par_register_data[2] ) : "rdx","cc");
  if(registers_num>3)
    __asm__ __volatile__("movq %[data],%%rcx\n\t" :  : [ data ] "m" ( par_register_data[3] ) : "rcx","cc");
  if(registers_num>4)
    __asm__ __volatile__("movq %[data],%%r8\n\t" :  : [ data ] "m" ( par_register_data[4] ) : "r8","cc");
  if(registers_num>5)
    __asm__ __volatile__("movq %[data],%%r9\n\t" :  : [ data ] "m" ( par_register_data[5] ) : "r9","cc");
*/

  __asm__ __volatile__(
    //"movq %1,%%rax\n\t"
    "call %1\n\t"
    "movq %%rax,%0\n\t"
    : "=r" (call_ret)
    : "r" (sym)
    //: "r" (pars_num), "r" (sym)
    : "rax","cc"
    );
  __asm__ __volatile__(
    "movq %0,%%rsp\n\t"
    : 
    : "r" (stack_pointer)
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
  free(par_stack_data);
  free(par_register_data);
  free(par_double_data);
  return(value);
}
