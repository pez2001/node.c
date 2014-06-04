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
#include "json.h"

char *json_AddCharToString(char *string,char letter)
{ 
  int len=strlen(string);
  string = (char*)realloc(string,len+2);
  string[len+1] = 0;
  string[len] = letter;
  return(string);
}

char *json_CreateEmptyString(void)
{
    char *string = (char*)malloc(1);
    string[0] = 0;
    return(string);
}

char *json_TrimString(char *string)
{
  long len = strlen(string);
  if(!len)
    return(string);
  long s=0;
  long e=len-1;
  while(s<len && (isspace(string[s]) || string[s]=='\t'))s++;
  while(e>0 && (isspace(string[e]) || string[s]=='\t'))e--;
  e=e+1;
  long nlen = e-s;
  if(nlen<=0)
  {
    free(string);
    return(json_CreateEmptyString());
  }
  if(nlen==len)
    return(string);
  char *r = (char*)malloc(nlen+1);
  memcpy(r,string+s,nlen);
  r[nlen] = 0;
  free(string);
  return(r);
}

char json_ConvertEscapeChar(char escape_char)
{
  char r = 0;
  switch(escape_char)
  {
    case 't':
      r = 9;
      break;
    case 'a':
      r = 7;
      break;
    case 'b':
      r = 8;
      break;
    case 'n':
      r = 10;
      break;
    case 'v':
      r = 11;
      break;
    case 'r':
      r = 13;
      break;
    case '"':
      r = 34;
      break;
    case '\'':
      r = 39;
      break;
    case '?':
      r = 63;
      break;
    case '\\':
      r = 92;
      break;
    default:
      r = 0;
  }
  return(r);
}

node *json_CreateNode(char *key_string,char *value_string,int is_value_string)
{
  node *kv = node_Create();
  if(key_string!=NULL)
    node_SetKey(kv,key_string);
  if(is_value_string)
  {
    node_SetString(kv,value_string);
  }
  else
    if(!strcmp(value_string,"null"))
      node_SetNull(kv);
    else
      if(!strcmp(value_string,"true"))
        node_SetBool(kv,True);
      else
        if(!strcmp(value_string,"false"))
          node_SetBool(kv,False);
        else
        {
          node_ParseNumber(kv,value_string);
        }
  return(kv);
}

void json_SetNode(node *n,char *value_string,int is_value_string)
{
  if(is_value_string)
  {
    node_SetString(n,value_string);
  }
  else
    if(!strcmp(value_string,"null"))
      node_SetNull(n);
    else
      if(!strcmp(value_string,"true"))
        node_SetBool(n,True);
      else
        if(!strcmp(value_string,"false"))
          node_SetBool(n,False);
        else
        {
          double d = atof(value_string);
          //node_SetDouble(n,d);/*used for easier fbx parsing - refactor this in*/
          node_ParseNumber(n,value_string);
        }
}

node *json_Load(char *json,unsigned long len)
{
  int state=0;   
  list *obj_stack=list_Create(0,0);
  unsigned long offset=0;
  char *value_string = json_CreateEmptyString();
  int is_value_string=0;
  int found_key = 0;
  //state |= FBX_STATE_IN_VALUE;
  node *new_obj  = NULL;
  node *root_obj = node_Create();
  node_SetType(root_obj,NODE_TYPE_NODE);
  node_SetKey(root_obj,"root");
  list_Push(obj_stack,root_obj);
  //new_obj = root_obj;

  while(offset<len)
  {
    node *actual_obj = (node*)list_GetTop(obj_stack);
    if(actual_obj == NULL) 
    {
       printf("no actual obj set\n");
       free(value_string);
       list_Close(obj_stack);
       if(new_obj!=NULL)
         node_Free(new_obj,True);
       return(root_obj);
    } 


    /*if((state & FBX_STATE_IN_COMMENT))
    {
      if(json[offset]=='\r' || json[offset]=='\n')
      {
      	state &= ~FBX_STATE_IN_COMMENT;
      }	
      offset++;
      continue;
    }*/
    if((state & JSON_STATE_IN_STRING))
    {
      char add_char = json[offset];
      offset++;
      if(add_char == '"')
      {
         state &= ~JSON_STATE_IN_STRING;
         is_value_string = 1;
         continue;
      }
      else
      if(add_char=='\\')
      {
         add_char = json_ConvertEscapeChar(json[offset]);
         offset++;
         if(add_char==0)
          continue;
      }
     value_string = json_AddCharToString(value_string,add_char);
     continue;
    }


    switch(json[offset])
    {
       case 0:
             free(value_string);
             list_Close(obj_stack);
             return(root_obj);
       //case ';':
       //      state |= FBX_STATE_IN_COMMENT;

       case '\t':
       case ' ':
            offset++;
            continue;

       case ']':
            //printf("closed array\n");
       case '}':
            list_Pop(obj_stack);
            //printf("obj above:%s\n",node_GetKey((node*)list_GetTop(obj_stack)));
       case '\n':
       case '\r':
       case ',': 
                 found_key = 0;
       //case '\t':
             if(!is_value_string)
                  value_string = json_TrimString(value_string);
               if((strlen(value_string) || is_value_string) && new_obj!=NULL)
               {
                 //if(actual_obj!=NULL && node_GetType(actual_obj)==NODE_TYPE_ARRAY)
                 if(node_GetType(actual_obj)==NODE_TYPE_ARRAY)
                 {
                   //if(json[offset+1]!=']')
                   //{
                     new_obj = node_Create();
                     json_SetNode(new_obj,value_string,is_value_string);
                     node_array_Add(actual_obj,new_obj);
                     node_SetParent(new_obj,actual_obj);
                     //printf("created new array obj\n");
                   //node_Print(new_obj,True);
                   //}
                 }
                 else
                   json_SetNode(new_obj,value_string,is_value_string);
                 found_key = 0;

                 free(value_string);
                 value_string=json_CreateEmptyString();
               }
            is_value_string=0;
            offset++;
            continue;
       case '{':
            //printf("new obj\n");

            //if(actual_obj!=NULL && node_GetType(actual_obj)==NODE_TYPE_ARRAY)
            if(node_GetType(actual_obj)==NODE_TYPE_ARRAY)
            {
              new_obj = node_Create();
              node_SetType(new_obj,NODE_TYPE_NODE);
              node_SetParent(new_obj,actual_obj);
              node_array_Add(actual_obj,new_obj);
              //printf("created new obj for array\n");
            }
            else
            {
              if(!found_key && actual_obj!=root_obj)
              {
                //printf("found no key for obj\n");
                new_obj = node_Create();
                //node_SetKey(new_obj,value_string);
                node_SetType(new_obj,NODE_TYPE_NODE);
                node_SetParent(new_obj,actual_obj);
                //if(actual_obj!=NULL)
                  node_AddItem(actual_obj,new_obj);
              }
            }
            if(new_obj != NULL)
              list_Push(obj_stack,new_obj);
            offset++;
            continue;
       /*
       case '}':
            printf("closed obj\n");
            list_Pop(obj_stack);
            offset++;
            continue;
      */
       case '[':
            if(!found_key)
            {
              //printf("found no key for array\n");
              new_obj = node_Create();
              //node_SetKey(new_obj,value_string);
              node_SetType(new_obj,NODE_TYPE_NODE);
              node_SetParent(new_obj,actual_obj);
              //if(actual_obj!=NULL)
                node_AddItem(actual_obj,new_obj);
            }
            //printf("new array\n");
            node_SetArray(new_obj,0);
            list_Push(obj_stack,new_obj);
            /*actual_obj = new_obj;
            new_obj = node_Create();
            node_SetType(new_obj,NODE_TYPE_NODE);
            //node_SetKey(new_obj,value_string);
            node_SetParent(new_obj,actual_obj);
            //node_AddItem(actual_obj,new_obj);
            node_array_Add(actual_obj,new_obj);
            printf("created new obj for array\n");*/
            offset++;
            continue;
      /*
       case ']':
            printf("closed array\n");
            list_Pop(obj_stack);
            offset++;
            continue;
       */

       case ':':
       case '=':
            is_value_string = 0;
            value_string = json_TrimString(value_string);
            //printf("new obj @:%x\n",&new_obj);
            //node_SetArray(new_obj,0);
            new_obj = node_Create();
            node_SetType(new_obj,NODE_TYPE_NODE);
            node_SetKey(new_obj,value_string);
            node_SetParent(new_obj,actual_obj);
            //if(actual_obj!=NULL)
              node_AddItem(actual_obj,new_obj);
            //printf("created new obj\n");
            //node_Print(new_obj,True);
            if(strlen(value_string))
            {
              free(value_string);
              value_string = json_CreateEmptyString();
            }
            found_key = 1;
            offset++;
            continue;
       case '"':
            state |= JSON_STATE_IN_STRING;
            value_string = json_TrimString(value_string);
            if(strlen(value_string))
            { 
              free(value_string);
              value_string = json_CreateEmptyString();
            }
            offset++;
            continue;
    }
    //if(!is_value_string)
    //{
      value_string = json_AddCharToString(value_string,json[offset]);
    //} 
    offset++;
  } 

 free(value_string);
 list_Close(obj_stack);
 return(root_obj);
}

node *json_LoadFile(char *filename)
{ 
  node *rn = NULL;
  FILE *json = fopen(filename,"rb");
  if(json==NULL)
    return(NULL);
  fseek(json, 0, SEEK_END);
  long json_len = ftell(json);
  fseek(json,0,SEEK_SET);
  char *json_data = (char*)malloc(json_len);
  int r = fread((void*)json_data,json_len,1,json);
  if(r)
  	rn = json_Load(json_data,json_len);
  free(json_data);
  fclose(json);
  return(rn);
}

node *json_LoadString(char *content)
{ 
  node *rn = NULL;
  if(content==NULL)
    return(NULL);
  long json_len = strlen(content);
  rn = json_Load(content,json_len);
  return(rn);
}

