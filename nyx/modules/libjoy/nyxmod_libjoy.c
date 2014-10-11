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

#include "nyxmod_libjoy.h"


node *nyxjoy_create_class_object(node *state)
{
  node *base = create_base_obj_layout("joystick");
  add_class_object_function(base,"=",nyxh_assign);
  add_class_object_function(base,"open",nyxjoy_open);

  node *base_class = get_base_class(state);
  node *any_id = create_class_instance(base_class);
  inc_obj_refcount(any_id);
  set_obj_string(any_id,"name","ANY");
  set_obj_int(any_id,"value",254);
  add_member(base,any_id);

  node *non_blocking = create_class_instance(base_class);
  inc_obj_refcount(non_blocking);
  set_obj_string(non_blocking,"name","NON_BLOCKING");
  set_obj_int(non_blocking,"value",1);
  add_member(base,non_blocking);

  node *blocking = create_class_instance(base_class);
  inc_obj_refcount(blocking);
  set_obj_string(blocking,"name","BLOCKING");
  set_obj_int(blocking,"value",0);
  add_member(base,blocking);

  node *joysticks_num = create_class_instance(base_class);
  inc_obj_refcount(joysticks_num);
  set_obj_string(joysticks_num,"name","num");
  int joys_num = ljoy_GetJoysticksNum();
  set_obj_int(joysticks_num,"value",(long)joys_num);
  add_member(base,joysticks_num);

  node *ordered_mode = create_base_obj_layout("ordered_mode");//TODO switch to create_class_instance and use remove_member_by_key to replace =
  add_class_object_function(ordered_mode,"=",nyxjoy_set_ordered_mode);
  int ordered_mode_state = ljoy_GetOrderedMode();
  set_obj_int(ordered_mode,"value",(long)ordered_mode_state);
  inc_obj_refcount(ordered_mode);
  add_member(base,ordered_mode);



  return(base);
}

/*
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
  //printf("curl data handler callback\n");
  //node_PrintTree(value);
  //node *read_obj = 
  //node_PrintTree(read_block);
  execute_obj(state,read_block,block,parameters,True,False,True);
  //execute_obj(state,read_block,block,parameters,True,True,True);
  return((size*nmemb));
}
*/

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

node *module_open(node *state)
{
  node *modules = get_modules(state);
  //node *block_class = get_block_class(state);
  node *ljoy = nyxjoy_bind(state,modules);
  //node *proxy = create_proxy_object(state,ljoy,"joystick");
  //inc_obj_refcount(ljoy);
  //add_member(block_class,proxy);
  //inc_obj_refcount(proxy);
  sys_add_module(state,"joystick");
  return(ljoy);
}

void module_close(node *state)
{
}


node *nyxjoy_bind(node *state,node *class)
{
  node *ljoy = nyxjoy_create_class_object(state);
  add_member(class,ljoy);
  inc_obj_refcount(ljoy);
  return(ljoy);
}

node *nyxjoy_open(node *state,node *self,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  set_obj_string(value,"name","joystick.device");
  add_garbage(state,value);
  add_class_object_function(value,"close",nyxjoy_close);
  add_class_object_function(value,"get_event",nyxjoy_get_event);
  int joy_id = -1;
  if(node_GetItemsNum(parameters))
  {
    node *joy_dev = node_GetItem(parameters,0);
    node *blocking_mode = node_GetItem(parameters,1);
    node *joy_dev_value = get_value(joy_dev);
    node *blocking_mode_value = get_value(blocking_mode);
    if(node_GetType(joy_dev_value)==NODE_TYPE_STRING)
      joy_id = ljoy_Open(node_GetString(joy_dev_value),node_GetSint32(blocking_mode_value));
    else
      joy_id = ljoy_OpenById(node_GetSint32(joy_dev_value),node_GetSint32(blocking_mode_value));

    node *name = create_class_instance(base_class);
    inc_obj_refcount(name);
    set_obj_string(name,"name","name");
    char *joy_name = ljoy_GetName(joy_id);
    set_obj_string(name,"value",joy_name);
    free(joy_name);
    add_member(value,name);

    node *axes_num = create_class_instance(base_class);
    inc_obj_refcount(axes_num);
    set_obj_string(axes_num,"name","axes_num");
    int joy_axes_num = ljoy_GetAxesNum(joy_id);
    set_obj_int(axes_num,"value",joy_axes_num);
    add_member(value,axes_num);

    node *buttons_num = create_class_instance(base_class);
    inc_obj_refcount(buttons_num);
    set_obj_string(buttons_num,"name","buttons_num");
    int joy_buttons_num = ljoy_GetButtonsNum(joy_id);
    set_obj_int(buttons_num,"value",(long)joy_buttons_num);
    add_member(value,buttons_num);
    //printf("opened joy device:(rq:%d) %d\n",node_GetSint32(joy_dev_value),joy_id);
  }
  set_obj_int(value,"value",joy_id);
  return(value);
}

node *nyxjoy_close(node *state,node *self,node *obj,node *block,node *parameters)
{
  node *value = get_false_class(state);
  node *obj_value = get_value(obj);
  long joy_dev = node_GetSint32(obj_value);
  if(joy_dev != -1)
  {
    ljoy_Close((int)joy_dev);
    value = get_true_class(state);
  }
  return(value);
}


node *nyxjoy_get_event(node *state,node *self,node *obj,node *block,node *parameters)
{
  node *value = get_false_class(state);
  node *obj_value = get_value(obj);
  long joy_dev = node_GetSint32(obj_value);
  if(joy_dev != -1)
  {
    struct js_event *event = (struct js_event*)malloc(sizeof(struct js_event));
    //printf("polling joy events:%d\n",joy_dev);
    int r=ljoy_GetEvent((int)joy_dev,event);
    if(r!= -1)
    {
      // struct js_event {
      //  __u32 time; /* event timestamp in milliseconds */
      //  __s16 value;  /* value */
      //  __u8 type;  /* event type */
      //  __u8 number;  /* axis/button number */
      // };
      //printf("received joystick event\n");
      node *base_class = get_base_class(state);
      value = create_class_instance(base_class);
      set_obj_string(value,"name","joystick.event");
      add_garbage(state,value);

      node *ev_value = create_class_instance(base_class);
      inc_obj_refcount(ev_value);
      set_obj_string(ev_value,"name","value");
      set_obj_int(ev_value,"value",event->value);
      add_member(value,ev_value);

      node *ev_type = create_class_instance(base_class);
      inc_obj_refcount(ev_type);
      set_obj_string(ev_type,"name","type");
      set_obj_int(ev_type,"value",event->type);
      add_member(value,ev_type);

      node *ev_time = create_class_instance(base_class);
      inc_obj_refcount(ev_time);
      set_obj_string(ev_time,"name","time");
      set_obj_int(ev_time,"value",event->time);
      add_member(value,ev_time);

      node *ev_number = create_class_instance(base_class);
      inc_obj_refcount(ev_number);
      set_obj_string(ev_number,"name","number");
      set_obj_int(ev_number,"value",event->number);
      add_member(value,ev_number);
      set_obj_int(value,"value",1);
    }
    else
    {
      //printf("error polling joystick :%s\n",strerror(errno));

    }
    free(event);
  }
  return(value);
}

node *nyxjoy_get_filtered_event(node *state,node *self,node *obj,node *block,node *parameters)
{

}

node *nyxjoy_get_filtered_type_event(node *state,node *self,node *obj,node *block,node *parameters)
{

}

node *nyxjoy_set_ordered_mode(node *state,node *self,node *obj,node *block,node *parameters)
{
  if(node_GetItemsNum(parameters))
  {
    node *mode = node_GetItem(parameters,0);
    node *mode_value=get_value(mode);
    int ordered_mode = (int)node_GetSint32(mode_value);
    ljoy_SetOrderedMode(ordered_mode);
  }

  int ordered_mode_state = ljoy_GetOrderedMode();
  set_obj_int(obj,"value",(long)ordered_mode_state);
  return(obj);
}



/*
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
*/

#pragma GCC diagnostic pop



