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

/*
int microhttpd_answer(void *cls,struct MHD_Connection *connection,const char *url,const char *method,const char *version,const char *upload_data, size_t *upload_data_size, void **ptr)
{
  struct MHD_Response *response;
  int ret;
  node *mhd_state = (node*)cls;
  node *state = node_GetItem(mhd_state,0);
  node *read_block = node_GetItem(mhd_state,1);
  node *block = node_GetItem(mhd_state,2);
  node *base_class = node_GetItemByKey(state,"nyx_object");

  node *parameters = create_obj("parameters");

  node *url_value = create_class_instance(base_class);
  reset_obj_refcount(url_value);
  add_garbage(state,url_value);
  set_obj_string(url_value,"name","url");
  set_obj_string(url_value,"value",(char*)url);
  set_obj_int(url_value,"item_index",0);
  node_AddItem(parameters,url_value);

  node *method_value = create_class_instance(base_class);
  reset_obj_refcount(method_value);
  add_garbage(state,method_value);
  set_obj_string(method_value,"value",(char*)method);
  set_obj_string(method_value,"name","method");
  set_obj_int(method_value,"item_index",1);
  node_AddItem(parameters,method_value);

  node *version_value = create_class_instance(base_class);
  reset_obj_refcount(version_value);
  add_garbage(state,version_value);
  set_obj_string(version_value,"name","version");
  set_obj_string(version_value,"value",(char*)version);
  set_obj_int(version_value,"item_index",2);
  node_AddItem(parameters,version_value);

  node *upload_value = create_class_instance(base_class);
  reset_obj_refcount(upload_value);
  add_garbage(state,upload_value);
  char *uploads = (char*)malloc(*upload_data_size+1);
  memset(uploads+*upload_data_size + 0, 0, 1);
  memcpy(uploads,upload_data,*upload_data_size);
  set_obj_string(upload_value,"name","upload");
  set_obj_string(upload_value,"value",uploads);
  set_obj_int(upload_value,"item_index",3);
  node_AddItem(parameters,upload_value);

  node *ret_obj = execute_obj(state,read_block,block,parameters,True,False);
  node *ret_obj_value = node_GetItemByKey(ret_obj,"value");
  char *me = node_GetString(ret_obj_value);

  response = MHD_create_response_from_buffer(strlen(me),(void *)me,MHD_RESPMEM_PERSISTENT);
  ret = MHD_queue_response(connection,MHD_HTTP_OK,response);
  MHD_destroy_response(response);
  return(ret);
}
*/


//#define LOCAL_RESOURCE_PATH "nyx/tests/websockets/server"
#define LOCAL_RESOURCE_PATH "websockets-test"
char *resource_path = LOCAL_RESOURCE_PATH;



#define MAX_ECHO_PAYLOAD 1400

struct per_session_data__http {
  int fd;
};

const char * get_mimetype(const char *file)
{
  int n = strlen(file);

  if (n < 5)
    return NULL;

  if (!strcmp(&file[n - 4], ".ico"))
    return "image/x-icon";

  if (!strcmp(&file[n - 4], ".png"))
    return "image/png";

  if (!strcmp(&file[n - 5], ".html"))
    return "text/html";

  return NULL;
}

static int callback_http(struct libwebsocket_context *context,struct libwebsocket *wsi,enum libwebsocket_callback_reasons reason, void *user,void *in, size_t len)
{
  char buf[256];
  char leaf_path[1024];
  char b64[64];
  struct timeval tv;
  int n, m;
  unsigned char *p;
  char *other_headers;
  static unsigned char buffer[4096];
  struct stat stat_buf;
  struct per_session_data__http *pss = (struct per_session_data__http *)user;

  //if(wsi && wsi->protocol && wsi->protocol->name)
  //  printf("prot:%s\n",wsi->protocol->name);

  const char *mimetype;
  switch (reason) {
  case LWS_CALLBACK_HTTP:
    if (len < 1) {
      libwebsockets_return_http_status(context, wsi,
            HTTP_STATUS_BAD_REQUEST, NULL);
      return -1;
    }

    /* this server has no concept of directories */
    if (strchr((const char *)in + 1, '/')) {
      libwebsockets_return_http_status(context, wsi,
            HTTP_STATUS_FORBIDDEN, NULL);
      return -1;
    }

    /* if a legal POST URL, let it continue and accept data */
    if (lws_hdr_total_length(wsi, WSI_TOKEN_POST_URI))
      return 0;

    /* check for the "send a big file by hand" example case */

    if (!strcmp((const char *)in, "/leaf.jpg")) {
      if (strlen(resource_path) > sizeof(leaf_path) - 10)
        return -1;
      sprintf(leaf_path, "%s/leaf.jpg", resource_path);

      /* well, let's demonstrate how to send the hard way */

      p = buffer;

#ifdef WIN32
      pss->fd = open(leaf_path, O_RDONLY | _O_BINARY);
#else
      pss->fd = open(leaf_path, O_RDONLY);
#endif

      if (pss->fd < 0)
        return -1;

      fstat(pss->fd, &stat_buf);

      /*
       * we will send a big jpeg file, but it could be
       * anything.  Set the Content-Type: appropriately
       * so the browser knows what to do with it.
       */

      p += sprintf((char *)p,
        "HTTP/1.0 200 OK\x0d\x0a"
        "Server: libwebsockets\x0d\x0a"
        "Content-Type: image/jpeg\x0d\x0a"
          "Content-Length: %u\x0d\x0a\x0d\x0a",
          (unsigned int)stat_buf.st_size);

      /*
       * send the http headers...
       * this won't block since it's the first payload sent
       * on the connection since it was established
       * (too small for partial)
       */

      n = libwebsocket_write(wsi, buffer,
           p - buffer, LWS_WRITE_HTTP);

      if (n < 0) {
        close(pss->fd);
        return -1;
      }
      /*
       * book us a LWS_CALLBACK_HTTP_WRITEABLE callback
       */
      libwebsocket_callback_on_writable(context, wsi);
      break;
    }

    /* if not, send a file the easy way */
    strcpy(buf, resource_path);
    if (strcmp(in, "/")) {
      if (*((const char *)in) != '/')
        strcat(buf, "/");
      strncat(buf, in, sizeof(buf) - strlen(resource_path));
    } else /* default file to serve */
      strcat(buf, "/test.html");
    buf[sizeof(buf) - 1] = '\0';

    /* refuse to serve files we don't understand */
    mimetype = get_mimetype(buf);
    if (!mimetype) {
      lwsl_err("Unknown mimetype for %s\n", buf);
      libwebsockets_return_http_status(context, wsi,
              HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE, NULL);
      return -1;
    }

    /* demostrates how to set a cookie on / */

    other_headers = NULL;
    if (!strcmp((const char *)in, "/") &&
         !lws_hdr_total_length(wsi, WSI_TOKEN_HTTP_COOKIE)) {
      /* this isn't very unguessable but it'll do for us */
      gettimeofday(&tv, NULL);
      sprintf(b64, "LWS_%u_%u_COOKIE",
        (unsigned int)tv.tv_sec,
        (unsigned int)tv.tv_usec);

      sprintf(leaf_path,
        "Set-Cookie: test=LWS_%u_%u_COOKIE;Max-Age=360000\x0d\x0a",
          (unsigned int)tv.tv_sec, (unsigned int)tv.tv_usec);
      other_headers = leaf_path;
      lwsl_err(other_headers);
    }

    if (libwebsockets_serve_http_file(context, wsi, buf,
            mimetype, other_headers))
      return -1; /* through completion or error, close the socket */

    /*
     * notice that the sending of the file completes asynchronously,
     * we'll get a LWS_CALLBACK_HTTP_FILE_COMPLETION callback when
     * it's done
     */

    break;

  case LWS_CALLBACK_HTTP_BODY:
    strncpy(buf, in, 20);
    buf[20] = '\0';
    if (len < 20)
      buf[len] = '\0';

    lwsl_notice("LWS_CALLBACK_HTTP_BODY: %s... len %d\n",
        (const char *)buf, (int)len);

    break;

  case LWS_CALLBACK_HTTP_BODY_COMPLETION:
    lwsl_notice("LWS_CALLBACK_HTTP_BODY_COMPLETION\n");
    /* the whole of the sent body arried, close the connection */
    libwebsockets_return_http_status(context, wsi,
            HTTP_STATUS_OK, NULL);

    return -1;

  case LWS_CALLBACK_HTTP_FILE_COMPLETION:
//    lwsl_info("LWS_CALLBACK_HTTP_FILE_COMPLETION seen\n");
    /* kill the connection after we sent one file */
    return -1;

  case LWS_CALLBACK_HTTP_WRITEABLE:
    /*
     * we can send more of whatever it is we were sending
     */

    do {
      n = read(pss->fd, buffer, sizeof buffer);
      /* problem reading, close conn */
      if (n < 0)
        goto bail;
      /* sent it all, close conn */
      if (n == 0)
        goto flush_bail;
      /*
       * because it's HTTP and not websocket, don't need to take
       * care about pre and postamble
       */
      m = libwebsocket_write(wsi, buffer, n, LWS_WRITE_HTTP);
      if (m < 0)
        /* write failed, close conn */
        goto bail;
      if (m != n)
        /* partial write, adjust */
        lseek(pss->fd, m - n, SEEK_CUR);

      if (m) /* while still active, extend timeout */
        libwebsocket_set_timeout(wsi,
          PENDING_TIMEOUT_HTTP_CONTENT, 5);

    } while (!lws_send_pipe_choked(wsi));
    libwebsocket_callback_on_writable(context, wsi);
    break;
flush_bail:
    /* true if still partial pending */
    if (lws_send_pipe_choked(wsi)) {
      libwebsocket_callback_on_writable(context, wsi);
      break;
    }

bail:
    close(pss->fd);
    return -1;

  /*
   * callback for confirming to continue with client IP appear in
   * protocol 0 callback since no websocket protocol has been agreed
   * yet.  You can just ignore this if you won't filter on client IP
   * since the default uhandled callback return is 0 meaning let the
   * connection continue.
   */

  case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:
    /* if we returned non-zero from here, we kill the connection */
    break;

  case LWS_CALLBACK_GET_THREAD_ID:
    break;

  default:
    break;
  }

  return 0;
}


/*struct per_session_data__echo {
  unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + MAX_ECHO_PAYLOAD + LWS_SEND_BUFFER_POST_PADDING];
  unsigned int len;
  unsigned int index;
};*/

node *create_session(node *state,node *sessions,long session_uid)
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

struct per_session_data__echo {
  node *session;
};


static int callback_echo(struct libwebsocket_context *context,struct libwebsocket *wsi,enum libwebsocket_callback_reasons reason, void *user,void *in,size_t len)
{
  struct per_session_data__echo *pss =(struct per_session_data__echo*)user;
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
      if (len < 1)
      {
        libwebsockets_return_http_status(context,wsi,HTTP_STATUS_BAD_REQUEST,NULL);
        return -1;
      }

    case LWS_CALLBACK_SERVER_WRITEABLE:
      {
        node *message = get_member(pss->session,"message");
        node *session_id = get_member(pss->session,"id");
        node *session_id_value = node_GetItemByKey(session_id,"value");
        if(message)
        {
          node *message_value = node_GetItemByKey(message,"value");
          char *me = node_GetString(message_value);
          printf("sending message now: [%s] to: %d\n",me,node_GetSint32(session_id_value));
          fflush(stdout);
          n = libwebsocket_write(wsi, me, strlen(me), LWS_WRITE_TEXT);
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
      pss->session = create_session(state,sessions,lsession_uid);
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
    if (len > MAX_ECHO_PAYLOAD) {
      lwsl_err("Server received packet bigger than %u, hanging up\n", MAX_ECHO_PAYLOAD);
      return 1;
    }
    /*memcpy(&pss->buf[LWS_SEND_BUFFER_PRE_PADDING], in, len);
    pss->len = (unsigned int)len;
    libwebsocket_callback_on_writable(context, wsi);
    */
    if(found_prot)
    {
      node *parameters = create_obj("parameters");
      node *base_class = get_base_class(state);
      
      node *prot_value = create_class_instance(base_class);
      //reset_obj_refcount(prot_value);
      //add_garbage(state,prot_value);
      set_obj_string(prot_value,"name","protocol");
      set_obj_string(prot_value,"value",get_obj_name(found_prot));
      //set_obj_int(prot_value,"item_index",0);
      node_AddItem(parameters,prot_value);
      inc_obj_refcount(prot_value);
      //node_SetParent(prot_value,para)

      char *msg = str_CreateEmpty();
      msg = str_AddChars(msg,in,len);
      node *msg_value = create_class_instance(base_class);
      //reset_obj_refcount(msg_value);
      //add_garbage(state,msg_value);//TODO check if "just survives"
      set_obj_string(msg_value,"name","message");
      set_obj_string(msg_value,"value",msg);
      //set_obj_int(msg_value,"item_index",1);
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
        printf("returning message: [%s] :%d\n",node_GetString(ret_obj_value),strlen(node_GetString(ret_obj_value)));
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
  printf("broadcasting\n");
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
    printf("checking protocols:%s,%s\n",node_GetString(state_protocol_value),node_GetString(protocol_value));
    struct libwebsocket_protocols *cprot = prots;
    if(!strcmp(node_GetString(state_protocol_value),node_GetString(protocol_value)))
    {
      printf("now broadcasting:[%s]\n",node_GetString(message_value));
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
        printf("sending message to uid: %d\n",node_GetSint32(session_uid_value));
        fflush(stdout);
        add_member(session,broadcast_message);
        inc_obj_refcount(broadcast_message);
      }

      libwebsocket_callback_on_writable_all_protocol(cprot);
      node_RemoveItem(wsd_state,broadcast_message);
      dec_obj_refcount(broadcast_message);
      add_garbage(state,broadcast_message);

    }
    prots+=sizeof(struct libwebsocket_protocols);
  }

  fflush(stdout);

  

  return(value);
}

node *websockets_broadcast_other(node *state,node *obj,node *block,node *parameters)
{
  printf("broadcasting_other\n");
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
      printf("now broadcasting:[%s]\n",node_GetString(message_value));
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
        long luid = node_GetSint32(session_uid_value);
        if(luid != lskip_uid)
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
  printf("broadcast_other finished\n");
  fflush(stdout);  
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
  //reset_obj_refcount(value);//TODO remove these after create_class_instance
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
      cprot->callback = callback_echo;
      cprot->per_session_data_size = sizeof(struct per_session_data__echo);
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

