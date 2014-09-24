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

#include "curl.h"

#ifdef USE_CURL




node *curl_create_class_object(void)
{
  node *base = create_base_obj_layout("curl");
  add_class_object_function(base,"=",nyxh_assign);
  add_class_object_function(base,"get",curl_get);
  return(base);
}

static size_t curl_write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  node *state = node_GetItem((node*)stream,0);
  node *read_block = node_GetItem((node*)stream,1);
  node *block = node_GetItem((node*)stream,2);
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  node *real_value = get_value(value);
  char *ret = (char*)malloc((size*nmemb)+1);
  memset(ret+(size*nmemb) + 0, 0, 1);
  memcpy(ret,ptr,(size*nmemb));
  node_SetString(real_value,ret);
  free(ret);
  set_obj_string(value,"name","data");
  node *parameters = create_obj("parameters");
  node_AddItem(parameters,value);
  //node *read_obj = 
  execute_obj(state,read_block,block,parameters,True,False,True);
  //execute_obj(state,read_block,block,parameters,True,True,True);
  return((size*nmemb));
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void curl_binding_open(node *state)
{
  node *modules = get_modules(state);
  node *block_class = get_block_class(state);
  node *curl = curl_bind(modules);
  node *proxy = create_proxy_object(state,curl,"curl");
  inc_obj_refcount(curl);
  add_member(block_class,proxy);
  inc_obj_refcount(proxy);
}

void curl_binding_close(node *state)
{

}


node *curl_bind(node *class)
{
  node *curl = curl_create_class_object();
  add_member(class,curl);
  inc_obj_refcount(curl);
  return(curl);
}

node *curl_get(node *state,node *self,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  set_obj_string(value,"name","http.answer");
  add_garbage(state,value);
  if(node_GetItemsNum(parameters))
  {
    node *nurl = node_GetItem(parameters,0);
    node *read_block = node_GetItem(parameters,1);

    node *url_value=get_value(nurl);
    CURL *curl_handle;
    node *curl_state = create_obj("curl_state");
    node_AddItem(curl_state,state);
    node_AddItem(curl_state,read_block);
    node_AddItem(curl_state,block);
    inc_obj_refcount(read_block);
    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle,CURLOPT_URL,node_GetString(url_value));
    curl_easy_setopt(curl_handle,CURLOPT_NOPROGRESS,1);
    curl_easy_setopt(curl_handle,CURLOPT_HEADERFUNCTION,curl_write_data);
    curl_easy_setopt(curl_handle,CURLOPT_WRITEFUNCTION,curl_write_data);
    curl_easy_setopt(curl_handle,CURLOPT_HEADERDATA,curl_state);
    curl_easy_setopt(curl_handle,CURLOPT_WRITEDATA,curl_state);
    curl_easy_perform(curl_handle);
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
    node_ClearItems(curl_state);
    node_Free(curl_state,True);
    dec_obj_refcount(read_block);
    add_garbage(state,read_block);
  }
  return(value);
}

#pragma GCC diagnostic pop


#endif

