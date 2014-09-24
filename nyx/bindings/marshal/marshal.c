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


node *marshal_create_class_object(void)
{
  node *base = create_base_obj_layout("http");
  add_class_object_function(base,"=",nyxh_assign);
  add_class_object_function(base,"test",marshal_test);
  return(base);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
 
void marshal_binding_open(node *state)
{
  node *modules = get_modules(state);
  node *block_class = get_block_class(state);
  node *marshal = marshal_bind(modules);
  node *proxy = create_proxy_object(state,marshal,"marshal");
  inc_obj_refcount(marshal);
  add_member(block_class,proxy);
  inc_obj_refcount(proxy);
}

void marshal_binding_close(node *state)
{

}

node *marshal_bind(node *class)
{
  node *marshal = marshal_create_class_object();
  add_member(class,marshal);
  inc_obj_refcount(marshal);
  return(marshal);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

typedef void (*func)();

#pragma GCC diagnostic pop


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

