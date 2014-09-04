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

#include "http.h"

#ifdef USE_HTTP
void http_binding_open(node *state)
{
  node *modules = get_modules(state);
  node *base_class = get_base_class(state);
  node *block_class = get_block_class(state);
  node *http = http_bind(modules);
  node *proxy = create_proxy_object(state,http,"http");
  inc_obj_refcount(http);
  add_member(block_class,proxy);
  inc_obj_refcount(proxy);
}

void http_binding_close(node *state)
{

}

node *http_bind(node *class)
{
  node *http = http_create_class_object();
  add_member(class,http);
  inc_obj_refcount(http);
  return(http);
}

node *http_create_class_object(void)
{
  node *base = create_base_obj_layout("http");
  add_class_object_function(base,"=",nyxh_assign);
  add_class_object_function(base,"parse_answer",http_parse_answer);
  add_class_object_function(base,"parse_url",http_parse_url);
  add_class_object_function(base,"create_response",http_create_request);
  add_class_object_function(base,"create_request",http_create_request);
  add_class_object_function(base,"parse_cookies",http_parse_cookies);
  add_class_object_function(base,"query",http_query);
  return(base);
}

node *http_create_response(node *state,node *obj,node *block,node *parameters)
{

  /*
  HTTP/1.1 200 OK
  Date: Thu, 15 Jul 2004 19:20:21 GMT
  Server: Apache/1.3.5 (Unix)
  Accept-Ranges: bytes
  Content-length: 46
  Connection: close
  Content-type: text/html

  <h1>Antwort</h1>
  <p>Ich bin eine Antwort</p>
  */
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  set_obj_string(value,"name","http.request");
  add_garbage(state,value);
  node *real_value = get_value(value);
  char *request=str_CreateEmpty();
  request=str_CatFree(request,"HTTP/1.1");

  node_SetString(real_value,request);//TODO add node_SetStringWithoutCopy
  free(request);
  return(value);
}


node *http_create_request(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  set_obj_string(value,"name","http.request");
  add_garbage(state,value);
  node *real_value = get_value(value);
  /*"GET /aluria/client/clients.json HTTP/1.1\r\nHost: openstrike.de\r\nUser-Agent: Mozilla/4.0\r\nAccept: * /*\r\nConnection: close\r\n\r\n"*/
  char *request = str_CreateEmpty();
  if(node_GetItemsNum(parameters))
  {
    node *host = node_GetItem(parameters,0);
    node *method = get_object(parameters,"method");
    node *port = get_object(parameters,"port");
    node *protocol = get_object(parameters,"protocol");
    node *uri = get_object(parameters,"uri");
    node *body = get_object(parameters,"body");
    node *agent = get_object(parameters,"agent");
    node *host_value = get_value(host);
    if(method!=NULL)
    {
      node *method_value = get_value(method);
      request = str_CatFree(request,node_GetString(method_value));
    }
    else
      request = str_CatFree(request,"GET");
    request = str_CatFree(request," /");
    if(uri!=NULL)
    {
      node *uri_value = get_value(uri);
      request = str_CatFree(request,node_GetString(uri_value));
    }

    request = str_CatFree(request," HTTP/1.1\r\nHost: ");
    request = str_CatFree(request,node_GetString(host_value));

  }
  node_SetString(real_value,request);
  return(value);
}


node *http_parse_cookies(node *state,node *obj,node *block,node *parameters)
{

}


node *http_parse_url(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value2 = NULL;
  node *value = create_class_instance(base_class);
  set_obj_string(value,"name","http.url");
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *items = create_obj("items");
  add_obj_kv(value,items);

  if(node_GetItemsNum(parameters))
  {
    value2 = node_GetItem(parameters,0);
  }
  else if(obj != block)
  { 
    value2 = obj;
  }
  else
  {
    value2 = create_class_instance(base_class);
    add_garbage(state,value2);
    set_obj_string(value2,"value","");
  }
  node *real_value2 = get_value(value2);
  char *url = node_GetString(real_value2);
  long url_len = strlen(url);
  if(!url_len)
    return(value);



  long ai = 0;
  node *nprot = create_class_instance(base_class);
  inc_obj_refcount(nprot);
  set_obj_string(nprot,"name","protocol");
  set_obj_string(nprot,"value","http");
  node_AddItem(items,nprot);
  set_obj_int(nprot,"item_index",ai);
  ai++;
  node *nhost = create_class_instance(base_class);
  inc_obj_refcount(nhost);
  set_obj_string(nhost,"name","host");
  node_AddItem(items,nhost);
  set_obj_int(nhost,"item_index",ai);
  ai++;
  node *nport = create_class_instance(base_class);
  inc_obj_refcount(nport);
  set_obj_string(nport,"name","port");
  set_obj_int(nport,"value",80);
  node_AddItem(items,nport);
  set_obj_int(nport,"item_index",ai);
  ai++;
  node *nuri = create_class_instance(base_class);
  inc_obj_refcount(nuri);
  set_obj_string(nuri,"name","uri");
  set_obj_string(nuri,"value","");
  node_AddItem(items,nuri);
  set_obj_int(nuri,"item_index",ai);
  ai++;
  node *npars = create_class_instance(base_class);
  inc_obj_refcount(npars);
  set_obj_string(npars,"name","parameters");
  set_obj_string(npars,"value","");
  node_AddItem(items,npars);
  set_obj_int(npars,"item_index",ai);
  ai++;
  node *npath = create_class_instance(base_class);
  inc_obj_refcount(npath);
  set_obj_string(npath,"name","path");
  set_obj_string(npath,"value","");
  node_AddItem(items,npath);
  set_obj_int(npath,"item_index",ai);
  ai++;
  node *nfile = create_class_instance(base_class);
  inc_obj_refcount(nfile);
  set_obj_string(nfile,"name","file");
  set_obj_string(nfile,"value","");
  node_AddItem(items,nfile);
  set_obj_int(nfile,"item_index",ai);
  ai++;


  long next = 0;
  long prot_end = str_IndexOf(url,"://");
  if(prot_end != -1)  //has protocol
  {
    char *prot = str_Sub(url,0,prot_end);
    set_obj_string(nprot,"value",prot);
    free(prot);
    next = prot_end+3;
  }

  int has_port = 0;
  int has_path = 0;
  long host_end = str_IndexOf(url+next,":");
  if(host_end != -1)  
    has_port = 1;
  if(host_end == -1)
  {
    host_end = str_IndexOf(url+next,"/");
    if(host_end != -1)
      has_path = 1;
  }

  if(str_IndexOf(url+next,"/") != -1)
    has_path = 1;

  char *host = str_Sub(url,next,host_end);
  set_obj_string(nhost,"value",host);
  free(host);
  next = next + host_end + 1;
  if(has_port)
  {
    long port_end = str_IndexOf(url+next,"/");
    char *port = str_Sub(url,next,port_end);
    set_obj_int(nport,"value",atoi(port));
    free(port);
    next = next + port_end+1;
  }

  if(has_path)
  {
    char *path = str_Sub(url,next,-1);

    set_obj_string(nuri,"value",path);

    //split path filename and parameters
    //first cut off parameters
    long path_end = str_IndexOf(url+next,"?");
    char *path_sub = str_Sub(url,next,path_end);

    if(path_end != -1) //found parameters
    {
      char *url_parameters = str_Sub(url,next+path_end+1,-1);
      set_obj_string(npars,"value",url_parameters);
      free(url_parameters);
    }

    long file_begin = str_LastIndexOf(path_sub,"/");

    if(file_begin != -1)
    {
      char *path_only = str_Sub(path_sub,0,file_begin);
      set_obj_string(npath,"value",path_only);
      free(path_only);
    }

    char *filename = str_Sub(path_sub,file_begin+1,-1);
    free(path_sub);

    set_obj_string(nfile,"value",filename);
    free(filename);
    free(path);
    next = next + path_end+1;
  }
  return(value);
}


node *http_parse_answer(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value2 = NULL;
  node *value = create_class_instance(base_class);
  set_obj_string(value,"name","http.answer");
  reset_obj_refcount(value);
  add_garbage(state,value);
  node *items = create_obj("items");
  add_obj_kv(value,items);

  if(node_GetItemsNum(parameters))
  {
    value2 = node_GetItem(parameters,0);
  }
  else if(obj != block)
  { 
    value2 = obj;
  }
  else
  {
    value2 = create_class_instance(base_class);
    add_garbage(state,value2);
    set_obj_string(value2,"value","");
  }
  node *real_value2 = get_value(value2);
  char *answer = node_GetString(real_value2);
  long answer_len = strlen(answer);
  if(!answer_len)
    return(value);
  long i = 0;
  long last_line_start = 0;
  long last_line_len = 0;
  while(i<answer_len)
  {
    char c = answer[i];
    if(c=='\r' || c == '\n')
    {
      if(i-last_line_start)
      {
        char *line = str_Sub(answer,last_line_start,i-last_line_start);
        free(line);
      }
      if(last_line_len==0 && (i-last_line_start)==0)
      {
        i++;
        break;
      }
      last_line_len = i-last_line_start;
      last_line_start=i+1;
    }
    i++;
  }
  char *body = str_Sub(answer,i+1,answer_len-i-1);


  char *header = str_Sub(answer,0,i);
  node *nheader = create_class_instance(base_class);
  inc_obj_refcount(nheader);
  set_obj_string(nheader,"name","header");
  set_obj_string(nheader,"value",header);
  node_AddItem(items,nheader);
  set_obj_int(nheader,"item_index",0);
  free(header);


  node *nbody = create_class_instance(base_class);
  inc_obj_refcount(nbody);
  set_obj_string(nbody,"name","body");
  set_obj_string(nbody,"value",body);
  node_AddItem(items,nbody);
  set_obj_int(nbody,"item_index",1);
  free(body);
  return(value);
}

void http_append_query_array_item(node *state,node *array,char *value)
{
  long value_len = strlen(value);
  long offset =0;
  char *val=str_CreateEmpty();
  char *key=str_CreateEmpty();
  int in_key=1;
  node *base_class = get_base_class(state);
  while(offset<value_len)
  {
    char vc=value[offset];
    if(vc=='=')
    {
      in_key=0;
      offset++;
      if(offset>=value_len)
        break;
      vc=value[offset];
    }
    if(in_key)
      key=str_AddChar(key,vc);
    else
      val=str_AddChar(val,vc);
    offset++;
  }
  node *item = create_class_instance(base_class);
  node_AddItem(array,item);
  node_SetParent(item,array);
  inc_obj_refcount(item);
  set_obj_string(item,"name",key);
  set_obj_string(item,"value",val);
  free(key);
  free(val);
}

void http_append_query_array(node *state,node *value)
{
  char *query = getenv("QUERY_STRING");
  if(query==NULL || !strlen(query))
    return;
  node *items = create_obj("items");
  add_obj_kv(value,items);
  long query_len = strlen(query);
  long offset =0;
  char *val=str_CreateEmpty();
  while(offset<query_len)
  {
    char qc=query[offset];
    if(qc=='&')
    {
      http_append_query_array_item(state,items,val);
      free(val);
      val = str_CreateEmpty();
    }
    else
      val=str_AddChar(val,qc);
    offset++;
  }
  if(strlen(val))
    http_append_query_array_item(state,items,val);
  free(val);
}

node *http_query(node *state,node *obj,node *block,node *parameters)
{
  //returns http query vars as array
  node *base_class = get_base_class(state);
  node *value = NULL;
  if(node_GetItemsNum(parameters))
  {
    value = node_GetItem(parameters,0);
  }
  else if(obj != block)
  { 
    value = obj;
  }
  else
  {
    value = create_class_instance(base_class);
    add_garbage(state,value);
  }
  http_append_query_array(state,value);
  return(value);
}


#endif

