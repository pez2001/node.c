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

#include "websockets.h"

#ifdef USE_WEBSOCKETS

void websockets_bind(node *class)
{
  node *websockets = websockets_create_class_object();
  add_member(class,websockets);
  inc_obj_refcount(websockets);
}

node *websockets_create_class_object(void)
{
  node *base = create_base_obj_layout("websockets");
  add_class_object_function(base,"=",nyxh_assign);
  add_class_object_function(base,"start",websockets_start);
  //add_class_object_function(base,"service",websockets_service);
  add_class_object_function(base,"stop",websockets_stop);
  return(base);
}

node *create_session(node *state,node *sessions,long session_uid,char *protocol_name)
{
  node *base_class = get_base_class(state);
  node *session = create_class_instance(base_class);
  //reset_obj_refcount(session);
  set_obj_string(session,"name","session");
  //set_obj_string(prot_value,"value",get_obj_name(found_prot));

  node *nsession_uid = create_class_instance(base_class);
  //reset_obj_refcount(nsession_uid);
  set_obj_string(nsession_uid,"name","id");
  set_obj_int(nsession_uid,"value",session_uid);
  add_member(session,nsession_uid);
  inc_obj_refcount(nsession_uid);

  node *nsession_protocol = create_class_instance(base_class);
  //reset_obj_refcount(nsession_uid);
  set_obj_string(nsession_protocol,"name","protocol");
  set_obj_string(nsession_protocol,"value",protocol_name);
  add_member(session,nsession_protocol);
  inc_obj_refcount(nsession_protocol);

  node *sessions_items = node_GetItemByKey(sessions,"items");
  set_obj_int(session,"item_index",get_last_index(sessions_items)+1);

  node_AddItem(sessions_items,session);
  node_SetParent(session,sessions_items);
  inc_obj_refcount(session);
  return(session);
}

void delete_session(node *state,node *sessions,node *session)
{
  node *sessions_items = node_GetItemByKey(sessions,"items");
  node_RemoveItem(sessions_items,session);
  node_SetParent(session,NULL);
  dec_obj_refcount(session);
  add_garbage(state,session);
}

struct per_session_data_nyx {
  node *session;
};


static int callback_nyx_websockets(struct libwebsocket_context *context,struct libwebsocket *wsi,enum libwebsocket_callback_reasons reason, void *user,void *in,size_t len)
{
  struct per_session_data_nyx *pss =(struct per_session_data_nyx*)user;
  int n;

  node *wsd_state = (node*)libwebsocket_context_user(context);
  node *found_prot = NULL;
  node *state = NULL;
  node *block = NULL;
  node *daemon = NULL;
  node *daemon_obj = NULL;
  node *session_uid = NULL;
  long lsession_uid = 0;
  node *sessions_num = NULL;
  node *sessions = NULL;
  node *broadcast_message = NULL;
  long lsessions_num = 0;
  if(wsi && wsd_state)
  {
    state = node_GetItem(wsd_state,0);
    block = node_GetItem(wsd_state,1);
    daemon = node_GetItem(wsd_state,2);
    node *protocols = node_GetItem(wsd_state,3);
    session_uid = node_GetItem(wsd_state,4);
    node *session_uid_value = node_GetItemByKey(session_uid,"value");
    lsession_uid = node_GetSint32(session_uid_value);
    sessions_num = node_GetItem(wsd_state,5);
    sessions = node_GetItem(wsd_state,6);
    node *sessions_num_value = node_GetItemByKey(sessions_num,"value");
    lsessions_num = node_GetSint32(sessions_num_value);
    daemon_obj = node_GetItem(wsd_state,9);
    //broadcast_message = node_GetItem(wsd_state,10);

    node *protocols_items = node_GetItemByKey(protocols,"items");
    struct libwebsocket_protocols *prot = libwebsockets_get_protocol(wsi);
    if(prot && prot->name)
    {
      node_ItemIterationReset(protocols_items);
      while(node_ItemIterationUnfinished(protocols_items))
      {
        node *proto = node_ItemIterate(protocols_items);
        if(!strcmp(get_obj_name(proto),prot->name))
        {
          //printf("found proto: %s : %d\n", get_obj_name(proto),reason);
          found_prot = proto;
        }
      }
    }
  }

  switch (reason) 
  {

    case LWS_CALLBACK_HTTP:
      if(len < 1)
      {
        libwebsockets_return_http_status(context,wsi,HTTP_STATUS_BAD_REQUEST,NULL);
        return(-1);
      }
      if(lws_hdr_total_length(wsi,WSI_TOKEN_POST_URI))
        return(0);

      if(found_prot)
      {
        //printf("found prot in http callback : %d,num:%d\n",lsession_uid,lsessions_num);
        lsession_uid++;
        node *session_uid_value = node_GetItemByKey(session_uid,"value");
        node_SetSint32(session_uid_value,lsession_uid);
        lsessions_num++;
        node *sessions_num_value = node_GetItemByKey(sessions_num,"value");
        node_SetSint32(sessions_num_value,lsessions_num);
        pss->session = create_session(state,sessions,lsession_uid,get_obj_name(found_prot));
        node *session_privates = node_GetItemByKey(pss->session,"privates");
        set_obj_int(session_privates,"is_http",1);


        node *parameters = create_obj("parameters");
        node *base_class = get_base_class(state);
        node *prot_value = create_class_instance(base_class);
        set_obj_string(prot_value,"name","protocol");
        set_obj_string(prot_value,"value",get_obj_name(found_prot));
        node_AddItem(parameters,prot_value);
        inc_obj_refcount(prot_value);
        char *url = str_CreateEmpty();
        url = str_AddChars(url,in,len);
        node *url_value = create_class_instance(base_class);
        set_obj_string(url_value,"name","url");
        set_obj_string(url_value,"value",url);
        node_AddItem(parameters,url_value);
        inc_obj_refcount(url_value);
        free(url);
        node_AddItem(parameters,pss->session);
        node_AddItem(parameters,daemon_obj);
        node *ret_obj = execute_obj(state,found_prot,block,parameters,True,False,True);
        //dec_obj_refcount(msg_value);
        dec_obj_refcount(prot_value);
        //add_garbage(state,msg_value);//TODO check if "just survives"
        add_garbage(state,prot_value);
        node *ret_obj_value = node_GetItemByKey(ret_obj,"value");
        if( (node_GetType(ret_obj_value)==NODE_TYPE_STRING && strlen(node_GetString(ret_obj_value))) || (node_GetType(ret_obj_value)==NODE_TYPE_BINARY && node_GetBinaryLength(ret_obj_value)) )
        {
          //printf("returning http message: [%s] :%d\n",node_GetString(ret_obj_value),strlen(node_GetString(ret_obj_value)));
          node *ret_obj_copy = node_CopyTree(ret_obj,True,True);
          set_obj_string(ret_obj_copy,"name","message");
          add_member(pss->session,ret_obj_copy);
          inc_obj_refcount(ret_obj_copy);
          libwebsocket_callback_on_writable(context, wsi);
        }
      }

      //libwebsocket_callback_on_writable(context, wsi);
      break;



    case LWS_CALLBACK_HTTP_BODY_COMPLETION:
      lwsl_notice("LWS_CALLBACK_HTTP_BODY_COMPLETION\n");
      libwebsockets_return_http_status(context,wsi,HTTP_STATUS_OK,NULL);
     return(-1);

    case LWS_CALLBACK_HTTP_FILE_COMPLETION:
      return(-1);


    case LWS_CALLBACK_HTTP_WRITEABLE: 
    case LWS_CALLBACK_SERVER_WRITEABLE:
      {
        node *message = get_member(pss->session,"message");
        node *session_id = get_member(pss->session,"id");
        node *session_id_value = node_GetItemByKey(session_id,"value");
        if(message)
        {
          node *session_privates = node_GetItemByKey(pss->session,"privates");
          node *http_only = node_GetItemByKey(session_privates,"is_http");
          node *message_value = node_GetItemByKey(message,"value");
          char *me = NULL;
          unsigned long me_len = 0;
          if(node_GetType(message_value)==NODE_TYPE_STRING)
          {
            me = node_GetString(message_value);
            me_len = strlen(me);
          }
          else if(node_GetType(message_value)==NODE_TYPE_BINARY)
          {
            me = node_GetBinary(message_value);
            me_len = node_GetBinaryLength(message_value);
          }
          //printf("sending message now: [%s] to: %d\n",me,node_GetSint32(session_id_value));
          //fflush(stdout);
          if(http_only)
            n = libwebsocket_write(wsi, me, me_len, LWS_WRITE_HTTP);
          else
            n = libwebsocket_write(wsi, me, me_len, LWS_WRITE_TEXT);
          if(n<0)
          {
            printf("ERROR %d writing to socket, hanging up\n", n);
            return(1);
          }
          if(n<strlen(me))
          {
            printf("Partial write\n");
            return(-1);
          }
          //node_FreeTree(pss->message);
          remove_member(pss->session,message);
          dec_obj_refcount(message);
          add_garbage(state,message);
          //pss->message = NULL;
        }
      }
    break;

  case LWS_CALLBACK_ESTABLISHED:
    //printf("init pss: %x ,message: %x\n",pss,pss->message);
    if(found_prot)
    {
      //printf("found prot in establish callback : %d,num:%d\n",lsession_uid,lsessions_num);
      lsession_uid++;
      node *session_uid_value = node_GetItemByKey(session_uid,"value");
      node_SetSint32(session_uid_value,lsession_uid);
      lsessions_num++;
      node *sessions_num_value = node_GetItemByKey(sessions_num,"value");
      node_SetSint32(sessions_num_value,lsessions_num);
      pss->session = create_session(state,sessions,lsession_uid,get_obj_name(found_prot));
    }
    break;

  case LWS_CALLBACK_CLOSED:
  case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
    //printf("closed pss: %x ,message: %x\n",pss,pss->message);
    if(found_prot)
    {
      //printf("found prot in closed callback : %d,num:%d\n",lsession_uid,lsessions_num);
      lsessions_num--;
      node *sessions_num_value = node_GetItemByKey(sessions_num,"value");
      node_SetSint32(sessions_num_value,lsessions_num);
      delete_session(state,sessions,pss->session);
    }
    break;



  case LWS_CALLBACK_RECEIVE:
    if(len>1024) { //TODO use some variable
      lwsl_err("Server received packet bigger than %u, hanging up\n", 1024);
      return(1);
    }
    if(found_prot)
    {
      node *parameters = create_obj("parameters");
      node *base_class = get_base_class(state);
      
      node *prot_value = create_class_instance(base_class);
      set_obj_string(prot_value,"name","protocol");
      set_obj_string(prot_value,"value",get_obj_name(found_prot));
      node_AddItem(parameters,prot_value);
      inc_obj_refcount(prot_value);

      char *msg = str_CreateEmpty();
      msg = str_AddChars(msg,in,len);
      node *msg_value = create_class_instance(base_class);
      set_obj_string(msg_value,"name","message");
      set_obj_string(msg_value,"value",msg);
      node_AddItem(parameters,msg_value);
      inc_obj_refcount(msg_value);
      free(msg);

      /*node *session_value = create_class_instance(base_class);
      reset_obj_refcount(session_value);
      add_garbage(state,session_value);
      set_obj_string(session_value,"name","session_id");
      set_obj_int(session_value,"value",lsession_uid);
      set_obj_int(session_value,"item_index",2);
      node_AddItem(parameters,session_value);
      */
      node_AddItem(parameters,pss->session);
      node_AddItem(parameters,daemon_obj);
      //inc_obj_refcount(daemon);


      node *ret_obj = execute_obj(state,found_prot,block,parameters,True,False,True);

      dec_obj_refcount(msg_value);
      dec_obj_refcount(prot_value);
      add_garbage(state,msg_value);//TODO check if "just survives"
      add_garbage(state,prot_value);
      //node *ret_obj_value = node_GetItemByKey(ret_obj,"value");
      //char *me = node_GetString(ret_obj_value);
      //printf("returned string:[%s]\n",me);
      node *ret_obj_value = node_GetItemByKey(ret_obj,"value");
      if(node_GetType(ret_obj_value)==NODE_TYPE_STRING && strlen(node_GetString(ret_obj_value)))
      {
        //printf("returning message: [%s] :%d\n",node_GetString(ret_obj_value),strlen(node_GetString(ret_obj_value)));
        node *ret_obj_copy = node_CopyTree(ret_obj,True,True);
        set_obj_string(ret_obj_copy,"name","message");
        add_member(pss->session,ret_obj_copy);
        inc_obj_refcount(ret_obj_copy);
        //set_obj_string(ret_obj,"name","message");
        //add_member(pss->session,ret_obj);
        //inc_obj_refcount(ret_obj);
        libwebsocket_callback_on_writable(context, wsi);
      }
    }

    break;
  default:
    break;
  }

  return(0);
}
  

node *websockets_broadcast(node *state,node *obj,node *block,node *parameters)
{
  //printf("broadcasting\n");
  node *value = get_true_class(state);
  node *wsd_state = node_GetNode(get_value(obj));
  node *daemon = node_GetItem(wsd_state,2);
  node *daemon_value = node_GetItemByKey(daemon,"value");
  node *state_protocols = node_GetItem(wsd_state,3);
  node *session_uid = node_GetItem(wsd_state,4);
  node *sessions_num = node_GetItem(wsd_state,5);
  node *sessions = node_GetItem(wsd_state,6);
  node *daemon_info = node_GetItem(wsd_state,7);
  node *daemon_info_value = node_GetItemByKey(daemon_info,"value");
  node *daemon_prots = node_GetItem(wsd_state,8);
  node *daemon_prots_value = node_GetItemByKey(daemon_prots,"value");
  struct libwebsocket_context *context = (struct libwebsocket_context *)node_GetValue(daemon_value);

  node *protocol = node_GetItem(parameters,0);
  node *protocol_value = node_GetItemByKey(protocol,"value");
  node *message = node_GetItem(parameters,1);
  node *message_value = node_GetItemByKey(message,"value");


  char *prots = (char*)node_GetValue(daemon_prots_value);
  node *state_protocols_items = node_GetItemByKey(state_protocols,"items");

  node_ItemIterationReset(state_protocols_items);
  while(node_ItemIterationUnfinished(state_protocols_items))
  {
    node *state_protocol = node_ItemIterate(state_protocols_items);
    node *state_protocol_value = node_GetItemByKey(state_protocol,"name");
    //printf("checking protocols:%s,%s\n",node_GetString(state_protocol_value),node_GetString(protocol_value));
    //fflush(stdout);
    struct libwebsocket_protocols *cprot = prots;
    if(!strcmp(node_GetString(state_protocol_value),node_GetString(protocol_value)))
    {
      //printf("now broadcasting:[%s]\n",node_GetString(message_value));
      //fflush(stdout);
      node *base_class = get_base_class(state);
      node *broadcast_message = create_class_instance(base_class);
      set_obj_string(broadcast_message,"name","message");
      set_obj_string(broadcast_message,"value",node_GetString(message_value));
      reset_obj_refcount(broadcast_message);//TODO remove these after create_class_instance
      //node_AddItem(wsd_state,broadcast_message);//3
      node *sessions_items = node_GetItemByKey(sessions,"items");
      node_ItemIterationReset(sessions_items);
      while(node_ItemIterationUnfinished(sessions_items))
      {
        node *session = node_ItemIterate(sessions_items);
        node *session_uid = get_member(session,"id");
        node *session_uid_value = get_value(session_uid);
        node *session_prot = get_member(session,"protocol");
        node *session_prot_value = get_value(session_prot);
        //printf("comparing session_prot: [%s] against prot: [%s]\n",node_GetString(session_prot_value),node_GetString(protocol_value));
        //fflush(stdout);
        if(!strcmp(node_GetString(protocol_value),node_GetString(session_prot_value)))
        {
          //printf("sending message to uid: %d\n",node_GetSint32(session_uid_value));
          //fflush(stdout);
          add_member(session,broadcast_message);
          inc_obj_refcount(broadcast_message);
        }
      }

      libwebsocket_callback_on_writable_all_protocol(cprot);
      node_RemoveItem(wsd_state,broadcast_message);
      dec_obj_refcount(broadcast_message);
      add_garbage(state,broadcast_message);

    }
    prots+=sizeof(struct libwebsocket_protocols);
  }
  return(value);
}

//TODO add checks for true and false lists

node *websockets_broadcast_other(node *state,node *obj,node *block,node *parameters)
{
  //printf("broadcasting_other\n");
  node *value = get_true_class(state);
  node *wsd_state = node_GetNode(get_value(obj));
  node *daemon = node_GetItem(wsd_state,2);
  node *daemon_value = node_GetItemByKey(daemon,"value");
  node *state_protocols = node_GetItem(wsd_state,3);
  node *session_uid = node_GetItem(wsd_state,4);
  node *sessions_num = node_GetItem(wsd_state,5);
  node *sessions = node_GetItem(wsd_state,6);
  node *daemon_info = node_GetItem(wsd_state,7);
  node *daemon_info_value = node_GetItemByKey(daemon_info,"value");
  node *daemon_prots = node_GetItem(wsd_state,8);
  node *daemon_prots_value = node_GetItemByKey(daemon_prots,"value");
  struct libwebsocket_context *context = (struct libwebsocket_context *)node_GetValue(daemon_value);

  node *protocol = node_GetItem(parameters,0);
  node *protocol_value = node_GetItemByKey(protocol,"value");
  node *message = node_GetItem(parameters,1);
  node *message_value = node_GetItemByKey(message,"value");
  node *skip_uid = node_GetItem(parameters,2);
  node *skip_uid_value = node_GetItemByKey(skip_uid,"value");
  long lskip_uid = node_GetSint32(skip_uid_value);

  char *prots = (char*)node_GetValue(daemon_prots_value);
  node *state_protocols_items = node_GetItemByKey(state_protocols,"items");

  node_ItemIterationReset(state_protocols_items);
  while(node_ItemIterationUnfinished(state_protocols_items))
  {
    node *state_protocol = node_ItemIterate(state_protocols_items);
    node *state_protocol_value = node_GetItemByKey(state_protocol,"name");
    //printf("checking protocols:%s,%s\n",node_GetString(state_protocol_value),node_GetString(protocol_value));
    struct libwebsocket_protocols *cprot = prots;
    if(!strcmp(node_GetString(state_protocol_value),node_GetString(protocol_value)))
    {
      //printf("now broadcasting:[%s]\n",node_GetString(message_value));
      node *base_class = get_base_class(state);
      node *broadcast_message = create_class_instance(base_class);
      set_obj_string(broadcast_message,"name","message");
      set_obj_string(broadcast_message,"value",node_GetString(message_value));
      //reset_obj_refcount(broadcast_message);//TODO remove these after create_class_instance
      //node_AddItem(wsd_state,broadcast_message);//3
      node *sessions_items = node_GetItemByKey(sessions,"items");
      node_ItemIterationReset(sessions_items);
      while(node_ItemIterationUnfinished(sessions_items))
      {
        node *session = node_ItemIterate(sessions_items);
        node *session_uid = get_member(session,"id");
        node *session_uid_value = node_GetItemByKey(session_uid,"value");
        node *session_prot = get_member(session,"protocol");
        node *session_prot_value = get_value(session_prot);
        long luid = node_GetSint32(session_uid_value);
        
        if(!strcmp(node_GetString(protocol_value),node_GetString(session_prot_value)) && luid != lskip_uid)
        {
          printf("sending message to uid: %d (skip:%d)\n",luid,lskip_uid);
          fflush(stdout);
          add_member(session,broadcast_message);
          inc_obj_refcount(broadcast_message);
        }
      }

      libwebsocket_callback_on_writable_all_protocol(cprot);
      node_RemoveItem(wsd_state,broadcast_message);
      dec_obj_refcount(broadcast_message);
      add_garbage(state,broadcast_message);

    }
    prots+=sizeof(struct libwebsocket_protocols);
  }
  //printf("broadcast_other finished\n");
  //fflush(stdout);  
  return(value);
}

node *websockets_send(node *state,node *obj,node *block,node *parameters)
{
  //printf("send\n");
  node *value = get_true_class(state);
  node *wsd_state = node_GetNode(get_value(obj));
  node *daemon = node_GetItem(wsd_state,2);
  node *daemon_value = node_GetItemByKey(daemon,"value");
  node *state_protocols = node_GetItem(wsd_state,3);
  node *session_uid = node_GetItem(wsd_state,4);
  node *sessions_num = node_GetItem(wsd_state,5);
  node *sessions = node_GetItem(wsd_state,6);
  node *daemon_info = node_GetItem(wsd_state,7);
  node *daemon_info_value = node_GetItemByKey(daemon_info,"value");
  node *daemon_prots = node_GetItem(wsd_state,8);
  node *daemon_prots_value = node_GetItemByKey(daemon_prots,"value");
  struct libwebsocket_context *context = (struct libwebsocket_context *)node_GetValue(daemon_value);

  node *protocol = node_GetItem(parameters,0);
  node *protocol_value = node_GetItemByKey(protocol,"value");
  node *message = node_GetItem(parameters,1);
  node *message_value = node_GetItemByKey(message,"value");
  node *send_uid = node_GetItem(parameters,2);
  node *send_uid_value = node_GetItemByKey(send_uid,"value");
  long lsend_uid = node_GetSint32(send_uid_value);

  char *prots = (char*)node_GetValue(daemon_prots_value);
  node *state_protocols_items = node_GetItemByKey(state_protocols,"items");

  node_ItemIterationReset(state_protocols_items);
  while(node_ItemIterationUnfinished(state_protocols_items))
  {
    node *state_protocol = node_ItemIterate(state_protocols_items);
    node *state_protocol_value = node_GetItemByKey(state_protocol,"name");
    //printf("checking protocols:%s,%s\n",node_GetString(state_protocol_value),node_GetString(protocol_value));
    struct libwebsocket_protocols *cprot = prots;
    if(!strcmp(node_GetString(state_protocol_value),node_GetString(protocol_value)))
    {
      //printf("now broadcasting:[%s]\n",node_GetString(message_value));
      node *base_class = get_base_class(state);
      node *broadcast_message = create_class_instance(base_class);
      set_obj_string(broadcast_message,"name","message");
      set_obj_string(broadcast_message,"value",node_GetString(message_value));
      reset_obj_refcount(broadcast_message);//TODO remove these after create_class_instance
      //node_AddItem(wsd_state,broadcast_message);//3
      node *sessions_items = node_GetItemByKey(sessions,"items");
      node_ItemIterationReset(sessions_items);
      while(node_ItemIterationUnfinished(sessions_items))
      {
        node *session = node_ItemIterate(sessions_items);
        node *session_uid = get_member(session,"id");
        node *session_uid_value = node_GetItemByKey(session_uid,"value");
        long luid = node_GetSint32(session_uid_value);
        if(luid == lsend_uid)
        {
          printf("sending message to uid: %d (send:%d)\n",luid,lsend_uid);
          fflush(stdout);
          add_member(session,broadcast_message);
          inc_obj_refcount(broadcast_message);
        }
      }

      libwebsocket_callback_on_writable_all_protocol(cprot);
      node_RemoveItem(wsd_state,broadcast_message);
      dec_obj_refcount(broadcast_message);
      add_garbage(state,broadcast_message);

    }
    prots+=sizeof(struct libwebsocket_protocols);
  }
  return(value);
}



node *websockets_start(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  set_obj_string(value,"name","websockets.daemon");
  add_garbage(state,value);
  if(node_GetItemsNum(parameters))
  {
    add_class_object_function(value,"service",websockets_service);
    add_class_object_function(value,"broadcast",websockets_broadcast);
    add_class_object_function(value,"broadcast_other",websockets_broadcast_other);
    add_class_object_function(value,"send",websockets_send);
    add_class_object_function(value,"stop",websockets_stop);
    node *port = node_GetItem(parameters,0);
    node *port_value = node_GetItemByKey(port,"value");
    node *protocols = node_GetItem(parameters,1);
    node *protocols_items = node_GetItemByKey(protocols,"items");
    node *wsd_state = node_Create();
    node_AddItem(wsd_state,state);//0
    node_AddItem(wsd_state,block);//1

    unsigned int prots_num = (unsigned int)node_GetItemsNum(protocols_items);
    char *lws_prots = (char*)malloc(sizeof(struct libwebsocket_protocols)*(prots_num+1));
    memset(lws_prots,0,sizeof(struct libwebsocket_protocols)*(prots_num+1));
    char *lws_prot = lws_prots;
    node_ItemIterationReset(protocols_items);
    while(node_ItemIterationUnfinished(protocols_items))
    {
      node *proto = node_ItemIterate(protocols_items);
      struct libwebsocket_protocols *cprot = lws_prot;
      cprot->name = str_Copy(get_obj_name(proto));
      //cprot->name = get_obj_name(proto);
      cprot->callback = callback_nyx_websockets;
      cprot->per_session_data_size = sizeof(struct per_session_data_nyx);
      lws_prot+=sizeof(struct libwebsocket_protocols);
    }
    struct libwebsocket_protocols *cprot = lws_prot;
    cprot->name = NULL;
    cprot->callback = NULL;
    cprot->per_session_data_size = 0;
    
    set_obj_node(value,"value",wsd_state);


    node *daemon = create_class_instance(base_class);
    set_obj_string(daemon,"name","daemon.instance");
    node_AddItem(wsd_state,daemon);//2
    inc_obj_refcount(daemon);
    
    node *state_protocols = node_CopyTree(protocols,True,True);
    node_SetParent(state_protocols,NULL);
    reset_obj_refcount(state_protocols);
    node_AddItem(wsd_state,state_protocols);//3
    inc_obj_refcount(state_protocols);

    node *session_uid = create_class_instance(base_class);
    set_obj_string(session_uid,"name","daemon.session_uid");
    set_obj_int(session_uid,"value",0);
    node_AddItem(wsd_state,session_uid);//4
    inc_obj_refcount(session_uid);

    node *sessions_num = create_class_instance(base_class);
    set_obj_string(sessions_num,"name","daemon.sessions_num");
    set_obj_int(sessions_num,"value",0);
    node_AddItem(wsd_state,sessions_num);//5
    inc_obj_refcount(sessions_num);

    node *sessions = create_class_instance(base_class);
    set_obj_string(sessions,"name","daemon.sessions");
    node *sessions_items = create_obj("items");
    add_obj_kv(sessions,sessions_items);
    node_AddItem(wsd_state,sessions);//6
    inc_obj_refcount(sessions);


    node *daemon_info = create_class_instance(base_class);
    set_obj_string(daemon_info,"name","daemon.info_ptr");
    node_AddItem(wsd_state,daemon_info);//7
    inc_obj_refcount(daemon_info);

    node *daemon_prots = create_class_instance(base_class);
    set_obj_string(daemon_prots,"name","daemon.protocols_ptr");
    node_AddItem(wsd_state,daemon_prots);//8
    inc_obj_refcount(daemon_prots);

    node_AddItem(wsd_state,value);//9 ->this/self
    //inc_obj_refcount(value);

   
    struct lws_context_creation_info *info=(struct lws_context_creation_info *)malloc(sizeof(struct lws_context_creation_info));
    struct libwebsocket_context *context;
    memset(info,0,sizeof(struct lws_context_creation_info));
    info->port = node_GetSint32(port_value);
    info->protocols = lws_prots;
    info->extensions = libwebsocket_get_internal_extensions();
    info->gid = -1;
    info->uid = -1;
    info->user = wsd_state;
    info->options = 0;

    context = libwebsocket_create_context(info);

    set_obj_ptr(daemon,"value",(void*)context);
    set_obj_ptr(daemon_info,"value",(void*)info);
    set_obj_ptr(daemon_prots,"value",(void*)lws_prots);

  }
  return(value);
}

node *websockets_stop(node *state,node *obj,node *block,node *parameters)
{
  //printf("stopping daemon");
  //fflush(stdout);
  node *value = get_true_class(state);

  node *wsd_state = node_GetNode(get_value(obj));
  node *daemon = node_GetItem(wsd_state,2);
  node *daemon_value = node_GetItemByKey(daemon,"value");
  node *state_protocols = node_GetItem(wsd_state,3);
  node *session_uid = node_GetItem(wsd_state,4);
  node *sessions_num = node_GetItem(wsd_state,5);
  node *sessions = node_GetItem(wsd_state,6);
  node *daemon_info = node_GetItem(wsd_state,7);
  node *daemon_info_value = node_GetItemByKey(daemon_info,"value");
  node *daemon_prots = node_GetItem(wsd_state,8);
  node *daemon_prots_value = node_GetItemByKey(daemon_prots,"value");
  struct libwebsocket_context *context = (struct libwebsocket_context *)node_GetValue(daemon_value);
  libwebsocket_context_destroy(context);
  
  dec_obj_refcount(daemon);//TODO move into add_garbage
  add_garbage(state,daemon);
  dec_obj_refcount(state_protocols);
  add_garbage(state,state_protocols);
  dec_obj_refcount(session_uid);
  add_garbage(state,session_uid);
  dec_obj_refcount(sessions_num);
  add_garbage(state,sessions_num);
  dec_obj_refcount(sessions);
  add_garbage(state,sessions);
  dec_obj_refcount(daemon_info);
  add_garbage(state,daemon_info);
  dec_obj_refcount(daemon_prots);
  add_garbage(state,daemon_prots);
  
  char *prots = (char*)node_GetUser(daemon_prots_value);
  node *state_protocols_items = node_GetItemByKey(state_protocols,"items");
  long prots_num = node_GetItemsNum(state_protocols_items);
  long prots_i = 0;
  while(prots_i<=prots_num-1)
  {
    struct libwebsocket_protocols *cprot = prots;
    free(cprot->name);
    prots+=sizeof(struct libwebsocket_protocols);
    prots_i++;
  }

  
  free(node_GetValue(daemon_info_value));
  free(node_GetValue(daemon_prots_value));


  node_ClearItems(wsd_state);
  node_FreeTree(wsd_state);
  node_SetUser(get_value(obj),NULL);
  return(value);
}

node *websockets_service(node *state,node *obj,node *block,node *parameters)
{
  node *value = get_true_class(state);
  node *wsd_state = node_GetNode(get_value(obj));
  node *daemon = node_GetItem(wsd_state,2);
  node *daemon_value = node_GetItemByKey(daemon,"value");
  struct libwebsocket_context *context = (struct libwebsocket_context*)node_GetValue(daemon_value);
  libwebsocket_service(context,10);
  //set_obj_string(value,"name","success");
  return(value);
}

#endif

