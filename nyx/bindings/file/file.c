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

void file_bind(node *class)
{
  node *file = file_create_class_object();
  add_member(class,file);
  inc_obj_refcount(file);
}

node *file_create_class_object(void)
{
  node *base = create_base_obj_layout("file");
  add_class_object_function(base,"=",nyxh_assign);
  add_class_object_function(base,"readall",file_readall);
  add_class_object_function(base,"writeall",file_writeall);
  add_class_object_function(base,"open",file_open);
  add_class_object_function(base,"close",file_close);
  return(base);
}

node *file_open(node *state,node *obj,node *block,node *parameters)
{
  //returns io stream object
  node *filename = NULL;
  node *mode = NULL;
  node *value = file_create_class_object();
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

node *file_close(node *state,node *obj,node *block,node *parameters)
{
  //close a file
  node *value = get_false_class(state);
  //printf("closing file\n");
  //node_PrintTree(obj);
  node *handle = node_GetItemByKey(obj,"file_handle");
  if(handle!=NULL)
  {
    FILE *fhandle = node_GetValue(handle);
    int ret = fclose(fhandle);
    if(ret)
      value = get_true_class(state);
    //node_SetNull(handle);
  }
  if(strcmp(get_obj_name(obj),"file"))
  {
    //printf("chained return\n");
    return(obj);
  }
  //printf("unchained return\n");
  return(value);
}

node *file_readall(node *state,node *obj,node *block,node *parameters)
{
  //returns string with all file content
  node *base_class = get_base_class(state);
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
      value = get_true_class(state);
    }
    //}
  }
  return(value);
}


#endif

