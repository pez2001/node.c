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
#include "fbx.h"

char fbx_ConvertEscapeChar(char escape_char)
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

node *fbx_CreateNode(char *key_string,char *value_string,int is_value_string)
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
          double d = atof(value_string);
          node_SetDouble(kv,d);
          //node_ParseNumber(kv,value_string);
        }
  return(kv);
}

void fbx_SetNode(node *n,char *value_string,int is_value_string)
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
          node_SetDouble(n,d);
          //node_ParseNumber(n,value_string);
        }
}

int fbx_isdigit(char c)
{

  return(isdigit(c) || c == '.'|| c== '-' || c=='+');
}

node *fbx_Load(char *fbx,unsigned long len)
{
  int state=0;   
  list *obj_stack=list_Create(0,0);
  unsigned long offset=0;
  char *value_string = str_CreateEmpty();
  int is_last_digit = 0;
  node *new_obj  = NULL;
  node *root_obj = node_Create();
  node_SetArray(root_obj,0);
  node_SetKey(root_obj,"root");
  list_Push(obj_stack,root_obj);

  while(offset<len)
  {
    node *actual_obj = (node*)list_GetTop(obj_stack);

    if((state & FBX_STATE_IN_COMMENT))
    {
      if(fbx[offset]=='\r' || fbx[offset]=='\n')
      {
      	state &= ~FBX_STATE_IN_COMMENT;
      }	
      offset++;
      continue;
    }
    if((state & FBX_STATE_IN_STRING))
    {
      char add_char = fbx[offset];
      offset++;
      if(add_char == '"')
      {
         state &= ~FBX_STATE_IN_STRING;
         value_string = str_Trim(value_string);
         node *array_obj = node_Create();
         fbx_SetNode(array_obj,value_string,True);
         node_array_Add(new_obj,array_obj);
         node_SetParent(array_obj,new_obj);
         free(value_string);
         value_string=str_CreateEmpty();
         continue;
      }
     value_string = str_AddChar(value_string,add_char);
     continue;
    }


    switch(fbx[offset])
    {
       case 0:
             free(value_string);
             return(root_obj);
       case '{':
            list_Push(obj_stack,new_obj);
            is_last_digit=0;
            offset++;
            value_string = str_Trim(value_string);
            if(strlen(value_string))
            {
              node *array_obj = node_Create();
              fbx_SetNode(array_obj,value_string,False);
              node_array_Add(new_obj,array_obj);
              node_SetParent(array_obj,new_obj);
              free(value_string);
              value_string=str_CreateEmpty();
            }
            break;
       case '}':
            list_Pop(obj_stack);
            is_last_digit=0;
            offset++;
            value_string = str_Trim(value_string);
            if(strlen(value_string))
            {
              node *array_obj = node_Create();
              fbx_SetNode(array_obj,value_string,False);
              node_array_Add(new_obj,array_obj);
              node_SetParent(array_obj,new_obj);
              free(value_string);
              value_string=str_CreateEmpty();
            }
            break;
       case ':':
            value_string = str_Trim(value_string);
            new_obj = node_Create();
            node_SetArray(new_obj,0);
            node_SetKey(new_obj,value_string);
            node_SetParent(new_obj,actual_obj);
            node_AddItem(actual_obj,new_obj);
            if(strlen(value_string))
            {
              free(value_string);
              value_string = str_CreateEmpty();
            }
            is_last_digit=0;
            offset++;
            break;
       case '"':
            state |= FBX_STATE_IN_STRING;
            value_string = str_Trim(value_string);
            if(strlen(value_string))
            { 
              free(value_string);
              value_string = str_CreateEmpty();
            }
            is_last_digit=0;
            offset++;
            break;
       case ';':
             state |= FBX_STATE_IN_COMMENT;
             //is_last_digit=0;
       case '\n':
       case '\r':
       case '\t': 
              offset++;
              break;
       default:
              if((is_last_digit==1 && fbx_isdigit(fbx[offset])==0 ))
              {
                is_last_digit = fbx_isdigit(fbx[offset]);
                value_string = str_Trim(value_string);
                if(strlen(value_string))
                {
                  node *array_obj = node_Create();
                  fbx_SetNode(array_obj,value_string,False);
                  node_array_Add(new_obj,array_obj);
                  node_SetParent(array_obj,new_obj);
                  free(value_string);
                  value_string= str_CreateEmpty();
                  value_string = str_AddChar(value_string,fbx[offset]);
                }
                offset++;
                break;
              }
               else
              {
                is_last_digit = fbx_isdigit(fbx[offset]);
                value_string = str_AddChar(value_string,fbx[offset]);
                offset++;
                break;
              }
       case ' ':
       case ',': 
               value_string = str_Trim(value_string);
               if(strlen(value_string))
               {
                 node *array_obj = node_Create();
                 fbx_SetNode(array_obj,value_string,False);
                 node_array_Add(new_obj,array_obj);
                 node_SetParent(array_obj,new_obj);
                 free(value_string);
                 value_string = str_CreateEmpty();
               }
               is_last_digit=0;
               offset++;
               break;
    }
  } 

 free(value_string);
 list_Close(obj_stack);
 return(root_obj);
}




node *fbx_LoadFile(char *filename)
{ 
  node *rn = NULL;
  FILE *fbx = fopen(filename,"rb");
  if(fbx==NULL)
    return(NULL);
  fseek(fbx, 0, SEEK_END);
  long fbx_len = ftell(fbx);
  fseek(fbx,0,SEEK_SET);
  char *fbx_data = (char*)malloc(fbx_len+1);
  int r = fread((void*)fbx_data,fbx_len,1,fbx);
  fbx_data[fbx_len]=0;
  if(r)
  	rn = fbx_Load(fbx_data,fbx_len);
  free(fbx_data);
  fclose(fbx);
  return(rn);
}

