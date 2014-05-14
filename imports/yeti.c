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
#include "yeti.h"

char *yeti_AddCharToString(char *string,char letter)
{ 
  int len=strlen(string);
  string = (char*)realloc(string,len+2);
  string[len+1] = 0;
  string[len] = letter;
  return(string);
}

char *yeti_CreateEmptyString(void)
{
    char *string = (char*)malloc(1);
    string[0] = 0;
    return(string);
}

char *yeti_TrimString(char *string)
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
    return(yeti_CreateEmptyString());
  }
  if(nlen==len)
    return(string);
  char *r = (char*)malloc(nlen+1);
  memcpy(r,string+s,nlen);
  r[nlen] = 0;
  free(string);
  return(r);
}

char yeti_ConvertEscapeChar(char escape_char)
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

node *yeti_CreateNode(char *key_string,char *value_string,int is_value_string)
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

void yeti_SetNode(node *n,char *value_string,int is_value_string)
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

int yeti_is_in_key_space(char c)
{
  if(c>=48 &&c<=57) /*number*/
    return(1);
  if(c>=63 && c<=90) /*? to Z */
    return(1);
  if(c>=97 && c<=122) /*a to z*/
    return(1);
  if(c==38) /* & */
    return(1);
  return(0);
}

void yeti_add_value_string(char **value_string,int state,node *yeti_stream)
{
 *value_string = yeti_TrimString(*value_string);
 if(strlen(*value_string))
 { 
  if(state & YETI_STATE_IN_VALUE  && !(state & YETI_STATE_IN_COMMENT))
  {
    node *val = yeti_CreateNode("val",*value_string,0);
    node_AddItem(yeti_stream,val);
    node_SetParent(val,yeti_stream);
  }
  if(state & YETI_STATE_IN_OP  && !(state & YETI_STATE_IN_COMMENT) && strcmp(*value_string,"/*"))
  {
    node *ops = yeti_CreateNode("ops",*value_string,0);
    node_AddItem(yeti_stream,ops);
    node_SetParent(ops,yeti_stream);
  }
  free(*value_string);
  *value_string = yeti_CreateEmptyString();
 }
}


node *yeti_Load(char *yeti,unsigned long len)
{
  int state=0;
  int comment_level=0;   
  list *obj_stack=list_Create(0,0);
  unsigned long offset=0;
  char *value_string = yeti_CreateEmptyString();
  node *new_obj  = NULL;
  node *root_obj = node_Create();
  node_SetType(root_obj,NODE_TYPE_NODE);
  node_SetKey(root_obj,"yeti_stream");
  list_Push(obj_stack,root_obj);
  
  new_obj = node_Create();
  node_SetType(new_obj,NODE_TYPE_NODE);
  node_SetKey(new_obj,"yeti_block");
  list_Push(obj_stack,new_obj);
  node_AddItem(root_obj,new_obj);
  node_SetParent(new_obj,root_obj);

  node *new_s_obj = yeti_CreateNode("yeti_statement","null",0);
  node_SetType(new_s_obj,NODE_TYPE_NODE);
  node_AddItem(new_obj,new_s_obj);
  node_SetParent(new_s_obj,new_obj);
  list_Push(obj_stack,new_s_obj);      

  if(len>2 && yeti[offset]=='#' && yeti[offset+1]=='!')
  {
    //printf("shebang found\n");
    while(offset<len)
    {
      if(yeti[offset]==10)
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

    if(state & YETI_STATE_IN_STRING)
    {
      if(yeti[offset] == '"')
      {
        node *str = yeti_CreateNode("str",value_string,1);
        node_AddItem(actual_obj,str);
        node_SetParent(str,actual_obj);

        free(value_string);
        value_string = yeti_CreateEmptyString();
        state &= ~YETI_STATE_IN_STRING;
        offset++;
        continue;
      }
      value_string = yeti_AddCharToString(value_string,yeti[offset]);
      offset++;
      continue;
    }

    if(yeti[offset] == '/' && ((offset+1)<(len-1)) && yeti[offset+1]=='*')
    {
      state |= YETI_STATE_IN_COMMENT;
      comment_level+=1;
      offset+=2;
      continue;
    }

    if(comment_level>0 && yeti[offset] == '*' && ((offset+1)<(len-1)) && yeti[offset+1]=='/')
    {
      comment_level-=1;
      if(!comment_level)
        state &= ~YETI_STATE_IN_COMMENT;
      offset+=2;
      continue;
    }

    if(state & YETI_STATE_IN_COMMENT)
    {
      offset++;
      continue;
    }

    if(yeti[offset] == '"')
    {
      yeti_add_value_string(&value_string,state,actual_obj);
      state |= YETI_STATE_IN_STRING;
      offset++;
      continue;
    }
    if(yeti[offset]==' ' || yeti[offset]=='\t')
    {
      yeti_add_value_string(&value_string,state,actual_obj);
      state &= ~YETI_STATE_IN_OP;
      state &= ~YETI_STATE_IN_VALUE;
      offset++;
      continue;
    }

    if(yeti[offset] == ';' || yeti[offset] == ',')
    {
      yeti_add_value_string(&value_string,state,actual_obj);
      state &= ~YETI_STATE_IN_OP;
      state &= ~YETI_STATE_IN_VALUE;
      node *top = list_Pop(obj_stack);
      long num_stuff=node_GetItemsNum(top);
      actual_obj = (node*)list_GetTop(obj_stack);
      if(num_stuff)
      {
        new_obj = yeti_CreateNode("yeti_statement","null",0);
        node_SetType(new_obj,NODE_TYPE_NODE);
        node_AddItem(actual_obj,new_obj);
        node_SetParent(new_obj,actual_obj);
        list_Push(obj_stack,new_obj);      
      }
      else
      {
        list_Push(obj_stack,top);
      }
      offset++;
      continue;
    }

    if(yeti[offset] == '}' || yeti[offset] == ')' || yeti[offset] == ']')
    {
      yeti_add_value_string(&value_string,state,actual_obj);
      state &= ~YETI_STATE_IN_OP;
      state &= ~YETI_STATE_IN_VALUE;
      node *top = list_Pop(obj_stack);

      //if(yeti[offset] == '}')
      //{
        long num_stuff=node_GetItemsNum(top);
        //node *top = list_Pop(obj_stack);//remove last statement + block 
        top = list_Pop(obj_stack);//remove last statement + block 
        if(!num_stuff)
        {
          long num_statements=node_GetItemsNum(top);
          node *last = node_GetItem(top,num_statements-1);
          node_RemoveItemByIndex(top,num_statements-1);
          node_Free(last,True);
        }
        actual_obj = (node*)list_GetTop(obj_stack);
      //}
      offset++;
      continue;
    }

    if(yeti[offset] == '{')
    {
      yeti_add_value_string(&value_string,state,actual_obj);
      state &= ~YETI_STATE_IN_OP;
      state &= ~YETI_STATE_IN_VALUE;
      new_obj = yeti_CreateNode("yeti_block","null",0);
      node_SetType(new_obj,NODE_TYPE_NODE);
      node_AddItem(actual_obj,new_obj);
      node_SetParent(new_obj,actual_obj);
      list_Push(obj_stack,new_obj);      
      
      actual_obj = new_obj;
      new_obj = yeti_CreateNode("yeti_statement","null",0);
      node_SetType(new_obj,NODE_TYPE_NODE);
      node_AddItem(actual_obj,new_obj);
      node_SetParent(new_obj,actual_obj);
      list_Push(obj_stack,new_obj);      
      
      offset++;
      continue;
    }

    if(yeti[offset] == '(')
    {
      yeti_add_value_string(&value_string,state,actual_obj);
      state &= ~YETI_STATE_IN_OP;
      state &= ~YETI_STATE_IN_VALUE;
      new_obj = yeti_CreateNode("yeti_parameters","null",0);
      node_SetType(new_obj,NODE_TYPE_NODE);
      node_AddItem(actual_obj,new_obj);
      node_SetParent(new_obj,actual_obj);
      list_Push(obj_stack,new_obj);      

      actual_obj = new_obj;
      new_obj = yeti_CreateNode("yeti_statement","null",0);
      node_SetType(new_obj,NODE_TYPE_NODE);
      node_AddItem(actual_obj,new_obj);
      node_SetParent(new_obj,actual_obj);
      list_Push(obj_stack,new_obj);      

      offset++;
      continue;
    }

    if(yeti[offset] == '[')
    {
      yeti_add_value_string(&value_string,state,actual_obj);
      state &= ~YETI_STATE_IN_OP;
      state &= ~YETI_STATE_IN_VALUE;
      new_obj = yeti_CreateNode("yeti_array","null",0);
      node_SetType(new_obj,NODE_TYPE_NODE);
      node_AddItem(actual_obj,new_obj);
      node_SetParent(new_obj,actual_obj);
      list_Push(obj_stack,new_obj);      

      actual_obj = new_obj;
      new_obj = yeti_CreateNode("yeti_statement","null",0);
      node_SetType(new_obj,NODE_TYPE_NODE);
      node_AddItem(actual_obj,new_obj);
      node_SetParent(new_obj,actual_obj);
      list_Push(obj_stack,new_obj);      

      offset++;
      continue;
    }

    if(yeti[offset] == '\r' || yeti[offset] == '\n')
    {
      yeti_add_value_string(&value_string,state,actual_obj);
      state &= ~YETI_STATE_IN_OP;
      state &= ~YETI_STATE_IN_VALUE;
      offset++;
      continue;
    }
    
    if(yeti_is_in_key_space(yeti[offset]))
    {
      if(!(state & YETI_STATE_IN_VALUE))
      {
        yeti_add_value_string(&value_string,state,actual_obj);
        state &= ~YETI_STATE_IN_OP;
      }
      state |= YETI_STATE_IN_VALUE;
    }
    else
    {
      if(!(state & YETI_STATE_IN_OP))
      {
        yeti_add_value_string(&value_string,state,actual_obj);
        state &= ~YETI_STATE_IN_VALUE;
      }
      state |= YETI_STATE_IN_OP;
    }

    value_string = yeti_AddCharToString(value_string,yeti[offset]);
    offset++;
  } 
 free(value_string);

 
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


node *yeti_LoadFile(char *filename)
{ 
  node *rn = NULL;
  FILE *yeti = fopen(filename,"rb");
  if(yeti==NULL)
    return(NULL);
  fseek(yeti, 0, SEEK_END);
  long yeti_len = ftell(yeti);
  fseek(yeti,0,SEEK_SET);
  char *yeti_data = (char*)malloc(yeti_len);
  int r = fread((void*)yeti_data,yeti_len,1,yeti);
  if(r)
  	rn = yeti_Load(yeti_data,yeti_len);
  free(yeti_data);
  fclose(yeti);
  return(rn);
}

