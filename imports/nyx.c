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
#include "nyx.h"

char nyx_ConvertEscapeChar(char escape_char)
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

node *nyx_CreateNode(char *key_string,char *value_string,int is_value_string)
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

void nyx_SetNode(node *n,char *value_string,int is_value_string)
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
          //double d = atof(value_string);
          //node_SetDouble(n,d);
          node_ParseNumber(n,value_string);
        }
}

int nyx_is_in_key_space(char c)
{
  if(c>=48 &&c<=57) /*numbers*/
    return(1);
  if(c>=65 && c<=90) /*A to Z */
    return(1);
  if(c>=97 && c<=122) /*a to z*/
    return(1);
  if(c==95) /* _ */  
    return(1);
  if(c==63) /* ? */
    return(1);
  return(0);
}

void nyx_add_value_string(char **value_string,int state,node *nyx_stream)
{
 *value_string = str_Trim(*value_string);
 if(strlen(*value_string))
 { 
  if(state & NYX_STATE_IN_VALUE  && !(state & NYX_STATE_IN_COMMENT))
  {
    node *val = nyx_CreateNode("val",*value_string,0);
    node_AddItem(nyx_stream,val);
    node_SetParent(val,nyx_stream);
  }
  if(state & NYX_STATE_IN_OP  && !(state & NYX_STATE_IN_COMMENT) && strcmp(*value_string,"/*"))
  {
    node *ops = nyx_CreateNode("ops",*value_string,0);
    node_AddItem(nyx_stream,ops);
    node_SetParent(ops,nyx_stream);
  }
  free(*value_string);
  *value_string = str_CreateEmpty();
 }
}

node *nyx_Load(char *nyx,unsigned long len)
{
  int state=0;
  int comment_level=0;   
  list *obj_stack=list_Create(0,0);
  unsigned long offset=0;
  char *value_string = str_CreateEmpty();
  node *new_obj  = NULL;
  node *root_obj = node_Create();
  node_SetType(root_obj,NODE_TYPE_NODE);
  node_SetKey(root_obj,"nyx_stream");
  list_Push(obj_stack,root_obj);
  
  new_obj = node_Create();
  node_SetType(new_obj,NODE_TYPE_NODE);
  node_SetKey(new_obj,"nyx_block");
  list_Push(obj_stack,new_obj);
  node_AddItem(root_obj,new_obj);
  node_SetParent(new_obj,root_obj);

  node *new_s_obj = nyx_CreateNode("nyx_statement","null",0);
  node_SetType(new_s_obj,NODE_TYPE_NODE);
  node_AddItem(new_obj,new_s_obj);
  node_SetParent(new_s_obj,new_obj);
  list_Push(obj_stack,new_s_obj);      

  if(len>2 && nyx[offset]=='#' && nyx[offset+1]=='!')
  {
    while(offset<len)
    {
      if(nyx[offset]==10)
        break;
      offset++;
    }
  }


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

    if(state & NYX_STATE_IN_STRING)
    {
      if(nyx[offset] == '"')
      {
        node *str = nyx_CreateNode("str",value_string,1);
        node_AddItem(actual_obj,str);
        node_SetParent(str,actual_obj);

        free(value_string);
        value_string = str_CreateEmpty();
        state &= ~NYX_STATE_IN_STRING;
        offset++;
        continue;
      }
      if((state & NYX_STATE_IN_STRING) && nyx[offset]=='\\')
      {
        offset++;
        value_string = str_AddChar(value_string,nyx_ConvertEscapeChar(nyx[offset]));
      }
      else
        value_string = str_AddChar(value_string,nyx[offset]);
      offset++;
      continue;
    }

    if(nyx[offset] == '/' && ((offset+1)<(len-1)) && nyx[offset+1]=='*')
    {
      state |= NYX_STATE_IN_COMMENT;
      comment_level+=1;
      offset+=2;
      continue;
    }

    if(comment_level>0 && nyx[offset] == '*' && ((offset+1)<(len-1)) && nyx[offset+1]=='/')
    {
      comment_level-=1;
      if(!comment_level)
        state &= ~NYX_STATE_IN_COMMENT;
      offset+=2;
      continue;
    }

    if(state & NYX_STATE_IN_COMMENT)
    {
      offset++;
      continue;
    }

    if(nyx[offset] == '"')
    {
      nyx_add_value_string(&value_string,state,actual_obj);
      state |= NYX_STATE_IN_STRING;
      offset++;
      continue;
    }
    if(nyx[offset]==' ' || nyx[offset]=='\t')
    {
      offset++;
      continue;
    }

    if(nyx[offset] == ';' || nyx[offset] == ',')
    {
      nyx_add_value_string(&value_string,state,actual_obj);
      state &= ~NYX_STATE_IN_OP;
      state &= ~NYX_STATE_IN_VALUE;
      node *top = list_Pop(obj_stack);
      long num_stuff=node_GetItemsNum(top);
      actual_obj = (node*)list_GetTop(obj_stack);
      if(num_stuff)//skips empty statements
      {
        new_obj = nyx_CreateNode("nyx_statement","null",0);
        node_SetType(new_obj,NODE_TYPE_NODE);
        node_AddItem(actual_obj,new_obj);
        node_SetParent(new_obj,actual_obj);
        list_Push(obj_stack,new_obj);      
      }
      else
      { //reuse 
        list_Push(obj_stack,top);
      }
      offset++;
      continue;
    }

    if(nyx[offset] == '}' || nyx[offset] == ')' || nyx[offset] == ']')
    {
      nyx_add_value_string(&value_string,state,actual_obj);
      state &= ~NYX_STATE_IN_OP;
      state &= ~NYX_STATE_IN_VALUE;
      node *top = list_Pop(obj_stack);
      long num_stuff=node_GetItemsNum(top);
      top = list_Pop(obj_stack);//remove last statement + block 
      if(!num_stuff)
      {
        long num_statements=node_GetItemsNum(top);
        node *last = node_GetItem(top,num_statements-1);
        node_RemoveItemByIndex(top,num_statements-1);
        node_Free(last,True);
      }
      actual_obj = (node*)list_GetTop(obj_stack);
      offset++;
      continue;
    }

    if(nyx[offset] == '{')
    {
      nyx_add_value_string(&value_string,state,actual_obj);
      state &= ~NYX_STATE_IN_OP;
      state &= ~NYX_STATE_IN_VALUE;
      new_obj = nyx_CreateNode("nyx_block","null",0);
      node_SetType(new_obj,NODE_TYPE_NODE);
      node_AddItem(actual_obj,new_obj);
      node_SetParent(new_obj,actual_obj);
      list_Push(obj_stack,new_obj);      
      
      actual_obj = new_obj;
      new_obj = nyx_CreateNode("nyx_statement","null",0);
      node_SetType(new_obj,NODE_TYPE_NODE);
      node_AddItem(actual_obj,new_obj);
      node_SetParent(new_obj,actual_obj);
      list_Push(obj_stack,new_obj);      
      
      offset++;
      continue;
    }

    if(nyx[offset] == '(')
    {
      nyx_add_value_string(&value_string,state,actual_obj);
      state &= ~NYX_STATE_IN_OP;
      state &= ~NYX_STATE_IN_VALUE;
      new_obj = nyx_CreateNode("nyx_parameters","null",0);
      node_SetType(new_obj,NODE_TYPE_NODE);
      node_AddItem(actual_obj,new_obj);
      node_SetParent(new_obj,actual_obj);
      list_Push(obj_stack,new_obj);      

      actual_obj = new_obj;
      new_obj = nyx_CreateNode("nyx_statement","null",0);
      node_SetType(new_obj,NODE_TYPE_NODE);
      node_AddItem(actual_obj,new_obj);
      node_SetParent(new_obj,actual_obj);
      list_Push(obj_stack,new_obj);      

      offset++;
      continue;
    }

    if(nyx[offset] == '[')
    {
      nyx_add_value_string(&value_string,state,actual_obj);
      state &= ~NYX_STATE_IN_OP;
      state &= ~NYX_STATE_IN_VALUE;
      new_obj = nyx_CreateNode("nyx_array","null",0);
      node_SetType(new_obj,NODE_TYPE_NODE);
      node_AddItem(actual_obj,new_obj);
      node_SetParent(new_obj,actual_obj);
      list_Push(obj_stack,new_obj);      

      actual_obj = new_obj;
      new_obj = nyx_CreateNode("nyx_statement","null",0);
      node_SetType(new_obj,NODE_TYPE_NODE);
      node_AddItem(actual_obj,new_obj);
      node_SetParent(new_obj,actual_obj);
      list_Push(obj_stack,new_obj);      

      offset++;
      continue;
    }

    if(nyx[offset] == '\r' || nyx[offset] == '\n' || nyx[offset]== 0)
    {
      nyx_add_value_string(&value_string,state,actual_obj);
      state &= ~NYX_STATE_IN_OP;
      state &= ~NYX_STATE_IN_VALUE;
      offset++;
      continue;
    }
    
    if(nyx_is_in_key_space(nyx[offset]))
    {
      if(!(state & NYX_STATE_IN_VALUE))
      {
        nyx_add_value_string(&value_string,state,actual_obj);
        state &= ~NYX_STATE_IN_OP;
      }
      state |= NYX_STATE_IN_VALUE;
    }
    else
    {
      if(!(state & NYX_STATE_IN_OP))
      {
        nyx_add_value_string(&value_string,state,actual_obj);
        state &= ~NYX_STATE_IN_VALUE;
      }
      state |= NYX_STATE_IN_OP;
    }

    value_string = str_AddChar(value_string,nyx[offset]);
    offset++;
  } 
 free(value_string);

 //remove empty statements possible at the end
 while(list_GetLen(obj_stack))
 {
 node *top = list_Pop(obj_stack);
 long num_stuff=node_GetItemsNum(top);
 if(!num_stuff)
 {
   node *actual_obj = (node*)list_GetTop(obj_stack);
   if(actual_obj!=NULL)
   {
     node_Free(top,True);
     long rem = node_GetItemIndex(actual_obj,top);
     if(rem ==-1)
     {
      printf("obj not found in supposed parent\n");
     }
     node_RemoveItem(actual_obj,top);
   }
 }
 }

 list_Close(obj_stack);
 return(root_obj);
}


node *nyx_LoadFile(char *filename)
{ 
  node *rn = NULL;
  FILE *nyx = fopen(filename,"rb");
  if(nyx==NULL)
    return(NULL);
  fseek(nyx, 0, SEEK_END);
  long nyx_len = ftell(nyx);
  if(!nyx_len)
    return(NULL);
  fseek(nyx,0,SEEK_SET);
  char *nyx_data = (char*)malloc(nyx_len+1);
  //memset(nyx_data+nyx_len,0,1);
  int r = fread((void*)nyx_data,nyx_len,1,nyx);
  nyx_data[nyx_len]=0;
  if(r)
    rn = nyx_Load(nyx_data,nyx_len+1);
  free(nyx_data);
  fclose(nyx);
  return(rn);
}

node *nyx_LoadString(char *content)
{ 
  node *rn = NULL;
  if(content==NULL || !strlen(content))
    return(NULL);
  long nyx_len = strlen(content)+1;
  rn = nyx_Load(content,nyx_len);
  return(rn);
}

