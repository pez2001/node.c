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
  add_member(class,microhttpd_create_class_object());
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
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *real_value = node_GetItemByKey(value,"value");
  node_SetString(real_value,url);
  set_obj_string(value,"name","url");
  node *parameters = create_obj("parameters");
  node_AddItem(parameters,value);
  node *ret_obj = execute_obj(state,read_block,block,parameters,True,False);
  node *ret_obj_value = node_GetItemByKey(ret_obj,"value");
  char *me = node_GetString(ret_obj_value);

  response = MHD_create_response_from_buffer(strlen(me),(void *)me,MHD_RESPMEM_PERSISTENT);
  ret = MHD_queue_response(connection,MHD_HTTP_OK,response);
  MHD_destroy_response(response);
  return(ret);
}


node *microhttpd_start(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  set_obj_string(value,"name","microhttpd.daemon");
  reset_obj_refcount(value);
  add_garbage(state,value);
  if(node_GetItemsNum(parameters))
  {
    node *port = node_GetItem(parameters,0);
    node *port_value=node_GetItemByKey(port,"value");
    node *read_block = node_GetItem(parameters,1);
    node *mhd_state = node_Create();
    inc_obj_refcount(read_block);
    node_AddItem(mhd_state,state);
    node_AddItem(mhd_state,read_block);
    node_AddItem(mhd_state,block);
    set_obj_node(value,"microhttpd.state",mhd_state);
    struct MHD_Daemon *d = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY,node_GetSint32(port_value),NULL,NULL,&microhttpd_answer,mhd_state,MHD_OPTION_CONNECTION_TIMEOUT,(unsigned int)120,MHD_OPTION_END);
    node *daemon = create_class_instance(base_class);
    reset_obj_refcount(daemon);
    set_obj_string(daemon,"name","daemon_instance");
    set_obj_ptr(daemon,"value",(void*)d);
    node_AddItem(mhd_state,daemon);
    inc_obj_refcount(daemon);
  }
  return(value);
}

node *microhttpd_stop(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
  node *value = create_class_instance(base_class);
  reset_obj_refcount(value);
  add_garbage(state,value);
  if(node_GetItemsNum(parameters))
  {
    node *pstate = node_GetItem(parameters,0);
    node *nmhd_state = node_GetItemByKey(pstate,"microhttpd.state");
    node *mhd_state = node_GetNode(nmhd_state);
    node *read_block = node_GetItem(mhd_state,1);
    node *daemon = node_GetItem(mhd_state,3);
    node *daemon_value = node_GetItemByKey(daemon,"value");
    struct MHD_Daemon *d = (struct MHD_Daemon*)node_GetValue(daemon_value);
    MHD_stop_daemon(d);

    dec_obj_refcount(daemon);
    add_garbage(state,daemon);
    dec_obj_refcount(read_block);
    add_garbage(state,read_block);
    node_ClearItems(mhd_state);
    node_FreeTree(mhd_state);
    set_obj_string(value,"name","success");
  }
  return(value);
}

#endif

