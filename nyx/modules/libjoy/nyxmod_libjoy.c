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


  add_class_object_function(base,"sm_open",nyxjoy_sm_open);
  add_class_object_function(base,"sm_close",nyxjoy_sm_close);
  add_class_object_function(base,"sm_add",nyxjoy_sm_add);
  add_class_object_function(base,"sm_remove",nyxjoy_sm_remove);
  add_class_object_function(base,"sm_poll",nyxjoy_sm_poll);
  add_class_object_function(base,"sm_wait",nyxjoy_sm_wait);
  add_class_object_function(base,"sm_wait_extra_precision",nyxjoy_sm_wait_extra_precision);
  add_class_object_function(base,"sm_get_state",nyxjoy_sm_get_state);


  node *base_class = get_base_class(state);
  node *any_id = create_class_instance(base_class);
  inc_obj_refcount(any_id);
  set_obj_string(any_id,"name","ANY");
  set_obj_int(any_id,"value",255);
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

  node *init_event = create_class_instance(base_class);
  inc_obj_refcount(init_event);
  set_obj_string(init_event,"name","INIT_EVENT");
  set_obj_int(init_event,"value",0x80);
  add_member(base,init_event);

  node *button_event = create_class_instance(base_class);
  inc_obj_refcount(button_event);
  set_obj_string(button_event,"name","BUTTON_EVENT");
  set_obj_int(button_event,"value",1);
  add_member(base,button_event);

  node *axis_event = create_class_instance(base_class);
  inc_obj_refcount(axis_event);
  set_obj_string(axis_event,"name","AXIS_EVENT");
  set_obj_int(axis_event,"value",2);
  add_member(base,axis_event);


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
  add_class_object_function(value,"get_filtered_event",nyxjoy_get_filtered_event);
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
  node *value = get_false_class(state);
  node *obj_value = get_value(obj);
  long joy_dev = node_GetSint32(obj_value);
  node *ntype = node_GetItem(parameters,0);
  node *nid = node_GetItem(parameters,1);
  node *type_value = get_value(ntype);
  node *id_value = get_value(nid);

  if(joy_dev != -1)
  {
    struct js_event *event = (struct js_event*)malloc(sizeof(struct js_event));
    //printf("polling joy events:%d\n",joy_dev);
    unsigned char type = (unsigned char)node_GetSint32(type_value);
    unsigned char id = (unsigned char)node_GetSint32(id_value);
    int r=ljoy_GetFilteredEvent((int)joy_dev,event,type,id);
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

node *nyxjoy_get_filtered_type_event(node *state,node *self,node *obj,node *block,node *parameters)
{
  node *value = get_false_class(state);
  node *obj_value = get_value(obj);
  long joy_dev = node_GetSint32(obj_value);
  node *ntype = node_GetItem(parameters,0);
  node *type_value = get_value(ntype);

  if(joy_dev != -1)
  {
    struct js_event *event = (struct js_event*)malloc(sizeof(struct js_event));
    unsigned char type = (unsigned char)node_GetSint32(type_value);
    int r=ljoy_GetFilteredTypeEvent((int)joy_dev,event,type);
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

node *nyxjoy_sm_open(node *state,node *self,node *obj,node *block,node *parameters)
{
  node *value = get_true_class(state);
  if(ljoy_sm_Init())
    value = get_false_class(state);
  return(value);
}

node *nyxjoy_sm_close(node *state,node *self,node *obj,node *block,node *parameters)
{
  node *value = get_true_class(state);
  if(ljoy_sm_Shutdown())
    value = get_false_class(state);
  return(value);
}

node *nyxjoy_sm_add(node *state,node *self,node *obj,node *block,node *parameters)
{
  node *value = get_true_class(state);
  node *joy = node_GetItem(parameters,0);
  node *joy_value = get_value(joy);
  long joy_dev = node_GetSint32(joy_value);
  if(joy_dev != -1)
    if(ljoy_sm_AddDevice(joy_dev))
      value = get_false_class(state);
  return(value);
}

node *nyxjoy_sm_remove(node *state,node *self,node *obj,node *block,node *parameters)
{
  node *value = get_true_class(state);
  node *joy = node_GetItem(parameters,0);
  node *joy_value = get_value(joy);
  long joy_dev = node_GetSint32(joy_value);
  if(joy_dev != -1)
    if(ljoy_sm_RemoveDevice(joy_dev))
      value = get_false_class(state);
  return(value);
}

node *nyxjoy_sm_poll(node *state,node *self,node *obj,node *block,node *parameters)
{
  node *value = get_true_class(state);
  if(ljoy_sm_Poll())
      value = get_false_class(state);
  return(value);
}


node *nyxjoy_sm_get_state(node *state,node *self,node *obj,node *block,node *parameters)
{
  struct ljoy_sm_state *sm_state=NULL;
  node *value = get_false_class(state);
  node *joy = node_GetItem(parameters,0);
  node *joy_value = get_value(joy);
  long joy_dev = node_GetSint32(joy_value);
  if(joy_dev != -1)
    if(!ljoy_sm_GetState(joy_dev,&sm_state))
    {
      node *base_class = get_base_class(state);
      value = create_class_instance(base_class);
      set_obj_string(value,"name","joystick.state");
      add_garbage(state,value);

      node *state_num_axes = create_class_instance(base_class);
      inc_obj_refcount(state_num_axes);
      set_obj_string(state_num_axes,"name","num_axes");
      set_obj_int(state_num_axes,"value",sm_state->num_axes);
      add_member(value,state_num_axes);

      node *state_num_buttons = create_class_instance(base_class);
      inc_obj_refcount(state_num_buttons);
      set_obj_string(state_num_buttons,"name","num_buttons");
      set_obj_int(state_num_buttons,"value",sm_state->num_buttons);
      add_member(value,state_num_buttons);

      node *state_buttons = create_class_instance(base_class);
      inc_obj_refcount(state_buttons);
      set_obj_string(state_buttons,"name","buttons");
      node *buttons_items = create_obj("items");
      add_obj_kv(state_buttons,buttons_items);
      add_member(value,state_buttons);

      node *state_axes = create_class_instance(base_class);
      inc_obj_refcount(state_axes);
      set_obj_string(state_axes,"name","axes");
      node *axes_items = create_obj("items");
      add_obj_kv(state_axes,axes_items);
      add_member(value,state_axes);

      for(int i=0;i<sm_state->num_axes;i++)
      {
        node *ev_value = create_class_instance(base_class);
        inc_obj_refcount(ev_value);
        set_obj_string(ev_value,"name","joystick.axis_value");
        set_obj_int(ev_value,"value",sm_state->axes[i]);
        add_member(value,ev_value);
        set_obj_int(ev_value,"item_index",i);
        inc_obj_refcount(ev_value);
        node_AddItem(axes_items,ev_value);
      }
      for(int i=0;i<sm_state->num_buttons;i++)
      {
        node *ev_value = create_class_instance(base_class);
        inc_obj_refcount(ev_value);
        set_obj_string(ev_value,"name","joystick.button_value");
        set_obj_int(ev_value,"value",sm_state->buttons[i]);
        add_member(value,ev_value);
        set_obj_int(ev_value,"item_index",i);
        inc_obj_refcount(ev_value);
        node_AddItem(buttons_items,ev_value);
      }

      node *state_time = create_class_instance(base_class);
      inc_obj_refcount(state_time);
      set_obj_string(state_time,"name","timestamp");
      set_obj_int(state_time,"value",sm_state->timestamp);
      add_member(value,state_time);
    }
  return(value);
}

node *nyxjoy_sm_wait(node *state,node *self,node *obj,node *block,node *parameters)
{
  node *timeout = node_GetItem(parameters,0);
  node *timeout_value = get_value(timeout);
  node *value = get_true_class(state);
  if(ljoy_sm_Wait(node_GetSint32(timeout_value)))
      value = get_false_class(state);
  return(value);
}

node *nyxjoy_sm_wait_extra_precision(node *state,node *self,node *obj,node *block,node *parameters)
{
  node *timeout_s = node_GetItem(parameters,0);
  node *timeout_s_value = get_value(timeout_s);
  node *timeout_ms = node_GetItem(parameters,1);
  node *timeout_ms_value = get_value(timeout_ms);
  node *value = get_true_class(state);
  if(ljoy_sm_WaitExtraPrecision(node_GetSint32(timeout_s_value),node_GetSint32(timeout_ms_value)))
      value = get_false_class(state);
  return(value);
}



#pragma GCC diagnostic pop



