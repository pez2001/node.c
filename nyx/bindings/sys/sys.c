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

#include "sys.h"

#ifdef USE_SYS

void sys_bind(node *base_class,node *class)
{
  node *sys = sys_create_class_object(base_class);
  add_member(class,sys);
  inc_obj_refcount(sys);
  srand(time(NULL));
}

void add_module(node *base_class,node *items,char *module_name)
{
  node *module = create_class_instance(base_class);
  inc_obj_refcount(module);
  set_obj_string(module,"name","module");
  set_obj_string(module,"value",module_name);
  set_obj_int(module,"item_index",get_last_index(items)+1);
  //set_obj_int(module,"item_index",get_items_new_index(items));
  
  node_AddItem(items,module);
  node_SetParent(module,items);
}

void sys_add_parameter(node *state,node *parameter)
{
  node *base_class = get_base_class(state);
  node *block_class = get_block_class(state);
  node *sys = get_member(block_class,"sys");
  node *parameters = get_member(sys,"parameters");
  node *items = node_GetItemByKey(parameters,"items");
  reset_obj_refcount(parameter);
  inc_obj_refcount(parameter);
  set_obj_int(parameter,"item_index",get_last_index(items)+1);
  
  node_AddItem(items,parameter);
  node_SetParent(parameter,items);
}



node *sys_create_class_object(node *base_class)
{
  //node *base_class = get_base_class(state);
  node *base = create_base_obj_layout("sys");
  add_class_object_function(base,"=",nyxh_assign);

  add_class_object_function(base,"name",sys_name);
  add_class_object_function(base,"working_directory",sys_working_directory); //contains name/path and files as sub items
  add_class_object_function(base,"change_working_directory",sys_change_working_directory);
  add_class_object_function(base,"dump",sys_dump);
  add_class_object_function(base,"time",sys_time);
  add_class_object_function(base,"random",sys_random);
  add_class_object_function(base,"execute",sys_execute);
  add_class_object_function(base,"exit",sys_exit);
  add_class_object_function(base,"sleep",sys_sleep);
  add_class_object_function(base,"script_filename",sys_script_filename);//if applicable
  add_class_object_function(base,"interpreter_filename",sys_interpreter_filename);
  add_class_object_function(base,"interpreter_version",sys_interpreter_version);//returns array with major/minor/build


  node *parameters = create_class_instance(base_class);
  inc_obj_refcount(parameters);
  set_obj_string(parameters,"name","parameters");
  add_member(base,parameters);
  node *parameters_items = create_obj("items");
  add_obj_kv(parameters,parameters_items);


  node *modules = create_class_instance(base_class);
  inc_obj_refcount(modules);
  set_obj_string(modules,"name","modules");
  add_member(base,modules);
  node *modules_items = create_obj("items");
  add_obj_kv(modules,modules_items);
  //long items_index = 0;
  #ifdef USE_SOCKETS
  add_module(base_class,modules_items,"sockets");
  #endif
  #ifdef USE_HTTP
  add_module(base_class,modules_items,"http");
  #endif
  #ifdef USE_CURL
  add_module(base_class,modules_items,"curl");
  #endif
  #ifdef USE_WEBSOCKETS
  add_module(base_class,modules_items,"websockets");
  #endif
  #ifdef USE_MICROHTTPD
  add_module(base_class,modules_items,"microhttpd");
  #endif
  #ifdef USE_CURSES
  add_module(base_class,modules_items,"curses");
  #endif
  //add_class_object_function(base,"load",nyxh_load);
  //add_class_object_function(base,"reset",nyxh_reset);
  //add_class_object_function(base,"remove",nyxh_remove);//remove file/directory
  //add_class_object_function(base,"gc_collect",nyxh_gc_collect);
  //add_class_object_function(base,"error",nyxh_error);//print to stderr
  //add_class_object_function(base,"obj_kv",nyxh_obj_kv);//returns an kv pair in an object as valued new base class instance
  return(base);
}

node *sys_time(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  node_SetSint32(real_value,(long)time(NULL));
  return(value);
}

unsigned long rand_unbiased(unsigned long max)
{
  unsigned long r;
  unsigned long min;
  if(max<2)
    return(0);
  min = -max % max;
  while(1)
  {
    r = rand();
    if(r>=min)
      break;
  }
  return(r%max);
}


node *sys_random(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  unsigned long max = ULONG_MAX;
  if(node_GetItemsNum(parameters))
  {
    node *nmax = node_GetItem(parameters,0);
    node *real_max = get_value(nmax);
    max = node_GetSint32(real_max);
  }
  node *real_value = get_value(value);
  node_SetSint32(real_value,(long)rand_unbiased(max));
  return(value);
}


node *sys_name(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  //struct utsname uname_data;
  //uname(&uname_data);
  //node_SetString(real_value,uname_data.sysname);
  node_SetString(real_value,"not supported");
  return(value);
}

node *sys(node *state,node *obj,node *block,node *parameters)
{
  node *value = sys_create_class_object(state);
  add_garbage(state,value);
  return(value);
}

node *sys_execute(node *state,node *obj,node *block,node *parameters)
{
  //execute an external program and returns the output
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  node *real_value = get_value(value);
  if(node_GetItemsNum(parameters))
  {
    node *command = node_GetItem(parameters,0);
    node *real_command = get_value(command);
    if(node_GetType(real_command)==NODE_TYPE_STRING)
    {
      char *cc = node_GetString(real_command);
      FILE *f = (FILE*)popen(cc,"rb");
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

node *sys_change_working_directory(node *state,node *obj,node *block,node *parameters)
{
  node *value = get_true_class(state);
  if(node_GetItemsNum(parameters))
  {
    node *dir = node_GetItem(parameters,0);
    node *real_dir = node_GetItemByKey(dir,"value");
    char *c=node_GetString(real_dir);
    chdir(c);//TODO check if cd was successful
  }
  return(value);
}

node *sys_working_directory(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  node *items = create_obj("items");
  add_obj_kv(value,items);
  node *path = create_class_instance(base_class);
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

node *sys_interpreter_version(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  set_obj_string(value,"name","nyx_version");
  node *items = create_obj("items");
  add_obj_kv(value,items);
  node *major = create_class_instance(base_class);
  inc_obj_refcount(major);
  set_obj_string(major,"name","major");
  set_obj_int(major,"value",(long) MAJOR_VERSION);
  set_obj_int(major,"item_index",0);
  node_AddItem(items,major);
  node *minor = create_class_instance(base_class);
  inc_obj_refcount(minor);
  set_obj_string(minor,"name","minor");
  set_obj_int(minor,"value",(long) MINOR_VERSION);
  set_obj_int(minor,"item_index",1);
  node_AddItem(items,minor);
  node *build = create_class_instance(base_class);
  inc_obj_refcount(build);
  set_obj_string(build,"name","build");
  set_obj_int(build,"value",(long) BUILD+1);
  set_obj_int(build,"item_index",2);
  node_AddItem(items,build);
  return(value);
}

node *sys_interpreter_filename(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *interpreter_filename = node_GetItemByKey(state,"interpreter_filename");
  node *value = create_class_instance(base_class);
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

node *sys_script_filename(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *script_filename = node_GetItemByKey(state,"script_filename");
  node *value = create_class_instance(base_class);
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

node *sys_exit(node *state,node *obj,node *block,node *parameters)
{
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
  node *value = get_true_class(state);
  return(value);
}

node *sys_dump(node *state,node *obj,node *block,node *parameters)
{
  //dumps the whole interpreter state as json
  node *base_class = get_base_class(state);
  //node *value = create_class_instance(base_class);
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
    add_garbage(state,value);
  }
  node *real_value = node_GetItemByKey(value,"value");
  char *json = state_to_json(state);
  node_SetString(real_value,json);
  free(json);
  return(value);
}

node *sys_sleep(node *state,node *obj,node *block,node *parameters)
{
  node *value = get_true_class(state);
  node *time_par = node_GetItem(parameters,0);
  node *time_val = node_GetItemByKey(time_par,"value");
  long ms = node_GetSint32(time_val);
  Sleep(ms);
  return(value);
}




#endif

