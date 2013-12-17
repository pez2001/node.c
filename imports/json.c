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


node *json_ParseObject();
node *json_ParseArray();
node *json_ParseString();


char *json_AddCharToString(char *string,char letter)
{ 
  int len=strlen(string);
  string = (char*)realloc(string,len+2);
  string[len+1] = 0;
  string[len] = letter;
  return(string);
}

char *json_CreateEmptyString()
{
    char *string = (char*)malloc(1);
    string[0] = 0;
    return(string);
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

node * json_CreateNode(char *key_string,char *value_string,int is_value_string)
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



node *json_Load(char *json,unsigned long len)
{
  int state=0;   
  node *root_obj=NULL;
  node *actual_obj=NULL;
  node *parent_obj=NULL;
  unsigned long offset=0;
  char *key_string = json_CreateEmptyString();
  char *value_string = json_CreateEmptyString();
  int is_value_string=0;
  int is_obj=0;

  state |= JSON_STATE_IN_KEY;

  while(offset<len)
  {
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

       if((state&JSON_STATE_IN_KEY))
       {
          key_string = json_AddCharToString(key_string,add_char);
       }	
       if((state&JSON_STATE_IN_VALUE))
       {
          value_string = json_AddCharToString(value_string,add_char);
       }	
       continue;
    }

    switch(json[offset])
    {
       case 0:
             free(key_string);
             free(value_string);
             return(root_obj);
       case '\n':
       case '\r':

             state |= JSON_STATE_IN_KEY;     
             if((state & JSON_STATE_IN_VALUE))
             {
               state &= ~JSON_STATE_IN_VALUE;
               if(actual_obj!=NULL)//validity check here
               {
                 node *kv = json_CreateNode(key_string,value_string,is_value_string);
                 node_AddItem(actual_obj,kv);
               }
               if(strlen(key_string))
               {
                 free(key_string);
                 key_string=json_CreateEmptyString();
               }
               if(strlen(value_string))
               {
                 free(value_string);
                 value_string=json_CreateEmptyString();
               }
              }
            offset++;
            continue;

       case '\t':
       case ' ':
            break;
            //offset++;
            //continue;
       case '{':
            state |= JSON_STATE_IN_OBJ;
            is_obj = 0;
            if((state & JSON_STATE_IN_ARRAY))
            {
              parent_obj = actual_obj;
              if(parent_obj!=NULL && node_IsType(parent_obj,NODE_TYPE_ARRAY))//validity check here
              {
                actual_obj = node_Create();
                node_SetType(actual_obj,NODE_TYPE_NODE);
                node_array_Add(parent_obj,actual_obj);
              }
              state |= JSON_STATE_IN_KEY;     
            }
            else
            {
              parent_obj = actual_obj;
              actual_obj = node_Create();
              node_SetType(actual_obj,NODE_TYPE_NODE);
              if(root_obj==NULL)
              {
              	root_obj = actual_obj;
                node_SetKey(actual_obj,"root");
              }	
              if((state & JSON_STATE_IN_VALUE))
              {
                node_SetKey(actual_obj,key_string);
                if(strlen(key_string))
                {
                  free(key_string);
                  key_string=json_CreateEmptyString();
                }
                node_SetParent(actual_obj,parent_obj);
                if(parent_obj!=NULL)
                {
                  node_AddItem(parent_obj,actual_obj);
                }
              }
              if((state & JSON_STATE_IN_KEY))
              {
                node_SetParent(actual_obj,parent_obj);
                if(parent_obj!=NULL)
                {
                  node_AddItem(parent_obj,actual_obj);
                }
              }
              state |= JSON_STATE_IN_KEY;     

            }
            state &= ~JSON_STATE_IN_VALUE;
            offset++;
            continue;
       case '}':
            if((state & JSON_STATE_IN_VALUE))
            {
               state &= ~JSON_STATE_IN_VALUE;
               if(actual_obj!=NULL)//validity check here
               {
                 node *kv = json_CreateNode(key_string,value_string,is_value_string);
                 node_AddItem(actual_obj,kv);
               }
               if(strlen(key_string))
               {
                 free(key_string);
                 key_string=json_CreateEmptyString();
               }
               if(strlen(value_string))
               {
                 free(value_string);
                 value_string=json_CreateEmptyString();
               }
            }
            state &= ~JSON_STATE_IN_OBJ;
            is_obj = 1;
            if(parent_obj!=NULL)
            {
            	 actual_obj = parent_obj;
            	 parent_obj = node_GetParent(actual_obj);
            }
            offset++;
            continue;
       case ',': 
            if((state & JSON_STATE_IN_ARRAY) && !(state & JSON_STATE_IN_OBJ))
            {
              state |= JSON_STATE_IN_VALUE;     
              state &= ~JSON_STATE_IN_KEY;
              if(!is_obj)
              {
                if(actual_obj!=NULL)//validity check here
                {
                  node *kv = json_CreateNode(NULL,value_string,is_value_string);
                  node_array_Add(actual_obj,kv);
                }
                if(strlen(value_string))
                {
                  free(value_string);
                  value_string = json_CreateEmptyString();
                }
              }
            }else
            {
             state |= JSON_STATE_IN_KEY;     
             if((state & JSON_STATE_IN_VALUE))
             {
               state &= ~JSON_STATE_IN_VALUE;
               if(!is_obj)
                {
                  if(actual_obj!=NULL)//validity check here
                  {
                    node *kv = json_CreateNode(key_string,value_string,is_value_string);
                    node_AddItem(actual_obj,kv);
                  }
                  if(strlen(key_string))
                  {
                    free(key_string);
                    key_string=json_CreateEmptyString();
                  }
                  if(strlen(value_string))
                  {
                    free(value_string);
                    value_string=json_CreateEmptyString();
                  }
                }
              }
            }
            is_obj = 0;
            offset++;
            continue;
       case '[':
            state |= JSON_STATE_IN_VALUE;
            state |= JSON_STATE_IN_ARRAY;
            state &= ~JSON_STATE_IN_OBJ;
            parent_obj = actual_obj;
            if(parent_obj!=NULL)//validity check here
            {
              actual_obj = node_Create();
              node_SetKey(actual_obj,key_string);
              node_SetArray(actual_obj,0);
              node_AddItem(parent_obj,actual_obj);
            }
            if(strlen(key_string))
            {
              free(key_string);
              key_string=json_CreateEmptyString();
            }
            is_obj = 0;
            offset++;
            continue;
       case ']':
             if((state & JSON_STATE_IN_ARRAY) && (state & JSON_STATE_IN_VALUE))
             {
               state &= ~JSON_STATE_IN_VALUE;
               if(strlen(value_string))
               {
                 if(actual_obj!=NULL)//validity check here
                 {
                   node *kv = json_CreateNode(NULL,value_string,is_value_string);
                   node_array_Add(actual_obj,kv);
                 }
               }
             }
            if(strlen(value_string))
            {
              free(value_string);
              value_string = json_CreateEmptyString();
            }

            state |= JSON_STATE_IN_OBJ;
            state &= ~JSON_STATE_IN_ARRAY;
            if(parent_obj!=NULL)
            {
            	actual_obj = parent_obj;
            	parent_obj = node_GetParent(actual_obj);
            }
            offset++;
            continue;
       case ':':
       case '=':
            state |= JSON_STATE_IN_VALUE;     
            state &= ~JSON_STATE_IN_KEY;
            is_value_string = 0;
            if(strlen(value_string))
            {
              free(value_string);
              value_string = json_CreateEmptyString();
            }
            offset++;
            continue;
       case '"':
            if(!(state&JSON_STATE_IN_STRING))
            {
            	state |= JSON_STATE_IN_STRING;
                if((state&JSON_STATE_IN_KEY) && strlen(key_string))
                {	
                   free(key_string);
                   key_string = json_CreateEmptyString();
                }
            }
            else
            {
            	state &= ~JSON_STATE_IN_STRING;
            }
            offset++;
            continue;
    }
    if((state&JSON_STATE_IN_VALUE))
    {
      value_string = json_AddCharToString(value_string,json[offset]);
    } 
    if((state&JSON_STATE_IN_KEY))
    {
      key_string = json_AddCharToString(key_string,json[offset]);
    } 
    offset++;
  } 

 free(key_string);
 free(value_string);
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


