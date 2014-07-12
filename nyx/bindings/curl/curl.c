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


void bind_curl(node *class)
{
  add_member(class,create_curl_class_object());
}

node *create_curl_class_object(void)
{
  node *base = create_base_obj_layout("curl");
  add_class_object_function(base,"=",nyxh_assign);

  add_class_object_function(base,"create_request",nyxh_http_create_request);
  add_class_object_function(base,"parse_answer",nyxh_http_parse_answer);
  return(base);
}

 
int test(void)
{
  CURL *curl;
  CURLcode res;
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    /* example.com is redirected, so we tell libcurl to follow redirection */ 
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
  return 0;
}


node *nyxh_curl(node *state,node *obj,node *block,node *parameters)
{
  node *base_class = node_GetItemByKey(state,"nyx_object");
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
    reset_obj_refcount(value2);
    add_garbage(state,value2);
    set_obj_string(value2,"value","");
  }
  node *real_value2 = node_GetItemByKey(value2,"value");
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
  reset_obj_refcount(nheader);
  inc_obj_refcount(nheader);
  set_obj_string(nheader,"name","header");
  set_obj_string(nheader,"value",header);
  node_AddItem(items,nheader);
  set_obj_int(nheader,"item_index",0);
  free(header);


  node *nbody = create_class_instance(base_class);
  reset_obj_refcount(nbody);
  inc_obj_refcount(nbody);
  set_obj_string(nbody,"name","body");
  set_obj_string(nbody,"value",body);
  node_AddItem(items,nbody);
  set_obj_int(nbody,"item_index",1);
  free(body);
  return(value);
}
