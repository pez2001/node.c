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

char *fbx_AddCharToString(char *string,char letter)
{ 
  int len=strlen(string);
  string = (char*)realloc(string,len+2);
  string[len+1] = 0;
  string[len] = letter;
  return(string);
}

char *fbx_CreateEmptyString()
{
    char *string = (char*)malloc(1);
    string[0] = 0;
    return(string);
}

char *fbx_TrimString(char *string)
{
  long len = strlen(string);
  if(!len)
    return(string);
  long s=0;
  long e=len-1;
  while(s<len && isspace(string[s]))s++;
  while(e>0 && isspace(string[e]))e--;
  e=e+1;
  long nlen = e-s;
  if(nlen<=0)
  {
    free(string);
    return(fbx_CreateEmptyString());
  }
  if(nlen==len)
    return(string);
  char *r = (char*)malloc(nlen+1);
  memcpy(r,string+s,nlen);
  r[nlen] = 0;
  free(string);
  return(r);
}

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
          node_ParseNumber(kv,value_string);
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
          node_ParseNumber(n,value_string);
        }
}








node *fbx_Load(char *fbx,unsigned long len)
{
  int state=0;   
  list *obj_stack=list_Create(0,0);
  unsigned long offset=0;
  char *value_string = fbx_CreateEmptyString();
  int is_value_string=0;
  //state |= FBX_STATE_IN_VALUE;
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
         is_value_string = 1;
         continue;
      }
      else
      if(add_char=='\\')
      {
         add_char = fbx_ConvertEscapeChar(fbx[offset]);
         offset++;
         if(add_char==0)
          continue;
      }
     value_string = fbx_AddCharToString(value_string,add_char);
     continue;
    }



    switch(fbx[offset])
    {
       case 0:
             free(value_string);
             return(root_obj);
       case ';':
             state |= FBX_STATE_IN_COMMENT;
       case '\n':
       case '\r':
       case '\t':
       case ' ':
       case ',': 
             if(!is_value_string)
                  value_string = fbx_TrimString(value_string);
               if(strlen(value_string))
               {
                 node *array_obj = node_Create();
                 fbx_SetNode(array_obj,value_string,is_value_string);
                 node_array_Add(new_obj,array_obj);
                 node_SetParent(array_obj,new_obj);
                 free(value_string);
                 value_string=fbx_CreateEmptyString();
               }
            offset++;
            is_value_string=0;
            continue;
       case '{':
            printf("found opening bracket\n");
             /* node *new_obj = node_Create();
              node_SetArray(new_obj,0);
                key_string = fbx_TrimString(key_string);
                printf("added new obj:%s\n",key_string);
                node_SetKey(new_obj,key_string);
                if(strlen(key_string))
                {
                  free(key_string);
                  key_string=fbx_CreateEmptyString();
                }
                node_SetParent(new_obj,actual_obj);
                node_AddItem(actual_obj,new_obj);
                printf("added new obj V:[%s] to:[%s]\n",node_GetKey(new_obj),node_GetKey(actual_obj));
             */
            list_Push(obj_stack,new_obj);

            offset++;
            continue;
       case '}':
            printf("found closing bracket\n");//:%s\n",node_GetKey(parent_obj));
            /*if(!is_value_string)
              value_string = fbx_TrimString(value_string);
            node *kv = fbx_CreateNode(key_string,value_string,is_value_string);
               node_AddItem(actual_obj,kv);
               node_SetParent(kv,actual_obj);
               if(strlen(value_string))
               {
                 free(value_string);
                 value_string=fbx_CreateEmptyString();
               }

            }
            is_value_string=0;
        	printf("a:[%s]\n",node_GetKey(actual_obj));
            list_Pop(obj_stack);
        	printf("p:[%s]\n",node_GetKey((node*)list_GetTop(obj_stack)));
          */
            list_Pop(obj_stack);
            offset++;
            continue;
       case ':':
            //printf("found ':' : [%s]\n",value_string);
            is_value_string = 0;
            value_string = fbx_TrimString(value_string);
            new_obj = node_Create();
            node_SetArray(new_obj,0);
            node_SetKey(new_obj,value_string);
            node_SetParent(new_obj,actual_obj);
            node_AddItem(actual_obj,new_obj);
            printf("added new obj V:[%s] to:[%s]\n",node_GetKey(new_obj),node_GetKey(actual_obj));
            if(strlen(value_string))
            {
              free(value_string);
              value_string = fbx_CreateEmptyString();
            }
            offset++;
            continue;
       case '"':
            state |= FBX_STATE_IN_STRING;
            value_string = fbx_TrimString(value_string);
            if(strlen(value_string))
            { 
              free(value_string);
              value_string = fbx_CreateEmptyString();
            }
            offset++;
            continue;
    }
    if(!is_value_string)
    {
      value_string = fbx_AddCharToString(value_string,fbx[offset]);
    } 
    offset++;
  } 

 free(value_string);
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
  char *fbx_data = (char*)malloc(fbx_len);
  int r = fread((void*)fbx_data,fbx_len,1,fbx);
  if(r)
  	rn = fbx_Load(fbx_data,fbx_len);
  free(fbx_data);
  fclose(fbx);
  return(rn);
}

