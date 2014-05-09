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

#include "yetii.h"


char *AddCharToString(char *string,char letter)
{ 
  int len=strlen(string);
  string = (char*)realloc(string,len+2);
  string[len+1] = 0;
  string[len] = letter;
  return(string);
}

char *CreateEmptyString()
{
    char *string = (char*)malloc(1);
    string[0] = 0;
    return(string);
}

node *create_obj(char *name)
{
  node *obj = node_Create();
  node_SetType(obj,NODE_TYPE_NODE);
  node_SetKey(obj,name);
  return(obj);
}

void add_obj_kv(node *obj,node *kv)
{
  node_AddItem(obj,kv);
  node_SetParent(kv,obj);
}




void add_obj_string(node *obj,char *key,char *string)
{
  node *kv = node_Create();
  node_SetKey(kv,key);
  node_SetString(kv,string);
  add_obj_kv(obj,kv); 
}

void set_obj_string(node *obj,char *key,char *string)
{
  node *kv = node_GetItemByKey(obj,key);
  if(kv==NULL)
    add_obj_string(obj,key,string);
  else
    node_SetString(kv,string);
}

void set_obj_int(node *obj,char *key,long i)
{
  node *kv = node_GetItemByKey(obj,key);
  node_SetSint32(kv,i);
}

void set_obj_node(node *obj,char *key,node *n)
{
  node *kv = node_GetItemByKey(obj,key);
  node_SetType(kv,NODE_TYPE_NODE);
  //printf("setting node value:%x\n",n);
  kv->value = n;
}


char *get_obj_name(node *obj)
{
  node *kv = node_GetItemByKey(obj,"name");
  return(node_GetString(kv));
}

char *get_obj_type(node *obj)
{
  node *kv = node_GetItemByKey(obj,"type");
  return(node_GetString(kv));
}


void add_obj_int(node *obj,char *key,long i)
{
  node *kv = node_Create();
  node_SetKey(kv,key);
  node_SetSint32(kv,i);
  add_obj_kv(obj,kv); 
}

void add_obj_double(node *obj,char *key,double d)
{
  node *kv = node_Create();
  node_SetKey(kv,key);
  node_SetDouble(kv,d);
  add_obj_kv(obj,kv); 
}

node *get_value(node *obj)
{
  return(node_GetItemByKey(obj,"value"));
}

void add_member(node *obj,node *member)
{
  node *members = node_GetItemByKey(obj,"members");
  add_obj_kv(members,member);
}

node *get_member(node *obj,char *key)
{
  node *members = node_GetItemByKey(obj,"members");
  if(members==NULL)
    return(NULL);
  //node *member = node_GetItemByKey(members,key);
  //printf("searching for member:%s\n",key);
  node_ItemIterationReset(members);
  while(node_ItemIterationUnfinished(members))
  {
    node *member = node_ItemIterate(members);
    node *member_name = node_GetItemByKey(member,"name");
    if(!strcmp(node_GetString(member_name),key))
      return(member);
  }
  //printf("member not found:%s\nsearching base class\n",key);
  //node_PrintTree(obj);
  node *instance = (node*)node_GetItemByKey(obj,"base_class_instance");
  //printf("member instance address:%x\n",instance->value);
  node *base_class_instance = (node*)node_GetValue(node_GetItemByKey(obj,"base_class_instance"));
  if(base_class_instance!=NULL)
    return(get_member(base_class_instance,key));
  //printf("no base class instance defined -> returning NULL\n");
  return(NULL);
}

node *get_item(node *obj,node *key)
{
  node *items = node_GetItemByKey(obj,"items");
  node *key_value = node_GetItemByKey(key,"value");
  if(node_GetType(key_value) == NODE_TYPE_STRING)
  {
    //node *key_name_obj = node_GetItemByKey(key,"name");
    //char *key_name = node_GetString(key_name_obj);
    char *key_name = node_GetString(key_value);
    node_ItemIterationReset(items);
    while(node_ItemIterationUnfinished(items))
    {
      node *item = node_ItemIterate(items);
      node *item_name = node_GetItemByKey(item,"name");
      if(!strcmp(node_GetString(item_name),key_name))
        return(item);
    }
  }
  else if(node_GetType(key_value) == NODE_TYPE_SINT32)
  {
    node *item = node_GetItem(items,node_GetSint32(key_value));
    return(item);
  }
  return(NULL);
}



/*node *get_root_class(node *class_instance)
{


}*/


node *create_class_instance(node *class_obj)
{
  node *child = node_CopyTree(class_obj,True,True);
  //node_SetKey(child,class_name);
  node *base_class_type = (node*)node_GetValue(node_GetItemByKey(class_obj,"base_class_type"));
  if(base_class_type!=NULL)
  {
    node *base_class_instance = create_class_instance(base_class_type);
    set_obj_node(child,"base_class_instance",base_class_instance);
  }
  return(child);
}



node *create_base_obj_layout(char *obj_name)
{
  node *base = create_obj("yeti_object");
  add_obj_string(base,"type","class");
  add_obj_string(base,"name",obj_name);
  add_obj_kv(base,create_obj("base_class_instance"));
  add_obj_kv(base,create_obj("base_class_type"));
  add_obj_kv(base,create_obj("members"));
  add_obj_kv(base,create_obj("value"));
  return(base);
}

void add_class_object_internal_function(node *class,char *method_name)//void *addr
{
  node *base = create_base_obj_layout(method_name);
  set_obj_string(base,"type","function");
  set_obj_node(base,"base_class_instance",class);
  set_obj_node(base,"base_class_type",class);
  add_member(class,base);
}


node *create_class_object()
{
  node *base = create_base_obj_layout("object");
  //base classes 
  //member search has to include all base classes
  add_class_object_internal_function(base,"=");
  add_class_object_internal_function(base,":");
  //add_internal_function(base,"-");
  add_class_object_internal_function(base,"+");
  //add_internal_function(base,"/");
  //add_internal_function(base,"*");
  add_class_object_internal_function(base,"print");
  //add_class_object_internal_function(base,"get");
  //add_class_object_internal_function(base,"test");
  //add_internal_function(base,"=");
  return(base);
}





node *create_block_obj(node *base_class,node *block)
{
  node *base = create_class_instance(base_class);//,"block");
  set_obj_string(base,"name","block");
  set_obj_string(base,"type","yeti_il_block");
  node *il_block = node_CopyTree(block,True,True);
  add_obj_kv(base,il_block);  
  return(base);
}

node *create_execution_obj(node *method,node *parameters,node *sub_execution_obj)
{
  node *base = create_obj("exe_object");
  node_AddItem(base,method);
  node_AddItem(base,parameters);
  if(sub_execution_obj!=NULL)
  {
    node_AddItem(base,sub_execution_obj);
    node_SetKey(sub_execution_obj,"sub_exe_object");
  }
  return(base);
}


void walk_node(node *n,node *il)
{
    node *block = il;
    printf("node: %s\n",node_GetKey(n));
    char *key = node_GetKey(n);
    if(!strcmp(key,"val"))
    {
      switch(node_GetType(n))
      {
        case NODE_TYPE_SINT32:
          //printf("integer:%d\n",node_GetSint32(n));
          break;
        case NODE_TYPE_STRING:
          //printf("string:%s\n",node_GetString(n));
          break;
        case NODE_TYPE_FLOAT:
          //printf("float:%f\n",node_GetFloat(n));
          break;

      }
    }else
    if(!strcmp(key,"ops"))
    {
      //printf("ops: %s\n",node_GetString(n));
    }else
    if(!strcmp(key,"yeti_block"))
    {
      printf("new block\n");
      /*node *il_block = node_Create();
      node_SetType(il_block,NODE_TYPE_NODE);
      node_SetKey(il_block,"yeti_il_block");
      node_AddItem(il,il_block);
      node_SetParent(il_block,il);*/
      node *il_block = create_obj("yeti_il_block");
      add_obj_kv(il,il_block);
      node *il_block_is_function = create_obj("is_function");
      add_obj_kv(il_block,il_block_is_function);
      node_SetBool(il_block_is_function,False);

      node *parent = node_GetParent(n);
      if(parent!=NULL)
      {
        if(!strcmp(node_GetKey(parent),"yeti_statement"))
        {
          printf("parent:%s\n",node_GetKey(parent));
          node *id = node_GetItemByKey(parent,"val");
          node *parameters = node_GetItemByKey(parent,"yeti_parameters");
          node *il_block_has_parameters = create_obj("has_parameters");
          add_obj_kv(il_block,il_block_has_parameters);
          node_SetBool(il_block_is_function,True);

          //check for a valid statement
          //add parameter obj stubs in block_obj
          if(id != NULL)
          {
            printf("parent id:%s\n",node_GetString(id));
            node *il_block_name = create_obj("name");
            node_SetString(il_block_name,node_GetString(id));
            add_obj_kv(il_block,il_block_name);

            /*node *il_block_name = node_Create();
            node_SetKey(il_block_name,"name");
            node_SetString(il_block_name,node_GetString(id));
            node_AddItem(il_block,il_block_name);
            node_SetParent(il_block_name,il_block);*/
          }
          if(parameters != NULL)
          {
            node_SetBool(il_block_has_parameters,True);
            //node *parameters_copy = node_CopyTree(parameters,True);
            node *il_block_parameters = create_obj("yeti_il_parameters");
            add_obj_kv(il_block,il_block_parameters);
          }
          else
            node_SetBool(il_block_has_parameters,False);



       }

      }
      block = il_block;

    }else
    if(!strcmp(key,"yeti_statement"))
    {
      printf("new statement\n");
    }else
    if(!strcmp(key,"yeti_parameters"))
    {
      printf("new parameters\n");
    }else
    if(!strcmp(key,"yeti_array"))
    {
      printf("new array\n");
    }



    node_ItemIterationReset(n);
    while(node_ItemIterationUnfinished(n))
    {
        node *yeti_unit = node_ItemIterate(n);
        walk_node(yeti_unit,block);
        //long items_num = node_GetItemsNum(yeti_unit);

        //node_Print(yeti_unit,True);
    }

}

yeti_state *create_yeti_state(node *base_class)
{
  yeti_state *state = (yeti_state*)malloc(sizeof(yeti_state));
  state->il_stream = node_Create();
  node_SetType(state->il_stream,NODE_TYPE_NODE);
  node_SetKey(state->il_stream,"yeti_il_stream");
  state->statement_stack=list_Create(0,0);
  state->obj_stack=list_Create(0,0);
  state->base_class = base_class;
  return(state);
}

void execute_statement2(yeti_state *state,node *statement)
{
  printf("executing statement:\n");
  //node_Print(statement,True);
  node *parent_scope = node_GetParent(statement);
  long len = node_GetItemsNum(statement);
  long i = 0;
  for(i=len-1;i>=0;i--)
  {
    node *token = node_GetItem(statement,i);
    node *actual_obj = token;
    //node_Print(token,True,True);
    node_Print(token,True,True);
    if(!strcmp(node_GetKey(token),"val"))
    {
      if(node_GetType(token) == NODE_TYPE_STRING)
      {
        actual_obj = node_GetItemByKey(parent_scope,node_GetValue(token));
      }

    }
    else
      if(!strcmp(node_GetKey(token),"ops"))
      {
        printf("searching for obj method: %s\n",node_GetString(token));
      }
  }
}

node *execute_obj(yeti_state *state,node *execution_obj,node *block,BOOL dont_execute_block)
{
  //printf("executing execution obj\n");
  //deep first recursive
  BOOL tmp_dont_execute_block = False;
  node *value = NULL;
  node *parameters = node_GetItemByKey(execution_obj,"parameters");
  node *real_parameters = create_obj("parameters");

  node *exe_obj = node_GetItem(execution_obj,0);
  //printf("checking exe_obj:\n");
  //node_PrintTree(exe_obj);
  //printf("unevaluated parameters:\n");
  //node_PrintTree(parameters);
  
  if(strcmp(node_GetKey(exe_obj),"array") && !strcmp(get_obj_type(exe_obj),"function"))
  {
    //char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
    node *parent = node_GetParent(node_GetParent(exe_obj));
    //node *parent = node_GetParent(exe_obj);
    char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
    printf("checking for = function:%s\n",name);
    //node_PrintTree(parent);
    if(!strcmp(name,"="))
    {
      node *empty_shell = node_GetItemByKey(parent,"empty_function_shell");   
      if(empty_shell!=NULL)
        //printf("found empty_shell\n");
        tmp_dont_execute_block = True;
    }
  } 
  

  node_ItemIterationReset(parameters);
  while(node_ItemIterationUnfinished(parameters))
  {
    node *token = node_ItemIterate(parameters);
    if(!strcmp(node_GetKey(token),"exe_object"))
    {
      //printf("found sub exe obj\n");
      node *sub = execute_obj(state,token,block,tmp_dont_execute_block);
      node_AddItem(real_parameters,sub);
    }
  }
  tmp_dont_execute_block = False;

  //node_PrintTree(execution_obj);

  if(!strcmp(node_GetKey(exe_obj),"array"))
  {
    value = create_class_instance(state->base_class);
    //node *real_value = create_class_instance(state->base_class);
    //set_obj_node(value,"value",real_value);
    node *items = create_obj("items");
    add_obj_kv(value,items);
    node_ItemIterationReset(exe_obj);
    while(node_ItemIterationUnfinished(exe_obj))
    {
      node *token = node_ItemIterate(exe_obj);
      if(!strcmp(node_GetKey(token),"exe_object"))
      {
        printf("found array sub exe obj\n");
        node *sub = execute_obj(state,token,block,False);
        //node *psub = node_GetParent(sub);
        //add_member(value,psub);
        node_AddItem(items,sub);
        //node_AddItem(real_parameters,sub);
      }
    }
    //set_obj_string(value,"name","array");
    //add_member(actual_obj,child);
    
  }
  /*else if(!strcmp(node_GetKey(exe_obj),"yeti_block"))
  {
    //printf("executing new block\n");
    //evaluate_block(state,exe_obj);
    value = exe_obj;

  }*/
  else if(!strcmp(get_obj_type(exe_obj),"yeti_il_block"))
  {
    node *exe_block = node_GetItemByKey(exe_obj,"execute_block");
    //node *anon_block = node_GetItemByKey(exe_obj,"anonymous_block");

    //if((exe_block!= NULL && !strcmp(node_GetString(exe_block),"True")) || (anon_block!= NULL && !strcmp(node_GetString(anon_block),"True")) )
    printf("evaluating new block dont:%d\n",dont_execute_block);
    if(dont_execute_block==True)
    {
      //int x = 1/0;
    }
    if( exe_block!= NULL && !strcmp(node_GetString(exe_block),"True") && dont_execute_block==False )
    {
      printf("evaluating new block\n");
      node_PrintTree(exe_obj);
      node *il_block = node_GetItemByKey(exe_obj,"yeti_block");
      node *block_parameters = node_GetItemByKey(exe_obj,"yeti_parameters");
      if(block_parameters!=NULL)
      {
        //node *block_parameters = node_GetItemByKey(exe_obj,"parameters_definition");
        printf("evaluating block parameters\n");
        node_PrintTree(block_parameters);
        printf("using these given parameters\n");
        node_PrintTree(real_parameters);
        //execute_obj(state,block_parameters,block);
        //execute_obj(state,block_parameters,exe_obj);
        node_ItemIterationReset(block_parameters);
        long p_index = 0;
        while(node_ItemIterationUnfinished(block_parameters))
        {
          node *token = node_ItemIterate(block_parameters);
          //if(!strcmp(node_GetKey(token),"exe_object"))
          if(!strcmp(node_GetKey(token),"yeti_statement"))
          {
            node *obj = evaluate_statement(state,token,exe_obj,0);
            node *sub = execute_obj(state,obj,exe_obj,False);
            node *obj_name = node_GetItemByKey(sub,"name");
            printf("assigning parameter:%s\n",node_GetString(obj_name));
            node *real_parent = node_GetParent(sub);
            node_RemoveItem(real_parent,sub);
            //node_op(value,)
            //node *org_value2 = node_GetParent(node_GetItem(real_parameters,0));
            value = node_CopyTree(node_GetItem(real_parameters,p_index),True,True);
            //node *value2 = node_CopyTree(org_value2,True,True);
            node_PrintTree(value);
            set_obj_string(value,"name",node_GetString(obj_name));
            node_AddItem(real_parent,value);
            node_SetParent(value,real_parent);
            node *obj_parameters = node_GetItemByKey(sub,"yeti_parameters");
            //node *obj_parameters = node_GetItemByKey(parent,"parameters_definition");
            if(obj_parameters!=NULL)
            {
              add_obj_kv(value,obj_parameters);
            }
            //printf("found sub exe obj\n");
            //node *sub = execute_obj(state,token,block);
            //node_AddItem(real_parameters,sub);
          }
          p_index++;
        }
      }
      else
        printf("block has no parameters\n");
      node_PrintTree(exe_obj);
      //evaluate_block(state,il_block);
      set_obj_string(exe_obj,"execute_block","False");
      evaluate_block_instance(state,exe_obj);
      value = exe_obj;
    }
    else
    {
      printf("getting new block\n");
      /*node *block_parameters = node_GetItemByKey(exe_obj,"yeti_parameters");
      if(block_parameters==NULL)
      {
        printf("block has no parameters\n");
        printf("block name:%s\n",get_obj_name(exe_obj));
        set_obj_string(exe_obj,"execute_block","False");
        evaluate_block_instance(state,exe_obj);
      }
      */
      //add_obj_string(exe_obj,"execute_block","True");//TODO check if assigning this block var executes it!!!
      value = exe_obj;
    }
  }
  else if(!strcmp(get_obj_type(exe_obj),"function"))
  {
    char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
    node *parent = node_GetParent(node_GetParent(exe_obj));
    printf("calling function:%s\n",name);
    //node_PrintTree(parent);
    node_PrintTree(real_parameters);
    if(!strcmp(name,"+"))
    {
      //node *value = node_CopyTree(node_GetItemByKey(parent,"value"),True,True);
      printf("adding\n");
      //value = node_GetItemByKey(parent,"value");
      //value = node_CopyTree(node_GetItemByKey(parent,"value"),True,True);
      value = node_CopyTree(parent,True,True);
      //node_op(value,)
      node *value2 = node_GetItem(real_parameters,0);
      node *real_value = node_GetItemByKey(value,"value");
      node *real_value2 = node_GetItemByKey(value2,"value");
      node_SetSint32(real_value,node_GetSint32(real_value)+node_GetSint32(real_value2));
      //return(value);
    }
    else if(!strcmp(name,":"))
    {
      node *obj_name = node_GetItemByKey(parent,"name");
      printf("assigning value:%s\n",node_GetString(obj_name));
      node_PrintTree(real_parameters);
      node_PrintTree(parent);
      value = node_GetItemByKey(parent,"value");
      node *value2_parent = node_GetItem(real_parameters,0);
      node *value2 = node_GetItemByKey(value2_parent,"value");
      node_SetType(value,node_GetType(value2));
      node_SetValue(value,node_GetValue(value2),True,True);
      value = parent;
    }
    else if(!strcmp(name,"="))
    {
      //value = node_GetItemByKey(parent,"value");
      node *obj_name = node_GetItemByKey(parent,"name");
      printf("assigning:%s\n",node_GetString(obj_name));
      node *real_parent = node_GetParent(parent);
      node_PrintTree(real_parent);
      node_RemoveItem(real_parent,parent);
      //node_op(value,)
      //node *org_value2 = node_GetParent(node_GetItem(real_parameters,0));
      value = node_CopyTree(node_GetItem(real_parameters,0),True,True);
      //node *value2 = node_CopyTree(org_value2,True,True);
      set_obj_string(value,"name",node_GetString(obj_name));
      node_AddItem(real_parent,value);
      node_SetParent(value,real_parent);

      node *obj_parameters = node_GetItemByKey(parent,"yeti_parameters");
      //node *obj_parameters = node_GetItemByKey(parent,"parameters_definition");
      if(obj_parameters!=NULL)
      {
        add_obj_kv(value,obj_parameters);

      }
      set_obj_string(value,"execute_block","False");

      node_PrintTree(value);

      //node_SetItemByKey
      //node_SetType(value,node_GetType(value2));
      //node_SetValue(value,node_GetValue(value2),True,True);
      //return(value);

      //not only setting value but the whole class should be replaced/copied here
      //except the name which should be kept

    }

    else if(!strcmp(name,"print"))
    {
      //node *value2 = node_GetItem(real_parameters,0);
      //printf("print();\n");
      node_ItemIterationReset(real_parameters);
      while(node_ItemIterationUnfinished(real_parameters))
      {
        node *token = node_ItemIterate(real_parameters);
        node *items = node_GetItemByKey(token,"items");
        if(items!=NULL)
        {
          node_ItemIterationReset(items);
          while(node_ItemIterationUnfinished(items))
          {
            node *item = node_ItemIterate(items);
            node_Print(node_GetItemByKey(item,"value"),False,False);
          } 
        }
        else
          node_Print(node_GetItemByKey(token,"value"),False,False);
      }      
      printf("\n");
      //node_PrintTree(value2);
      //return(value2);
    }

    else if(!strcmp(name,"test"))
    {
      //node *value2 = node_GetItem(real_parameters,0);
      printf("test(a);\n");
      value = node_GetItemByKey(parent,"value");
      node *value_a = node_GetItem(real_parameters,0);
      //node *value = node_CopyTree(node_GetItemByKey(parent,"value"),True,True);
      node_SetSint32(value,20+node_GetSint32(value_a));
      node_PrintTree(value);
      //return(value);
      value = parent;
      //node_ItemIterationReset(real_parameters);
      //while(node_ItemIterationUnfinished(real_parameters))
      //{
      //  node *token = node_ItemIterate(real_parameters);
        //node_Print(token,False,False);
      //}      
      //printf("\n");
      //node_PrintTree(value2);
      //return(value2);
    }

  }
  else if(!strcmp(node_GetKey(exe_obj),"yeti_object"))
  {
    char *name = node_GetString(node_GetItemByKey(exe_obj,"name"));
    //value = node_GetItemByKey(exe_obj,"value");
    value = exe_obj;
    printf("getting value of:%s\n",name);
    node_PrintTree(value);
    //return(value);
  }

  node *sub_exe_obj = node_GetItemByKey(execution_obj,"sub_exe_object");
  if(sub_exe_obj!=NULL)
  {
    printf("has sub exe object\n");
    value = execute_obj(state,sub_exe_obj,block,False);
    //return(sub);
  }
  //return(exe_obj);
  return(value);
}


//node *execute_statement(yeti_state *state,node *statement,node *block,node *parameters,long iteration_start_index)
node *evaluate_statement(yeti_state *state,node *statement,node *block,long iteration_start_index)
{
  printf("evaluating statement:\n");
  //printf("eos\n");

  //node_ItemIterationReset(statement);
  long old_iteration_index = node_GetItemIterationIndex(statement);
  node_SetItemIterationIndex(statement,iteration_start_index);
  long index = iteration_start_index;
  node *parameters = create_obj("parameters");
  node *actual_obj = block;
  node *sub_exe_obj = NULL;
  node *prev_token = NULL;
  while(node_ItemIterationUnfinished(statement))
  {
    node *token = node_ItemIterate(statement);
    node_PrintTree(token);
    if(!strcmp(node_GetKey(token),"yeti_parameters"))
    {
      printf("got some parameters\n");
      node_ItemIterationReset(token);
      while(node_ItemIterationUnfinished(token))
      {
        node *parameter_token = node_ItemIterate(token);
        node *sub_obj = evaluate_statement(state,parameter_token,block,0);
        node_AddItem(parameters,sub_obj);
      }
      if(index+1<node_GetItemsNum(statement))
      {
        node *sub_obj = evaluate_statement(state,statement,block,index+1);
        //node_AddItem(parameters,sub_obj);
        sub_exe_obj = sub_obj;
        //actual_obj = token;
      }
      node_PrintTree(parameters);
    }
    else if(prev_token!=NULL && !strcmp(node_GetKey(prev_token),"val") && !strcmp(node_GetKey(token),"yeti_array"))
    {
      //printf("found an array key\n");
      //node_PrintTree(actual_obj);
      //printf("getting object using key:\n");
      //node_PrintTree(token);
      node *key_exe_obj = evaluate_statement(state,node_GetItem(token,0),block,0);
      node *key_obj = execute_obj(state,key_exe_obj,block,False);/*TODO move to execute somehow*/
      //node_PrintTree(key_obj);
      //printf("searching now\n");
      node *found_obj = get_item(actual_obj,key_obj);
      if(found_obj==NULL)
      {
        printf("array entry not found\n");
      }
      else
      {
        //printf("found id:%s\n",node_GetValue(token));
        actual_obj = found_obj;
        node_PrintTree(actual_obj);
      }
    }
    else if(!strcmp(node_GetKey(token),"yeti_array"))
    {
      //printf("found an array\n");
      //node *items = node_GetItemByKey(actual_obj,"items");
      //if(items!=NULL)
      //{
      //  printf("actual obj has items\n");
      //}
      //node *array = create_class_instance(state->base_class);
      //node *array_members = node_GetItemByKey(array,"members");
      node *array = create_obj("array");
      node_ItemIterationReset(token);
      while(node_ItemIterationUnfinished(token))
      {
        node *parameter_token = node_ItemIterate(token);
        node *sub_obj = evaluate_statement(state,parameter_token,block,0);
        node_AddItem(array,sub_obj);
      }
      actual_obj=array;
      if(index+1<node_GetItemsNum(statement))
      {
        node *sub_obj = evaluate_statement(state,statement,block,index+1);
        //node_AddItem(parameters,sub_obj);
        sub_exe_obj = sub_obj;
        //actual_obj = token;
      }
      //node_PrintTree(array);
    }
    else if(!strcmp(node_GetKey(token),"yeti_block"))
    {
      printf("found yeti_block\n");
      //node_PrintTree(token);
      node *block_class_instance = create_block_obj(state->base_class,token);
      //node *il_block = node_GetItemByKey(block_class_instance,"yeti_block");
      //add_obj_string(block_class_instance,"anonymous","True");
      //add_obj_string(block_class_instance,"anonymous_block","True");
      add_obj_string(block_class_instance,"execute_block","True");
      actual_obj = block_class_instance;
    }
    else if(!strcmp(node_GetKey(token),"str"))
    {
      node *child = create_class_instance(state->base_class);
      set_obj_string(child,"value",node_GetValue(token));
      actual_obj = child;
    }
    else if(!strcmp(node_GetKey(token),"val"))
    {
      if(node_GetType(token) == NODE_TYPE_STRING)
      {
        if(!strcmp(node_GetValue(token),"value"))
        {
          //actual_obj = node_GetParent(actual_obj);
          printf("setting actual obj to:\n");
          node *old_actual_obj = actual_obj;
          actual_obj = node_GetItemByKey(actual_obj,"value");
          if(node_GetValue(actual_obj)==NULL)
          {
            printf("value undefined\n");
            node *child = create_class_instance(state->base_class);
            set_obj_node(old_actual_obj,"value",child);
            node_SetParent(child,old_actual_obj);
            actual_obj = child;
          }
          node_PrintTree(actual_obj);
        }
        else
        if(!strcmp(node_GetValue(token),"@"))
        {
          //actual_obj = node_GetParent(actual_obj);
          printf("leaving actual obj to be:\n");
          node_PrintTree(actual_obj);
        }
        else
        {
          node *found_obj = get_member(actual_obj,node_GetValue(token));
          if(found_obj==NULL)
          {
            printf("created class with id:%s\n",node_GetValue(token));
            node *child = create_class_instance(state->base_class);
            set_obj_string(child,"name",node_GetValue(token));
            add_member(actual_obj,child);
            actual_obj = child;
            node *peek = node_ItemPeek(statement);
            if(peek!=NULL && !strcmp(node_GetKey(peek),"yeti_parameters"))
            {
              //printf("found parameter definition\n");
              node *parameters_definition = node_ItemIterate(statement);
              //set_obj_string(child,"name",node_GetValue(token));
              //node_SetKey(parameters_definition,"yeti_il");
              /*node *func_parameters = create_obj("parameters_definition");
              node_ItemIterationReset(parameters_definition);
              while(node_ItemIterationUnfinished(parameters_definition))
              {
                node *parameter_token = node_ItemIterate(parameters_definition);
                //node *sub_obj = evaluate_statement(state,parameter_token,block,0);
                node *sub_obj = evaluate_statement(state,parameter_token,child,0);
                node_AddItem(func_parameters,sub_obj);
              }
              node_AddItem(child,func_parameters);
              */
              node_AddItem(child,parameters_definition);
              add_obj_string(child,"empty_function_shell","True");
              set_obj_string(child,"execute_block","False");

              //node_PrintTree(child);
              //*add statement to object as parameter_definition
              //per call execute statements included
              //add to members
              //use names to identify objects to be replaced by parameters given with the call
              index++;
            }
          }
          else
          {
            printf("found id:%s\n",node_GetValue(token));
            actual_obj = found_obj;
            if(!strcmp(get_obj_type(actual_obj),"yeti_il_block"))
            {
              node *peek = node_ItemPeek(statement);
              if(peek!=NULL && !strcmp(node_GetKey(peek),"yeti_parameters"))
              {
                set_obj_string(actual_obj,"execute_block","True");
              }


            }
            node_PrintTree(actual_obj);
          }
        }
      }
      else if(node_GetType(token) == NODE_TYPE_SINT32)
      {
        node *child = create_class_instance(state->base_class);
        set_obj_int(child,"value",node_GetSint32(token));
        actual_obj = child;//get_member(child,"get");
      }
    }
    else if(!strcmp(node_GetKey(token),"ops"))
    {
      if(!strcmp(node_GetString(token),"."))
      {
        //printf("sub id following\n");
        //
      }
      else
      {
        node *found_obj = get_member(actual_obj,node_GetValue(token));
        if(found_obj==NULL)
        { 
          //printf("method not found\n");
        }
        else
        {
          if(index+1<node_GetItemsNum(statement))
          {
            node *sub_obj = evaluate_statement(state,statement,block,index+1);
            node_AddItem(parameters,sub_obj);
            actual_obj = found_obj;
          }
        }
      }
    }
    prev_token = token;
    index++;    
  }
  node *exe_obj = create_execution_obj(actual_obj,parameters,sub_exe_obj);
  return(exe_obj);
}


void evaluate_block(yeti_state *state,node *block)
{
    node *block_class_instance = create_block_obj(state->base_class,block);
    node *il_block = node_GetItemByKey(block_class_instance,"yeti_block");
    node_ItemIterationReset(il_block);
    while(node_ItemIterationUnfinished(il_block))
    {
        node *yeti_statement = node_ItemIterate(il_block);
        //node *parameters = create_obj("parameters");
        //node *obj = execute_statement(state,yeti_statement,block_class_instance,parameters,0);
        node *obj = evaluate_statement(state,yeti_statement,block_class_instance,0);
        //node *exe_obj = create_execution_obj(obj,parameters);
        //node_Print(exe_obj,True);
        //node_Print(obj,True,False);
        node_PrintTree(obj);
        execute_obj(state,obj,block_class_instance,False);
    }
    //node_PrintTree(block_class_instance);
}


void evaluate_block_instance(yeti_state *state,node *block_class_instance)
{
    node *il_block = node_GetItemByKey(block_class_instance,"yeti_block");
    node_ItemIterationReset(il_block);
    while(node_ItemIterationUnfinished(il_block))
    {
        node *yeti_statement = node_ItemIterate(il_block);
        //node *parameters = create_obj("parameters");
        //node *obj = execute_statement(state,yeti_statement,block_class_instance,parameters,0);
        node *obj = evaluate_statement(state,yeti_statement,block_class_instance,0);
        //node *exe_obj = create_execution_obj(obj,parameters);
        //node_Print(exe_obj,True);
        //node_Print(obj,True,False);
        //node_PrintTree(obj);
        execute_obj(state,obj,block_class_instance,False);
    }
    //node_PrintTree(block_class_instance);
}




int main(int argc, char** argv)
{
  node *base_class = create_class_object();
  yeti_state *state = create_yeti_state(base_class);

  printf("yeti interpreter 0.1\n");
  //printf("base class address:%x\n",base_class);

  //node *yeti_stream = yeti_LoadFile("node.c/tests/test_print.yeti");
  //node *yeti = yeti_LoadFile("node.c/tests/test_block2.yeti");
  //node *yeti_stream = yeti_LoadFile("node.c/tests/test_nesting.yeti");
  //node *yeti_stream = yeti_LoadFile("node.c/tests/test_simple4.yeti");
  
  //node *yeti_stream = yeti_LoadFile("node.c/tests/test_simple_nest.yeti");
  //node *yeti_stream = yeti_LoadFile("node.c/tests/test_simple_nest3.yeti");
  if(argc<2)
  {
    printf("error: no input file given as parameter\n");
    return(1);
  }
  node *yeti_stream = yeti_LoadFile(argv[1]);

  //node *yeti_stream = yeti_LoadFile("node.c/tests/test_simple9.yeti");
  
  //node *yeti_stream = node_GetItemByKey(yeti,"yeti_stream");
  //node_PrintTree(yeti);
  //printf("yeti key: %s\n",node_GetKey(yeti_stream));
  node_PrintTree(yeti_stream);
  
  state->top_scope = node_GetItemByKey(yeti_stream,"yeti_block");
  if(yeti_stream!=NULL)
  {
    //printf("found stream\n");
    //walk_node(yeti_stream,il_stream);
    //execute_statement(state,state->top_scope);
    evaluate_block(state,state->top_scope);
  }
  //node_PrintTree(il_stream);
  free(state);
}