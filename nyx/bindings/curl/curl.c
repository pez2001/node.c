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

void curl_bind(node *class)
{
  add_member(class,curl_create_class_object());
}

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

  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  char *ret = (char*)malloc((size*nmemb)+1);
  memset(ret+(size*nmemb) + 0, 0, 1);
  memcpy(ret,ptr,(size*nmemb));
  node_SetString(real_value,ret);
  free(ret);
  set_obj_string(value,"name","data");
  node *parameters = create_obj("parameters");
  node_AddItem(parameters,value);
  node *read_obj = execute_obj(state,read_block,block,parameters,True,False);
  return((size*nmemb));
}

node *curl_get(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  set_obj_string(value,"name","http.answer");
  reset_obj_refcount(value);
  add_garbage(state,value);
  if(node_GetItemsNum(parameters))
  {
    node *nurl = node_GetItem(parameters,0);
    node *read_block = node_GetItem(parameters,1);

    node *url_value=node_GetItemByKey(nurl,"value");
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
    node_ClearItems(curl_state);
    node_Free(curl_state,True);
    dec_obj_refcount(read_block);
    add_garbage(state,read_block);
  }
  return(value);
}

#endif
