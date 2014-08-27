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

#include "sockets.h"

#ifdef USE_SOCKETS

void sockets_binding_open(node *state)
{
  #ifdef WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2),&wsaData);
  #endif
  node *modules = get_modules(state);
  node *base_class = get_base_class(state);
  node *block_class = get_block_class(state);
  node *sockets = sockets_bind(modules);
  node *proxy = create_proxy_object(state,sockets,"socket");
  inc_obj_refcount(sockets);
  add_member(block_class,proxy);
  inc_obj_refcount(proxy);
}

void sockets_binding_close(node *state)
{
  #ifdef WIN32
    WSACleanup();
  #endif
}

node *sockets_bind(node *class)
{
  node *sockets = sockets_create_class_object();
  add_member(class,sockets);
  inc_obj_refcount(sockets);
  return(sockets);
}

node *sockets_create_class_object(void)
{
  node *base = create_base_obj_layout("socket");
  add_class_object_function(base,"=",nyxh_assign);
  add_class_object_function(base,"open",sockets_open);
  add_class_object_function(base,"close",sockets_close);
  add_class_object_function(base,"write",sockets_write);
  add_class_object_function(base,"read",sockets_read);
  add_class_object_function(base,"set_address",sockets_set_address);
  add_class_object_function(base,"connect",sockets_connect);
  node *sock_dgram_const = create_base_obj_layout("SOCK_DGRAM");
  node *sock_stream_const = create_base_obj_layout("SOCK_STREAM");
  add_member(base,sock_dgram_const);
  inc_obj_refcount(sock_dgram_const);
  add_member(base,sock_stream_const);
  inc_obj_refcount(sock_stream_const);
  return(base);
}

node *sockets_open(node *state,node *obj,node *block,node *parameters)
{
  //returns socket stream object
  node *base_class = get_base_class(state);
  node *value = sockets_create_class_object();
  //node_SetParent(value,NULL);
  //reset_obj_refcount(value);
  add_garbage(state,value);
  set_obj_int(value,"value",0);

  node *is_connected = create_class_instance(base_class);
  add_garbage(state,is_connected);
  set_obj_string(is_connected,"name","is_connected");
  set_obj_int(is_connected,"value",0);
  add_member(value,is_connected);
  inc_obj_refcount(is_connected);

  if(node_GetItemsNum(parameters))
  {
    //node *nprotocol = NULL;
    node *ntype = NULL;
    //node *ndomain = NULL;
    //node *real_ndomain = NULL;
    //node *real_nprotocol = NULL;
    ntype = node_GetItem(parameters,0);
    //if(node_GetItemsNum(parameters)>=1)
    //  ndomain = node_GetItem(parameters,1);
    //if(node_GetItemsNum(parameters)>=2)
    //  nprotocol = node_GetItem(parameters,2);
    node *real_ntype = node_GetItemByKey(ntype,"name");
    //if(ndomain!=NULL)
    //  real_ndomain = node_GetItemByKey(ndomain,"name");
    //if(nprotocol!=NULL)
    //  real_nprotocol = node_GetItemByKey(nprotocol,"name");
    if(node_GetType(real_ntype)==NODE_TYPE_STRING)
    {
      int type = SOCK_STREAM;
      int domain = PF_INET;
      int protocol = 0;
      char *stype = node_GetString(real_ntype);
      if(!strcmp(stype,"SOCK_DGRAM"))
        type = SOCK_DGRAM;
      int sock = socket(domain,type,protocol);
      node *privates = node_GetItemByKey(value,"privates");
      //set_obj_int(value,"socket_handle",(long)sock);
      //set_obj_int(value,"socket_type",(long)type);
      set_obj_int(privates,"socket_handle",(long)sock);
      set_obj_int(privates,"socket_type",(long)type);
      set_obj_int(privates,"socket_is_connected",0);

      //printf("socket_handle opened:%d\n",sock);
    }
  }
  return(value);
}

node *sockets_close(node *state,node *obj,node *block,node *parameters)
{
  //close a socket
  node *value = get_false_class(state);
  node *privates = node_GetItemByKey(obj,"privates");
  node *handle = node_GetItemByKey(privates,"socket_handle");
  if(handle!=NULL)
  {
    node *is_connected = get_member_non_recursive(obj,"is_connected");
    int socket_handle = (int)node_GetSint32(handle);
    //printf("socket_handle to close:%d\n",socket_handle);
    #ifdef WIN32 
      int ret = closesocket(socket_handle);
    #else  
      int ret = close(socket_handle);
    #endif
    if(ret>0)
    	value=get_true_class(state);
    set_obj_int(is_connected,"value",0);
  }
  /*if(strcmp(get_obj_name(obj),"socket"))
  {
    return(obj);
  }*/
  return(value);
}

node *sockets_set_address(node *state,node *obj,node *block,node *parameters)
{
  //set address of stream object
  node *value = get_false_class(state);
  node *privates = node_GetItemByKey(obj,"privates");
  node *socket_handle = node_GetItemByKey(privates,"socket_handle");

  if(socket_handle!=NULL)
  {
    //int sock = node_GetValue(socket_handle);
    if(node_GetItemsNum(parameters))
    {
      node *nhost = node_GetItem(parameters,0);
      node *nport = node_GetItem(parameters,1);
      node *real_nhost = node_GetItemByKey(nhost,"value");
      node *real_nport = node_GetItemByKey(nport,"value");
      if(node_GetType(real_nhost)==NODE_TYPE_STRING && node_GetType(real_nport)==NODE_TYPE_SINT32)
      { 
        struct sockaddr_in addr;
        char *host = node_GetString(real_nhost);
        int port = node_GetSint32(real_nport);
        addr.sin_family = AF_INET;
        addr.sin_port = port;
        addr.sin_addr.s_addr = inet_addr(host);
        //addr.sin_zero = 0;
        set_obj_ptr(privates,"socket_addr",&addr);
        value = get_true_class(state);
      }
    }
  }
  return(value);
}

node *sockets_connect(node *state,node *obj,node *block,node *parameters)
{
  //connect stream object to some host and port
  node *value = get_false_class(state);

  node *privates = node_GetItemByKey(obj,"privates");
  node *socket_handle = node_GetItemByKey(privates,"socket_handle");
  node *socket_type = node_GetItemByKey(privates,"socket_type");
  set_obj_int(obj,"value",0);
  if(socket_handle!=NULL && socket_type != NULL)
  {
    int sock = (int)node_GetSint32(socket_handle);
    int type = (int)node_GetSint32(socket_type);
    node *is_connected = get_member_non_recursive(obj,"is_connected");
    if(node_GetItemsNum(parameters))
    {
      node *nhost = node_GetItem(parameters,0);
      node *nport = node_GetItem(parameters,1);
      node *real_nhost = node_GetItemByKey(nhost,"value");
      node *real_nport = node_GetItemByKey(nport,"value");
      if(node_GetType(real_nhost)==NODE_TYPE_STRING && node_GetType(real_nport)==NODE_TYPE_SINT32)
      { 
        struct addrinfo hints;
        struct addrinfo *result;
        int r;
        char *host = node_GetString(real_nhost);
        int port = node_GetSint32(real_nport);
        char *sport = str_FromLong(port);
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = type;
        hints.ai_flags = AI_PASSIVE;
        hints.ai_protocol = 0;
        hints.ai_canonname = NULL;
        hints.ai_addr = NULL;
        hints.ai_next = NULL;


        r = getaddrinfo(host,sport,&hints,&result);
        if(r==0)
        {
          //printf("host:%s found!:%d\n",host,sock);
          struct addrinfo *test=result;
          while(test!=NULL)
          {
            /*struct timeval tv;
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            if(setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char*)&tv,sizeof(tv)))
            { 
              perror("setsockopt");
              return(value); 
            }*/ 
            /*int timeout=10;
            if(setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout)))
            { 
              perror("setsockopt");
              return(value); 
            }*/             
            if(connect(sock,test->ai_addr,test->ai_addrlen)!=-1)
            {
              set_obj_int(is_connected,"value",1);
              //printf("connected to host\n");
              unsigned long one = 1;
              if(ioctlsocket(sock,FIONBIO,&one)==SOCKET_ERROR)
              {
                printf("ioctlsocket error\n");
                return(value); 
              }
              /*int timeout=0;
              if(setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout)))
              { 
                perror("setsockopt");
                return(value); 
              }*/              
              //set_obj_int(value,"value",1);
              value = get_true_class(state);
              set_obj_int(obj,"value",1);
              break;
            }
            //printf("reopening socket\n");
            #ifdef WIN32 
              int ret = closesocket(sock);
            #else  
              int ret = close(sock);
            #endif
              if(ret==-1)
              {
                set_obj_int(obj,"value",1);
                break;
              }
            sock = socket(test->ai_family,type,test->ai_protocol);
            node_SetSint32(socket_handle,sock);
            //printf("updated socket handle\n");
            test=test->ai_next;
          }

          if(test!=NULL && result!=NULL)
            freeaddrinfo(result);
        }
        else
        {
          printf("host:%s not found:%d\n",host,r);
        }
        free(sport);
      }
    }
  }
  return(value);
}

node *sockets_read(node *state,node *obj,node *block,node *parameters)
{
  //returns string with data received
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  add_garbage(state,value);
  node *real_value = get_value(value);
  node *privates = node_GetItemByKey(obj,"privates");
  node *nsocket_handle = node_GetItemByKey(privates,"socket_handle");
  //node *ntype = node_GetItemByKey(fvalue,"socket_type");
  char *ret = str_CreateEmpty();
  if(nsocket_handle!=NULL)
  {
    node *is_connected = get_member_non_recursive(obj,"is_connected");
    int socket_handle = (int)node_GetSint32(nsocket_handle);
    //printf("receiving data:%d\n",socket_handle);
    //fflush(stdout);
    int len = 1023;
    int data_received = 0;
    char buf[1024];
    while(len==1023)
    {
      memset(buf,0,1024);
      len = recv(socket_handle,(char*)&buf,1023,0);
      data_received+=len;
      //printf("recv len:%d\n",len);
      //fflush(stdout);
      if(len<0)
      {
        //printf("recv error:%d\n",errno);
        //printf("error receiving data:%d\n",len);
        
        //if(errno!=10035 && errno != 0)
          break;
        //else
        //  len=1023;
      }
      if(len==0)
      {
        set_obj_int(is_connected,"value",0);

        //printf("received data:%d\n",data_received);
        break;
      }
      if(len)
        ret=str_CatFree(ret,buf);
    }
    node *value_privates = node_GetItemByKey(value,"privates");
    set_obj_int(value_privates,"socket_handle",(long)socket_handle);
  }
  node_SetString(real_value,ret);
  free(ret);
  return(value);
}

node *sockets_write(node *state,node *obj,node *block,node *parameters)
{
  //writes string to socket
  node *value = get_true_class(state);
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *value2 = NULL;
  if(node_GetItemsNum(parameters))
  {
    value2 = node_GetItem(parameters,0);
  }
  if(obj != block && value2 != NULL)
  { 
    node *real_value = node_GetItemByKey(value2,"value");
    node *privates = node_GetItemByKey(obj,"privates");
    node *nsocket_handle = node_GetItemByKey(privates,"socket_handle");
    if(nsocket_handle!=NULL)
    {
      int socket_handle = (int)node_GetSint32(nsocket_handle);
      char *content = node_GetString(real_value);
      //printf("sending:[%s]\n",content);
      long len = strlen(content);
      int data_send = 0;
      int num = 0;
      while(data_send<len)
      {
        num = send(socket_handle,content+data_send,len-data_send,0);
        if(num==-1)
        {
          //set_obj_int(value,"value",-1);        
          value = get_false_class(state);
          break;
        }
        data_send+=num;
      }
    }
  }
  return(value);
}

#endif

