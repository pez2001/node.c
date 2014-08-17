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

void http_bind(node *class)
{
  node *http = http_create_class_object();
  add_member(class,http);
  inc_obj_refcount(http);
}

node *http_create_class_object(void)
{
  node *base = create_base_obj_layout("http");
  add_class_object_function(base,"=",nyxh_assign);
  add_class_object_function(base,"create_request",http_create_request);
  add_class_object_function(base,"parse_answer",http_parse_answer);
  add_class_object_function(base,"query",http_query);
  return(base);
}

node *http_create_request(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = get_base_class(state);
  node *value = create_class_instance(base_class);
  set_obj_string(value,"name","http.request");
  add_garbage(state,value);
  node *real_value = get_value(value);
  node_SetString(real_value,"not supported");
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
  //long header_length = 0;
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
        //printf("line:[%s]\n",line);
        free(line);
      }
      if(last_line_len==0 && (i-last_line_start)==0)
      {
        //printf("header end at:%d\n",i);
        i++;
        break;
      }
      last_line_len = i-last_line_start;
      last_line_start=i+1;
    }
    i++;
  }
  char *body = str_Sub(answer,i+1,answer_len-i-1);
  //printf("body:[%s]\n",body);

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

