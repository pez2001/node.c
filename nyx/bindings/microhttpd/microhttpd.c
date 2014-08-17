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

#include "microhttpd.h"

#ifdef USE_MICROHTTPD

void microhttpd_bind(node *class)
{
  node *httpd = microhttpd_create_class_object();
  add_member(class,httpd);
  inc_obj_refcount(httpd);
}

node *microhttpd_create_class_object(void)
{
  node *base = create_base_obj_layout("microhttpd");
  add_class_object_function(base,"=",nyxh_assign);
  add_class_object_function(base,"start",microhttpd_start);
  add_class_object_function(base,"stop",microhttpd_stop);
  return(base);
}

int microhttpd_answer(void *cls,struct MHD_Connection *connection,const char *url,const char *method,const char *version,const char *upload_data, size_t *upload_data_size, void **ptr)
{
  struct MHD_Response *response;
  int ret;
  node *mhd_state = (node*)cls;
  node *state = node_GetItem(mhd_state,0);
  node *read_block = node_GetItem(mhd_state,1);
  node *block = node_GetItem(mhd_state,2);
  node *base_class = get_base_class(state);

  node *parameters = create_obj("parameters");

  node *url_value = create_class_instance(base_class);
  add_garbage(state,url_value);
  set_obj_string(url_value,"name","url");
  set_obj_string(url_value,"value",(char*)url);
  set_obj_int(url_value,"item_index",0);
  node_AddItem(parameters,url_value);

  node *method_value = create_class_instance(base_class);
  add_garbage(state,method_value);
  set_obj_string(method_value,"value",(char*)method);
  set_obj_string(method_value,"name","method");
  set_obj_int(method_value,"item_index",1);
  node_AddItem(parameters,method_value);

  node *version_value = create_class_instance(base_class);
  add_garbage(state,version_value);
  set_obj_string(version_value,"name","version");
  set_obj_string(version_value,"value",(char*)version);
  set_obj_int(version_value,"item_index",2);
  node_AddItem(parameters,version_value);

  node *upload_value = create_class_instance(base_class);
  add_garbage(state,upload_value);
  char *uploads = (char*)malloc(*upload_data_size+1);
  memset(uploads+*upload_data_size + 0, 0, 1);
  memcpy(uploads,upload_data,*upload_data_size);
  set_obj_string(upload_value,"name","upload");
  set_obj_string(upload_value,"value",uploads);
  set_obj_int(upload_value,"item_index",3);
  node_AddItem(parameters,upload_value);

  node *ret_obj = execute_obj(state,read_block,block,parameters,True,False,True);
  node *ret_obj_value = get_value(ret_obj);
  char *me = node_GetString(ret_obj_value);

  response = MHD_create_response_from_buffer(strlen(me),(void *)me,MHD_RESPMEM_PERSISTENT);
  ret = MHD_queue_response(connection,MHD_HTTP_OK,response);
  MHD_destroy_response(response);
  return(ret);
}


node *microhttpd_start(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  set_obj_string(value,"name","microhttpd.daemon");
  add_garbage(state,value);
  if(node_GetItemsNum(parameters))
  {
    node *port = node_GetItem(parameters,0);
    node *port_value = get_value(port);
    node *read_block = node_GetItem(parameters,1);
    node *mhd_state = node_Create();
    inc_obj_refcount(read_block);
    node_AddItem(mhd_state,state);
    node_AddItem(mhd_state,read_block);
    node_AddItem(mhd_state,block);
    node *value_privates = node_GetItemByKey(value,"privates");

    set_obj_node(value_privates,"microhttpd.state",mhd_state);
    struct MHD_Daemon *d = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY,node_GetSint32(port_value),NULL,NULL,&microhttpd_answer,mhd_state,MHD_OPTION_CONNECTION_TIMEOUT,(unsigned int)120,MHD_OPTION_END);
    node *daemon = create_class_instance(base_class);
    set_obj_string(daemon,"name","daemon_instance");
    set_obj_ptr(daemon,"value",(void*)d);
    node_AddItem(mhd_state,daemon);
    inc_obj_refcount(daemon);
  }
  return(value);
}

node *microhttpd_stop(node *state,node *obj,node *block,node *parameters)
{
  node *value = get_false_class(state);
  if(node_GetItemsNum(parameters))
  {
    node *pstate = node_GetItem(parameters,0);
    node *privates = node_GetItemByKey(pstate,"privates");
    node *nmhd_state = node_GetItemByKey(privates,"microhttpd.state");
    node *mhd_state = node_GetNode(nmhd_state);
    node *read_block = node_GetItem(mhd_state,1);
    node *daemon = node_GetItem(mhd_state,3);
    node *daemon_value = get_value(daemon);
    struct MHD_Daemon *d = (struct MHD_Daemon*)node_GetValue(daemon_value);
    MHD_stop_daemon(d);

    dec_obj_refcount(daemon);
    add_garbage(state,daemon);
    dec_obj_refcount(read_block);
    add_garbage(state,read_block);
    node_ClearItems(mhd_state);
    node_FreeTree(mhd_state);
    value = get_true_class(state);
  }
  return(value);
}

#endif

