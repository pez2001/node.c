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


node *json_Load(char *json,unsigned long len)
{
  int state=0;   
  node *root_obj=NULL;
  node *actual_obj=NULL;
  node *parent_obj=NULL;
  unsigned long offset=0;
  unsigned long start_offset=0;
  char *key_string = NULL;
  char *value_string =NULL;
  int is_value_string=0;
  int is_value=0;

  while(offset<len)
  {
    if((state & JSON_STATE_IN_STRING))
    {
      char add_char = json[offset];
      if(json[offset]=='\\')
      {
         offset++;
         char escape_char = json_ConvertEscapeChar(json[offset]);
         if((state&JSON_STATE_IN_KEY))
         {
             key_string = json_AddCharToString(key_string,escape_char);
         }	

         if((state&JSON_STATE_IN_VALUE))
         {
            value_string = json_AddCharToString(value_string,escape_char);
         }	 

         offset++;
         continue;
      }
      if(json[offset] == '"')
      {
         state &= ~JSON_STATE_IN_STRING;
         is_value_string = 1;
         offset++;
         continue;
      }else
      {
       if((state&JSON_STATE_IN_KEY))
       {
          key_string = json_AddCharToString(key_string,json[offset]);
       }	

       if((state&JSON_STATE_IN_VALUE))
       {
          value_string = json_AddCharToString(value_string,json[offset]);
       }	
       offset++;
       continue;
     }
    }



    switch(json[offset])
    {
       //interpret complete values (null,True,False)
       case '\n':
       case '\r':
       case '\t':
       case ' ':
            offset++;
            continue;
            break;
       case '{':
            state |= JSON_STATE_IN_OBJ;

            start_offset = offset;

            if((state & JSON_STATE_IN_ARRAY))
            {
              printf("new array object found\n"); 
              parent_obj = actual_obj;
              actual_obj = node_Create();
              node_array_Add(parent_obj,actual_obj);
              state |= JSON_STATE_IN_KEY;     
            }
            else
            {
            parent_obj = actual_obj;
            actual_obj = node_Create();
            if(root_obj==NULL)
            {
            	root_obj = actual_obj;
                node_SetKey(actual_obj,"root");
            }	
            state |= JSON_STATE_IN_KEY;     
            printf("new object found\n");
            if((state & JSON_STATE_IN_VALUE))
            {
               printf("is child of object\n");
               node_SetKey(actual_obj,key_string);
               printf("key:%s\n",key_string);

               node_SetParent(actual_obj,parent_obj);
               if(parent_obj!=NULL)
               {
                 node_AddItem(parent_obj,actual_obj);
                 printf("added item:%s to %s\n",actual_obj->key,parent_obj->key);
               }
            }

            }
            state &= ~JSON_STATE_IN_VALUE;
            offset++;
            continue;
            break;
       case '}':
             if((state & JSON_STATE_IN_VALUE))
             {
               state |= JSON_STATE_IN_KEY;     
               state &= ~JSON_STATE_IN_VALUE;
               //printf("key:%s\n",key_string);
               //printf("value:%s\n",value_string);
               node *kv = node_Create();
               node_SetKey(kv,key_string);
               if(is_value_string)
               {
                 node_SetString(kv,value_string);
                 printf("string:%s\n",value_string);
                 //printf(value_string);
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
                          printf("number:%s\n",value_string);
                          
               	       }
               node_AddItem(actual_obj,kv);
               state &= ~JSON_STATE_IN_VALUE;
               state |= JSON_STATE_IN_KEY;     
               }
            state &= ~JSON_STATE_IN_OBJ;
            if(parent_obj!=NULL)
            {
            	actual_obj = parent_obj;
            	printf("actual:%s\n",actual_obj->key);
            	parent_obj = node_GetParent(actual_obj);
            }
            //state &= ~JSON_STATE_IN_STRING;
            //state &= ~JSON_STATE_IN_KEY;
            //actual_obj=NULL;
            offset++;
            continue;
            break;
       case ',':

            if((state & JSON_STATE_IN_ARRAY) && !(state & JSON_STATE_IN_OBJ))
            {
              printf("next array item\n"); 
              printf("string:k:%s,v:%s\n",key_string,value_string);
              state |= JSON_STATE_IN_VALUE;     
              state &= ~JSON_STATE_IN_KEY;
              value_string = json_CreateEmptyString();
              //parent_obj = actual_obj;
              //actual_obj = node_Create();
              //node_array_Add(parent_obj,actual_obj);
              //state |= JSON_STATE_IN_KEY;     
            }else

             if((state & JSON_STATE_IN_VALUE))
             {
               state |= JSON_STATE_IN_KEY;     
               state &= ~JSON_STATE_IN_VALUE;
               //printf("key:%s\n",key_string);
               node *kv = node_Create();
               node_SetKey(kv,key_string);
               if(is_value_string)
               {
                 node_SetString(kv,value_string);
                 printf("string:%s\n",value_string);
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
                          printf("number:%s\n",value_string);
                          
               	       }
               node_AddItem(actual_obj,kv);
             }
            offset++;
            continue;
            break;
       case '[':
            state |= JSON_STATE_IN_ARRAY;
            printf("adding array with key:%s\n",key_string);     
            parent_obj = actual_obj;
            actual_obj = node_Create();
            node_SetArray(actual_obj,0);
            node_AddItem(parent_obj,actual_obj);
            offset++;
            continue;
            break;
       case ']':

              printf("last array item\n"); 
              printf("string:k:%s,v:%s\n",key_string,value_string);
              state |= JSON_STATE_IN_VALUE;     
              state &= ~JSON_STATE_IN_KEY;
              value_string = (char*)malloc(1);
              value_string[0] = 0;
              //value_string_len=0;


            state &= ~JSON_STATE_IN_ARRAY;
            if(parent_obj!=NULL)
            {
            	actual_obj = parent_obj;
            	printf("actual:%s\n",actual_obj->key);
            	parent_obj = node_GetParent(actual_obj);
            }
            offset++;
            continue;
            break;
       case ':':
            state |= JSON_STATE_IN_VALUE;     
            state &= ~JSON_STATE_IN_KEY;
            is_value_string = 0;
            if((state&JSON_STATE_IN_VALUE))
            {	
              value_string = (char*)malloc(1);
              value_string[0] = 0;
              //value_string_len=0;
            }

            offset++;
            continue;
            break;
       case '"':
            if(!(state&JSON_STATE_IN_STRING))
            {
            	state |= JSON_STATE_IN_STRING;
                if((state&JSON_STATE_IN_KEY))
                {	
                   key_string = (char*)malloc(1);
                   key_string[0] = 0;
                   //key_string_len=0;
                }
            }
            else
            {
            	state &= ~JSON_STATE_IN_STRING;
            }
            offset++;
            continue;
            break;

    }
    //if((state&JSON_STATE_IN_VALUE))
    if((state&JSON_STATE_IN_VALUE))
    {
      value_string = json_AddCharToString(value_string,json[offset]);
      //value_string = (char*)realloc(value_string,value_string_len+2);
      //value_string[value_string_len+1] = 0;
      //value_string[value_string_len] = json[offset];
      //value_string_len++;
      offset++;
      continue;
    }	


    printf("%c",json[offset]);

    //else
    //  printf("[%c]",json[offset]);
    offset++;
  } 
 return(root_obj);

}

node *json_LoadFile(char *filename)
{ 
  node *rn = NULL;
  FILE *json = fopen(filename,"rb");
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


