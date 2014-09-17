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

#include "file.h"

#ifdef USE_HTTP


node *file_create_class_object(node *base_class)
{
  //node *base = create_base_obj_layout("file");
  node *base = create_class_instance(base_class);
  set_obj_string(base,"name","file");
  //add_class_object_function(base,"=",nyxh_assign);
  add_class_object_function(base,"readallbytes",file_readallbytes);
  add_class_object_function(base,"writeallbytes",file_writeallbytes);
  add_class_object_function(base,"readall",file_readall);
  add_class_object_function(base,"writeall",file_writeall);
  add_class_object_function(base,"open",file_open);
  add_class_object_function(base,"close",file_close);
  return(base);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void file_binding_open(node *state)
{
  node *modules = get_modules(state);
  node *base_class = get_base_class(state);
  node *block_class = get_block_class(state);
  node *file = file_bind(base_class,modules);
  node *proxy = create_proxy_object(state,file,"file");
  inc_obj_refcount(file);
  add_member(block_class,proxy);
  inc_obj_refcount(proxy);
}

void file_binding_close(node *state)
{

}

node *file_bind(node *base_class,node *class)
{
  node *file = file_create_class_object(base_class);
  add_member(class,file);
  inc_obj_refcount(file);
  return(file);
}

node *file_open(node *state,node *obj,node *block,node *parameters)
{
  //returns io stream object
  node *filename = NULL;
  node *mode = NULL;
  node *base_class = get_base_class(state);
  node *value = file_create_class_object(base_class);
  add_garbage(state,value);
  if(node_GetItemsNum(parameters))
  {
    filename = node_GetItem(parameters,0);
    mode = node_GetItem(parameters,1);
    node *real_filename = get_value(filename);
    node *real_mode = get_value(mode);
    if(node_GetType(real_filename)==NODE_TYPE_STRING)
    {
      node *privates = node_GetItemByKey(value,"privates");
      char *fname = node_GetString(real_filename);
      char *m = node_GetString(real_mode);
      FILE *f = fopen(fname,m);
      set_obj_ptr(privates,"file_handle",f);
    }
  }
  return(value);
}

node *file_close(node *state,node *obj,node *block,node *parameters)
{
  //close a file
  node *value = get_false_class(state);
  //printf("closing file\n");
  //node_PrintTree(obj);
  node *privates = node_GetItemByKey(obj,"privates");
  node *handle = node_GetItemByKey(privates,"file_handle");
  if(handle!=NULL)
  {
    FILE *fhandle = (FILE*)(unsigned long)node_GetValue(handle);
    //int ret = fclose(fhandle);
    fclose(fhandle);
    //if(ret)
    return(obj);
      //value = get_true_class(state);
    //node_SetNull(handle);
  }
  /*if(strcmp(get_obj_name(obj),"file"))
  {
    //printf("chained return\n");
    return(obj);
  }*/
  //printf("unchained return\n");
  return(value);
}

node *file_writeallbytes(node *state,node *obj,node *block,node *parameters)
{
  //writes binary blob to file 
  node *value = get_false_class(state);
  node *value2 = NULL;
  if(node_GetItemsNum(parameters))
  {
    value2 = node_GetItem(parameters,0);
    node *real_value = node_GetItemByKey(value2,"value");
    node *privates = node_GetItemByKey(obj,"privates");
    node *handle = node_GetItemByKey(privates,"file_handle");
    if(handle!=NULL)
    {
      FILE *fhandle = (FILE*)(unsigned long)node_GetValue(handle);
      void *content = node_GetBinary(real_value);
      unsigned long len = node_GetBinaryLength(real_value);
      fwrite(content,len,1,fhandle);
      node *value_privates = node_GetItemByKey(value,"privates");
      set_obj_ptr(value_privates,"file_handle",fhandle);
      value = get_true_class(state);
    }
  }
  return(value);
}

node *file_readallbytes(node *state,node *obj,node *block,node *parameters)
{
  //returns binary blob with all file content
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  node *privates = node_GetItemByKey(obj,"privates");
  node *handle = node_GetItemByKey(privates,"file_handle");
  if(handle!=NULL)
  {
    char *ret = NULL;
    FILE *fhandle = (FILE*)(unsigned long)node_GetValue(handle);
    fseek(fhandle,0,SEEK_END);
    long len = ftell(fhandle);
    fseek(fhandle,0,SEEK_SET);
    ret = (char*)malloc(len);
    fread(ret,len,1,fhandle);
    node_SetBinary(real_value,ret,len);
    node *value_privates = node_GetItemByKey(value,"privates");
    set_obj_ptr(value_privates,"file_handle",fhandle);
    add_class_object_function(value,"close",file_close);
    //free(ret);
  }
  return(value);
}

node *file_readall(node *state,node *obj,node *block,node *parameters)
{
  //returns string with all file content
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  //reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = get_value(value);
  node *privates = node_GetItemByKey(obj,"privates");
  node *handle = node_GetItemByKey(privates,"file_handle");
  if(handle!=NULL)
  {
    char *ret = NULL;
    FILE *fhandle = (FILE*)(unsigned long)node_GetValue(handle);
    fseek(fhandle,0,SEEK_END);
    long len = ftell(fhandle);
    fseek(fhandle,0,SEEK_SET);
    ret = (char*)malloc(len+1);
    memset(ret+len,0,1);
    fread(ret,len,1,fhandle);
    node_SetString(real_value,ret);
    node *value_privates = node_GetItemByKey(value,"privates");
    set_obj_ptr(value_privates,"file_handle",fhandle);
    add_class_object_function(value,"close",file_close);
    free(ret);
  }
  return(value);
}

node *file_writeall(node *state,node *obj,node *block,node *parameters)
{
  //writes string to file 
  node *value = get_false_class(state);
  node *value2 = NULL;
  if(node_GetItemsNum(parameters))
  {
    value2 = node_GetItem(parameters,0);
    node *real_value = node_GetItemByKey(value2,"value");
    node *privates = node_GetItemByKey(obj,"privates");
    node *handle = node_GetItemByKey(privates,"file_handle");
    if(handle!=NULL)
    {
      FILE *fhandle = (FILE*)(unsigned long)node_GetValue(handle);
      char *content = node_GetString(real_value);
      long len = strlen(content);
      fwrite(content,len,1,fhandle);
      node *value_privates = node_GetItemByKey(value,"privates");
      set_obj_ptr(value_privates,"file_handle",fhandle);
      value = get_true_class(state);
    }
  }
  return(value);
}

#pragma GCC diagnostic pop

#endif

